#define GLUT_DISABLE_ATEXIT_HACK	

#include <math.h>
#include <iostream>
#include <GL/glut.h>


#define KEY_ESC 27

int num_segmentos=100;

using namespace std;
void cuadrado(int x,int y,int arista){
	glBegin(GL_LINE_LOOP);
		arista=arista/2;
		glVertex2f(x-arista,y+arista);
		glVertex2f(x+arista,y+arista);
		glVertex2f(x+arista,y-arista);
		glVertex2f(x-arista,y-arista);
	glEnd();
}

void circulo(int cx, int cy,int radio){
	glBegin(GL_LINE_LOOP);
		for(int i=0;i<num_segmentos;i++){
			float angulo=2.0f*3.1415926f*float(i)/float(num_segmentos);
			float x=radio*cosf(angulo);
			float y=radio*sinf(angulo);
			glVertex2f(x+cx,y+cy);
		}
	glEnd();
}

void circulos_dentro(int cx,int cy, int radio, int reduccion,int cantidad){
	double temp_radio;
	for(int i=0; i < cantidad; i++){
		circulo(cx,cy,radio);
		temp_radio=radio-(radio*reduccion/100);
		//cout<<"temp"<<temp_radio<<endl;
		cx=cx-(radio-temp_radio);
		radio=temp_radio;		
	}		
}

void circulos_lado(int cx,int cy, int radio, int reduccion,int cantidad){
	double temp_radio;
	for(int i=0; i < cantidad; i++){
		circulo(cx,cy,radio);
		temp_radio=radio-(radio*reduccion/100);
		//cout<<"temp"<<temp_radio<<endl;
		cx=cx+(radio+temp_radio);
		radio=temp_radio;		
	}		
}

void circulo_angulo(int cx,int cy, int radio, int reduccion, int cantidad, int angulo){
	double temp_radio;
	double radianes=angulo*3.1415926f/180;
	for(int i=0; i < cantidad; i++){
		circulo(cx,cy,radio);
		temp_radio=radio-(radio*reduccion/100);
		cx=cx+((radio+temp_radio)*cos(radianes));
		cy=cy+((radio+temp_radio)*sin(radianes));
		radio=temp_radio;		
	}
}

void displayGizmo()
{
	cuadrado(-40,40,10);	

	circulo(-20,40,5);

	circulos_dentro(0,40,10,20,5);

	circulos_lado(-40,20,10,20,5);

	circulo_angulo(-40,-30,10,20,4,45);
}

//
//funcion llamada a cada imagen
void glPaint(void) {

	//El fondo de la escena al color initial
	glClear(GL_COLOR_BUFFER_BIT); //(R, G, B, transparencia) en este caso un fondo negro
	glLoadIdentity();
	
	
	//dibuja el gizmo
	displayGizmo();

	//doble buffer, mantener esta instruccion al fin de la funcion
	glutSwapBuffers();
}

//
//inicializacion de OpenGL
//
void init_GL(void) {
	//Color del fondo de la escena
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f); //(R, G, B, transparencia) en este caso un fondo negro

	//modo projeccion 
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
}

//en el caso que la ventana cambie de tama�o
GLvoid window_redraw(GLsizei width, GLsizei height) {
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	
	glOrtho(-50.0f,  50.0f,-50.0f, 50.0f, -1.0f, 1.0f); 
	// todas la informaciones previas se aplican al la matrice del ModelView
	glMatrixMode(GL_MODELVIEW);
}

GLvoid window_key(unsigned char key, int x, int y) {
	switch (key) {
	case KEY_ESC:
		exit(0);
		break;

	default:
		break;
	}

}
//
//el programa principal
//
int main(int argc, char** argv) {

	//Inicializacion de la GLUT
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(600, 600); //tama�o de la ventana
	glutInitWindowPosition(100, 100); //posicion de la ventana
	glutCreateWindow("TP1 OpenGL : hello_world_OpenGL"); //titulo de la ventana

	init_GL(); //funcion de inicializacion de OpenGL

	glutDisplayFunc(glPaint); 
	glutReshapeFunc(&window_redraw);
	// Callback del teclado
	glutKeyboardFunc(&window_key);

	glutMainLoop(); //bucle de rendering

	return 0;
}
