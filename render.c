#include "render.h"

// Textures
GLuint textures[NUM_TEXTURES];

extern void load_textures() {
  glGenTextures(NUM_TEXTURES, textures);

  int alignment;
  glGetIntegerv(GL_UNPACK_ALIGNMENT, &alignment);

  image *image = init_image();

  read_image(image, TEXTURE_PATH_WALL_DARK, alignment);

  glBindTexture(GL_TEXTURE_2D, textures[TEXTURE_WALL_DARK]);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image->width, image->height, 0, GL_RGB, GL_UNSIGNED_BYTE, image->pixels);

  read_image(image, TEXTURE_PATH_WALL_LIGHT, alignment);

  glBindTexture(GL_TEXTURE_2D, textures[TEXTURE_WALL_LIGHT]);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image->width, image->height, 0, GL_RGB, GL_UNSIGNED_BYTE, image->pixels);

  read_image(image, TEXTURE_PATH_HEART, alignment);

  glBindTexture(GL_TEXTURE_2D, textures[TEXTURE_HEART]);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image->width, image->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image->pixels);

  free_image(image);

  glBindTexture(GL_TEXTURE_2D, 0);
}

extern void free_textures() {
  glDeleteTextures(NUM_TEXTURES, textures);
}

static void draw_cylinder(float radius, float length, int points) {
  glBegin(GL_QUAD_STRIP);
    for(int i = 0; i < points; i++) {
      glNormal3f(0, cos((float)i/points*2*PI), sin((float)i/points*2*PI));
      glVertex3f(0, -radius*cos((float)i/points*2*PI), radius*sin((float)i/points*2*PI));
      glVertex3f(length, -radius*cos((float)i/points*2*PI), radius*sin((float)i/points*2*PI));
    }
    glNormal3f(0, -radius, 0);
    glVertex3f(0, -radius, 0);
    glVertex3f(length, -radius, 0);
  glEnd();
  glNormal3f(-1, 0, 0);
  glBegin(GL_TRIANGLE_FAN);
    for(int i = 0; i < points; i++) {
      glVertex3f(0, -radius*cos((float)i/points*2*PI), radius*sin((float)i/points*2*PI));
    }
  glEnd();
  glNormal3f(1, 0, 0);
  glBegin(GL_TRIANGLE_FAN);
    for(int i = 0; i < points; i++) {
      glVertex3f(length, radius*cos((float)i/points*2*PI), radius*sin((float)i/points*2*PI));
    }
  glEnd();
}

static void draw_circle(float radius, int points) {
  glNormal3f(1, 0, 0);
  glBegin(GL_TRIANGLE_FAN);
  for(int i = 0; i < points; i++) {
    glVertex3f(0, radius*cos((float)i/points*2*PI), radius*sin((float)i/points*2*PI));
  }
  glEnd();
}

extern bool player_animatation = false;
extern double player_animation_param = 0;

extern void draw_player(double yaw, double pitch) {
  bool color_material = glIsEnabled(GL_COLOR_MATERIAL);
  if(!color_material)
    glEnable(GL_COLOR_MATERIAL);

  glRotatef(-yaw, 0, 1, 0);

  // Head
  glColor3f(0.9f, 0.8f, 0.7f);
  glPushMatrix();
    glTranslatef(0, 1, 0);
    glScalef(0.2f, 0.2f, 0.2f);
    glutSolidSphere(0.5f, 20, 20);
  glPopMatrix();

  // Body
  glColor3f(0.1f, 0.5f, 0.6f);
  glPushMatrix();
    glTranslatef(0, 0.675f, 0);
    glScalef(0.2f, 0.45f, 0.26f);
    glutSolidCube(1);
  glPopMatrix();

  // Legs
  glColor3f(0.1f, 0.1f, 1.0f);
  glPushMatrix();
    glTranslatef(0, 0.45f, -0.075f);
    glRotatef(60*sin(360*player_animation_param), 0, 0, 1);
    glTranslatef(0, -0.225f, 0);
    glScalef(0.1f, 0.45f, 0.1f);
    glutSolidCube(1);
  glPopMatrix();

  glColor3f(0.1f, 0.1f, 1.0f);
  glPushMatrix();
    glTranslatef(0, 0.45f, 0.075f);
    glRotatef(-60*sin(360*player_animation_param), 0, 0, 1);
    glTranslatef(0, -0.225f, 0);
    glScalef(0.1f, 0.45f, 0.1f);
    glutSolidCube(1);
  glPopMatrix();

  // Arms
  glColor3f(0.1f, 0.5f, 0.6f);
  glPushMatrix();
    glTranslatef(0, 0.8f, 0.17f);
    glRotatef(-pitch, 0, 0, 1);
    glRotatef(10, 0, 1, 0);

    glPushMatrix();
      glRotatef(80, 0, 0, 1);
      glTranslatef(0, -0.15f, 0);
      glScalef(0.08f, 0.3f, 0.08f);
      glutSolidCube(1);
    glPopMatrix();

    glPushMatrix();
      glTranslatef(0.35f, 0, 0);
      glTranslatef(-0.1f, 0, 0);
      glScalef(0.7f, 0.7f, 0.7f);
      draw_portal_gun();
    glPopMatrix();
  glPopMatrix();

  glColor3f(0.1f, 0.5f, 0.6f);
  glPushMatrix();
    glTranslatef(0.05f, 0.8f, -0.17f);
    glRotatef(-pitch, 0, 0, 1);
    glRotatef(-40, 0, 1, 0);
    glRotatef(80, 0, 0, 1);
    glTranslatef(0, -0.15f, 0);
    glScalef(0.08f, 0.3f, 0.08f);
    glutSolidCube(1);
  glPopMatrix();

  if(!color_material)
    glDisable(GL_COLOR_MATERIAL);
}

extern void draw_portal_gun() {
  bool color_material = glIsEnabled(GL_COLOR_MATERIAL);
  if(!color_material)
    glEnable(GL_COLOR_MATERIAL);

  glPushMatrix();
    // Primary base
    glColor3f(0.8f, 0.8f, 0.8f);
    glPushMatrix();
      glScalef(0.18f, 0.12f, 0.12f);
      glutSolidSphere(1, 20, 20);
    glPopMatrix();

    // Power indicator
    glColor3f(0.1f, 0.5f, 0.9f);
    glPushMatrix();
      glTranslatef(0, 0.1f, 0);
      glScalef(0.03, 0.03f, 0.03f);
      glutSolidSphere(1, 10, 10);
    glPopMatrix();

    // Secondary base
    glColor3f(0.8f, 0.8f, 0.8f);
    glPushMatrix();
      glTranslatef(0.32f, -0.06f, 0);
      glScalef(0.11f, 0.07f, 0.12f);
      glutSolidSphere(1, 20, 20);
    glPopMatrix();

    // Power
    glColor3f(0.1f, 0.5f, 0.9f);
    glPushMatrix();
      glTranslatef(0.15f, 0, 0);
      draw_cylinder(0.05f, 0.2f, 20);
    glPopMatrix();

    // Barrels
    glPushMatrix();
      glColor3f(0.2f, 0.2f, 0.2f);
      glTranslatef(0.35f, 0, 0);
      draw_cylinder(0.08f, 0.06f, 20);

      glColor3f(0.3f, 0.3f, 0.3f);
      glTranslatef(0.06f, 0, 0);
      glScalef(0.7f, 0.7f, 0.7f);
      draw_cylinder(0.08f, 0.03f, 20);
    glPopMatrix();

    // Base connector
    glColor3f(0.2f, 0.2f, 0.2f);
    glPushMatrix();
      glTranslatef(0.2f, -0.05f, 0);
      glScalef(0.2f, 0.04f, 0.08f);
      glutSolidCube(1);
    glPopMatrix();

    // Hands
    glColor3f(0.2f, 0.2f, 0.2f);
    for(int i = 0; i < 3; i++) {
      glPushMatrix();
        glRotatef((float)i/3*360, 1, 0, 0);
        glTranslatef(0.37f, 0.07f, 0);
        glRotatef(-50, 0, 0, 1);
        glPushMatrix();
          glTranslatef(0, 0.14f, 0);
          glRotatef(-50, 0, 0, 1);
          glTranslatef(0, 0.04f, 0);
          glScalef(0.02f, 0.1f, 0.02f);
          glutSolidCube(1);
        glPopMatrix();
        glTranslatef(0, 0.07f, 0);
        glScalef(0.02f, 0.14f, 0.02f);
        glutSolidCube(1);
      glPopMatrix();
    }
  glPopMatrix();

  if(!color_material)
    glDisable(GL_COLOR_MATERIAL);
}

extern void draw_companion_cube() {
  bool color_material = glIsEnabled(GL_COLOR_MATERIAL);
  if(!color_material)
    glEnable(GL_COLOR_MATERIAL);

  glPushMatrix();

    glColor3f(0.4f, 0.4f, 0.4f);
    glTranslatef(-0.3f, 0.4f, -0.3f);
    glutSolidCube(0.6f);

    glColor3f(0.8f, 0.8f, 0.8f);

    // Corners
    glPushMatrix();
      glTranslatef(-0.23f, 0.23f, -0.23f);
      glutSolidCube(0.2f);
    glPopMatrix();

    glPushMatrix();
      glTranslatef(0.23f, 0.23f, -0.23f);
      glutSolidCube(0.2f);
    glPopMatrix();

    glPushMatrix();
      glTranslatef(0.23f, 0.23f, 0.23f);
      glutSolidCube(0.2f);
    glPopMatrix();

    glPushMatrix();
      glTranslatef(-0.23f, 0.23f, 0.23f);
      glutSolidCube(0.2f);
    glPopMatrix();

    glPushMatrix();
      glTranslatef(-0.23f, -0.23f, -0.23f);
      glutSolidCube(0.2f);
    glPopMatrix();

    glPushMatrix();
      glTranslatef(0.23f, -0.23f, -0.23f);
      glutSolidCube(0.2f);
    glPopMatrix();

    glPushMatrix();
      glTranslatef(0.23f, -0.23f, 0.23f);
      glutSolidCube(0.2f);
    glPopMatrix();

    glPushMatrix();
      glTranslatef(-0.23f, -0.23f, 0.23f);
      glutSolidCube(0.2f);
    glPopMatrix();

    // Sides
    glPushMatrix();
      glTranslatef(-0.28f, 0.28f, 0);
      glScalef(0.5f, 0.5f, 0.8f);
      glutSolidCube(0.2f);
    glPopMatrix();

    glPushMatrix();
      glTranslatef(0.28f, 0.28f, 0);
      glScalef(0.5f, 0.5f, 0.8f);
      glutSolidCube(0.2f);
    glPopMatrix();

    glPushMatrix();
      glTranslatef(0, 0.28f, 0.28f);
      glScalef(0.8f, 0.5f, 0.5f);
      glutSolidCube(0.2f);
    glPopMatrix();

    glPushMatrix();
      glTranslatef(0, 0.28f, -0.28f);
      glScalef(0.8f, 0.5f, 0.5f);
      glutSolidCube(0.2f);
    glPopMatrix();

    glPushMatrix();
      glTranslatef(-0.28f, -0.28f, 0);
      glScalef(0.5f, 0.5f, 0.8f);
      glutSolidCube(0.2f);
    glPopMatrix();

    glPushMatrix();
      glTranslatef(0.28f, -0.28f, 0);
      glScalef(0.5f, 0.5f, 0.8f);
      glutSolidCube(0.2f);
    glPopMatrix();

    glPushMatrix();
      glTranslatef(0, -0.28f, 0.28f);
      glScalef(0.8f, 0.5f, 0.5f);
      glutSolidCube(0.2f);
    glPopMatrix();

    glPushMatrix();
      glTranslatef(0, -0.28f, -0.28f);
      glScalef(0.8f, 0.5f, 0.5f);
      glutSolidCube(0.2f);
    glPopMatrix();

    glPushMatrix();
      glTranslatef(-0.28f, 0, -0.28f);
      glScalef(0.5f, 0.8f, 0.5f);
      glutSolidCube(0.2f);
    glPopMatrix();

    glPushMatrix();
      glTranslatef(0.28f, 0, -0.28f);
      glScalef(0.5f, 0.8f, 0.5f);
      glutSolidCube(0.2f);
    glPopMatrix();

    glPushMatrix();
      glTranslatef(-0.28f, 0, 0.28f);
      glScalef(0.5f, 0.8f, 0.5f);
      glutSolidCube(0.2f);
    glPopMatrix();

    glPushMatrix();
      glTranslatef(0.28f, 0, 0.28f);
      glScalef(0.5f, 0.8f, 0.5f);
      glutSolidCube(0.2f);
    glPopMatrix();

    if(!color_material)
      glDisable(GL_COLOR_MATERIAL);

    // Faces
    glColor3f(0.6f , 0.6f, 0.6f);
    for(int i = 0; i < 4; i++) {
      glPushMatrix();
        glRotatef(i*90, 0, 1, 0);
        glTranslatef(0.31f, 0, 0);
        draw_circle(0.14f, 20);
      glPopMatrix();
    }

    glBindTexture(GL_TEXTURE_2D, textures[TEXTURE_HEART]);
    glColor3f(1, 0, 0);

    glBegin(GL_QUADS);
      glNormal3f(1, 0, 0);

      glTexCoord2f(0, 1);
      glVertex3f(0.32f, 0.1f, -0.1f);

      glTexCoord2f(1, 1);
      glVertex3f(0.32f, 0.1f, 0.1f);

      glTexCoord2f(1, 0);
      glVertex3f(0.32f, -0.1f, 0.1f);

      glTexCoord2f(0, 0);
      glVertex3f(0.32f, -0.1f, -0.1f);

      glNormal3f(-1, 0, 0);

      glTexCoord2f(0, 1);
      glVertex3f(-0.32f, 0.1f, 0.1f);

      glTexCoord2f(1, 1);
      glVertex3f(-0.32f, 0.1f, -0.1f);

      glTexCoord2f(1, 0);
      glVertex3f(-0.32f, -0.1f, -0.1f);

      glTexCoord2f(0, 0);
      glVertex3f(-0.32f, -0.1f, 0.1f);

      glNormal3f(0, 0, -1);

      glTexCoord2f(0, 1);
      glVertex3f(-0.1f, 0.1f, -0.32f);

      glTexCoord2f(1, 1);
      glVertex3f(0.1f, 0.1f, -0.32f);

      glTexCoord2f(1, 0);
      glVertex3f(0.1f, -0.1f, -0.32f);

      glTexCoord2f(0, 0);
      glVertex3f(-0.1f, -0.1f, -0.32f);

      glNormal3f(0, 0, 1);

      glTexCoord2f(0, 1);
      glVertex3f(0.1f, 0.1f, 0.32f);

      glTexCoord2f(1, 1);
      glVertex3f(-0.1f, 0.1f, 0.32f);

      glTexCoord2f(1, 0);
      glVertex3f(-0.1f, -0.1f, 0.32f);

      glTexCoord2f(0, 0);
      glVertex3f(0.1f, -0.1f, 0.32f);
    glEnd();

    glBindTexture(GL_TEXTURE_2D, 0);

  glPopMatrix();
}

extern void draw_cake() {
  bool color_material = glIsEnabled(GL_COLOR_MATERIAL);
  if(!color_material)
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

  if(!color_material)
    glDisable(GL_COLOR_MATERIAL);
}
