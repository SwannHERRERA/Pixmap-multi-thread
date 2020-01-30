//
// Created by MaleWhere on 30/01/2020.
//

#include "ppm_image.h"

bool pixel_equals(const pixel_t *self, const pixel_t *other) {
    return self->green == other->green && self->red == other->red && self->blue == other->blue;
}

bool pixel_equals_flex(const pixel_t *p, const pixel_t *pbis, int accuracy) {
    return abs(p->red - pbis->red) <= accuracy &&
           abs(p->green - pbis->green) < accuracy &&
           abs(p->blue - pbis->blue) < accuracy;
}

uint8_t pixel_blue(const pixel_t *p) {
    return p->blue;
}

uint8_t pixel_red(const pixel_t *p) {
    return p->red;
}

uint8_t pixel_green(const pixel_t *p) {
    return p->green;
}

int ppm_image_t_width(const ppm_image_t *ppmImage) {
    return ppmImage->width;
}

int ppm_image_t_height(const ppm_image_t *ppmImage) {
    return ppmImage->height;
}

size_t ppm_image_t_lenght(const ppm_image_t *ppmImage) {
    return ppmImage->lenght;
}

size_t ppm_black_pixels(const ppm_image_t *img) {
    pixel_t blackPixel = pixel_new(0, 0, 0);
    size_t nb_black_pixels = 0;
    for (int i = 0; i < img->lenght; ++i) {
        if (pixel_equals(&img->data[i], &blackPixel))
            nb_black_pixels++;
    }
    return nb_black_pixels;
}

size_t ppm_black_pixels_flex(const ppm_image_t *img, int accuracy) {
    pixel_t blackPixel = pixel_new(0, 0, 0);
    size_t nb_black_pixels = 0;
    for (int i = 0; i < img->lenght; ++i) {
        if (pixel_equals_flex(&img->data[i], &blackPixel, accuracy))
            nb_black_pixels++;
    }
    return nb_black_pixels;
}

pixel_t pixel_new(uint8_t red, uint8_t green, uint8_t blue) {
    pixel_t pixel = {red, green, blue};

    return pixel;
}

pixel_t *ppm_image_t_data(const ppm_image_t *ppmImage) {
    return ppmImage->data;
}

pixel_t ppm_pixel(const ppm_image_t *img, size_t x, size_t y) {
    if (x >= img->height || y >= img->width) {
        fprintf(stderr, "Error! x,y out of range!\n");
        exit(1);
    }

    return img->data[x + img->width * y];
}

pixel_t pixel_invert(const pixel_t *p) {
    pixel_t pixel = {~p->red, ~p->green, ~p->blue};
    return pixel;
}

ppm_image_t *ppm_malloc(const char *pathname) {
    char buff[16];
    ppm_image_t *img;
    FILE *fp;

    fp = fopen(pathname, "rb");
    if (!fp) {
        fprintf(stderr, "Can't open file: %s\n", pathname);
        exit(1);
    }

    fscanf(fp, "%c%c", buff, buff + 1);
    if (buff[0] != 'P' || buff[1] != '6') {
        fprintf(stderr, "Wrong format !\n");
        exit(1);
    }

    img = malloc(sizeof(ppm_image_t));
    getc(fp);

    fscanf(fp, "%d %d", &img->height, &img->width);
    img->lenght = img->height * img->width;

    getc(fp);

    while (getc(fp) != '\n');

    img->data = malloc(img->height * img->width * sizeof(pixel_t));

    if (fread(img->data, sizeof(pixel_t), img->height * img->width, fp) != img->height * img->width) {
        fprintf(stderr, "Error while reading image\n");
        exit(1);
    }

    return img;
}

void ppm_image_save(const char *pathname, const ppm_image_t *img) {
    char *newPath = malloc((strlen(pathname) + 5) * sizeof(char));
    char bufferH[5], bufferW[5];
    strcpy(newPath, "copy_");
    strcat(newPath, pathname);

    FILE *fp = fopen(newPath, "wb");
    if (!fp) {
        fprintf(stderr, "Can't create file: %s\n", newPath);
        exit(1);
    }

    sprintf(bufferH, "%d", img->height);
    sprintf(bufferW, "%d", img->width);

    if (fprintf(fp, "P6\n%d %d\n255\n", img->height, img->width) != 9 + strlen(bufferH) + strlen(bufferW)) {
        fprintf(stderr, "Error while writing header to file\n");
        exit(1);
    }
    if (fwrite(img->data, sizeof(pixel_t), img->lenght, fp) != img->lenght) {
        fprintf(stderr, "Error while writing pixels to file\n");
        exit(1);
    }

    fclose(fp);
}

void ppm_negatif(ppm_image_t *img) {
    int i;
    pixel_t pixel;

    for (i = 0; i < img->lenght; ++i) {
        pixel = pixel_invert(img->data + i);
        img->data[i].red = pixel.red;
        img->data[i].green = pixel.green;
        img->data[i].blue = pixel.blue;
    }
}
