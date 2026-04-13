#ifndef TYPES_H
#define TYPES_H

#include <stdint.h>

#define P 761
#define Q 4591
#define W 286
#define adj ((Q-1)/2)

typedef int16_t Fq;

typedef int8_t F3;

void F3_random(F3 *f);
void F3_random_weightw(F3 *f);

void Fq_random(Fq *f);

// reductions from int32 to int16 or int8 modq or 3 respectively
// These functions relied on a very complex constant time helper modulo function in the reference implementation
// We are however focusing on correctness for now.

/*
    This function assumes that the starting value of x is not close to the top range of int32 to avoid overflow. 
    The "adj" variable shifts x so that the starting range centered around 0 [(-Q-1)/2, (Q-1)/2] maps to [0, Q-1], which allows us to use the modulo operator directly.
*/
static inline int16_t Fq_mod(int32_t x) {
    int32_t r = (x + adj) % Q;
    if (r < 0) r += Q;
    return (int16_t)(r - adj); // typecast down to int16_t to save space
}

/*
This does the same but assumes a starting range of [(-1, 0, 1)] for x, which maps to [0, 1, 2] for the modulo operation. The output is then shifted back to [-1, 0, 1].
*/
static inline int8_t F3_mod(int32_t x) {
    int32_t r = (x + 1) % 3;
    if (r < 0) r += 3;
    return (int8_t)(r - 1); // typecast down to int8_t to save space
}



#include "types.c"
#endif