#ifndef LAB3_IMAGE_H
#define LAB3_IMAGE_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

struct __attribute__ ((packed)) pixel {
    uint8_t b;
    uint8_t g;
    uint8_t r;
};

struct image {
    uint64_t width;
    uint64_t height;
    struct pixel *data;
};

struct image image_create(uint64_t image_width, uint64_t image_height);
struct image copy(struct image image);
void image_destroy(const struct image* image);

static inline struct image some_image(uint64_t width, uint64_t height, struct pixel *data) {
    return (struct image) {.width = width, .height = height, .data = data};
}

static inline size_t get_index(const size_t x, const size_t y, const size_t width) {
    return y * width + x;
}

#endif //ASSIGNMENT_IMAGE_ROTATION_IMAGE_H