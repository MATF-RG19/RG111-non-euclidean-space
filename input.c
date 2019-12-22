#include "input.h"

int mouse_x = 0;
int mouse_y = 0;

int mouse_dx = 0;
int mouse_dy = 0;

bool is_forward_pressed = false;
bool is_backward_pressed = false;
bool is_left_pressed = false;
bool is_right_pressed = false;

bool was_reset_pressed = false;

void flush_keyboard() {
  was_reset_pressed = false;
}

void on_keyboard(unsigned char key, int m_x, int m_y) {
  (void) m_x;
  (void) m_y;
  switch(key) {
    case 27:
      exit(0);
      break;
    case 'w':
    case 'W':
      is_forward_pressed = true;
      break;
    case 's':
    case 'S':
      is_backward_pressed = true;
      break;
    case 'a':
    case 'A':
      is_left_pressed = true;
      break;
    case 'd':
    case 'D':
      is_right_pressed = true;
      break;
    case 'r':
    case 'R':
      was_reset_pressed = true;
      break;
  }
}

void on_keyboard_up(unsigned char key, int m_x, int m_y) {
  (void)m_x;
  (void)m_y;
  switch(key) {
    case 'w':
    case 'W':
      is_forward_pressed = false;
      break;
    case 's':
    case 'S':
      is_backward_pressed = false;
      break;
    case 'a':
    case 'A':
      is_left_pressed = false;
      break;
    case 'd':
    case 'D':
      is_right_pressed = false;
      break;
  }
}

void on_mouse_move(int m_x, int m_y) {
  static bool warped = false;

  if(warped) {
    warped = false;
    return;
  }

  mouse_dx += m_x - window_width/2;
  mouse_dy += m_y - window_height/2;

  glutWarpPointer(window_width/2, window_height/2);
  warped = true;
}

void update_mouse() {
  mouse_x += mouse_dx;
  mouse_y += mouse_dy;

  mouse_dx = 0;
  mouse_dy = 0;
}
