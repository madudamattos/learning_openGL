#include "arena.h"

extern CollisionSystem* gCollisionSystem;

void Arena::SetParameters(GLfloat x, GLfloat y, GLfloat r)
{
    this->gX = x;
    this->gY = y;
    this->radius = r;

    // collider.SetParameters(x,y, r, 0.0f, CollisionType::Internal, NULL);
}

void Arena::AddObstacle(GLfloat x, GLfloat y, GLfloat r)
{
    obstacles.emplace_back();           
    Obstacle &dst = obstacles.back();   

    dst.x = x;
    dst.y = y;
    dst.r = r;

    // dst.obstacleCollider.SetParameters(x, y, r, 0.0f, CollisionType::External, Collider::Callback());
}

void Arena::Clear()
{
    Arena::ClearObstacles(true);
}

void Arena::ClearObstacles(bool freeMemory = false){
    obstacles.clear(); // destrói elementos -> size() == 0
    if (freeMemory) {
        obstacles.shrink_to_fit(); // tenta liberar o buffer interno (capacity -> size)
    }
}

int Arena::ObstacleCount()
{
    return static_cast<int>(obstacles.size());
}

std::vector<Obstacle> Arena::GetObstacles(){
    return obstacles;
}

void Arena::DrawCircle(GLint radius, GLfloat R, GLfloat G, GLfloat B)
{
    glColor3f(R, G, B); 
    glBegin(GL_POLYGON);
        for(int deg=0; deg<360; deg+=12)
        {
            float theta = deg* M_PI / 180.0f; // deg->rad
            float x = radius * cos(theta);
            float y = radius * sin(theta);

            glVertex3f(x, y, 0.0f); 
        }
    glEnd();
}

void Arena::Draw()
{
    // Draw the main circle
    glPushMatrix();
    glTranslatef(gX, gY, 0);
    DrawCircle(radius, 0, 0, 1.0f);
    glPopMatrix();
    
    int obsCount = ObstacleCount();
    
    // Draw individual obstacles
    for (const auto &obs : obstacles) {
        glPushMatrix();
        glTranslatef(obs.x, obs.y, 0.0f);
        DrawCircle(obs.r, 0.0f, 0.0f, 0.0f);
        glPopMatrix();
    }

}