#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <stdio.h>
#define TAMANHO_JANELA 500

// variaveis para controlar a posição do quadrado
float gX = 0.25;
float gY = 0.25;

void keyPress(unsigned char key, int x,
int y)
{
   if(key == 'a')
   {
      gX -= 0.05;
   }
   else if(key == 'd')
   {
      gX += 0.05;
   }
   else if(key == 'w')
   {
      gY += 0.05; 
   }
   else if(key =='s')
   {
      gY -= 0.05;
   }

   glutPostRedisplay();
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

    glutMainLoop();

    return 0;
}
