#ifndef PLAYER_H
#define PLAYER_H

#include <GL/gl.h>
#include <iostream>
#include <math.h>

class Player
{
    GLint gXinit, gYinit, radius;
    GLint gX, gY;
    GLfloat R, G, B;
    // Armazena cor

    public:
        Player(){
            this->gXinit = 0.0f;
            this->gYinit = 0.0f;
            this->radius = 0.0f;
            this->gX = 0.0f;
            this->gY = 0.0f;
            this->R = 0.0f;
            this->G = 0.0f;
            this->B = 0.0f;
        }

        void SetParameters(GLint x, GLint y, GLint rad);

        void DefineColor(GLfloat R, GLfloat G, GLfloat B);
        
        void DebugPrint(const char* tag) const {
            std::cout << "[DEBUG] " << tag
                      << " addr=" << this
                      << " gX=" << gX << " gY=" << gY
                      << " radius=" << radius
                      << " color=(" << R << "," << G << "," << B << ")\n";
        }
        void Draw();
        void MovePlayer(GLint x, GLint y);
        
    private:
        void DrawCircle(GLint radius, GLfloat R, GLfloat G, GLfloat B);
        void DrawRect(GLint height, GLint width, GLfloat R, GLfloat G, GLfloat B);
};

#endif