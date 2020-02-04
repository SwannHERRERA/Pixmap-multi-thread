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


        // Structure that stocks the number of black pixels calculated by each new thread
        pixels_count black_pixels;
        black_pixels.count_T1 = 0;
        black_pixels.count_T2 = 0;

        // Thread 1 function : count first half of black pixels
        void* ppm_black_pixels_T1(void* arg) {
            printf("T1 created!\n"
                    "Counting first half of black pixels with T1...\n");
            pixel_t blackPixel = pixel_new(0, 0, 0);
            for (int i = 0; i < (img->totalPixels) / 2; i++) {
                if (pixel_equals(&img->data[i], &blackPixel))
                    black_pixels.count_T1++;
            }
            printf("T1 first half count: done!\n"
                   "First half of black pixels in ppm image with T1: %zu\n\n", black_pixels.count_T1);

            // End of the thread
            pthread_exit(NULL);
        }

        // Thread 2 function : count second half of black pixels
        void* ppm_black_pixels_T2 (void* arg) {
            printf("T2 created!\n"
                   "Counting first half of black pixels with T2...\n");
            pixel_t blackPixel = pixel_new(0, 0, 0);
            for (size_t i = (img->totalPixels) / 2; i < img->totalPixels; i++) {
                if (pixel_equals(&img->data[i], &blackPixel))
                    black_pixels.count_T2++;
            }
            printf("T2 second half count: done!\n"
                   "Second half of black pixels in ppm image with T2: %zu\n\n", black_pixels.count_T1);
            // End of the thread
            pthread_exit(NULL);
        }

        pthread_t thread1;
        pthread_t thread2;
        printf("Creating new threads...\n");

        if(pthread_create(&thread1, NULL, ppm_black_pixels_T1, (void*)NULL)) {
            fprintf(stderr, "Cannot create new thread : T1\n");
            return EXIT_FAILURE;
        }

        if(pthread_create(&thread2, NULL, ppm_black_pixels_T2, (void*)NULL)) {
            fprintf(stderr, "Cannot create new thread : T2\n");
            return EXIT_FAILURE;
        }

        // Waiting for T1 to finish
        if(pthread_join(thread1, NULL)) {
            fprintf(stderr, "Cannot wait for thread 1 to finish.\n");
            return EXIT_FAILURE;
        }

        // Waiting for T2 to finish
        if(pthread_join(thread2, NULL)) {
            fprintf(stderr, "Cannot wait for thread 2 to finish.\n");
            return EXIT_FAILURE;
        }

        printf("Multithreaded total of black pixels in ppm image: %zu\n\n", black_pixels.count_T1 + black_pixels.count_T2);

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