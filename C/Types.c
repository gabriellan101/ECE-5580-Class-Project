#include "Types.h"

static void Rq_reduceR3(F3 *out, const Fq *in) {
    for (int i = 0; i < P; i++) {
        out[i] = F3_mod(in[i]);
    }
}
