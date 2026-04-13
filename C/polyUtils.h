#ifndef POLYUTILS_H
#define POLYUTILS_H

#include "Types.h"
#include <stdbool.h>

/*
This function is based off Fermat's little theorem, which states that since q is prime, for any nonzero element x in Fq, we have x^(q-1) = 1. (mod Q)
Therefore, x^(q-2) = x^(-1) (mod Q).
*/ 
static Fq Fq_recip(Fq x);

/*
 This function is used to reduce from Rq to R3 in the decryption process.
 This step is necessary because the decryption process involves multiplying the ciphertext
 (which is in Rq) by the secret key (which is in R3), and we need to ensure that the result
 is in R3 to correctly recover the message.
*/
static void RqtoR3(F3 *out,const Fq *in);

/**
 * checks if the given polynomial is small (all coefficient are -1, 0, or 1)
 */
static bool isSmall(int * f, int f_length) {
    for(int i = 0; i < f_length; i++) {
        if(f[i] > 1) 
            return false;
        if(f[i] < -1)
            return false;
    }
    return true;
}
/*
 * Checks if the given polynomial is t-small (over 2t coefficients are -1, 0, 1, where 2t = w)
 */
static bool isTSmall(int * f, int f_length) {
    int bigCount = 0;
    for(int i = 0; i < f_length; i++) {
        if(f[i] > 1) 
            bigCount++;
        if(f[i] < -1)
            bigCount++;
    }
    return (bigCount > W) ? false: true;
}

static int isNonZero(int16_t f){
    return (f != 0) ? -1 : 0;
}

static int isNegative(int16_t f){
    return (f < 0) ? -1 : 0;
}

#include "polyUtils.c"

#endif