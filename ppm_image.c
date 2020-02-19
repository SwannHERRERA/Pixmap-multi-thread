#include "ppm_image.h"

// Check if two pixels are equal (same value for red, green and blue)
// Returns boolean (logical verification)
bool pixel_equals(const pixel_t *self, const pixel_t *other) {
    return self->green == other->green && self->red == other->red && self->blue == other->blue;
}

// Check if two pixels are equal but with a flexibility on the RGB intensity
// Example : a pixel with r = 0, g = 0, b = 0 is black but, to the eye, so is one with r = 5, g = 5, b = 5
bool pixel_equals_flex(const pixel_t *p, const pixel_t *pbis, int accuracy) {
    return abs(p->red - pbis->red) <= accuracy &&
           abs(p->green - pbis->green) <= accuracy &&
           abs(p->blue - pbis->blue) <= accuracy;
}

// Get the intensity of blue pixel
uint8_t pixel_blue(const pixel_t *p) {
    return p->blue;
}

// Get the intensity of red pixel
uint8_t pixel_red(const pixel_t *p) {
    return p->red;
}

// Get the intensity of green pixel
uint8_t pixel_green(const pixel_t *p) {
    return p->green;
}

// Get the width of the image
int ppm_image_t_width(const ppm_image_t *ppmImage) {
    return ppmImage->width;
}

// Get the height of the image
int ppm_image_t_height(const ppm_image_t *ppmImage) {
    return ppmImage->height;
}

// Get the total number of pixels of the image
size_t ppm_image_t_totalPixels(const ppm_image_t *ppmImage) {
    return ppmImage->totalPixels;
}

// Get the number of black pixels
size_t ppm_black_pixels(const ppm_image_t *img) {
    pixel_t blackPixel = pixel_new(0, 0, 0);
    size_t nb_black_pixels = 0;
    for (size_t i = 0; i < img->totalPixels; ++i) {
        if (pixel_equals(&img->data[i], &blackPixel))
            nb_black_pixels++;
        }

    return nb_black_pixels;
}

// Get the number of black pixels, with a flexibility on the intensity
size_t ppm_black_pixels_flex(const ppm_image_t *img, int accuracy) {
    pixel_t blackPixel = pixel_new(0, 0, 0);
    size_t nb_black_pixels = 0;
    for (size_t i = 0; i < img->totalPixels; ++i) {
        if (pixel_equals_flex(&img->data[i], &blackPixel, accuracy))
            nb_black_pixels++;
        }
    return nb_black_pixels;
}

// Create a new pixel
pixel_t pixel_new(uint8_t red, uint8_t green, uint8_t blue) {
    pixel_t pixel = {red, green, blue};
    return pixel;
}

// Get the pixels array of the image
pixel_t *ppm_image_t_data(const ppm_image_t *ppmImage) {
    return ppmImage->data;
}

// Get a pixel of the image, depending on x and y coordinates
pixel_t ppm_pixel(const ppm_image_t *img, size_t x, size_t y) {
    assert((x < img->height && y < img->width) ? 1 : !fprintf(stderr, "Error! x,y out of range!\n"));
    return img->data[x + img->width * y];
}

// Bit by bit inversion of the RGB fields of a pixel
pixel_t pixel_invert(const pixel_t *p) {
    pixel_t pixel = {~p->red, ~p->green, ~p->blue};
    return pixel;
}

// Load the image data into the structure
ppm_image_t *ppm_malloc(const char *pathname) {
    char magicNumber[2], charInc;
    ppm_image_t *img;
    FILE *fp;

    // Open the binary file in reading mode
    fp = fopen(pathname, "rb");
    assert(fp ? 1 : fprintf(stderr, "Can't open file: %s\n", pathname));
    // Read the first two characters of the file
    // Check if the image format is P6
    fscanf(fp, "%c%c", magicNumber, magicNumber + 1);
    assert((magicNumber[0] == 'P' && magicNumber[1] == '6') ? 1 : !fprintf(stderr, "Wrong format!\n"));

    // Allocate the size of the image structure
    img = malloc(sizeof(ppm_image_t));
    assert(img != NULL ? 1 : fprintf(stderr, "Error allocating memory for img structure.\n"));

    // Move the position indicator the size of one character (handles both the space and return characters)
    fread(&charInc, sizeof(char), 1, fp);

    // Read the two next characters (height and width)
    fscanf(fp, "%lu %lu", &img->height, &img->width);
    // Multiply to get the image's total number of pixels
    img->totalPixels = img->height * img->width;

    // Move the position indicator the size of one character (handles both the space and return characters)
    fread(&charInc, sizeof(char), 1, fp);

    // Keep moving the position indicator until the next return character
    while (getc(fp) != '\n');

    // Allocate the size of the pixels array
    img->data = malloc(img->height * img->width * sizeof(pixel_t));
    // Store the pixels array into the structure
    // Check if the return value (the third parameters) matches the number of number of pixels
    assert(fread(img->data, sizeof(pixel_t), img->height * img->width, fp) == img->height * img->width ? 1 :
           fprintf(stderr, "Error while reading image\n"));

    return img;
}

// Save the copied negative image
void ppm_image_save(const char *pathname, const ppm_image_t *img) {
    // Allocate the size of the new path string (old path + 10 for "negative_" & '\0')
    char *newPath = malloc((strlen(pathname) + 10) * sizeof(char));
    char heightCopy[5], widthCopy[5]; // array of 4 + '\0' : supposing the image's longest side won't be more than 9999 px

    // Create the new path
    strcpy(newPath, "negative_");
    strcat(newPath, pathname);

    // Open the binary file in writing mode
    FILE *fp = fopen(newPath, "wb");
    assert(fp ? 1 : fprintf(stderr, "Can't create file: %s\n", newPath));

    // Get the original image's dimensions and store them into the negative version's.
    sprintf(heightCopy, "%lu", img->height);
    sprintf(widthCopy, "%lu", img->width);

    // Write the negative image header (format, height, width and '255')
    // Check if the return value (total number of written characters) matches the number of characters in the original image
    assert((unsigned int)fprintf(fp, "P6\n%lu %lu\n255\n", img->height, img->width) == 9 + strlen(heightCopy) + strlen(widthCopy) ? 1 :
           fprintf(stderr, "Error while writing header to file\n"));

    // Write the pixels on the copied image
    assert(fwrite(img->data, sizeof(pixel_t), img->totalPixels, fp) == img->totalPixels ? 1 :
           fprintf(stderr, "Error while writing pixels to file\n"));

    free(newPath);
    // Close the file
    fclose(fp);
}


// Bit by bit inversion of all the image's pixels (create a negative version of the image)
void ppm_negative(ppm_image_t *img) {
    for (size_t i = 0; i < img->totalPixels; ++i) {
        img->data[i] = pixel_invert(&img->data[i]);
    }
}

// Thread 1 function : count first half of black and black-ish pixels
void *ppm_black_pixels_T1(void *arg) {
    printf("T1 created!\n"
           "Counting first half of black pixels with T1...\n");

    // Casting back the argument to its original thread_args format
    thread_args *arguments = (thread_args *) arg;

    pixel_t blackPixel = pixel_new(0, 0, 0);
    for (size_t i = 0; i < (arguments->img->totalPixels) / 2; i++) {
        if (pixel_equals(&(arguments->img->data[i]), &blackPixel))
            arguments->black_pixels->count_T1++;
        }
    printf("T1 first half count: done!\n");

    printf("Counting first half of black-ish pixels with T1 (accuracy = 10)...\n");
    for (size_t i = 0; i < (arguments->img->totalPixels) / 2; i++) {
        if (pixel_equals_flex(&(arguments->img->data[i]), &blackPixel, 10))
            arguments->black_pixels->flex_count_T1++;
        }
    printf("T1 first half flex count: done!\n");

    // End of the thread
    pthread_exit(NULL);
}

// Thread 2 function : count second half of black and black-ish pixels
void *ppm_black_pixels_T2(void *arg) {
    printf("T2 created!\n"
           "Counting first half of black pixels with T2...\n");

    // Casting back the argument to its original thread_args format
    thread_args *arguments = (thread_args *) arg;

    pixel_t blackPixel = pixel_new(0, 0, 0);
    for (size_t i = (arguments->img->totalPixels) / 2; i < arguments->img->totalPixels; i++) {
        if (pixel_equals(&(arguments->img->data[i]), &blackPixel)) {
            arguments->black_pixels->count_T2++;
        }
    }
    printf("T2 second half count: done!\n");

    printf("Counting second half of black-ish pixels with T2 (accuracy = 10)...\n");
    for (size_t i = (arguments->img->totalPixels) / 2; i < arguments->img->totalPixels; i++) {
        if (pixel_equals_flex(&(arguments->img->data[i]), &blackPixel, 10)) {
            arguments->black_pixels->flex_count_T2++;
        }
    }
    printf("T2 first half flex count: done!\n");

    // End of the thread
    pthread_exit(NULL);
}

void ppm_destroy(ppm_image_t *ppmImage) {
    free(ppmImage->data);
    free(ppmImage);
}
