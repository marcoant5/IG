//**************************************************************************
// Práctica 5
//**************************************************************************

#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <ctype.h>
#include <math.h>
#include <vector>
#include "objetos_B3.h"
#include "CImg.h"
#include <string>

// ----------------------------------------------------------------------
// NECESARIO PARA CARGAR LAS IMÁGENES DE DISCO
using namespace cimg_library;
// ----------------------------------------------------------------------

using namespace std;

// tipos
typedef enum{CUBO, PIRAMIDE, OBJETO_PLY, ROTACION, EXTRUSION, PIRAMIDE_SIN_PICO, TEJADO, CILINDRO, CONO, ESFERA, TROFEO, BOMBILLA, ROTACION_PLY,
             MONTANA, EXCAVADORA, TEXTURA, CUBOTEX, P38, P38_ESCENA, SEMIESFERA, TRONCO_CONO} _tipo_objeto;
_tipo_objeto t_objeto=CUBO;
_modo   modo=POINTS;

// variables que definen la posicion de la camara en coordenadas polares
GLfloat Observer_distance;
GLfloat Observer_angle_x;
GLfloat Observer_angle_y;

// variables que controlan la ventana y la transformacion de perspectiva
GLfloat Size_x,Size_y,Front_plane,Back_plane;

// variables que determninan la posicion y tamaño de la ventana X
int Window_x=50,Window_y=50,Window_width=650,Window_high=650;


// objetos
_cubo cubo;
_piramide piramide(0.85,1.3);
_objeto_ply  ply; 
_rotacion rotacion; 
_extrusion *extrusion;
_piramide_sin_pico piramide_sin_pico(0.5,0.15,0.75);
_tejado tejado(0.4, 0.7,1,false);
_cilindro cilindro(1,2,50);
_cono cono(1, 2, 50);
_esfera esfera(1, 50, 50);
_trofeo trofeo(50);
_bombilla bombilla(50);
_rotacion_PLY rotacion_PLY;
_excavadora excavadora;
_montana montana(6,0.9, 0.8);
_cubo_textura cubo_textura;
_escena escena;
_semiesfera semiesfera(1, 50, 50);
_tronco_cono tronco_cono(1, 0.5, 2, 50);
_p38 p38;


// bool que indica si hay animación o no
bool animacion = false;
// bool que indica si se esta girando o no
float alfa = 0;
// variable que indica si la luz puntual está activada (1) o desactivada (0)
int luz_puntual = 0;
// variable que indica el cambio de la cámara
int cambio = 0;
// Variable para el estado del raton
int estadoRaton, xc, yc;

float izquierda = -2, derecha = 2, arriba = 2, abajo = -2, factor = 1.0;
int ancho, alto;

// IDENTIFICADOR DE TEXTURA
GLuint textura_id;
GLuint textura_id1;
GLuint textura_id2;
GLuint textura_id3;
GLuint textura_id4;
GLuint textura_id5;
GLuint textura_suelo;
GLuint textura_cielo;
GLuint textura_estrella;




// _objeto_ply *ply;

float giro1=0.0, giro2=0.0, giro3=0.0;
int pulsar=0.0;
int paso=0;


//**************************************************************************
//
//***************************************************************************

void clean_window()
{

glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
}


//**************************************************************************
// Funcion para definir la transformación de proyeccion
//***************************************************************************

/*void change_projection()
{

glMatrixMode(GL_PROJECTION);
glLoadIdentity();

// formato(x_minimo,x_maximo, y_minimo, y_maximo,plano_delantero, plano_traser)
//  plano_delantero>0  plano_trasero>PlanoDelantero)
glFrustum(-Size_x,Size_x,-Size_y,Size_y,Front_plane,Back_plane);
}*/

//**************************************************************************
// Funcion para definir la transformación de proyeccion
//***************************************************************************

void perspectiva()
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    glViewport(0,0,ancho,alto);
    // formato(x_minimo,x_maximo, y_minimo, y_maximo,plano_delantero, plano_traser)
    // plano_delantero>0  plano_trasero>PlanoDelantero)
    glFrustum(-Size_x,Size_x,-Size_y,Size_y,Front_plane,Back_plane);
}

//**************************************************************************
// Funcion para definir la transformación*ply1 de vista (posicionar la camara)
//***************************************************************************

void change_observer()
{

// posicion del observador
glMatrixMode(GL_MODELVIEW);
glLoadIdentity();
glTranslatef(0,0,-Observer_distance);
glRotatef(Observer_angle_x,1,0,0);
glRotatef(Observer_angle_y,0,1,0);
}

//**************************************************************************
// Funcion que dibuja los ejes utilizando la primitiva grafica de lineas
//***************************************************************************

void draw_axis()
{
	
glDisable(GL_LIGHTING);
glLineWidth(2);
glBegin(GL_LINES);
// eje X, color rojo
glColor3f(1,0,0);
glVertex3f(-AXIS_SIZE,0,0);
glVertex3f(AXIS_SIZE,0,0);
// eje Y, color verde
glColor3f(0,1,0);
glVertex3f(0,-AXIS_SIZE,0);
glVertex3f(0,AXIS_SIZE,0);
// eje Z, color azul
glColor3f(0,0,1);
glVertex3f(0,0,-AXIS_SIZE);
glVertex3f(0,0,AXIS_SIZE);
glEnd();
}


//**************************************************************************
// Funcion que dibuja los objetos
//***************************************************************************

void draw_objects()
{

switch (t_objeto){
	case CUBO: cubo.draw(modo,1.0,0.0,0.0,5);break;
	case PIRAMIDE: piramide.draw(modo,1.0,0.0,0.0,5);break;
    case OBJETO_PLY: ply.draw(modo,1.0,0.6,0.0,5);break;
    case ROTACION: rotacion.draw(modo,1.0,0.0,0.0,5);break;
    case EXTRUSION: extrusion->draw(modo,1.0,0.0,0.0,5);break;
    case PIRAMIDE_SIN_PICO: piramide_sin_pico.draw(modo,1.0,0.0,0.0,5);break;
    case TEJADO: tejado.draw(modo,1.0,0.0,0.0,5);break;
	case CILINDRO: cilindro.draw(modo,1.0,0.0,0.0,5);break;
    case CONO: cono.draw(modo,1.0,0.0,0.0,5);break;
    case ESFERA: esfera.draw(modo,1.0,0.0,0.0,5);break;
	case TROFEO: trofeo.draw(modo,1.0,0.0,0.0,5);break;
	case BOMBILLA: bombilla.draw(modo,1.0,0.0,0.0,5);break;
	case ROTACION_PLY: rotacion_PLY.draw(modo,1.0,0.0,0.0,5);break;
    case EXCAVADORA: excavadora.draw(modo,1.0,0.0,0.0,5);break;
    case MONTANA: montana.draw(modo,0.2,0.7,0.0,1);break;
    case TEXTURA: escena.draw(modo,1.0,0.0,0.0,5);break;
    case CUBOTEX: cubo_textura.draw(modo,1.0,0.0,0.0,5);break;
    case TRONCO_CONO: tronco_cono.draw(modo,1.0,0.0,0.0,5);break;
    case SEMIESFERA: semiesfera.draw(modo,1.0,0.0,0.0,5);break;
    case P38: p38.draw(modo,1.0,0.0,0.0,5);break;
    case P38_ESCENA: p38.draw_escena(modo,1.0,0.0,0.0,5);break;
    }
}

void luces(float alfa)
{
    // 3 primeras coordenadas es el valor de la luz, la última es 1 si luz puntual o 0 si es luz direccional
    // luz direccional
    float luz_ambiente[] = {0.2, 0.2, 0.2, 0.0},
        luz_difusa[] = {1.0, 1.0, 1.0, 0.0},
        luz_especular[] = {0.8, 0.8, 0.8, 0.0},
        luz_posicion[] = {-20.0, 20.0, 0.0, 0.0};
    
    // luz puntual
    float luz_ambiente2[] = {0.8, 0.0, 0.0, 1.0}, 
        luz_difusa2[] = {1.0, 1.0, 1.0, 1.0},
        luz_especular2[] = {0.5, 0.5, 0.5, 1.0},
        luz_posicion2[] = {20.0, 20.0, 0.0, 1.0};

    glLightfv(GL_LIGHT1, GL_AMBIENT, luz_ambiente);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, luz_difusa);
    glLightfv(GL_LIGHT1, GL_SPECULAR, luz_especular);
    glLightfv(GL_LIGHT1, GL_POSITION, luz_posicion);

    glLightfv(GL_LIGHT2, GL_AMBIENT, luz_ambiente2);
    glLightfv(GL_LIGHT2, GL_DIFFUSE, luz_difusa2);
    glLightfv(GL_LIGHT2, GL_SPECULAR, luz_especular2);

    glPushMatrix();
    glRotatef(alfa,0,1,0);
    glLightfv(GL_LIGHT2, GL_POSITION, luz_posicion2);
    glPopMatrix();

    glEnable(GL_LIGHT1);
    if(luz_puntual == 0)
        glDisable(GL_LIGHT2);
    else
        glEnable(GL_LIGHT2);
    glDisable(GL_LIGHT0);
    
}

//***************************************************************************
// Vistas multiples
//***************************************************************************

void vistas_multiples()
{
    // Alzado
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glViewport(0,alto/2,ancho/2.0,alto/2.0);
    glOrtho(izquierda*factor, derecha*factor, abajo*factor, arriba*factor, -100, 100);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    draw_axis();
    draw_objects();

    // Planta
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glViewport(0,0,ancho/2.0,alto/2.0);
    glOrtho(izquierda*factor, derecha*factor, abajo*factor, arriba*factor, -100, 100);    
    glRotatef(90,1,0,0);    // planta (vista desde arriba)
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    draw_axis();
    draw_objects();

    // Perfil
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glViewport(ancho/2,alto/2,ancho/2.0,alto/2.0);
    glOrtho(izquierda*factor, derecha*factor, abajo*factor, arriba*factor, -100, 100);    
    glRotatef(90,0,1,0);    // planta (vista desde arriba)
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    draw_axis();
    draw_objects();

    // Perspectiva
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glViewport(ancho/2,0,ancho/2.0,alto/2.0);
    glFrustum(-Size_x,Size_x,-Size_y,Size_y,Front_plane,Back_plane);
    change_observer();

    draw_axis();
    draw_objects();
}

void vistas_multiples_seleccion()
{
    // Alzado
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glViewport(0,alto/2,ancho/2.0,alto/2.0);
    glOrtho(izquierda*factor, derecha*factor, abajo*factor, arriba*factor, -100, 100);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    if (t_objeto==EXCAVADORA) excavadora.seleccion();
    if (t_objeto==P38 || t_objeto==P38_ESCENA) p38.seleccion();

    // Planta
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glViewport(0,0,ancho/2.0,alto/2.0);
    glOrtho(izquierda*factor, derecha*factor, abajo*factor, arriba*factor, -100, 100);    
    glRotatef(90,1,0,0);    // planta (vista desde arriba)
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    if (t_objeto==EXCAVADORA) excavadora.seleccion();
    if (t_objeto==P38 || t_objeto==P38_ESCENA) p38.seleccion();

    // Perfil
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glViewport(ancho/2,alto/2,ancho/2.0,alto/2.0);
    glOrtho(izquierda*factor, derecha*factor, abajo*factor, arriba*factor, -100, 100);    
    glRotatef(90,0,1,0);    // planta (vista desde arriba)
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    if (t_objeto==EXCAVADORA) excavadora.seleccion();
    if (t_objeto==P38 || t_objeto==P38_ESCENA) p38.seleccion();

    // Perspectiva
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glViewport(ancho/2,0,ancho/2.0,alto/2.0);
    glFrustum(-Size_x,Size_x,-Size_y,Size_y,Front_plane,Back_plane);
    change_observer();

    if (t_objeto==EXCAVADORA) excavadora.seleccion();
    if (t_objeto==P38 || t_objeto==P38_ESCENA) p38.seleccion();
}


//**************************************************************************
//
//***************************************************************************

void draw(void)
{   
    glDrawBuffer(GL_FRONT);
    clean_window();

    if (cambio == 0) {
        perspectiva();
        change_observer();

        draw_axis();
        draw_objects();
    }
    else vistas_multiples();

    luces(alfa);
    //glutSwapBuffers();

    if (t_objeto==EXCAVADORA) {

        glDrawBuffer(GL_BACK);
        clean_window();

        if (cambio == 0) {
            perspectiva();
            change_observer();

            if (t_objeto==EXCAVADORA) excavadora.seleccion();
            
        }
        else vistas_multiples_seleccion();        
    }


    if (t_objeto==P38 || t_objeto==P38_ESCENA) {

        glDrawBuffer(GL_BACK);
        clean_window();

        if (cambio == 0) {
            perspectiva();
            change_observer();

            if (t_objeto==P38 || t_objeto==P38_ESCENA) p38.seleccion();
            
        }
        else vistas_multiples_seleccion();        
    }

    glFlush();
}


//***************************************************************************
// Funcion llamada cuando se produce un cambio en el tamaño de la ventana
//
// el evento manda a la funcion:
// nuevo ancho
// nuevo alto
//***************************************************************************

void change_window_size(int Ancho1,int Alto1)
{
    float Aspect_ratio;

    Aspect_ratio=(float) Alto1/(float )Ancho1;
    Size_y=Size_x*Aspect_ratio;
    perspectiva();
    glViewport(0,0,Ancho1,Alto1);
    ancho = Ancho1; 
    alto = Alto1;
    glutPostRedisplay();
}

//***************************************************************************
// Funcion llamada cuando se aprieta una tecla normal
//
// el evento manda a la funcion:
// codigo de la tecla
// posicion x del raton
// posicion y del raton
//***************************************************************************

void normal_key(unsigned char Tecla1,int x,int y)
{
switch (toupper(Tecla1)){
		case 'Q':exit(0);
		case '1':modo=POINTS;break;
		case '2':modo=EDGES;break;
		case '3':modo=SOLID;break;
		case '4':modo=SOLID_COLORS;break;
        case '5':modo=SOLID_COLORS_GOURAUD;break;
        case '6':modo=SOLID_PHONG_FLAT;break;
        case '7':modo=SOLID_PHONG_GOURAUD;break;
        case 'P':t_objeto=PIRAMIDE;break;
        case 'C':t_objeto=CUBO;break;
        case 'O':t_objeto=OBJETO_PLY;break;	
        case 'R':t_objeto=ROTACION;break;
        case 'X':t_objeto=EXTRUSION;break;
		case 'T':t_objeto=TEJADO;break;
        case 'Z':t_objeto=PIRAMIDE_SIN_PICO;break;
		case 'L':t_objeto=CILINDRO;break;
		case 'A':t_objeto=CONO;break;
        case 'E':t_objeto=ESFERA;break;
		case 'W':t_objeto=TROFEO;break;
		case 'B':t_objeto=BOMBILLA;break;
		case 'V':t_objeto=ROTACION_PLY;break;
        case 'G':t_objeto=EXCAVADORA;break;
        case 'F':t_objeto=MONTANA;break;
        case 'K':t_objeto=TEXTURA;break;
        case 'J':t_objeto=CUBOTEX;
        case 'D':t_objeto=SEMIESFERA;break;
        case 'H':t_objeto=TRONCO_CONO;break;
        case 'M':t_objeto=P38;break;
        case 'N':t_objeto=P38_ESCENA;break;
        /*case 'S':if (pulsar==0)
                    {giro1=1.0;
                     giro2=1.0;
                     giro3=0.25;
                     pulsar=1;
                     }
                 else
                    {giro1=0.0;
                     giro2=0.0;
                     giro3=0.0;
                     pulsar=0;
                     }    
                 break;*/
        // bool animacion se comporta con valor 1 o 0 (dependiendo si es true o false)
        case '.': animacion = (animacion+1)%2; break; //Alterna entre true y false
        case 'I': luz_puntual = (luz_puntual+1)%2; break;
        case ',': cambio = (cambio+1)%2; break;
                // rotación de la iluminación puntual
        case '+': alfa+=5;break;
        case '-': alfa-=5;break;
    }
glutPostRedisplay();
}


//***************************************************************************
// Funcion llamada cuando se aprieta una tecla especial
//
// el evento manda a la funcion:
// codigo de la tecla
// posicion x del raton
// posicion y del raton

//***************************************************************************
void special_key(int Tecla1,int x,int y)
{
        // Controles generales
    switch (Tecla1)
    {
        case GLUT_KEY_LEFT:Observer_angle_y--;break;
        case GLUT_KEY_RIGHT:Observer_angle_y++;break;
        case GLUT_KEY_UP:Observer_angle_x--;break;
        case GLUT_KEY_DOWN:Observer_angle_x++;break;
        case GLUT_KEY_PAGE_UP:Observer_distance*=1.2;break;
        case GLUT_KEY_PAGE_DOWN:Observer_distance/=1.2;break;
    }

    if(t_objeto==P38 || t_objeto==P38_ESCENA){
        switch (Tecla1){

            case GLUT_KEY_F1:p38.gira_helices();break;
            case GLUT_KEY_F2:p38.sube_aleron(true);break;
            case GLUT_KEY_F3:p38.sube_aleron(false);break;
            case GLUT_KEY_F4:p38.cierra_alas(true);break;
            case GLUT_KEY_F5:p38.cierra_alas(false);break;
            case GLUT_KEY_F6:p38.rotar_rueda(true);break;
            case GLUT_KEY_F7:p38.rotar_rueda(false);break;
            case GLUT_KEY_F8:p38.guardar_rueda(true);break;
            case GLUT_KEY_F9:p38.guardar_rueda(false);break;
            case GLUT_KEY_F10:p38.desplazarse(true);break;
            case GLUT_KEY_F11:p38.desplazarse(false);break;
            case GLUT_KEY_F12:p38.valores_iniciales();break;
        }
    }

    if (t_objeto == EXCAVADORA) // Controles para la excavadora
    {
        switch (Tecla1)
        {
            case GLUT_KEY_F1:excavadora.giro_cabina+=5;break;
            case GLUT_KEY_F2:excavadora.giro_cabina-=5;break;
            case GLUT_KEY_F3:excavadora.giro_primer_brazo+=1;
                    if (excavadora.giro_primer_brazo > excavadora.giro_primer_brazo_max)
                        excavadora.giro_primer_brazo = excavadora.giro_primer_brazo_max;break;
            case GLUT_KEY_F4:excavadora.giro_primer_brazo-=1;
                    if (excavadora.giro_primer_brazo < excavadora.giro_primer_brazo_min)
                        excavadora.giro_primer_brazo = excavadora.giro_primer_brazo_min;break;
            case GLUT_KEY_F5:excavadora.giro_segundo_brazo+=1;
                    if (excavadora.giro_segundo_brazo > excavadora.giro_segundo_brazo_max)
                        excavadora.giro_segundo_brazo = excavadora.giro_segundo_brazo_max;break;
            case GLUT_KEY_F6:excavadora.giro_segundo_brazo-=1;
                    if (excavadora.giro_segundo_brazo < excavadora.giro_segundo_brazo_min) 
                        excavadora.giro_segundo_brazo = excavadora.giro_segundo_brazo_min;break;
            case GLUT_KEY_F7:excavadora.giro_pala+=1;
                    if (excavadora.giro_pala > excavadora.giro_pala_max)
                        excavadora.giro_pala = excavadora.giro_pala_max;break;
            case GLUT_KEY_F8:excavadora.giro_pala-=1;
                    if (excavadora.giro_pala < excavadora.giro_pala_min)
                        excavadora.giro_pala = excavadora.giro_pala_min;break;
        }
    }

glutPostRedisplay();
}

//***************************************************************************
// Funcion de animación
//****************************************************************************
void animacion_p38(){
    if(animacion){
        switch (p38.caso){
        case 0: if(p38.animacion_paso0()) p38.caso = 1; break;
        case 1: if(p38.animacion_paso1()) p38.caso = 2; break;
        case 2: if(p38.animacion_paso2()) p38.caso = 3; break;
        case 3: if(p38.animacion_paso3()) p38.caso = 4; break;
        case 4: if(p38.animacion_paso4()) p38.caso = 5; break;
        case 5: p38.valores_iniciales(); break;
        }
    }
    glutPostRedisplay();
    /*switch (paso){
  case 0:excavadora.giro_cabina-=giro1;
         if (excavadora.giro_cabina<-45) paso=1; break;
  case 1:excavadora.giro_primer_brazo-=giro3;*/
}


//***************************************************************************
// Funciones para la selección por color
//***************************************************************************

void procesar_color(unsigned char color[3])
{
int i;

if (t_objeto==EXCAVADORA){
    for (i=0;i<excavadora.piezas;i++)
    {if (color[0]==excavadora.color_select[i].r &&
            color[1]==excavadora.color_select[i].g &&
            color[2]==excavadora.color_select[i].r)
        {if (excavadora.activo[i]==0) 
                        {excavadora.activo[i]=1;
                        }
                    else 
                        {excavadora.activo[i]=0;
                        }
            glutPostRedisplay();
            }
        }  
}

if (t_objeto==P38 || t_objeto==P38_ESCENA){
    for (i=0;i<p38.piezas;i++)
    {if (color[0]==p38.color_select[i].r &&
            color[1]==p38.color_select[i].g &&
            color[2]==p38.color_select[i].b)
        {if (p38.activo[i]==0) 
                        {p38.activo[i]=1;
                        }
                    else 
                        {p38.activo[i]=0;
                        }
            glutPostRedisplay();
            }
        }             
    }
}

void pick(int x, int y)
{
   // GLint viewport[4];
    unsigned char pixel[3];

   // glGetIntegerv(GL_VIEWPORT, viewport);
    glReadBuffer(GL_BACK);
    glReadPixels(x,alto-y,1,1,GL_RGB,GL_UNSIGNED_BYTE,(GLubyte *) &pixel[0]);
    printf(" valor x %d, valor y %d, color %d, %d, %d \n",x,y,pixel[0],pixel[1],pixel[2]);

    procesar_color(pixel);
}

void clickRaton( int boton, int estado, int x, int y )
{
    if ( boton == GLUT_RIGHT_BUTTON ) {   
        if ( estado == GLUT_DOWN )
        {
            // se pulsa el botón derecho, entrado en el estado "moviendo cámara"
            estadoRaton=1;
            xc=x;
            yc=y;
        }
        else estadoRaton=0;
        // se levanta el botón derecho, saliendo del estado "moviendo cámara"
    }
            
    if ( boton == GLUT_LEFT_BUTTON ){ 
        if ( estado == GLUT_DOWN ){
            // se pulsa el botón izquierdo, entrado en el estado "pick"
            estadoRaton = 2;
            xc=x;
            yc=y;
            pick(xc, yc);
        }
    }

    if (boton == 3) {
        factor /= 1.1;
        glutPostRedisplay();
    }
    if (boton == 4) {
        factor *= 1.1;
        glutPostRedisplay();
    }
    
    // estadoRaton=0 cuando se deja de pulsar el botón derecho
    // estadoRaton=1 cuando se pulsa el botón derecho
    // estadoRaton=2 cuando se pulsa el botón izquierdo
    // xc, yc almacenan la posición del ratón
}

void ratonMovido( int x, int y )
{
    if(estadoRaton==1)
    {
        Observer_angle_y=Observer_angle_y-(x-xc);
        Observer_angle_x=Observer_angle_x+(y-yc);
        xc=x;
        yc=y;
        glutPostRedisplay();
    }
}


//***************************************************************************
// Funcion llamada cuando se selecciona una opción del menú
//
// el evento manda a la funcion:
// entero que representa una entrada del menú 
//***************************************************************************

void menu(int key)
{

   if (key==0)
        {excavadora.giro_pala+=4;
        if (excavadora.giro_pala > excavadora.giro_pala_max)
            excavadora.giro_pala = excavadora.giro_pala_max;
        }
   if (key==1)
        {excavadora.giro_pala-=4;
        if (excavadora.giro_pala < excavadora.giro_pala_min)
            excavadora.giro_pala = excavadora.giro_pala_min;
        }
   if (key==2)
          {giro1=1.0;
           giro2=1.0;
           giro3=0.25;
           }
   if (key==3)
          {giro1=0.0;
          giro2=0.0;
          giro3=0.0;
          }
}




//***************************************************************************
// Funcion de animación automática
//***************************************************************************

void animacionExcavadora()
{
switch (paso){
  case 0:excavadora.giro_cabina-=giro1;
         if (excavadora.giro_cabina<-45) paso=1; break;
  case 1:excavadora.giro_primer_brazo-=giro3;
         if (excavadora.giro_primer_brazo<-80) paso=2; break;
  case 2: excavadora.giro_pala-=giro2;
        if (excavadora.giro_pala < excavadora.giro_pala_min)
           {excavadora.giro_pala = excavadora.giro_pala_min;
            paso=0;
            excavadora.giro_cabina=0.0;
            excavadora.giro_primer_brazo=0.0;
            excavadora.giro_pala=0.0;} 
        break;
   } 
  
glutPostRedisplay();
}

//Funcion para librerar las texturas
void libera_textura (GLuint *tex_id)
{
   glDeleteTextures(1, tex_id);
}


//***************************************************************************
// Funcion de incializacion
//***************************************************************************

void initialize(void)
{
// se inicalizan la ventana y los planos de corte
Size_x=0.5;
Size_y=0.5;
Front_plane=1;
Back_plane=1000;

// se incia la posicion del observador, en el eje z
Observer_distance=4*Front_plane;
Observer_angle_x=0;
Observer_angle_y=0;

// se indica el color para limpiar la ventana	(r,v,a,al)
// blanco=(1,1,1,1) rojo=(1,0,0,1), ...
glClearColor(1,1,1,1);

// se habilita el z-bufer
glEnable(GL_DEPTH_TEST);
perspectiva();
glViewport(0,0,Window_width,Window_high);
}

// Funcion prepara texturas
void prepara_textura (char *file, GLuint *tex_id )
{
   vector<unsigned char> data; 
   CImg <unsigned char> image;

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


//***************************************************************************
// Programa principal
//
// Se encarga de iniciar la ventana, asignar las funciones e comenzar el
// bucle de eventos
//***************************************************************************

int main(int argc, char *argv[] )
{
 
// perfil 

vector<_vertex3f> perfil, poligono;
_vertex3f aux;

aux.x=1.0; aux.y=-1.0; aux.z=0.0;
perfil.push_back(aux);
aux.x=1.5; aux.y=0.0; aux.z=0.0;
perfil.push_back(aux);
aux.x=1.0; aux.y=1.0; aux.z=0.0;
perfil.push_back(aux);
aux.x=1.5; aux.y=1.2; aux.z=0.0;
perfil.push_back(aux);


rotacion.parametros(perfil,6,1,1,0);

aux.x=1.0; aux.y=0.0; aux.z=1.0;
poligono.push_back(aux);
aux.x=1.0; aux.y=0.0; aux.z=-1.0;
poligono.push_back(aux);
aux.x=-1.2; aux.y=0.0; aux.z=-1.0;
poligono.push_back(aux);
aux.x=-0.8; aux.y=0.0; aux.z=0.0;
poligono.push_back(aux);
aux.x=-1.2; aux.y=0.0; aux.z=1.0;
poligono.push_back(aux);

extrusion= new _extrusion(poligono, 0.25, 1.0, 0.25);

// se llama a la inicialización de glut
glutInit(&argc, argv);

// se indica las caracteristicas que se desean para la visualización con OpenGL
// Las posibilidades son:
// GLUT_SIMPLE -> memoria de imagen simple
// GLUT_DOUBLE -> memoria de imagen doble
// GLUT_INDEX -> memoria de imagen con color indizado
// GLUT_RGB -> memoria de imagen con componentes rojo, verde y azul para cada pixel
// GLUT_RGBA -> memoria de imagen con componentes rojo, verde, azul y alfa para cada pixel
// GLUT_DEPTH -> memoria de profundidad o z-bufer
// GLUT_STENCIL -> memoria de estarcido_rotation Rotation;
glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);

// posicion de la esquina inferior izquierdad de la ventana
glutInitWindowPosition(Window_x,Window_y);

// tamaño de la ventana (ancho y alto)
glutInitWindowSize(Window_width,Window_high);

// llamada para crear la ventana, indicando el titulo (no se visualiza hasta que se llama
// al bucle de eventos)
glutCreateWindow("PRACTICA - 5");

// asignación de la funcion llamada "dibujar" al evento de dibujo
glutDisplayFunc(draw);
// asignación de la funcion llamada "change_window_size" al evento correspondiente
glutReshapeFunc(change_window_size);
// asignación de la funcion llamada "normal_key" al evento correspondiente
glutKeyboardFunc(normal_key);
// asignación de la funcion llamada "tecla_Especial" al evento correspondiente
glutSpecialFunc(special_key);
// función de animación
glutIdleFunc(animacion_p38);
// funcion de inicialización
initialize();

 /*// Creamos menú
glutCreateMenu(menu);
glutAddMenuEntry("Girar pala positivo", 0);
glutAddMenuEntry("Girar pala negativo", 1);
glutAddMenuEntry("Activar animación", 2);
glutAddMenuEntry("Desactivar animación", 3);
glutAttachMenu(GLUT_LEFT_BUTTON);*/


//glutIdleFunc(animacionExcavadora);


// creación del objeto ply
// creación de los objetos ply
ply.parametros(argv[1]);
rotacion_PLY.parametros_PLY(argv[2], 50);

//ply = new _objeto_ply(argv[1]);

// Texturas - Prac4
prepara_textura("./texturas/realmadrid.png", &textura_id);
prepara_textura("./texturas/campo-futbol.webp", &textura_id1);
prepara_textura("./texturas/pared1.jpg", &textura_id2);
prepara_textura("./texturas/pared2.jpeg", &textura_id3);
prepara_textura("./texturas/metal.jpg", &textura_id4);
prepara_textura("./texturas/madrid.png", &textura_id5);

// Texturas modelo
prepara_textura("./texturas/suelo.jpg", &textura_suelo);
prepara_textura("./texturas/cielo.jpg", &textura_cielo);
prepara_textura("./texturas/estrella.png", &textura_estrella);


cubo_textura.textura(textura_id5);
escena.texturas(textura_id, textura_id1, textura_id2, textura_id3, textura_id4);
p38.texturas(textura_suelo, textura_cielo, textura_estrella);

// Movimiento del raton con la camara
glutMouseFunc(clickRaton);
glutMotionFunc(ratonMovido);

// inicio del bucle de eventos
glutMainLoop();

// Libera las texturas
libera_textura(&textura_id);
libera_textura(&textura_id1);
libera_textura(&textura_id2);
libera_textura(&textura_id3);
libera_textura(&textura_id4);
libera_textura(&textura_id5);
libera_textura(&textura_suelo);
libera_textura(&textura_cielo);
libera_textura(&textura_estrella);

return 0;
}
