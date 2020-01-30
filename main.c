/*
Author: Joëlle Castelli, Noé Larrieu-Lacoste, Swann HERRERA
Objectif: Ouvrir un fichier ppm le lire avec plusieur thread voir syllabus
Date: 13/01/2020
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <libgen.h>

#include "ppm_image.h"

int main(int argc, char **argv) {
    if (argc != 2) {
        fprintf(stderr, "Wrong number of arguments !");
        return EXIT_FAILURE;
    } else {
        char *filePath = malloc(strlen(argv[1]));
        strcpy(filePath, argv[1]);

        printf("\nLoading ppm image into ppm_image_t structure...\n");
        ppm_image_t *img = ppm_malloc(filePath);
        printf("Done!\n"
               "Name: %s\n"
               "Width: %d\n"
               "Height: %d\n"
               "Lenght: %zu\n\n", basename(filePath), img->width, img->height, img->lenght);

        printf("Reading pixel x:5, y:128\n");
        pixel_t pixel = ppm_pixel(img, 5, 128);
        printf("Done!\n"
               "Pixel: {\n"
               "   red: %d,\n"
               "   green: %d,\n"
               "   blue: %d\n"
               "};\n\n", pixel.red, pixel.green, pixel.blue);

        printf("Counting black pixels...\n");
        size_t nb_black_pixels = ppm_black_pixels(img);
        printf("Done!\n"
               "Black pixel count in ppm image: %zu\n\n", nb_black_pixels);

        printf("Counting black pixels flexible (10 accuracy)...\n");
        size_t nb_black_pixels_flex = ppm_black_pixels_flex(img, 10);
        printf("Done!\n"
               "Flexible black pixel count in ppm image: %zu\n\n", nb_black_pixels_flex);

        printf("Converting image's pixel to negatif...\n");
        ppm_negatif(img);
        printf("Done!\n\n");

        printf("Saving new negatif image...\n");
        ppm_image_save(filePath, img);
        printf("Done!\n\n");

        printf("Releasing memory...\n");
        free(filePath);
        free(img->data);
        free(img);
        printf("Done!\n\n"
               "Closing program");
        return EXIT_SUCCESS;
    }
}