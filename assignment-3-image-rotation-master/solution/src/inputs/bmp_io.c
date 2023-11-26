#include "bmp_io.h"
#include "stdint.h"
#include <stdbool.h>

struct __attribute__((packed)) bmp_header
{
    uint16_t bfType;
    uint32_t  bfileSize;
    uint32_t bfReserved;
    uint32_t bOffBits;
    uint32_t biSize;
    uint32_t biWidth;
    uint32_t  biHeight;
    uint16_t  biPlanes;
    uint16_t biBitCount;
    uint32_t biCompression;
    uint32_t biSizeImage;
    uint32_t biXPelsPerMeter;
    uint32_t biYPelsPerMeter;
    uint32_t biClrUsed;
    uint32_t  biClrImportant;
};

inline static size_t size_of_padding(const size_t width) {

    if (width % 4 == 0) {
        return 0;
    }
    return 4 - ((width * sizeof(struct pixel)) % 4);

}

inline static bool head_read(FILE* file, struct bmp_header* header) {
    return fread(header, sizeof(struct bmp_header), 1, file);
}

inline static size_t size_of_image(const struct image* image) {
    return (image->width * sizeof(struct pixel) + size_of_padding(image->width)) * image->height;
}

inline static size_t size_of_file(const struct image* image) {
    return size_of_image(image) + sizeof(struct bmp_header);
}

inline static struct bmp_header create_header(const struct image *image) {
    return (struct bmp_header) {
            .bfType = 19778,
            .bfileSize = size_of_file(image),
            .bfReserved = 0,
            .bOffBits = 54,
            .biSize = 40,
            .biWidth = image->width,
            .biHeight = image->height,
            .biPlanes = 1,
            .biBitCount = 24,
            .biCompression = 0,
            .biSizeImage = size_of_image(image),
            .biXPelsPerMeter = 0,
            .biYPelsPerMeter = 0,
            .biClrUsed = 0,
            .biClrImportant = 0
    };
}


bool from_bmp(FILE* in, struct image* image) {

    struct bmp_header header = {0};
    if (!head_read(in, &header)) {
        image_destroy(image);
        return false;
    }

    //size_t size = size_of_padding(image->width);
    *image = image_create(header.biWidth, header.biHeight);

    const size_t padding = size_of_padding(image->width);

    for (size_t i = 0; i < image->height; ++i) {
        for (size_t j = 0; j < image->width; ++j) {
            if (!fread(&(image->data[image->width * i + j]), sizeof(struct pixel), 1, in)) {
                image_destroy(image);
                return false;
            }
        }
        if (fseek(in, padding, SEEK_CUR)) {
            image_destroy(image);
            return false;
        }
    }

    return true;
}

bool to_bmp(FILE* out, const struct image* image) {

    struct bmp_header header = create_header(image);

    if (!fwrite(&header, sizeof(struct bmp_header), 1, out)) {
        return false;
    }

    if (fseek(out, header.bOffBits, SEEK_SET)) {
        return false;
    }

    //const int8_t zero = 0;

    const uint8_t paddings[3] = {0};

    const size_t padding = size_of_padding(image->width);

    if (image->data == NULL) {
        return false;
    }

    for (size_t i = 0; i < image->height; ++i) {

        if (    !fwrite(image->data + i * image->width, image->width * sizeof(struct pixel), 1, out) ||
                !fwrite(paddings, padding, 1, out) ) {
            return false;
        }

    }

    return true;

}