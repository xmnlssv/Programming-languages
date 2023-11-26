#ifndef LAB3_BMP_IO_H
#define LAB3_BMP_IO_H

#include <malloc.h>
#include <stdint.h>

#include "../image/image.h"

bool from_bmp(FILE* in, struct image* image);

bool to_bmp(FILE* out, const struct image* image);

#endif //LAB3_BMP_IO_H