#include "logic.h"

// Wall data
unsigned int wall_count = 0;
static unsigned int wall_allocated = 0;
unsigned int wall_allocation_size = 5;
wall **walls;

// Portal data
// First 2 ids are reserved for the user portals
unsigned int portal_count = 2;
static unsigned int portal_allocated = 0;
unsigned int portal_allocation_size = 5;
portal **portals;

// Wall functions
void initialize_walls(unsigned int n) {
  walls = calloc(n, sizeof(wall *));
  if(walls == NULL) {
    fprintf(stderr, "Error Allocating Memory");
    exit(EXIT_FAILURE);
  }
  wall_allocated = n;
  for(unsigned int i = 0; i < n; i++) {
    walls[i] = NULL;
  }
}

static void realloc_walls(unsigned int step) {
  walls = realloc(walls, (wall_allocated+step)*sizeof(wall *));
  if(walls == NULL) {
    fprintf(stderr, "Error Allocating Memory");
    exit(EXIT_FAILURE);
  }
  for(unsigned int i = wall_allocated; i < wall_allocated+step; i++) {
    walls[i] = NULL;
  }
  wall_allocated += step;
}

int create_wall(float x, float y, float z, float normal_x, float normal_y, float normal_z, float width, float height, bool anchor, material* material) {
  wall *w = malloc(sizeof(wall));
  w->position[0] = x;
  w->position[1] = y;
  w->position[2] = z;
  w->normal[0] = normal_x;
  w->normal[1] = normal_y;
  w->normal[2] = normal_z;
  w->width = width;
  w->height = height;
  w->anchor = anchor;
  w->material = material;

  if(wall_allocated == wall_count) {
    realloc_walls(wall_allocation_size);
  }

  walls[wall_count] = w;
  return wall_count++;
}

void free_walls() {
  for(unsigned int i = 0; i < wall_count; i++) {
    if(walls[i] == NULL)
      continue;

    free(walls[i]);
  }
}

// Portal functions
void initialize_portals(unsigned int n) {
  portals = calloc(n, sizeof(portal *));
  if(portals == NULL) {
    fprintf(stderr, "Error Allocating Memory");
    exit(EXIT_FAILURE);
  }
  portal_allocated = n;
  for(unsigned int i = 0; i < n; i++) {
    portals[i] = NULL;
  }
}

static void realloc_portals(unsigned int step) {
  portals = realloc(portals, (portal_allocated+step)*sizeof(portal *));
  if(portals == NULL) {
    fprintf(stderr, "Error Allocating Memory");
    exit(EXIT_FAILURE);
  }
  for(unsigned int i = portal_allocated; i < portal_allocated+step; i++) {
    portals[i] = NULL;
  }
  portal_allocated += step;
}

int create_user_portal(portal_color c, float x, float y, float z, wall* wall) {
  if(portals[c]!=NULL)
    free_user_portal(c);

  if(wall == NULL || !wall->anchor)
    return -1;

  portal *p = malloc(sizeof(portal));
  p->position[0] = x;
  p->position[1] = y;
  p->position[2] = z;
  p->normal[0] = wall->normal[0];
  p->normal[1] = wall->normal[1];
  p->normal[2] = wall->normal[2];
  p->width = PORTAL_WIDTH;
  p->height = PORTAL_HEIGHT;
  p->wall = wall;
  p->link = NULL;
  portals[c] = p;

  (void) link_portals(portals[BLUE], portals[ORANGE]);
  return c;
}

int create_portal(float x, float y, float z, wall* wall, float width, float height) {
  if(wall == NULL || !wall->anchor)
    return -1;

  portal *p = malloc(sizeof(portal));
  p->position[0] = x;
  p->position[1] = y;
  p->position[2] = z;
  p->normal[0] = wall->normal[0];
  p->normal[1] = wall->normal[1];
  p->normal[2] = wall->normal[2];
  p->width = width;
  p->height = height;
  p->wall = wall;
  p->link = NULL;

  if(portal_allocated == portal_count) {
    realloc_portals(portal_allocation_size);
  }

  portals[portal_count] = p;
  return portal_count++;
}

void free_user_portal(portal_color c) {
  if(portals[c]==NULL)
    return;

  (void) unlink_portal(portals[c]);

  free(portals[c]);
  portals[c] = NULL;
}

void free_portals() {
  for(unsigned int i = 0; i < portal_count; i++) {
    if(portals[i] == NULL)
      continue;

    (void) unlink_portal(portals[i]);

    free(portals[i]);
  }
}
