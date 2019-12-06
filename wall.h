#ifndef WALL_H_
#define WALL_H_

#include <GL/glut.h>
#include <stdbool.h>
#include <math.h>

#include "shared.h"
#include "light.h"
#include "util.h"

typedef struct wall {
  GLfloat position[3];
  GLfloat normal[3];
  GLfloat width;
  GLfloat height;
  material *material;
} wall;

extern void draw_wall(wall *w);
extern float distance_from_wall(float x, float z, wall *w, float *intersect_x, float *intersect_z);
extern bool is_colliding_with_wall(float x, float z, wall *w, float *dist);

#endif
