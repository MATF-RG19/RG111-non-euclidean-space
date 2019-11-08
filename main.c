#include <stdio.h>
#include <stdlib.h>
#include <GL/glut.h>

static int window_width = 960;
static int window_height = 540;

static void on_display(void);
static void on_reshape(int width, int height);
static void on_keyboard(unsigned char key, int x, int y);
static void on_mouse_click(int button, int state, int x, int y);
static void on_mouse_move(int x, int y);

int main(int argc, char** argv) {
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);

  glutInitWindowSize(window_width, window_height);
  glutInitWindowPosition(300, 300);
  glutCreateWindow("Non-Euclidean Space");

  glutDisplayFunc(on_display);
  glutReshapeFunc(on_reshape);
  glutKeyboardFunc(on_keyboard);
  glutMouseFunc(on_mouse_click);
  glutPassiveMotionFunc(on_mouse_move);

  glutSetCursor(GLUT_CURSOR_NONE);

  glEnable(GL_DEPTH_TEST);

  glClearColor(0, 0, 0, 0);

  glutMainLoop();

  return 0;
}

static void on_keyboard(unsigned char key, int x, int y) {
  switch(key) {
    case 27:
      exit(0);
      break;
  }
}

static void on_mouse_click(int button, int state, int x, int y) {

}

static void on_mouse_move(int x, int y) {

}

static void on_display(void) {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glutSwapBuffers();
}

static void on_reshape(int width, int height) {
  window_width = width;
  window_height = height;
  glutPostRedisplay();
}
