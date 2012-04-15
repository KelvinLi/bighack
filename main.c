#include <stdio.h>
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

int pngtest() {
    const char file_name[] = "hi.png";
    FILE *fp;
    png_structp png_ptr;
    png_infop info_ptr;
    png_colorp palette;

    fp = fopen(file_name, "wb");
    if (fp == NULL) return ERROR;

    png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (png_ptr == NULL) {
        fclose(fp);
        return ERROR;
    }

    info_ptr = png_create_info_struct(png_ptr);
    if (info_ptr == NULL) {
        fclose(fp);
        png_de
    }
    fclose(fp);
    return OK;
}

int main() {
    int is_success = pngtest();
    return is_success;
}
