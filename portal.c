#include "portal.h"

extern void draw_portal_frame(portal p) {
  set_material(material_portal);

  glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK);

  glBegin(GL_QUADS);
    glNormal3f(p.normal[0], p.normal[1], p.normal[2]);
    glVertex3f(p.position[0]-p.normal[2]*p.width/2+p.normal[0]*0.05f, p.position[1]-p.height/2, p.position[2]+p.normal[0]*p.width/2+p.normal[2]*0.05f);
    glVertex3f(p.position[0]+p.normal[2]*p.width/2+p.normal[0]*0.05f, p.position[1]-p.height/2, p.position[2]-p.normal[0]*p.width/2+p.normal[2]*0.05f);
    glVertex3f(p.position[0]+p.normal[2]*p.width/2+p.normal[0]*0.05f, p.position[1]+p.height/2, p.position[2]-p.normal[0]*p.width/2+p.normal[2]*0.05f);
    glVertex3f(p.position[0]-p.normal[2]*p.width/2+p.normal[0]*0.05f, p.position[1]+p.height/2, p.position[2]+p.normal[0]*p.width/2+p.normal[2]*0.05f);
  glEnd();

  glDisable(GL_CULL_FACE);
}
