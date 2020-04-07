#include "render.h"

extern void load_textures() {
  glGenTextures(2, textures);

  int alignment;
  glGetIntegerv(GL_UNPACK_ALIGNMENT, &alignment);

  image *image = init_image();

  read_image(image, "resources/wall_dark.bmp", alignment);

  glBindTexture(GL_TEXTURE_2D, textures[0]);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image->width, image->height, 0, GL_RGB, GL_UNSIGNED_BYTE, image->pixels);

  read_image(image, "resources/wall_light.bmp", alignment);

  glBindTexture(GL_TEXTURE_2D, textures[1]);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image->width, image->height, 0, GL_RGB, GL_UNSIGNED_BYTE, image->pixels);

  free_image(image);

  glBindTexture(GL_TEXTURE_2D, 0);
}

extern void free_textures() {
  glDeleteTextures(2, textures);
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
