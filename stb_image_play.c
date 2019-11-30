#include <stdio.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

int main() {
    int width, height, channels;
    unsigned char* pixels;
    pixels = stbi_load("hazard.png", &width, &height, &channels, 0);
    printf("Loaded %dX%d image with %d channels.", width, height, channels);
    stbi_image_free(pixels);
}
