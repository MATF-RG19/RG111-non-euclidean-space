#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <GL/glut.h>

#include "shared.h"
#include "util.h"
#include "input.h"
#include "light.h"
#include "portal.h"
#include "wall.h"

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

wall walls[4];

wall wall_front = { {8, 2, 0}, {-1, 0, 0}, 16, 4, &material_concrete_green };
wall wall_back = { {-8, 2, 0}, {1, 0, 0}, 16, 4, &material_concrete_red };
wall wall_left = { {0, 2, -8}, {0, 0, 1}, 16, 4, &material_concrete_yellow };
wall wall_right = { {0, 2, 8}, {0, 0, -1}, 16, 4, &material_concrete_blue };

// Lights
GLfloat main_light_position[] = { 0, 5, 0, 1 };

// GLUT Event Handlers
static void on_display(void);
static void on_reshape(int width, int height);
static void on_mouse_click(int button, int state, int m_x, int m_y);
static void on_timer(int data);

portal portals[4];

portal portal1 = { { 8, 1, 0 }, { -1, 0, 0 }, 6, 4, NULL };
portal portal2 = { { 0, 1, -8 }, { 0, 0, 1 }, 6, 4, NULL };
portal portal3 = { { 0, 1, 8 }, { 0, 0, -1 }, 6, 4, NULL };
portal portal4 = { { 6, 1, 6 }, { -1, 0, -1 }, 3, 4, NULL };

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

  glutSetCursor(GLUT_CURSOR_NONE);
  glutWarpPointer(window_width/2, window_height/2);

  glEnable(GL_DEPTH_TEST);
  glEnable(GL_CULL_FACE);
  glEnable(GL_LIGHTING);
  glEnable(GL_NORMALIZE);

  glClearColor(0, 0, 0, 0);

  glutTimerFunc(20, on_timer, 0);

  // Initialize walls array
  walls[0] = wall_front;
  walls[1] = wall_back;
  walls[2] = wall_left;
  walls[3] = wall_right;

  // Initialize portals array
  portals[0] = portal1;
  portals[1] = portal2;
  portals[2] = portal3;
  portals[3] = portal4;
  normalize3v(portals[3].normal);

  // Link portals to show each other's view
  (void)link_portals(&portals[0], &portals[3]);
  (void)link_portals(&portals[1], &portals[2]);

  // (void)link_portals(&portals[0], &portals[1]);

  glutMainLoop();

  return 0;
}

void update_camera() {
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
  for(unsigned int i = 0; i < sizeof(portals)/sizeof(portal); i++) {
    // If the portal is linked and we passed the portal plane
    if(is_linked(&portals[i]) && sidexz3v(portals[i].position, portals[i].normal, x, z)*sidexz3v(portals[i].position, portals[i].normal, new_x, new_z) <= 0) {

      float d = det2f(new_x-x, new_z-z, -portals[i].normal[2]*portals[i].width/2, portals[i].normal[0]*portals[i].width/2);

      // Calculate the intersection parameter on the portal
      float s = det2f(portals[i].position[0]-x, portals[i].position[2]-z, new_x-x, new_z-z)/d;

      // Check if the player went through the portal, otherwise continue
      if(fabs(s) >= 1)
        continue;

      // Calculate the intersection parameter on the player move vector
      float t = det2f(portals[i].position[0]-x, portals[i].position[2]-z, -portals[i].normal[2]*portals[i].width/2, portals[i].normal[0]*portals[i].width/2)/d;

      // Calculate the yaw change
      float angle = angle_between2f(-portals[i].normal[0], -portals[i].normal[2], portals[i].link->normal[0], portals[i].link->normal[2]);

      // Move the player
      float offset_x = (new_x-x)*(1-t);
      float offset_z = (new_z-z)*(1-t);

      new_x = portals[i].link->position[0] + portals[i].link->normal[2]*portals[i].link->width/2*s;
      new_z = portals[i].link->position[2] - portals[i].link->normal[0]*portals[i].link->width/2*s;

      new_x += cos(angle)*offset_x - sin(angle)*offset_z;
      new_z += sin(angle)*offset_x + cos(angle)*offset_z;

      // Update player rotation
      yaw = clamp_yaw(yaw + angle);

      break;
    }
  }

  x = new_x;
  z = new_z;

  update_camera();

  // Check collisions
  float dist = 0;
  // Is the player hitting a wall
  for(unsigned int i = 0; i < sizeof(walls)/sizeof(wall); i++) {
    if(is_colliding_with_wall(x, z, &walls[i], &dist)) {
      // And is there not a portal there
      // TODO attach portals to walls so we don't have to check collisions with all of them
      bool in_portal = false;
      for(unsigned int j = 0; j < sizeof(portals)/sizeof(portal); j++) {
        if(is_linked(&portals[j]) && is_colliding_with_portal(x, y, z, &portals[j])) {
          in_portal = true;
          break;
        }
      }
      if(!in_portal) {
        // Move the player back
        x = x + (PLAYER_RADIUS-dist)*walls[i].normal[0];
        z = z + (PLAYER_RADIUS-dist)*walls[i].normal[2];
      }
    }
  }

  glutPostRedisplay();

  glutTimerFunc(20, on_timer, 0);
}

static void on_mouse_click(int button, int state, int m_x, int m_y) {
  (void) button;
  (void) state;
  (void) m_x;
  (void) m_y;
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
  for(unsigned int i = 0; i < sizeof(walls)/sizeof(wall); i++) {
    draw_wall(&walls[i]);
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

void draw_scene() {
  portal p;
  for(unsigned int i = 0; i < sizeof(portals)/sizeof(portal); i++) {
    p = portals[i];

    // If the portal is linked draw the link's view
    // Otherwise only draw the frame
    if(is_linked(&p)) {
      glEnable(GL_STENCIL_TEST);

      // Create a mask for the portal in the stencil buffer
      // Disable drawing to the color and depth buffer
      glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
      glDepthMask(GL_FALSE);
      glDisable(GL_DEPTH_TEST);

      // Fail the stencil test on every drawn pixel
      // Increment the stencil buffer value for every failed test
      glStencilFunc(GL_NEVER, 0, 0xFF);
      glStencilOp(GL_INCR, GL_KEEP, GL_KEEP);
      // Alternatively always pass the stencil test and increment on success
      // glStencilFunc(GL_ALWAYS, 0, 0xFF);
      // glStencilOp(GL_KEEP, GL_KEEP, GL_INCREMENT);

      // Clearing the stencil buffer depends on the mask value
      glStencilMask(0xFF);
      glClear(GL_STENCIL_BUFFER_BIT);

      // Draw the portal frame in the stencil buffer
      draw_portal_frame(&p);

      // Enable drawing to the color and depth buffers only where the portal was
      glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
      glDepthMask(GL_TRUE);
      glEnable(GL_DEPTH_TEST);

      // Disable drawing to the stencil buffer
      glStencilMask(0x00);

      // Draw where the stencil value is 1
      glStencilFunc(GL_EQUAL, 1, 0xFF);

      // Render a flipped mirror perspective from the portal
      glMatrixMode(GL_MODELVIEW);
      glPushMatrix();

        // Add a clipping plane so we don't render objects behind the destination portal
        double clip_plane[] = {
          -p.normal[0], -p.normal[1], -p.normal[2],
          p.normal[0]*p.position[0]*0.99f + p.normal[1]*p.position[1]*0.99f + p.normal[2]*p.position[2]*0.99f
        };
        glClipPlane(GL_CLIP_PLANE0, clip_plane);
        glEnable(GL_CLIP_PLANE0);

        // Flip the camera
        glRotatef(180, 0, 1, 0);
        // Find the transformation between the portals
        // We multiply by 0.99 so it doesn't look disconnected
        // because the portal is offset from the wall a few pixels
        glTranslatef(-0.99f*p.position[0], p.position[1], -0.99f*p.position[2]);
        glRotatef(angle_betweenxz3v(p.normal, p.link->normal), 0, 1, 0);
        glTranslatef(-0.99f*p.link->position[0], -p.link->position[1], -0.99f*p.link->position[2]);

        draw_world();

        glDisable(GL_CLIP_PLANE0);

      glPopMatrix();

      glDisable(GL_STENCIL_TEST);
    } else {
      draw_portal_frame(&p);
    }
  }

  // Run a depth test to find objects closer to the camera than the portals
	glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);
	glClear(GL_DEPTH_BUFFER_BIT);

  for(unsigned int i = 0; i < sizeof(portals)/sizeof(portal); i++) {
    draw_portal_frame(&portals[i]);
  }

  // Render the world from the players perspective
  glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);

	draw_world();
}

static void on_display(void) {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  gluLookAt(x, y, z, x + look_x, y + look_y, z + look_z, 0.0f, 1.0f, 0.0f);

  draw_scene();

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
