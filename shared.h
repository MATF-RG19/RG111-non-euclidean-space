#ifndef SHARED_H_
#define SHARED_H_

#define PI 3.14159265359
#define MOUSE_SENSITIVITY 0.2

#define MAX_RECURSION_LEVEL 2

#define PLAYER_RADIUS 0.5f
#define PLAYER_HEIGHT 1.5f
#define PLAYER_SPEED 0.1

#define PORTAL_WIDTH 2.0f
#define PORTAL_HEIGHT 3.0f

typedef enum portal_color {
  BLUE = 0,
  ORANGE = 1
} portal_color;

extern int window_width;
extern int window_height;

#endif
