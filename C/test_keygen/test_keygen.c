
#define USE_REF 0


#include "includes/rng.h"
#if USE_REF
#include "ref_keygen.c"
#endif
#include <stdio.h>
#if !USE_REF
#include "includes/NTRUPrime.h"
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
    printf("using impl\n");
    KeyGen(h,f,ginv);
#endif

    for(int i = 0; i < 100; i++) {
        printf("%i\t", finv[i]);
    }
    
    printf("\ndone\n");
    return 0;
}

