//
// Created by MaleWhere on 22/01/2020.
//

#ifndef PIXMAP_MULTI_THREAD_READPPM_H
#define PIXMAP_MULTI_THREAD_READPPM_H

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


pixel_t pixel_new(uint8_t red, uint8_t green, uint8_t blue) {
    pixel_t pixel = {red, green, blue};

    return pixel;
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

bool pixel_equals(const pixel_t *p, const pixel_t *pbis) {
    return p->green == pbis->green && p->red == pbis->red && p->blue == pbis->blue;
}

bool pixel_equals_flex(const pixel_t *p, const pixel_t *pbis) {
    return abs(p->red - pbis->red) < 10 && abs(p->green - pbis->green) && abs(p->blue - pbis->blue);
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

    fgets(buff, sizeof(buff), fp);
    if (buff[0] != 'P' || buff[1] != '6') {
        fprintf(stderr, "Wrong format !\n");
        exit(1);
    }

    img = malloc(sizeof(ppm_image_t));

    fscanf(fp, "%d %d", &img->height, &img->width);
    img->lenght = img->height * img->width;

    while (getc(fp) != '\n');
    while (getc(fp) != '\n');

    img->data = malloc(img->height * img->width * sizeof(pixel_t));

    if (fread(img->data, sizeof(pixel_t), img->height * img->width, fp) != img->height * img->width) {
        fprintf(stderr, "Error while reading image\n");
        exit(1);
    }

    return img;
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

pixel_t *ppm_image_t_data(const ppm_image_t *ppmImage) {
    return ppmImage->data;
}

pixel_t ppm_pixel(const ppm_image_t *img, const size_t i, const size_t j) {
    if (i >= img->height || j >= img->width) {
        fprintf(stderr, "Error! i,j out of range!\n");
        exit(1);
    }

    return img->data[i * j];
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

size_t ppm_black_pixels_flex(const ppm_image_t *img) {
    pixel_t blackPixel = pixel_new(0, 0, 0);
    size_t nb_black_pixels = 0;
    for (int i = 0; i < img->lenght; ++i) {
        if (pixel_equals_flex(&img->data[i], &blackPixel))
            nb_black_pixels++;
    }
    return nb_black_pixels;
}


#endif //PIXMAP_MULTI_THREAD_READPPM_H
