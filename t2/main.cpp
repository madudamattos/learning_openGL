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

GLfloat playerHeight, playerRadius;
GLfloat camDist = 0.0f, camHeight = 0.0f, camXYAngle = -10.0f, camXZAngle = 0.0f;

int toggleCam = 0, rightButtonDown = 0, lastX = 0, lastY = 0;

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

    // // Use the same fovy used in init() and compute a camera distance that
    // // fits the whole "ViewingHeight" in view (minimal perspective distortion).
    // const float fovy_deg = 30.0f; // must match init()'s fovy
    // const float fovy_rad = fovy_deg * (float)M_PI / 180.0f;
    // const float halfViewHeight = (float)ViewingHeight * 0.5f;

    // // distance so that the vertical span at z=0 equals ViewingHeight
    // float eyeDist = (halfViewHeight / tanf(fovy_rad * 0.5f));

    // // small margin so edges are not clipped
    // const float margin = 1.05f;
    // eyeDist *= margin;

    // // position camera on +Y looking to origin, Z is up
    // gluLookAt(300.0f, eyeDist * 0.2, -700.0f,   // eye
    //         0.0f, 0.0f, 0.0f,      // center
    //         0.0f, 1.0f, 0.0f);     // up = Z
            
    
    //  camera vista de cima
    // gluLookAt(0, 1100, 0,   // eye
    //     0.0f, 0.0f, 0.0f,      // center
    //     0.0f, 0.0f, 1.0f);     // up = Z

    GLfloat playerX = player1.GetXPos(), playerY = player1.GetYPos(), playerZ = player1.GetZPos();

    // visao em terceira pessoa
    if (toggleCam == 0)
    {
        float playerAngle = (player1.GetBodyRotation() + camXZAngle) * M_PI / 180.0f;

        float camX = playerX + cos(camXYAngle * M_PI / 180.0f) * sin(-playerAngle) * camDist;
        float camY = playerY + playerHeight/2 - sin(camXYAngle * M_PI / 180.0f) * camDist;
        float camZ = playerZ + cos(camXYAngle * M_PI / 180.0f) *(-cos(-playerAngle)) * camDist;

        gluLookAt(camX, camY, camZ,  // posição da câmera
                  playerX, playerY + playerHeight/2, playerZ,  // para onde olha
                  0, 1, 0);  // up

    } else if(toggleCam == 1)
    {    
        // float playerAngle = player1.GetBodyRotation() * M_PI / 180.0f;

        // float eyeX = playerX;
        // float eyeY = playerY + camHeight;  // altura da cabeça
        // float eyeZ = playerZ - camDist;

        // float lookX = eyeX + sin(-playerAngle);
        // float lookY = eyeY - camHeight * 0.5f;  // inclinação leve para baixo
        // float lookZ = eyeZ + (-cos(-playerAngle));

        // gluLookAt(
        //         eyeX, eyeY, eyeZ,
        //         lookX, lookY, lookZ,
        //         0, 1, 0
        //     );
    
        gluLookAt(0, 1100, 0,   // eye
            0.0f, 0.0f, 0.0f,      // center
            0.0f, 0.0f, 1.0f);     // up = Z
    }

    // update light position to follow camera (optional but usually desirable)
    GLfloat light_position[] = { 0.0f, 4 * playerHeight, 0.0f, 1.0f };

    glLightfv(GL_LIGHT0, GL_POSITION, light_position);

    DrawAxes(70); // DEBUG

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


void setPlayersInitialRotation()
{
    // coloca os personagens na posição certa
    Alglib::Tuple2 p1(player1.GetXPos(), player1.GetZPos(), 1.0f);
    Alglib::Tuple2 p2(player2.GetXPos(), player2.GetZPos(), 1.0f);

    Alglib::Tuple2 v1 = Alglib::Subtract(p2, p1); // aponta de p1 para p2
    Alglib::Tuple2 v2 = Alglib::Subtract(p1, p2); // aponta de p2 para p1

    double angP1 = std::atan2((double)-v1.y, (double)v1.x) * 180.0 / M_PI;
    double angP2 = std::atan2((double)-v2.y, (double)v2.x) * 180.0 / M_PI;

    const double orientationOffset = 90.0; 
    angP1 += orientationOffset;
    angP2 += orientationOffset;

    player1.SetRotation((GLfloat)angP1);

    player2.SetRotation((GLfloat)angP2);

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

    setPlayersInitialRotation();
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
        // player 2 
        case '4':
            keyStatus[(int)('4')] = 1;
            break;
        case '6':
            keyStatus[(int)('6')] = 1;
            break;
        case '8':
            keyStatus[(int)('8')] = 1;
            break;
        case '2':
            keyStatus[(int)('2')] = 1;
            break;
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
        // toggle camera 
        case 'v':
            keyStatus[(int)('v')] = 1;
            break;
        case 'b':
            keyStatus[(int)('b')] = 1; 
            break;  
        case '=':
            keyStatus[(int)('=')] = 1;
            break;
        case '-':
            keyStatus[(int)('-')] = 1;
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

    GLfloat thetaX = GUN_ANGLE - mouseX * GUN_ANGLE * 2;

    float mouseY = (float)y/(float)WINDOW_SIZE;

    GLfloat thetaY = GUN_ANGLE - mouseY * GUN_ANGLE * 2;

    player1.SetArmAngle(thetaX, thetaY);

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

    if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {
        lastX = x;
        lastY = y;
        rightButtonDown = 1;
    } 
    if (button == GLUT_RIGHT_BUTTON && state == GLUT_UP) {
        rightButtonDown = 0;
    }
 
}

void onMouseDrag(int x, int y)
{
    if (!rightButtonDown)
        return;

    camXYAngle += (lastY - y) * 0.5f;

    camXZAngle += (lastX - x) * 0.5f;

    if (camXYAngle > 89.0f) camXYAngle = 89.0f;
    if (camXYAngle < -89.0f) camXYAngle = -89.0f;

    if (camXZAngle > 179.0f) camXZAngle = 179.0f;
    if (camXZAngle < -179.0f) camXZAngle = -179.0f;

    lastY = y; lastX = x;
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

    // camera 
    if(keyStatus[(int)('v')]) toggleCam = 0;
    else if(keyStatus[(int)('b')]) toggleCam = 1;
    
    // zoom da camera
    if(keyStatus[(int)('=')])
    {
        camDist += 200 * dt;
        if(camDist >= 500) camDist = 500;
    }
    if(keyStatus[(int)('-')])
    {
        camDist -= 200 * dt; 
        if(camDist <= playerRadius) camDist = playerRadius + 0.2f;
    } 


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
    if(keyStatus[(int)('4')])  player2.IncreaseArmAngle(0, (GLfloat)((double)ANGULAR_SPEED * dt));
    if(keyStatus[(int)('6')])  player2.IncreaseArmAngle(0, (GLfloat)((double)-ANGULAR_SPEED * dt));

    if(keyStatus[(int)('8')])  player2.IncreaseArmAngle((GLfloat)((double)ANGULAR_SPEED * dt), 0);
    if(keyStatus[(int)('2')])  player2.IncreaseArmAngle((GLfloat)((double)-ANGULAR_SPEED * dt), 0);


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


void changeCamera(int angle, int w, int h)
{
    glMatrixMode (GL_PROJECTION);

    glLoadIdentity ();

    gluPerspective (angle, 
            (GLfloat)w / (GLfloat)h, 1, 2000.0);

    glMatrixMode (GL_MODELVIEW);
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

    // Projeção em perspectiva com FOV menor (20-35)
    float fovy = 30.0f;                      
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

    GLfloat arenaCx, arenaCy, arenaR, playerR;
    if (c1) {
        arenaCx = c1->IntAttribute("cx", 0);
        arenaCy = c1->IntAttribute("cy", 0);
        arenaR  = c1->IntAttribute("r",  0);

        ViewingHeight = ViewingWidth = 2 * arenaR; // MUDAR AQUI 
    }
    if (c2) {
        GLfloat cx = c2->IntAttribute("cx", 0);
        GLfloat cy = c2->IntAttribute("cy", 0);
        GLfloat r  = c2->IntAttribute("r",  0);

        playerHeight = 3.65 * r;

        player2.SetParameters(arenaCx - cx, 0, - cy + arenaCy, r, playerHeight, [](){ gameManager.decLifePlayer2(); });
        player2.DefineColor(1.0f, 0, 0); // Player 2 is red
    }
    if (c3) {
        GLfloat cx = c3->IntAttribute("cx", 0);
        GLfloat cy = c3->IntAttribute("cy", 0);
        GLfloat r  = c3->IntAttribute("r",  0);

        playerRadius = r;
        playerHeight = 3.65 * r;
        
        player1.SetParameters(arenaCx - cx, 0, - cy + arenaCy, r, playerHeight, [](){ gameManager.decLifePlayer1(); });
        player1.DefineColor(0, 1.0f, 0); // Player 1 is green
    }

    camDist = 20 * playerRadius;
    camHeight = 3.05 * playerRadius;

    arena.SetParameters(0, 0, 0, arenaR, 4*playerHeight);

    int obstacleCount = 0;
    for (XMLElement* c = c3 ? c3->NextSiblingElement("circle") : nullptr; c; c = c->NextSiblingElement("circle")) ++obstacleCount;
    arena.GetObstacleVector().reserve(obstacleCount); 

    for (XMLElement* c = c3 ? c3->NextSiblingElement("circle") : nullptr; c; c = c->NextSiblingElement("circle")) {
        int cx = c->IntAttribute("cx", 0);
        int cy = c->IntAttribute("cy", 0);
        int r  = c->IntAttribute("r",  0);
        
        arena.AddObstacle(arenaCx - cx, 0, - cy + arenaCy, r, playerHeight);
    }

    // ajuste para os players spawnarem olhando um pro outro
    setPlayersInitialRotation();

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

    glutKeyboardFunc(keyPress);
    glutKeyboardUpFunc(keyUp);

    glutPassiveMotionFunc(onMouseMove);
    glutMouseFunc(onMouseClick);
    
    glutMotionFunc(onMouseDrag);

    init();

    glutMainLoop();

    return 0;
}
