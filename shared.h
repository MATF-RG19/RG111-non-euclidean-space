#ifndef SHARED_H_
#define SHARED_H_

#include <stdbool.h>
#include <GL/glut.h>

#define INITIAL_WIDTH 960
#define INITIAL_HEIGHT 540

#define PI 3.14159265359
#define MOUSE_SENSITIVITY 0.2

#define MAX_RECURSION_LEVEL 2

#define PLAYER_HEIGHT 1.0f
#define PLAYER_COLLISION_RADIUS 0.5f
#define PLAYER_COLLISION_HEIGHT 1.5f
#define PLAYER_SPEED 0.1

#define PORTAL_WIDTH 2.0f
#define PORTAL_HEIGHT 3.0f
#define PORTAL_BORDER 0.1f

typedef enum portal_color {
  BLUE = 0,
  ORANGE = 1
} portal_color;

extern int window_width;
extern int window_height;
extern bool fullscreen;

extern GLuint textures[2];

#endif
