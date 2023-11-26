#include "90rotate.h"
#include "../image/image.h"
#include <stdlib.h>
#include <malloc.h>

static struct image rotate_90_degrees( const struct image source, struct pixel *pixels ) {

    for (size_t y = 0; y < source.height; ++y) {
        for (size_t x = 0; x < source.width; ++x) {
            pixels[ get_index(source.height - 1 - y, x, source.height) ] = source.data [get_index(x, y, source.width)];
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


struct image rotate(const struct image source, const uint16_t degrees ) {

    if (source.data == NULL) {
        return some_image(source.height, source.width, NULL);
    }

    struct pixel *pixels = malloc(sizeof(struct pixel) * source.width * source.height);

    switch (degrees) {
        case 90:  return rotate_90_degrees(source, pixels);
        case 180: return rotate_180_degrees(source, pixels);
        case 270: return rotate_270_degrees(source, pixels);
        default: return rotate_90_degrees(source, pixels);
    }
}