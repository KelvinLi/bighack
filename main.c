#include <stdio.h>
#include <pari/pari.h>

int main(){
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
    return 0;
}
