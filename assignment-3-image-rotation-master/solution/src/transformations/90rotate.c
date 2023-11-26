#include "90rotate.h"
#include <malloc.h>

static struct image rotate_90_degrees_counter_clockwise(const struct image source, struct pixel *pixels) {
    for (size_t y = 0; y < source.height; ++y) {
        for (size_t x = 0; x < source.width; ++x) {
            pixels[get_index(x, source.height - 1 - y, source.height)] = source.data[get_index(y, x, source.width)];
        }
    }
    return some_image(source.height, source.width, pixels);
}

static struct image rotate_180_degrees( const struct image source, struct pixel *pixels ) {

    for (size_t i = 0; i < source.height * source.width; ++i) {
        pixels[ i ] = source.data [ source.height * source.width - i - 1 ];
    }

    return some_image(source.width, source.height, pixels);

}

static struct image rotate_270_degrees( const struct image source, struct pixel *pixels ) {

    for (size_t i = 0; i < source.height; ++i) {
        for (size_t j = 0; j < source.width; ++j) {
            pixels[ get_index(i, j, source.width) ] =
                    source.data [ get_index((source.width - 1 - j), i, source.height)];
        }
    }

    return some_image(source.height, source.width, pixels);

}

struct image rotate(const struct image source, const int angle) {
    if (source.data == NULL) {
        return some_image(0, 0, NULL);
    }

    struct pixel *pixels = malloc(sizeof(struct pixel) * source.width * source.height);
    if (pixels == NULL) {
        return some_image(0, 0, NULL);
    }

    struct image result;
    switch (angle) {
        case 90:
        case -270:
            result = rotate_90_degrees_counter_clockwise(source, pixels);
            break;
        case -90:
        case 270:
            result = rotate_270_degrees(source, pixels);
            break;
        case 180:
        case -180:
            result = rotate_180_degrees(source, pixels);
            break;
        case 0:
            free(pixels);
            return copy(source);
        default:
            free(pixels);
            return some_image(0, 0, NULL);
    }

    struct image final_result = copy(result);
    free(pixels);
    return final_result;
}
