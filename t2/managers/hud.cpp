#include "hud.h"
#include <GL/glut.h>
#include <string>
#include <cstdio>
#include <cmath>

extern class GameManager* refGameManager;

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

    // decide o que desenhar com base no enum Screen
    if (currentScreen == Screen::baseHUD) {
        DrawBaseHUD();
    } else if (currentScreen == Screen::gameOver) {
        DrawGameOverScreen();
    }

    glPopMatrix(); // MODELVIEW
    glMatrixMode(GL_PROJECTION);
    glPopMatrix(); // PROJECTION
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
    //Draw text considering a 2D space (disable all 3d features)
    glMatrixMode (GL_PROJECTION);
    //Push to recover original PROJECTION MATRIX
    glPushMatrix();
        glLoadIdentity ();
        glOrtho (0, 1, 0, 1, -1, 1);
        RasterChars(x, y, 0, text, r, g, b);    
    glPopMatrix();
    glMatrixMode (GL_MODELVIEW);
}