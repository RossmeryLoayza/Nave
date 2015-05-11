#ifndef ENEMIE_H
#define ENEMIE_H
#include <windows.h>
#include "GL/glut.h"


class Enemie
{
    public:
        Enemie(int x , int t);
        void animar();
        void meteorito();
        void disparar();
        virtual ~Enemie();
    protected:
        int pos_x;
        int pos_y = 20;
        int type;
    private:
};

#endif // ENEMIE_H
