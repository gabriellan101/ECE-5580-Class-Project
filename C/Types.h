#ifndef TYPES_H
#define TYPES_H

#include <stdint.h>

#define P 761
#define Q 4591
#define W 286


typedef int16_t Fq;

typedef int8_t F3;

void F3_random(F3 *f);
void F3_random_weightw(F3 *f);

void Fq_random(Fq *f);

// reduction from int32 to int16 or int8 modq or 3 respectively
static inline int16_t Fq_freeze(int32_t x) {
    x %= Q;
    if (x < 0) x += Q;
    return (int16_t)x;
}

static inline int8_t F3_freeze(int32_t x) {
    x %= 3;
    if (x < -1) x += 3;
    else if (x > 1) x -= 3;
    return (int8_t)x;
}




#endif