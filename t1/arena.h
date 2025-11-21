#ifndef ARENA_H
#define ARENA_H

#include <GL/gl.h>
#include <vector>
#include <cmath>
#include <iostream>

// Lembrar de desalocar memória ao liberar a arena !! 

struct Obstacle
{
    GLfloat x, y, r;
};

class Arena
{
    GLfloat gX, gY, radius;
    std::vector<Obstacle> obstacles; 

    public:
        Arena(){
            this->gX = 0.0f;
            this->gY = 0.0f;
            this->radius = 0.0f;
        }

        void SetParameters(GLfloat x, GLfloat y, GLfloat radius);
        
        void DebugPrint() const {
            std::cout << "[DEBUG] Arena addr=" << this
                      << " center=(" << gX << "," << gY << ")"
                      << " radius=" << radius
                      << " obstacles=" << obstacles.size() << "\n";
        }

        // Adiciona um obstáculo dinamicamente ao vetor obstaculos
        void AddObstacle(GLfloat x, GLfloat y, GLfloat r);

        // Retorna a quantidade de obstáculos existente
        int ObstacleCount();
        
        // Return obstacles vector
        std::vector<Obstacle> GetObstacles();

        // Desenha a arena
        void Draw();

        // Libera memória da arena
        void Clear();

    private:
        void DrawCircle(GLint rad, GLfloat R, GLfloat G, GLfloat B);
        
        // Remove todos os obstáculos e desaloca memória
        void ClearObstacles(bool freeMemory);
};

#endif