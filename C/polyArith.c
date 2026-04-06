#include "polyArith.h"


// returns 0 if succeeded, else -1
static int R3_inv(F3 *out, const F3 *in)
{
    F3 f[P+1],g[P+1],v[P+1],r[P+1];
    int i,loop,delta;
    int sign,swap,t;
  
    for (i = 0;i < P+1;++i) v[i] = 0;
    for (i = 0;i < P+1;++i) r[i] = 0;
    r[0] = 1;
    for (i = 0;i < P;++i) f[i] = 0;
    f[0] = 1; f[P-1] = f[P] = -1;
    for (i = 0;i < P;++i) g[P-1-i] = in[i];
    g[P] = 0;
    
    delta = 1; 

    for (loop = 0;loop < 2*P-1;++loop) {
        for (i = P;i > 0;--i) v[i] = v[i-1];
        v[0] = 0;
    
        sign = -g[0]*f[0];
        swap = int16_negative_mask(-delta) & int16_nonzero_mask(g[0]);
        delta ^= swap&(delta^-delta);
        delta += 1;
    
        for (i = 0;i < P+1;++i) {
            t = swap&(f[i]^g[i]); f[i] ^= t; g[i] ^= t;
            t = swap&(v[i]^r[i]); v[i] ^= t; r[i] ^= t;
        }
  
        for (i = 0;i < P+1;++i) g[i] = F3_freeze(g[i]+sign*f[i]);
        for (i = 0;i < P+1;++i) r[i] = F3_freeze(r[i]+sign*v[i]);

        for (i = 0;i < P;++i) g[i] = g[i+1];
        g[P] = 0;
    }
  
    sign = f[0];
    for (i = 0;i < P;++i) out[i] = sign*v[P-1-i];
  
    return int16_nonzero_mask(delta);
}

static int Rq_inv(Fq *out, const Fq *in)
{
    Fq f[p+1],g[p+1],v[p+1],r[p+1];
    int i,loop,delta;
    int swap,t;
    int32 f0,g0;
    Fq scale;

    for (i = 0;i < p+1;++i) v[i] = 0;
    for (i = 0;i < p+1;++i) r[i] = 0;
    r[0] = Fq_recip(3);
    for (i = 0;i < p;++i) f[i] = 0;
    f[0] = 1; f[p-1] = f[p] = -1;
    for (i = 0;i < p;++i) g[p-1-i] = in[i];
    g[p] = 0;

    delta = 1;

    for (loop = 0;loop < 2*p-1;++loop) {
        for (i = p;i > 0;--i) v[i] = v[i-1];
        v[0] = 0;

        swap = int16_negative_mask(-delta) & int16_nonzero_mask(g[0]);
        delta ^= swap&(delta^-delta);
        delta += 1;

        for (i = 0;i < p+1;++i) {
        t = swap&(f[i]^g[i]); f[i] ^= t; g[i] ^= t;
        t = swap&(v[i]^r[i]); v[i] ^= t; r[i] ^= t;
        }

        f0 = f[0];
        g0 = g[0];
        for (i = 0;i < p+1;++i) g[i] = Fq_freeze(f0*g[i]-g0*f[i]);
        for (i = 0;i < p+1;++i) r[i] = Fq_freeze(f0*r[i]-g0*v[i]);

        for (i = 0;i < p;++i) g[i] = g[i+1];
        g[p] = 0;
    }

    scale = Fq_recip(f[0]);
    for (i = 0;i < p;++i) out[i] = Fq_freeze(scale*(int32_t)v[p-1-i]);

    return int16_nonzero_mask(delta);
}