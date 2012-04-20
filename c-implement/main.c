#include "rect_coord.h"
#include <stdlib.h>
#include <stdio.h>
#include <png.h>
#include <pari/pari.h>

int pari_gen_turn_arr(int **turn_arr, int size_hint) {
    ulong m;
    int i = 0;
    ulong p = 0;

    *turn_arr = malloc(size_hint * sizeof(**turn_arr));
    if (*turn_arr == NULL) {
        return (int) NULL;
    }
    pari_init(10000000, size_hint);
    
    m = maxprime();

    byteptr ptr = diffptr;
    
    while (p < m && i < size_hint) {
        NEXT_PRIME_VIADIFF(p, ptr);
        (*turn_arr)[i] = p;
        i++;
    }

    pari_close();
    return i;
}

void free_row_pointers(png_bytepp rp, unsigned int height) {
    unsigned int i;
    for (i = 0; i < height; i++) {
        free(rp[i]);
    }
    free(rp);
}

int pngtest(int **map, int WIDTH, int HEIGHT) {
    const char file_name[] = "hi.png";
    const unsigned char BITDEPTH = 8;
    png_bytepp row_pointers;
    int i;
    int j;
    int intensity;

    FILE *fp;
    png_structp png_ptr;
    png_infop info_ptr;

    row_pointers = malloc(HEIGHT * sizeof(png_bytep));
    if (row_pointers == NULL) {
        return EXIT_FAILURE;
    }

    for (i = 0; i < HEIGHT; i++) {
        row_pointers[i] = malloc(4*WIDTH * sizeof(png_byte));
        if (row_pointers[i] == NULL) {
            free_row_pointers(row_pointers, i);
            return EXIT_FAILURE;
        }
        memset(row_pointers[i], 0xFF, 4*WIDTH*sizeof(png_byte));
        for (j = 0; j < WIDTH; j++) {
            intensity = 50*(map[i][j] - 1);
            if (map[i][j] > 0) {
                row_pointers[i][4*j]=(intensity < 0xFF) ? intensity : 0xFF;
                row_pointers[i][4*j+1]=0;
                row_pointers[i][4*j+2]=0;
            }
        }
    }

    fp = fopen(file_name, "wb");
    if (fp == NULL) return EXIT_FAILURE;
    png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (png_ptr == NULL) {
        fclose(fp); free_row_pointers(row_pointers, HEIGHT);
        return EXIT_FAILURE;
    }
    info_ptr = png_create_info_struct(png_ptr);
    if (info_ptr == NULL) {
        fclose(fp); free_row_pointers(row_pointers, HEIGHT);
        png_destroy_write_struct(&png_ptr, NULL);
        return EXIT_FAILURE;
    }
    if (setjmp(png_jmpbuf(png_ptr))) {
        fclose(fp); free_row_pointers(row_pointers, HEIGHT);
        png_destroy_write_struct(&png_ptr, &info_ptr);
        return EXIT_FAILURE;
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
    return EXIT_SUCCESS;
}

int main(int argc, char **argv) {
    int a = -500;
    int b =  500;
    int c =  500;
    int d = -500;
    if (argc == 1) {
        printf("using defaults\n");
    } else if (argc == 5) {
        a = atoi(argv[1]);
        b = atoi(argv[2]);
        c = atoi(argv[3]);
        d = atoi(argv[4]);
    } else {
        printf("Bad arguments\n");
        return EXIT_FAILURE;
    }

    int *turn_arr;
    struct point map_top_left = {.x=a, .y=b};
    struct point map_bot_right = {.x=c, .y=d};
    struct rect_region map_region = { .top_left=map_top_left, .bot_right=map_bot_right };
    int turn_arr_size = pari_gen_turn_arr(&turn_arr, 1000000);
    int** map = make_map(turn_arr, turn_arr_size, &map_region);
    int is_success = pngtest(map, rect_region_width(&map_region), rect_region_height(&map_region));
    return is_success;
}
