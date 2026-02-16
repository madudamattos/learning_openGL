#include "player.h"
#include "material.h"
#include <GL/glut.h>

#define timeOffset 0.4f // intervalo em segundos entre troca de perna
#define epsilon 0.0f 

extern CollisionSystem* gCollisionSystem;

Player::Player(){
    this->gXinit = 0.0f;
    this->gYinit = 0.0f;
    this->radius = 0.0f;
    
    this->material = Material(1.0f,1.0f,1.0f);

    this->footWalking = Foot::RightFront; 
    this->isWalking = false;
    this->lastWalkTime = 0.0f;

    this->armAngle = 0.0f; 
    this->bodyAngle = 0.0f;

    this->pos = Alglib::Mat2::Identity();
    this->gunPos = Alglib::Mat2::Identity();
}

void Player::SetParameters(GLint x, GLint y, GLint rad, std::function<void()> onCollision = nullptr){
    this->gXinit = x;
    this->gYinit = y;
    this->radius = rad;

    this->bodyAngle = 0.0f;
    this->footWalking = Foot::LeftFront;
    this->isWalking = false;
    
    // seta matriz de posicao inicial 
    pos.Translate(x,y);
}

GLfloat Player::GetXPos()
{
    return this->pos.m[0][2];
}

GLfloat Player::GetYPos()
{
    return this->pos.m[1][2];
}

void Player::SetFootWalking(Foot value)
{
    
}

void Player::SetPlayerCallback(PlayerCallback cb)
{
    
}

void Player::CheckFootChange(GLdouble t)
{      

}

void Player::SetArmAngle(GLfloat degrees)
{

}

void Player::IncreaseArmAngle(GLfloat inc)
{

}

void Player::DefineColor(GLfloat R, GLfloat G, GLfloat B){
    this->material.SetColor(R,G,B);
}

void Player::DrawElipse(GLint radius, GLfloat R, GLfloat G, GLfloat B, bool isCircle)
{   

}

void Player::DrawRect(GLfloat sx, GLfloat sy, GLfloat sz)
{   
    glScalef(sx, sy, sz);
    glutSolidCube(1.0);
}

void Player::DrawSphere(GLfloat r)
{
    glutSolidSphere((GLdouble) r, 20, 10);
}

void Player::Draw()
{   
    GLfloat gX = this->GetXPos();
    GLfloat gZ = this->GetYPos();

    this->material.Apply(GL_FRONT_AND_BACK);

    // pernas
    glPushMatrix();
    glTranslatef(gX, 0.75*radius, gZ);
    glRotatef(bodyAngle, 0, 0, 1);
    DrawRect(radius*.8, 1.5*radius, radius*.8);
    glPopMatrix();

    // corpo
    glPushMatrix();
    glTranslatef(gX, 0.75*radius, gZ);
    glRotatef(bodyAngle, 0, 0, 1);
    glTranslatef(0, radius*0.9, 0);
    DrawSphere(radius);
    glPopMatrix();

    // braço
    glPushMatrix();
    glTranslatef(gX, 0.75*radius, gZ);
    glRotatef(bodyAngle, 0, 0, 1);
    glTranslatef(0, radius*0.9, 0); // translada pro meio do corpo
    glTranslatef(radius, 0, radius); // translada pra lateral da esfera
    DrawRect(rectWidth,rectWidth, rectHeight);
    glPopMatrix();

    // cabeça
    glPushMatrix();
    glTranslatef(gX, 0.75*radius, gZ);
    glRotatef(bodyAngle, 0, 0, 1);
    glTranslatef(0, radius*0.9, 0);
    glTranslatef(0, 2*radius*0.7, 0);
    DrawSphere(radius * 0.6);
    glPopMatrix();
}

void Player::SetRotation(GLfloat angle)
{

}

void Player::Rotate(GLfloat inc, GLdouble tDif)
{

}

void Player::ResetPosition()
{

}

void Player::Move(GLfloat inc, GLdouble tDif)
{

}

Bullet* Player::Shoot()
{
    return nullptr;
}