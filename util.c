#include "util.h"

double to_radians(double angle) {
  return PI/180.0f*angle;
}

double to_degrees(double angle) {
  return angle/PI*180.0f;
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

extern int sgn(float x) {
  return (x > 0) ? 1 : ((x < 0) ? -1 : 0);
}

extern float norm2f(float x, float y) {
  return sqrt(x*x + y*y);
}

extern float norm3v(float *vector) {
  return sqrt(vector[0]*vector[0] + vector[1]*vector[1] + vector[2]*vector[2]);
}

extern void normalize3v(GLfloat *vector) {
  float norm = norm3v(vector);
  vector[0] = vector[0]/norm;
  vector[1] = vector[1]/norm;
  vector[2] = vector[2]/norm;
}

extern float dot_prod2f(float x1, float y1, float x2, float y2) {
  return x1*x2+y1*y2;
}

extern float dot_prod3f(float x1, float y1, float z1, float x2, float y2, float z2) {
  return x1*x2+y1*y2+z1*z2;
}

extern float dot_prod3v(float *v1, float *v2) {
  return v1[0]*v2[0]+v1[1]*v2[1]+v1[2]*v2[2];
}

extern float orientation2f(float x1, float y1, float x2, float y2) {
  return x1*y2-x2*y1;
}

extern float orientationy3v(float *v1, float *v2) {
  return orientation2f(v1[0], v1[2], v2[0], v2[2]);
}

extern int side2f(float lx, float ly, float nx, float ny, float x, float y) {
  return sgn(nx*x+ny*y-nx*lx-ny*ly);
}

extern int sidexz3v(float *position, float *normal, float x, float z) {
  return side2f(position[0], position[2], normal[0], normal[2], x, z);
}

extern float det2f(float x1, float y1, float x2, float y2) {
  return x1*y2-y1*x2;
}

extern float angle_between2f(float x1, float y1, float x2, float y2) {
  float angle = to_degrees(acos(dot_prod2f(x1, y1, x2, y2)/norm2f(x1, y1)/norm2f(x2, y2)));
  int orientation = sgn(orientation2f(x1, y1, x2, y2));
  return orientation==0 ? angle : orientation*angle;
}

extern float angle_betweenxz3v(float *v1, float *v2) {
  return angle_between2f(v1[0], v1[2], v2[0], v2[2]);
}
