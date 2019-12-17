#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <limits.h>
#include <GL/glut.h>

#include "shared.h"
#include "util.h"
#include "input.h"
#include "light.h"
#include "portal.h"
#include "wall.h"
#include "logic.h"

// Player Position
static double x = 0;
static double y = 1.0f;
static double z = 0;

static double new_x = 0;
static double new_z = 0;

// Camera Rotation
static double yaw = 0;
static double pitch = 0;

// Look Vector
static double look_x = 1;
static double look_y = 0;
static double look_z = 0;

// Lights
static GLfloat main_light_position[] = { 0, 5, 0, 1 };

// GLUT Event Handlers
static void on_display(void);
static void on_reshape(int width, int height);
static void on_mouse_click(int button, int state, int m_x, int m_y);
static void on_timer(int data);
static void on_close(void);

int main(int argc, char** argv) {
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_STENCIL | GLUT_DOUBLE);

  glutInitWindowSize(window_width, window_height);
  glutInitWindowPosition(300, 300);
  glutCreateWindow("Non-Euclidean Space");

  glutDisplayFunc(on_display);
  glutReshapeFunc(on_reshape);
  glutKeyboardFunc(on_keyboard);
  glutKeyboardUpFunc(on_keyboard_up);
  glutMouseFunc(on_mouse_click);
  glutPassiveMotionFunc(on_mouse_move);
  // For some reason I get an implicit declaration warning for this
  // Even though it works and is in the API
  glutCloseFunc(on_close);

  glutSetCursor(GLUT_CURSOR_NONE);
  glutWarpPointer(window_width/2, window_height/2);

  glEnable(GL_DEPTH_TEST);
  glEnable(GL_CULL_FACE);
  glEnable(GL_LIGHTING);
  glEnable(GL_NORMALIZE);

  glClearColor(0, 0, 0, 0);

  glutTimerFunc(20, on_timer, 0);

  // Initialize walls array
  initialize_walls(wall_allocation_size);
  unsigned int wall_front = create_wall(8, 2, 0, -1, 0, 0, 16, 4, &material_concrete_green);
  unsigned int wall_back = create_wall(-8, 2, 0, 1, 0, 0, 16, 4, &material_concrete_red);
  unsigned int wall_left = create_wall(0, 2, -8, 0, 0, 1, 16, 4, &material_concrete_yellow);
  unsigned int wall_right = create_wall(0, 2, 8, 0, 0, -1, 16, 4, &material_concrete_blue);

  // Initialize portals array
  initialize_portals(portal_allocation_size);
  unsigned int p1 = create_portal(8, 1.5f, 0, walls[wall_front], 6, 3);
  unsigned int p2 = create_portal(0, 1.5f, -8, walls[wall_left], 6, 3);
  unsigned int p3 = create_portal(0, 1.5f, 8, walls[wall_right], 6, 3);
  // unsigned int p4 = create_portal(8, 1.5f, 6, &wall_front, 2, 3);

  // Link portals to show each other's view
  // (void)link_portals(portals[p1], portals[p4]);
  (void)link_portals(portals[p2], portals[p3]);

  // (void)link_portals(portals[p1], portals[p2]);

  glutMainLoop();

  return 0;
}

static void update_camera() {
  look_x = cos(to_radians(pitch))*cos(to_radians(yaw));
  look_y = -sin(to_radians(pitch));
  look_z = cos(to_radians(pitch))*sin(to_radians(yaw));
}

static void on_timer(int data) {
  if(data != 0)
    return;

  yaw = clamp_yaw(yaw + mouse_dx * MOUSE_SENSITIVITY);
  pitch = clamp_pitch(pitch + mouse_dy * MOUSE_SENSITIVITY);

  update_mouse();

  new_x = x;
  new_z = z;

  // TODO: Normalize move vector
  if(is_forward_pressed) {
    new_x += look_x * PLAYER_SPEED;
    new_z += look_z * PLAYER_SPEED;
  }
  if(is_backward_pressed) {
    new_x -= look_x * PLAYER_SPEED;
    new_z -= look_z * PLAYER_SPEED;
  }
  if(is_left_pressed) {
    new_x += cos(to_radians(yaw)-PI/2) * PLAYER_SPEED;
    new_z += sin(to_radians(yaw)-PI/2) * PLAYER_SPEED;
  }
  if(is_right_pressed) {
    new_x += cos(to_radians(yaw)+PI/2) * PLAYER_SPEED;
    new_z += sin(to_radians(yaw)+PI/2) * PLAYER_SPEED;
  }

  // Check if the player should be teleported
  for(unsigned int i = 0; i < portal_count; i++) {
    if(portals[i] == NULL)
      continue;

    // If the portal is linked and we passed the portal plane
    float *offset_pos = get_offset_position(portals[i]);
    if(is_linked(portals[i]) && sidexz3v(offset_pos, portals[i]->normal, x, z)*sidexz3v(offset_pos, portals[i]->normal, new_x, new_z) <= 0) {

      float d = det2f(new_x-x, new_z-z, -portals[i]->normal[2]*portals[i]->width/2, portals[i]->normal[0]*portals[i]->width/2);

      // Calculate the intersection parameter on the portal
      float s = det2f(offset_pos[0]-x, offset_pos[2]-z, new_x-x, new_z-z)/d;

      // Check if the player went through the portal, otherwise continue
      if(fabs(s) >= 1) {
        free(offset_pos);
        continue;
      }

      // Calculate the intersection parameter on the player move vector
      float t = det2f(offset_pos[0]-x, offset_pos[2]-z, -portals[i]->normal[2]*portals[i]->width/2, portals[i]->normal[0]*portals[i]->width/2)/d;

      // Calculate the yaw change
      float angle = angle_between2f(-portals[i]->normal[0], -portals[i]->normal[2], portals[i]->link->normal[0], portals[i]->link->normal[2]);

      // Move the player
      float offset_x = (new_x-x)*(1.0f-t);
      float offset_z = (new_z-z)*(1.0f-t);

      new_x = portals[i]->link->position[0] + portals[i]->link->normal[0]*0.1f + portals[i]->link->normal[2]*portals[i]->link->width/2*s;
      new_z = portals[i]->link->position[2] + portals[i]->link->normal[2]*0.1f - portals[i]->link->normal[0]*portals[i]->link->width/2*s;

      new_x += cos(angle)*offset_x - sin(angle)*offset_z;
      new_z += sin(angle)*offset_x + cos(angle)*offset_z;

      // Update player rotation
      yaw = clamp_yaw(yaw + angle);

      free(offset_pos);
      break;
    }

    free(offset_pos);
  }

  x = new_x;
  z = new_z;

  update_camera();

  // Check collisions
  float dist = 0;
  // Is the player hitting a wall
  for(unsigned int i = 0; i < wall_count; i++) {
    if(is_colliding_with_wall(x, z, walls[i], &dist)) {
      // And is there not a portal there
      // TODO attach portals to walls so we don't have to check collisions with all of them
      bool in_portal = false;
      for(unsigned int j = 0; j < portal_count; j++) {
        if(portals[j] == NULL || portals[j]->wall != walls[i])
          continue;

        if(is_linked(portals[j]) && is_colliding_with_portal(x, y, z, portals[j])) {
          in_portal = true;
          break;
        }
      }
      if(!in_portal) {
        // Move the player back
        x = x + (PLAYER_RADIUS-dist)*walls[i]->normal[0];
        z = z + (PLAYER_RADIUS-dist)*walls[i]->normal[2];
      }
    }
  }

  glutPostRedisplay();

  glutTimerFunc(20, on_timer, 0);
}

static void on_mouse_click(int button, int state, int m_x, int m_y) {
  (void) m_x;
  (void) m_y;

  // Buttons are reported from X display server, not sure if they are standard
  // 0 - LMB
  // 2 - RMB
  if(button!=0 && button!=2)
    return;

  // 0 - On mouse down
  // 1 - On mouse up
  if(state==0)
    return;

  // Free the previous portal
  if(button == 0)
    free_user_portal(BLUE);
  else if(button == 2)
    free_user_portal(ORANGE);

  // Find the closest wall the player is looking at
  float t = INT_MAX;
  wall *w = NULL;
  for(unsigned int i = 0; i < wall_count; i++) {
    // If the look vector is parallel to the wall there is no intersection
    float d = dot_prod3f(look_x, look_y, look_z, walls[i]->normal[0], walls[i]->normal[1], walls[i]->normal[2]);
    if(d == 0)
      continue;

    // Calculate the intersection parameter
    float nt = -(dot_prod3f(x, y, z, walls[i]->normal[0], walls[i]->normal[1], walls[i]->normal[2])+(-walls[i]->normal[0]*walls[i]->position[0]-walls[i]->normal[1]*walls[i]->position[1]-walls[i]->normal[2]*walls[i]->position[2]))/
    dot_prod3f(look_x, look_y, look_z, walls[i]->normal[0], walls[i]->normal[1], walls[i]->normal[2]);

    // We don't care about walls behind the player
    if(nt<=0)
      continue;

      // Check if the intersection is inside the wall and there is enough space to make a portal there
    float dist_h = sqrt((x+look_x*nt-walls[i]->position[0])*(x+look_x*nt-walls[i]->position[0])+(z+look_z*nt-walls[i]->position[2])*(z+look_z*nt-walls[i]->position[2]));
    float dist_v = y+look_y*nt-walls[i]->position[1];

    if(y+look_y*nt > 3)
      continue;

    if(fabs(dist_h)>walls[i]->width/2-PORTAL_WIDTH/2 || fabs(dist_v)>walls[i]->height/2)
      continue;

    // If the current wall is closer use it instead
    if(nt < t) {
      t = nt;
      w = walls[i];
    }
  }

  // Check if there was an intersection point
  if(t == INT_MAX)
    return;

  float ny = y+look_y*t < PORTAL_HEIGHT/2 ? PORTAL_HEIGHT/2 : y+look_y*t;

  // Check if there is another portal at that position
  for(unsigned int i = 0; i < portal_count; i++) {
    if(portals[i] != NULL && portals[i]->wall == w && sqrt(fabs(x+look_x*t-portals[i]->position[0])*fabs(x+look_x*t-portals[i]->position[0])+fabs(z+look_z*t-portals[i]->position[2])*fabs(z+look_z*t-portals[i]->position[2]))<portals[i]->width/2+PORTAL_WIDTH/2) {
      return;
    }
  }

  // Create the portal on the closest wall
  // printf("%f - %f %f %f\n", t, x+look_x*t, y+look_y*t, z+look_z*t);
  if(button == 0)
    create_user_portal(BLUE, x+look_x*t, ny, z+look_z*t, w);
  else if(button == 2)
    create_user_portal(ORANGE, x+look_x*t, ny, z+look_z*t, w);
}

static void draw_world() {
  glEnable(GL_CULL_FACE);
  glDisable(GL_LIGHTING);

  // Draw the inside of the skybox sphere
  glCullFace(GL_FRONT);

  // Skybox
  glColor3f(0.2f, 0.8f, 1);
  glutSolidSphere(50, 10, 10);

  glEnable(GL_LIGHTING);

  // Draw the front faces of the walls
  glCullFace(GL_BACK);

  init_light(GL_LIGHT0, main_light_position, &light_basic);

  // Draw the walls
  for(unsigned int i = 0; i < wall_count; i++) {
    draw_wall(walls[i]);
  }

  // Draw the floor
  glBegin(GL_QUADS);
    set_material(&material_concrete_white);
    glNormal3f(0, 1, 0);

    glVertex3f(-8.0f, 0, -8.0f);
    glVertex3f(-8.0f, 0, 8.0f);
    glVertex3f(8.0f, 0, 8.0f);
    glVertex3f(8.0f, 0, -8.0f);
  glEnd();

  glDisable(GL_CULL_FACE);

  glPushMatrix();
    set_material(&material_concrete_yellow);
    glTranslatef(-3, 1, 0);
    glutSolidTeapot(1);
  glPopMatrix();
}

void draw_scene(int level) {
  portal p;
  for(unsigned int i = 0; i < portal_count; i++) {
    if(portals[i] == NULL)
      continue;

    p = *portals[i];

    // Create a mask for the portal in the stencil buffer
    glEnable(GL_STENCIL_TEST);

    // Disable drawing to the color and depth buffer
    glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
    glDepthMask(GL_FALSE);
    glDisable(GL_DEPTH_TEST);

    // Fail the stencil test on every pixel drawn inside the current level
    // Increment the stencil buffer value for every failed test
    glStencilFunc(GL_NOTEQUAL, level, 0xFF);
    glStencilOp(GL_INCR, GL_KEEP, GL_KEEP);
    glStencilMask(0xFF);

    // Draw the portal frame in the stencil buffer
    draw_portal_frame(&p);

    // Enable drawing to the color and depth buffers only where the portal was
    glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
    glDepthMask(GL_TRUE);
    glClear(GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);

    // If the portal is linked draw the link's view
    // Otherwise only draw the frame
    if(is_linked(&p)) {
      glPushMatrix();

        // Add a clipping plane so we don't render objects behind the destination portal
        double clip_plane[] = {
          -p.normal[0], -p.normal[1], -p.normal[2],
          p.normal[0]*p.position[0] + p.normal[1]*p.position[1] + p.normal[2]*p.position[2]
        };
        glClipPlane(GL_CLIP_PLANE0, clip_plane);
        glEnable(GL_CLIP_PLANE0);

        // Flip the camera
        glRotatef(180, 0, 1, 0);
        // Find the transformation between the portals
        // We multiply by 0.99 so it doesn't look disconnected
        // because the portal is offset from the wall a few pixels
        glTranslatef(-p.position[0]-0.005f*p.normal[0], p.position[1], -p.position[2]-0.005f*p.normal[2]);
        glRotatef(angle_betweenxz3v(p.normal, p.link->normal), 0, 1, 0);
        glTranslatef(-p.link->position[0]-0.005f*p.link->normal[0], -p.link->position[1], -p.link->position[2]-0.005f*p.link->normal[2]);

        if(level == MAX_RECURSION_LEVEL) {
          // Disable drawing to the stencil buffer
          glStencilMask(0x00);

          // Draw where the stencil value is equal to the current level + 1
          glStencilFunc(GL_EQUAL, level + 1, 0xFF);

          draw_world();
        } else {
          // Draw the view recursively from the current portal
          draw_scene(level + 1);
        }

        glDisable(GL_CLIP_PLANE0);

      glPopMatrix();
    } else {
      // Disable drawing to the stencil buffer
      glStencilMask(0x00);

      // Draw where the stencil value is equal to the current level + 1
      glStencilFunc(GL_EQUAL, level + 1, 0xFF);

      draw_portal_frame(&p);
    }

    // Revert the stencil buffer values

    // Create a mask for the portal in the stencil buffer
    glEnable(GL_STENCIL_TEST);

    // Disable drawing to the color and depth buffer
    glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
    glDepthMask(GL_FALSE);
    glDisable(GL_DEPTH_TEST);

    // Fail the stencil test on every pixel drawn inside the current level
    // Decrement the stencil buffer value for every failed test
    glStencilFunc(GL_NOTEQUAL, level+1, 0xFF);
    glStencilOp(GL_DECR, GL_KEEP, GL_KEEP);
    glStencilMask(0xFF);

    // Draw the portal frame in the stencil buffer
    draw_portal_frame(&p);

    glDisable(GL_STENCIL_TEST);
  }

  // Run a depth test to find objects closer to the camera than the portals
  glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
  glEnable(GL_DEPTH_TEST);
  glDepthMask(GL_TRUE);
  glClear(GL_DEPTH_BUFFER_BIT);

  for(unsigned int i = 0; i < portal_count; i++) {
    if(portals[i] == NULL)
      continue;

    draw_portal_frame(portals[i]);
  }

  // Only draw at the current level
  glEnable(GL_STENCIL_TEST);
  glStencilMask(0x00);
  glStencilFunc(GL_LEQUAL, level, 0xFF);

  // Render the world from the players perspective
  glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);

  draw_world();

  glDisable(GL_STENCIL_TEST);
}

static void on_display(void) {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  gluLookAt(x, y, z, x + look_x, y + look_y, z + look_z, 0.0f, 1.0f, 0.0f);

  draw_scene(0);

  glutSwapBuffers();
}

static void on_reshape(int width, int height) {
  window_width = width;
  window_height = height;

  glViewport(0, 0, (GLsizei)width, (GLsizei)height);

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective((GLdouble)60, (GLdouble)width / (GLdouble)height, (GLdouble)0.1, (GLdouble)100.0);

  glutPostRedisplay();
}

static void on_close(void) {
  free_portals();
  free_walls();
}
