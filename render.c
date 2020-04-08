#include "render.h"

// Textures
GLuint textures[NUM_TEXTURES];

extern void load_textures() {
  glGenTextures(NUM_TEXTURES, textures);

  int alignment;
  glGetIntegerv(GL_UNPACK_ALIGNMENT, &alignment);

  image *image = init_image();

  read_image(image, TEXTURE_WALL_DARK, alignment);

  glBindTexture(GL_TEXTURE_2D, textures[0]);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image->width, image->height, 0, GL_RGB, GL_UNSIGNED_BYTE, image->pixels);

  read_image(image, TEXTURE_WALL_LIGHT, alignment);

  glBindTexture(GL_TEXTURE_2D, textures[1]);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image->width, image->height, 0, GL_RGB, GL_UNSIGNED_BYTE, image->pixels);

  read_image(image, TEXTURE_HEART, alignment);

  glBindTexture(GL_TEXTURE_2D, textures[2]);
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

extern void draw_companion_cube() {
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

    glDisable(GL_COLOR_MATERIAL);

    // Faces
    glColor3f(0.6f , 0.6f, 0.6f);

    glPushMatrix();
      glTranslatef(0.31f, 0, 0);
      glBegin(GL_TRIANGLE_FAN);
      for(int i = 0; i < 15; i++) {
        glVertex3f(0, 0.14*cos((float)i/15*2*PI), 0.14*sin((float)i/15*2*PI));
      }
      glEnd();
    glPopMatrix();

    glPushMatrix();
      glTranslatef(0, 0, 0.31f);
      glBegin(GL_TRIANGLE_FAN);
      for(int i = 0; i < 15; i++) {
        glVertex3f(0.14*sin((float)i/15*2*PI), -0.14*cos((float)i/15*2*PI), 0);
      }
      glEnd();
    glPopMatrix();

    glPushMatrix();
      glTranslatef(-0.31f, 0, 0);
      glBegin(GL_TRIANGLE_FAN);
      for(int i = 0; i < 15; i++) {
        glVertex3f(0, -0.14*cos((float)i/15*2*PI), 0.14*sin((float)i/15*2*PI));
      }
      glEnd();
    glPopMatrix();

    glPushMatrix();
      glTranslatef(0, 0, -0.31f);
      glBegin(GL_TRIANGLE_FAN);
      for(int i = 0; i < 15; i++) {
        glVertex3f(0.14*sin((float)i/15*2*PI), 0.14*cos((float)i/15*2*PI), 0);
      }
      glEnd();
    glPopMatrix();

    glBindTexture(GL_TEXTURE_2D, textures[2]);
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
