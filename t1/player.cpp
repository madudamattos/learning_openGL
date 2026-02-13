#include "player.h"

#define timeOffset 0.4f // intervalo em segundos entre troca de perna
#define epsilon 0.0f 

extern CollisionSystem* gCollisionSystem;

Player::Player(){
    this->gXinit = 0.0f;
    this->gYinit = 0.0f;
    this->radius = 0.0f;
    
    this->R = 0.0f;
    this->G = 0.0f;
    this->B = 0.0f;

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
    
    // inicia colisor 
    collider.SetParameters(x,y,rad, epsilon, CollisionType::External, [onCollision](Collider*, Collider*){if (onCollision) onCollision();});

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
    this->footWalking = value;
}

void Player::SetPlayerCallback(PlayerCallback cb)
{
    this->callback = cb;
}

void Player::CheckFootChange(GLdouble t)
{      
    // acumula tempo de caminhada desde a última troca
    if (t - lastWalkTime < timeOffset) return;

    if(isWalking)
    {
        if(footWalking == Player::Foot::LeftFront){
            this->SetFootWalking(Player::Foot::RightFront);
            lastWalkTime = t;
        } else
        {
            this->SetFootWalking(Player::Foot::LeftFront);
            lastWalkTime = t;
        }       
    }
}

void Player::SetArmAngle(GLfloat degrees)
{
    if (degrees > 45.0f) degrees = 45.0f;
    if (degrees < -45.0f) degrees = -45.0f;
    
    this->armAngle = degrees;

    Alglib::Mat2 m = Alglib::Mat2::Identity();

    m.Translate(GetXPos(), GetYPos())
     .Rotate(this->bodyAngle)
     .Translate(this->radius*2 - this->radius *0.5, 0)
     .Rotate(this->armAngle);
    
    this->gunPos.Replace(m);
}

void Player::IncreaseArmAngle(GLfloat inc)
{
    this->armAngle += inc;

    if (armAngle > 45.0f) armAngle = 45.0f;
    else if (armAngle < -45.0f) armAngle = -45.0f;
    else gunPos.Rotate(inc);
}

void Player::DefineColor(GLfloat R, GLfloat G, GLfloat B){
    this->R = R;
    this->G = G;
    this->B = B;
}

void Player::DrawElipse(GLint radius, GLfloat R, GLfloat G, GLfloat B, bool isCircle)
{   
    float rx = radius;
    float ry = isCircle? radius : radius/4;
    
    glColor3f(R, G, B); 
    glBegin(GL_POLYGON);
        for(int deg=0; deg<360; deg+=20)
        {
            float theta = deg* M_PI / 180.0f; // deg->rad
            float x = rx * cos(theta);
            float y = ry * sin(theta);

            glVertex3f(x, -y, 0.0f); 
        }
    glEnd();

    // Desenha o contorno preto em volta
    glColor3f(0, 0, 0); 
    glLineWidth(1.0f);
    glBegin(GL_LINE_LOOP);
        for(int deg=0; deg<360; deg+=20)
        {
            float theta = deg* M_PI / 180.0f; // deg->rad
            float x = rx * cos(theta);
            float y = ry * sin(theta);

            glVertex3f(x, -y, 0.0f); 
        }
    glEnd();
}

void Player::DrawRect(GLint height, GLint width, GLfloat R, GLfloat G, GLfloat B)
{
    glColor3f (R, G, B);

    glBegin(GL_POLYGON);
      glVertex3f (-width/2, 0, 0.0);
      glVertex3f (width/2, 0, 0.0);
      glVertex3f (width/2, height, 0.0);
      glVertex3f (-width/2, height, 0.0);
   glEnd();

   glColor3f (0.0f, 0.0f, 0.0f);
   glLineWidth(1.0f);
   glBegin(GL_LINE_LOOP);
      glVertex3f (-width/2, 0, 0.0);
      glVertex3f (width/2, 0, 0.0);
      glVertex3f (width/2, height, 0.0);
      glVertex3f (-width/2, height, 0.0);
   glEnd();
}

void Player::Draw()
{   
    GLfloat gX = this->GetXPos();
    GLfloat gY = this->GetYPos();

    // // desenha as pernas
    if(footWalking == Foot::LeftFront){
        glPushMatrix();
        glTranslatef(gX, gY, 0);
        glRotatef(bodyAngle, 0, 0, 1);
        glTranslatef(radius/2, 0, 0); 
        DrawRect(rectHeight, rectWidth, 0.0f, 0.0f, 0.0f); // perna direita na frente
        glTranslatef(-radius, 0, 0); 
        DrawRect(-rectHeight, rectWidth, 0.0f, 0.0f, 0.0f); // perna esquerda atras
        glPopMatrix();
    } else if (footWalking == Foot::RightFront)
    {
        glPushMatrix();
        glTranslatef(gX, gY, 0);
        glRotatef(bodyAngle, 0, 0, 1);
        glTranslatef(-radius/2, 0, 0); 
        DrawRect(rectHeight, rectWidth, 0.0f, 0.0f, 0.0f); // perna direita na frente
        glTranslatef(radius, 0, 0); 
        DrawRect(-rectHeight, rectWidth, 0.0f, 0.0f, 0.0f); // perna esquerda atras
        glPopMatrix();
    }

    // Desenha a arma
    glPushMatrix();
    glTranslatef(gX, gY, 0); 
    glRotatef(bodyAngle, 0, 0, 1);
    glTranslatef(radius*2 - radius *0.5, 0, 0); 
    glRotatef(armAngle, 0.0f, 0.0f, 1.0f); // aplica o ângulo da arma
    DrawRect(rectHeight, rectWidth, R, G, B); //desenha a arma
    glPopMatrix();

    // Desenha a cabeça e o corpo
    glPushMatrix();
    glTranslatef(gX, gY, 0);
    glRotatef(bodyAngle, 0, 0, 1);
    DrawElipse(radius * 2, R, G, B, false); // desenha o corpo
    DrawElipse(radius, R, G, B, true); // desenha a cabeça
    glPopMatrix();

}

void Player::SetRotation(GLfloat angle)
{
    while (angle < 0.0f) angle += 360.0f;
    while (angle >= 360.0f) angle -= 360.0f;

    float delta = angle - bodyAngle;
    if (delta > 180.0f) delta -= 360.0f;
    else if (delta <= -180.0f) delta += 360.0f;

    this->pos.Rotate(delta);
    bodyAngle = angle;

    while (bodyAngle < 0.0f) bodyAngle += 360.0f;
    while (bodyAngle >= 360.0f) bodyAngle -= 360.0f;
}

void Player::Rotate(GLfloat inc, GLdouble tDif)
{
    bodyAngle += inc;
    if(bodyAngle >= 360) bodyAngle = 0;
    else if(bodyAngle < 0) bodyAngle = 360;

    this->pos.Rotate(inc);
    this->CheckFootChange(tDif);
}

void Player::ResetPosition()
{
    this->pos.Replace(Alglib::Mat2::Identity());
    pos.Translate(this->gXinit, this->gYinit);

    this->bodyAngle = 0.0f;
    this->armAngle = 0.0f;
    this->footWalking = Foot::RightFront;
    this->isWalking = false;
    this->lastWalkTime = 0.0;

    this->SetArmAngle(0.0f); 

    this->collider.SetCircle(this->GetXPos(), this->GetYPos(), (float)this->radius, epsilon);
}

void Player::Move(GLfloat inc, GLdouble tDif)
{
    Alglib::Mat2 tmp = this->pos.Copy();
    tmp.Translate(0, inc);

    GLfloat newX = tmp.m[0][2];
    GLfloat newY = tmp.m[1][2];

    Collider proposed;

    proposed.SetParameters(newX, newY, (float)this->radius, epsilon, CollisionType::External, NULL);

    if (gCollisionSystem->TestCollision(proposed, &this->collider)) {
        return;
    }

    this->pos.Translate(0, inc);
    this->collider.SetCircle(newX, newY, (float)this->radius, epsilon);

    this->CheckFootChange(tDif);
}

Bullet* Player::Shoot()
{
    Alglib::Mat2 shootPos = Alglib::Mat2::Identity();

    shootPos.Translate(GetXPos(), GetYPos())
            .Rotate(this->bodyAngle)
            .Translate(this->radius*2 - this->radius *0.5, 0)
            .Rotate(this->armAngle);

    Alglib::Tuple2 bulletPos(1.0f);
    Alglib::Tuple2 armPos(1.0f);


    armPos.Alglib::Tuple2::Transform(shootPos);

    shootPos.Translate(0, rectHeight);

    bulletPos.Alglib::Tuple2::Transform(shootPos);

    Alglib::Tuple2 directionVector = Alglib::Subtract(bulletPos, armPos);

    return new Bullet(bulletPos, directionVector);
}