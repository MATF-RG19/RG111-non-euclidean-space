#ifndef UTIL_H_
#define UTIL_H_

#include <GL/glut.h>

extern double to_radians(double angle);
extern double to_degrees(double angle);
extern double clamp_yaw(double yaw);
extern double clamp_pitch(double pitch);

extern int sgn(float x);
extern float norm2f(float x, float y);
extern float norm3v(float *vector);
extern void normalize3v(GLfloat *vector);
extern float dot_prod2f(float x1, float y1, float x2, float y2);
extern float dot_prod3v(float *v1, float *v2);
extern float orientation2f(float x1, float y1, float x2, float y2);
extern float orientationy3f(float *v1, float *v2);

#endif
