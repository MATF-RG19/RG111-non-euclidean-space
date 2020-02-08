#ifndef IMAGE_H_
#define IMAGE_H_

typedef struct BITMAP_FILE_HEADER {
  unsigned short type;      // used to identify the BMP and DIB file
  unsigned int size;        // size of the BMP file in bytes
  unsigned short reserved1; // reserved; actual value depends on the application that creates the image
  unsigned short reserved2; // reserved; actual value depends on the application that creates the image
  unsigned int offset;      // starting address of the byte where the bitmap image data (pixel array) can be found
} BITMAP_FILE_HEADER;

typedef struct BITMAP_DIB_HEADER {
  unsigned int size;            // size of this header
  unsigned int width;           // bitmap width in pixels
  unsigned int height;          // bitmap height in pixels
  unsigned short planes;        // number of color planes
  unsigned short colordepth;    // number of bits per pixel
  unsigned int compression;     // compression method being used
  unsigned int sizeimage;       // image size
  int xppm;                     // horizontal resolution of the image
  int yppm;                     // vertical resolution of the image
  unsigned int colors;          // number of colors in the color palette
  unsigned int colorsimportant; // number of important colors used
} BITMAP_DIB_HEADER;

typedef struct image {
  int width, height;
  char *pixels;
} image;

image *init_image();

void free_image(image *img);

void read_image(image *img, char *filename, int pack_alignment);

#endif
