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

using namespace tinyxml2;

// Window dimensions
const GLint Width = 500;
const GLint Height = 500;

// Viewing dimensions
GLint ViewingWidth = 500;
GLint ViewingHeight = 500;

// Controla a animacao do player
int animate = 0;

//Componentes do mundo virtual sendo modelado
GameManager gameManager;
Arena arena;
Player player1;
Player player2;

int atingido = 0;
static char str[1000];
void * font = GLUT_BITMAP_9_BY_15;

void renderScene(void)
{
    // Clear the screen.
    glClear(GL_COLOR_BUFFER_BIT);

    arena.Draw();

    player1.Draw();

    player2.Draw();

    glutSwapBuffers(); // Desenha the new frame of the game.
}

void keyPress(unsigned char key, int x, int y)
{
    // switch (key)
    // {
    //     case '1':
    //          animate = !animate;
    //          break;
    //     case 'a':
    //     case 'A':
    //          keyStatus[(int)('a')] = 1; //Using keyStatus trick
    //          break;
    //     case 'd':
    //     case ' ':
    //          if (!tiro)
    //             tiro = robo.Atira();
    //          break;
    //     case 27 :
    //          exit(0);
    // }
    glutPostRedisplay();
}

void keyup(unsigned char key, int x, int y)
{
    // keyStatus[(int)(key)] = 0;
    glutPostRedisplay();
}

void ResetKeyStatus()
{

}

void init(void)
{
    ResetKeyStatus();
    // The color the windows will redraw. Its done to erase the previous frame.
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Black, no opacity(alpha).
 
    glMatrixMode(GL_PROJECTION); // Select the projection matrix    
    glOrtho(-(ViewingWidth/2),     // X coordinate of left edge             
            (ViewingWidth/2),     // X coordinate of right edge            
            -(ViewingHeight/2),     // Y coordinate of bottom edge             
            (ViewingHeight/2),     // Y coordinate of top edge             
            -100,     // Z coordinate of the “near” plane            
            100);    // Z coordinate of the “far” plane
    glMatrixMode(GL_MODELVIEW); // Select the projection matrix    
    glLoadIdentity();
      
}

void idle(void)
{
    // blabla
    
    glutPostRedisplay();
}
 
void ReadSVG(const char *fileName)
{
    XMLDocument doc;
    XMLError eResult = doc.LoadFile(fileName);

    if (eResult != XML_SUCCESS) {
        std::cerr << "Error loading file: " << eResult << std::endl;
        return;
    }

    // Cabeçalho
    XMLNode* root = doc.FirstChild();
    if (root) {
        std::cout << "Primeiro nó: " << root->Value() << "\n";
    }

    // Primeiro elemento <svg>
    XMLElement* svg = doc.FirstChildElement("svg");
    if (!svg) {
        std::cerr << "Elemento <svg> não encontrado\n";
        return;
    }
    std::cout << "Elemento raiz: " << svg->Name() << "\n";

    // Read key elements from svg 
    XMLElement* c1 = svg->FirstChildElement("circle");
    XMLElement* c2 = c1 ? c1->NextSiblingElement("circle") : nullptr;
    XMLElement* c3 = c2 ? c2->NextSiblingElement("circle") : nullptr;

    GLfloat arenaCx, arenaCy, arenaR;
    if (c1) {
        arenaCx = c1->IntAttribute("cx", 0);
        arenaCy = c1->IntAttribute("cy", 0);
        arenaR  = c1->IntAttribute("r",  0);

        arena.SetParameters(0, 0, arenaR);

        ViewingHeight = ViewingWidth = 2 * arenaR;
    }
    if (c2) {
        GLfloat cx = c2->IntAttribute("cx", 0);
        GLfloat cy = c2->IntAttribute("cy", 0);
        GLfloat r  = c2->IntAttribute("r",  0);

        player2.SetParameters(cx - arenaCx, cy - arenaCy, r);
        player2.DefineColor(1.0f, 0, 0); // Player 2 is red
    }
    if (c3) {
        GLfloat cx = c3->IntAttribute("cx", 0);
        GLfloat cy = c3->IntAttribute("cy", 0);
        GLfloat r  = c3->IntAttribute("r",  0);

        player1.SetParameters(cx - arenaCx, cy - arenaCy, r);
        player1.DefineColor(0, 1.0f, 0); // Player 1 is green
    }

    // Add obstacles to the arena if they exist
    for (XMLElement* c = c3 ? c3->NextSiblingElement("circle") : nullptr; c; c = c->NextSiblingElement("circle")) {
        int cx = c->IntAttribute("cx", 0);
        int cy = c->IntAttribute("cy", 0);
        int r  = c->IntAttribute("r",  0);
        
        arena.AddObstacle(cx - arenaCx, cy - arenaCy, r);
    }
}

int main(int argc, char *argv[])
{
    ReadSVG(argv[1]);
    
    // Initialize openGL with Double buffer and RGB color without transparency.
    // Its interesting to try GLUT_SINGLE instead of GLUT_DOUBLE.
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);

    // Create the window.
    glutInitWindowSize(Width, Height);
    glutInitWindowPosition(150,50);
    glutCreateWindow("Arena");
 
    // Define callbacks.
    glutDisplayFunc(renderScene);
    glutIdleFunc(idle);
    // glutKeyboardFunc(keyPress);
    // glutKeyboardUpFunc(keyup);
    
    init();
 
    glutMainLoop();

    return 0;
}
