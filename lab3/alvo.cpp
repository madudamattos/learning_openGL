#include "alvo.h"
#include <stdio.h>
#include <math.h>

void Alvo::DesenhaCirc(GLint radius, GLfloat R, GLfloat G, GLfloat B)
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

void Alvo::DesenhaAlvo(GLfloat x, GLfloat y)
{
    GLfloat R = 0, G = 0, B = 0;
    
    if(gColor == 0) R = 1;
    else if(gColor == 1) G = 1;
    else if(gColor == 2) B = 1;

    glPushMatrix();
    glTranslatef(x,y,0);
    DesenhaCirc(radiusAlvo, R, G, B);
    glPopMatrix();  
}

void Alvo::Recria(GLfloat x, GLfloat y)
{
    gX = x; 
    gY = y;
    
    gColor = (gColor + 1) % 3;
}

bool Alvo::Atingido(Tiro *tiro)
{
    GLfloat tx, ty;
    tiro->GetPos(tx, ty);

    // Calcula a distância entre o tiro e o centro do alvo usando o teorema de Pitágoras
    float dx = tx - gX;
    float dy = ty - gY;
    float distancia = sqrt(dx*dx + dy*dy);

    // Se a distância for menor que o raio do alvo, houve colisão
    if(distancia <= radiusAlvo)
    {
        printf("Acertou o alvo na posição (%.2f, %.2f)!\n", tx, ty);
        return true;
    }

    return false;
}
