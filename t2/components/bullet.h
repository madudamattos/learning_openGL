#ifndef TIRO_H
#define	TIRO_H
#include <GL/gl.h>
#include <GL/glu.h>
#include "alglib.h"
#include "collider.h"
#include "collisionSystem.h"
#include <functional>

#define SPEED 300.f

extern class CollisionSystem* gCollisionSystem;

class Bullet {
public:
    using DestroyCallback = std::function<void(Bullet*)>;
    void SetDestroyCallback(DestroyCallback cb) { onDestroy = cb; }

    Bullet(Alglib::Tuple2 position, Alglib::Tuple2 directionVector);

    void Draw();

    void Move(GLdouble timeDiff);
    
    bool isValid();

    GLfloat GetXPos() { return currentPos.GetX();}
    GLfloat GetYPos() { return currentPos.GetY();}
    GLfloat GetWPos() { return currentPos.GetW();}

    void DestroySelf();

private:
    Alglib::Tuple2 initPos; // ponto inicial
    Alglib::Tuple2 currentPos;
    Alglib::Tuple2 directionAng;
    GLfloat gVel;
    Collider col;
    DestroyCallback onDestroy;

    void DrawCircle(GLint radius, GLfloat R, GLfloat G, GLfloat B);
};

#endif	/* TIRO_H */

