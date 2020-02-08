#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "image.h"

image *init_image() {
  image *img;

  // Allocate space for the image
  img = (image *)malloc(sizeof(image));
  assert(img != NULL);

  return img;
}

void free_image(image *img) {
  free(img->pixels);
  free(img);
}

/*
 *  pack_alignment is the GL_UNPACK_ALIGNMENT value while drawing the image
 *  The GL_UNPACK_ALIGNMENT value is set using the glPixelStorei function
 *  and can be retrieved using glGetIntegerv(GL_UNPACK_ALIGNMENT, &value)
 *  if you aren't changing this value, the default is 4
 */
void read_image(image *img, char *filename, int pack_alignment) {
  FILE *file;
  BITMAP_FILE_HEADER bfh;
  BITMAP_DIB_HEADER bdh;
  unsigned char r, g, b, a;

  if(pack_alignment != 1 && pack_alignment != 2 && pack_alignment != 4 && pack_alignment != 8) {
    fprintf(stderr, "Invalid alignment: %d\n", pack_alignment);
    exit(EXIT_FAILURE);
  }

  // Free previous pixel data
  free(img->pixels);
  img->pixels = NULL;

  // Open the image for reading
  file = fopen(filename, "rb");
  assert(file != NULL);

  // Read the file header
  fread(&bfh.type, 2, 1, file);
  assert(bfh.type == 0x4d42); // Make sure the format is correct
  fread(&bfh.size, 4, 1, file);
  fread(&bfh.reserved1, 2, 1, file);
  fread(&bfh.reserved2, 2, 1, file);
  fread(&bfh.offset, 4, 1, file);

  // Read the DIB header
  fread(&bdh.size, 4, 1, file);
  fread(&bdh.width, 4, 1, file);
  fread(&bdh.height, 4, 1, file);
  fread(&bdh.planes, 2, 1, file);
  fread(&bdh.colordepth, 2, 1, file);
  fread(&bdh.compression, 4, 1, file);
  fread(&bdh.sizeimage, 4, 1, file);
  fread(&bdh.xppm, 4, 1, file);
  fread(&bdh.yppm, 4, 1, file);
  fread(&bdh.colors, 4, 1, file);
  fread(&bdh.colorsimportant, 4, 1, file);

  // Seek to the beginning of the pixel data, pointed to by bfh.offset
  fseek(file, bfh.offset, SEEK_SET);

  // Initialize the width and height
  img->width = bdh.width;
  img->height = bdh.height;

  // Each scanline in a bitmap is aligned to the next address divisible by 4
  // So we need to skip those bytes
  unsigned int padding = (4 - (bdh.width * (bdh.colordepth / 8) % 4)) % 4;

  // OpenGL can specify the alignment requirements for the start of each pixel row in memory
  // This is done by setting GL_UNPACK_ALIGNMENT using the glPixelStorei function
  // Allowed values are 1, 2, 4 and 8 (default is 4)
  unsigned int alignment = (pack_alignment - (bdh.width * (bdh.colordepth / 8) % pack_alignment)) % pack_alignment;

  // Read the pixel array
  if (bdh.colordepth == 24) {
    img->pixels = (char *)malloc(3 * bdh.width * bdh.height * sizeof(char) + alignment * bdh.height);
  } else if (bdh.colordepth == 32) {
    padding = 0;
    img->pixels = (char *)malloc(4 * bdh.width * bdh.height * sizeof(char) + alignment * bdh.height);
  } else {
    fprintf(stderr, "Unsupported bitmap type. Only 24 or 32 bits per pixel allowed\n");
    exit(EXIT_FAILURE);
  }
  assert(img->pixels != NULL);

  unsigned int i, j;
  unsigned int index = 0;
  if (bdh.colordepth == 24) {
    for (i = 0; i < bdh.height; i++) {
      for (j = 0; j < bdh.width; j++) {
        // Read the colors in order: B, G, R
        fread(&b, sizeof(char), 1, file);
        fread(&g, sizeof(char), 1, file);
        fread(&r, sizeof(char), 1, file);

        img->pixels[index + 0] = r;
        img->pixels[index + 1] = g;
        img->pixels[index + 2] = b;
        index += 3;
      }
      fseek(file, padding, SEEK_CUR);
      for(j = 0; j < alignment; j++) {
        img->pixels[index + j] = 0;
      }
      index += alignment;
    }
  } else if (bdh.colordepth == 32) {
    for (i = 0; i < bdh.height; i++) {
      for (j = 0; j < bdh.width; j++) {
        // Read the colors in order: B, G, R, A
        fread(&b, sizeof(char), 1, file);
        fread(&g, sizeof(char), 1, file);
        fread(&r, sizeof(char), 1, file);
        fread(&a, sizeof(char), 1, file);

        img->pixels[index + 0] = r;
        img->pixels[index + 1] = g;
        img->pixels[index + 2] = b;
        img->pixels[index + 3] = a;
        index += 4;
      }
      for(j = 0; j < alignment; j++) {
        img->pixels[index + j] = 0;
      }
      index += alignment;
    }
  }

  fclose(file);
}
