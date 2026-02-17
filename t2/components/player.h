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
#include "../utils/material.h"

#define rectWidth 10
#define rectHeight 40

class Player
{
    GLint gXinit, gYinit, gZinit, radius;
    Material material;
    GLfloat bodyAngle;
    Alglib::Tuple3 armAngle;
    Alglib::Mat3 pos, gunPos; 
    bool isWalking;
    GLdouble lastWalkTime;
    Collider collider;

    public:
        using PlayerCallback = std::function<void(Player*)>;
    private:
        PlayerCallback callback;

    public:
        enum class Foot { LeftFront = 0, RightFront = 1 };

    private:
        Foot footWalking; 

    public:
        Player();

        void SetParameters(GLfloat x, GLfloat y, GLfloat z, GLfloat rad, std::function<void()> onCollision);

        void DefineColor(GLfloat R, GLfloat G, GLfloat B);

        void Draw();

        void SetIsWalking(bool value) { isWalking = value; }
        bool GetIsWalking() const { return isWalking; }

        void Rotate(GLfloat inc, GLdouble tDif);
        GLfloat GetBodyRotation() { return bodyAngle; }
        void SetRotation(GLfloat angle);

        void Move(GLfloat inc, GLdouble tDif); 
        
        void SetArmAngle(GLfloat degX, GLfloat degY);
        void IncreaseArmAngle(GLfloat incX, GLfloat degY);

        Alglib::Tuple3 GetArmAngle() const { return armAngle; }
        Alglib::Mat3 GetGunPos();

        GLfloat GetYPos();
        GLfloat GetXPos();
        GLfloat GetZPos();

        void ResetPosition();
        
        Bullet* Shoot();

    private:
        void DrawRect(GLfloat sx, GLfloat sy, GLfloat sz);
        void DrawSphere(GLfloat r);
        void DrawAxes(double size);
};

#endif