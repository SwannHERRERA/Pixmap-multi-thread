//
// Created by MaleWhere on 22/01/2020.
//

#ifndef PIXMAP_MULTI_THREAD_READPPM_H
#define PIXMAP_MULTI_THREAD_READPPM_H

typedef struct {
    guint8 red;
    guint8 green;
    guint8 blue;
} pixel_t;

typedef struct {
    int x, y;
    pixel_t *data;
} image_t;

image_t *readPPM(const char *fileName) {
    char buff[16];
    image_t *img;
    FILE *fp;

    fp = fopen(fileName, "rb");
    if (!fp) {
        fprintf(stderr, "Can't open file: %s\n", fileName);
        exit(1);
    }

    fgets(buff, sizeof(buff), fp);
    if (buff[0] != 'P' || buff[1] != '6') {
        fprintf(stderr, "Wrong format !\n");
        exit(1);
    }

    img = malloc(sizeof(image_t));

    fscanf(fp, "%d %d", &img->x, &img->y);

    while (getc(fp) != '\n');
    while (getc(fp) != '\n');

    img->data = malloc(img->x * img->y * sizeof(pixel_t));

    if (fread(img->data, sizeof(pixel_t), img->x * img->y, fp) != img->x * img->y) {
        fprintf(stderr, "Error while reading image\n");
        exit(1);
    }

    return img;
}


#endif //PIXMAP_MULTI_THREAD_READPPM_H
