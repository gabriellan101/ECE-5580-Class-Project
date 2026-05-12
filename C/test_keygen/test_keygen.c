
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
    FILE * out_file = fopen("r_out.vals", "w");
    fprintf(out_file, "Reference\n");


    printf("using ref\n");
    r_KeyGen(h,f,ginv);
#else
    FILE * out_file = fopen("i_out.vals", "w");
    fprintf(out_file, "Implementation\n");



    KeyGen(h,f,ginv);

#endif
    fprintf(out_file, "g = ");
    for(int i = 0; i < 761; i++) {
        fprintf(out_file, "%02x", g[i]);
    }
    fprintf(out_file, "\n");

    fprintf(out_file, "ginv = ");
    for(int i = 0; i < 761; i++) {
        fprintf(out_file, "%02x", ginv[i]);
    }
    fprintf(out_file, "\n");

    fprintf(out_file, "f = ");
    for(int i = 0; i < 761; i++) {
        fprintf(out_file, "%02x", f[i]);
    }
    fprintf(out_file, "\n");

    fprintf(out_file, "finv = ");
    for(int i = 0; i < 761; i++) {
        fprintf(out_file, "%04x", finv[i]);
    }
    fprintf(out_file, "\n");




    fprintf(out_file, "h = ");
    for(int i = 0; i < 761; i++) {
        fprintf(out_file, "%04x", h[i]);
    }
    fprintf(out_file, "\n");
    printf("\ndone\n");
    return 0;
}


