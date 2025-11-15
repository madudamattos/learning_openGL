#include "tiro.h"
#include <math.h>
#include <stdio.h>
#define DISTANCIA_MAX 10

void Tiro::DesenhaCirc(GLint radius, GLfloat R, GLfloat G, GLfloat B)
{
    glPointSize(2.0f); 
    glColor3f(R, G, B); 
    glBegin(GL_POLYGON);
        for(int ang=0; ang<360; ang+=20)
        {
            float rad = ang * M_PI / 180.0f; // converte para radianos
            float x = radius * cos(rad);
            float y = radius * sin(rad);

            glVertex3f(x, y, 0.0f); // ponto no círculo
        }
    glEnd();
}

void Tiro::DesenhaTiro(GLfloat x, GLfloat y)
{
    glPushMatrix();
    glTranslatef(x,y,0);
    DesenhaCirc(radiusTiro, 1, 1, 1);
    glPopMatrix();  

    gXInit = x; 
    gYInit = y;
}

void Tiro::Move(GLdouble timeDiff )
{
    gX += gVel * cos(gDirectionAng * M_PI / 180.0f) * timeDiff;
    gY += gVel * sin(gDirectionAng * M_PI / 180.0f) * timeDiff;
}

bool Tiro::Valido()
{   
    // Calcula a distância total percorrida pelo tiro
    float dx = gX - gXInit;
    float dy = gY - gYInit;
    float distanciaPercorrida = sqrt(dx*dx + dy*dy);
    
    if(distanciaPercorrida > DISTANCIA_MAX)
    {    
        printf("O tiro deixou de ser válido. Distância percorrida: %.2f\n", distanciaPercorrida);
        return false;
    }
    return true;
}
