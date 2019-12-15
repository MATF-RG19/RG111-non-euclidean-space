#ifndef UTIL_H_
#define UTIL_H_

#include <math.h>
#include <GL/glut.h>

#include "shared.h"

extern double to_radians(double angle);
extern double to_degrees(double angle);
extern double clamp_yaw(double yaw);
extern double clamp_pitch(double pitch);

extern int sgn(float x);
extern float norm2f(float x, float y);
extern float norm3v(float *vector);
extern void normalize3v(GLfloat *vector);
extern float dot_prod2f(float x1, float y1, float x2, float y2);
extern float dot_prod3f(float x1, float y1, float z1, float x2, float y2, float z2);
extern float dot_prod3v(float *v1, float *v2);
extern float orientation2f(float x1, float y1, float x2, float y2);
extern float orientationy3v(float *v1, float *v2);
extern int side2f(float lx, float ly, float nx, float ny, float x, float y);
extern int sidexz3v(float *position, float *normal, float x, float z);
extern float det2f(float x1, float y1, float x2, float y2);
extern float angle_between2f(float x1, float y1, float x2, float y2);
extern float angle_betweenxz3v(float *v1, float *v2);

#endif
