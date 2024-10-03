//**************************************************************************
// Práctica 5
//**************************************************************************

#include <vector>
#include <GL/gl.h>
#include "vertex.h"
#include <stdlib.h>


const float AXIS_SIZE=5000;
typedef enum{POINTS,EDGES,SOLID,SOLID_COLORS, SOLID_COLORS_GOURAUD, SOLID_PHONG_FLAT, SOLID_PHONG_GOURAUD, SELECT} _modo;

//*************************************************************************
// clase punto
//*************************************************************************

class _puntos3D
{
public:

  
	_puntos3D();
void 	draw_puntos(float r, float g, float b, int grosor);

vector<_vertex3f> vertices;
};

//*************************************************************************
// clase triángulo
//*************************************************************************

class _triangulos3D: public _puntos3D
{
       public:

              _triangulos3D();
       void 	draw_aristas(float r, float g, float b, int grosor);
       void   draw_solido(float r, float g, float b);
       void 	draw_solido_colores(); // Caras
       void   draw_solido_colores_vertices(); // Vertices
       void   draw_solido_phong_flat(); // Vertices
       void   draw_solido_phong_gouraud(); // Vertices
       void   draw_solido_textura(int textura_id); // Textura
       void   draw_seleccion(int r, int g, int b);
       void 	draw(_modo modo, float r, float g, float b, float grosor);

       /* asignación de colores */
       void 	colors_random();
       void 	colors_chess(float r1, float g1, float b1, float r2, float g2, float b2);
       void   colores_altura();
	void   colores_degradado(float r1, float g1, float b1, float r2, float g2, float b2);
       void   colores_solido(float r, float g, float b);

	std::pair<float, float> encontrar_y();
       void   colors_diffuse_flat (float kr, float kg, float kb,
                                   float lpx, float lpy, float lpz);
                                   
       void   colors_diffuse_gouraud  (float kr, float kg, float kb,
                                   float lpx, float lpy, float lpz);                            
       /* calcular normales */

       void    calcular_normales_caras();
       void    calcular_normales_vertices();

       vector<_vertex3i> caras;
       vector<_vertex3f> colores_caras;
       vector<_vertex3f> colores_vertices;
       vector<_vertex3f> normales_caras;
       vector<_vertex3f> normales_vertices;
       vector<_vertex2f> texturas_vertices;


       int calculadas_normales_caras;

       //Material
       _vertex4f ambiente;
       _vertex4f difusa;
       _vertex4f especular;
       float brillo;

};

//*************************************************************************
// objetos o modelos
//*************************************************************************

//*************************************************************************
// clase cubo textura
//*************************************************************************

class _cubo_textura: public _triangulos3D
{
public:

	_cubo_textura(float tam=0.5);
       void textura(int textura);
       void  draw(_modo modo, float r, float g, float b, float grosor);
         
       int textura_id;
};

//*************************************************************************
// clase _suelo_tex textura
//*************************************************************************

class _base_textura: public _triangulos3D
{
public:

	_base_textura(float ancho=20, float largo=30);
};

//*************************************************************
// Escena con objetos con texturas
//*************************************************************
class _escena: public _triangulos3D
{
public:
   _escena();
   void  draw(_modo modo, float r, float g, float b, float grosor);
   void texturas(int textura, int textura1, int textura2, int textura3, int textura4);
   int textura_id, textura_id1, textura_id2, textura_id3, textura_id4;

   protected:
   _cubo_textura cubo_textura;
   _base_textura base_textura;
};

//*************************************************************************
// clase cubo
//*************************************************************************

class _cubo: public _triangulos3D
{
public:

	_cubo(float tam=0.5);
};


//*************************************************************************
// clase piramide
//*************************************************************************

class _piramide: public _triangulos3D
{
public:

	_piramide(float tam=0.5, float al=1.0);
};

//*************************************************************************
// clase objeto ply
//*************************************************************************

class _objeto_ply: public _triangulos3D
{
public:
   _objeto_ply();

void  parametros(char *archivo);
};

//************************************************************************
// objeto por revolución
//************************************************************************


class _rotacion: public _triangulos3D
{
public:
       _rotacion();
       
void  parametros(vector<_vertex3f> perfil, int num,  
                 int tapa_in, int tapa_su, int tipo);
};

// tapa_in=0 sin tapa, tapa_in=1 con tapa
// tapa_su=0 sin tapa, tapa_su=1 con tapa
// tipo=0 revolucion normal
// tipo=1 esfera
// tipo=2 cono

//************************************************************************
// objeto por extrusión
//************************************************************************

class _extrusion: public _triangulos3D
{
public:
       _extrusion(vector<_vertex3f> poligono, float x, float y, float z);
};


//************************************************************************
// objeto por revolución especiales
//************************************************************************

 
//************************************************************************
// cilindro
//************************************************************************

class _cilindro: public _rotacion
{
public:
       _cilindro(float radio=1.0, float altura=2.0, int num=50);
};

//************************************************************************
// cono
//************************************************************************

class _cono: public _rotacion
{
public:
       _cono(float radio=1.0, float altura=2.0, int num=50);
};

//************************************************************************
// esfera
//************************************************************************

class _esfera: public _rotacion
{
public:
       _esfera(float radio=1.0, int latitud=50, int longitud=50);
};

//************************************************************************
// trofeo
//************************************************************************

class _trofeo: public _rotacion
{
public:
       _trofeo(int lados);
};

//************************************************************************
// bombilla
//************************************************************************

class _bombilla: public _rotacion
{
public:
       _bombilla(int lados);
};

//*************************************************************************
// clase piramide rectangular sin pico
//*************************************************************************

class _tronco_piramide: public _triangulos3D
{
public:
	_tronco_piramide(float base_abajo = 0.75, float base_arriba = 0.5, float altura = 0.75);

};

//*************************************************************************
// clase tejado
//*************************************************************************

class _tejado: public _triangulos3D
{
public:
	_tejado(float ancho = 1, float largo = 1, float altura = 1, bool eje_z = true);

};


//************************************************************************
// rotacion archivo PLY
//************************************************************************

class _rotacion_PLY: public _rotacion
{
public:
       _rotacion_PLY();
void  parametros_PLY(char *archivo, int num);
};


//************************************************************************
// objeto montaña fractal
//************************************************************************

class _montana: public _triangulos3D
{
public:
       _montana(int nivelmax, float sigma, float h);
};

// nivelmax, ha de ser par es el tamaño de la cuadrícula
// sigma, se relaciona con la altura máxima de la montaña
// h 0<=h<=1, dimensión de Hausdorff indica la forma del relieve, más suave 1




//************************************************************************
// práctica 3, objeto jerárquico articulado excavadora
//************************************************************************

//************************************************************************
// piezas
//************************************************************************

//************************************************************************
// pala
//************************************************************************

class _pala: public _triangulos3D
{
public:
      _pala(float radio=1.0, float ancho=2.0, int num=8);
};

//************************************************************************
// brazo
//************************************************************************

class _brazo: public _triangulos3D
{
public:
      _brazo();
void  draw(_modo modo, float r, float g, float b, float grosor);

float ancho;
float alto;
float fondo;

protected:
_cubo cubo;
};

//************************************************************************
// cabina
//************************************************************************

class _cabina: public _triangulos3D
{
public:
       _cabina();
void  draw(_modo modo, float r, float g, float b, float grosor);

float ancho;
float alto;
float fondo;

protected:
_cubo cubo;
};

//************************************************************************
// sustentación
//************************************************************************

class _sustentacion: public _triangulos3D
{
public:
      _sustentacion();
void  draw(_modo modo, float r, float g, float b, float grosor);
       
float ancho;
float alto;
float fondo;

float radio;

protected:
_cilindro rueda;
_cubo base;
};

//************************************************************************
// excavadora (montaje del objeto final)
//************************************************************************

class _excavadora: public _triangulos3D
{
public:
       _excavadora();
       
void  draw(_modo modo, float r, float g, float b, float grosor);
void  seleccion(); 

float giro_cabina;
float giro_primer_brazo;
float giro_segundo_brazo;
float giro_pala;

float giro_primer_brazo_max;
float giro_primer_brazo_min;
float giro_segundo_brazo_max;
float giro_segundo_brazo_min;
float giro_pala_max;
float giro_pala_min;

float tamanio_pala;

// atributos para la selección

_vertex3f color_pick;
vector<_vertex3i> color_select;
vector<int> activo;
int piezas;
int grosor_select;  

protected:
_pala pala;
_brazo brazo;
_cabina cabina;
_sustentacion sustentacion;
};


//************************************************************************
// práctica 3, objeto jerárquico articulado Lockheed P-38 Lightning
//************************************************************************

//************************************************************************
// objetos especiales para el modelo jerárquico
//************************************************************************

//************************************************************************
// objeto tronco de cono (caso especial de rotacion)
//************************************************************************

class _tronco_cono: public _rotacion
{
public:
       _tronco_cono(float radio_inf=0.75, float radio_sup=0.25, float altura=2.0, int num=50);
};

//************************************************************************
// objeto semiesfera (caso especial de rotacion)
//************************************************************************

class _semiesfera: public _rotacion
{
public:
       _semiesfera(float radio=1.0, int latitud=50, int longitud=50);
};

//************************************************************************
// objeto semiesfera hueca (caso especial de rotacion)
//************************************************************************

class _semiesfera_hueca: public _rotacion
{
public:
       _semiesfera_hueca(float radio=1.0, int latitud=50, int longitud=50);
};

//************************************************************************
// piezas
//************************************************************************

//************************************************************************
// Estructura
//************************************************************************

class _estructura: public _triangulos3D
{
public:
      _estructura();
void  draw(_modo modo, float r, float g, float b, float grosor);

protected:
_esfera estructura_central;
_tronco_piramide union_estructuras;
_tronco_cono estructura_lateral;
_semiesfera_hueca escape;
_semiesfera complemento;
};

//************************************************************************
// Modulo de vuelo (cabina piloto)
//************************************************************************

class _modulo: public _triangulos3D
{
public:
      _modulo();
void  draw(_modo modo, float r, float g, float b, float grosor);

protected:
_semiesfera cupula;
};

//************************************************************************
// Rueda
//************************************************************************

class _rueda: public _triangulos3D
{
public:
      _rueda();
void  draw(_modo modo, float r, float g, float b, float grosor);

protected:
_cilindro neumatico;
_cilindro eje;
_cilindro llanta;
};

//************************************************************************
// Rueda principal
//************************************************************************

class _rueda_principal: public _triangulos3D
{
public:
      _rueda_principal();
void  draw(_modo modo, float r, float g, float b, float grosor);

protected:
_rueda rueda;
_cilindro eje;
};


//************************************************************************
// Helices
//************************************************************************

class _helices: public _triangulos3D
{
public:
      _helices();
void  draw(_modo modo, float r, float g, float b, float grosor);

protected:
_tronco_piramide aspa;
_cono soporte;
};

//************************************************************************
// Alas
//************************************************************************

class _ala: public _triangulos3D
{
public:
      _ala();
void  draw(_modo modo, float r, float g, float b, float grosor);

protected:
_tronco_cono estructura_ala;
_semiesfera punta;
};

//************************************************************************
// Estructura del Aleron
//************************************************************************

class _estructura_aleron: public _triangulos3D
{
public:
      _estructura_aleron();
void  draw(_modo modo, float r, float g, float b, float grosor);

protected:
_cubo base;
};

//************************************************************************
// Aleta
//************************************************************************

class _aleta: public _triangulos3D
{
public:
      _aleta();
void  draw(_modo modo, float r, float g, float b, float grosor);

protected:
_semiesfera base;
};

//************************************************************************
// Cañon
//************************************************************************

class _canon: public _triangulos3D
{
public:
      _canon();
void  draw(_modo modo, float r, float g, float b, float grosor);

protected:
_cilindro base;
_semiesfera complemento;
};

//************************************************************************
// Llama
//************************************************************************

class _llama: public _triangulos3D
{
public:
      _llama();
void  draw(_modo modo, float r, float g, float b, float grosor);

protected:
_cono fuego;
_cono fuego_pequeño;
_cono fuego_centro;
};

//************************************************************************
// P38 (montaje del objeto final)
//************************************************************************


class _p38: public _triangulos3D
{
public:
       _p38();
       
       void draw(_modo modo, float r, float g, float b, float grosor);
       void draw_escena(_modo modo, float r, float g, float b, float grosor);
       void gira_helices();
       void sube_aleron(bool sube);
       void cierra_alas(bool cierra);
       void rotar_rueda(bool rota);
       void guardar_rueda(bool guarda);
       void desplazarse(bool adelante);
       bool animacion_paso0();
       bool animacion_paso1();
       bool animacion_paso2();
       bool animacion_paso3();
       bool animacion_paso4();
       bool animacion_paso5();
       bool animacion_paso6();
       void valores_iniciales();
       void seleccion(); 
       void texturas(int textura_suelo, int textura_cielo, int textura_estrella);
       
       int tex_suelo, tex_cielo, tex_estrella;

       // atributos para la selección
       _vertex3f color_pick;
       vector<_vertex3i> color_select;
       vector<int> activo;
       int piezas;
       int grosor_select;

       // Atributos para los movimientos
       float giro_helices;
       float subida_aleron;
       float subida_aleron_max;
       float subida_aleron_min;
       float cierre_alas;
       float cierre_alas_max;
       float cierre_alas_min;
       float rota_rueda;
       float rota_rueda_max;
       float rota_rueda_min;
       float guarda_rueda;
       float guarda_rueda_max;
       float guarda_rueda_min;
       float desplazamiento;
       float desplazamiento_max;
       float despegue;
       float despegue_max;
       float diagonal;
       float diagonal_max;
       float caida;
       float caida_max;
       float incendio;
       float incendio_max;
       float movimiento;
       float movimiento_max;
       float movimiento_min;

       int caso;


protected:
       _estructura estructura;
       _modulo modulo;
       _rueda rueda;
       _rueda_principal rueda_principal;
       _cilindro union_rueda;
       _helices helices;
       _ala ala;
       _estructura_aleron aleron;
       _aleta aleta;
       _canon canon;
       _llama llama;
       _base_textura suelo;
       _base_textura cielo;
       _base_textura estrella;
};