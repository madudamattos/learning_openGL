#include "player.h"


void Player::SetParameters(GLint x, GLint y, GLint rad){
    this->gXinit = x;
    this->gYinit = y;
    this->radius = rad;
    this->gX = x;
    this->gY = y;
}

void Player::DefineColor(GLfloat R, GLfloat G, GLfloat B){
    this->R = R;
    this->G = G;
    this->B = B;
}

void Player::Draw()
{
    // Draw the main circle
    glPushMatrix();
    glScalef(1.0f, -1.0f, 1.0f);
    glTranslatef(gX, gY, 0);
    DrawCircle(radius, R, G, B);
    glPopMatrix();
}

void Player::DrawCircle(GLint radius, GLfloat R, GLfloat G, GLfloat B)
{
    glColor3f(R, G, B); 
    glBegin(GL_POLYGON);
        for(int deg=0; deg<360; deg+=20)
        {
            float theta = deg* M_PI / 180.0f; // deg->rad
            float x = radius * cos(theta);
            float y = radius * sin(theta);

            glVertex3f(x, -y, 0.0f); 
        }
    glEnd();
}

void Player::DrawRect(GLint height, GLint width, GLfloat R, GLfloat G, GLfloat B)
{

}

