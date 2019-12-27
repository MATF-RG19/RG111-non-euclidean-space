#include "wall.h"

extern void draw_wall(wall *w) {
  set_material(w->material);

  // Draw a quad at specified position normal to the secified vector
  glBegin(GL_QUADS);
    glNormal3f(w->normal[0], w->normal[1], w->normal[2]);
    glVertex3f(w->position[0]-w->normal[2]*w->width/2, w->position[1]-w->height/2, w->position[2]+w->normal[0]*w->width/2);
    glVertex3f(w->position[0]+w->normal[2]*w->width/2, w->position[1]-w->height/2, w->position[2]-w->normal[0]*w->width/2);
    glVertex3f(w->position[0]+w->normal[2]*w->width/2, w->position[1]+w->height/2, w->position[2]-w->normal[0]*w->width/2);
    glVertex3f(w->position[0]-w->normal[2]*w->width/2, w->position[1]+w->height/2, w->position[2]+w->normal[0]*w->width/2);
  glEnd();
}

extern void draw_textured_wall(wall *w) {
  set_material(w->material);

  // Set the wall texture depending on the anchor field
  if(w->anchor)
    glBindTexture(GL_TEXTURE_2D, textures[1]);
  else
    glBindTexture(GL_TEXTURE_2D, textures[0]);

  // Draw a quad at specified position normal to the secified vector
  glBegin(GL_QUADS);
    glNormal3f(w->normal[0], w->normal[1], w->normal[2]);

    glTexCoord2f(0, w->height);
    glVertex3f(w->position[0]-w->normal[2]*w->width/2, w->position[1]-w->height/2, w->position[2]+w->normal[0]*w->width/2);

    glTexCoord2f(w->width, w->height);
    glVertex3f(w->position[0]+w->normal[2]*w->width/2, w->position[1]-w->height/2, w->position[2]-w->normal[0]*w->width/2);

    glTexCoord2f(w->width, 0);
    glVertex3f(w->position[0]+w->normal[2]*w->width/2, w->position[1]+w->height/2, w->position[2]-w->normal[0]*w->width/2);

    glTexCoord2f(0, 0);
    glVertex3f(w->position[0]-w->normal[2]*w->width/2, w->position[1]+w->height/2, w->position[2]+w->normal[0]*w->width/2);
  glEnd();

  glBindTexture(GL_TEXTURE_2D, 0);
}

extern float distance_from_wall(float x, float z, wall *w, float *intersect_x, float *intersect_z) {
  float a = w->normal[0];
  float b = w->normal[2];
  float c = -a*w->position[0]-b*w->position[2];
  float dist = fabs(a*x+b*z+c)/norm2f(a, b);
  float t = -(a*x+b*z+c)/(a*a+b*b);
  *intersect_x = x+a*t;
  *intersect_z = z+b*t;
  return dist;
}

extern bool is_colliding_with_wall(float x, float z, wall *w, float *dist) {
  float intersect_x, intersect_z = 0;
  *dist = distance_from_wall(x, z, w, &intersect_x, &intersect_z);
  return *dist < PLAYER_RADIUS && norm2f(intersect_x-w->position[0], intersect_z-w->position[2]) < w->width/2 + PLAYER_RADIUS;
}
