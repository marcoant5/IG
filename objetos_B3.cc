//**************************************************************************
// Práctica 5
//**************************************************************************

#include "objetos_B3.h"
#include "file_ply_stl.hpp"
#include <time.h> 


//*************************************************************************
// _puntos3D
//*************************************************************************

_puntos3D::_puntos3D()
{
}

//*************************************************************************
// dibujar puntos
//*************************************************************************

void _puntos3D::draw_puntos(float r, float g, float b, int grosor)
{
//**** usando vertex_array ****
glPointSize(grosor);
glColor3f(r,g,b);

glEnableClientState(GL_VERTEX_ARRAY);
glVertexPointer(3,GL_FLOAT,0,&vertices[0]);
glDrawArrays(GL_POINTS,0,vertices.size()); 

/*int i;
glPointSize(grosor);
glColor3f(r,g,b);
glBegin(GL_POINTS);
for (i=0;i<vertices.size();i++){
	glVertex3fv((GLfloat *) &vertices[i]);
	}
glEnd();*/
}


//*************************************************************************
// _triangulos3D
//*************************************************************************

_triangulos3D::_triangulos3D()
{
  int calculadas_normales_caras = 0;
  // Temporalmente ponemos aquí los materiales
	ambiente = _vertex4f(0, 0, 1.0, 1.0);
  difusa = _vertex4f(0, 0, 1.0, 1.0);
	especular = _vertex4f(0.5, 0.5, 0.5, 1.0);
	brillo = 100; // De 20 a 180
}

//************************************************************************
// función para hallar la minima y maxima altura
//************************************************************************

std::pair<float, float> _triangulos3D::encontrar_y()
{
    if (vertices.empty()) {
        // Devuelve un par de coordenadas "y" vacías si no hay vértices.
        return std::make_pair(0.0f, 0.0f);
    }

    float y_mas_bajo = vertices[0].y;
    float y_mas_alto = vertices[0].y;

    for (unsigned long int i = 1; i < vertices.size(); i++) {
        if (vertices[i].y < y_mas_bajo) {
            y_mas_bajo = vertices[i].y;
        }
        if (vertices[i].y > y_mas_alto) {
            y_mas_alto = vertices[i].y;
        }
    }

    return std::make_pair(y_mas_bajo, y_mas_alto);
}


//*************************************************************************
// dibujar en modo arista
//*************************************************************************

void _triangulos3D::draw_aristas(float r, float g, float b, int grosor)
{
//**** usando vertex_array ****
glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
glLineWidth(grosor);
glColor3f(r,g,b);

glEnableClientState(GL_VERTEX_ARRAY);
glVertexPointer(3,GL_FLOAT,0,&vertices[0]);
glDrawElements(GL_TRIANGLES,caras.size()*3,GL_UNSIGNED_INT,&caras[0]);

/*int i;
glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
glLineWidth(grosor);
glColor3f(r,g,b);
glBegin(GL_TRIANGLES);
for (i=0;i<caras.size();i++){
	glVertex3fv((GLfloat *) &vertices[caras[i]._0]);
	glVertex3fv((GLfloat *) &vertices[caras[i]._1]);
	glVertex3fv((GLfloat *) &vertices[caras[i]._2]);
	}
glEnd();*/
}

//*************************************************************************
// dibujar en modo sólido con un único color
//*************************************************************************

void _triangulos3D::draw_solido(float r, float g, float b)
{
int i;
glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
glColor3f(r,g,b);
glBegin(GL_TRIANGLES);
for (i=0;i<caras.size();i++){
	glVertex3fv((GLfloat *) &vertices[caras[i]._0]);
	glVertex3fv((GLfloat *) &vertices[caras[i]._1]);
	glVertex3fv((GLfloat *) &vertices[caras[i]._2]);
	}
glEnd();

}

//*************************************************************************
// dibujar en modo sólido con colores diferentes para cada cara
//*************************************************************************

void _triangulos3D::draw_solido_colores( )
{
int i;
glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
glBegin(GL_TRIANGLES);
for (i=0;i<caras.size();i++){
	glColor3f(colores_caras[i].r,colores_caras[i].g,colores_caras[i].b);
	glVertex3fv((GLfloat *) &vertices[caras[i]._0]);
	glVertex3fv((GLfloat *) &vertices[caras[i]._1]);
	glVertex3fv((GLfloat *) &vertices[caras[i]._2]);
	}
glEnd();
}

//*************************************************************************
// dibujar en modo sólido con colores diferentes para cada vertice
//*************************************************************************

void _triangulos3D::draw_solido_colores_vertices( )
{
int i;
glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
glShadeModel(GL_SMOOTH);
glBegin(GL_TRIANGLES);
for (i=0;i<caras.size();i++){
	glColor3f(colores_vertices[caras[i]._0].r,colores_vertices[caras[i]._0].g,colores_vertices[caras[i]._0].b);
	glVertex3fv((GLfloat *) &vertices[caras[i]._0]);
  glColor3f(colores_vertices[caras[i]._1].r,colores_vertices[caras[i]._1].g,colores_vertices[caras[i]._1].b);
	glVertex3fv((GLfloat *) &vertices[caras[i]._1]);
  glColor3f(colores_vertices[caras[i]._2].r,colores_vertices[caras[i]._2].g,colores_vertices[caras[i]._2].b);
	glVertex3fv((GLfloat *) &vertices[caras[i]._2]);
	}
glEnd();
glShadeModel(GL_FLAT);
}

//*************************************************************************
// dibujar en modo sólido con iluminacion plano
//*************************************************************************

void _triangulos3D::draw_solido_phong_flat( )
{
int i;
glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
glEnable(GL_LIGHTING);
glShadeModel(GL_FLAT);  // tipo plano
glEnable(GL_NORMALIZE);

glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, (GLfloat*) &ambiente);
glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, (GLfloat*) &difusa);
glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, (GLfloat*) &especular);
glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, brillo);

glBegin(GL_TRIANGLES);
for (i=0;i<caras.size();i++){
	glNormal3f(normales_caras[i].x,normales_caras[i].y,normales_caras[i].z);  // establece el vector normal actual
	glVertex3fv((GLfloat *) &vertices[caras[i]._0]);
	glVertex3fv((GLfloat *) &vertices[caras[i]._1]);
	glVertex3fv((GLfloat *) &vertices[caras[i]._2]);
	}
glEnd();
glDisable(GL_LIGHTING);
}

//*************************************************************************
// dibujar en modo sólido con iluminación suave
//*************************************************************************

void _triangulos3D::draw_solido_phong_gouraud()
{
int i;
glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
glEnable(GL_LIGHTING);
glShadeModel(GL_SMOOTH);  // tipo suave
glEnable(GL_NORMALIZE);

glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, (GLfloat*) &ambiente);
glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, (GLfloat*) &difusa);
glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, (GLfloat*) &especular);
glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, brillo);

glBegin(GL_TRIANGLES);
for (i=0;i<caras.size();i++){
	glNormal3f(normales_vertices[caras[i]._0].x,normales_vertices[caras[i]._0].y,normales_vertices[caras[i]._0].z);
	glVertex3fv((GLfloat *) &vertices[caras[i]._0]);
  
  glNormal3f(normales_vertices[caras[i]._1].x,normales_vertices[caras[i]._1].y,normales_vertices[caras[i]._1].z);
	glVertex3fv((GLfloat *) &vertices[caras[i]._1]);
  
  glNormal3f(normales_vertices[caras[i]._2].x,normales_vertices[caras[i]._2].y,normales_vertices[caras[i]._2].z);
	glVertex3fv((GLfloat *) &vertices[caras[i]._2]);
	}
glEnd();
glDisable(GL_LIGHTING);
}

//*************************************************************************
// dibujar en modo seleccion
//*************************************************************************

void _triangulos3D::draw_seleccion(int r, int g, int b)
{
  int i;

  glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
  glColor3ub(r,g,b);
  glBegin(GL_TRIANGLES);
  for (i=0;i<caras.size();i++)
  {
    glVertex3fv((GLfloat *) &vertices[caras[i]._0]);
    glVertex3fv((GLfloat *) &vertices[caras[i]._1]);
    glVertex3fv((GLfloat *) &vertices[caras[i]._2]);
  }
  glEnd();
}

//*************************************************************************
// dibujar con texturas
//*************************************************************************

void _triangulos3D::draw_solido_textura(int textura_id)
{
int i;
glEnable(GL_TEXTURE_2D);
glBindTexture(GL_TEXTURE_2D, textura_id);
glColor3f(1,1,1);
glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
glBegin(GL_TRIANGLES);
for (i=0;i<caras.size();i++){
  glTexCoord2f(texturas_vertices[caras[i]._0].x, texturas_vertices[caras[i]._0].y);
	glVertex3fv((GLfloat *) &vertices[caras[i]._0]);
	glTexCoord2f(texturas_vertices[caras[i]._1].x, texturas_vertices[caras[i]._1].y);
	glVertex3fv((GLfloat *) &vertices[caras[i]._1]);
	glTexCoord2f(texturas_vertices[caras[i]._2].x, texturas_vertices[caras[i]._2].y);
	glVertex3fv((GLfloat *) &vertices[caras[i]._2]);
	}
glEnd();
glDisable(GL_TEXTURE_2D);
}


//*************************************************************************
// dibujar con distintos modos
//*************************************************************************

void _triangulos3D::draw(_modo modo, float r, float g, float b, float grosor)
{
switch (modo){
	case POINTS:draw_puntos(r, g, b, grosor);break;
	case EDGES:draw_aristas(r, g, b, grosor);break;
	case SOLID:draw_solido(r, g, b);break;
	case SOLID_COLORS:draw_solido_colores();break;
  case SOLID_COLORS_GOURAUD:draw_solido_colores_vertices();break;
  case SOLID_PHONG_FLAT:draw_solido_phong_flat();break;
  case SOLID_PHONG_GOURAUD:draw_solido_phong_gouraud();break;
  case SELECT:draw_seleccion(r, g, b);break;
	}
}

//*************************************************************************
// normales 
//*************************************************************************

void _triangulos3D::calcular_normales_caras()
{
int i, n_c;
_vertex3f va, vb;
float modulo;

n_c=caras.size();
normales_caras.resize(n_c);
for (i=0;i<n_c;i++)
  {va=vertices[caras[i]._1]-vertices[caras[i]._0];
   vb=vertices[caras[i]._2]-vertices[caras[i]._1];
   normales_caras[i].x=va.y*vb.z-va.z*vb.y;
   normales_caras[i].y=va.z*vb.x-va.x*vb.z;   
   normales_caras[i].z=va.x*vb.y-va.y*vb.x; 
   modulo=sqrt(normales_caras[i].x*normales_caras[i].x+
              normales_caras[i].y*normales_caras[i].y+
              normales_caras[i].z*normales_caras[i].z);
   normales_caras[i].x/=modulo;
   normales_caras[i].y/=modulo;   
   normales_caras[i].z/=modulo;     
  }

  calculadas_normales_caras=1;
}

//*************************************************************************

void _triangulos3D::calcular_normales_vertices()
{
int i, n_c, n_v;
float modulo;

if (calculadas_normales_caras==0)
   calcular_normales_caras();

n_v=vertices.size();
normales_vertices.resize(n_v);

n_c=caras.size();

for (i=0;i<n_v;i++)
 normales_vertices[i]=_vertex3f(0.0,0.0,0.0);
 
for (i=0;i<n_c;i++)
  {
  normales_vertices[caras[i]._0]+=normales_caras[i];
  normales_vertices[caras[i]._1]+=normales_caras[i];
  normales_vertices[caras[i]._2]+=normales_caras[i];
  } 
  
for (i=0;i<n_v;i++)
      {modulo=sqrt(normales_vertices[i].x*normales_vertices[i].x+
                   normales_vertices[i].y*normales_vertices[i].y+
                   normales_vertices[i].z*normales_vertices[i].z);
       normales_vertices[i].x/=modulo;
       normales_vertices[i].y/=modulo;
       normales_vertices[i].z/=modulo;            
      }
}


//*************************************************************************
// asignación colores
//*************************************************************************

void _triangulos3D::colors_random()
{
int i, n_c;
n_c=caras.size();
colores_caras.resize(n_c);
srand (time(NULL));
for (i=0;i<n_c;i++)  
  {colores_caras[i].r=rand()%1000/1000.0;
   colores_caras[i].g=rand()%1000/1000.0;
   colores_caras[i].b=rand()%1000/1000.0;
  }
int n_v;
n_v=vertices.size();
colores_vertices.resize(n_c);
srand (time(NULL));
for (i=0;i<n_c;i++)  
  {colores_vertices[i].r=rand()%1000/1000.0;
   colores_vertices[i].g=rand()%1000/1000.0;
   colores_vertices[i].b=rand()%1000/1000.0;
  }
}

//*************************************************************************

void _triangulos3D::colors_chess(float r1, float g1, float b1, float r2, float g2, float b2)
{
int i, n_c;
n_c=caras.size();
colores_caras.resize(n_c);
for (i=0;i<n_c;i++)  
  {if (i%2==0) 
     {colores_caras[i].r=r1;
      colores_caras[i].g=g1;
      colores_caras[i].b=b1;
     }
   else 
     {colores_caras[i].r=r2;
      colores_caras[i].g=g2;
      colores_caras[i].b=b2;
     } 
  }
int n_v;
n_v=vertices.size();
colores_vertices.resize(n_c);
for (i=0;i<n_c;i++)  
  {if (i%2==0) 
     {colores_vertices[i].r=r1;
      colores_vertices[i].g=g1;
      colores_vertices[i].b=b1;
     }
   else 
     {colores_vertices[i].r=r2;
      colores_vertices[i].g=g2;
      colores_vertices[i].b=b2;
     } 
  }
}


//*************************************************************************
// asignar colores altura para bombilla
//*************************************************************************

void _triangulos3D::colores_altura()
{
    colores_caras.resize(caras.size());

    for (unsigned long int i = 0; i < caras.size(); i++){
        if (vertices[caras[i]._0].y >= 0 && vertices[caras[i]._1].y >= 0 && vertices[caras[i]._2].y >= 0)
        {
            colores_caras[i].r = 1.0; // Componente roja al máximo (amarillo)
            colores_caras[i].g = 1.0; // Componente verde al máximo (amarillo)
            colores_caras[i].b = 0.0; // Componente azul a cero
        }
        else
        {
            colores_caras[i].r = 0.4; // Componente roja a cero
            colores_caras[i].g = 0.4; // Componente verde a cero
            colores_caras[i].b = 0.4; // Componente azul al máximo (azul)
        }
    }
}

//************************************************************************
// colores degradado
//************************************************************************

void _triangulos3D::colores_degradado(float r1, float g1, float b1, float r2, float g2, float b2)
{
    colores_caras.resize(caras.size());

    // Encontrar y mínimo y máximo
    pair<float, float> y = encontrar_y();

    struct Color {
      float r, g, b;
    };

    // Definir los colores límite del degradado
    Color colorMin = {r1, g1, b1};  // Color para los valores de y mínimos
    Color colorMax = {r2, g2, b2};  // Color para los valores de y máximos

    for (unsigned long int i = 0; i < caras.size(); i++) {
        float y0 = vertices[caras[i]._0].y;
        float y1 = vertices[caras[i]._1].y;
        float y2 = vertices[caras[i]._2].y;
        // Normalizar los valores de y al rango [0, 1]
        float t0 = (y0 - y.first) / (y.second - y.first);
        float t1 = (y1 - y.first) / (y.second - y.first);
        float t2 = (y2 - y.first) / (y.second - y.first);
        // Interpolar los colores en función de t0, t1 y t2
        colores_caras[i].r = (1.0 - t0) * colorMin.r + t0 * colorMax.r;
        colores_caras[i].g = (1.0 - t1) * colorMin.g + t1 * colorMax.g;
        colores_caras[i].b = (1.0 - t2) * colorMin.b + t2 * colorMax.b;
    }
}

void _triangulos3D::colores_solido(float r, float g, float b){
  int i, n_c;
  n_c=caras.size();
  colores_caras.resize(n_c);
  for (i=0;i<n_c;i++){
      colores_caras[i].r=r;
      colores_caras[i].g=g;
      colores_caras[i].b=b;
  }
}


//*************************************************************************

void _triangulos3D::colors_diffuse_flat (float kr, float kg, float kb,
                             float lpx, float lpy, float lpz)
{
int i, n_c;
float modulo, escalar;
_vertex3f l;

n_c=caras.size();
colores_caras.resize(n_c);
for (i=0;i<n_c;i++)
  { // Deberia de ser un punto promedio de los 3 vertices, no _0 solo
   l.x=lpx-vertices[caras[i]._0].x;
   l.y=lpy-vertices[caras[i]._0].y;
   l.z=lpz-vertices[caras[i]._0].z;
   modulo=sqrt(l.x*l.x+l.y*l.y+l.z*l.z);
   l.x/=modulo;
   l.y/=modulo;
   l.z/=modulo;
   
   escalar=l.x*normales_caras[i].x+l.y*normales_caras[i].y+l.z*normales_caras[i].z;
   if (escalar>0.0)
     {colores_caras[i].r=kr*escalar;
      colores_caras[i].g=kg*escalar;
      colores_caras[i].b=kb*escalar;
     }
   else {colores_caras[i].r=0.0;
        colores_caras[i].g=0.0;
        colores_caras[i].b=0.0;
        }
  }
}

// Color difuso gouraud
void _triangulos3D::colors_diffuse_gouraud (float kr, float kg, float kb,
                             float lpx, float lpy, float lpz)
{
int i, n_v;
float modulo, escalar;
_vertex3f l;

n_v=vertices.size();
colores_vertices.resize(n_v);
for (i=0;i<n_v;i++)
  { 
   l.x=lpx-vertices[i].x;
   l.y=lpy-vertices[i].y;
   l.z=lpz-vertices[i].z;
   modulo=sqrt(l.x*l.x+l.y*l.y+l.z*l.z);
   l.x/=modulo;
   l.y/=modulo;
   l.z/=modulo;
   
   escalar=l.x*normales_vertices[i].x+l.y*normales_vertices[i].y+l.z*normales_vertices[i].z;
   if (escalar>0.0)
     {colores_vertices[i].r=kr*escalar;
      colores_vertices[i].g=kg*escalar;
      colores_vertices[i].b=kb*escalar;
     }
   else {colores_vertices[i].r=0.0;
        colores_vertices[i].g=0.0;
        colores_vertices[i].b=0.0;
        }
  }
}

//*************************************************************************
// objetos o modelos
//*************************************************************************

//*************************************************************************
// clase cubo
//*************************************************************************

_cubo::_cubo(float tam)
{
//vertices
vertices.resize(8);
vertices[0].x=-tam;vertices[0].y=-tam;vertices[0].z=tam;
vertices[1].x=tam;vertices[1].y=-tam;vertices[1].z=tam;
vertices[2].x=tam;vertices[2].y=tam;vertices[2].z=tam;
vertices[3].x=-tam;vertices[3].y=tam;vertices[3].z=tam;
vertices[4].x=-tam;vertices[4].y=-tam;vertices[4].z=-tam;
vertices[5].x=tam;vertices[5].y=-tam;vertices[5].z=-tam;
vertices[6].x=tam;vertices[6].y=tam;vertices[6].z=-tam;
vertices[7].x=-tam;vertices[7].y=tam;vertices[7].z=-tam;

// triangulos
caras.resize(12);
caras[0]._0=0;caras[0]._1=1;caras[0]._2=3;
caras[1]._0=3;caras[1]._1=1;caras[1]._2=2;
caras[2]._0=1;caras[2]._1=5;caras[2]._2=2;
caras[3]._0=5;caras[3]._1=6;caras[3]._2=2;
caras[4]._0=5;caras[4]._1=4;caras[4]._2=6;
caras[5]._0=4;caras[5]._1=7;caras[5]._2=6;
caras[6]._0=0;caras[6]._1=7;caras[6]._2=4;
caras[7]._0=0;caras[7]._1=3;caras[7]._2=7;
caras[8]._0=3;caras[8]._1=2;caras[8]._2=7;
caras[9]._0=2;caras[9]._1=6;caras[9]._2=7;
caras[10]._0=0;caras[10]._1=1;caras[10]._2=4;
caras[11]._0=1;caras[11]._1=5;caras[11]._2=4; 

// normales
calcular_normales_caras();
calcular_normales_vertices();

//colores de las caras
colors_random();

}


//*************************************************************************
// clase piramide
//*************************************************************************

_piramide::_piramide(float tam, float al)
{
int i;
//vertices 
vertices.resize(5); 
vertices[0].x=-tam;vertices[0].y=0;vertices[0].z=tam;
vertices[1].x=tam;vertices[1].y=0;vertices[1].z=tam;
vertices[2].x=tam;vertices[2].y=0;vertices[2].z=-tam;
vertices[3].x=-tam;vertices[3].y=0;vertices[3].z=-tam;
vertices[4].x=0;vertices[4].y=al;vertices[4].z=0;

caras.resize(6);
caras[0]._0=0;caras[0]._1=1;caras[0]._2=4;
caras[1]._0=1;caras[1]._1=2;caras[1]._2=4;
caras[2]._0=2;caras[2]._1=3;caras[2]._2=4;
caras[3]._0=3;caras[3]._1=0;caras[3]._2=4;
caras[4]._0=3;caras[4]._1=1;caras[4]._2=0;
caras[5]._0=3;caras[5]._1=2;caras[5]._2=1;

// normales
calcular_normales_caras();
calcular_normales_vertices();

//colores de las caras
colors_random();

}

//*************************************************************************
// clase piramide rectangular sin pico 
//*************************************************************************

_tronco_piramide::_tronco_piramide(float base_abajo, float base_arriba, float altura)
{
    //vertices 
    vertices.resize(16); 
    //Base abajo grande
    vertices[0].x = -base_abajo/2; vertices[0].y = 0; vertices[0].z = -base_abajo/2;
    vertices[1].x = base_abajo/2; vertices[1].y = 0; vertices[1].z = -base_abajo/2;
    vertices[2].x = base_abajo/2; vertices[2].y = 0; vertices[2].z = base_abajo/2;
    vertices[3].x = -base_abajo/2; vertices[3].y = 0; vertices[3].z = base_abajo/2;
    //Base abajo pequeña X
    vertices[4].x = -base_arriba/2; vertices[4].y = 0; vertices[4].z = -base_abajo/2;
    vertices[5].x = base_arriba/2; vertices[5].y = 0; vertices[5].z = -base_abajo/2;
    vertices[6].x = base_arriba/2; vertices[6].y = 0; vertices[6].z = base_abajo/2;
    vertices[7].x = -base_arriba/2; vertices[7].y = 0; vertices[7].z = base_abajo/2;
    //Base arriba
    vertices[8].x = -base_arriba/2; vertices[8].y = altura; vertices[8].z = -base_arriba/2;
    vertices[9].x = base_arriba/2; vertices[9].y = altura; vertices[9].z = -base_arriba/2;
    vertices[10].x = base_arriba/2; vertices[10].y = altura; vertices[10].z = base_arriba/2;
    vertices[11].x = -base_arriba/2; vertices[11].y = altura; vertices[11].z = base_arriba/2;
    //Base abajo pequeña Z
    vertices[12].x = -base_abajo/2; vertices[12].y = 0; vertices[12].z = -base_arriba/2;
    vertices[13].x = base_abajo/2; vertices[13].y = 0; vertices[13].z = -base_arriba/2;
    vertices[14].x = base_abajo/2; vertices[14].y = 0; vertices[14].z = base_arriba/2;
    vertices[15].x = -base_abajo/2; vertices[15].y = 0; vertices[15].z = base_arriba/2;

    caras.resize(12);
    //Base abajo
    caras[0]._0 = 0; caras[0]._1 = 1; caras[0]._2 = 2;
    caras[1]._0 = 0; caras[1]._1 = 2; caras[1]._2 = 3;
    //Base arriba
    caras[2]._0 = 8; caras[2]._1 = 9; caras[2]._2 = 10;
    caras[3]._0 = 8; caras[3]._1 = 10; caras[3]._2 = 11;
    //Alrededor cara 1
    caras[4]._0 = 0; caras[4]._1 = 9; caras[4]._2 = 8;
    caras[5]._0 = 0; caras[5]._1 = 9; caras[5]._2 = 1;
    //Alrededor cara 2
    caras[6]._0 = 1; caras[6]._1 = 9; caras[6]._2 = 10;
    caras[7]._0 = 1; caras[7]._1 = 10; caras[7]._2 = 2;
    //Alrededor cara 3
    caras[8]._0 = 2; caras[8]._1 = 10; caras[8]._2 = 11;
    caras[9]._0 = 2; caras[9]._1 = 11; caras[9]._2 = 3;
    //Alrededor cara 4
    caras[10]._0 = 3; caras[10]._1 = 11; caras[10]._2 = 8;
    caras[11]._0 = 3; caras[11]._1 = 8; caras[11]._2 = 0;

  // normales
  calcular_normales_caras();
  calcular_normales_vertices();

  //colores de las caras
  colors_random();

}

//*************************************************************************
// clase tejado
//*************************************************************************

_tejado::_tejado(float ancho, float largo, float altura, bool eje_z)
{
    //vertices 
    vertices.resize(6); 
    //caras
    caras.resize(8);
    //Base abajo
    vertices[0].x = -ancho/2; vertices[0].y = 0; vertices[0].z = -largo/2;
    vertices[1].x = ancho/2; vertices[1].y = 0; vertices[1].z = -largo/2;
    vertices[2].x = ancho/2; vertices[2].y = 0; vertices[2].z = largo/2;
    vertices[3].x = -ancho/2; vertices[3].y = 0; vertices[3].z = largo/2;

    //Base
    caras[0]._0 = 0; caras[0]._1 = 1; caras[0]._2 = 3;
    caras[1]._0 = 1; caras[1]._1 = 2; caras[1]._2 = 3;

    //Altura
    if (eje_z) {
        vertices[4].x = 0; vertices[4].y = altura; vertices[4].z = largo/2;
        vertices[5].x = 0; vertices[5].y = altura; vertices[5].z = -largo/2;
        //Caras
        caras[2]._0 = 1; caras[2]._1 = 2; caras[2]._2 = 4;
        caras[3]._0 = 5; caras[3]._1 = 1; caras[3]._2 = 4;
        caras[4]._0 = 0; caras[4]._1 = 5; caras[4]._2 = 3;
        caras[5]._0 = 5; caras[5]._1 = 4; caras[5]._2 = 3;
        caras[6]._0 = 2; caras[6]._1 = 3; caras[6]._2 = 4;
        caras[7]._0 = 0; caras[7]._1 = 1; caras[7]._2 = 5;
    }
    else {
        vertices[4].x = ancho/2; vertices[4].y = altura; vertices[4].z = 0;
        vertices[5].x = -ancho/2; vertices[5].y = altura; vertices[5].z = 0;
        //Caras
        caras[2]._0 = 0; caras[2]._1 = 1; caras[2]._2 = 4;
        caras[3]._0 = 0; caras[3]._1 = 4; caras[3]._2 = 5;
        caras[4]._0 = 2; caras[4]._1 = 3; caras[4]._2 = 4;
        caras[5]._0 = 5; caras[5]._1 = 4; caras[5]._2 = 3;
        caras[6]._0 = 1; caras[6]._1 = 2; caras[6]._2 = 4;
        caras[7]._0 = 3; caras[7]._1 = 0; caras[7]._2 = 5;
    }

  // normales
  calcular_normales_caras();
  calcular_normales_vertices();

  //colores de las caras
  colors_random();
}


//*************************************************************************
// clase objeto ply
//*************************************************************************


_objeto_ply::_objeto_ply() 
{
   // leer lista de coordenadas de vértices y lista de indices de vértices
 
}



void _objeto_ply::parametros(char *archivo)
{
int i, n_ver,n_car;

vector<float> ver_ply ;
vector<int>   car_ply ;
 
_file_ply::read(archivo, ver_ply, car_ply );

n_ver=ver_ply.size()/3;
n_car=car_ply.size()/3;

printf("Number of vertices=%d\nNumber of faces=%d\n", n_ver, n_car);

vertices.resize(n_ver);
caras.resize(n_car);
// vértices

for (i=0;i<n_ver;i++)
  {vertices[i].x=ver_ply[3*i];
   vertices[i].y=ver_ply[3*i+1];
   vertices[i].z=ver_ply[3*i+2];
  }

// vértices
for (i=0;i<n_car;i++)
  {caras[i].x=car_ply[3*i];
   caras[i].y=car_ply[3*i+1];
   caras[i].z=car_ply[3*i+2];
  }

// normales
calcular_normales_caras();
calcular_normales_vertices();

// colores
//colors_random();
colors_diffuse_flat(1,0.5,0.0,   0,0,20);
colors_diffuse_gouraud(0,0.5,1,   0,0,20);

}


//************************************************************************
// objeto por revolucion
//************************************************************************

_rotacion::_rotacion()
{

}


void _rotacion::parametros(vector<_vertex3f> perfil, int num, 
                           int tapa_in, int tapa_su, int tipo)
{
int i,j;
_vertex3f vertice_aux;
_vertex3i cara_aux;
int num_aux;
float radio;

// tratamiento de los vértice
radio=sqrt(perfil[0].x*perfil[0].x+perfil[0].y*perfil[0].y);

num_aux=perfil.size();
if (tipo==2) num_aux=1;
vertices.resize(num_aux*num);

for (j=0;j<num;j++)
  {for (i=0;i<num_aux;i++)
     {
      vertice_aux.x=perfil[i].x*cos(2.0*M_PI*j/(1.0*num))+
                    perfil[i].z*sin(2.0*M_PI*j/(1.0*num));
      vertice_aux.z=-perfil[i].x*sin(2.0*M_PI*j/(1.0*num))+
                    perfil[i].z*cos(2.0*M_PI*j/(1.0*num));
      vertice_aux.y=perfil[i].y;
      vertices[i+j*num_aux]=vertice_aux;
     }
  }

// tratamiento de las caras 
if (tipo<2)
{
for (j=0;j<num;j++)
  {for (i=0;i<num_aux-1;i++)
     {cara_aux._0=i+j*num_aux;
      cara_aux._1=i+((j+1)%num)*num_aux;
      cara_aux._2=i+1+j*num_aux;
      caras.push_back(cara_aux);

      cara_aux._0=i+((j+1)%num)*num_aux;
      cara_aux._1=i+1+((j+1)%num)*num_aux;
      cara_aux._2=i+1+j*num_aux;
      caras.push_back(cara_aux);
     }
   }
}
 
// tapa inferior
if (tapa_in==1)
{
  // punto central de la tapa
  vertice_aux.x=0.0;
  if (tipo==1) vertice_aux.y=-radio;
  else vertice_aux.y=perfil[0].y;
  vertice_aux.z=0.0;
  vertices.push_back(vertice_aux);
  //caras tapa inferior

  cara_aux._0=num_aux*num;
  for (j=0;j<num;j++)
    {
     cara_aux._1=((j+1)%num)*num_aux;
     cara_aux._2=j*num_aux;
     caras.push_back(cara_aux);
    }

}
 
// tapa superior

if (tapa_su==1)
{
  // punto central de la tapa
  vertice_aux.x=0.0;
  vertice_aux.y=perfil[num_aux-1].y;
  if (tipo==1) vertice_aux.y=radio;
  if (tipo==2) vertice_aux.y=perfil[1].y;
  vertice_aux.z=0.0;
  vertices.push_back(vertice_aux);

  //caras tapa superior
  if (tapa_in==1) cara_aux._0=num_aux*num+1;
  else cara_aux._0=num_aux*num;

  for (j=0;j<num;j++)
    {cara_aux._1=j*num_aux+num_aux-1;
     cara_aux._2=((j+1)%num)*num_aux+num_aux-1;
     caras.push_back(cara_aux);
    }
}

// normales
calcular_normales_caras();
calcular_normales_vertices();

//colores de las caras
colors_random();
}


//************************************************************************
// objeto por extrusión
//************************************************************************

_extrusion::_extrusion(vector<_vertex3f> poligono, float x, float y, float z)
{
int i;
_vertex3f vertice_aux;
_vertex3i cara_aux;
int num_aux;

// tratamiento de los vértice

num_aux=poligono.size();
vertices.resize(num_aux*2);
for (i=0;i<num_aux;i++)
    {
      vertices[2*i]=poligono[i];
      vertices[2*i+1].x=poligono[i].x+x;
      vertices[2*i+1].y=poligono[i].y+y;
      vertices[2*i+1].z=poligono[i].z+z;
    }
    
// tratamiento de las caras 

caras.resize(num_aux*2);
int c=0;
for (i=0;i<num_aux;i++)         
  {
   caras[c]._0=i*2;
   caras[c]._1=(i*2+2)%(num_aux*2);
   caras[c]._2=i*2+1;    
   c=c+1;
   caras[c]._0=(i*2+2)%(num_aux*2);
   caras[c]._1=(i*2+2)%(num_aux*2)+1;
   caras[c]._2=i*2+1;    
   c=c+1;    
   }  
   
// normales
calcular_normales_caras();   
calcular_normales_vertices();
   
//colores de las caras
colors_random();
}

//************************************************************************
//************************************************************************

//************************************************************************
// objeto cilindro (caso especial de rotacion)
//************************************************************************

_cilindro::_cilindro(float radio, float altura, int num)
{
vector<_vertex3f> perfil;
_vertex3f aux;

aux.x=radio; aux.y=-altura/2.0; aux.z=0.0;
perfil.push_back(aux);
aux.x=radio; aux.y=altura/2.0; aux.z=0.0;
perfil.push_back(aux);
parametros(perfil,num,1,1,0);
}

//************************************************************************
// objeto cono (caso especial de rotacion)
//************************************************************************

_cono::_cono(float radio, float altura, int num)
{
vector<_vertex3f> perfil;
_vertex3f aux;

aux.x=radio; aux.y=0; aux.z=0.0;
perfil.push_back(aux);
aux.x=0.0; aux.y=altura; aux.z=0.0;
perfil.push_back(aux);
parametros(perfil,num,1,1,2);
}

//************************************************************************
// objeto esfera (caso especial de rotacion)
//************************************************************************

_esfera::_esfera(float radio, int latitud, int longitud)
{
vector<_vertex3f> perfil;
_vertex3f aux;
int i;
for (i=1;i<latitud;i++)
  {aux.x=radio*cos(M_PI*i/(latitud*1.0)-M_PI/2.0);
   aux.y=radio*sin(M_PI*i/(latitud*1.0)-M_PI/2.0);
   aux.z=0.0;
   perfil.push_back(aux);
  }
parametros(perfil,longitud,1,1,1);

colores_degradado(0,1,0.92,0,0.35,0.68);
}

//************************************************************************
// objeto trofeo
//************************************************************************

_trofeo::_trofeo(int lados)
{
  vector<_vertex3f> perfil;
  _vertex3f aux;
  aux.z=0.0;

    aux.x=0.75;
    aux.y=-1.25;
    perfil.push_back(aux);

    aux.x=0.5;
    aux.y=-1.1;
    perfil.push_back(aux);

    aux.x=0.5;
    aux.y=-0.7;
    perfil.push_back(aux);

    aux.x=0.3;
    aux.y=-0.4;
    perfil.push_back(aux);

    aux.x=0.3;
    aux.y=0.5;
    perfil.push_back(aux);

    aux.x=0.5;
    aux.y=0.6;
    perfil.push_back(aux);

    aux.x=0.3;
    aux.y=0.6;
    perfil.push_back(aux);

    aux.x=0.5;
    aux.y=0.8;
    perfil.push_back(aux);

    aux.x=0.6;
    aux.y=0.9;
    perfil.push_back(aux);

    aux.x=0.7;
    aux.y=1;
    perfil.push_back(aux);

    aux.x=0.8;
    aux.y=1.1;
    perfil.push_back(aux);

    aux.x=0.9;
    aux.y=1.2;
    perfil.push_back(aux);

    aux.x=0.95;
    aux.y=1.3;
    perfil.push_back(aux);

    aux.x=1;
    aux.y=1.4;
    perfil.push_back(aux);

    aux.x=0.95;
    aux.y=1.5;
    perfil.push_back(aux);

    aux.x=0.9;
    aux.y=1.6;
    perfil.push_back(aux);

    aux.x=0.8;
    aux.y=1.7;
    perfil.push_back(aux);

    aux.x=0.7;
    aux.y=1.8;
    perfil.push_back(aux);

    aux.x=0.35;
    aux.y=1.9;
    perfil.push_back(aux);

    aux.x=0.15;
    aux.y=2;
    perfil.push_back(aux);

  parametros(perfil,lados,1,1,0);

  colors_chess(0.4, 1, 0.2, 0.1, 0.1, 0.9);
}

//************************************************************************
// objeto bombilla
//************************************************************************

_bombilla::_bombilla(int lados)
{
  vector<_vertex3f> perfil;
  _vertex3f aux;
  aux.z=0.0;

    aux.x=0.05;
    aux.y=-0.1;
    perfil.push_back(aux);

    aux.x=0.08;
    aux.y=-0.075;
    perfil.push_back(aux);

    aux.x=0.08;
    aux.y=0.075;
    perfil.push_back(aux);

    aux.x=0.13;
    aux.y=0.125;
    perfil.push_back(aux);

    aux.x=0.18;
    aux.y=0.175;
    perfil.push_back(aux);

    aux.x=0.21;
    aux.y=0.2;
    perfil.push_back(aux);

    aux.x=0.26;
    aux.y=0.25;
    perfil.push_back(aux);

    aux.x=0.3;
    aux.y=0.3;
    perfil.push_back(aux);

    aux.x=0.33;
    aux.y=0.32;
    perfil.push_back(aux);

    aux.x=0.4;
    aux.y=0.4;
    perfil.push_back(aux);

    aux.x=0.425;
    aux.y=0.5;
    perfil.push_back(aux);

    aux.x=0.4;
    aux.y=0.6;
    perfil.push_back(aux);

    aux.x=0.35;
    aux.y=0.7;
    perfil.push_back(aux);

    aux.x=0.275;
    aux.y=0.8;
    perfil.push_back(aux);

    aux.x=0.2;
    aux.y=0.85;
    perfil.push_back(aux);

    aux.x=0.15;
    aux.y=0.875;
    perfil.push_back(aux);

    aux.x=0.05;
    aux.y=0.9;
    perfil.push_back(aux);

  parametros(perfil,lados,1,1,0);
  colores_altura();
}


//************************************************************************
// rotacion archivo PLY (caso especial de rotacion)
//************************************************************************

_rotacion_PLY::_rotacion_PLY()
{

}

void _rotacion_PLY::parametros_PLY(char *archivo, int num){
  unsigned long int n_ver, i;
  vector<_vertex3f> perfil;
  _vertex3f aux;

  vector<float> ver_ply ;
  vector<int> car_ply ;
  
  _file_ply::read(archivo, ver_ply, car_ply);

  n_ver=ver_ply.size()/3;

  printf("Number of vertices=%d\n", n_ver);

  vertices.resize(n_ver);

  // vertices
  for(i=0; i<vertices.size();i++){
    aux.x=ver_ply[3*i];
    aux.y=ver_ply[3*i+1];
    aux.z=ver_ply[3*i+2];
    perfil.push_back(aux);
  }

  parametros(perfil, num, 0, 1,0);

  // normales
  calcular_normales_caras();   
  calcular_normales_vertices();

  colores_degradado(0.722, 0.71, 0.651, 0.341, 0.333, 0.298);
  
}


//************************************************************************
// objeto montaña fractal
//************************************************************************

float gauss(float ga, float gf )
{
float sum;
int i;
sum=0.0;
for (i=0;i<4;i++) sum=sum+rand()%32767;
return gf*sum/4.0-ga;
}


_montana::_montana(int nivelmax, float sigma, float h)

{

if (nivelmax>8) nivelmax=8;
int i,j,etapa;
float ga=sqrt(12.0);
float gf=2*ga/(32767*1.0);
int num=pow(2,nivelmax)+1;
srand (time(NULL));

vertices.resize(num*num);

for (j=0;j<num;j++)
  for (i=0;i<num;i++)
    {vertices[i+j*num].x=-0.1*(num-1)/2.0+i*0.1;
     vertices[i+j*num].z=-0.1*(num-1)/2.0+j*0.1;
     vertices[i+j*num].y=0.0;
    }

vertices[0].y=sigma*gauss(ga,gf);
vertices[num-1].y=sigma*gauss(ga,gf);
vertices[num*(num-1)].y=sigma*gauss(ga,gf);
vertices[num*num-1].y=sigma*gauss(ga,gf);

int d1=num-1;
int d2=(num-1)/2;

for (etapa=0;etapa<nivelmax;etapa++)
   {
    sigma=sigma*pow(0.5,0.5*h);
    for (j=d2;j<num-d2;j=j+d1)
       for (i=d2;i<num-d2;i=i+d1)
         {vertices[i+j*num].y=sigma*gauss(ga,gf)+
             (vertices[i+d2+(j+d2)*num].y+vertices[i+d2+(j-d2)*num].y+
              vertices[i-d2+(j+d2)*num].y+vertices[i-d2+(j-d2)*num].y)/4.0;
         }
     sigma=sigma*pow(0.5,0.5*h);
     for (i=d2;i<num-d2;i=i+d1)
	{vertices[i].y=sigma*gauss(ga,gf)+(vertices[i+d2].y+
                     vertices[i-d2].y+vertices[i+d2*num].y)/3.0;
         vertices[i+num*(num-1)].y=sigma*gauss(ga,gf)+
                    (vertices[i+d2+num*(num-1)].y+
                     vertices[i-d2+num*(num-1)].y+
                     vertices[i+(num-1-d2)*num].y)/3.0;
	 vertices[i*num].y=sigma*gauss(ga,gf)+(vertices[(i+d2)*num].y+
                     vertices[(i-d2)*num].y+vertices[d2+i*num].y)/3.0;
         vertices[num-1+i*num].y=sigma*gauss(ga,gf)+
                    (vertices[num-1+(i+d2)*num].y+
                     vertices[num-1+(i-d2)*num].y+
                     vertices[num-1-d2+i*num].y)/3;
	}
      	
     for (j=d2;j<num-d2;j=j+d1)
	for (i=d1;i<num-d2;i=i+d1)
          vertices[i+j*num].y=sigma*gauss(ga,gf)+
                   (vertices[i+(j+d2)*num].y+vertices[i+(j-d2)*num].y+
                    vertices[i+d2+j*num].y+vertices[i-d2+j*num].y)/4.0;
     for (j=d1;j<num-d2;j=j+d1)
	for (i=d2;i<num-d2;i=i+d1)
	  vertices[i+j*num].y=sigma*gauss(ga,gf)+
                   (vertices[i+(j+d2)*num].y+vertices[i+(j-d2)*num].y+
                    vertices[i+d2+j*num].y+vertices[i-d2+j*num].y)/4.0;

     d1=(int)d1/2;
     d2=(int)d2/2;
    }


// caras 
caras.resize((num-1)*(num-1)*2);
int c=0; 
for (j=0;j<num-1;j++)
  for (i=0;i<num-1;i++)
    {caras[c]._0=i+j*num;
     caras[c]._1=i+1+j*num; 
     caras[c]._2=i+1+(j+1)*num;
     c=c+1; 
     caras[c]._0=i+j*num;
     caras[c]._1=i+1+(j+1)*num; 
     caras[c]._2=i+(j+1)*num;
     c=c+1;
    }

// normales
calcular_normales_caras();
calcular_normales_vertices();

//colores de las caras
colors_chess(0.2,1.0,0.2,0.3,0.8,0.1);

}


//************************************************************************
// práctica 3, objeto jerárquico articulado
//************************************************************************

//************************************************************************
// piezas
//************************************************************************

//************************************************************************
// pala
//************************************************************************

_pala::_pala(float radio, float ancho, int num)
{
vector<_vertex3f> perfil;
_vertex3f vertice_aux;
_vertex3i cara_aux;
int i, j;

vertice_aux.x=radio; vertice_aux.y=0; vertice_aux.z=-ancho/2.0;
perfil.push_back(vertice_aux);
vertice_aux.z=ancho/2.0;
perfil.push_back(vertice_aux);

// tratamiento de los vértices

for (j=0;j<=num;j++)
  {for (i=0;i<2;i++)	
     {
      vertice_aux.x=perfil[i].x*cos(M_PI*j/(1.0*num))-
                    perfil[i].y*sin(M_PI*j/(1.0*num));
      vertice_aux.y=perfil[i].x*sin(M_PI*j/(1.0*num))+
                    perfil[i].y*cos(M_PI*j/(1.0*num));
      vertice_aux.z=perfil[i].z;
      vertices.push_back(vertice_aux);
     }
  }
  
// tratamiento de las caras 

for (j=0;j<num;j++)
   {cara_aux._0=j*2;
    cara_aux._1=(j+1)*2;
    cara_aux._2=(j+1)*2+1;
    caras.push_back(cara_aux);
    
    cara_aux._0=j*2;
    cara_aux._1=(j+1)*2+1;
    cara_aux._2=j*2+1;
    caras.push_back(cara_aux);
   }
   
// tapa inferior
vertice_aux.x=0;
vertice_aux.y=0;
vertice_aux.z=-ancho/2.0;
vertices.push_back(vertice_aux); 

for (j=0;j<num;j++)
   {cara_aux._0=j*2;
    cara_aux._1=(j+1)*2;
    cara_aux._2=vertices.size()-1;
    caras.push_back(cara_aux);
   }
  
// tapa superior
vertice_aux.x=0;
vertice_aux.y=0;
vertice_aux.z=ancho/2.0;
vertices.push_back(vertice_aux); 

for (j=0;j<num;j++)
   {cara_aux._0=j*2+1;
    cara_aux._1=(j+1)*2+1;
    cara_aux._2=vertices.size()-1;
    caras.push_back(cara_aux);
   }

// normales
calcular_normales_caras();
calcular_normales_vertices();
  
colors_chess(1.0,1.0,0.0,0.0,0.0,1.0);
}

//************************************************************************
// brazo
//************************************************************************

_brazo::_brazo()
{
ancho=0.6;
alto=0.1;
fondo=0.1;
colors_chess(1.0,1.0,0.0,0.0,0.0,1.0);

cubo.ambiente = _vertex4f(0.0,0.4,0.8,1.0);  // Coeficiente ambiente
cubo.difusa = _vertex4f(0.0,0.4,0.8,1.0);    // Coeficiente difuso
cubo.especular = _vertex4f(0.5,0.5,0.5,1.0); // Coeficiente especular
cubo.brillo = 10.0;                          // Exponente del brillo

};

void _brazo::draw(_modo modo, float r, float g, float b, float grosor)
{
glPushMatrix();
glScalef(ancho, alto, fondo);
glTranslatef(0.5,0,0);
cubo.draw(modo, r, g, b, grosor);
glPopMatrix();
};

//************************************************************************
// cabina
//************************************************************************

_cabina::_cabina()
{
ancho=0.4;
alto=0.6;
fondo=0.4;
cubo.colors_chess(1.0,1.0,0.0,0.0,0.0,1.0);
};

void _cabina::draw(_modo modo, float r, float g, float b, float grosor)
{
glPushMatrix();
glScalef(ancho, alto, fondo);
cubo.draw(modo, r, g, b, grosor);
glPopMatrix();
};

//************************************************************************
// sustentación
//************************************************************************

_sustentacion::_sustentacion()
{
ancho=1.2;
alto=0.3;
fondo=0.8;
radio=0.15;
base.colors_chess(1.0,1.0,0.0,0.0,0.0,1.0);
};

void _sustentacion::draw(_modo modo, float r, float g, float b, float grosor)
{
glPushMatrix();
glTranslatef(2*ancho/6,-alto/2.0,0);
glRotatef(90,1,0,0);
glScalef(radio, fondo/2.2, radio);
rueda.draw(modo, r, g, b, grosor);
glPopMatrix();

glPushMatrix();
glTranslatef(-2*ancho/6,-alto/2.0,0);
glRotatef(90,1,0,0);
glScalef(radio, fondo/2.2, radio);
rueda.draw(modo, r, g, b, grosor);
glPopMatrix();

glPushMatrix();
glScalef(ancho, alto, fondo);
base.draw(modo, r, g, b, grosor);
glPopMatrix();
};

//************************************************************************
// excavadora (montaje del objeto final)
//************************************************************************

_excavadora::_excavadora()
{
giro_cabina = 0.0;
giro_primer_brazo = 0.0;
giro_primer_brazo_max = 0;
giro_primer_brazo_min = -90;
giro_segundo_brazo = 0.0;
giro_segundo_brazo_max = 30;
giro_segundo_brazo_min = 0;
giro_pala = 0.0;
giro_pala_max = 50.0;
giro_pala_min = -90.0;

tamanio_pala=0.15;

int color=80;
piezas=5;
grosor_select=2;
color_pick=_vertex3f(1.0,0.0,0.0); 
color_select.resize(piezas);
activo.resize(piezas);

for (int i=0;i<piezas;i++)
  {
   activo[i]=0;
   color_select[i].r=color_select[i].g=color_select[i].b=color;
   color+=20;
  }
};


void _excavadora::draw(_modo modo, float r, float g, float b, float grosor)
{
float r_p,g_p,b_p;

r_p=color_pick.r;
g_p=color_pick.g;
b_p=color_pick.b;

glPushMatrix();

if (activo[0]==1) sustentacion.draw(EDGES, r_p, g_p, b_p, grosor_select);
else sustentacion.draw(modo, r, g, b, grosor);

glTranslatef(0,(cabina.alto+sustentacion.alto)/2.0,0);
glRotatef(giro_cabina,0,1,0);
if (activo[1]==1) cabina.draw(EDGES, r_p, g_p, b_p, grosor_select);
else cabina.draw(modo, r, g, b, grosor);

glTranslatef(cabina.ancho/2.0,0,0);
glRotatef(giro_segundo_brazo,0,0,1);
if (activo[2]==1) brazo.draw(EDGES, r_p, g_p, b_p, grosor_select);
else brazo.draw(modo, r, g, b, grosor);

glTranslatef(brazo.ancho,0,0);
glRotatef(giro_primer_brazo,0,0,1);
if (activo[3]==1) brazo.draw(EDGES, r_p, g_p, b_p, grosor_select);
else brazo.draw(modo, r, g, b, grosor);

glTranslatef(brazo.ancho,0,0);
glRotatef(giro_pala,0,0,1);
glTranslatef(tamanio_pala,0,0);
glScalef(tamanio_pala, tamanio_pala, tamanio_pala);
if (activo[4]==1) pala.draw(EDGES, r_p, g_p, b_p, grosor_select);
else pala.draw(modo, r, g, b, grosor);

glPopMatrix();
};

//************************************************************************

void _excavadora::seleccion()
{
_vertex3i color;

glPushMatrix();
color=color_select[0];
sustentacion.draw(SELECT, color.r, color.g, color.b, 1);

glTranslatef(0,(cabina.alto+sustentacion.alto)/2.0,0);
glRotatef(giro_cabina,0,1,0);
color=color_select[1];
cabina.draw(SELECT, color.r, color.g, color.b, 1);

glTranslatef(cabina.ancho/2.0,0,0);
glRotatef(giro_segundo_brazo,0,0,1);
color=color_select[2];
brazo.draw(SELECT, color.r, color.g, color.b, 1);

glTranslatef(brazo.ancho,0,0);
glRotatef(giro_primer_brazo,0,0,1);
color=color_select[3];
brazo.draw(SELECT, color.r, color.g, color.b, 1);

glTranslatef(brazo.ancho,0,0);
glRotatef(giro_pala,0,0,1);
glTranslatef(tamanio_pala,0,0);
glScalef(tamanio_pala, tamanio_pala, tamanio_pala);
color=color_select[4];
pala.draw(SELECT, color.r, color.g, color.b, 1);

glPopMatrix();
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

_tronco_cono::_tronco_cono(float radio_inf, float radio_sup, float altura, int num)
{
vector<_vertex3f> perfil;
_vertex3f aux;

aux.x=radio_inf; aux.y=-altura/2.0; aux.z=0.0;
perfil.push_back(aux);
aux.x=radio_sup; aux.y=altura/2.0; aux.z=0.0;
perfil.push_back(aux);
parametros(perfil,num,1,1,0);
}

//************************************************************************
// objeto semiesfera (caso especial de rotacion)
//************************************************************************

_semiesfera::_semiesfera(float radio, int latitud, int longitud)
{
vector<_vertex3f> perfil;
_vertex3f aux;
int i;
for (i=1;i<=latitud/2;i++)
  {aux.x=radio*cos(M_PI*i/(latitud*1.0)-M_PI/2.0);
   aux.y=radio*sin(M_PI*i/(latitud*1.0)-M_PI/2.0);
   aux.z=0.0;
   perfil.push_back(aux);
  }
parametros(perfil,longitud,1,1,0);
}

//************************************************************************
// objeto semiesfera hueca (caso especial de rotacion)
//************************************************************************

_semiesfera_hueca::_semiesfera_hueca(float radio, int latitud, int longitud)
{
vector<_vertex3f> perfil;
_vertex3f aux;
int i;
for (i=1;i<=latitud/2;i++)
  {aux.x=radio*cos(M_PI*i/(latitud*1.0)-M_PI/2.0);
   aux.y=radio*sin(M_PI*i/(latitud*1.0)-M_PI/2.0);
   aux.z=0.0;
   perfil.push_back(aux);
  }
parametros(perfil,longitud,1,0,0);
}

//************************************************************************
// piezas
//************************************************************************

//************************************************************************
// Estructura P38
//************************************************************************

_estructura::_estructura(){
  estructura_central.colores_solido(0.55,0.55,0.55);
  estructura_lateral.colores_solido(0.55,0.55,0.55);
  union_estructuras.colores_solido(0.625,0.625,0.625);
  escape.colores_solido(0.65,0.65,0.65);
  complemento.colores_solido(0,0,0.55);
}

void _estructura::draw(_modo modo, float r, float g, float b, float grosor){

// Acabados en redondo (parte trasera)
glPushMatrix();
glTranslatef(3,0,-1.75);
glRotatef(90,0,0,1);
glScalef(0.125,0.125,0.125);
complemento.draw(modo, r, g, b, grosor);
glPopMatrix();

glPushMatrix();
glTranslatef(3,0,1.75);
glRotatef(90,0,0,1);
glScalef(0.125,0.125,0.125);
complemento.draw(modo, r, g, b, grosor);
glPopMatrix();

// Acabados en redondo (parte delantera)
glPushMatrix();
glTranslatef(-1,0,-1.75);
glRotatef(-90,0,0,1);
glScalef(0.375,0.15,0.375);
complemento.draw(modo, r, g, b, grosor);
glPopMatrix();

glPushMatrix();
glTranslatef(-1,0,1.75);
glRotatef(-90,0,0,1);
glScalef(0.375,0.15,0.375);
complemento.draw(modo, r, g, b, grosor);
glPopMatrix();

// Escapes de un lado
glPushMatrix();
glTranslatef(1.5,0,1.95);
glRotatef(90,0,0,1);
glScalef(0.15,0.5,0.15);
escape.draw(modo, r, g, b, grosor);
glPopMatrix();

glPushMatrix();
glTranslatef(1.5,0,1.55);
glRotatef(90,0,0,1);
glScalef(0.15,0.5,0.15);
escape.draw(modo, r, g, b, grosor);
glPopMatrix();

// Escapes del otro lado
glPushMatrix();
glTranslatef(1.5,0,-1.95);
glRotatef(90,0,0,1);
glScalef(0.15,0.5,0.15);
escape.draw(modo, r, g, b, grosor);
glPopMatrix();

glPushMatrix();
glTranslatef(1.5,0,-1.55);
glRotatef(90,0,0,1);
glScalef(0.15,0.5,0.15);
escape.draw(modo, r, g, b, grosor);
glPopMatrix();

// Estructura lateral de un lado
glPushMatrix();
glTranslatef(1,0,-1.75);
glRotatef(-90,0,1,0);
glRotatef(-90,1,0,0);
glScalef(0.5,2,0.5);
estructura_lateral.draw(modo, r, g, b, grosor);
glPopMatrix();

// Estructura lateral del otro lado
glPushMatrix();
glTranslatef(1,0,1.75);
glRotatef(-90,0,1,0);
glRotatef(-90,1,0,0);
glScalef(0.5,2,0.5);
estructura_lateral.draw(modo, r, g, b, grosor);
glPopMatrix();

// Union de un lado
glPushMatrix();
glTranslatef(0,0,-0.05);
glRotatef(-90,1,0,0);
glScalef(1.75,2,0.25);
union_estructuras.draw(modo, r, g, b, grosor);
glPopMatrix();

// Union del otro lado
glPushMatrix();
glTranslatef(0,0,0.05);
glRotatef(90,1,0,0);
glScalef(1.75,2,0.25);
union_estructuras.draw(modo, r, g, b, grosor);
glPopMatrix();

// Estructura central
glPushMatrix();
glTranslatef(-0.8,0,0);
glScalef(1.5,0.5,0.5);
estructura_central.draw(modo, r, g, b, grosor);
glPopMatrix();

};

//************************************************************************
// Módulo de vuelo (cabina piloto)
//************************************************************************

_modulo::_modulo(){
  cupula.colores_solido(0,0.78,1);
}

void _modulo::draw(_modo modo, float r, float g, float b, float grosor){
glPushMatrix();
glRotatef(180,1,0,0);
glScalef(0.5,0.3,0.25);
cupula.draw(modo, r, g, b, grosor);
glPopMatrix();
}

//************************************************************************
// Rueda
//************************************************************************

_rueda::_rueda(){
  neumatico.colores_solido(0,0,0);
  eje.colores_solido(0.65,0.65,0.65);
  llanta.colores_solido(0.7,0.7,0.7);
}

void _rueda::draw(_modo modo, float r, float g, float b, float grosor){

glPushMatrix();
glTranslatef(0,0,0.075);
glRotatef(90,1,0,0);
glScalef(0.085,0.001,0.085);
llanta.draw(modo, r, g, b, grosor);
glPopMatrix();

glPushMatrix();
glTranslatef(0,0,-0.075);
glRotatef(90,1,0,0);
glScalef(0.085,0.001,0.085);
llanta.draw(modo, r, g, b, grosor);
glPopMatrix();

glPushMatrix();
glRotatef(90,1,0,0);
glScalef(0.08,0.025,0.08);
eje.draw(modo, r, g, b, grosor);
glPopMatrix();

glPushMatrix();
glTranslatef(0,0,0.05);
glRotatef(90,1,0,0);
glScalef(0.12,0.025,0.12);
neumatico.draw(modo, r, g, b, grosor);
glPopMatrix();

glPushMatrix();
glTranslatef(0,0,-0.05);
glRotatef(90,1,0,0);
glScalef(0.12,0.025,0.12);
neumatico.draw(modo, r, g, b, grosor);
glPopMatrix();
}

//************************************************************************
// Rueda principal
//************************************************************************

_rueda_principal::_rueda_principal(){
  eje.colores_solido(0.8,0.8,0.8);
}

void _rueda_principal::draw(_modo modo, float r, float g, float b, float grosor){

glPushMatrix();
glTranslatef(0,-0.25,0);
rueda.draw(modo, r, g, b, grosor);
glTranslatef(0,0.2,0);
glScalef(0.03,0.125,0.03);
eje.draw(modo, r, g, b, grosor);
glPopMatrix();
}

//************************************************************************
// Helices
//************************************************************************

_helices::_helices(){
  aspa = _tronco_piramide(1,0.3,1);
  aspa.colores_solido(0,0,0);
  soporte.colores_solido(0.65,0.65,0.65);
}

void _helices::draw(_modo modo, float r, float g, float b, float grosor){

glPushMatrix();
glTranslatef(4.35,0.5,0);
glRotatef(-90,0,1,0);
glRotatef(90,1,0,0);
glScalef(1.5,4,0.15);
aspa.draw(modo, r, g, b, grosor);
glPopMatrix();

glPushMatrix();
glTranslatef(-2.25,0.5,-3.75);
glRotatef(30,0,1,0);
glRotatef(90,1,0,0);
glScalef(1.5,4,0.15);
aspa.draw(modo, r, g, b, grosor);
glPopMatrix();

glPushMatrix();
glTranslatef(-2.25,0.5,3.75);
glRotatef(-30,0,1,0);
glRotatef(-90,1,0,0);
glScalef(1.5,4,0.15);
aspa.draw(modo, r, g, b, grosor);
glPopMatrix();

glPushMatrix();
glScalef(1,0.5,1);
soporte.draw(modo, r, g, b, grosor);
glPopMatrix();
}

//************************************************************************
// Ala
//************************************************************************

_ala::_ala(){
  estructura_ala = _tronco_cono(0.75,0.35,1.5);
  estructura_ala.colores_solido(0.65,0.65,0.65);
  punta.colores_solido(0.65,0.65,0.65);
}

void _ala::draw(_modo modo, float r, float g, float b, float grosor){

glPushMatrix();
glScalef(2.5,4.5,0.35);
estructura_ala.draw(modo, r, g, b, grosor);
glTranslatef(0,0.75,0);
glRotatef(180,1,0,0);
glScalef(0.35,0.15,0.35);
punta.draw(modo, r, g, b, grosor);
glPopMatrix();
}

//************************************************************************
// Estructura del aleron
//************************************************************************

_estructura_aleron::_estructura_aleron(){
  base.colores_solido(0.65,0.65,0.65);
}

void _estructura_aleron::draw(_modo modo, float r, float g, float b, float grosor){

glPushMatrix();
glScalef(0.35,0.125,3.25);
base.draw(modo, r, g, b, grosor);
glPopMatrix();
}

//************************************************************************
// Aleta
//************************************************************************

_aleta::_aleta(){
  base.colores_solido(0.65,0.65,0.65);
}

void _aleta::draw(_modo modo, float r, float g, float b, float grosor){

glPushMatrix();
glScalef(0.35,0.65,0.1);
base.draw(modo, r, g, b, grosor);
glPopMatrix();
}

//************************************************************************
// Cañon
//************************************************************************

_canon::_canon(){
  base = _cilindro(0.25,2);
  complemento.colores_solido(0.25,0.25,0.5);
  base.colores_solido(0.25,0.25,0.5);  
}

void _canon::draw(_modo modo, float r, float g, float b, float grosor){

glPushMatrix();
glRotatef(90,1,0,0);
glScalef(0.25,0.25,0.25);
complemento.draw(modo, r, g, b, grosor);
glPopMatrix();

glPushMatrix();
glTranslatef(0,-0.5,0);
glScalef(1,0.5,1);
base.draw(modo, r, g, b, grosor);
glPopMatrix();

glPushMatrix();
glTranslatef(0,0,1);
glRotatef(90,1,0,0);
base.draw(modo, r, g, b, grosor);
glPopMatrix();

}

//************************************************************************
// Llama
//************************************************************************

_llama::_llama(){
  fuego_centro.colores_solido(0.9,0.1,0.1);
  fuego.colores_solido(0.8,0.25,0);
  fuego_pequeño.colores_solido(0.8,0.6,0.2);
}

void _llama::draw(_modo modo, float r, float g, float b, float grosor){

glPushMatrix();
glScalef(1,1.35,1);
fuego_centro.draw(modo, r, g, b, grosor);
glPopMatrix();

glPushMatrix();
glTranslatef(0.25,0.75,0.25);
glRotatef(-15,0,0,1);
glRotatef(15,1,0,0);
glScalef(0.5,0.7,0.5);
fuego_pequeño.draw(modo, r, g, b, grosor);
glPopMatrix();

glPushMatrix();
glTranslatef(-0.25,0.75,-0.25);
glRotatef(15,0,0,1);
glRotatef(-15,1,0,0);
glScalef(0.5,0.7,0.5);
fuego_pequeño.draw(modo, r, g, b, grosor);
glPopMatrix();

glPushMatrix();
glTranslatef(0.25,0.75,-0.25);
glRotatef(-15,1,0,1);
glScalef(0.5,0.7,0.5);
fuego_pequeño.draw(modo, r, g, b, grosor);
glPopMatrix();

glPushMatrix();
glTranslatef(-0.25,0.75,0.25);
glRotatef(15,1,0,1);
glScalef(0.5,0.7,0.5);
fuego_pequeño.draw(modo, r, g, b, grosor);
glPopMatrix();

glPushMatrix();
glTranslatef(-0.325,0.375,0);
glRotatef(30,0,0,1);
glScalef(0.75,0.85,0.75);
fuego.draw(modo, r, g, b, grosor);
glPopMatrix();

glPushMatrix();
glTranslatef(0.325,0.375,0);
glRotatef(-30,0,0,1);
glScalef(0.75,0.85,0.75);
fuego.draw(modo, r, g, b, grosor);
glPopMatrix();

glPushMatrix();
glTranslatef(0,0.375,-0.325);
glRotatef(-30,1,0,0);
glScalef(0.75,0.85,0.75);
fuego.draw(modo, r, g, b, grosor);
glPopMatrix();

glPushMatrix();
glTranslatef(0,0.375,0.325);
glRotatef(30,1,0,0);
glScalef(0.75,0.85,0.75);
fuego.draw(modo, r, g, b, grosor);
glPopMatrix();

}


//************************************************************************
// P38 (montaje del objeto final)
//************************************************************************

_p38::_p38()
{
  union_rueda.colores_solido(0.8,0.8,0.8);
  caso = 0;
  giro_helices = 0;
  subida_aleron = 0;
  subida_aleron_max = 45;
  subida_aleron_min = 0;
  cierre_alas = 0;
  cierre_alas_max = 37.5;
  cierre_alas_min = 0;
  rota_rueda = 0;
  rota_rueda_max = 55;
  rota_rueda_min = 0;
  guarda_rueda = 0;
  guarda_rueda_max = 0.275;
  guarda_rueda_min = 0;
  desplazamiento = 0;
  desplazamiento_max = 5;
  despegue = 0;
  despegue_max = -30;
  diagonal = 0;
  diagonal_max = 5;
  caida = 0;
  caida_max = -25;
  incendio = 0;
  incendio_max = 3.5;
  movimiento = 0;
  movimiento_max = -5;
  movimiento_min = 5;
  tex_suelo = 0;
  tex_cielo = 0;
  tex_estrella = 0;

  int color=0;
  piezas=22;
  grosor_select=0.5;
  color_pick=_vertex3f(0.6,0.0,0.2); 
  color_select.resize(piezas);
  activo.resize(piezas);

  for (int i=0;i<piezas;i++){
    activo[i]=0;
    color_select[i].r=color_select[i].g=color_select[i].b=color;
    color+=1;
  }
};

void _p38::draw(_modo modo, float r, float g, float b, float grosor)
{

  float r_p,g_p,b_p;
  r_p=color_pick.r;
  g_p=color_pick.g;
  b_p=color_pick.b;

if(caso == 4){
    glPushMatrix();

    glTranslatef(-40,0,0);

    glPushMatrix();
    glScalef(incendio,incendio,incendio);
    llama.draw(modo, r, g, b, grosor);
    glPopMatrix();
    
    glPushMatrix();

    glTranslatef(0,0.5,0);

    // Ala derecha
    glPushMatrix();
    glTranslatef(0,0,-2.5);
    glRotatef(-90,1,0,0);
    glScalef(0.3,0.3,0.3);
    ala.draw(modo, r, g, b, grosor);
    glPopMatrix();

    // Helices parte izquierda
    glPushMatrix();
    glTranslatef(-1.1,0,1.75);
    glRotatef(45,1,0,0);
    glRotatef(-90,0,1,0);
    glRotatef(90,1,0,0);
    glScalef(0.2,0.2,0.2);
    helices.draw(modo, r, g, b, grosor);
    glPopMatrix();

    // Cabina
    glPushMatrix();
    glTranslatef(-0.35,0.4,0);
    glRotatef(-5,0,0,1);
    modulo.draw(modo, r, g, b, grosor);
    glPopMatrix();

    // Estructura
    glPushMatrix();
    estructura.draw(modo, r, g, b, grosor);
    glPopMatrix();

    glPopMatrix();

    glPopMatrix();


  }
  else {
    glPushMatrix();

    glTranslatef(-diagonal-desplazamiento, diagonal+caida, 0);
    glTranslatef(-diagonal-desplazamiento, diagonal, 0);
    glRotatef(despegue,0,0,1);
    glTranslatef(-desplazamiento,0,0);
    glTranslatef(movimiento,0,0);

    if(modo == SOLID_COLORS && activo[0]==0) {
      glPushMatrix();
      glTranslatef(0,0.0825,-0.875);
      glRotatef(-1,1,0,0);
      glRotatef(-90,0,1,0);
      glScalef(0.6,0.6,0.6);
      estrella.draw_solido_textura(tex_estrella);
      estrella = _base_textura(1,0.5);
      glPopMatrix();

      glPushMatrix();
      glTranslatef(0,0.0825,0.875);
      glRotatef(1,1,0,0);
      glRotatef(-90,0,1,0);
      glScalef(0.6,0.6,0.6);
      estrella.draw_solido_textura(tex_estrella);
      estrella = _base_textura(1,0.5);
      glPopMatrix();
    }

    // Cañon derecho
    glPushMatrix();
    glTranslatef(-1.85,0.25,-0.35);
    glRotatef(-90,1,0,0);
    glRotatef(-90,0,1,0);
    glScalef(0.15,0.15,0.15);
    if(activo[21]==1) canon.draw(EDGES,r_p,g_p,b_p,grosor_select);
    else canon.draw(modo, r, g, b, grosor);
    glPopMatrix();

    // Cañon izquierdo
    glPushMatrix();
    glTranslatef(-1.85,0.25,0.35);
    glRotatef(90,1,0,0);
    glRotatef(-90,0,1,0);
    glScalef(0.15,0.15,0.15);
    if(activo[20]==1) canon.draw(EDGES,r_p,g_p,b_p,grosor_select);
    else canon.draw(modo, r, g, b, grosor);
    glPopMatrix();

    // Cañon central
    glPushMatrix();
    glTranslatef(-1.85,0.45,0);
    glRotatef(-90,0,1,0);
    glScalef(0.15,0.15,0.15);
    if(activo[19]==1) canon.draw(EDGES,r_p,g_p,b_p,grosor_select);
    else canon.draw(modo, r, g, b, grosor);
    glPopMatrix();

    // Aletas derechas
    glPushMatrix();

    glTranslatef(2.825,0,-1.75);
    glScalef(0.5,0.5,0.5);

    glPushMatrix();
    glTranslatef(0,-0.25,0);
    if(activo[18]==1) aleta.draw(EDGES,r_p,g_p,b_p,grosor_select);
    else aleta.draw(modo, r, g, b, grosor);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0,0,-0.25);
    glRotatef(90,1,0,0);
    if(activo[17]==1) aleta.draw(EDGES,r_p,g_p,b_p,grosor_select);
    else aleta.draw(modo, r, g, b, grosor);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0,0.25,0);
    glRotatef(180,1,0,0);
    if(activo[16]==1) aleta.draw(EDGES,r_p,g_p,b_p,grosor_select);
    else aleta.draw(modo, r, g, b, grosor);
    glPopMatrix();

    glPopMatrix();

    // Aletas izquierdas
    glPushMatrix();

    glTranslatef(2.825,0,1.75);
    glScalef(0.5,0.5,0.5);

    glPushMatrix();
    glTranslatef(0,-0.25,0);
    if(activo[15]==1) aleta.draw(EDGES,r_p,g_p,b_p,grosor_select);
    else aleta.draw(modo, r, g, b, grosor);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0,0,0.25);
    glRotatef(-90,1,0,0);
    if(activo[14]==1) aleta.draw(EDGES,r_p,g_p,b_p,grosor_select);
    else aleta.draw(modo, r, g, b, grosor);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0,0.25,0);
    glRotatef(180,1,0,0);
    if(activo[13]==1) aleta.draw(EDGES,r_p,g_p,b_p,grosor_select);
    else aleta.draw(modo, r, g, b, grosor);
    glPopMatrix();

    glPopMatrix();

    // Aleron
    glPushMatrix();
    glTranslatef(2.6,0,0);
    glRotatef(subida_aleron,0,0,1);
    if(activo[12]==1) aleron.draw(EDGES,r_p,g_p,b_p,grosor_select);
    else aleron.draw(modo, r, g, b, grosor);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(2.85,0,0);
    glRotatef(subida_aleron,0,0,1);
    if(activo[11]==1) aleron.draw(EDGES,r_p,g_p,b_p,grosor_select);
    else aleron.draw(modo, r, g, b, grosor);
    glPopMatrix();

    // Ala izquierda
    glPushMatrix();
    glTranslatef(0,0,2.5);
    glRotatef(cierre_alas,0,1,0);
    glRotatef(90,1,0,0);
    glScalef(0.3,0.3,0.3);
    if(activo[10]==1) ala.draw(EDGES,r_p,g_p,b_p,grosor_select);
    else ala.draw(modo, r, g, b, grosor);
    glPopMatrix();

    // Ala derecha
    glPushMatrix();
    glTranslatef(0,0,-1.5);
    glRotatef(-cierre_alas,0,1,0);
    glTranslatef(0,0,-1);
    glRotatef(-90,1,0,0);
    glScalef(0.3,0.3,0.3);
    if(activo[9]==1) ala.draw(EDGES,r_p,g_p,b_p,grosor_select);
    else ala.draw(modo, r, g, b, grosor);
    glPopMatrix();

    // Helices parte izquierda
    glPushMatrix();
    glTranslatef(-1.1,0,1.75);
    glRotatef(giro_helices,1,0,0);
    glRotatef(45,1,0,0);
    glRotatef(-90,0,1,0);
    glRotatef(90,1,0,0);
    glScalef(0.2,0.2,0.2);
    if(activo[8]==1) helices.draw(EDGES,r_p,g_p,b_p,grosor_select);
    else helices.draw(modo, r, g, b, grosor);
    glPopMatrix();

    // Helices parte derecha
    glPushMatrix();
    glTranslatef(-1.1,0,-1.75);
    glRotatef(giro_helices,1,0,0);
    glRotatef(-90,0,1,0);
    glRotatef(90,1,0,0);
    glScalef(0.2,0.2,0.2);
    if(activo[7]==1) helices.draw(EDGES,r_p,g_p,b_p,grosor_select);
    else helices.draw(modo, r, g, b, grosor);
    glPopMatrix();

    // Rueda derecha
    glPushMatrix();
    glTranslatef(0,guarda_rueda,0);
    glTranslatef(0,-0.7,-1.75);
    if(activo[6]==1) rueda.draw(EDGES,r_p,g_p,b_p,grosor_select);
    else rueda.draw(modo, r, g, b, grosor);
    glTranslatef(0,0.2,0);
    glScalef(0.03,0.25,0.03);
    if(activo[5]==1) union_rueda.draw(EDGES,r_p,g_p,b_p,grosor_select);
    else union_rueda.draw(modo, r, g, b, grosor);
    glPopMatrix();

    // Rueda izquierda
    glPushMatrix();
    glTranslatef(0,guarda_rueda,0);
    glTranslatef(0,-0.7,1.75);
    if(activo[4]==1) rueda.draw(EDGES,r_p,g_p,b_p,grosor_select);
    else rueda.draw(modo, r, g, b, grosor);
    glTranslatef(0,0.2,0);
    glScalef(0.03,0.25,0.03);
    if(activo[3]==1) union_rueda.draw(EDGES,r_p,g_p,b_p,grosor_select);
    else union_rueda.draw(modo, r, g, b, grosor);
    glPopMatrix();

    // Rueda principal
    glPushMatrix();
    glTranslatef(-1.5,-0.45,0);
    glRotatef(rota_rueda,0,0,1);
    if(activo[2]==1) rueda_principal.draw(EDGES,r_p,g_p,b_p,grosor_select);
    else rueda_principal.draw(modo,r,g,b,grosor);
    glPopMatrix();

    // Cabina
    glPushMatrix();
    glTranslatef(-0.35,0.4,0);
    glRotatef(-5,0,0,1);
    if(activo[1]==1) modulo.draw(EDGES,r_p,g_p,b_p,grosor_select);
    else modulo.draw(modo, r, g, b, grosor);
    glPopMatrix();

    // Estructura
    glPushMatrix();
    if(activo[0]==1) estructura.draw(EDGES, r_p, g_p, b_p, grosor_select);
    else estructura.draw(modo, r, g, b, grosor);
    glPopMatrix();

    glPopMatrix();
  }
}

void _p38::draw_escena(_modo modo, float r, float g, float b, float grosor)
{

  glPushMatrix();
  glScalef(1,2.5,2.5);
  glTranslatef(75,15,0);
  glRotatef(90,0,1,0);
  glRotatef(90,1,0,0);
  cielo.draw_solido_textura(tex_cielo);
  cielo = _base_textura(50,25);
  glPopMatrix();

  glPushMatrix();
  glScalef(2.5,2.5,1);
  glTranslatef(0,15,-75);
  glRotatef(90,1,0,0);
  cielo.draw_solido_textura(tex_cielo);
  cielo = _base_textura(50,25);
  glPopMatrix();

  glPushMatrix();
  glTranslatef(0,-0.83,0);
  suelo.draw_solido_textura(tex_suelo);
  suelo = _base_textura(75,75);
  glPopMatrix();

  draw(modo,r,g,b,grosor);
};

//************************************************************************
// Métodos del modelo jerárquico
//************************************************************************

//************************************************************************
// Método selección
//************************************************************************

void _p38::seleccion()
{
  _vertex3i color;

  // Cañon derecho
  glPushMatrix();
  glTranslatef(-1.85, 0.25, -0.35);
  glRotatef(-90, 1, 0, 0);
  glRotatef(-90, 0, 1, 0);
  glScalef(0.15, 0.15, 0.15);
  color=color_select[21];
  canon.draw(SELECT, color.r, color.g, color.b, 1);
  glPopMatrix();

  // Cañon izquierdo
  glPushMatrix();
  glTranslatef(-1.85, 0.25, 0.35);
  glRotatef(90, 1, 0, 0);
  glRotatef(-90, 0, 1, 0);
  glScalef(0.15, 0.15, 0.15);
  color=color_select[20];
  canon.draw(SELECT, color.r, color.g, color.b, 1);
  glPopMatrix();

  // Cañon central
  glPushMatrix();
  glTranslatef(-1.85, 0.45, 0);
  glRotatef(-90, 0, 1, 0);
  glScalef(0.15, 0.15, 0.15);
  color=color_select[19];
  canon.draw(SELECT, color.r, color.g, color.b, 1);
  glPopMatrix();

  // Aletas derechas
  glPushMatrix();

  glTranslatef(2.825, 0, -1.75);
  glScalef(0.5, 0.5, 0.5);

  glPushMatrix();
  glTranslatef(0, -0.25, 0);
  color=color_select[18];
  aleta.draw(SELECT, color.r, color.g, color.b, 1);
  glPopMatrix();

  glPushMatrix();
  glTranslatef(0, 0, -0.25);
  glRotatef(90, 1, 0, 0);
  color=color_select[17];
  aleta.draw(SELECT, color.r, color.g, color.b, 1);
  glPopMatrix();

  glPushMatrix();
  glTranslatef(0, 0.25, 0);
  glRotatef(180, 1, 0, 0);
  color=color_select[16];
  aleta.draw(SELECT, color.r, color.g, color.b, 1);
  glPopMatrix();

  glPopMatrix();

  // Aletas izquierdas
  glPushMatrix();

  glTranslatef(2.825, 0, 1.75);
  glScalef(0.5, 0.5, 0.5);

  glPushMatrix();
  glTranslatef(0, -0.25, 0);
  color=color_select[15];
  aleta.draw(SELECT, color.r, color.g, color.b, 1);
  glPopMatrix();

  glPushMatrix();
  glTranslatef(0, 0, 0.25);
  glRotatef(-90, 1, 0, 0);
  color=color_select[14];
  aleta.draw(SELECT, color.r, color.g, color.b, 1);
  glPopMatrix();

  glPushMatrix();
  glTranslatef(0, 0.25, 0);
  glRotatef(180, 1, 0, 0);
  color=color_select[13];
  aleta.draw(SELECT, color.r, color.g, color.b, 1);
  glPopMatrix();

  glPopMatrix();

  // Aleron
  glPushMatrix();
  glTranslatef(2.6, 0, 0);
  glRotatef(subida_aleron, 0, 0, 1);
  color=color_select[12];
  aleron.draw(SELECT, color.r, color.g, color.b, 1);
  glPopMatrix();

  glPushMatrix();
  glTranslatef(2.85, 0, 0);
  glRotatef(subida_aleron, 0, 0, 1);
  color=color_select[11];
  aleron.draw(SELECT, color.r, color.g, color.b, 1);
  glPopMatrix();

  // Ala izquierda
  glPushMatrix();
  glTranslatef(0, 0, 2.5);
  glRotatef(cierre_alas, 0, 1, 0);
  glRotatef(90, 1, 0, 0);
  glScalef(0.3, 0.3, 0.3);
  color=color_select[10];
  ala.draw(SELECT, color.r, color.g, color.b, 1);
  glPopMatrix();

  // Ala derecha
  glPushMatrix();
  glTranslatef(0, 0, -2.5);
  glRotatef(-cierre_alas, 0, 1, 0);
  glRotatef(-90, 1, 0, 0);
  glScalef(0.3, 0.3, 0.3);
  color=color_select[9];
  ala.draw(SELECT, color.r, color.g, color.b, 1);
  glPopMatrix();

  // Helices parte izquierda
  glPushMatrix();
  glTranslatef(-1.1, 0, 1.75);
  glRotatef(giro_helices, 1, 0, 0);
  glRotatef(45, 1, 0, 0);
  glRotatef(-90, 0, 1, 0);
  glRotatef(90, 1, 0, 0);
  glScalef(0.25, 0.25, 0.25);
  color=color_select[8];
  helices.draw(SELECT, color.r, color.g, color.b, 1);
  glPopMatrix();

  // Helices parte derecha
  glPushMatrix();
  glTranslatef(-1.1, 0, -1.75);
  glRotatef(giro_helices, 1, 0, 0);
  glRotatef(-90, 0, 1, 0);
  glRotatef(90, 1, 0, 0);
  glScalef(0.25, 0.25, 0.25);
  color=color_select[7];
  helices.draw(SELECT, color.r, color.g, color.b, 1);
  glPopMatrix();

  // Rueda derecha
  glPushMatrix();
  glTranslatef(0, guarda_rueda, 0);
  glTranslatef(0, -0.7, -1.75);
  color=color_select[6];
  rueda.draw(SELECT, color.r, color.g, color.b, 1);
  glTranslatef(0, 0.2, 0);
  glScalef(0.03, 0.25, 0.03);
  color=color_select[5];
  union_rueda.draw(SELECT, color.r, color.g, color.b, 1);
  glPopMatrix();

  // Rueda izquierda
  glPushMatrix();
  glTranslatef(0, guarda_rueda, 0);
  glTranslatef(0, -0.7, 1.75);
  color=color_select[4];
  rueda.draw(SELECT, color.r, color.g, color.b, 1);
  glTranslatef(0, 0.2, 0);
  glScalef(0.03, 0.25, 0.03);
  color=color_select[3];
  union_rueda.draw(SELECT, color.r, color.g, color.b, 1);
  glPopMatrix();

  // Rueda principal
  glPushMatrix();
  glTranslatef(-1.5,-0.45,0);
  glRotatef(rota_rueda,0,0,1);
  color=color_select[2];
  rueda_principal.draw(SELECT, color.r, color.g, color.b, 1);
  glPopMatrix();

  // Cabina
  glPushMatrix();
  glTranslatef(-0.35,0.4,0);
  glRotatef(-5,0,0,1);
  color=color_select[1];
  modulo.draw(SELECT, color.r, color.g, color.b, 1);
  glPopMatrix();

  // Estructura
  glPushMatrix();
  color=color_select[0];
  estructura.draw(SELECT, color.r, color.g, color.b, 1);
  glPopMatrix();
}

//************************************************************************
// Método para asignar texturas
//************************************************************************
void _p38::texturas(int textura_suelo, int textura_cielo, int textura_estrella)
{
  // Inicialización de las variables
  tex_suelo = textura_suelo;
  tex_cielo = textura_cielo;
  tex_estrella = textura_estrella;
}

//************************************************************************
// Método giro de las hélices
//************************************************************************
void _p38::gira_helices(){
    giro_helices+=30;
}

//************************************************************************
// Método subir aleron
//************************************************************************
void _p38::sube_aleron(bool sube){
  if(sube){
     if(subida_aleron<subida_aleron_max)
      subida_aleron+=1;
  }
  else {
    if(subida_aleron>subida_aleron_min)
    subida_aleron-=1;
  }
}

//************************************************************************
// Método cerrar las alas
//************************************************************************
void _p38::cierra_alas(bool cierra){
  if(cierra){
    if(cierre_alas<cierre_alas_max)
      cierre_alas+=1.25;
  }
  else {
    if(cierre_alas>cierre_alas_min)
      cierre_alas-=1.25;
  }
}

//************************************************************************
// Método rotar rueda central
//************************************************************************
void _p38::rotar_rueda(bool rota){
  if(rota){
    if(rota_rueda<rota_rueda_max)
      rota_rueda+=1.25;
  }
  else {
    if(rota_rueda>rota_rueda_min)
      rota_rueda-=1.25;
  }
}

//************************************************************************
// Método guardar rueda lateral
//************************************************************************
void _p38::guardar_rueda(bool guarda){
  if(guarda){
    if(guarda_rueda<guarda_rueda_max)
      guarda_rueda+=0.025;
  }
  else {
    if(guarda_rueda>guarda_rueda_min)
      guarda_rueda-=0.025;
  }
}

//************************************************************************
// Método desplazarse
//************************************************************************
void _p38::desplazarse(bool adelante){
  if(adelante){
    if(movimiento_max<movimiento){
      movimiento-=0.25;
      gira_helices();
    }

  }
  else {
    if(movimiento_min>movimiento){
      movimiento+=0.25;
      gira_helices();
    }
  }
}

//************************************************************************
// Método animacion inicial
//************************************************************************
bool _p38::animacion_paso0(){
  gira_helices();
  sube_aleron(true);
  if(desplazamiento<desplazamiento_max){
    desplazamiento+=0.01;
    return false;
  }
  else {
    subida_aleron = subida_aleron_max;
    return true;
  }
}

bool _p38::animacion_paso1(){
  gira_helices();
  if(despegue_max<despegue){
    despegue-=0.025;
    diagonal+=0.01;
    guardar_rueda(true);
    rotar_rueda(true);
    return false;
  }
  else {
    rota_rueda = rota_rueda_max;
    guarda_rueda = guarda_rueda_max;
    return true;
  }
}

bool _p38::animacion_paso2(){
  gira_helices();
  if(diagonal<diagonal_max){
    diagonal+=0.01;
    cierra_alas(true);
    return false;
  }
  else {
    cierre_alas = cierre_alas_max;
    return true;
  }
}

bool _p38::animacion_paso3(){
  if(caida>caida_max){
    caida-=0.05;
    return false;
  }
  else {
    return true;
  }
}

bool _p38::animacion_paso4(){
  if(incendio<incendio_max){
    incendio+=0.001;
    return false;
  }
  else {
    return true;
  }
}

void _p38::valores_iniciales(){
  giro_helices = 0;
  subida_aleron = 0;
  cierre_alas = 0;
  rota_rueda = 0;
  guarda_rueda = 0;
  desplazamiento = 0;
  despegue = 0;
  diagonal = 0;
  caida = 0;
  caso = 0;
  incendio = 0;
  movimiento = 0; 
}

//************************************************************************************************
// Objetos con texturas (Práctica 4)
//************************************************************************************************

_cubo_textura::_cubo_textura(float tam)
{
  //vertices
vertices.resize(14);
vertices[0].x=-tam;vertices[0].y=-tam;vertices[0].z=tam;
vertices[1].x=tam;vertices[1].y=-tam;vertices[1].z=tam;
vertices[2].x=tam;vertices[2].y=tam;vertices[2].z=tam;
vertices[3].x=-tam;vertices[3].y=tam;vertices[3].z=tam;
vertices[4].x=-tam;vertices[4].y=-tam;vertices[4].z=-tam;
vertices[5].x=tam;vertices[5].y=-tam;vertices[5].z=-tam;
vertices[6].x=tam;vertices[6].y=tam;vertices[6].z=-tam;
vertices[7].x=-tam;vertices[7].y=tam;vertices[7].z=-tam;
vertices[8].x=-tam;vertices[8].y=tam;vertices[8].z=tam;
vertices[9].x=-tam;vertices[9].y=-tam;vertices[9].z=tam;
vertices[10].x=tam;vertices[10].y=tam;vertices[10].z=tam;
vertices[11].x=-tam;vertices[11].y=tam;vertices[11].z=tam;
vertices[12].x=-tam;vertices[12].y=-tam;vertices[12].z=tam;
vertices[13].x=tam;vertices[13].y=-tam;vertices[13].z=tam;


// triangulos
caras.resize(12);
caras[0]._0=0;caras[0]._1=1;caras[0]._2=3;
caras[1]._0=3;caras[1]._1=1;caras[1]._2=2;
caras[2]._0=1;caras[2]._1=5;caras[2]._2=2;
caras[3]._0=2;caras[3]._1=5;caras[3]._2=6;

caras[4]._0=5;caras[4]._1=4;caras[4]._2=6;
caras[5]._0=4;caras[5]._1=7;caras[5]._2=6;
caras[6]._0=9;caras[6]._1=7;caras[6]._2=4;
caras[7]._0=9;caras[7]._1=8;caras[7]._2=7;
caras[8]._0=11;caras[8]._1=10;caras[8]._2=7;
caras[9]._0=10;caras[9]._1=6;caras[9]._2=7;
caras[10]._0=12;caras[10]._1=4;caras[10]._2=13;
caras[11]._0=13;caras[11]._1=4;caras[11]._2=5; 

// Vectores texturas
texturas_vertices.resize(14);
texturas_vertices[0].x = 0.0; texturas_vertices[0].y = 0.75;
texturas_vertices[1].x = 0.25; texturas_vertices[1].y = 0.75;
texturas_vertices[2].x = 0.25; texturas_vertices[2].y = 0.5;
texturas_vertices[3].x = 0.0; texturas_vertices[3].y = 0.5;

texturas_vertices[4].x = 0.75; texturas_vertices[4].y = 0.75;
texturas_vertices[5].x = 0.50; texturas_vertices[5].y = 0.75;
texturas_vertices[6].x = 0.50; texturas_vertices[6].y = 0.5;

texturas_vertices[7].x = 0.75; texturas_vertices[7].y = 0.5;
texturas_vertices[8].x = 1.0; texturas_vertices[8].y = 0.5;
texturas_vertices[9].x = 1.0; texturas_vertices[9].y = 0.75;
texturas_vertices[10].x = 0.5; texturas_vertices[10].y = 0.25;
texturas_vertices[11].x = 0.75; texturas_vertices[11].y = 0.25;
texturas_vertices[12].x = 0.75; texturas_vertices[12].y = 1.0;
texturas_vertices[13].x = 0.5; texturas_vertices[13].y = 1.0;

textura_id = 0;

}

void _cubo_textura::textura(int textura)
{
  textura_id = textura;
}

void _cubo_textura::draw(_modo modo, float r, float g, float b, float grosor)
{ 
  draw_solido_textura(textura_id);
}

_base_textura::_base_textura(float ancho, float largo)
{
  //vertices
vertices.resize(4);
vertices[0].x=-ancho;vertices[0].y=0;vertices[0].z=largo;
vertices[1].x=ancho;vertices[1].y=0;vertices[1].z=largo;
vertices[2].x=ancho;vertices[2].y=0;vertices[2].z=-largo;
vertices[3].x=-ancho;vertices[3].y=0;vertices[3].z=-largo;

// triangulos
caras.resize(2);
caras[0]._0=3;caras[0]._1=0;caras[0]._2=1;
caras[1]._0=1;caras[1]._1=2;caras[1]._2=3;

// Vectores texturas
texturas_vertices.resize(4);
texturas_vertices[0].x = 0.0; texturas_vertices[0].y = 1.0;
texturas_vertices[1].x = 1.0; texturas_vertices[1].y = 1.0;
texturas_vertices[2].x = 1.0; texturas_vertices[2].y = 0.0;
texturas_vertices[3].x = 0.0; texturas_vertices[3].y = 0.0;

}

_escena::_escena()
{
  // Inicialización de las variables
  textura_id = 0;
  textura_id1 = 0;
  textura_id2 = 0;
  textura_id3 = 0;
  textura_id4 = 0;
};

void _escena::texturas(int textura, int textura1, int textura2, int textura3, int textura4)
{
  // Inicialización de las variables
  textura_id = textura;
  textura_id1 = textura1;
  textura_id2 = textura2;
  textura_id3 = textura3;
  textura_id4 = textura4;
}

void _escena::draw(_modo modo, float r, float g, float b, float grosor)
{ 
  glPushMatrix();
  glTranslatef(0,12,0);
  glScalef(5,5,5);
  cubo_textura.draw_solido_textura(textura_id);
  glPopMatrix();

  glPushMatrix();
  glTranslatef(0,5,0);
  glScalef(1.25,10,1.25);
  cubo_textura.draw_solido_textura(textura_id4);
  glPopMatrix();

  glPushMatrix();
  glTranslatef(0,-0.001,0);
  base_textura.draw_solido_textura(textura_id1);
  glPopMatrix();

  glPushMatrix();
  glTranslatef(0,15,-30);
  glRotatef(90,1,0,0);
  glScalef(1,1,0.5);
  base_textura.draw_solido_textura(textura_id3);
  glPopMatrix();

  glPushMatrix();
  glTranslatef(-20,15,0);
  glRotatef(90,0,1,0);
  glRotatef(90,1,0,0);
  glScalef(1.5,1,0.5);
  base_textura.draw_solido_textura(textura_id2);
  glPopMatrix();
};

