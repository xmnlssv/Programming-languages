#include "bmp_io.h"
#include "stdint.h"
#include <stdbool.h>

struct __attribute__((packed)) bmp_header {
    uint16_t bfType;
    uint32_t bfileSize;
    uint32_t bfReserved;
    uint32_t bOffBits;
    uint32_t biSize;
    uint32_t biWidth;
    uint32_t biHeight;
    uint16_t biPlanes;
    uint16_t biBitCount;
    uint32_t biCompression;
    uint32_t biSizeImage;
    uint32_t biXPelsPerMeter;
    uint32_t biYPelsPerMeter;
    uint32_t biClrUsed;
    uint32_t biClrImportant;
};

inline static size_t size_of_padding(const size_t width) {
    return (4 - (width * sizeof(struct pixel)) % 4) % 4;
}

inline static bool head_read(FILE* file, struct bmp_header* header) {
    return fread(header, sizeof(struct bmp_header), 1, file) == 1;
}

bool from_bmp(FILE* in, struct image* image) {
    struct bmp_header header = {0};
    if (!head_read(in, &header)) {
        image_destroy(image);
        return false;
    }

    *image = image_create(header.biWidth, header.biHeight);

    size_t padding = size_of_padding(image->width);

    for (size_t i = 0; i < image->height; ++i) {
        for (size_t j = 0; j < image->width; ++j) {
            if (!fread(&(image->data[image->width * i + j]), sizeof(struct pixel), 1, in)) {
                image_destroy(image);
                return false;
            }
        }
        if (fseek(in, (long)padding, SEEK_CUR)) {
            perror("Error seeking in BMP file");
            image_destroy(image);
            return false;
        }
    }

    return true;
}

