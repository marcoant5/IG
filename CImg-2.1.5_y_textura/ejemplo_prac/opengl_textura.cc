#include <vector>
#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <string>

const float AxisSize=10000;
// ----------------------------------------------------------------------
// NECESARIO PARA CARGAR LAS IMÁGENES DE DISCO
#include "CImg.h"
using namespace cimg_library;
// ----------------------------------------------------------------------

using namespace std;

//Functions declarations
void Init();
void OnDraw();
void SpecialKeys(int key,int x,int y);

// FUNCIÓN QUE PREPARA LA TEXTURA
void prepara_textura(char *file, GLuint *tex_id);

// FUNCIÓN QUE DIBUJA
void dibuja(void);

// FUNCIÓN QUE LIBERA LA TEXTURA DE GPU
void libera_textura(GLuint *tex_id);


// IDENTIFICADOR DE TEXTURA
GLuint textura_id;
GLuint textura_id2;
GLuint textura_id3;

// OBJETO A DIBUJAR
GLfloat vertices[] = {-1,0.01,1,   1,0.01,1, 1,2.01,1,  -1,2.01,1, // delante
                     1,0.01,1,   1,0.01,-1, 1,2.01,-1,  1,2.01,1,  // derecha
                     1,0.01,-1,   -1,0.01,-1, -1,2.01,-1,  1,2.01,-1,  // detras
                     -1,0.01,-1,  -1,0.01,1,   -1,2.01,1,   -1,2.01,-1,  // izquierda
                     -1,2.01,-1,  -1,2.01,1,   1,2.01,1,   1,2.01,-1,  // arriba
                     -1,0.01,-1,  -1,0.01,1,   1,0.01,1,   1,0.01,-1,  // abajo

                     -10,0,-15,   -10,0,15,  10,0,15,   10,0,-15, // alfombra

                    0,0.01,0,   -0.5,0.01,-1,  0.5,0.01,-1,  //base
                    0,0.01,0,  -0.5,0.01,-1,  0,1,-0.75, 
                    -0.5,0.01,-1,  0.5,0.01,-1,  0,1,-0.75,
                    0.5,0.01,-1,  0,0.01,0,   0,1,-0.75

                    };
GLfloat texVertices[] = {0.25,0.33,  0.25,0.66,   0.5,0.66,   0.5,0.33,
                         0.5,0.33,  0.5,0.66,   0.75,0.66,   0.75,0.33,
                         0.75,0.33,  0.75,0.66,   1,0.66,   1,0.33,
                         0,0.33,  0,0.66,   0.25,0.66,   0.25,0.33,
                         0.25,0,  0.25,0.33,   0.5,0.33,   0.5,0,
                         0.25,0.66,  0.25,1,   0.5,1,   0.5,0.66,

                         0,0,  0,1,  1,1,    1,0,

                         0.011,0.485,  0.243,0.485,  0.1256,0.2488,
                         0.3589,0.2488,  0.243,0.485,  0.4778,0.485,
                         0.1256,0.2488,  0.3589,0.2488,  0.2456,0.0263,
                         0.1289,0.2488,  0.3589,0.2488,  0.2456,0.485,

                 };

float latitud=0.0,longitud=0.0,radio=24;


void Init(int argc, char **argv){
   //Initialize GLUT windows manager
   //Create the window
   glutInit(&argc, argv);
   glutInitWindowSize(700,700);
   glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
   glutCreateWindow("My World");

   glEnable(GL_DEPTH_TEST);
   //glEnable(GL_COLOR_MATERIAL);
   glMatrixMode(GL_PROJECTION);
   gluPerspective( 40.0, 700/700.0f, 0.1, 150);
  
   prepara_textura("./rubik.png", &textura_id);
   prepara_textura("./alfombra.jpg", &textura_id2);
   prepara_textura("./dados.jpg", &textura_id3);

}

void draw_axis(){
   glLineWidth(2);
   glBegin(GL_LINES);
       // eje X, color rojo
       glColor3f(1,0,0);
       glVertex3f(-AxisSize,0,0);
       glVertex3f(AxisSize,0,0);
       // eje Y, color verde
       glColor3f(0,1,0);
       glVertex3f(0,-AxisSize,0);
       glVertex3f(0,AxisSize,0);
       // eje Z, color azul
       glColor3f(0,0,1);
       glVertex3f(0,0,-AxisSize);
       glVertex3f(0,0,AxisSize);
   glEnd();
   glLineWidth(1);
   glColor3f(1,1,1);
}

void OnDraw(void){

   // cámara orbital
   float observador[3];
   float phir,thetar;
   phir=latitud*M_PI/180.0; 
   thetar=longitud*M_PI/180.0;
   observador[0]=radio*sin(thetar)*cos(phir);
   observador[1]=radio*sin(phir);
   observador[2]=radio*cos(phir)*cos(thetar);

   //cleaning the screen
   glClearColor(0.5,0.5,0.5,1);
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

   //Define view
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();

   // eye position -> observador
   // target -> (0,0,0)
   // define positive Y axis  -> (0.0, 1.0, 0.0)		
   gluLookAt (observador[0],observador[1],observador[2],0.0,0.0,0.0,0.0,1.0*cos(phir),0.0);

   draw_axis();
   //Put your code here to draw objects
   dibuja();

   //no delete this line
   glutSwapBuffers();
}


void SpecialKeys(int key,int x,int y) {
    switch (key){
	case GLUT_KEY_LEFT:longitud +=2.0;break;
	case GLUT_KEY_RIGHT:longitud -=2.0;break;
	case GLUT_KEY_UP:latitud +=2.0;break;
	case GLUT_KEY_DOWN:latitud -=2.0;break;
	case GLUT_KEY_PAGE_UP:radio +=1.0;break;
	case GLUT_KEY_PAGE_DOWN:radio -=1.0;if (radio<1) radio=1;break;
	}
    glutPostRedisplay();
}

int main(int argc, char** argv) {
   Init(argc, argv);
   //Enter the callbacks
   glutDisplayFunc(OnDraw);
   glutSpecialFunc(SpecialKeys);

   glutMainLoop(); // begin the loop

   // LIBERA LA TEXTURA
   libera_textura(&textura_id);
   libera_textura(&textura_id2);
   libera_textura(&textura_id3);


   return 0;
}


// ----------------------------------------------------------------------
void prepara_textura (char *file, GLuint *tex_id )
{
   vector<unsigned char> data; 
   CImg<unsigned char> image;

   image.load(file);

   // empaquetamos bien los datos
   for (long y = 0; y < image.height(); y ++)
      for (long x = 0; x < image.width(); x ++)
      {
	 unsigned char *r = image.data(x, y, 0, 0);
	 unsigned char *g = image.data(x, y, 0, 1);
	 unsigned char *b = image.data(x, y, 0, 2);
	 data.push_back(*r);
	 data.push_back(*g);
	 data.push_back(*b);
      }

   glGenTextures(1, tex_id);
   glBindTexture(GL_TEXTURE_2D, *tex_id);

   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

   // TRASFIERE LOS DATOS A GPU
   glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image.width(), image.height(),
		0, GL_RGB, GL_UNSIGNED_BYTE, &data[0]);
}


void dibuja (void)
{
   glEnable(GL_TEXTURE_2D);
   glEnableClientState(GL_VERTEX_ARRAY);
   glEnableClientState (GL_TEXTURE_COORD_ARRAY_EXT);

   glVertexPointer(3, GL_FLOAT, 0, vertices);
   glTexCoordPointer(2, GL_FLOAT, 0, texVertices);

   glBindTexture(GL_TEXTURE_2D, textura_id);

   glPushMatrix();
   glDrawArrays(GL_QUADS, 0, 24);
   glPopMatrix();

   glPushMatrix();
   glTranslatef(2,0,0);
   glRotatef(35,0,1,0);
   glScalef(0.5,0.5,0.5);
   glDrawArrays(GL_QUADS, 0, 24);
   glPopMatrix();

   glPushMatrix();
   glTranslatef(-4,1.61,3);
   glRotatef(60,0,1,0);
   glRotatef(180,0,0,1);
   glScalef(0.8,0.8,0.8);
   glDrawArrays(GL_QUADS, 0, 24);
   glPopMatrix();

   glBindTexture(GL_TEXTURE_2D, textura_id2);
   glPushMatrix();
   glDrawArrays(GL_QUADS, 24, 4);   
   glPopMatrix();

   glBindTexture(GL_TEXTURE_2D, textura_id3);
   glPushMatrix();
   glTranslatef(-3,0,0);
   glScalef(3,3,3);
   glDrawArrays(GL_TRIANGLES, 28, 12);   
   glPopMatrix();

   glPushMatrix();
   glTranslatef(2,0,-2.5);
   glRotatef(30,0,1,0);
   glScalef(4,4,4);
   glDrawArrays(GL_TRIANGLES, 28, 12);   
   glPopMatrix();
   
   glDisableClientState(GL_VERTEX_ARRAY);
   glBindTexture(GL_TEXTURE_2D, 0);
   glDisable(GL_TEXTURE_2D);
}

void libera_textura (GLuint *tex_id)
{
   glDeleteTextures(1, tex_id);
}

