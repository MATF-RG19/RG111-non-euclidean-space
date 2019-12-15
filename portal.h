#ifndef PORTAL_H_
#define PORTAL_H_

#include <stdbool.h>
#include <GL/glut.h>
#include <math.h>

#include "shared.h"
#include "light.h"
#include "util.h"

typedef struct portal {
  GLfloat position[3];
  GLfloat normal[3];
  GLfloat width;
  GLfloat height;
  struct portal *link;
} portal;

extern void draw_portal_frame(portal *p);
extern bool is_linked(portal *p);
extern bool link_portals(portal *p1, portal *p2);
extern bool unlink_portal(portal *p);
extern float distance_from_portal(float x, float z, portal *p, float *intersect_x, float *intersect_z);
extern bool is_colliding_with_portal(float x, float y, float z, portal *p);
extern float* get_offset_position(portal* p);

#endif
