#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <stdlib.h>
#include <math.h>
#include <iostream>
#include "tinyxml2.h"
#include "arena.h"
#include "gameManager.h"
#include "player.h"
#include "hud.h"
#include "collisionSystem.h"
#include "collider.h"

using namespace tinyxml2;

#define WINDOW_SIZE 500
#define GUN_ANGLE 45
#define LINEAR_SPEED 150.0
#define ANGULAR_SPEED 100.0


// Window dimensions
const GLint Width = WINDOW_SIZE;
const GLint Height = WINDOW_SIZE;

// Viewing dimensions
GLint ViewingWidth;
GLint ViewingHeight;

GameManager gameManager;
GameManager* refGameManager = nullptr;

Arena arena;

Player player1;
Player player2;

CollisionSystem collisionSystem;
CollisionSystem* gCollisionSystem = nullptr; // ponteiro inicializado em main()

Bullet* bulletP1 = NULL; // apenas uma bala por vez do player 1
Bullet* bulletP2 = NULL; // apenas uma bala por vez do player 2

HUD hud(Width, Height);

static char str[1000];
void * font = GLUT_BITMAP_9_BY_15;

int keyStatus[256];

// debug function
void DrawAxes(double size)
{
    GLfloat mat_ambient_r[] = { 1.0, 0.0, 0.0, 1.0 };
    GLfloat mat_ambient_g[] = { 0.0, 1.0, 0.0, 1.0 };
    GLfloat mat_ambient_b[] = { 0.0, 0.0, 1.0, 1.0 };
    GLfloat no_mat[] = { 0.0, 0.0, 0.0, 1.0 };
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, 
            no_mat);
    glMaterialfv(GL_FRONT, GL_SPECULAR, no_mat);
    glMaterialfv(GL_FRONT, GL_SHININESS, no_mat);

    //x axis red
    glPushMatrix();
        glMaterialfv(GL_FRONT, GL_EMISSION, 
                mat_ambient_r);
        glColor3fv(mat_ambient_r);
        glScalef(size, size*0.1, size*0.1);
        glTranslatef(0.5, 0, 0); // put in one end
        glutSolidCube(1.0);
    glPopMatrix();

    //y axis green
    glPushMatrix();
        glMaterialfv(GL_FRONT, GL_EMISSION, 
                mat_ambient_g);
        glColor3fv(mat_ambient_g);
        glRotatef(90,0,0,1);
        glScalef(size, size*0.1, size*0.1);
        glTranslatef(0.5, 0, 0); // put in one end
        glutSolidCube(1.0);
    glPopMatrix();

    //z axis blue
    glPushMatrix();
        glMaterialfv(GL_FRONT, GL_EMISSION, mat_ambient_b);
        glColor3fv(mat_ambient_b);
        glRotatef(-90,0,1,0);
        glScalef(size, size*0.1, size*0.1);
        glTranslatef(0.5, 0, 0); // put in one end
        glutSolidCube(1.0);
    glPopMatrix();    
}

void renderScene(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // Use the same fovy used in init() and compute a camera distance that
    // fits the whole "ViewingHeight" in view (minimal perspective distortion).
    const float fovy_deg = 30.0f; // must match init()'s fovy
    const float fovy_rad = fovy_deg * (float)M_PI / 180.0f;
    const float halfViewHeight = (float)ViewingHeight * 0.5f;

    // distance so that the vertical span at z=0 equals ViewingHeight
    float eyeDist = (halfViewHeight / tanf(fovy_rad * 0.5f));

    // small margin so edges are not clipped
    const float margin = 1.05f;
    eyeDist *= margin;

    // DEBUG: VISTA ANGULADA
    // position camera on +Y looking to origin, Z is up
    // gluLookAt(300.0f, eyeDist * 0.2, -700.0f,   // eye
    //           0.0f, 0.0f, 0.0f,      // center
    //           0.0f, 1.0f, 0.0f);     // up = Z


    // DEBUG: VISTA DE CIMA 
    gluLookAt(0, 1100, 0,   // eye
              0.0f, 0.0f, 0.0f,      // center
              0.0f, 0.0f, 1.0f);     // up = Z

    // update light position to follow camera (optional but usually desirable)
    GLfloat light_position[] = { 0.0f, eyeDist, 0.0f, 1.0f };
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);

    DrawAxes(70); // DEBUG
    arena.Draw();
    player2.Draw();
    player1.Draw();

    glutSwapBuffers();
}


void keyUp(unsigned char key, int x, int y)
{
    keyStatus[(int)(key)] = 0;
    glutPostRedisplay();
}

void ResetKeyStatus()
{
    int i;
    for(i = 0; i < 256; i++)
       keyStatus[i] = 0; 
}


void restartGame()
{

}

void keyPress(unsigned char key, int x, int y)
{   

}

void idle(void)
{

}
 
void init(void)
{
    glClearColor (0.0, 0.0, 0.0, 0.0);
    glShadeModel (GL_SMOOTH);
    glEnable(GL_CULL_FACE);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_DEPTH_TEST);

    ResetKeyStatus();

    // Projeção em perspectiva com FOV menor (menos distorção)
    float fovy = 30.0f;                      // experimente 20..35
    float aspect = (GLfloat)Width / (GLfloat)Height;
    float nearv = 1.0f;
    float farv  = 2000.0f;

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(fovy, aspect, nearv, farv);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void readSVG(const char *fileName)
{
    XMLDocument doc;
    XMLError eResult = doc.LoadFile(fileName);

    if (eResult != XML_SUCCESS) {
        std::cerr << "Error loading file: " << eResult << std::endl;
        return;
    }

    // Cabeçalho
    XMLNode* root = doc.FirstChild();

    // Primeiro elemento <svg>
    XMLElement* svg = doc.FirstChildElement("svg");
    if (!svg) {
        std::cerr << "Elemento <svg> não encontrado\n";
        return;
    }

    // Read key elements from svg 
    XMLElement* c1 = svg->FirstChildElement("circle");
    XMLElement* c2 = c1 ? c1->NextSiblingElement("circle") : nullptr;
    XMLElement* c3 = c2 ? c2->NextSiblingElement("circle") : nullptr;

    GLfloat arenaCx, arenaCy, arenaR, height;
    if (c1) {
        arenaCx = c1->IntAttribute("cx", 0);
        arenaCy = c1->IntAttribute("cy", 0);
        arenaR  = c1->IntAttribute("r",  0);

        arena.SetParameters(0, 0, arenaR);

        ViewingHeight = ViewingWidth = 2 * arenaR; // MUDAR AQUI 
    }
    if (c2) {
        GLfloat cx = c2->IntAttribute("cx", 0);
        GLfloat cy = c2->IntAttribute("cy", 0);
        GLfloat r  = c2->IntAttribute("r",  0);

        player2.SetParameters(arenaCx - cx, - cy + arenaCy, r, [](){ gameManager.decLifePlayer2(); });
        player2.DefineColor(1.0f, 0, 0); // Player 2 is red
    }
    if (c3) {
        GLfloat cx = c3->IntAttribute("cx", 0);
        GLfloat cy = c3->IntAttribute("cy", 0);
        GLfloat r  = c3->IntAttribute("r",  0);

        height = 6*r;
        
        player1.SetParameters(arenaCx - cx, - cy + arenaCy, r, [](){ gameManager.decLifePlayer1(); });
        player1.DefineColor(0, 1.0f, 0); // Player 1 is green
    }

    int obstacleCount = 0;
    for (XMLElement* c = c3 ? c3->NextSiblingElement("circle") : nullptr; c; c = c->NextSiblingElement("circle")) ++obstacleCount;
    arena.GetObstacleVector().reserve(obstacleCount); 

    for (XMLElement* c = c3 ? c3->NextSiblingElement("circle") : nullptr; c; c = c->NextSiblingElement("circle")) {
        int cx = c->IntAttribute("cx", 0);
        int cy = c->IntAttribute("cy", 0);
        int r  = c->IntAttribute("r",  0);
        
        arena.AddObstacle(arenaCx - cx, - cy + arenaCy, r, 2*height);
    }
}

int main(int argc, char *argv[])
{
    readSVG(argv[1]);
    
    // Initialize openGL with Double buffer and RGB color without transparency.
    // Its interesting to try GLUT_SINGLE instead of GLUT_DOUBLE.
    glutInit(&argc, argv);
    glutInitDisplayMode (GLUT_DOUBLE | 
                        GLUT_RGB | 
                        GLUT_DEPTH);

    // Create the window.
    glutInitWindowSize(Width, Height);
    glutInitWindowPosition(150,50);
    glutCreateWindow("Arena");
 
    // Define callbacks.
    glutDisplayFunc(renderScene);
    glutIdleFunc(idle);
    
    init();

    glutMainLoop();

    return 0;
}
