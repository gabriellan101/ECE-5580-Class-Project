/*
   PQCgenKAT_kem.c
   Created by Bassham, Lawrence E (Fed) on 8/29/17.
   Copyright © 2017 Bassham, Lawrence E (Fed). All rights reserved.
   + mods from djb: see KATNOTES
*/

char seed1[48] = {0x06,0x15,0x50,0x23,0x4D,0x15,0x8C,0x5E,0xC9,0x55,0x95,0xFE,0x04,0xEF,0x7A,0x25,0x76,0x7F,0x2E,0x24,0xCC,0x2B,0xC4,0x79,0xD0,0x9D,0x86,0xDC,0x9A,0xBC,0xFD,0xE7,0x05,0x6A,0x8C,0x26,0x6F,0x9E,0xF9,0x7E,0xD0,0x85,0x41,0xDB,0xD2,0xE1,0xFF,0xA1};

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "includes/OuterLayer.h"
#define KATNUM 5

/* ── cycle counter ───────────────────────────────────────────────────────── */
static inline uint64_t cpucycles(void) {
    uint32_t lo, hi;
    __asm__ volatile (
        "rdtscp\n\t"
        "mov %%eax, %0\n\t"
        "mov %%edx, %1\n\t"
        "cpuid"
        : "=r"(lo), "=r"(hi)
        :: "%rax", "%rbx", "%rcx", "%rdx"
    );
    return ((uint64_t)hi << 32) | lo;
}

static int cmp_u64(const void *a, const void *b) {
    return (*(uint64_t*)a > *(uint64_t*)b) - (*(uint64_t*)a < *(uint64_t*)b);
}

static uint64_t median_u64(uint64_t *arr, size_t n) {
    qsort(arr, n, sizeof(uint64_t), cmp_u64);
    return arr[n / 2];
}
/* ───────────────────────────────────────────────────────────────────────── */

unsigned char entropy_input[48];
unsigned char seed[KATNUM][48];

#define crypto_kem_CIPHERTEXTBYTES 1039
#define crypto_kem_BYTES 32
#define crypto_kem_PUBLICKEYBYTES 1158
#define crypto_kem_SECRETKEYBYTES 1763

int
main()
{
    int                 ret_val;
    int i;
    unsigned char *ct = 0;
    unsigned char *ss = 0;
    unsigned char *ss1 = 0;
    unsigned char *pk = 0;
    unsigned char *sk = 0;

    /* cycle count accumulators */
    uint64_t cycles_keygen[KATNUM];
    uint64_t cycles_encap[KATNUM];
    uint64_t cycles_decap[KATNUM];
    uint64_t t0;

    FILE * out_file = fopen("kat_out.vals", "w");
    if(out_file == NULL) {
        printf("ERROR: File not created/found\n");
        return -1;
    }

    for (i=0; i<48; i++)
        entropy_input[i] = i;
    randombytes_init(entropy_input, NULL, 256);

    for (i=0; i<KATNUM; i++)
        randombytes(seed[i], 48);

    for (i=0; i<KATNUM; i++) {
        if (!ct) ct = malloc(crypto_kem_CIPHERTEXTBYTES);
        if (!ct) abort();
        if (!ss) ss = malloc(crypto_kem_BYTES);
        if (!ss) abort();
        if (!ss1) ss1 = malloc(crypto_kem_BYTES);
        if (!ss1) abort();
        if (!pk) pk = malloc(crypto_kem_PUBLICKEYBYTES);
        if (!pk) abort();
        if (!sk) sk = malloc(crypto_kem_SECRETKEYBYTES);
        if (!sk) abort();

        randombytes_init(seed[i], NULL, 256);

        fprintf(out_file, "count = %i\n", i);
        fprintf(out_file, "seed = ");
        for(int c = 0; c< 48; c++) {
            fprintf(out_file, "%02hhX", seed[i][c]);
        }
        fprintf(out_file, "\n");

        /* ── KeyGen ── */
        t0 = cpucycles();
        KEM_KeyGen(pk, sk);
        cycles_keygen[i] = cpucycles() - t0;

        fprintf(out_file, "pk = ");
        for(int c = 0; c< crypto_kem_PUBLICKEYBYTES; c++) {
            fprintf(out_file, "%02hhX", pk[c]);
        }
        fprintf(out_file, "\n");

        fprintf(out_file, "sk = ");
        for(int c = 0; c< crypto_kem_SECRETKEYBYTES; c++) {
            fprintf(out_file, "%02hhX", sk[c]);
        }
        fprintf(out_file, "\n");

        /* ── Encap ── */
        t0 = cpucycles();
        Encap(ct, ss, pk);
        cycles_encap[i] = cpucycles() - t0;

        fprintf(out_file, "ct = ");
        for(int c = 0; c< crypto_kem_CIPHERTEXTBYTES; c++) {
            fprintf(out_file, "%02hhX", ct[c]);
        }
        fprintf(out_file, "\n");

        fprintf(out_file, "ss = ");
        for(int c = 0; c< crypto_kem_BYTES; c++) {
            fprintf(out_file, "%02hhX", ss[c]);
        }
        fprintf(out_file, "\n\n");

        /* ── Decap ── */
        t0 = cpucycles();
        Decap(ss1, ct, sk);
        cycles_decap[i] = cpucycles() - t0;

        if ( memcmp(ss, ss1, crypto_kem_BYTES) ) {
            fprintf(stderr, "crypto_kem_dec returned bad 'ss' value\n");
        }
    }

    /* ── Print cycle count summary ── */
    printf("\n=== Cycle Count Summary (median over %d runs) ===\n", KATNUM);
    printf("  KeyGen : %lu cycles\n", (unsigned long)median_u64(cycles_keygen, KATNUM));
    printf("  Encap  : %lu cycles\n", (unsigned long)median_u64(cycles_encap,  KATNUM));
    printf("  Decap  : %lu cycles\n", (unsigned long)median_u64(cycles_decap,  KATNUM));

    printf("\n=== Per-run breakdown ===\n");
    printf("  %4s  %12s  %12s  %12s\n", "run", "keygen", "encap", "decap");
    /* NOTE: median_u64 sorts in-place, so print before calling it, or
       store medians first (as done above) and print the raw arrays here. */
    for (i = 0; i < KATNUM; i++) {
        printf("  %4d  %12lu  %12lu  %12lu\n", i,
               (unsigned long)cycles_keygen[i],
               (unsigned long)cycles_encap[i],
               (unsigned long)cycles_decap[i]);
    }

    fclose(out_file);
    return 0;
}