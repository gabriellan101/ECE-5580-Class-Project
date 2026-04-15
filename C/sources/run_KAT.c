#include "KAT_rng.h"
#include <stdio.h>

#define KATNUM 1
#define SEED_LENGTH 10

unsigned char entropy_input[48];
unsigned char seed[KATNUM][48];


int main() {
    printf("started\n");
    FILE                *fp_req, *fp_rsp;
    int                 ret_val;
    int i;
    unsigned char *ct = 0;
    unsigned char *ss = 0;
    unsigned char *ss1 = 0;
    unsigned char *pk = 0;
    unsigned char *sk = 0;

    FILE * seed_file = fopen("seeds.txt", "r");
    char buffer[1028];
    int len = SEED_LENGTH;

    fgets(buffer, len, seed_file);

    printf(buffer);

    for (i=0; i<48; i++)
        entropy_input[i] = i;
    randombytes_init(entropy_input, NULL, 256);

    for (i=0; i<KATNUM; i++)
        randombytes(seed[i], 48);

    for(int i = 0; i < 48; i++) {
        printf("%i\n", seed[0][i]);
    }
}