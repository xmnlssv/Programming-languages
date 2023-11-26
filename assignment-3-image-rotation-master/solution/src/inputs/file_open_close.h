#ifndef LAB3_FILE_OPEN_CLOSE_H
#define LAB3_FILE_OPEN_CLOSE_H

#include "../image/image.h"

#include <stdbool.h>
#include <stdio.h>

bool file_open(const char* fileName, FILE** file, const char* mode);

bool file_close(FILE* file);

static inline int16_t try_to_close_file(FILE* file) {
    if (!file_close(file)) {
        perror("error: close file failed");
        return -4;
    }
    return 0;
}

#endif //LAB3_FILE_OPEN_CLOSE_H