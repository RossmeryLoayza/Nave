#include "Enemie.h"

Enemie::Enemie(int x , int t)
{
    pos_x = x;
    type = t;
}

void Enemie::animar()
{
    if (type == 0)
    {
        meteorito();
    }
    else
    {
        glPushMatrix();
            glTranslatef(pos_x , pos_y , 0);
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

void Enemie::disparar()
{
    glPushMatrix();
        glTranslatef(pos_x , pos_y , 0);
        glBegin(GL_LINE);
        glColor3f(1,1,1);
            glVertex3f(0,0.3,0);
            glVertex3f(0,0,0);
        glEnd();
    glPopMatrix();
}

void Enemie::meteorito()
{
    glPushMatrix();
        glTranslatef(pos_x , pos_y , 0);
        glutSolidSphere(0.5,16,16);
    glPopMatrix();
}

Enemie::~Enemie()
{
    //dtor
}
