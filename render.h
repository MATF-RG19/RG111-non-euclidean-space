#ifndef RENDER_H_
#define RENDER_H_

#include <stdbool.h>
#include <math.h>

#include "shared.h"
#include "image.h"

#define TEXTURE_PATH_WALL_DARK "resources/wall_dark.bmp"
#define TEXTURE_PATH_WALL_LIGHT "resources/wall_light.bmp"
#define TEXTURE_PATH_HEART "resources/heart.bmp"

#define TEXTURE_WALL_DARK 0
#define TEXTURE_WALL_LIGHT 1
#define TEXTURE_HEART 2

#define NUM_TEXTURES 3

extern GLuint textures[NUM_TEXTURES];

extern bool player_animatation;
extern double player_animation_param;

extern void load_textures();
extern void free_textures();

extern void draw_player(double yaw, double pitch);
extern void draw_portal_gun();
extern void draw_companion_cube();
extern void draw_cake();

#endif
