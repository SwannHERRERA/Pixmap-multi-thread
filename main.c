/* Authors: Joëlle CASTELLI, Noé LARRIEU-LACOSTE, Swann HERRERA
Objectives :
        - multithreaded count of total the number of black pixels
        - creating and saving a negative version of the image
Date: 13/01/2020 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <libgen.h>
#include "ppm_image.h"

int main(int argc, char **argv) {
    if (argc != 2) { // Check the number of arguments (only one expected)
        fprintf(stderr, "Wrong number of arguments!");
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
               "totalPixels: %zu\n\n", basename(filePath), img->width, img->height, img->totalPixels);

        printf("Reading pixel x:5, y:128\n");
        pixel_t pixel = ppm_pixel(img, 5, 128);
        printf("Done!\n"
               "Pixel: {\n"
               "   red: %d,\n"
               "   green: %d,\n"
               "   blue: %d\n"
               "};\n\n", pixel.red, pixel.green, pixel.blue);

        printf("Counting total number of black pixels...\n");
        size_t nb_black_pixels = ppm_black_pixels(img);
        printf("Done!\n"
               "Total number of black pixels in ppm image: %zu\n\n", nb_black_pixels);

        printf("Flexible counting total number of black pixels (accuracy = 10)...\n");
        size_t nb_black_pixels_flex = ppm_black_pixels_flex(img, 10);
        printf("Done!\n"
               "Total number of black and black-ish pixels in ppm image: %zu\n\n", nb_black_pixels_flex);

        /* COUNT WITH TWO THREADS */

        printf("Counting first half of black pixels with T0...\n");
        size_t nb_black_pixels0 = ppm_black_pixels_T0(img);
        size_t nb_black_pixels1 = 0;
        printf("Done!\n"
               "First half of black pixels in ppm image: %zu\n\n", nb_black_pixels0);

        //arg1 : pointeur vers l'identifiant du thread (pthread_t)
        //arg2 : attributs du thread (joignable, détaché, politique d'ordonnancement usuelle ou temps-réel... = NULL)
        //arg3 : pointeur vers la fonction à exécuter : doit être void* fonction(void* arg)
        //arg4 : argument à passer au thread.
        pthread_t thread1;
        printf("Creating new thread...\n");
        if(pthread_create(&thread1, NULL, ppm_black_pixels_T1, (void*)NULL) == -1) {
            fprintf(stderr, "Cannot create new thread\n");
            return EXIT_FAILURE;
        }


        /* END */

        printf("Converting image's pixels to negative...\n");
        ppm_negative(img);
        printf("Done!\n\n");

        printf("Saving new negative image...\n");
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