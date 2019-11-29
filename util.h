#ifndef UTIL_H_
#define UTIL_H_

#include <GL/glut.h>

extern double to_radians(double angle);
extern double clamp_yaw(double yaw);
extern double clamp_pitch(double pitch);

extern void normalize3(GLfloat *vector);

#endif
