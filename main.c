#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <GL/glut.h>

#include "shared.h"
#include "util.h"
#include "input.h"
#include "light.h"
#include "portal.h"
#include "wall.h"
#include "logic.h"
#include "bitmaps.h"
#include "render.h"
#include "level.h"

// Player Position
static double x = 0;
static double y = 1.0f;
static double z = 0;

static double new_x = 0;
static double new_y = 0;
static double new_z = 0;

// Camera Rotation
static double yaw = 0;
static double pitch = 0;

// Look Vector
static double look_x = 1;
static double look_y = 0;
static double look_z = 0;

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

  // Take over the cursor
  glutSetCursor(GLUT_CURSOR_NONE);
  glutWarpPointer(window_width/2, window_height/2);

  glEnable(GL_DEPTH_TEST);
  glEnable(GL_CULL_FACE);
  glEnable(GL_LIGHTING);
  glEnable(GL_NORMALIZE);
  glEnable(GL_TEXTURE_2D);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

  // Load textures
  load_textures();

  // Set the pixel storage mode for bitmaps
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

  glClearColor(0, 0, 0, 0);

  // Initialize walls array
  initialize_walls(wall_allocation_size);

  // Initialize portals array
  initialize_portals(portal_allocation_size);

  // Load the level and set the player's position
  load_level(&x, &y, &z);

  // Start main timer
  glutTimerFunc(20, on_timer, 0);

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

  static bool should_rerender = false;

  if(mouse_dx != 0 || mouse_dy != 0)
    should_rerender = true;

  yaw = clamp_yaw(yaw + mouse_dx * MOUSE_SENSITIVITY);
  pitch = clamp_pitch(pitch + mouse_dy * MOUSE_SENSITIVITY);

  update_mouse();

  new_x = x;
  new_y = y;
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

  // Apply gravity
  if(new_y > PLAYER_HEIGHT) {
    new_y = new_y - PLAYER_SPEED < PLAYER_HEIGHT ? PLAYER_HEIGHT : new_y - PLAYER_SPEED;
    y = new_y;
    should_rerender = true;
  }

  // Close user portals
  if(was_reset_pressed) {
    free_user_portal(BLUE);
    free_user_portal(ORANGE);
    should_rerender = true;
  }

  // Reset keyboard state
  flush_keyboard();

  // Only calculate collisions if the player has moved
  if(x != new_x || z != new_z) {
    should_rerender = true;

    player_animatation = true;
    player_animation_param += 0.001;
    if(player_animation_param > 1)
      player_animation_param -= 1;

    check_teleportation(&x, &y, &z, new_x, new_y, new_z, &yaw);
    check_collisions(&x, &y, &z);
  } else {
    if(player_animatation)
      should_rerender = true;

    player_animatation = false;
    player_animation_param = 0;
  }

  update_camera();

  if(should_rerender) {
    should_rerender = false;
    glutPostRedisplay();
  }

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
  if(button == 0) {
    free_user_portal(BLUE);
    place_portal(x, y, z, look_x, look_y, look_z, BLUE);
  } else if(button == 2) {
    free_user_portal(ORANGE);
    place_portal(x, y, z, look_x, look_y, look_z, ORANGE);
  }

  glutPostRedisplay();
}

void draw_scene(int level, double px, double pz, double lx, double lz) {
  // Draw user portal borders
  for(unsigned int i = 0; i < portal_count; i++) {
    if(portals[i] == NULL)
      continue;

    if(i == BLUE || i == ORANGE)
      draw_portal_frame(portals[i], i);
  }

  // Draw portal views
  portal p;
  for(unsigned int i = 0; i < portal_count; i++) {
    if(portals[i] == NULL)
      continue;

    p = *portals[i];

    // Optimizations
    // Don't render portal if it's behind current position (based on looking direction)
    // Don't render portal if the current position is behind it (based on looking direction)
    // Don't render portal if the current looking direction is away from it
    // Note: For level 0 the current position and location is based on the player
    // For other levels its based on the latest portal we're drawing in
    if(
      p.normal[0]*px + p.normal[2]*pz - p.normal[0]*p.position[0] - p.normal[2]*p.position[2] < 0 ||
      lx*p.position[0] + lz*p.position[2] - lx*px - lz*pz < -p.width/2 ||
      dot_prod2f(lx, lz, p.normal[0], p.normal[2])>=0.7f
    ) {
      continue;
    }

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
    if(i == BLUE || i == ORANGE)
      draw_user_portal_frame(&p, i);
    else
      draw_portal_frame(&p, i);

    // Enable drawing to the color and depth buffers only where the portal was
    glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
    glDepthMask(GL_TRUE);
    glClear(GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);

    // Disable drawing to the stencil buffer
    glStencilMask(0x00);

    // Draw where the stencil value is equal to the current level + 1
    glStencilFunc(GL_EQUAL, level + 1, 0xFF);

    // If the portal is linked draw the link's view
    // Otherwise only draw the frame
    if(is_linked(&p)) {
      glPushMatrix();

        // Add a clipping plane so we don't render objects behind the destination portal
        if(level<GL_MAX_CLIP_PLANES) {
          double clip_plane[] = {
            -p.normal[0], -p.normal[1], -p.normal[2],
            p.normal[0]*p.position[0] + p.normal[1]*p.position[1] + p.normal[2]*p.position[2]
          };

          // Note GL_CLIP_PLANEi = GL_CLIP_PLANE0 + i
          glClipPlane(GL_CLIP_PLANE0 + level, clip_plane);
          glEnable(GL_CLIP_PLANE0 + level);
        }

        // Flip the camera
        glRotatef(180, 0, 1, 0);
        // Find the transformation between the portals
        // We offset by 0.005 in the direction of the normal vector so it doesn't look disconnected
        // because the portal is offset from the wall a few pixels
        glTranslatef(-p.position[0]-0.005f*p.normal[0], p.position[1], -p.position[2]-0.005f*p.normal[2]);
        glRotatef(angle_betweenxz3v(p.normal, p.link->normal), 0, 1, 0);
        glTranslatef(-p.link->position[0]-0.005f*p.link->normal[0], -p.link->position[1], -p.link->position[2]-0.005f*p.link->normal[2]);

        if(level == MAX_RECURSION_LEVEL) {
          // Draw the view without portals
          draw_world(x, y, z, yaw, pitch);
          if(DRAW_PLAYER) {
            glPushMatrix();
              glTranslatef(x, y-1, z);
              draw_player(yaw, pitch);
            glPopMatrix();
          }
        } else {
          // Draw the view recursively from the current portal
          draw_scene(level + 1, p.link->position[0], p.link->position[2], p.link->normal[0], p.link->normal[2]);
        }

        if(level<GL_MAX_CLIP_PLANES) {
          glDisable(GL_CLIP_PLANE0+level);
        }

      glPopMatrix();
    } else {
      draw_portal_frame(&p, i);
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
    if(i == BLUE || i == ORANGE)
      draw_user_portal_frame(&p, i);
    else
      draw_portal_frame(&p, i);

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

    draw_portal_frame(portals[i], i);
  }

  // Only draw at the current level
  glEnable(GL_STENCIL_TEST);
  glStencilMask(0x00);
  glStencilFunc(GL_EQUAL, level, 0xFF);

  // Render the world from the players perspective
  glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);

  draw_world(x, y, z, yaw, pitch);

  // Draw the player on all levels except 0 (the actual world)
  if(DRAW_PLAYER && level != 0) {
    glPushMatrix();
      glTranslatef(x, y-1, z);
      draw_player(yaw, pitch);
    glPopMatrix();
  }

  glDisable(GL_STENCIL_TEST);
}

static void draw_hud() {
  glDisable(GL_LIGHTING);

  glMatrixMode(GL_PROJECTION);
  glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0, window_width, 0, window_height);

    // Trick for bitmaps not to disappear when a wall is too close to the player
    glTranslatef(0, 0, 1.0f);

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    	glLoadIdentity();

      glColor3f(0.8f, 0.8f, 0.8f);
    	glRasterPos2i(window_width/2 - 8, window_height/2 - 8);
    	glBitmap(16, 16, 0, 0, 0, 0, crosshair);

      glColor3f(0.1f, 0.1f, 0.6f);
      glRasterPos2i(window_width/2 - 28, window_height/2 - 24);
      if(portals[BLUE] == NULL)
        glBitmap(24, 48, 0, 0, 0, 0, indicator_left_off);
      else
        glBitmap(24, 48, 0, 0, 0, 0, indicator_left_on);

      glColor3f(0.6f, 0.2f, 0.1f);
      glRasterPos2i(window_width/2 + 4, window_height/2 - 24);
      if(portals[ORANGE] == NULL)
        glBitmap(24, 48, 0, 0, 0, 0, indicator_right_off);
      else
        glBitmap(24, 48, 0, 0, 0, 0, indicator_right_on);

		glPopMatrix();
  glMatrixMode(GL_PROJECTION);
	glPopMatrix();

  glEnable(GL_LIGHTING);
}

static void on_display(void) {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  gluLookAt(x, y, z, x + look_x, y + look_y, z + look_z, 0.0f, 1.0f, 0.0f);

  draw_scene(0, x, z, look_x, look_z);

  glClear(GL_DEPTH_BUFFER_BIT);

  glPushMatrix();
    glTranslatef(x, y, z);
    glRotatef(-yaw, 0, 1, 0);
    glRotatef(-pitch, 0, 0, 1);
    glTranslatef(0.05f, -0.05f, 0.1f);
    glScalef(0.3f, 0.3f, 0.3f);
    draw_portal_gun();
  glPopMatrix();

  draw_hud();

  glutSwapBuffers();
}

static void on_reshape(int width, int height) {
  window_width = width;
  window_height = height;

  glViewport(0, 0, (GLsizei)width, (GLsizei)height);

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective((GLdouble)60, (GLdouble)width / (GLdouble)height, (GLdouble)0.01, (GLdouble)100.0);

  glutPostRedisplay();
}

static void on_close(void) {
  free_textures();
  free_portals();
  free_walls();
}
