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

extern void check_teleportation(double *x, double *y, double *z, double new_x, double new_y, double new_z, double *yaw) {
  // Check if the player should be teleported
  for(unsigned int i = 0; i < portal_count; i++) {
    if(portals[i] == NULL)
      continue;

    // If the portal is linked and we passed the portal plane
    float *offset_pos = get_offset_position(portals[i]);
    if(is_linked(portals[i]) && sidexz3v(offset_pos, portals[i]->normal, *x, *z)*sidexz3v(offset_pos, portals[i]->normal, new_x, new_z) <= 0) {

      float d = det2f(new_x-*x, new_z-*z, -portals[i]->normal[2]*portals[i]->width/2, portals[i]->normal[0]*portals[i]->width/2);

      // Calculate the intersection parameter on the portal
      float s = det2f(offset_pos[0]-*x, offset_pos[2]-*z, new_x-*x, new_z-*z)/d;

      // Check if the player went through the portal, otherwise continue
      if(fabs(s) >= 1) {
        free(offset_pos);
        continue;
      }

      // Calculate the intersection parameter on the player move vector
      float t = det2f(offset_pos[0]-*x, offset_pos[2]-*z, -portals[i]->normal[2]*portals[i]->width/2, portals[i]->normal[0]*portals[i]->width/2)/d;

      // Calculate the yaw change
      float angle = angle_between2f(-portals[i]->normal[0], -portals[i]->normal[2], portals[i]->link->normal[0], portals[i]->link->normal[2]);

      // Move the player
      float offset_x = (new_x-*x)*(1.0f-t);
      float offset_z = (new_z-*z)*(1.0f-t);

      new_x = portals[i]->link->position[0] + portals[i]->link->normal[0]*0.1f + portals[i]->link->normal[2]*portals[i]->link->width/2*s;
      new_y = portals[i]->link->position[1] - portals[i]->position[1] + *y;
      new_z = portals[i]->link->position[2] + portals[i]->link->normal[2]*0.1f - portals[i]->link->normal[0]*portals[i]->link->width/2*s;

      new_x += cos(angle)*offset_x - sin(angle)*offset_z;
      new_z += sin(angle)*offset_x + cos(angle)*offset_z;

      // Update player rotation
      *yaw = clamp_yaw(*yaw + angle);

      free(offset_pos);
      break;
    }

    free(offset_pos);
  }

  *x = new_x;
  *y = new_y;
  *z = new_z;
}

extern void place_portal(double x, double y, double z, double look_x, double look_y, double look_z, portal_color color) {
  // Find the closest wall the player is looking at
  float t = INT_MAX;
  wall *w = NULL;
  float dist_horizontal = 0;

  for(unsigned int i = 0; i < wall_count; i++) {
    // Make sure the player is facing the front side of the wall
    if(sidexz3v(walls[i]->position, walls[i]->normal, x, z) != 1)
      continue;

    // If the look vector is parallel to the wall there is no intersection
    float d = dot_prod3f(look_x, look_y, look_z, walls[i]->normal[0], walls[i]->normal[1], walls[i]->normal[2]);
    if(d == 0)
      continue;

    // Calculate the intersection parameter
    float nt = -(dot_prod3f(x, y, z, walls[i]->normal[0], walls[i]->normal[1], walls[i]->normal[2])+(-walls[i]->normal[0]*walls[i]->position[0]-walls[i]->normal[1]*walls[i]->position[1]-walls[i]->normal[2]*walls[i]->position[2]))/d;

    // We don't care about walls behind the player
    if(nt<=0)
      continue;

    // Check if the intersection is inside the wall
    float dist_h = sqrt((x+look_x*nt-walls[i]->position[0])*(x+look_x*nt-walls[i]->position[0])+(z+look_z*nt-walls[i]->position[2])*(z+look_z*nt-walls[i]->position[2]));
    float dist_v = y+look_y*nt-walls[i]->position[1];

    if(fabs(dist_h)>walls[i]->width/2 || fabs(dist_v)>walls[i]->height/2)
      continue;

    // If the current wall is closer use it instead
    if(nt < t) {
      t = nt;
      w = walls[i];
      dist_horizontal = dist_h;
    }
  }

  // Check if there was an intersection point
  if(t != INT_MAX) {
    // Clamp position on x and z axes
    float nx = x+look_x*t;
    float nz = z+look_z*t;
    if(fabs(dist_horizontal)>w->width/2-PORTAL_WIDTH/2) {
      dist_horizontal = clamp(dist_horizontal, -w->width/2+PORTAL_WIDTH/2, w->width/2-PORTAL_WIDTH/2);
      nx = w->position[0]+sgn(x+look_x*t-w->position[0])*dist_horizontal*w->normal[2];
      nz = w->position[2]+sgn(z+look_z*t-w->position[2])*dist_horizontal*w->normal[0];
    }

    // Clamp position on y axis
    float ny = clamp(y+look_y*t, w->position[1]-w->height/2+PORTAL_HEIGHT/2, w->position[1]+w->height/2-PORTAL_HEIGHT/2);

    // Check if there is another portal at that position
    bool should_create_portal = true;
    for(unsigned int i = 0; i < portal_count; i++) {
      if(portals[i] != NULL && portals[i]->wall == w && sqrt(fabs(nx-portals[i]->position[0])*fabs(nx-portals[i]->position[0])+fabs(nz-portals[i]->position[2])*fabs(nz-portals[i]->position[2]))<portals[i]->width/2+PORTAL_WIDTH/2) {
        should_create_portal = false;;
      }
    }

    if(should_create_portal) {
      // Create the portal on the closest wall
      create_user_portal(color, nx, ny, nz, w);
    }
  }
}

extern void check_collisions(double *x, double *y, double *z) {
  float dist = 0;
  // Is the player hitting a wall
  for(unsigned int i = 0; i < wall_count; i++) {
    if(is_colliding_with_wall(*x, *z, walls[i], &dist)) {
      // And is there not a portal there
      bool in_portal = false;
      for(unsigned int j = 0; j < portal_count; j++) {
        if(portals[j] == NULL || portals[j]->wall != walls[i])
          continue;

        if(is_linked(portals[j]) && is_colliding_with_portal(*x, *y, *z, portals[j])) {
          in_portal = true;
          break;
        }
      }
      if(!in_portal) {
        // Move the player back
        *x = *x + (PLAYER_COLLISION_RADIUS-dist)*walls[i]->normal[0];
        *z = *z + (PLAYER_COLLISION_RADIUS-dist)*walls[i]->normal[2];
      }
    }
  }
}

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
