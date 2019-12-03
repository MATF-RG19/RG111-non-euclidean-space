#ifndef UTIL_H_
#define UTIL_H_

#include <GL/glut.h>

extern double to_radians(double angle);
extern double clamp_yaw(double yaw);
extern double clamp_pitch(double pitch);

extern float norm2f(float x, float y);
extern float norm3v(float *vector);
extern void normalize3v(GLfloat *vector);

#endif
