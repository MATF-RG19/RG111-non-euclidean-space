#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <GL/glut.h>

#define PI 3.14159265359
#define MOUSE_SENSITIVITY 0.2
#define PLAYER_SPEED 0.1

static int window_width = 960;
static int window_height = 540;

// Player Position
static double x = 0;
static double y = 1.0f;
static double z = 0;

// Camera Rotation
static double yaw = 0;
static double pitch = 0;

static double look_x = 1;
static double look_y = 0;
static double look_z = 0;

// Mouse Position
static int mouse_x = 0;
static int mouse_y = 0;

static void on_display(void);
static void on_reshape(int width, int height);
static void on_keyboard(unsigned char key, int m_x, int m_y);
static void on_mouse_click(int button, int state, int m_x, int m_y);
static void on_mouse_move(int m_x, int m_y);

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

  // Leave commented for debugging
  // glutSetCursor(GLUT_CURSOR_NONE);
  // glutWarpPointer(window_width/2, window_height/2);

  glEnable(GL_DEPTH_TEST);

  glClearColor(0, 0, 0, 0);

  glutMainLoop();

  return 0;
}

double to_radians(double angle) {
  return PI/180.0f*angle;
}

void update_camera() {
  look_x = cos(to_radians(pitch))*cos(to_radians(yaw));
  look_y = -sin(to_radians(pitch));
  look_z = cos(to_radians(pitch))*sin(to_radians(yaw));
}

static void on_keyboard(unsigned char key, int m_x, int m_y) {
  (void) m_x;
  (void) m_y;
  switch(key) {
    case 27:
      exit(0);
      break;
    case 'w':
    case 'W':
      x += look_x * PLAYER_SPEED;
      z += look_z * PLAYER_SPEED;
      glutPostRedisplay();
      break;
    case 's':
    case 'S':
      x -= look_x * PLAYER_SPEED;
      z -= look_z * PLAYER_SPEED;
      glutPostRedisplay();
      break;
    case 'a':
    case 'A':
      x += cos(to_radians(yaw)-PI/2) * PLAYER_SPEED;
      z += sin(to_radians(yaw)-PI/2) * PLAYER_SPEED;
      glutPostRedisplay();
      break;
    case 'd':
    case 'D':
      x += cos(to_radians(yaw)+PI/2) * PLAYER_SPEED;
      z += sin(to_radians(yaw)+PI/2) * PLAYER_SPEED;
      glutPostRedisplay();
      break;
  }
}

static void on_mouse_click(int button, int state, int m_x, int m_y) {
  (void) button;
  (void) state;
  (void) m_x;
  (void) m_y;
}

static void on_mouse_move(int m_x, int m_y) {
  int dx = m_x - mouse_x;
  int dy = m_y - mouse_y;

  mouse_x = m_x;
  mouse_y = m_y;

  // TODO Fix getting stuck when warping the pointer
  // glutWarpPointer(window_width/2, window_height/2);

  yaw += dx * MOUSE_SENSITIVITY;
  if(yaw > 180.0)
    yaw -= 360.0;
  else if(yaw < -180.0)
    yaw += 360.0;

  pitch += dy * MOUSE_SENSITIVITY;
  if(pitch >= 90.0)
    pitch = 89.9;
  else if(pitch <= -90.0)
    pitch = -89.9;

  update_camera();
  glutPostRedisplay();
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
  gluPerspective(60, (GLdouble)width / (GLdouble)height, 0.1 , 100.0);

  glutPostRedisplay();
}
