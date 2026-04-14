#include "../includes/polyUtils.h"
#define KAT 1
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

#if KAT
static uint32_t urandom32(void)
{
  unsigned char c[4];
  uint32_t out[4];

  randombytes(c,4);
  out[0] = (uint32_t)c[0];
  out[1] = ((uint32_t)c[1])<<8;
  out[2] = ((uint32_t)c[2])<<16;
  out[3] = ((uint32_t)c[3])<<24;
  return out[0]+out[1]+out[2]+out[3];
}

void makeGPoly(Fq * g) {
    for (int i = 0;i < P;++i) g[i] = (((urandom32()&0x3fffffff)*3)>>30)-1;
}
#endif