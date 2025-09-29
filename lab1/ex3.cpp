#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <stdio.h>
#define TAMANHO_JANELA 500
#define TAMANHO_QUADRADO 0.40

// variaveis para controlar a posição do quadrado
float gX = 0.40;
float gY = 0.40;

float mouseY = 0;
float mouseX = 0;

int quadState = 0;

bool clicouNoQuadrado(int x, int y)
{
    if(mouseX>gX && mouseX<gX+TAMANHO_QUADRADO && mouseY>gY && mouseY<gY+TAMANHO_QUADRADO)
    {
        printf("clicou dentro do quadrado!\n");
        return true;
    }
    
    return false;
}

void mouseMotion(int x, int y)
{
    if(quadState == 0)
    {
        gX = mouseX;
        gY = mouseY;
    }

    glutPostRedisplay();
}


void mouse(int button, int state, int x, int y)
{
    y = TAMANHO_JANELA - y;

    mouseX = (float)x/(float)TAMANHO_JANELA;
    mouseY = (float)y/(float)TAMANHO_JANELA;

    if(state == 1)
    {
        quadState = 1;
    }
    
    if(state == 0 && clicouNoQuadrado(mouseX, mouseY))
    {
        quadState = 0;
    }

    // printf("Quad x:%f, y:%f\n", gX, gY);
    printf("Mouse x:%f, y:%f, state:%d\n", mouseX, mouseY, state);

}

void display(void)
{
   /* Limpar todos os pixels  */
   glClear (GL_COLOR_BUFFER_BIT);

   /* Define cor dos vértices com os valores R, G e B variando de 0.0 a 1.0 */
   glColor3f (1.0, 0.0, 0.0);
   /* Desenhar um polígono branco (retângulo) */
   glBegin(GL_POLYGON);
      glVertex3f (gX, gY, 0.0);
      glVertex3f (gX+TAMANHO_QUADRADO/2, gY, 0.0);
      glVertex3f (gX+TAMANHO_QUADRADO/2, gY+TAMANHO_QUADRADO/2, 0.0);
      glVertex3f (gX, gY+TAMANHO_QUADRADO/2, 0.0);
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
    glutMouseFunc(mouse);
    glutMotionFunc(mouseMotion);

    glutMainLoop();

    return 0;
}
