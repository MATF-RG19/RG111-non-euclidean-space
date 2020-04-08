#ifndef RENDER_H_
#define RENDER_H_

#include <math.h>

#include "shared.h"
#include "image.h"

#define TEXTURE_WALL_DARK "resources/wall_dark.bmp"
#define TEXTURE_WALL_LIGHT "resources/wall_light.bmp"
#define TEXTURE_HEART "resources/heart.bmp"

#define NUM_TEXTURES 3

extern GLuint textures[NUM_TEXTURES];

extern void load_textures();
extern void free_textures();

extern void draw_companion_cube();
extern void draw_cake();

#endif
