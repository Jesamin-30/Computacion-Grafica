//g++ tp4.cpp TextureManager.cpp -lGL -lGLU -lglut -lfreeimage
#define GLUT_DISABLE_ATEXIT_HACK	
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>

#include <glm/glm.hpp>
#include "GL/glut.h"

#include "TextureManager.h"

using namespace std;

#define RED 0
#define GREEN 0
#define BLUE 0
#define ALPHA 1

#define ECHAP 27
void init_scene();

GLvoid initGL();
GLvoid window_display();
GLvoid window_reshape(GLsizei width, GLsizei height);
GLvoid window_key(unsigned char key, int x, int y);


//variables para el gizmo
float delta_x = 0.0; 
float delta_y = 0.0;
float mouse_x, mouse_y;
float var_x = 0.0;
float var_z = -30.0;
float step = 0; //0.0 Posicion inicial. 1.0 Traslacion. 2.0 Primera Rotacion(en y). 3.0 Segunda Rotacion (en x) 4.0 Ultima Rotacion (en z)

glm::vec3 centroPlano(0,0,0);
int tamano=10;

glm::vec3 centroCasa(2,0,2);
int ancho=1;
int largo=5;
int alto=5;

//TEXTURAS
GLint TexturaPasto;
GLint TexturaPared;
GLint TexturaHojas;
GLint TexturaTronco;
GLint TexturaTecho;
GLint TexturaPuerta;
GLint TexturaVentana;

//MATERIALES
GLfloat pastoAmbient[3] = {0.022f, 0.175f, 0.022f};
GLfloat pastoDiffuse[3] = {0.076f, 0.614f, 0.076f};
GLfloat pastoSpecular[3] = {0.633f, 0.728f, 0.633f};
GLfloat pastoShininess[] = {75.0f};

GLfloat paredAmbient[3] = {0.2125f, 0.1275f, 0.054f};
GLfloat paredDiffuse[3] = {0.714f, 0.4284f, 0.18144f};
GLfloat paredSpecular[3] = {0.393548f, 0.271906f, 0.166721f};
GLfloat paredShininess[] = {25.6f};

GLfloat techoAmbient[3] = {0.250f, 0.250f, 0.250f};
GLfloat techoDiffuse[3] = {0.4f, 0.4f, 0.4f};
GLfloat techoSpecular[3] = {0.775f, 0.775f, 0.775f};
GLfloat techoShininess[] = {75.0f};

GLfloat hojasAmbient[3] = {0.02f, 0.175f, 0.2f};
GLfloat hojasDiffuse[3] = {0.076f, 0.614f, 0.076f};
GLfloat hojasSpecular[3] = {0.63f, 0.72f, 0.633f};
GLfloat hojasShininess[] = {75.0f};

GLfloat troncoAmbient[3] = {0.213f, 0.127f, 0.054f};
GLfloat troncoDiffuse[3] = {0.714f, 0.428f, 0.181f};
GLfloat troncoSpecular[3] = {0.394f, 0.272f, 0.167f};
GLfloat troncoShininess[] = {25.0f};

///////////////////////////////////////////////////////////////////////////////
//(1)
///////////////////////////////////////////////////////////////////////////////
GLvoid callback_special(int key, int x, int y)
{
	switch (key)
	{
	case GLUT_KEY_UP:
		var_z += 0.5;
		glutPostRedisplay();
		break;

	case GLUT_KEY_DOWN:
		var_z -= 0.5;
		glutPostRedisplay();
		break;

	case GLUT_KEY_LEFT:
		var_x += 0.5;
		glutPostRedisplay();
		break;

	case GLUT_KEY_RIGHT:
		var_x -= 0.5;
		glutPostRedisplay();
		break;

	case GLUT_KEY_PAGE_UP:
		step++;
		glutPostRedisplay();
		break;
	case GLUT_KEY_PAGE_DOWN:
		step--;
		glutPostRedisplay();
		break;
	}
}

///////////////////////////////////////////////////////////////////////////////
//(2)
///////////////////////////////////////////////////////////////////////////////
GLvoid callback_mouse(int button, int state, int x, int y)
{
	if (state == GLUT_DOWN && button == GLUT_LEFT_BUTTON)
	{
		mouse_x = x;
		mouse_y = y;
	}
}

///////////////////////////////////////////////////////////////////////////////
//(3)
///////////////////////////////////////////////////////////////////////////////
GLvoid callback_motion(int x, int y)
{
	
	delta_x += x - mouse_x;
	delta_y += y - mouse_y;
	mouse_x = x;
	mouse_y = y;
	glutPostRedisplay();
}

GLvoid initGL(){
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);
	glClearColor(RED, GREEN, BLUE, ALPHA);
	
	glShadeModel(GL_SMOOTH); // modelo de shading try GL_FLAT
	glEnable(GL_CULL_FACE); //no trata las caras escondidas
	glEnable(GL_DEPTH_TEST); // Activa el Z-Buffer
	glDepthFunc(GL_LEQUAL); //Modo de funcionamiento del Z-Buffer
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST); // Activa la corrección de perspectiva

	/*GLfloat MatAmbient[] = {0.3f, 0.3f, 0.3f, 1.0f};
	GLfloat MatDiffuse[] = {0.7f, 0.7f, 0.7f, 1.0f};
	GLfloat MatSpecular[] = {1.0f,1.0f, 1.0f, 1.0f};
	GLfloat MatShininess[] = {50.0f};*/

	GLfloat LightAmbient[] = {0.0f, 0.0f, 0.0f , 1.0f}; 
	GLfloat LightDiffuse[] = {1.0f, 1.0f, 1.0f , 1.0f}; 
	GLfloat LightSpecular[] = {1.0f, 1.0f, 1.0f , 1.0f};
	GLfloat LightPosition[] = {0.0f, 10.0f, 0.0f, 0.0f};

	glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, LightAmbient);

	glLightfv(GL_LIGHT0, GL_AMBIENT, LightAmbient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, LightDiffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, LightSpecular);
	glLightfv(GL_LIGHT0, GL_POSITION, LightPosition);
}


void pasto(){
	//glColor3f(0,1,0);
	/*glBegin(GL_QUADS);
		glVertex3f(centroPlano.x-tamano,centroPlano.y,centroPlano.z+tamano);
		glVertex3f(centroPlano.x+tamano,centroPlano.y,centroPlano.z+tamano);
		glVertex3f(centroPlano.x+tamano,centroPlano.y,centroPlano.z-tamano);
		glVertex3f(centroPlano.x-tamano,centroPlano.y,centroPlano.z-tamano);
	glEnd();*/
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT , pastoAmbient);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE , pastoDiffuse);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, pastoSpecular);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, pastoShininess);

	glPushMatrix();
	glBindTexture(GL_TEXTURE_2D,TexturaPasto);
	glNormal3f( 0.0f , 1.0f ,0.0f );
	glBegin(GL_QUADS);
		glTexCoord2f(0.0f,0.0f);
		glVertex3f(centroPlano.x-tamano,centroPlano.y,centroPlano.z+tamano);
		glTexCoord2f(1.0f,0.0f);
		glVertex3f(centroPlano.x+tamano,centroPlano.y,centroPlano.z+tamano);
		glTexCoord2f(1.0f,1.0f);
		glVertex3f(centroPlano.x+tamano,centroPlano.y,centroPlano.z-tamano);
		glTexCoord2f(0.0f,1.0f);
		glVertex3f(centroPlano.x-tamano,centroPlano.y,centroPlano.z-tamano);
	glEnd();	
	glPopMatrix();
}

void puerta(){
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT , techoAmbient);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE , techoDiffuse);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, techoSpecular);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, techoShininess);

	glBindTexture(GL_TEXTURE_2D,TexturaPuerta);
	//pared con puerta
	glNormal3f( 0.0f , 1.0f ,0.0f );
	glBegin(GL_QUADS);
		glTexCoord2f(0.0f,1.0f);
		glVertex3f(centroCasa.x+ancho-0.1,centroCasa.y+alto/1.2,centroCasa.z-largo/2);//1
		glTexCoord2f(0.0f,0.0f);
		glVertex3f(centroCasa.x+ancho-0.1,centroCasa.y,centroCasa.z-largo/2);//2
		glTexCoord2f(1.0f,0.0f);
		glVertex3f(centroCasa.x+ancho-0.1,centroCasa.y,centroCasa.z+largo/2);//3
		glTexCoord2f(1.0f,1.0f);
		glVertex3f(centroCasa.x+ancho-0.1,centroCasa.y+alto/1.2,centroCasa.z+largo/2);//4
	glEnd();
}

void ventana(){
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT , techoAmbient);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE , techoDiffuse);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, techoSpecular);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, techoShininess);

	glBindTexture(GL_TEXTURE_2D,TexturaVentana);
	//pared con puerta
	glNormal3f( 0.0f , 1.0f ,0.0f );
	glBegin(GL_QUADS);
		glTexCoord2f(0.0f,1.0f);
		glVertex3f(centroCasa.x+ancho+1,centroCasa.y+alto-0.5,centroCasa.z+largo+0.1);
		glTexCoord2f(0.0f,0.0f);
		glVertex3f(centroCasa.x+ancho+1,centroCasa.y+1,centroCasa.z+largo+0.1);
		glTexCoord2f(1.0f,0.0f);
		glVertex3f(3*(centroCasa.x+ancho)-1,centroCasa.y+1,centroCasa.z+largo+0.1);
		glTexCoord2f(1.0f,1.0f);
		glVertex3f(3*(centroCasa.x+ancho)-1,centroCasa.y+alto-0.5,centroCasa.z+largo+0.1);
	glEnd();
}

/* 1------4
   |	  |	
   2------3	
 */
void paredes(){
	//glColor3f(1,0,0);
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT , paredAmbient);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE , paredDiffuse);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, paredSpecular);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, paredShininess);

	glBindTexture(GL_TEXTURE_2D,TexturaPared);
	glPushMatrix();
	//pared al centro con puerta
	glNormal3f( 0.0f , 1.0f ,0.0f );
	glBegin(GL_QUADS);
		glTexCoord2f(0.0f,1.0f);
		glVertex3f(centroCasa.x+ancho,centroCasa.y+alto,centroCasa.z-largo);//1
		glTexCoord2f(0.0f,0.0f);
		glVertex3f(centroCasa.x+ancho,centroCasa.y,centroCasa.z-largo);//2
		glTexCoord2f(1.0f,0.0f);
		glVertex3f(centroCasa.x+ancho,centroCasa.y,centroCasa.z+largo);//3
		glTexCoord2f(1.0f,1.0f);
		glVertex3f(centroCasa.x+ancho,centroCasa.y+alto,centroCasa.z+largo);//4
	glEnd();
	
	//pared sin triangulo afuera
	glBegin(GL_QUADS);
		glTexCoord2f(0.0f,1.0f);
		glVertex3f(3*(centroCasa.x+ancho),centroCasa.y+alto,centroCasa.z+largo);//4
		glTexCoord2f(0.0f,0.0f);
		glVertex3f(3*(centroCasa.x+ancho),centroCasa.y,centroCasa.z+largo);//3
		glTexCoord2f(1.0f,0.0f);
		glVertex3f(3*(centroCasa.x+ancho),centroCasa.y,centroCasa.z-largo);//2
		glTexCoord2f(1.0f,1.0f);
		glVertex3f(3*(centroCasa.x+ancho),centroCasa.y+alto,centroCasa.z-largo);//1
	glEnd();

	//pared con triangulo izquierda
	glBegin(GL_QUADS);
		glTexCoord2f(0.0f,1.0f);
		glVertex3f(3*(centroCasa.x+ancho),centroCasa.y+alto,centroCasa.z-largo);
		glTexCoord2f(0.0f,0.0f);
		glVertex3f(3*(centroCasa.x+ancho),centroCasa.y,centroCasa.z-largo);
		glTexCoord2f(1.0f,0.0f);
		glVertex3f(centroCasa.x+ancho,centroCasa.y,centroCasa.z-largo);
		glTexCoord2f(1.0f,1.0f);
		glVertex3f(centroCasa.x+ancho,centroCasa.y+alto,centroCasa.z-largo);				
	glEnd();

	//pared con triangulo derecha
	glBegin(GL_QUADS);
		glTexCoord2f(0.0f,1.0f);
		glVertex3f(centroCasa.x+ancho,centroCasa.y+alto,centroCasa.z+largo);
		glTexCoord2f(0.0f,0.0f);
		glVertex3f(centroCasa.x+ancho,centroCasa.y,centroCasa.z+largo);
		glTexCoord2f(1.0f,0.0f);
		glVertex3f(3*(centroCasa.x+ancho),centroCasa.y,centroCasa.z+largo);
		glTexCoord2f(1.0f,1.0f);
		glVertex3f(3*(centroCasa.x+ancho),centroCasa.y+alto,centroCasa.z+largo);
	glEnd();	

	/*	3
	   / \
	  1---2
 	*/
 	//izquierda
	glBegin(GL_TRIANGLES);
		glTexCoord2f(0.0f,0.6f);
		glVertex3f(3*(centroCasa.x+ancho),centroCasa.y+alto,centroCasa.z-largo);//2
		glTexCoord2f(1.0f,0.6f);
		glVertex3f(centroCasa.x+ancho,centroCasa.y+alto,centroCasa.z-largo);//1
		glTexCoord2f(0.5f,1.0f);
		glVertex3f(3*(centroCasa.x+ancho)-(centroCasa.x+ancho),2+centroCasa.y+alto,centroCasa.z-largo);//3
	glEnd();

	//derecha
	glBegin(GL_TRIANGLES);
		glTexCoord2f(0.0f,0.6f);
		glVertex3f(centroCasa.x+ancho,centroCasa.y+alto,centroCasa.z+largo);
		glTexCoord2f(1.0f,0.6f);
		glVertex3f(3*(centroCasa.x+ancho),centroCasa.y+alto,centroCasa.z+largo);
		glTexCoord2f(0.5f,1.0f);
		glVertex3f(3*(centroCasa.x+ancho)-(centroCasa.x+ancho),2+centroCasa.y+alto,centroCasa.z+largo);
	glEnd();

	glPopMatrix();
}


void techo(){
	//techo
	//glColor3f(0.5f,0.5f,0.5f);
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT , techoAmbient);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE , techoDiffuse);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, techoSpecular);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, techoShininess);

	glBindTexture(GL_TEXTURE_2D,TexturaTecho);
	glNormal3f( 1.0f , 1.0f ,0.0f );
	glBegin(GL_QUADS);
		glTexCoord2f(0.0f,1.0f);
		glVertex3f(3*(centroCasa.x+ancho)-(centroCasa.x+ancho),2+centroCasa.y+alto,centroCasa.z-largo);//3
		glTexCoord2f(0.0f,0.0f);
		glVertex3f(centroCasa.x+ancho,centroCasa.y+alto,centroCasa.z-largo);//1
		glTexCoord2f(1.0f,0.0f);
		glVertex3f(centroCasa.x+ancho,centroCasa.y+alto,centroCasa.z+largo);//2
		glTexCoord2f(1.0f,1.0f);
		glVertex3f(3*(centroCasa.x+ancho)-(centroCasa.x+ancho),2+centroCasa.y+alto,centroCasa.z+largo);
	glEnd();	
	
	glBindTexture(GL_TEXTURE_2D,TexturaTecho);
	glNormal3f(1.0f , 1.0f ,0.0f );
	glBegin(GL_QUADS);
		glTexCoord2f(0.0f,1.0f);
		glVertex3f(3*(centroCasa.x+ancho)-(centroCasa.x+ancho),2+centroCasa.y+alto,centroCasa.z+largo);
		glTexCoord2f(0.0f,0.0f);
		glVertex3f(3*(centroCasa.x+ancho),centroCasa.y+alto,centroCasa.z+largo);//4
		glTexCoord2f(1.0f,0.0f);
		glVertex3f(3*(centroCasa.x+ancho),centroCasa.y+alto,centroCasa.z-largo);//1
		glTexCoord2f(1.0f,1.0f);
		glVertex3f(3*(centroCasa.x+ancho)-(centroCasa.x+ancho),2+centroCasa.y+alto,centroCasa.z-largo);//3
	glEnd();
}

void arbol(){

	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT , troncoAmbient);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE , troncoDiffuse);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, troncoSpecular);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, troncoShininess);	

	//tronco
	glPushMatrix();
		//glColor3f(0.5f,0.2f,0.1f);
		GLUquadricObj *tronco = gluNewQuadric();
		gluQuadricTexture(tronco,GLU_TRUE);
		glBindTexture(GL_TEXTURE_2D, TexturaTronco);
		glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
		glTranslatef(centroCasa.x-7,centroCasa.y+5,centroCasa.z-5);
		gluCylinder(tronco,0.8f,0.8f,3.0f,32,32);	
	glPopMatrix();

	//glColor3f(0.0f,0.5f,0.0f);
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT , hojasAmbient);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE , hojasDiffuse);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, hojasSpecular);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, hojasShininess);
	
	GLUquadricObj* hojas = 0;
	hojas = gluNewQuadric();
	gluQuadricTexture(hojas,GLU_TRUE);
	glBindTexture(GL_TEXTURE_2D, TexturaHojas);
	glNormal3f( -1.0f , 0.0f ,1.0f );
	glTranslatef(centroCasa.x-7,centroCasa.y+4,centroCasa.z+3);
	//glutSolidSphere( 2.0, 20.0, 20.0);
	gluSphere(hojas,2.0,20.0,20.0);
}

void casa(){
	pasto();
	paredes();
	puerta();
	ventana();
	techo();
	arbol();
}

void LoadTexture(){
	TexturaPasto = TextureManager::Inst()->LoadTexture("texturas/pasto.jpg",GL_BGR_EXT,GL_RGB);
	TexturaPared = TextureManager::Inst()->LoadTexture("texturas/pared.jpg",GL_BGR_EXT,GL_RGB);
	TexturaHojas = TextureManager::Inst()->LoadTexture("texturas/hojas.jpg",GL_BGR_EXT,GL_RGB);
	TexturaTronco = TextureManager::Inst()->LoadTexture("texturas/tronco.jpg",GL_BGR_EXT,GL_RGB);
	TexturaTecho = TextureManager::Inst()->LoadTexture("texturas/techo.jpg",GL_BGR_EXT,GL_RGB);
	TexturaPuerta = TextureManager::Inst()->LoadTexture("texturas/puerta.jpg",GL_BGR_EXT,GL_RGB);
	TexturaVentana = TextureManager::Inst()->LoadTexture("texturas/ventana.jpg",GL_BGR_EXT,GL_RGB);
}


GLvoid window_display()
{

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	gluPerspective(45.0f, 1.0f, 0.01f, 100.0f);

	glTranslatef(var_x, 0.0, var_z);
	glRotatef(delta_x, 0.0, 1.0, 0.0);
	glRotatef(delta_y, 1.0, 0.0, 0.0);


	casa();
	
	glutSwapBuffers();
	glFlush();
}

GLvoid window_reshape(GLsizei width, GLsizei height){
	glViewport(0, 0, width, height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glMatrixMode(GL_MODELVIEW);
}



void init_scene(){
}

GLvoid window_key(unsigned char key, int x, int y)
{
	switch (key) {
	case ECHAP:
		exit(1);
		break;
	default:
		printf("La touche %d non active.\n", key);
		break;
	}
}


//function called on each frame
GLvoid window_idle(){
	glutPostRedisplay();
}

int main(int argc, char **argv)
{
	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);


	glutInitWindowSize(800, 800);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("Practica 4");

	LoadTexture();

	initGL();
	init_scene();

	
	glutDisplayFunc(&window_display);
	glutReshapeFunc(&window_reshape);
	
	
	glutMouseFunc(&callback_mouse);
	glutMotionFunc(&callback_motion);

	glutKeyboardFunc(&window_key);
	//glutKeyboardUpFunc(&window_key_up); //key release events
	glutSpecialFunc(&callback_special);
	//glutSpecialUpFunc(&callback_special_up); //key release events
	
	//function called on each frame
	glutIdleFunc(&window_idle);

	glutMainLoop();

	return 1;
}









