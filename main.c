#include <stdio.h>
#include <stdlib.h>
#include <png.h>
#include <pari/pari.h>

#define ERROR 1
#define OK 0

void pari_test() {
    ulong m;

    printf("hi\n");
    pari_init(100000000, 1000000000);

    m = maxprime();
    printf("%ld\n", (long) m);

    #if 0
    byteptr ptr = diffptr;
    ulong p = 0;
    while (p < m) {
        NEXT_PRIME_VIADIFF(p, ptr);
        printf("%ld\n", (long) p);
    }
    #endif

    pari_close();
}

void free_row_pointers(png_bytepp rp, unsigned int height) {
    unsigned int i;
    for (i = 0; i < height; i++) {
        free(rp[i]);
    }
    free(rp);
}

int pngtest() {
    const char file_name[] = "hi.png";
    const unsigned int WIDTH = 500;
    const unsigned int HEIGHT = 500;
    const unsigned char BITDEPTH = 16;
    png_bytepp row_pointers;
    unsigned int i;
    unsigned int j;

    row_pointers = malloc(HEIGHT * sizeof(png_bytep));
    if (row_pointers == NULL) {
        return ERROR;
    }

    for (i = 0; i < HEIGHT; i++) {
        row_pointers[i] = malloc(WIDTH * sizeof(png_byte));
        if (row_pointers[i] == NULL) {
            free_row_pointers(row_pointers, i);
            return ERROR;
        }
        for (j = 0; j < WIDTH; j++) {
            row_pointers[i][j] = 0x1FF;
        }
    }

    FILE *fp;
    png_structp png_ptr;
    png_infop info_ptr;

    fp = fopen(file_name, "wb");
    if (fp == NULL) return ERROR;
    png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (png_ptr == NULL) {
        fclose(fp); free_row_pointers(row_pointers, HEIGHT);
        return ERROR;
    }
    info_ptr = png_create_info_struct(png_ptr);
    if (info_ptr == NULL) {
        fclose(fp); free_row_pointers(row_pointers, HEIGHT);
        png_destroy_write_struct(&png_ptr, NULL);
        return ERROR;
    }
    if (setjmp(png_jmpbuf(png_ptr))) {
        fclose(fp); free_row_pointers(row_pointers, HEIGHT);
        png_destroy_write_struct(&png_ptr, &info_ptr);
        return ERROR;
    }
    png_init_io(png_ptr, fp);

    png_set_IHDR(png_ptr, info_ptr,
                 WIDTH, HEIGHT, BITDEPTH,
                 PNG_COLOR_TYPE_RGBA, PNG_INTERLACE_NONE,
                 PNG_COMPRESSION_TYPE_BASE,
                 PNG_FILTER_TYPE_BASE);

    png_write_info(png_ptr, info_ptr);
    png_write_image(png_ptr, row_pointers);
    png_write_end(png_ptr, info_ptr);

    png_destroy_write_struct(&png_ptr, &info_ptr);
    fclose(fp); free_row_pointers(row_pointers, HEIGHT);
    return OK;
}

int main() {
    int is_success = pngtest();
    return is_success;
}
