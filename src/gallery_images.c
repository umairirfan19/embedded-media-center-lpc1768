#include "gallery_images.h"

// Array of pointers to each image (as unsigned char*)
const unsigned char* gallery_images[NUM_GALLERY_IMAGES] = {
    united_bitmap,
    landscape_bitmap,
    booker_bitmap,
		grealish_bitmap
};

// Helper function to safely get an image
const unsigned char* get_gallery_image(int index) {
    if (index >= 0 && index < NUM_GALLERY_IMAGES) {
        return gallery_images[index];
    }
    return gallery_images[0]; // Default to first image
}
