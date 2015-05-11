#include <windows.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <ctime>
#include <time.h>
#include <string>
#include <queue>
#include "Enemie.h"
#include "GL/glut.h"

using namespace std;

#define RED 0
#define GREEN 0
#define BLUE 0
#define ALPHA 1
#define MAX_LIVES 3
#define ECHAP 27


GLfloat xi = 0.0;
GLfloat yi = 0.0;
int points = 0;
int level = 1;

void init_scene();
GLvoid initGL();
GLvoid window_display();
GLvoid window_reshape(GLsizei width, GLsizei height);
GLvoid window_key(unsigned char key, int x, int y);
void timer(int n);


//variables para el gizmo
float delta_x = 0.0;
float delta_y = 0.0;
float mouse_x, mouse_y;
float var_x = 0.0;
float var_z = -30.0;
float step = 0; //0.0 Posicion inicial. 1.0 Traslacion. 2.0 Primera Rotacion(en y). 3.0 Segunda Rotacion (en x) 4.0 Ultima Rotacion (en z)
float distancia = 25;
bool direccion = 0;

///////////////////////////////////////////////////////////////////////////////
//(1)
///////////////////////////////////////////////////////////////////////////////
GLvoid callback_special(int key, int x, int y)
{
    if( xi < 21 && xi > -21)
    {
        switch (key)
        {
            case GLUT_KEY_RIGHT:
            xi += 0.5;
            break;
            case GLUT_KEY_LEFT:
            xi -= 0.5;
            break;
            case GLUT_KEY_UP:
            yi += 0.5;
            break;
            case GLUT_KEY_DOWN:
            yi -= 0.5;
            break;
        }
    }
    else
    {
        if (xi >= 21)
            xi = 20.99;
        else
            xi = -20.99;
    }
}



///////////////////////////////////////////////////////////////////////////////
//(2)
///////////////////////////////////////////////////////////////////////////////
GLvoid callback_mouse(int button, int state, int x, int y)
{
	if (state == GLUT_DOWN && button == GLUT_LEFT_BUTTON)
	{
		mouse_x = (x - 400) * 3.0 / 800;
		mouse_y = (400 - y) * 3.0 / 800;
		glutPostRedisplay();
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


//function called on each frame
GLvoid window_idle();

int main(int argc, char **argv)
{
    int i = 60;
	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);


	glutInitWindowSize(800, 800);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("Space");


	initGL();
	init_scene();


	glutDisplayFunc(&window_display);
	glutReshapeFunc(&window_reshape);


	glutMouseFunc(&callback_mouse);
	//glutMotionFunc(&callback_motion);

	glutKeyboardFunc(&window_key);
	//glutKeyboardUpFunc(&window_key_up); //key release events
	glutSpecialFunc(&callback_special);
	//glutSpecialUpFunc(&callback_special_up); //key release events

	//function called on each frame
	glutIdleFunc(&window_idle);

	glutMainLoop();

	return 1;
}



GLvoid initGL()
{

	glEnable(GL_DEPTH_TEST);

	glClearColor(RED, GREEN, BLUE, ALPHA);
}

void Estrellas()
{
    if(distancia >= 25)
        direccion = 1;
    if(distancia <= -25)
        direccion = 0;
    if(direccion)
        distancia = distancia - 0.01;
    else
        distancia = 25;
    glTranslatef(0,distancia,-10);
    glBegin(GL_POINTS);
    for(int i = 0 ; i < 30 ; i++)
    {
        //int r , q;
        srand((unsigned) time(NULL));
        /*r = (rand() % 51) - 25;
        q = (rand() % 3) + 20;*/
        glVertex3f((rand() % 51) - 25 , 0, 0.0f);
    }
    glEnd();
}

void renderBitmapString(float x, float y, float z, void *font, char *string)
{
    char *c;
    glRasterPos3f(x, y,z);

    for(c = string; *c != '\0'; c++)
        glutBitmapCharacter(font, *c);
}

void imprimirPuntos()
{
    glPushMatrix();
    char puntos[30], puntos_num[30];
    sprintf(puntos , "Puntos:");
    glColor3ub(255,255,0);
    renderBitmapString(17, 22, -10, GLUT_BITMAP_TIMES_ROMAN_10, puntos);
    sprintf(puntos_num , "%d", points);
    glColor3ub(255,255,255);
    renderBitmapString(22, 22, -10, GLUT_BITMAP_TIMES_ROMAN_10, puntos_num);
    glPopMatrix();
}

void imprimirNivel()
{
    glPushMatrix();
    char nivel[30], nivel_num[30];
    sprintf(nivel , "Nivel:");
    glColor3ub(255,255,0);
    renderBitmapString(-2 , 22 , -10, GLUT_BITMAP_TIMES_ROMAN_10, nivel);
    sprintf(nivel_num , "%d", level);
    glColor3ub(255,255,255);
    renderBitmapString(2 , 22, -10, GLUT_BITMAP_TIMES_ROMAN_10, nivel_num);
    glPopMatrix();
}

void imprimirVidas()
{
    glPushMatrix();
    char vidas[30];
    sprintf(vidas , "Vidas: ");
    glColor3ub(255,255,0);
    renderBitmapString(-22 , 22 , -10, GLUT_BITMAP_TIMES_ROMAN_10, vidas);
    glTranslatef(-18 , 22 , -10.0);
    glutSolidSphere(0.3 , 20 , 20);
    for (int i = 1 ; i < MAX_LIVES ; i++)
    {
        glTranslatef(1 , 0 , 0.0);
        glutSolidSphere(0.3 , 20 , 20);
    }
    glPopMatrix();
}

queue<int> enemies;

void dibujar_nave()
{
    glPushMatrix();
    glTranslatef(xi, yi, 0.0);
    glBegin(GL_TRIANGLES);
    glColor3f(1.0, 1.0, 1.0);
    glVertex3f(-0.5, 0.0 , 0.0);
    glVertex3f(0.5, 0.0, 0.0);
    glVertex3f(0.0, 1.0 , 0.0);
    glEnd();
    glPopMatrix();
}

float e1x = 0;
float e1y = 21;
float e2x = -10;
float e2y = 21;
float e3x = 10;
float e3y = 21;
float mx, my;

void avanzar()
{
    if(e1y >= -21)
    {
        glTranslatef(e1x , e1y , 0);
        e1y = e1y - 0.009;
    }
}

float bx = e2x;
float by = e2y;

void disparar1()
{
    if(by > -21 && bx > -21)
    {
        glPushMatrix();
            glTranslatef(bx , by , 0);
            by = by - 0.05;
            glBegin(GL_LINE);
                glColor3f(0,1,0);
                glVertex3f(0,0.2,0);
                glVertex3f(0,0,0);
            glEnd();
        glPopMatrix();
    }
}

void disparar2()
{
    if(by > -21 && bx > -21)
    {
        glPushMatrix();
            glTranslatef(bx , by , 0);
            bx = bx - 0.05;
            by = (((bx - e2x)/(xi-e2x))*(yi-e2y))-e2y;
            glBegin(GL_LINE);
                glColor3f(0,1,0);
                glVertex3f(0,0.2,0);
                glVertex3f(0,0,0);
            glEnd();
        glPopMatrix();
    }
}

void enemigo1()
{
    if (e1y > -21)
    {
        glPushMatrix();
            avanzar();
            glBegin(GL_QUADS);
                glColor3f(1,0,0);
                glVertex3f(-0.5,-0.5,0);
                glVertex3f(0.5,-0.5,0);
                glVertex3f(0.5,0.5,0);
                glVertex3f(-0.5,0.5,0);
            glEnd();
        glPopMatrix();
    }
}

void enemigo2()
{
    if (e2y > -21)
    {
        glPushMatrix();
            glTranslatef(e2x , e2y , 0);
            e2y = e2y - 0.005;
            glBegin(GL_QUADS);
                glColor3f(0,1,0);
                glVertex3f(-0.5,-0.5,0);
                glVertex3f(0.5,-0.5,0);
                glVertex3f(0.5,0.5,0);
                glVertex3f(-0.5,0.5,0);
            glEnd();
        glPopMatrix();
    }
}

float angulo = 0;
bool subir = 0;
float altura = 8;

void rotar()
{
    if(e3y >= -21)
    {
        glTranslatef(e3x , e3y , 0);
        e3y = e3y - 0.009;
    }
}

void enemigo3()
{
    if (e1y > -21)
    {
        glPushMatrix();
            rotar();
            glBegin(GL_QUADS);
                glColor3f(0,0,1);
                glVertex3f(-0.5,-0.5,0);
                glVertex3f(0.5,-0.5,0);
                glVertex3f(0.5,0.5,0);
                glVertex3f(-0.5,0.5,0);
            glEnd();
        glPopMatrix();
    }
}


void meteorito()
{
    //
}

GLvoid window_display()
{

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-25.0f, 25.0f, -25.0f, 25.0f, -25.0f, 25.0f);
    /*enemigo1();
    disparar1();
    if (e1y < 10)
        enemigo2();*/
        enemigo3();
    imprimirPuntos();
    imprimirNivel();
    imprimirVidas();

	dibujar_nave();

	Estrellas();


	glutSwapBuffers();

	glFlush();

}

GLvoid window_reshape(GLsizei width, GLsizei height)
{
	glViewport(0, 0, width, height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

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

void init(void)
{
    glClearColor (0.0, 0.0, 0.0, 0.0);
}
