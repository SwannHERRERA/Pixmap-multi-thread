/*
Author: Joëlle Castelli, Noé Larrieu-Lacoste, Swann HERRERA
Objectif: Ouvrir un fichier ppm le lire avec plusieur thread voir syllabus
Date: 13/01/2020
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <glib.h>
#include <gtk/gtk.h>
#include <libgen.h>

#include "readPPM.h"
#include "gtkWindow.h"

int main(int argc, char **argv) {
    if (argc != 2) {
        fprintf(stderr, "Wrong number of arguments !");
        return EXIT_FAILURE;
    } else {
        char *filePath = malloc(strlen(argv[1]));
        strcpy(filePath, argv[1]);
        image_t *img = readPPM(filePath);

        showImage(img, basename(filePath), argc, argv);

        free(filePath);
        free(img->data);
        free(img);
    }
}