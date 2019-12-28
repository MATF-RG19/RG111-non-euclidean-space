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

static void draw_cake() {
  glEnable(GL_COLOR_MATERIAL);

  glPushMatrix();
    // Draw table
    glColor3f(0.6f, 0.4f, 0.3f);

    glPushMatrix();
      glTranslatef(0, 0.5f, 0);
      glScalef(2.0f, 0.1f, 2.0f);
      glutSolidCube(1);
    glPopMatrix();

    glPushMatrix();
      glTranslatef(-0.95f, 0, -0.95f);

      glScalef(0.1f, 0.5f, 0.1f);
      glTranslatef(0, 0.45f, 0);
      glutSolidCube(1);
    glPopMatrix();

    glPushMatrix();
      glTranslatef(0.95f, 0, -0.95f);

      glScalef(0.1f, 0.5f, 0.1f);
      glTranslatef(0, 0.45f, 0);
      glutSolidCube(1);
    glPopMatrix();

    glPushMatrix();
      glTranslatef(-0.95f, 0, 0.95f);

      glScalef(0.1f, 0.5f, 0.1f);
      glTranslatef(0, 0.45f, 0);
      glutSolidCube(1);
    glPopMatrix();

    glPushMatrix();
      glTranslatef(0.95f, 0, 0.95f);

      glScalef(0.1f, 0.5f, 0.1f);
      glTranslatef(0, 0.45f, 0);
      glutSolidCube(1);
    glPopMatrix();

    // Draw cake
    glColor3f(0.2f, 0.1f, 0.0f);

    glPushMatrix();
      glTranslatef(0, 0.8f, 0);
      glRotatef(90, 1, 0, 0);
      glutSolidCylinder(0.6f, 0.25f, 20, 20);
    glPopMatrix();

    // Draw decorations
    glTranslatef(0, 0.8f, 0);
    for(int i = 0; i < 8; i++) {
      glPushMatrix();

      glColor3f(0.5f, 0.2f, 0.2f);
      glTranslatef(0.45f*cos(i*PI/4.0f), 0.0125f, 0.45f*sin(i*PI/4.0f));
      glutSolidSphere(0.06f, 10, 10);

      glColor3f(1.0f, 1.0f, 1.0f);
      glScalef(1.1f, 0.4f, 1.1f);
      glutSolidSphere(0.06f, 10, 10);

      glPopMatrix();
    }

    // Draw candle and flame
    glColor3f(1.0f, 1.0f, 0.8f);

    glPushMatrix();
      glTranslatef(0, 0.25f, 0);
      glRotatef(90, 1, 0, 0);
      glutSolidCylinder(0.04f, 0.25f, 10, 10);
    glPopMatrix();

    glColor3f(0.8f, 0.4f, 0.1f);

    glPushMatrix();
      glTranslatef(0, 0.29f, 0);
      glScalef(0.5f, 0.8f, 0.5f);
      glRotatef(90, 1, 0, 0);
      glutSolidSphere(0.04f, 10, 10);
    glPopMatrix();

  glPopMatrix();

  glDisable(GL_COLOR_MATERIAL);
}

extern void draw_world() {
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

  glDisable(GL_CULL_FACE);

  glPushMatrix();
    glTranslatef(5.0f, 0.0f, 5.0f);
    draw_cake();
  glPopMatrix();
}
