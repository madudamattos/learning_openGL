#include "hud.h"
#include "arena.h"
#include "player.h"
#include <GL/glut.h>
#include <string>
#include <cstdio>
#include <cmath>

extern class GameManager* refGameManager;
extern Arena arena;
extern Player player1;
extern Player player2;

static void glutDrawString(int x, int y, void *font, const std::string &s) {
    glRasterPos2i(x, y);
    for (char c : s) glutBitmapCharacter(font, c);
}

void HUD::DrawText(int x, int y, const char *text) {
    glColor3f(1.0f, 1.0f, 1.0f);
    glutDrawString(x, y, GLUT_BITMAP_9_BY_15, std::string(text));
}

void HUD::DrawHearts(int x, int y, int n) {
    const int slices = 12;
    const float step = 2.0f * 3.14159265f / slices;
    const float r = 8.0f;
    for (int k = 0; k < n; ++k) {
        int cx = x + r + k * (int)(r * 3.0f);
        glBegin(GL_TRIANGLE_FAN);
        glVertex2f((float)cx, (float)y);
        for (int i = 0; i <= slices; ++i) {
            float theta = i * step;
            float vx = cx + r * cosf(theta);
            float vy = y + r * sinf(theta);
            glVertex2f(vx, vy);
        }
        glEnd();
    }
}

void HUD::DrawGameOverScreen()
{
    const std::string go = "GAME OVER";
    int sx = (windowWidth / 4) - (int)go.size() * 4;
    DrawText(sx, (windowHeight - heightEye) / 3, go.c_str());

    std::string winner;

    if(refGameManager->getWinner() == Winner::Player1) winner = "Player 1 wins!";
    if(refGameManager->getWinner() == Winner::Player2) winner = "Player 2 wins!";
    if(refGameManager->getWinner() == Winner::None) winner = "No winners!";

    sx = (windowWidth / 4) - (int)winner.size() * 4;
    DrawText(sx, (windowHeight - heightEye) / 3 + 20, winner.c_str());
}

void HUD::DrawBaseHUD()
{
    glColor3f(1.0f, 1.0f, 1.0f);

    char buf[64];
    std::snprintf(buf, sizeof(buf), "Player 1");
    DrawText(20, 20, buf); // 10px da esquerda, 20px do topo

    std::snprintf(buf, sizeof(buf), "Player 2");
    DrawText(windowWidth/2 - 90, 20, buf); 

    int lives1 = refGameManager->getLifePlayer1();
    int lives2 = refGameManager->getLifePlayer2();

    glColor3f(0.0f, 1.0f, 0.0f);
    DrawHearts(20, 40, lives1);
    
    glColor3f(1.0f, 0.0f, 0.0f);
    DrawHearts(windowWidth/2 - 80, 40, lives2);
}

void HUD::Draw() {
    
    // antes de desenhar faz um update no estado do jogo 
    currentScreen = refGameManager->isGameOver() ? Screen::gameOver : Screen::baseHUD; 
    
    // salva estados relevantes (lighting/depth/texture) e define projeção para coordenadas em pixels (origem top-left)
    glPushAttrib(GL_ENABLE_BIT);
    // desenhar HUD em cima da cena: desabilita teste de profundidade, iluminação, texturas e face culling
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_LIGHTING);
    glDisable(GL_TEXTURE_2D);
    glDisable(GL_CULL_FACE);

    // salva e define projeção para coordenadas em pixels (origem top-left)
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glOrtho(0, windowWidth/2, 500, 0, -1, 1); // origem no topo-left

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    if (currentScreen == Screen::baseHUD) {
        DrawBaseHUD();
    } else if (currentScreen == Screen::gameOver) {
        DrawGameOverScreen();
    }

    glPopMatrix(); 
    glMatrixMode(GL_PROJECTION);
    glPopMatrix(); 
    glMatrixMode(GL_MODELVIEW);

    // restaura estados anteriores (depth/light/texture)
    glPopAttrib();
}

void HUD::RasterChars(GLfloat x, GLfloat y, GLfloat z, const char * text, double r, double g, double b)
{
    //Push to recover original attributes
    glPushAttrib(GL_ENABLE_BIT);
        glDisable(GL_LIGHTING);
        glDisable(GL_TEXTURE_2D);
        //Draw text in the x, y, z position
        glColor3f(r,g,b);
        glRasterPos3f(x, y, z);
        const char* tmpStr;
        tmpStr = text;
        while( *tmpStr ){
            glutBitmapCharacter(GLUT_BITMAP_9_BY_15, *tmpStr);
            tmpStr++;
        }
    glPopAttrib();
}

void HUD::PrintText(GLfloat x, GLfloat y, const char * text, double r, double g, double b)
{
    glMatrixMode (GL_PROJECTION);
    glPushMatrix();
        glLoadIdentity ();
        glOrtho (0, 1, 0, 1, -1, 1);
        RasterChars(x, y, 0, text, r, g, b);    
    glPopMatrix();
    glMatrixMode (GL_MODELVIEW);
}

void HUD::DrawMiniMap()
{
    // Minimapa quadrado
    int miniSize = windowWidth / 4;
    int miniW = miniSize;
    int miniH = miniSize;

    int posX = windowWidth - miniW;
    int posY = 0;

    // Salva viewport atual
    GLint oldViewport[4];
    glGetIntegerv(GL_VIEWPORT, oldViewport);

    glViewport(posX, posY, miniW, miniH);
    
    // Estados
    glPushAttrib(GL_ENABLE_BIT);
    glDisable(GL_LIGHTING);
    glDisable(GL_TEXTURE_2D);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);


    // Projeção ortográfica
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();

    float arenaR = arena.GetRadius();
    glOrtho(-arenaR, arenaR,
            -arenaR, arenaR,
            -1, 1);

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();


    // DESENHAR ARENA
    glColor3f(1,1,1);
    glBegin(GL_LINE_LOOP);
    for(int i = 0; i < 64; i++)
    {
        float ang = 2.0f * M_PI * i / 64;
        glVertex2f(arenaR * cos(ang),
                   arenaR * sin(ang));
    }
    glEnd();


    // OBSTÁCULOS
    glColor3f(0,0,0);

    for(auto& obs : arena.GetObstacleVector())
    {
        glBegin(GL_LINE_LOOP);
        for(int i = 0; i < 32; i++)
        {
            float ang = 2.0f * M_PI * i / 32;
            glVertex2f(
                obs.x + obs.r * cos(ang),
                obs.z + obs.r * sin(ang)
            );
        }
        glEnd();
    }

    // PLAYER 1 
    float r1 = player1.GetRadius();

    glColor3f(0,1,0);
    glBegin(GL_LINE_LOOP);
    for(int i = 0; i < 32; i++)
    {
        float ang = 2.0f * M_PI * i / 32;
        glVertex2f(
            player1.GetXPos() + r1 * cos(ang),
            player1.GetZPos() + r1 * sin(ang)
        );
    }
    glEnd();

    // PLAYER 2 
    float r2 = player2.GetRadius();

    glColor3f(1,0,0);
    glBegin(GL_LINE_LOOP);
    for(int i = 0; i < 32; i++)
    {
        float ang = 2.0f * M_PI * i / 32;
        glVertex2f(
            player2.GetXPos() + r2 * cos(ang),
            player2.GetZPos() + r2 * sin(ang)
        );
    }
    glEnd();

    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);

    glPopAttrib();

    // Restaurar viewport original
    glViewport(oldViewport[0],
               oldViewport[1],
               oldViewport[2],
               oldViewport[3]);
}
