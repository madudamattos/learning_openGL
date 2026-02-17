#ifndef TIRO_H
#define	TIRO_H
#include <GL/gl.h>
#include <GL/glu.h>
#include "alglib.h"
#include "collider.h"
#include "collisionSystem.h"
#include "material.h"
#include <functional>

#define SPEED 300.f

extern class CollisionSystem* gCollisionSystem;

class Bullet {
public:
    using DestroyCallback = std::function<void(Bullet*)>;
    void SetDestroyCallback(DestroyCallback cb) { onDestroy = cb; }

    Bullet(Alglib::Tuple3 position, Alglib::Tuple3 directionVector, Material material = Material(1.0f, 1.0f, 1.0f));

    void Draw();

    void Move(GLdouble timeDiff);
    
    bool isValid();

    GLfloat GetXPos() { return currentPos.GetX();}
    GLfloat GetYPos() { return currentPos.GetY();}
    GLfloat GetZPos() { return currentPos.GetZ();}
    GLfloat GetWPos() { return currentPos.GetW();}

    void DestroySelf();

private:
    Material material;
    Alglib::Tuple3 initPos; // ponto inicial
    Alglib::Tuple3 currentPos;
    Alglib::Tuple3 directionAng;
    GLfloat gVel;
    Collider col;
    DestroyCallback onDestroy;

    void DrawSphere(GLfloat radius);
};

#endif	/* TIRO_H */

