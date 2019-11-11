#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <GL/glut.h>

#include "constants.h"
#include "util.h"
#include "input.h"

static int window_width = 960;
static int window_height = 540;

// Player Position
static double x = 0;
static double y = 1.0f;
static double z = 0;

// Camera Rotation
static double yaw = 0;
static double pitch = 0;

// Look Vector
static double look_x = 1;
static double look_y = 0;
static double look_z = 0;

// GLUT Event Handlers
static void on_display(void);
static void on_reshape(int width, int height);
static void on_mouse_click(int button, int state, int m_x, int m_y);
static void on_timer(int data);

int main(int argc, char** argv) {
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);

  glutInitWindowSize(window_width, window_height);
  glutInitWindowPosition(300, 300);
  glutCreateWindow("Non-Euclidean Space");

  glutDisplayFunc(on_display);
  glutReshapeFunc(on_reshape);
  glutKeyboardFunc(on_keyboard);
  glutKeyboardUpFunc(on_keyboard_up);
  glutMouseFunc(on_mouse_click);
  glutPassiveMotionFunc(on_mouse_move);

  // Leave commented for debugging
  // glutSetCursor(GLUT_CURSOR_NONE);
  // glutWarpPointer(window_width/2, window_height/2);

  glEnable(GL_DEPTH_TEST);

  glClearColor(0, 0, 0, 0);

  glutTimerFunc(20, on_timer, 0);

  glutMainLoop();

  return 0;
}

void update_camera() {
  look_x = cos(to_radians(pitch))*cos(to_radians(yaw));
  look_y = -sin(to_radians(pitch));
  look_z = cos(to_radians(pitch))*sin(to_radians(yaw));
}

static void on_timer(int data) {
  if(data != 0)
    return;

  yaw = clamp_yaw(yaw + mouse_dx * MOUSE_SENSITIVITY);
  pitch = clamp_pitch(pitch + mouse_dy * MOUSE_SENSITIVITY);

  update_mouse();
  update_camera();

  if(is_forward_pressed) {
    x += look_x * PLAYER_SPEED;
    z += look_z * PLAYER_SPEED;
  }
  if(is_backward_pressed) {
    x -= look_x * PLAYER_SPEED;
    z -= look_z * PLAYER_SPEED;
  }
  if(is_left_pressed) {
    x += cos(to_radians(yaw)-PI/2) * PLAYER_SPEED;
    z += sin(to_radians(yaw)-PI/2) * PLAYER_SPEED;
  }
  if(is_right_pressed) {
    x += cos(to_radians(yaw)+PI/2) * PLAYER_SPEED;
    z += sin(to_radians(yaw)+PI/2) * PLAYER_SPEED;
  }

  glutPostRedisplay();

  glutTimerFunc(20, on_timer, 0);
}

static void on_mouse_click(int button, int state, int m_x, int m_y) {
  (void) button;
  (void) state;
  (void) m_x;
  (void) m_y;
}

static void on_display(void) {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  gluLookAt(x, y, z, x + look_x, y + look_y, z + look_z, 0.0f, 1.0f, 0.0f);

  glBegin(GL_QUADS);
    glVertex3f(-1.0f, 0, -1.0f);
    glVertex3f(-1.0f, 0, 1.0f);
    glVertex3f(1.0f, 0, 1.0f);
    glVertex3f(1.0f, 0, -1.0f);
  glEnd();

  glutSwapBuffers();
}

static void on_reshape(int width, int height) {
  window_width = width;
  window_height = height;

  glViewport(0, 0, (GLsizei)width, (GLsizei)height);

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective((GLdouble)60, (GLdouble)width / (GLdouble)height, (GLdouble)0.1, (GLdouble)100.0);

  glutPostRedisplay();
}
