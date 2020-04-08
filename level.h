#ifndef LEVEL_H_
#define LEVEL_H_

#include <GL/glut.h>

#include "shared.h"
#include "light.h"
#include "portal.h"
#include "wall.h"
#include "logic.h"
#include "render.h"

extern void load_level(double *x, double *y, double *z);
extern void draw_world();

#endif
