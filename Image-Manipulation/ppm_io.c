
#include <assert.h>
#include "ppm_io.h"
#include <stdlib.h>
#define RGB_COMPONENT_COLOR 255

/* Read a PPM-formatted image from a file (assumes fp != NULL).
 * Returns the address of the heap-allocated Image struct it
 * creates and populates with the Image data.
 */

Image * read_ppm(FILE *fp) {

    char buff[46];
    Image *img;

    int c, rgb_comp_color;
    //open PPM file for reading

    if (!fp) {

        exit(1);
    }

    //read image format
    if (!fgets(buff, sizeof(buff), fp)) {
        exit(1);
    }

    //check the image format
    if (buff[0] != 'P' || buff[1] != '6') {
        fprintf(stderr, "Invalid image format (must be 'P6')\n");
        exit(1);
    }

    //alloc memory form image
    img = (Image *)malloc(sizeof(Image));
    if (!img) {
        fprintf(stderr, "Unable to allocate memory\n");
        exit(1);
    }

    //check for comments
    c = getc(fp);
    while (c == '#') {
        while (getc(fp) != '\n') ;
        c = getc(fp);
    }

    ungetc(c, fp);
    //read image size information
    if (fscanf(fp, "%d %d", &img->cols, &img->rows) != 2) {
        exit(1);
    }

    //read rgb component
    if (fscanf(fp, "%d", &rgb_comp_color) != 1) {
        exit(1);
    }

    //check rgb component depth
    if (rgb_comp_color!= RGB_COMPONENT_COLOR) {
        exit(1);
    }

    while (fgetc(fp) != '\n') ;
    //memory allocation for pixel data
    img->data = (Pixel*)malloc(img->cols * img->rows * sizeof(Pixel));

    if (!img) {
        fprintf(stderr, "Unable to allocate memory\n");
        exit(1);
    }
    unsigned int nrElems=img->cols * img->rows;
    //read pixel data from file
    if (fread(img->data, 3*sizeof(char), img->cols*img->rows, fp) != nrElems) {
        exit(1);
    }

    fclose(fp);
    return img;
  
}

Pixel *getPixel(Image *img, int x, int y)
{
    return &img->data[img->cols*x+y];
}
void setPixel(Image *img,int x, int y, Pixel pix)
{
    img->data[img->cols*x+y]=pix;
}
/* Write a PPM-formatted image to a file (assumes fp != NULL),
 * and return the number of pixels successfully written.
 */
int write_ppm(FILE *fp, const Image *im) {

  // check that fp is not NULL
  assert(fp); 

  // write PPM file header, in the following format
  // P6
  // cols rows
  // 255
  fprintf(fp, "P6\n%d %d\n255\n", im->cols, im->rows);
unsigned int nrElems=im->cols * im->rows;
  // now write the pixel array
  int num_pixels_written = fwrite(im->data, sizeof(Pixel), nrElems, fp);

  if (num_pixels_written != im->cols * im->rows) {
    fprintf(stderr, "Uh oh. Pixel data failed to write properly!\n");
  }

  return num_pixels_written;
}

