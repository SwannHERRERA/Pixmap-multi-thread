#include "ppm_image.h"

// Check if two pixels are equal (same value for red, green and blue)
bool pixel_equals(const pixel_t *self, const pixel_t *other) {
    return self->green == other->green && self->red == other->red && self->blue == other->blue;
}

// Check if two pixels are equal but with a flexibility on the RGB intensity
// Example : a pixel with r0, g0, b0 is black but, to the eye, so is one with 5, 5, 5
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
    for (int i = 0; i < img->totalPixels; ++i) {
        if (pixel_equals(&img->data[i], &blackPixel))
            nb_black_pixels++;
    }
    return nb_black_pixels;
}

// Get the number of black pixels, with a flexibility on the intensity
size_t ppm_black_pixels_flex(const ppm_image_t *img, int accuracy) {
    pixel_t blackPixel = pixel_new(0, 0, 0);
    size_t nb_black_pixels = 0;
    for (int i = 0; i < img->totalPixels; ++i) {
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
    if (x >= img->height || y >= img->width) {
        fprintf(stderr, "Error! x,y out of range!\n");
        exit(1);
    }
    return img->data[x + img->width * y];
}

// Bit by bit inversion of the RGB fields of a pixel
pixel_t pixel_invert(const pixel_t *p) {
    pixel_t pixel = {~p->red, ~p->green, ~p->blue};
    return pixel;
}

// Load the image data into the structure
ppm_image_t *ppm_malloc(const char *pathname) {
    char magicNumber[2];
    ppm_image_t *img;
    FILE *fp;

    // Open the binary file in reading mode
    fp = fopen(pathname, "rb");
    if (!fp) { // Check if the file has correctly been open
        fprintf(stderr, "Can't open file: %s\n", pathname);
        exit(EXIT_FAILURE);
    }

    // Read the first two characters of the file
    // Check if the image format is P6
    fscanf(fp, "%c%c", magicNumber, magicNumber + 1);
    if (magicNumber[0] != 'P' || magicNumber[1] != '6') {
        fprintf(stderr, "Wrong format!\n");
        exit(EXIT_FAILURE);
    }

    // Allocate the size of the image structure
    img = malloc(sizeof(ppm_image_t));
    if (img == NULL) { // Check if the memory has correctly been allocated
        fprintf(stderr, "Error allocating memory for img structure.\n");
        exit(EXIT_FAILURE);
    }

    // Move the position indicator the size of one character (handles both the space and return characters)
    fseek(fp, sizeof(char), SEEK_CUR);

    // Read the two next characters (height and width)
    fscanf(fp, "%d %d", &img->height, &img->width);
    // Multiply to get the image's total number of pixels
    img->totalPixels = img->height * img->width;

    // Move the position indicator the size of one character (handles both the space and return characters)
    fseek(fp, sizeof(char), SEEK_CUR);

    // Keep moving the position indicator until the next return character
    while (getc(fp) != '\n');

    // Allocate the size of the pixels array
    img->data = malloc(img->height * img->width * sizeof(pixel_t));
    // Store the pixels array into the structure
    // Check if the return value (the third parameters) matches the number of number of pixels
    if (fread(img->data, sizeof(pixel_t), img->height * img->width, fp) != img->height * img->width) {
        fprintf(stderr, "Error while reading image\n");
        exit(EXIT_FAILURE);
    }

    return img;
}

// Save the copied negative image
void ppm_image_save(const char *pathname, const ppm_image_t *img) {
    // Allocate the size of the new path string (old path + 9 for "negative_")
    char *newPath = malloc((strlen(pathname) + 9) * sizeof(char));
    char heightCopy[5], widthCopy[5]; // array of 4 + '\0' : supposing the image's longest side won't be more than 9999 px

    // Create the new path
    strcpy(newPath, "negative_");
    strcat(newPath, pathname);

    // Open the binary file in writing mode
    FILE *fp = fopen(newPath, "wb");
    if (!fp) { // Check if the file has correctly been open
        fprintf(stderr, "Can't create file: %s\n", newPath);
        exit(EXIT_FAILURE);
    }

    // Get the original image's dimensions and store them into the negative version's.
    sprintf(heightCopy, "%d", img->height);
    sprintf(widthCopy, "%d", img->width);

    // Write the negative image header (format, height, width and '255')
    // Check if the return value (total number of written characters) matches the number of characters in the original image
    if (fprintf(fp, "P6\n%d %d\n255\n", img->height, img->width) != 9 + strlen(heightCopy) + strlen(widthCopy)) {
        fprintf(stderr, "Error while writing header to file\n");
        exit(EXIT_FAILURE);
    }

    // Write the pixels on the copied image
    if (fwrite(img->data, sizeof(pixel_t), img->totalPixels, fp) != img->totalPixels) {
        fprintf(stderr, "Error while writing pixels to file\n");
        exit(EXIT_FAILURE);
    }

    // Close the file
    fclose(fp);
}


// Bit by bit inversion of all the image's pixels (create a negative version of the image)
void ppm_negative(ppm_image_t *img) {
    int i;
    pixel_t pixel;

    for (i = 0; i < img->totalPixels; ++i) {
        pixel = pixel_invert(img->data + i);
        img->data[i].red = pixel.red;
        img->data[i].green = pixel.green;
        img->data[i].blue = pixel.blue;
    }
}
