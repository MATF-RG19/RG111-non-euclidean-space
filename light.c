#include <GL/glut.h>

#include "light.h"

material material_concrete_white  = { { 0.6f, 0.6f, 0.6f, 1 }, { 0.8f, 0.8f, 0.8f, 1 }, { 0.1f, 0.1f, 0.1f, 1 }, 20 };
material material_concrete_blue  = { { 0.1f, 0.1f, 0.5f, 1 }, { 0.2f, 0.2f, 0.8f, 1 }, { 0.05f, 0.05f, 0.05f, 1 }, 20 };
material material_concrete_red  = { { 0.5f, 0.1f, 0.1f, 1 }, { 0.8f, 0.2f, 0.2f, 1 }, { 0.05f, 0.05f, 0.05f, 1 }, 20 };
material material_concrete_green  = { { 0.1f, 0.5f, 0.1f, 1 }, { 0.2f, 0.8f, 0.2f, 1 }, { 0.05f, 0.05f, 0.05f, 1 }, 20 };
material material_concrete_yellow  = { { 0.5f, 0.5f, 0.1f, 1 }, { 0.8f, 0.8f, 0.2f, 1 }, { 0.05f, 0.05f, 0.05f, 1 }, 20 };

light light_basic = { { 0.4f, 0.4f, 0.4f, 1 }, { 0.7f, 0.7f, 0.7f, 1 }, { 0.5f, 0.5f, 0.5f, 1 } };

extern void init_light(GLenum id, GLfloat *position, light light) {
  glEnable(id);
  glLightfv(id, GL_POSITION, position);
  glLightfv(id, GL_AMBIENT, light.ambient);
  glLightfv(id, GL_DIFFUSE, light.diffuse);
  glLightfv(id, GL_SPECULAR, light.specular);
}

static void init_material(GLfloat *ambient_coeffs, GLfloat *diffuse_coeffs, GLfloat *specular_coeffs, GLfloat shininess) {
  glMaterialfv(GL_FRONT, GL_AMBIENT, ambient_coeffs);
  glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse_coeffs);
  glMaterialfv(GL_FRONT, GL_SPECULAR, specular_coeffs);
  glMaterialf(GL_FRONT, GL_SHININESS, shininess);
}

extern void set_material(material m) {
  init_material(m.ambient_coeffs, m.diffuse_coeffs, m.specular_coeffs, m.shininess);
}
