#include<math.h>

#include "shared.h"
#include "util.h"

double to_radians(double angle) {
  return PI/180.0f*angle;
}

double clamp_yaw(double yaw) {
  if(yaw > 180.0)
    yaw -= 360.0;
  else if(yaw < -180.0)
    yaw += 360.0;
  return yaw;
}

double clamp_pitch(double pitch) {
  if(pitch >= 90.0)
    pitch = 89.9;
  else if(pitch <= -90.0)
    pitch = -89.9;
  return pitch;
}

extern float norm2f(float x, float y) {
  return sqrt(x*x + y*y);
}

extern float norm3v(float *vector) {
  return sqrt(vector[0]*vector[0] + vector[1]*vector[1] + vector[2]*vector[2]);
}

extern void normalize3(GLfloat *vector) {
  float sum = vector[0] + vector[1] + vector[2];
  vector[0] = vector[0]/sum;
  vector[1] = vector[1]/sum;
  vector[2] = vector[2]/sum;
}
