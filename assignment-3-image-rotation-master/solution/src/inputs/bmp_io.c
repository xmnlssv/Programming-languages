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

static struct bmp_header create_header(const struct image *img) {
    struct bmp_header header = {0};

    size_t padding = size_of_padding(img->width);
    size_t paddedRowSize = img->width * sizeof(struct pixel) + padding;
    size_t totalImageSize = paddedRowSize * img->height;

    header.bfType = 0x4D42;
    header.bfileSize = sizeof(struct bmp_header) + totalImageSize;
    header.bfReserved = 0;
    header.bOffBits = sizeof(struct bmp_header);
    header.biSize = 40;
    header.biWidth = img->width;
    header.biHeight = img->height;
    header.biPlanes = 1;
    header.biBitCount = 24;
    header.biCompression = 0;
    header.biSizeImage = totalImageSize;
    header.biXPelsPerMeter = 0;
    header.biYPelsPerMeter = 0;
    header.biClrUsed = 0;
    header.biClrImportant = 0;

    return header;
}

inline static bool head_read(FILE* file, struct bmp_header* header) {
    return fread(header, sizeof(struct bmp_header), 1, file) == 1;
}

bool from_bmp(FILE* in, struct image* image) {
    struct bmp_header header = {0};
    if (!head_read(in, &header) || header.bfType != 0x4D42) {
        image_destroy(image);
        return false;
    }

    *image = image_create(header.biWidth, header.biHeight);
    if (image->data == NULL) {
        image_destroy(image);
        return false;
    }

    size_t padding = size_of_padding(image->width);

    for (size_t i = 0; i < image->height; ++i) {
        if (fread(image->data + i * image->width, sizeof(struct pixel), image->width, in) != image->width) {
            image_destroy(image);
            return false;
        }
        if (padding > 0 && fseek(in, (long)padding, SEEK_CUR) != 0) {
            perror("Error seeking in BMP file");
            image_destroy(image);
            return false;
        }
    }
    return true;
}

bool to_bmp(FILE* out, const struct image* image) {
    if (image == NULL || image->data == NULL) {
        fprintf(stderr, "Error: Invalid image data\n");
        return false;
    }

    struct bmp_header header = create_header(image);

    if (fwrite(&header, sizeof(struct bmp_header), 1, out) != 1) {
        fprintf(stderr, "Error: Failed to write BMP header\n");
        return false;
    }

    const uint8_t paddings[3] = {0};
    size_t padding = size_of_padding(image->width);

    for (size_t i = 0; i < image->height; ++i) {
        if (fwrite(image->data + i * image->width, sizeof(struct pixel), image->width, out) != image->width ||
            fwrite(paddings, 1, padding, out) != padding) {
            fprintf(stderr, "Error: Failed to write image data\n");
            return false;
        }
    }

    return true;
}

