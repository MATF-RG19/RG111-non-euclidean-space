#ifndef LIGHT_H_
#define LIGHT_H_

#include <GL/glut.h>

typedef struct material {
  GLfloat ambient_coeffs[4];
  GLfloat diffuse_coeffs[4];
  GLfloat specular_coeffs[4];
  GLfloat shininess;
} material;

typedef struct light {
  GLfloat ambient[4];
  GLfloat diffuse[4];
  GLfloat specular[4];
} light;

extern material material_concrete_white;
extern material material_concrete_blue;
extern material material_concrete_red;
extern material material_concrete_green;
extern material material_concrete_yellow;

extern light light_basic;

extern void init_light();
extern void set_material(material m);

#endif
