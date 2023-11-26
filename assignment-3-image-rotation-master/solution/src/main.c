
#include "image/image.h"
#include "inputs/bmp_io.h"
#include "inputs/file_open_close.h"
#include "transformations/90rotate.h"

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

int main(int argc, char *argv[]) {

    if (argc != 3) {
        perror("Incorrect arguments");
        return -1;
    }

    FILE* file;
    if (!file_open(argv[1], &file, "r")) {
        return -2;
    }

    struct image img = {0};

    if (!from_bmp(file, &img)) {
        perror("error: bmp reader failed");
        return -3;
    }
    if (!file_close(file)) {
        perror("error: close file failed");
        image_destroy(&img);
        return -4;
    }

    struct image res = rotate(img, 90);
    image_destroy(&img);
    FILE* res_file;

    if (!file_open(argv[2], &res_file, "w")) {
        image_destroy(&res);
        return -2;
    }

    if (!to_bmp(res_file, &res)) {
        perror("error: bmp writer failed");
        image_destroy(&res);
        try_to_close_file(res_file);
        return -5;
    }

    image_destroy(&res);
    return try_to_close_file(res_file);

}