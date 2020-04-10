#include "level.h"

// Lights
static GLfloat main_light_position[] = { 0, 5, 0, 1 };

extern void load_level(double *x, double *y, double *z) {
  // Starting room
  create_wall(-10, 2, -3, 1, 0, 0, 14, 4, true, NULL);        // Back wall
  create_wall(-6, 2, 4, 0, 0, -1, 8, 4, false, NULL);         // Right wall
  create_wall(-6, 2, -10, 0, 0, 1, 8, 4, false, NULL);        // Left wall
  create_wall(-2, 0.25, -3, -1, 0, 0, 14, 0.5, false, NULL);  // Lower starting room facing separator
  create_wall(-2, 3, -3, -1, 0, 0, 14, 2, false, NULL);       // Upper starting room facing separator

  // Middle rooms
  create_wall(4, 2, -10, 0, 0, 1, 12, 4, true, NULL);         // Left wall
  create_wall(4, 2, 10, 0, 0, -1, 12, 4, false, NULL);        // Right wall

  // Starting room separators
  create_wall(-2, 0.25, -3, 1, 0, 0, 14, 0.5, false, NULL);   // Lower middle room facing separator
  create_wall(-2, 3, -3, 1, 0, 0, 14, 2, false, NULL);        // Upper middle room facing separator
  create_wall(-2, 2, 8, 1, 0, 0, 4, 4, true, NULL);           // Full anchored separator
  create_wall(-2, 2, 5, 1, 0, 0, 2, 4, false, NULL);          // Full non-anchored separator

  // Far room separators
  create_wall(10, 0.25, -3, -1, 0, 0, 14, 0.5, false, NULL);  // Lower middle room facing separator
  create_wall(10, 3, -3, -1, 0, 0, 14, 2, false, NULL);       // Upper middle room facing separator
  create_wall(10, 2, 7, -1, 0, 0, 6, 4, false, NULL);         // Full middle room facing separator

  // Middle rooms separator
  create_wall(4, 2, 0, 0, 0, 1, 12, 4, false, NULL);          // Right room facing middle separator
  create_wall(4, 2, 0, 0, 0, -1, 12, 4, false, NULL);         // Left room facing middle separator

  // Far room
  create_wall(16, 2, 0, -1, 0, 0, 20, 4, false, NULL);        // Front wall
  create_wall(13, 2, 10, 0, 0, -1, 6, 4, true, NULL);         // Right wall
  create_wall(13, 2, -10, 0, 0, 1, 6, 4, false, NULL);        // Left Wall

  // Far room separators
  create_wall(10, 0.25, -3, 1, 0, 0, 14, 0.5, false, NULL);   // Lower far room facing separator
  create_wall(10, 3, -3, 1, 0, 0, 14, 2, false, NULL);        // Upper far room facing separator
  create_wall(10, 2, 7, 1, 0, 0, 6, 4, false, NULL);          // Full far room facing separator

  // Player position
  *x = -6.0f;
  *y = 1.0f;
  *z = -5.0f;
}

extern void draw_world(double x, double y, double z, double yaw, double pitch) {
  (void) x;
  (void) y;
  (void) z;
  (void) yaw;
  (void) pitch;

  glEnable(GL_CULL_FACE);
  glDisable(GL_LIGHTING);

  // Draw the inside of the skybox sphere
  glCullFace(GL_FRONT);

  // Skybox
  glColor3f(0.2f, 0.8f, 1);
  glutSolidSphere(50, 10, 10);

  glEnable(GL_LIGHTING);

  // Draw the front faces of the walls
  glCullFace(GL_BACK);

  init_light(GL_LIGHT0, main_light_position, &light_basic);

  // Draw the walls
  for(unsigned int i = 0; i < wall_count; i++) {
    draw_textured_wall(walls[i]);
  }

  // Draw the floor
  glBegin(GL_QUADS);
    set_material(&material_concrete_white);
    glNormal3f(0, 1, 0);

    glVertex3f(-10.0f, 0, -10.0f);
    glVertex3f(-10.0f, 0, 10.0f);
    glVertex3f(16.0f, 0, 10.0f);
    glVertex3f(16.0f, 0, -10.0f);
  glEnd();

  // Draw the player
  // glPushMatrix();
  //   glTranslatef(-5.0f, 0, -5.0f);
  //   draw_player(yaw, pitch);
  // glPopMatrix();

  // Draw the cake
  glPushMatrix();
    glTranslatef(5.0f, 0.0f, 5.0f);
    draw_cake();
  glPopMatrix();

  // Draw the companion cube
  glPushMatrix();
    glTranslatef(7.0f, 0.0f, -7.0f);
    draw_companion_cube();
  glPopMatrix();

  glDisable(GL_CULL_FACE);
}
