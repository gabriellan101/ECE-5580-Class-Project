#include "../includes/polyUtils.h"

// this function has been tested against the original O(Q) implementation and produces the same output for every nonzero element in Fq
static Fq Fq_recip(Fq x) {
    int32_t result = 1;
    int32_t base = x;
    int exp = Q - 2;

    while (exp > 0) {
        if (exp & 1)
            result = Fq_mod(base * result);
        base = Fq_mod(base * base);
        exp >>= 1;
    }
    return result;
}

static void RqtoR3(F3 *out, const Fq *in) {
    for (int i = 0; i < P; i++) {
        out[i] = F3_mod(in[i]);
    }
}