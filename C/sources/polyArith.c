#include "../includes/polyArith.h"


// returns 0 if succeeded, else -1
// in implementation, polynomials are repeatedly generated until one is found that is invertible
static int R3_inv(F3 *out, const F3 *in)
{
    F3 f[P+1],g[P+1],v[P+1],r[P+1];
    int i,loop,delta, sign, swap, t;
  
    for (i = 0;i < P+1;i++){
        v[i] = 0;
        r[i] = 0;
        if (i < P){
            g[P-1-i] = in[i]; // reverse of the input polynomial
            f[i] = 0;
        }
    } 
    r[0] = 1;
    f[0] = 1;
    f[P-1] = -1;
    f[P] = -1; // Ring modulus X^p - x - 1, inverse must be modulo ring
    g[P] = 0;
    
    delta = 1; 

    // MAIN LOOP: 2P - 1 iterations
    for (loop = 0;loop < 2*P-1;++loop) {
        for (i = P;i > 0;--i) v[i] = v[i-1];
        v[0] = 0;
    
        sign = -g[0]*f[0];
        swap = isNegative(-delta) & isNonZero(g[0]);
        delta ^= swap&(delta^-delta);
        delta += 1;
    
        for (i = 0;i < P+1;i++) {
            t = swap&(f[i]^g[i]); f[i] ^= t; g[i] ^= t;
            t = swap&(v[i]^r[i]); v[i] ^= t; r[i] ^= t;
        }
  
        for (i = 0;i < P+1;i++){
            g[i] = F3_mod(g[i]+sign*f[i]);
            r[i] = F3_mod(r[i]+sign*v[i]);
        } 

        for (i = 0;i < P;i++) g[i] = g[i+1];
        g[P] = 0;
    }
  
    sign = f[0];
    for (i = 0;i < P;i++) out[i] = sign*v[P-1-i];
  
    return isNonZero(delta);
}

// returns 0 if succeeded, else -1
// output is 1/(3*in)
static int Rq3_inv(Fq *out, const F3 *in)
{
    Fq f[P+1],g[P+1],v[P+1],r[P+1];
    int i,loop,delta, swap, t;
    int32_t f0,g0;
    Fq scale;

    for (i = 0;i < P+1;i++){
        v[i] = 0;
        r[i] = 0;
        if (i < P){
            g[P-1-i] = in[i]; // reverse of the input polynomial
            f[i] = 0;
        }
    } 

    r[0] = Fq_recip(3);
    f[0] = 1;
    f[P-1] = -1;
    f[P] = -1; // Ring modulus X^p - x - 1, inverse must be modulo ring
    g[P] = 0;

    delta = 1;

    for (loop = 0;loop < 2*P-1;++loop) {
        for (i = P;i > 0;--i) v[i] = v[i-1];
        v[0] = 0;

        swap = isNegative(-delta) & isNonZero(g[0]);
        delta ^= swap&(delta^-delta);
        delta += 1;

        for (i = 0;i < P+1;i++) {
        t = swap&(f[i]^g[i]); f[i] ^= t; g[i] ^= t;
        t = swap&(v[i]^r[i]); v[i] ^= t; r[i] ^= t;
        }

        f0 = f[0];
        g0 = g[0];
        for (i = 0;i < P+1;i++){
            g[i] = Fq_mod(f0*g[i]-g0*f[i]);
            r[i] = Fq_mod(f0*r[i]-g0*v[i]);
        } 

        for (i = 0;i < P;i++) g[i] = g[i+1];
        g[P] = 0;
    }

    scale = Fq_recip(f[0]);
    for (i = 0;i < P;++i) out[i] = Fq_mod(scale*(int32_t)v[P-1-i]);

    return isNonZero(delta);
}

static void Fq_add(Fq *h,const Fq *f,const Fq *g, int F_scale, int G_scale, int index_adj) {
    for (int i = 0; i < (P + index_adj); i++) {
        h[i] = Fq_mod((int32_t)f[i] * F_scale + g[i] * G_scale);
    }
}

static void Fq_sub(Fq *h,const Fq *f,const Fq *g, int F_scale, int G_scale, int index_adj) {
    for (int i = 0; i < (P + index_adj); i++) {
        h[i] = Fq_mod((int32_t)f[i] * F_scale - g[i] * G_scale);
    }
}

static void F3_add(F3 *h,const F3 *f,const F3 *g, int F_scale, int G_scale, int index_adj) {
    for (int i = 0; i < (P + index_adj); i++) {
        h[i] = F3_mod((int32_t)f[i] * F_scale + g[i] * G_scale);
    }
}

static void F3_sub(F3 *h,const F3 *f,const F3 *g, int F_scale, int G_scale, int index_adj) {
    for (int i = 0; i < (P + index_adj); i++) {
        h[i] = F3_mod((int32_t)f[i] * F_scale - g[i] * G_scale);
    }
}

