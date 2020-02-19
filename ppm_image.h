#ifndef PIXMAP_MULTI_THREAD_PPM_IMAGE_H
#define PIXMAP_MULTI_THREAD_PPM_IMAGE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include <pthread.h>
#include <assert.h>

// Structure representing a pixel
// uint8_t because the intensity goes from 0 to 255 : 8 bits
typedef struct {
    uint8_t red;
    uint8_t green;
    uint8_t blue;
} pixel_t;

// Structure representing the image
typedef struct {
    size_t height, width;
    size_t totalPixels; // biggest type to store the unknown and possibly big total number
    pixel_t *data; // dynamic array of pixels
} ppm_image_t;

// Structure for the multithreaded count of black pixels
typedef struct pixels_count {
    size_t count_T1;
    size_t count_T2;
    size_t flex_count_T1;
    size_t flex_count_T2;
} pixels_count;

// Structure to pass the arguments to the threads
typedef struct {
    ppm_image_t *img;
    pixels_count *black_pixels;
} thread_args;

// Signatures
bool pixel_equals(const pixel_t *self, const pixel_t *other);

bool pixel_equals_flex(const pixel_t *p, const pixel_t *pbis, int accuracy);

uint8_t pixel_blue(const pixel_t *p);

uint8_t pixel_red(const pixel_t *p);

uint8_t pixel_green(const pixel_t *p);

int ppm_image_t_width(const ppm_image_t *ppmImage);

int ppm_image_t_height(const ppm_image_t *ppmImage);

size_t ppm_image_t_totalPixels(const ppm_image_t *ppmImage);

size_t ppm_black_pixels(const ppm_image_t *img);

size_t ppm_black_pixels_flex(const ppm_image_t *img, int accuracy);

void *ppm_black_pixels_T1(void *arg);

void *ppm_black_pixels_T2(void *arg);

pixel_t pixel_new(uint8_t red, uint8_t green, uint8_t blue);

pixel_t *ppm_image_t_data(const ppm_image_t *ppmImage);

pixel_t ppm_pixel(const ppm_image_t *img, size_t x, size_t y);

pixel_t pixel_invert(const pixel_t *p);

ppm_image_t *ppm_malloc(const char *pathname);

void ppm_image_save(const char *pathname, const ppm_image_t *img);

void ppm_negative(ppm_image_t *img);

void ppm_destroy(ppm_image_t *ppmImage);

#endif //PIXMAP_MULTI_THREAD_PPM_IMAGE_H
