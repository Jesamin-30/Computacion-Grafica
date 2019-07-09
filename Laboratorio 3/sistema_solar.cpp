#define GLUT_DISABLE_ATEXIT_HACK

#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>

#include "GL/glut.h"
using namespace std;

#define RED 0
#define GREEN 0
#define BLUE 0
#define ALPHA 1

#define ECHAP 27
void init_scene();
void render_scene();
GLvoid initGL();
GLvoid window_display();
GLvoid window_reshape(GLsizei width, GLsizei height);
GLvoid window_key(unsigned char key, int x, int y);

//function called on each frame
GLvoid window_idle();
GLvoid initGL()
{
	GLfloat position[] = { 0.0f, 5.0f, 10.0f, 0.0 };

	//enable light : try without it
	glLightfv(GL_LIGHT0, GL_POSITION, position);
	glEnable(GL_LIGHTING);
	//light 0 "on": try without it
	glEnable(GL_LIGHT0);

	//shading model : try GL_FLAT
	glShadeModel(GL_SMOOTH);

	glEnable(GL_DEPTH_TEST);

	//enable material : try without it
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
	glEnable(GL_COLOR_MATERIAL);

	glClearColor(RED, GREEN, BLUE, ALPHA);
}

float angulo_rotacional_sol=0;
float angulo_rotacional_marte=0;//alrededor de si mismo
float angulo_rotacional_luna=0;
float angulo_rotacional_tierra=0;

float angulo_t_marte=0;//alrededor del sol
float angulo_t_luna=0;
float angulo_t_tierra=0;
float angulo_t_sol=0;

float VS = 1;
float VM = VS;
float Vt=0.02;
float Vl=2*Vt;
float VT=3*VS; 
float VL=1.5*VS;
float Vm=Vt;
float X,Y,Z=0;
float Vs=Vm;


void luna(){
	angulo_rotacional_luna+=VL;
	angulo_t_luna+=Vl;
	glColor3f(0.5, 0.5, 0.5);
	//glRotatef(angulo_t_luna,0,1,0);
	glTranslatef(2*sin(angulo_t_luna),0,2*cos(angulo_t_luna));
	//glTranslatef(2*sin(angulo_t_luna),2*cos(angulo_t_luna),0);
	glRotatef(angulo_rotacional_luna,0,1,0);
	glutSolidSphere(0.5, 8, 8);
}




void marte(){
	angulo_rotacional_marte+= VM;
	angulo_t_marte+=Vm;
	glPushMatrix();
		glColor3f(1, 0.5,0);
		//glRotatef(angulo_t_marte,0,0,1);
		glTranslatef(18*sin(angulo_t_marte),0,18*cos(angulo_t_marte));
		//glTranslatef(18*sin(angulo_t_marte),18*cos(angulo_t_marte),0);
		glRotatef(angulo_rotacional_marte,0,1,0);
		glutSolidSphere( 2.0, 8, 8);
	glPopMatrix();
}



void tierra(){
	angulo_rotacional_tierra+=VT;
	angulo_t_tierra+=Vt;
    /*X=10*sin(angulo_t_tierra); con y
    Y=0;
    Z=10*cos(angulo_t_tierra);*/
	X=10*sin(angulo_t_tierra);
    Y=0;
    Z=10*cos(angulo_t_tierra);
	//cout<< X <<" "<<Z<<endl;
	glPushMatrix();
		glColor3f(0, 0.5,1);
		glTranslatef(X,Y,Z);
		glRotatef(angulo_rotacional_tierra,0,1,0);
		glutSolidSphere( 1.0, 8, 8);

		luna();
		
		
        
	glPopMatrix();
}

void sol(){
	angulo_rotacional_sol+=VS;
	angulo_t_sol+=Vs;
	glPushMatrix();
		glColor3f(1,1,0);	
		//glTranslatef(10*sin(angulo_t_sol),0,10*cos(angulo_t_sol));
		//glTranslatef(10*sin(angulo_t_sol),10*cos(angulo_t_sol),0);
		glRotatef(angulo_rotacional_sol,0,1,0);
		glutSolidSphere( 2.0, 8, 8);	
	glPopMatrix();
	tierra();
	marte();
	
}

GLvoid window_display(){
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	//glOrtho(-25.0f, 25.0f, -25.0f, 25.0f, -25.0f, 25.0f);
    
    //EJERCICIO 2
    gluPerspective(45,1,1,100);//glperspective(angulo de vision,ancho/alto,vista cercana,vista lejana) 
    //glTranslatef(0,0,-50);
    gluLookAt(20,20,20,X,Y,Z,0,1,0);
	
	sol();	
    //tierra();

	glutSwapBuffers();
	glFlush();
}

GLvoid window_reshape(GLsizei width, GLsizei height){
	glViewport(0, 0, width, height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	//glOrtho(-25.0f, 25.0f, -25.0f, 25.0f, -25.0f, 25.0f);

	glMatrixMode(GL_MODELVIEW);
}



void init_scene()
{

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
GLvoid window_idle()
{
	glutPostRedisplay();
}

int main(int argc, char **argv)
{
	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(600, 600);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("TP 2 : Transformaciones");

	initGL();
	init_scene();

	glutDisplayFunc(&window_display);
	glutReshapeFunc(&window_reshape);
	glutKeyboardFunc(&window_key);

	//function called on each frame
	glutIdleFunc(&window_idle);
	glutMainLoop();

	return 1;
}

