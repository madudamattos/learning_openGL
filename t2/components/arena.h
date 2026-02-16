#ifndef ARENA_H
#define ARENA_H

#include <GL/gl.h>
#include <vector>
#include <cmath>
#include <iostream>
#include "collisionSystem.h"
#include "collider.h"

struct Obstacle
{
    GLfloat x, y, r;
    GLfloat height;
    Collider obstacleCollider;

    Obstacle() : x(0), y(0), r(0) {}
    Obstacle(GLfloat xx, GLfloat yy, GLfloat rr, GLfloat h = 0.0f)
        : x(xx), y(yy), r(rr), height(h)
    {
        obstacleCollider.SetParameters(x, y, r, 0.0f, CollisionType::External, Collider::Callback());
    }
};

class Arena
{
    GLfloat gX, gY, radius;
    std::vector<Obstacle> obstacles; 
    Collider collider;

    public:
        Arena(){
            this->gX = 0.0f;
            this->gY = 0.0f;
            this->radius = 0.0f;
        }

        void SetParameters(GLfloat x, GLfloat y, GLfloat radius);

    // Adiciona um obstáculo dinamicamente ao vetor obstaculos (agora com height)
    void AddObstacle(GLfloat x, GLfloat y, GLfloat r, GLfloat height);

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
        
        // Remove todos os obstáculos e desaloca memória
        void ClearObstacles(bool freeMemory);
};

#endif