#include "player.h"
#include "material.h"
#include <GL/glut.h>

#define timeOffset 0.4f // intervalo em segundos entre troca de perna
#define epsilon 0.0f 

// extern CollisionSystem* gCollisionSystem;

Player::Player(){
    this->gXinit = 0.0f;
    this->gYinit = 0.0f;
    this->gZinit = 0.0f;
    this->radius = 0.0f;
    
    this->material = Material(1.0f,1.0f,1.0f);

    this->footWalking = Foot::RightFront; 
    this->isWalking = false;
    this->lastWalkTime = 0.0f;

    this->armAngle = 0.0f; 
    this->bodyAngle = 0.0f;

    this->pos = Alglib::Mat3::Identity();
    this->gunPos = Alglib::Mat3::Identity();
}

void Player::SetParameters(GLfloat x, GLfloat y, GLfloat z, GLfloat rad, std::function<void()> onCollision = nullptr){
    this->gXinit = x;
    this->gYinit = y;
    this->gZinit = z;
    this->radius = rad;

    this->bodyAngle = 0.0f;
    this->footWalking = Foot::LeftFront;
    this->isWalking = false;
    
    // seta matriz de posicao inicial 
    pos.Translate(x,y,z);
}

GLfloat Player::GetXPos()
{
    return this->pos.m[0][3];
}

GLfloat Player::GetYPos()
{
    return this->pos.m[1][3];
}

GLfloat Player::GetZPos()
{
    return this->pos.m[2][3];
}


void Player::DefineColor(GLfloat R, GLfloat G, GLfloat B){
    this->material.SetColor(R,G,B);
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
    GLfloat gZ = this->GetZPos();

    this->material.Apply(GL_FRONT_AND_BACK);

    // pernas
    glPushMatrix();
    glTranslatef(gX, 0.75*radius, gZ);
    glRotatef(bodyAngle, 0, 1, 0);
    DrawRect(radius*.8, 1.5*radius, radius*.8);
    glPopMatrix();

    // corpo
    glPushMatrix();
    glTranslatef(gX, 0.75*radius, gZ);
    glRotatef(bodyAngle, 0, 1, 0);
    glTranslatef(0, radius*0.9, 0);
    DrawSphere(radius);
    glPopMatrix();

    // braço
    glPushMatrix();
    glTranslatef(gX, 0.75*radius, gZ);
    glRotatef(bodyAngle, 0, 1, 0);
    glTranslatef(0, radius*0.9, 0); // translada pro meio do corpo
    glTranslatef(radius, 0, radius); // translada pra lateral da esfera
    DrawRect(rectWidth,rectWidth, rectHeight);
    glPopMatrix();

    // cabeça
    glPushMatrix();
    glTranslatef(gX, 0.75*radius, gZ);
    glRotatef(bodyAngle, 0, 1, 0);
    glTranslatef(0, radius*0.9, 0);
    glTranslatef(0, 2*radius*0.7, 0);
    DrawSphere(radius * 0.6);
    glPopMatrix();
}

void Player::SetRotation(GLfloat angle)
{
    while (angle < 0.0f) angle += 360.0f;
    while (angle >= 360.0f) angle -= 360.0f;

    float delta = angle - bodyAngle;
    if (delta > 180.0f) delta -= 360.0f;
    else if (delta <= -180.0f) delta += 360.0f;

    this->pos.RotateY(delta);
    bodyAngle = angle;

    while (bodyAngle < 0.0f) bodyAngle += 360.0f;
    while (bodyAngle >= 360.0f) bodyAngle -= 360.0f;
}

void Player::Rotate(GLfloat inc, GLdouble tDif)
{
    bodyAngle += inc;
    if(bodyAngle >= 360) bodyAngle = 0;
    else if(bodyAngle < 0) bodyAngle = 360;

    this->pos.RotateY(inc);
    // this->CheckFootChange(tDif);
}


void Player::Move(GLfloat inc, GLdouble tDif)
{
    Alglib::Mat3 tmp = this->pos.Copy();
    tmp.Translate(0, 0, inc);

    GLfloat newX = tmp.m[0][3];
    GLfloat newY = .0f;
    GLfloat newZ = tmp.m[2][3];

    //Collider proposed;

    // proposed.SetParameters(newX, newY, (float)this->radius, epsilon, CollisionType::External, NULL);

    // if (gCollisionSystem->TestCollision(proposed, &this->collider)) {
    //     return;
    // }

    this->pos.Translate(0, 0, inc);
    // this->collider.SetCircle(newX, newY, (float)this->radius, epsilon);

    // this->CheckFootChange(tDif);
}
