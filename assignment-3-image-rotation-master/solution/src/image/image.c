#include "image.h"
#include <malloc.h>
#include <memory.h>

struct image image_create(const size_t image_width, const size_t image_height) {
    struct pixel* pixels = malloc(sizeof(struct pixel) * image_width * image_height);
    if (pixels == NULL) {
        struct image empty_image = {0};
        return empty_image;
    }
    return some_image(image_width, image_height, pixels);
}

struct image copy(struct image image) {
    size_t size_of_array = sizeof(struct pixel) * image.width * image.height;
    struct pixel* pixels = malloc(size_of_array);
    if (pixels == NULL) {
        struct image empty_image = {0};
        return empty_image;
    }
    for (size_t i = 0; i < image.height * image.width; ++i) {
        pixels[i] = image.data[i];
    }
    return some_image(image.width, image.height, pixels);
}

void image_destroy(const struct image* image) {
    free(image->data);
}
