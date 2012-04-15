#include <stdio.h>
#include <pari/pari.h>

int main(){
    pari_init(100*1000*1000, 0);
    printf("hi\n");
    pari_close();
    return 0;
}
