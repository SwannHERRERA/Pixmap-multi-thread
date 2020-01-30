//
// Created by MaleWhere on 30/01/2020.
//

#ifndef PIXMAP_MULTI_THREAD_PPM_IMAGE_H
#define PIXMAP_MULTI_THREAD_PPM_IMAGE_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>


typedef struct {
    uint8_t red;
    uint8_t green;
    uint8_t blue;
} pixel_t;

typedef struct {
    int height, width;
    size_t lenght;
    pixel_t *data;
} ppm_image_t;

bool pixel_equals(const pixel_t *self, const pixel_t *other);

bool pixel_equals_flex(const pixel_t *p, const pixel_t *pbis, int accuracy);

uint8_t pixel_blue(const pixel_t *p);

uint8_t pixel_red(const pixel_t *p);

uint8_t pixel_green(const pixel_t *p);

int ppm_image_t_width(const ppm_image_t *ppmImage);

int ppm_image_t_height(const ppm_image_t *ppmImage);

size_t ppm_image_t_lenght(const ppm_image_t *ppmImage);

size_t ppm_black_pixels(const ppm_image_t *img);

size_t ppm_black_pixels_flex(const ppm_image_t *img, int accuracy);

pixel_t pixel_new(uint8_t red, uint8_t green, uint8_t blue);

pixel_t *ppm_image_t_data(const ppm_image_t *ppmImage);

pixel_t ppm_pixel(const ppm_image_t *img, size_t x, size_t y);

pixel_t pixel_invert(const pixel_t *p);

ppm_image_t *ppm_malloc(const char *pathname);

void ppm_image_save(const char *pathname, const ppm_image_t *img);

void ppm_negatif(ppm_image_t *img);

#endif //PIXMAP_MULTI_THREAD_PPM_IMAGE_H
