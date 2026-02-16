#include "arena.h"
#include <GL/glut.h>

extern CollisionSystem* gCollisionSystem;

void Arena::SetParameters(GLfloat x, GLfloat y, GLfloat r)
{
    this->gX = x;
    this->gY = y;
    this->radius = r;

    // collider.SetParameters(x,y, r, 0.0f, CollisionType::Internal, NULL);
}

void Arena::AddObstacle(GLfloat x, GLfloat y, GLfloat r, GLfloat height)
{
    obstacles.emplace_back();
    Obstacle &dst = obstacles.back();

    dst.x = x;
    dst.y = y;
    dst.r = r;
    dst.height = height;
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

void Arena::DrawCircle(GLfloat radius, GLfloat R, GLfloat G, GLfloat B)
{
    int segments = 30;

    // salvar estado de cull (simples)
    GLboolean wasCull = glIsEnabled(GL_CULL_FACE);

    // evitar que o disco seja escondido por culling de winding errado
    glDisable(GL_CULL_FACE);

    // material um pouco mais forte para ficar visível com iluminação
    GLfloat materialEmission[] = { 0.10f * R, 0.10f * G, 0.10f * B, 1.0f };
    GLfloat materialAmbient[]  = { 0.40f * R, 0.40f * G, 0.40f * B, 1.0f };
    GLfloat materialDiffuse[]  = { 0.50f * R, 0.50f * G, 0.50f * B, 1.0f };
    GLfloat mat_specular[]     = { 0.0f, 0.0f, 0.0f, 1.0f }; // sem brilho
    GLfloat mat_shininess[]    = { 0.0f };

    // Aplicar ao front e back evita variações por winding
    glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, materialEmission);
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, materialAmbient);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, materialDiffuse);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, mat_shininess);

    glBegin(GL_TRIANGLE_FAN);
        // para chão no plano XZ a normal deve apontar para +Y (câmera no eixo Y)
        glNormal3f(0.0f, 1.0f, 0.0f);

        glVertex3f(0.0f, 0.0f, 0.0f);

        for (int i = 0; i <= segments; ++i)
        {
            float angle = 2.0f * M_PI * i / segments;
            float x = radius * cosf(angle);
            float z = radius * sinf(angle);

            glVertex3f(x, 0.0f, z);
        }

    glEnd();

    // restaurar estado de cull
    if (wasCull) glEnable(GL_CULL_FACE);
}

void Arena::DrawSphere(GLfloat radius, GLfloat R, GLfloat G, GLfloat B)
{
    GLfloat materialEmission[] = { 0.00, 0.00, 0.00, 1.0};
    GLfloat materialColor[] = { R, G, B, 1.0};
    GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0};
    GLfloat mat_shininess[] = { 30 };
    glMaterialfv(GL_FRONT, GL_EMISSION, materialEmission);
    glMaterialfv(GL_FRONT, GL_AMBIENT, materialColor);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, materialColor);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
    glColor3f(1,0,0);
    glutSolidSphere((GLdouble) radius, 20, 10);
}

void Arena::Draw()
{
    // Desenha o circulo do chão da arena 
    glPushMatrix();
    glTranslatef(gX, 0, gY);
    DrawCircle(radius, 0.0f, 0.0f, 1.0f); // arena é azul
    glPopMatrix();
    
    int obsCount = ObstacleCount();
    
    // Draw individual obstacles
    for (const auto &obs : obstacles) {
        glPushMatrix();
        glTranslatef(obs.x, 0.0f, obs.y);
        DrawSphere(obs.r, 0.1f,0.1f,0.2f);
        glPopMatrix();
    }

}