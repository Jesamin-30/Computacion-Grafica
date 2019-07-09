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

float a,b,c,d=0.0f;
//float temp=0.0f;
bool estado=true;

void cubo(){
	d+=2;	
	glPushMatrix();
		glColor3f(1,0,1);
		glRotatef(d,1,0,0);
		glTranslatef(0,0,-10);
		glutSolidCube(2);
	glPopMatrix();
}

void toro(){
	c+=a*3;	
	glPushMatrix();
		glColor3f(0,1,0);
		glRotatef(c,0,1,0);
		glTranslatef(-5,0,0);
		glutSolidTorus(0.5,1,20,20);
	glPopMatrix();
}

void tetera(){
	a+=2;	
	glPushMatrix();
		glColor3f(1,1,1);
		glRotatef(a,0,0,1);
		glTranslatef(-15,0,0);
		glutSolidTeapot(3);

		toro();
	glPopMatrix();
}


void esfera_lineal(){
	if(estado){
		if(b<8){
			b+=0.2;
		}
		else{
			estado=false;
		}
	}
	else{
		b-=0.2;
		if(b<-8){
			estado=true;
		}
	}
	
	glColor3f(0.0,0.0,1.0);
	glTranslatef(b,0,0);
	glutSolidSphere( 2.0, 20.0, 20.0);

	tetera();
	cubo();
}

GLvoid window_display()
{

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-25.0f, 25.0f, -25.0f, 25.0f, -25.0f, 25.0f);
	
	esfera_lineal();	

	glutSwapBuffers();
	glFlush();
}

GLvoid window_reshape(GLsizei width, GLsizei height)
{
	glViewport(0, 0, width, height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-25.0f, 25.0f, -25.0f, 25.0f, -25.0f, 25.0f);

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


	glutInitWindowSize(800, 800);
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

