/*
gcc test_rng.c -o test.exe -I"C:/Program Files/OpenSSL-Win64/include" -I"C:/Program Files/OpenSSL-Win64/bin"

gcc test_rng.c rng.c -o test.exe -I"C:/Program Files/OpenSSL-Win64/include" -L"C:/Program Files/OpenSSL-Win64/bin" -llibcrypto-3-x64
 */

//#define _GNU_SOURCE 
#include "includes/rng.h"
#include <stdio.h>
#include <stdint.h>
#define KATNUM 1
#define SEED_LENGTH

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
    int len = 10;

    fgets(buffer, len, seed_file);

    printf(buffer);

    for (i=0; i<48; i++)
        entropy_input[i] = i;
    randombytes_init(entropy_input, NULL, 256);

    for (i=0; i<KATNUM; i++)
        randombytes(seed[i], 48);
 

}