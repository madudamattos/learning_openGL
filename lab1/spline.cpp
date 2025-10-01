#include <GL/gl.h>
#include <GL/glu.h>
#include <stdlib.h>
#include <GL/glut.h>
#include <stdio.h>
#define TAMANHO_JANELA 500

float mouseX, mouseY;

float pointState[4] = {1,1,1,1};

int pointIndex = -1;

float radius = 0.2;

float R = 1.0, G = 1.0, B = 0.0;

// //Pontos de controle da Spline
GLfloat ctrlpoints[4][3] = {
   {0.1, 0.1, 0.0}, 
   {0.3, 0.9, 0.0}, 
   {0.6, 0.1, 0.0}, 
   {0.9, 0.9, 0.0}
};

void idle(void)
{
   if(pointIndex > -1 && pointState[pointIndex] == 0)
   {
      ctrlpoints[pointIndex][0] = mouseX;
      ctrlpoints[pointIndex][1] = mouseY;

      glMap1f(GL_MAP1_VERTEX_3, 0.0, 1.0, 3, 4, &ctrlpoints[0][0]); 
   }

   glutPostRedisplay();
}

int clicouEmUmPonto(float x, float y)
{
   for(int i=0; i<4; i++)
   {
      float z,w;
      
      z = ctrlpoints[i][0];
      w = ctrlpoints[i][1];

      printf("ctpoint x: %f, ctpoint y: %f, x: %f, y:%f\n", z, w, x, y);

      if(x>z-radius && x<z+radius && y<w+radius && y>w-radius)
      {
         printf("clicou dentro do ponto %d!\n", i);
         return i;
      }
   }

   return -1;
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
      // seta o estado dos pontos para não-clicado
      for(int i=0; i<4; i++)
      {
         pointState[i] = 1;
      }

      // mudar de cor -> amarelo
      R = 1.0;
   }
   
   pointIndex = clicouEmUmPonto(mouseX, mouseY);

   if(state == 0 && pointIndex > -1)
   {
      // muda de cor -> verde
      R = 0.0;
      // seta ponto para clicado
      pointState[pointIndex] = 0;
   }
}


void init(void)
{
   glClearColor(0.0, 0.0, 0.0, 0.0);
   glShadeModel(GL_FLAT);
   glEnable(GL_MAP1_VERTEX_3);

   //Definicao do polinomio com os pontos de controle
   glMap1f(GL_MAP1_VERTEX_3, 0.0, 1.0, 3, 4, &ctrlpoints[0][0]); 
   
   //Muda para a matriz de projecao (aulas futuras)
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   //Define a area/volume de visualizacao. Os objetos desenhados devem estar dentro desta area
   glOrtho(0.0, 1.0, 0.0, 1.0, -1.0, 1.0);
}

void display(void)
{
   int i;

   glClear(GL_COLOR_BUFFER_BIT);
   
   /* Desenha a curva aproximada por n+1 pontos. */
   int n = 30;
   glColor3f(1.0, 1.0, 1.0);
   glBegin(GL_LINE_STRIP);
      for (i = 0; i <= n; i++){
        //Avaliacao do polinomio, retorna um vertice (equivalente a um glVertex3fv) 
        glEvalCoord1f((GLfloat) i/(GLfloat)n);
      }
   glEnd();
   
   /* Desenha os pontos de controle. */
   glPointSize(5.0);
   glColor3f(R, G, B);
   glBegin(GL_POINTS);
      for (i = 0; i < 4; i++) 
         glVertex3fv(&ctrlpoints[i][0]);
   glEnd();
   
   glutSwapBuffers();
}

int main(int argc, char** argv)
{
   glutInit(&argc, argv);
   glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB);
   glutInitWindowSize (TAMANHO_JANELA, TAMANHO_JANELA);
   glutInitWindowPosition (100, 100);
   glutCreateWindow (argv[0]);
   init ();
   glutDisplayFunc(display);
   glutMouseFunc(mouse);
   glutMotionFunc(mouseMotion);
   glutIdleFunc(idle);
   glutMainLoop();

   return 0;
}

// #include <GL/gl.h>
// #include <GL/glu.h>
// #include <stdlib.h>
// #include <GL/glut.h>
// #define TAMANHO_JANELA 500

// float size = 5.0;

// // //Pontos de controle da Spline
// // GLfloat ctrlpoints[4][3] = {
// //         { -4.0, -4.0, 0.0}, { -2.0, 4.0, 0.0}, 
// //         {2.0, -4.0, 0.0}, {4.0, 4.0, 0.0}};

// GLfloat ctrlpoints[4][3] = {
//    {0.1, 0.1, 0.0}, 
//    {0.3, 0.9, 0.0}, 
//    {0.6, 0.1, 0.0}, 
//    {0.9, 0.9, 0.0}
// };

// void init(void)
// {
//    glClearColor(0.0, 0.0, 0.0, 0.0);
//    glShadeModel(GL_FLAT);
//    glEnable(GL_MAP1_VERTEX_3);

//    // // ajusta o polinômio de controle
//    // for(int i=0; i<4; i++)
//    // {
//    //    for(int j=0;j<2; j++)
//    //    {
//    //       ctrlpoints[i][j] = (ctrlpoints[i][j]+10)/10;
//    //    }
//    // }

//    //Definicao do polinomio com os pontos de controle
//    glMap1f(GL_MAP1_VERTEX_3, 0.0, 1.0, 3, 4, &ctrlpoints[0][0]); 
   
//    //Muda para a matriz de projecao (aulas futuras)
//    glMatrixMode(GL_PROJECTION);
//    glLoadIdentity();
//    //Define a area/volume de visualizacao. Os objetos desenhados devem estar dentro desta area
//    glOrtho(0.0, 1.0, 0.0, 1.0, -1.0, 1.0);
// }

// void display(void)
// {
//    int i;

//    glClear(GL_COLOR_BUFFER_BIT);
   
//    /* Desenha a curva aproximada por n+1 pontos. */
//    int n = 30;
//    glColor3f(1.0, 1.0, 1.0);
//    glBegin(GL_LINE_STRIP);
//       for (i = 0; i <= n; i++){
//         //Avaliacao do polinomio, retorna um vertice (equivalente a um glVertex3fv) 
//         glEvalCoord1f((GLfloat) i/(GLfloat)n);
//       }
//    glEnd();
   
//    /* Desenha os pontos de controle. */
//    glPointSize(5.0);
//    glColor3f(1.0, 1.0, 0.0);
//    glBegin(GL_POINTS);
//       for (i = 0; i < 4; i++) 
//          glVertex3fv(&ctrlpoints[i][0]);
//    glEnd();
   
//    glutSwapBuffers();
// }

// void reshape(int w, int h)
// {
//    //Define a porcao visivel da janela
//    glViewport(0, 0, (GLsizei) w, (GLsizei) h);
   
//    //Muda para a matriz de projecao (aulas futuras)
//    glMatrixMode(GL_PROJECTION);
//    glLoadIdentity();

//    //Controla o redimensionamento da janela mantendo o aspect ration do objeto
//    if (w <= h)
//       glOrtho(0.0, 1.0, 0.0, 1.0, -1.0, 1.0);

//    else
//       glOrtho(0.0, 1.0, 0.0, 1.0, -1.0, 1.0);

   
//    //Muda para a matriz de trasformacoes (aulas futuras)
//    glMatrixMode(GL_MODELVIEW);
//    glLoadIdentity();
// }

// int main(int argc, char** argv)
// {
//    glutInit(&argc, argv);
//    glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB);
//    glutInitWindowSize (TAMANHO_JANELA, TAMANHO_JANELA);
//    glutInitWindowPosition (100, 100);
//    glutCreateWindow (argv[0]);
//    init ();
//    glutDisplayFunc(display);
//    //glutReshapeFunc(reshape);
//    glutMainLoop();
//    return 0;
// }
