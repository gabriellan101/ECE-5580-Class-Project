#include "polyArith.h"
#include "Types.h"

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