#ifndef HUD_H
#define HUD_H

#include <GL/glut.h>
#include "gameManager.h"

enum class Screen { 
    baseHUD = 0, 
    gameOver = 1 
};

class HUD {
    GLint windowHeight;
    GLint windowWidth;
    Screen currentScreen;

public:
    HUD(int w, int h)
    {
        this->windowHeight = h;
        this->windowWidth = w;
        this->currentScreen = Screen::baseHUD;
    }

    void SetScreen(Screen s) { currentScreen = s; }
    void Draw();

    void RasterChars(GLfloat x, GLfloat y, GLfloat z, const char * text, double r, double g, double b);
    void PrintText(GLfloat x, GLfloat y, const char * text, double r, double g, double b);


private:
    void DrawText(int x, int y, const char *text);
    void DrawHearts(int x, int y, int n);
    void DrawBaseHUD();
    void DrawGameOverScreen();
};
#endif