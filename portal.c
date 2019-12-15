#include "portal.h"

extern void draw_portal_frame(portal *p) {
  set_material(&material_portal);

  glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK);

  // Draw a quad at specified position normal to the secified vector
  // The quad is moved a few pixels in the direction of the normal vector
  // in order to prevent overlapping with the wall
  glBegin(GL_QUADS);
    glNormal3f(p->normal[0], p->normal[1], p->normal[2]);
    glVertex3f(p->position[0]-p->normal[2]*p->width/2+p->normal[0]*0.01f, p->position[1]-p->height/2, p->position[2]+p->normal[0]*p->width/2+p->normal[2]*0.01f);
    glVertex3f(p->position[0]+p->normal[2]*p->width/2+p->normal[0]*0.01f, p->position[1]-p->height/2, p->position[2]-p->normal[0]*p->width/2+p->normal[2]*0.01f);
    glVertex3f(p->position[0]+p->normal[2]*p->width/2+p->normal[0]*0.01f, p->position[1]+p->height/2, p->position[2]-p->normal[0]*p->width/2+p->normal[2]*0.01f);
    glVertex3f(p->position[0]-p->normal[2]*p->width/2+p->normal[0]*0.01f, p->position[1]+p->height/2, p->position[2]+p->normal[0]*p->width/2+p->normal[2]*0.01f);
  glEnd();

  glDisable(GL_CULL_FACE);
}

extern bool is_linked(portal *p) {
  return p->link != NULL;
}

extern bool link_portals(portal *p1, portal *p2) {
  if(p1 == NULL || p2 == NULL || p1 == p2 || is_linked(p1) || is_linked(p2))
    return false;
  p1->link = p2;
  p2->link = p1;
  return true;
}

extern bool unlink_portal(portal *p) {
  if(p!=NULL && is_linked(p)) {
    p->link->link = NULL;
    p->link = NULL;
    return true;
  }
  return false;
}

extern float distance_from_portal(float x, float z, portal *p, float *intersect_x, float *intersect_z) {
  float a = p->normal[0];
  float b = p->normal[2];
  float c = -a*p->position[0]-b*p->position[2];
  float dist = fabs(a*x+b*z+c)/norm2f(a, b);
  float t = -(a*x+b*z+c)/(a*a+b*b);
  *intersect_x = x+a*t;
  *intersect_z = z+b*t;
  return dist;
}

extern bool is_colliding_with_portal(float x, float y, float z, portal *p) {
  float dist, intersect_x, intersect_z = 0;
  dist = distance_from_portal(x, z, p, &intersect_x, &intersect_z);
  return dist < PLAYER_RADIUS &&
    norm2f(intersect_x-p->position[0], intersect_z-p->position[2]) < p->width/2 - PLAYER_RADIUS &&
    p->position[1]-p->height/2 <= y-PLAYER_HEIGHT/2 && p->position[1]+p->height/2 >= y+PLAYER_HEIGHT/2;
}

extern float* get_offset_position(portal* p) {
  float *pos = calloc(3, sizeof(float));
  pos[0] = p->position[0]+0.1f*p->normal[0];
  pos[1] = p->position[1];
  pos[2] = p->position[2]+0.1f*p->normal[2];
  return pos;
}
