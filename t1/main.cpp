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

int uid = 1000;
std::mutex uidMutex; 

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

void renderScene(void)
{
    glClear(GL_COLOR_BUFFER_BIT);

    arena.Draw();

    player2.Draw();

    player1.Draw();

    hud.Draw();

    if(bulletP1) bulletP1->Draw();

    if(bulletP2) bulletP2->Draw();

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

void setPlayersInicialRotation()
{
    // coloca os personagens na posição certa
    Alglib::Tuple2 p1(player1.GetXPos(), player1.GetYPos(), 1.0f);
    Alglib::Tuple2 p2(player2.GetXPos(), player2.GetYPos(), 1.0f);

    Alglib::Tuple2 v1 = Alglib::Subtract(p2, p1); // aponta de p1 para p2
    Alglib::Tuple2 v2 = Alglib::Subtract(p1, p2); // aponta de p2 para p1

    double angP1 = std::atan2((double)-v1.y, (double)v1.x) * 180.0 / M_PI;
    double angP2 = std::atan2((double)-v2.y, (double)v2.x) * 180.0 / M_PI;

    const double orientationOffset = 90.0; 
    angP1 += orientationOffset;
    angP2 += orientationOffset;

    player1.SetRotation((GLfloat)-angP1);

    player2.SetRotation((GLfloat)-angP2);

}

void restartGame()
{
    // reseta os status no gameManager
    gameManager.resetGame();

    // deleta as balas
    if(bulletP1) {
        delete bulletP1;
        bulletP1 = nullptr; 
    }

    if(bulletP2) {
        delete bulletP2;
        bulletP2 = nullptr; 
    }

    // // reseta os jogadores para a posição inicial 
    player1.ResetPosition();
    player2.ResetPosition();

    setPlayersInicialRotation();
}

void keyPress(unsigned char key, int x, int y)
{   
    if(key == 'r' || key == 'R')
        restartGame();
    
    if(gameManager.isGameOver()) return; 

    unsigned int code = static_cast<unsigned char>(key); 
    switch (code) {
        // movimentação player1 (usar keyStatus + idle)
        case 'a': case 'A':
            keyStatus[(int)('a')] = 1;
            break;
        case 'd': case 'D':
            keyStatus[(int)('d')] = 1;
            break;
        case 'w': case 'W':
            keyStatus[(int)('w')] = 1;
            break;
        case 's': case 'S':
            keyStatus[(int)('s')] = 1;
            break;
        // gun rotation
        case '4':
            keyStatus[(int)('4')] = 1;
            break;
        case '6':
            keyStatus[(int)('6')] = 1;
            break;
        // player2 
        case 'o': case 'O':
            keyStatus[(int)('o')] = 1;
            break;
        case 'l': case 'L':
            keyStatus[(int)('l')] = 1;
            break;
        case 'k': case 'K':
            keyStatus[(int)('k')] = 1;
            break;
        case (unsigned char)231: // 'ç' (231) 
            keyStatus[231] = 1;
            break;
        case (unsigned char)199: // 'Ç' (199)
            keyStatus[199] = 1;
            break;
        case '5':
            if(!bulletP2)
            {
                bulletP2 = player2.Shoot();
                bulletP2->SetDestroyCallback([](Bullet* b){
                    delete b;
                    bulletP2 = nullptr; 
                });
            }
            break;
        case 27:
            exit(0);
            break;
        default:
            break;
    }
}

void onMouseMove(int x, int y)
{
    if(gameManager.isGameOver()) return; 

    float mouseX = (float)x/(float)WINDOW_SIZE;

    GLfloat theta = GUN_ANGLE - mouseX * GUN_ANGLE * 2;

    player1.SetArmAngle(theta);
}

void onMouseClick(int button, int state, int x, int y)
{
    if(gameManager.isGameOver()) return; 

    if(button == GLUT_LEFT_BUTTON && state == GLUT_UP)
    {
        if(!bulletP1)
        {
            bulletP1 = player1.Shoot();
                bulletP1->SetDestroyCallback([](Bullet* b){
                delete b;
                bulletP1 = nullptr; 
            });
        }  
    }
}

void idle(void)
{
    static GLdouble previousTime = glutGet(GLUT_ELAPSED_TIME);
    GLdouble currentTime = glutGet(GLUT_ELAPSED_TIME);
    GLdouble timeDiffMs = currentTime - previousTime;
    previousTime = currentTime;

    // converte ms -> segundos
    double dt = timeDiffMs / 1000.0;

    double ct = currentTime / 1000.0;

    // player 1
    player1.SetIsWalking(false);

    if(keyStatus[(int)('w')])
    {
        player1.SetIsWalking(true);
        player1.Move((GLfloat)((double)LINEAR_SPEED * dt), ct);
    }
    if(keyStatus[(int)('s')])
    {
        player1.SetIsWalking(true);
        player1.Move((GLfloat)((double)-LINEAR_SPEED * dt), ct);
    }
    if(keyStatus[(int)('a')])
    {
        player1.Rotate((GLfloat)((double)ANGULAR_SPEED * dt), ct);
    }
    if(keyStatus[(int)('d')])
    {
        player1.Rotate((GLfloat)((double)-ANGULAR_SPEED * dt), ct);
    }

    // player 2
    player2.SetIsWalking(false);
    if(keyStatus[(int)('o')])
    {
        player2.SetIsWalking(true);
        player2.Move((GLfloat)((double)LINEAR_SPEED * dt), ct);
    }
    if(keyStatus[(int)('l')])
    {
        player2.SetIsWalking(true);
        player2.Move((GLfloat)((double)-LINEAR_SPEED * dt), ct);
    }
    if(keyStatus[(int)('k')])
    {
        player2.Rotate((GLfloat)((double)ANGULAR_SPEED * dt), ct);
    }
    if(keyStatus[(int)0xE7])
    {
        player2.Rotate((GLfloat)((double)-ANGULAR_SPEED * dt), ct);
    }

    // Player 2 gun rotation (também com dt)
    if(keyStatus[(int)('4')])  player2.IncreaseArmAngle((GLfloat)((double)ANGULAR_SPEED * dt));
    if(keyStatus[(int)('6')])  player2.IncreaseArmAngle((GLfloat)((double)-ANGULAR_SPEED * dt));

    // bala: chamar com dt (segundos)
    if(bulletP1)
    {
        bulletP1->Move(dt);
    }

    if(bulletP2)
    {
        bulletP2->Move(dt);
    }

    if (gCollisionSystem) gCollisionSystem->Update();

    glutPostRedisplay();
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

    GLfloat arenaCx, arenaCy, arenaR;
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

        player2.SetParameters(cx - arenaCx, - cy + arenaCy, r, [](){ gameManager.decLifePlayer2(); });
        player2.DefineColor(1.0f, 0, 0); // Player 2 is red
    }
    if (c3) {
        GLfloat cx = c3->IntAttribute("cx", 0);
        GLfloat cy = c3->IntAttribute("cy", 0);
        GLfloat r  = c3->IntAttribute("r",  0);

        player1.SetParameters(cx - arenaCx, - cy + arenaCy, r, [](){ gameManager.decLifePlayer1(); });
        player1.DefineColor(0, 1.0f, 0); // Player 1 is green
    }

    int obstacleCount = 0;
    for (XMLElement* c = c3 ? c3->NextSiblingElement("circle") : nullptr; c; c = c->NextSiblingElement("circle")) ++obstacleCount;
    arena.GetObstacleVector().reserve(obstacleCount); 

    for (XMLElement* c = c3 ? c3->NextSiblingElement("circle") : nullptr; c; c = c->NextSiblingElement("circle")) {
        int cx = c->IntAttribute("cx", 0);
        int cy = c->IntAttribute("cy", 0);
        int r  = c->IntAttribute("r",  0);
        
        arena.AddObstacle(cx - arenaCx, - cy + arenaCy, r);
    }

    // ajuste para os players spawnarem olhando um pro outro
    setPlayersInicialRotation();

    // DEBUG
    std::cout << "registered colliders: " << collisionSystem.GetCount() << std::endl;
}

int main(int argc, char *argv[])
{
    gCollisionSystem = &collisionSystem;
    refGameManager = &gameManager;
    
    readSVG(argv[1]);
    
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
    
    glutKeyboardFunc(keyPress);
    glutKeyboardUpFunc(keyUp);

    glutPassiveMotionFunc(onMouseMove);
    glutMouseFunc(onMouseClick);
    
    init();

    glutMainLoop();

    // libera memoria
    // desaloca esses caras aqui 
    
    if(bulletP1) {
        delete bulletP1;
        bulletP1 = nullptr; 
    }

    if(bulletP2) {
        delete bulletP2;
        bulletP2 = nullptr; 
    }
     
    // CollisionSystem* collisionSystem = NULL;

    return 0;
}
