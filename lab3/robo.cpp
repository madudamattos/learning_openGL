#include "robo.h"
#include <stdio.h>
#include <math.h>

void Robo::DesenhaRect(GLint height, GLint width, GLfloat R, GLfloat G, GLfloat B)
{
    glColor3f (R, G, B);

    glBegin(GL_POLYGON);
      glVertex3f (-width/2, 0, 0.0);
      glVertex3f (width/2, 0, 0.0);
      glVertex3f (width/2, height, 0.0);
      glVertex3f (-width/2, height, 0.0);
   glEnd();
}

void Robo::DesenhaCirc(GLint radius, GLfloat R, GLfloat G, GLfloat B)
{
    glPointSize(2.0f); 
    glColor3f(R, G, B); 
    glBegin(GL_POINTS);
        for(int ang=0; ang<360; ang+=20)
        {
            float rad = ang * M_PI / 180.0f; // converte para radianos
            float x = radius * cos(rad);
            float y = radius * sin(rad);

            glVertex3f(x, y, 0.0f); // ponto no círculo
        }
      
    glEnd();
}

void Robo::DesenhaRoda(GLfloat x, GLfloat y, GLfloat thetaWheel, GLfloat R, GLfloat G, GLfloat B)
{
    glPushMatrix();
    glTranslatef(x,y,0);
    glRotatef(thetaWheel,0,0,1);
    DesenhaCirc(radiusWheel, R, G, B);
    glPopMatrix();  
}

void Robo::DesenhaBraco(GLfloat x, GLfloat y, GLfloat theta1, GLfloat theta2, GLfloat theta3)
{
    glPushMatrix();
    glTranslatef(x,y,0);
    glRotatef(theta1,0,0,1);
    DesenhaRect(paddleHeight, paddleWidth, 0, 0, 1);
    glTranslatef(0,paddleHeight,0);
    glRotatef(theta2,0,0,1);
    DesenhaRect(paddleHeight, paddleWidth, 1, 1, 0);
    glTranslatef(0,paddleHeight,0);
    glRotatef(theta3,0,0,1);
    DesenhaRect(paddleHeight, paddleWidth, 0, 1, 0);
    glPopMatrix();    
}

void Robo::DesenhaRobo(GLfloat x, GLfloat y, GLfloat thetaWheel, GLfloat theta1, GLfloat theta2, GLfloat theta3)
{
    glPushMatrix();
    glTranslatef(x,y,0);
    DesenhaRect(baseHeight,baseWidth,1,0,0);
    DesenhaBraco(0, baseHeight,theta1,theta2,theta3);
    DesenhaRoda(-baseWidth/2, 0, thetaWheel, 1,1,1);
    DesenhaRoda(baseWidth/2, 0, thetaWheel, 1,1,1);

    glPopMatrix();

}

void Robo::RodaBraco1(GLfloat inc)
{
    gTheta1 += inc;
}

void Robo::RodaBraco2(GLfloat inc)
{
    gTheta2 += inc;
}

void Robo::RodaBraco3(GLfloat inc)
{
    gTheta3 += inc;
}

void Robo::MoveEmX(GLfloat dx)
{
    gX+=dx*100;

    gThetaWheel -= dx * 100 * 180.0f / (M_PI * radiusWheel);
}

void RotatePoint(GLfloat x, GLfloat y, GLfloat angle, GLfloat &xOut, GLfloat &yOut)
{
    // Converte ângulo de graus para radianos
    float angleRad = angle * M_PI / 180.0f;
    
    // Aplica matriz de rotação
    xOut = x * cos(angleRad) - y * sin(angleRad);
    yOut = x * sin(angleRad) + y * cos(angleRad);
}

Tiro* Robo::Atira()
{
    GLfloat xBase = 0, yBase = 0;
    GLfloat xTip = 0, yTip = paddleHeight;
    GLfloat xFinal, yFinal;
    
    // Aplica as transformações na mesma ordem do desenho
    // Translação para posição do robô
    xBase += gX;
    yBase += gY + baseHeight;  // Adiciona altura da base
    xTip += gX;
    yTip += gY + baseHeight;
    
    // Rotação do primeiro braço
    RotatePoint(xTip - xBase, yTip - yBase, gTheta1, xTip, yTip);
    xTip += xBase;
    yTip += yBase;
    
    // Atualiza base para próxima seção
    xBase = xTip;
    yBase = yTip;
    xTip = xBase;
    yTip = yBase + paddleHeight;
    
    // Rotação do segundo braço
    RotatePoint(xTip - xBase, yTip - yBase, gTheta2, xTip, yTip);
    xTip += xBase;
    yTip += yBase;
    
    // Atualiza base para última seção
    xBase = xTip;
    yBase = yTip;
    xTip = xBase;
    yTip = yBase + paddleHeight;
    
    // Rotação do terceiro braço
    RotatePoint(xTip - xBase, yTip - yBase, gTheta3, xTip, yTip);
    xTip += xBase;
    yTip += yBase;
    
    // Calcula o ângulo do tiro
    float dx = xTip - xBase;
    float dy = yTip - yBase;
    float anguloTiro = atan2(dy, dx) * 180.0f / M_PI;
    
    // Cria novo tiro na posição da ponta do braço
    return new Tiro(xTip, yTip, anguloTiro);
}