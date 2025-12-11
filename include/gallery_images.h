#ifndef GALLERY_IMAGES_H
#define GALLERY_IMAGES_H

#include <stdint.h>
#include "united.h"
#include "landscape.h"
#include "booker.h"
#include "grealish.h"

// Define how many images we have
#define NUM_GALLERY_IMAGES 4

// Array of pointers to the image bitmaps
extern const unsigned char* gallery_images[NUM_GALLERY_IMAGES];

// Function to get image by index
const unsigned char* get_gallery_image(int index);

#endif
