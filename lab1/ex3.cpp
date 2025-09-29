#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <stdio.h>
#define TAMANHO_JANELA 500
#define TAMANHO_QUADRADO 0.20

// variaveis para controlar a posição do quadrado

float R = 1.0;
float B = 0.0;

float gX = 0.40;
float gY = 0.40;

float mouseY = 0.0;
float mouseX = 0.0;

float deltaX = 0.50;
float deltaY = 0.50;

int quadState = 1;

void idle(void)
{
    if(quadState == 0)
    {
        printf("Mouse x:%.2f, y:%.2f\n", mouseX, mouseY);
        printf("1 - delta x = 1 - %.2f = %.2f\n", deltaX, (TAMANHO_QUADRADO - deltaX));


        // verifica se a posição do mouse é limite da tela 
        if(mouseX < 1 - (TAMANHO_QUADRADO - deltaX) && mouseX > deltaX && mouseY < 1 - (TAMANHO_QUADRADO - deltaY) && mouseY > deltaY)
        {
            // mover o quadrado
            gX = mouseX - deltaX; 
            gY = mouseY - deltaY; 
        }

        // mudar de cor -> azul
        R = 0.0;
        B = 1.0;
        
        printf("Quad x:%.2f, y:%.2f\n", gX, gY);
    }

    glutPostRedisplay();
}

bool clicouNoQuadrado(float x, float y)
{
    if(mouseX>gX && mouseX<gX+TAMANHO_QUADRADO && mouseY>gY && mouseY<gY+TAMANHO_QUADRADO)
    {
        printf("clicou dentro do quadrado!\n");

        deltaX = x - gX;
        deltaY = y - gY;

        return true;
    }
    
    return false;
}

void mouseMotion(int x, int y)
{
    y = TAMANHO_JANELA - y;

    mouseX = (float)x/(float)TAMANHO_JANELA;
    mouseY = (float)y/(float)TAMANHO_JANELA;
}


void mouse(int button, int state, int x, int y)
{
    y = TAMANHO_JANELA - y;

    mouseX = (float)x/(float)TAMANHO_JANELA;
    mouseY = (float)y/(float)TAMANHO_JANELA;

    printf("Mouse x:%.2f, y:%.2f\n", mouseX, mouseY);

    if(state == 1)
    {
        quadState = 1;
        
        // mudar de cor -> vermelho
        R = 1.0;
        B = 0.0;
    }
    
    if(state == 0 && clicouNoQuadrado(mouseX, mouseY))
    {
        quadState = 0;
    }

}

void display(void)
{
   /* Limpar todos os pixels  */
   glClear (GL_COLOR_BUFFER_BIT);

   /* Define cor dos vértices com os valores R, G e B variando de 0.0 a 1.0 */
    //    glColor3f (1.0, 0.0, 0.0);
   glColor3f (R, 0.0, B);
   /* Desenhar um polígono branco (retângulo) */
   glBegin(GL_POLYGON);
      glVertex3f (gX, gY, 0.0);
      glVertex3f (gX+TAMANHO_QUADRADO, gY, 0.0);
      glVertex3f (gX+TAMANHO_QUADRADO, gY+TAMANHO_QUADRADO, 0.0);
      glVertex3f (gX, gY+TAMANHO_QUADRADO, 0.0);
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
    glutIdleFunc(idle);

    glutMainLoop();

    return 0;
}
