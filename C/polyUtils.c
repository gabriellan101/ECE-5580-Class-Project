#include "polyUtils.h"

// this function has been tested against the original O(Q) implementation and produces the same output for every nonzero element in Fq
static Fq Fq_recip(Fq x) {
    int32_t result = 1;
    int32_t base = x;
    int exp = Q - 2;

    while (exp > 0) {
        if (exp & 1)
            result = Fq_freeze(base * result);
        base = Fq_freeze(base * base);
        exp >>= 1;
    }
    return result;
}