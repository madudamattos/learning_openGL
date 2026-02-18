#ifndef ARENA_H
#define ARENA_H

#include <GL/gl.h>
#include <vector>
#include <cmath>
#include <iostream>
#include "collisionSystem.h"
#include "collider.h"
#include "material.h"

struct Obstacle
{
    GLfloat x, y, z, r, h;
    Collider obstacleCollider;

    Obstacle() : x(0), y(0), z(0), r(0), h(0) {}
    Obstacle(GLfloat xx, GLfloat yy, GLfloat zz, GLfloat rr, GLfloat hh = 0.0f)
        : x(xx), y(yy), r(rr), h(h)
    {
        obstacleCollider.SetParameters(x, y, z, r, h, 0.0f, CollisionType::External, Collider::Callback());
    }
};

class Arena
{
    GLfloat gX, gY, gZ, radius, height;
    std::vector<Obstacle> obstacles; 
    Collider collider;

    public:
        Arena(){
            this->gX = 0.0f;
            this->gY = 0.0f;
            this->gZ = 0.0f;
            this->radius = 0.0f;
            this->height = 0.0f;
        }

        void SetParameters(GLfloat x, GLfloat y, GLfloat z, GLfloat radius, GLfloat height);
        void SetHeight(GLfloat height) { this->height = height; };

    // Adiciona um obstáculo dinamicamente ao vetor obstaculos 
    void AddObstacle(GLfloat x, GLfloat y, GLfloat z, GLfloat r, GLfloat height);

        std::vector<Obstacle>& GetObstacleVector() { return obstacles; }

        // Retorna a quantidade de obstáculos existente
        int ObstacleCount();
        
        // Return obstacles vector
        std::vector<Obstacle> GetObstacles();

        // Desenha a arena
        void Draw();

        // Libera memória da arena
        void Clear();
        
    private:
        void DrawCircle(GLfloat rad, GLfloat R, GLfloat G, GLfloat B);
        void DrawSphere(GLfloat radius, GLfloat R, GLfloat G, GLfloat B);
        void DrawCylinder(GLfloat radius, GLfloat height, bool insideView);

        void DrawObstacle(GLfloat radius, GLfloat height);
        
        // Remove todos os obstáculos e desaloca memória
        void ClearObstacles(bool freeMemory);
};

#endif