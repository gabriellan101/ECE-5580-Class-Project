
#define USE_REF 0
//gcc test_keygen.c sources/*.c -o test.exe -Iopenssl_files -Lopenssl_files -llibcrypto-3-x64


#include "includes/rng.h"
#if USE_REF
#include "ref_keygen.c"
#endif
#include <stdio.h>
#if !USE_REF
#include "includes/NTRUPrime.h"
#include <time.h>
#endif
typedef int8_t small;
int main() {
    printf("start\n");
    Fq h[761];
    small f[761], g[761], ginv[761];
    int16_t finv[761];
    int rand[100];
#if USE_REF
    printf("using ref\n");
    r_KeyGen(h,f,ginv);
#else
    clock_t start= clock();
    KeyGen(h,f,ginv);
    clock_t end = clock();
    printf("%l\n", (end-start)/CLOCKS_PER_SEC);
#endif
/*
    for(int i = 0; i < 100; i++) {
        printf("%i\t", finv[i]);
    }
  */  
    printf("\ndone\n");
    return 0;
}

