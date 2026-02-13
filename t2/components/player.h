#ifndef PLAYER_H
#define PLAYER_H

#include <GL/gl.h>
#include <iostream>
#include <math.h>
#include <functional>        
#include "alglib.h"
#include "bullet.h"
#include "collider.h"
#include "collisionSystem.h" 

#define rectWidth 10
#define rectHeight 40

class Player
{
    GLint gXinit, gYinit, radius;
    GLfloat R, G, B;
    GLfloat armAngle, bodyAngle;
    Alglib::Mat2 pos, gunPos; 
    bool isWalking;
    GLdouble lastWalkTime;
    Collider collider;

    public:
        using PlayerCallback = std::function<void(Player*)>;
    private:
        PlayerCallback callback;

    public:
        enum class Foot { LeftFront = 0, RightFront = 1 };
        void SetPlayerCallback(PlayerCallback cb);

    private:
        Foot footWalking; 

    public:
        Player();

        void SetParameters(GLint x, GLint y, GLint rad, std::function<void()> onCollision);

        void DefineColor(GLfloat R, GLfloat G, GLfloat B);

        void Draw();

        void SetFootWalking(Foot value);
        Foot GetFootWalking() const { return footWalking; }

        void SetWalkTime(GLdouble t) { lastWalkTime = t; };
        void CheckFootChange(GLdouble t);

        void SetIsWalking(bool value) { isWalking = value; }
        bool GetIsWalking() const { return isWalking; }

        void SetArmAngle(GLfloat degrees);
        void IncreaseArmAngle(GLfloat inc);
        GLfloat GetArmAngle() const { return armAngle; }
        
        void Rotate(GLfloat inc, GLdouble tDif);
        GLfloat GetBodyRotation() { return bodyAngle; }

        void SetRotation(GLfloat angle);

        void Move(GLfloat inc, GLdouble tDif); 
        
        GLfloat GetYPos();
        GLfloat GetXPos();

        void ResetPosition();
        
        Bullet* Shoot();

    private:
        void DrawElipse(GLint radius, GLfloat R, GLfloat G, GLfloat B, bool isCircle);
        void DrawRect(GLint height, GLint width, GLfloat R, GLfloat G, GLfloat B);
};

#endif