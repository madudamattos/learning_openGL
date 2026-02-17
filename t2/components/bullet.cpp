#include "bullet.h"
#include <math.h>
#include <stdio.h>
#include <iostream>
#include <GL/glut.h>

#define DISTANCIA_MAX 300.0f 
#define BULLET_RADIUS 5


Bullet::Bullet(Alglib::Tuple3 position, Alglib::Tuple3 directionVector, Material mat){
    this->initPos = position;
    this->currentPos = position;
    this->directionAng = directionVector; 
    this->gVel = SPEED;

    this->material = mat;
    
    // col.SetParameters(position.GetX(), position.GetY(), BULLET_RADIUS, 0.0f, CollisionType::External, [this](Collider* me, Collider* other){ this->DestroySelf(); });
}

void Bullet::DrawSphere(GLfloat r)
{
    glutSolidSphere((GLdouble) r, 20, 10);
}

void Bullet::Draw()
{   
    this->material.Apply(GL_FRONT_AND_BACK);
    glPushMatrix();
    glTranslatef(currentPos.GetX(), currentPos.GetY(), currentPos.GetZ());
    DrawSphere(BULLET_RADIUS);
    glPopMatrix();
}

void Bullet::Move(GLdouble timeDiff )
{
    // normaliza os vetores
    float dx = directionAng.GetX();
    float dy = directionAng.GetY();
    float dz = directionAng.GetZ();

    float len = sqrtf(dx*dx + dy*dy + dz*dz);
    if (len <= 1e-6f) return; // direção inválida, não move

    dx /= len; dy /= len; dz /= len;

    // deslocamento = direção_normalizada * velocidade * delta_time
    float travel = gVel * (float)timeDiff;

    Alglib::Tuple3 step(dx * travel, dy * travel, dz * travel);

    // atualiza posição
    currentPos = Alglib::Add(currentPos, step);

    // this->col.SetCircle(GetXPos(), GetYPos(), BULLET_RADIUS, 0.0f);
}

bool Bullet::isValid()
{
    float dx = currentPos.GetX() - initPos.GetX();
    float dy = currentPos.GetY() - initPos.GetY();
    float dz = currentPos.GetZ() - initPos.GetZ();
    float dist = sqrtf(dx*dx + dy*dy + dz*dz);
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