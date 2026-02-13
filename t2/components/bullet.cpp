#include "bullet.h"
#include <math.h>
#include <stdio.h>
#include <iostream>

#define DISTANCIA_MAX 300.0f 
#define BULLET_RADIUS 5


Bullet::Bullet(Alglib::Tuple2 position, Alglib::Tuple2 directionVector){
    this->initPos = position;
    this->currentPos = position;
    this->directionAng = directionVector; 
    this->gVel = SPEED;
    
    col.SetParameters(position.GetX(), position.GetY(), BULLET_RADIUS, 0.0f, CollisionType::External, [this](Collider* me, Collider* other){ this->DestroySelf(); });
};


void Bullet::DrawCircle(GLint radius, GLfloat R, GLfloat G, GLfloat B)
{
    glColor3f(R, G, B);
    glBegin(GL_POLYGON);
        for(int deg=0; deg<360; deg+=20)
        {
            float theta = deg* M_PI / 180.0f; // deg->rad
            float x = radius * cosf(theta);
            float y = radius * sinf(theta);

            glVertex3f(x, -y, 0.0f);
        }
    glEnd();

    // Desenha o contorno AMARELO em volta
    glColor3f(1.0f, 1.0f, 0);
    glLineWidth(1.0f);
    glBegin(GL_LINE_LOOP);
        for(int deg=0; deg<360; deg+=20)
        {
            float theta = deg* M_PI / 180.0f; // deg->rad
            float x = radius * cosf(theta);
            float y = radius * sinf(theta);

            glVertex3f(x, -y, 0.0f); 
        }
    glEnd();
}

void Bullet::Draw()
{
    glPushMatrix();
    glTranslatef(currentPos.GetX(), currentPos.GetY(), 0);
    DrawCircle(BULLET_RADIUS, 1.0f, 1.0f, 1.0f);
    glPopMatrix();
}

void Bullet::Move(GLdouble timeDiff )
{
    // normaliza os vetores
    float dx = directionAng.GetX();
    float dy = directionAng.GetY();
    float len = sqrtf(dx*dx + dy*dy);
    if (len <= 1e-6f) return; // direção inválida, não move

    dx /= len; dy /= len;

    // deslocamento = direção_normalizada * velocidade * delta_time
    float travel = gVel * (float)timeDiff;

    Alglib::Tuple2 step(dx * travel, dy * travel, 0.0f);

    // atualiza posição
    currentPos = Alglib::Add(currentPos, step);

    this->col.SetCircle(GetXPos(), GetYPos(), BULLET_RADIUS, 0.0f);
}

bool Bullet::isValid()
{
    float dx = currentPos.GetX() - initPos.GetX();
    float dy = currentPos.GetY() - initPos.GetY();
    float dist = sqrtf(dx*dx + dy*dy);
    return dist <= DISTANCIA_MAX;
}

void Bullet::DestroySelf()
{
    gCollisionSystem->Unregister(&col);
    if (onDestroy) {
        onDestroy(this);
        return; 
    }

}