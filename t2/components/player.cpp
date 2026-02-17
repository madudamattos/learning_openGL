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
    this->height = 0.0f;
    
    this->material = Material(1.0f,1.0f,1.0f);

    this->footWalking = Foot::RightFront; 
    this->isWalking = false;
    this->lastWalkTime = 0.0f;

    this->armAngle = 0.0f; 
    this->bodyAngle = 0.0f;

    this->pos = Alglib::Mat3::Identity();
}

void Player::SetParameters(GLfloat x, GLfloat y, GLfloat z, GLfloat rad, GLfloat height, std::function<void()> onCollision = nullptr){
    this->gXinit = x;
    this->gYinit = y;
    this->gZinit = z;
    this->radius = rad;
    this->height = height;

    this->bodyAngle = 0.0f;
    this->footWalking = Foot::LeftFront;
    this->isWalking = false;
    
    // inicia colisor 
    collider.SetParameters(x,y, z, rad, height, epsilon, CollisionType::External, [onCollision](Collider*, Collider*){if (onCollision) onCollision();});


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

GLfloat Player::GetHeight()
{
    return this->height;
}

void Player::DefineColor(GLfloat R, GLfloat G, GLfloat B){
    this->material.SetColor(R,G,B);
}

// debug function
void Player:: DrawAxes(double size)
{
    GLfloat mat_ambient_r[] = { 1.0, 0.0, 0.0, 1.0 };
    GLfloat mat_ambient_g[] = { 0.0, 1.0, 0.0, 1.0 };
    GLfloat mat_ambient_b[] = { 0.0, 0.0, 1.0, 1.0 };
    GLfloat no_mat[] = { 0.0, 0.0, 0.0, 1.0 };
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, 
            no_mat);
    glMaterialfv(GL_FRONT, GL_SPECULAR, no_mat);
    glMaterialfv(GL_FRONT, GL_SHININESS, no_mat);

    //x axis red
    glPushMatrix();
        glMaterialfv(GL_FRONT, GL_EMISSION, 
                mat_ambient_r);
        glColor3fv(mat_ambient_r);
        glScalef(size, size*0.1, size*0.1);
        glTranslatef(0.5, 0, 0); // put in one end
        glutSolidCube(1.0);
    glPopMatrix();

    //y axis green
    glPushMatrix();
        glMaterialfv(GL_FRONT, GL_EMISSION, 
                mat_ambient_g);
        glColor3fv(mat_ambient_g);
        glRotatef(90,0,0,1);
        glScalef(size, size*0.1, size*0.1);
        glTranslatef(0.5, 0, 0); // put in one end
        glutSolidCube(1.0);
    glPopMatrix();

    //z axis blue
    glPushMatrix();
        glMaterialfv(GL_FRONT, GL_EMISSION, mat_ambient_b);
        glColor3fv(mat_ambient_b);
        glRotatef(-90,0,1,0);
        glScalef(size, size*0.1, size*0.1);
        glTranslatef(0.5, 0, 0); // put in one end
        glutSolidCube(1.0);
    glPopMatrix();    
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
    glTranslatef(radius, 0, 0); // translada pra lateral da esfera
    glRotatef(armAngle.x, 1, 0, 0); // rotaciona a arma em x
    glRotatef(armAngle.y, 0, 1, 0); // rotaciona a arma em y
    glTranslatef(0, 0, rectHeight/2); // translata para desenhar a partir centro do paralelepipedo
    DrawRect(rectWidth,rectWidth, rectHeight); // desenha a arma
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
    GLfloat newY = tmp.m[1][3];
    GLfloat newZ = tmp.m[2][3];

    Collider proposed;

    proposed.SetParameters(newX, newY, newZ, (float)this->radius, this->GetHeight(), epsilon, CollisionType::External, NULL);

    if (gCollisionSystem->TestCollision(proposed, &this->collider)) {
        return;
    }

    this->pos.Translate(0, 0, inc);
    this->collider.SetCircle(newX, newY, newZ, (float)this->radius, this->GetHeight(), epsilon);

    // this->CheckFootChange(tDif);
}

void Player::ResetPosition()
{
    this->pos.Replace(Alglib::Mat3::Identity());
    pos.Translate(this->gXinit, this->gYinit, this->gZinit);

    this->bodyAngle = 0.0f;
    this->armAngle.x = this->armAngle.y = 0.0f;
    //this->footWalking = Foot::RightFront;
    this->isWalking = false;
    this->lastWalkTime = 0.0;

    this->SetArmAngle(0.0f, 0.0f); 

    this->collider.SetCircle(this->GetXPos(), this->GetYPos(), this->GetZPos(), (float)this->radius, this->GetHeight(), epsilon);
}

void Player::SetArmAngle(GLfloat degX, GLfloat degY)
{
    if (degX > 45.0f) degX = 45.0f;
    if (degX < -45.0f) degX = -45.0f;

    if (degY > 45.0f) degY = 45.0f;
    if (degY < -45.0f) degY = -45.0f;
    
    this->armAngle.x = degY;
    this->armAngle.y = degX;

    Alglib::Mat3 m = Alglib::Mat3::Identity();

    m = GetGunPos().Copy();

    m.Rotate(this->armAngle.x, this->armAngle.y, 0);
    
    this->gunPos.Replace(m);
}

void Player::IncreaseArmAngle(GLfloat incX, GLfloat incY)
{
    this->armAngle.x += incX;
    this->armAngle.y += incY;

    if (this->armAngle.x > 45.0f) this->armAngle.x = 45.0f;
    else if (this->armAngle.x < -45.0f) this->armAngle.x = -45.0f;

    if (this->armAngle.y > 45.0f) this->armAngle.y = 45.0f;
    else if (this->armAngle.y < -45.0f) this->armAngle.y = -45.0f;

    Alglib::Mat3 m = GetGunPos().Copy();
    m.Rotate(this->armAngle.x, this->armAngle.y, 0);
    this->gunPos.Replace(m);
}

Alglib::Mat3 Player::GetGunPos()
{
    Alglib::Mat3 m = Alglib::Mat3::Identity();

    m.Translate(GetXPos(), GetYPos(), GetZPos())
     .Translate(0, 0.75*radius, 0)
     .RotateY(this->bodyAngle)
     .Translate(radius, radius*0.9, 0);

    return m;
}

Bullet* Player::Shoot()
{
    Alglib::Mat3 shootPos = GetGunPos().Copy();
 
    Alglib::Tuple3 bulletPos(1.0f);
    Alglib::Tuple3 armPos(1.0f);

    armPos.Alglib::Tuple3::Transform(shootPos);

    shootPos.Rotate(armAngle.x, armAngle.y, 0)
            .Translate(0, 0, rectHeight);

    bulletPos.Alglib::Tuple3::Transform(shootPos);

    Alglib::Tuple3 directionVector = Alglib::Subtract(bulletPos, armPos);

    return new Bullet(bulletPos, directionVector);
}