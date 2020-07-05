#ifndef LOGIC_H_
#define LOGIC_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>
#include <GL/glut.h>

#include "shared.h"
#include "portal.h"
#include "wall.h"
#include "util.h"

// Wall data
extern unsigned int wall_count;
extern unsigned int wall_allocation_size;
extern wall **walls;

// Portal data
extern unsigned int portal_count;
extern unsigned int portal_allocation_size;
extern portal **portals;

// Game Logic
extern void check_teleportation(double *x, double *y, double *z, double new_x, double new_y, double new_z, double *yaw);
extern void check_collisions(double *x, double *y, double *z);
extern void place_portal(double x, double y, double z, double look_x, double look_y, double look_z, portal_color color);

extern void initialize_walls(unsigned int n);
extern int create_wall(float x, float y, float z, float normal_x, float normal_y, float normal_z, float width, float height, bool anchor, material* material);
extern void free_walls();

extern void initialize_portals(unsigned int n);
extern int create_user_portal(portal_color c, float x, float y, float z, wall* wall);
extern int create_portal(float x, float y, float z, wall* wall, float width, float height);
extern void free_user_portal(portal_color c);
extern void free_portals();

#endif
