#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <stdio.h>
#define TAMANHO_JANELA 500

// variaveis para controlar a posição do quadrado
float gX = 0.25;
float gY = 0.25;

int keyStatus[256];

enum keyState{
    PRESSIONADO = 1,
    NAO_PRESSIONADO = 0,
};

void idle(void)
{
    if(keyStatus[(int)('a')] == PRESSIONADO){
        gX -= 0.001;
    }
    else if(keyStatus[(int)('d')] == PRESSIONADO)
    {
        gX += 0.001;
    }
    else if(keyStatus[(int)('w')] == PRESSIONADO)
    {
        gY += 0.001; 
    }
    else if(keyStatus[(int)('s')] == PRESSIONADO)
    {
        gY -= 0.001;
    }

    glutPostRedisplay();
}

void keyUp(unsigned char key, int x, int y)
{
    keyStatus[(int)(key)] = NAO_PRESSIONADO;
    glutPostRedisplay();
}

void keyPress(unsigned char key, int x,
int y)
{
    keyStatus[(int)(key)] = PRESSIONADO;
}

void display(void)
{
   /* Limpar todos os pixels  */
   glClear (GL_COLOR_BUFFER_BIT);

   /* Define cor dos vértices com os valores R, G e B variando de 0.0 a 1.0 */
   glColor3f (0.90, 0.38, 0.59);
   /* Desenhar um polígono branco (retângulo) */
   glBegin(GL_POLYGON);
      glVertex3f (gX, gY, 0.0);
      glVertex3f (gX+0.50, gY, 0.0);
      glVertex3f (gX+0.50, gY+0.50, 0.0);
      glVertex3f (gX, gY+0.50, 0.0);
   glEnd();

   /* Desenhar no frame buffer! */
   glutSwapBuffers(); //Funcao apropriada para janela double buffer
}

void init (void) 
{
  /* selecionar cor de fundo (preto) */
  glClearColor (0.0, 0.0, 0.0, 0.0);

  /* inicializar sistema de visualizacao */
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(0.0, 1.0, 0.0, 1.0, -1.0, 1.0);
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize (TAMANHO_JANELA, TAMANHO_JANELA); 
    glutInitWindowPosition (100, 100);
    glutCreateWindow ("hello world");
    init ();
    glutDisplayFunc(display); 
    glutKeyboardFunc(keyPress);
    glutKeyboardUpFunc(keyUp);
    glutIdleFunc(idle);

    glutMainLoop();

    return 0;
}
