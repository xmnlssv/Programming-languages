#include "image/image.h"
#include "inputs/bmp_io.h"
#include "inputs/file_open_close.h"
#include "transformations/90rotate.h"

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    if (argc != 4) {
        fprintf(stderr, "Usage: %s <source-image> <transformed-image> <angle>\n", argv[0]);
        return -1;
    }

    FILE* file;
    if (!file_open(argv[1], &file, "rb")) {
        return -2;
    }

    int angle = atoi(argv[3]);
    if (angle != 0 && angle != 90 && angle != -90 && angle != 180 && angle != -180 && angle != 270 && angle != -270) {
        fprintf(stderr, "Invalid angle. Allowed values are 0, 90, -90, 180, -180, 270, -270.\n");
        file_close(file); // Закрытие файла в случае ошибки
        return -1;
    }

    struct image img = {0};
    if (!from_bmp(file, &img)) {
        perror("Error: BMP reader failed");
        file_close(file); // Закрытие файла в случае ошибки
        return -3;
    }

    if (!file_close(file)) {
        perror("Error: Close file failed");
        image_destroy(&img);
        return -4;
    }

    struct image res = rotate(img, angle);
    image_destroy(&img);

    FILE* res_file;
    if (!file_open(argv[2], &res_file, "wb")) {
        image_destroy(&res);
        return -2;
    }

    if (!to_bmp(res_file, &res)) {
        perror("Error: BMP writer failed");
        image_destroy(&res);
        try_to_close_file(res_file);
        return -5;
    }

    image_destroy(&res);
    return try_to_close_file(res_file);
}
