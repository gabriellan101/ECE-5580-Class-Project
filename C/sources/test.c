#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "../includes/Types.h"
#include "../includes/polyUtils.h"
#include "../includes/polyArith.h"
#include <string.h>
#include <time.h>

#define Q 4591
#define adj ((Q-1)/2)

// testing definitions for individual files - comment out which aren't being tested
//#define POLYUTILS
#define POLYARITHTEST

typedef int16_t Fq;



// Original O(Q) implementation
static Fq Fq_recip_original(Fq a1) {
    int i = 1;
    Fq ai = a1;
    while (i < Q - 2) {
        ai = Fq_mod(a1 * (int32_t)ai);
        i += 1;
    }
    return ai;
}



#ifdef POLYARITHTEST
// helper functions from reference implementation for testing purposes
static int int16_negative_mask(int16_t x)
{
  uint16_t u = x;
  u >>= 15;
  return -(int) u;
  /* alternative with gcc -fwrapv: */
  /* x>>15 compiles to CPU's arithmetic right shift */
}

static int int16_nonzero_mask(int16_t x)
{
  uint16_t u = x; /* 0, else 1...65535 */
  uint32_t v = u; /* 0, else 1...65535 */
  v = -v; /* 0, else 2^32-65535...2^32-1 */
  v >>= 31; /* 0, else 1 */
  return -v; /* 0, else -1 */
}

// polynomial inverse functions from reference implementation for testing purposes
/* returns 0 if recip succeeded; else -1 */
static int R3_recip(F3 *out,const F3 *in)
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
  
    for (i = 0;i < P+1;++i) g[i] = F3_mod(g[i]+sign*f[i]);
    for (i = 0;i < P+1;++i) r[i] = F3_mod(r[i]+sign*v[i]);

    for (i = 0;i < P;++i) g[i] = g[i+1];
    g[P] = 0;
  }
  
  sign = f[0];
  for (i = 0;i < P;++i) out[i] = sign*v[P-1-i];
  
  return int16_nonzero_mask(delta);
}

/* out = 1/(3*in) in Rq */
/* returns 0 if recip succeeded; else -1 */
static int Rq_recip3(Fq *out,const F3 *in)
{ 
  Fq f[P+1],g[P+1],v[P+1],r[P+1];
  int i,loop,delta;
  int swap,t;
  int32_t f0,g0;
  Fq scale;

  for (i = 0;i < P+1;++i) v[i] = 0;
  for (i = 0;i < P+1;++i) r[i] = 0;
  r[0] = Fq_recip_original(3);
  for (i = 0;i < P;++i) f[i] = 0;
  f[0] = 1; f[P-1] = f[P] = -1;
  for (i = 0;i < P;++i) g[P-1-i] = in[i];
  g[P] = 0;

  delta = 1;

  for (loop = 0;loop < 2*P-1;++loop) {
    for (i = P;i > 0;--i) v[i] = v[i-1];
    v[0] = 0;

    swap = int16_negative_mask(-delta) & int16_nonzero_mask(g[0]);
    delta ^= swap&(delta^-delta);
    delta += 1;

    for (i = 0;i < P+1;++i) {
      t = swap&(f[i]^g[i]); f[i] ^= t; g[i] ^= t;
      t = swap&(v[i]^r[i]); v[i] ^= t; r[i] ^= t;
    }

    f0 = f[0];
    g0 = g[0];
    for (i = 0;i < P+1;++i) g[i] = Fq_mod(f0*g[i]-g0*f[i]);
    for (i = 0;i < P+1;++i) r[i] = Fq_mod(f0*r[i]-g0*v[i]);

    for (i = 0;i < P;++i) g[i] = g[i+1];
    g[P] = 0;
  }

  scale = Fq_recip_original(f[0]);
  for (i = 0;i < P;++i) out[i] = Fq_mod(scale*(int32_t)v[P-1-i]);

  return int16_nonzero_mask(delta);
}

// below are helper test functions for the above implementations
/* Random ternary polynomial with exactly w_pos +1s and w_neg -1s */
static void rand_ternary(F3 *p, int w_pos, int w_neg)
{
    int i;
    memset(p, 0, P * sizeof(F3));

    /* Fisher-Yates partial shuffle to place non-zero coefficients */
    int indices[P];
    for (i = 0; i < P; ++i) indices[i] = i;

    for (i = 0; i < w_pos + w_neg; ++i) {
        int j = i + rand() % (P - i);
        int tmp = indices[i]; indices[i] = indices[j]; indices[j] = tmp;
    }
    for (i = 0;        i < w_pos;          ++i) p[indices[i]] =  1;
    for (i = w_pos;    i < w_pos + w_neg;  ++i) p[indices[i]] = -1;
}

/*
 * R3_recip test
 * Strategy:
 *   1. Generate a random ternary polynomial.
 *   2. Call both implementations.
 *   3. If return codes differ → FAIL.
 *   4. If both succeed, compare every coefficient of the output.
 */
static int test_R3_recip(int n_random, int n_invertible)
{
    int passed = 0, failed = 0, skipped = 0;
    F3 in[P], ref_out[P], my_out[P];
    int test_num = 0;

    /* ── fixed / edge-case inputs ── */
    struct { const char *label; F3 val[P]; } edge[] = {
        /* constant 1  → always invertible */
        { "const +1" },
        /* constant -1 → always invertible */
        { "const -1" },
    };
    memset(edge[0].val, 0, sizeof edge[0].val); edge[0].val[0] =  1;
    memset(edge[1].val, 0, sizeof edge[1].val); edge[1].val[0] = -1;

    for (int e = 0; e < (int)(sizeof edge / sizeof edge[0]); ++e) {
        memcpy(in, edge[e].val, sizeof in);
        int r_ref = R3_recip(ref_out, in);
        int r_my  = R3_inv(my_out, in);
        ++test_num;

        if (r_ref != r_my) {
            printf("  [R3_recip] edge '%s': return code mismatch "
                   "(ref=%d my=%d)\n", edge[e].label, r_ref, r_my);
            ++failed;
        } else if (r_ref == 0 && memcmp(ref_out, my_out, P * sizeof(F3)) != 0) {
            printf("  [R3_recip] edge '%s': output mismatch\n", edge[e].label);
            for (int i = 0; i < P; ++i) {
                if (ref_out[i] != my_out[i]) {
                    printf("    first diff at index %d: ref=%d my=%d\n",
                           i, (int)ref_out[i], (int)my_out[i]);
                    break;
                }
            }
            ++failed;
        } else {
            ++passed;
        }
    }

    /* ── random ternary polynomials (may or may not be invertible) ── */
    for (int t = 0; t < n_random; ++t, ++test_num) {
        rand_ternary(in, 143, 143); /* 143 + 143 = 286, sntrup761 weight */

        int r_ref = R3_recip(ref_out, in);
        int r_my  = R3_inv(my_out, in);

        if (r_ref != r_my) {
            printf("  [R3_recip] random %d: return code mismatch "
                   "(ref=%d my=%d)\n", t, r_ref, r_my);
            ++failed; continue;
        }
        if (r_ref != 0) { ++skipped; continue; } /* not invertible, skip cmp */

        if (memcmp(ref_out, my_out, P * sizeof(F3)) != 0) {
            printf("  [R3_recip] random %d: output mismatch\n", t);
            for (int i = 0; i < P; ++i) {
                if (ref_out[i] != my_out[i]) {
                    printf("    first diff at index %d: ref=%d my=%d\n",
                           i, (int)ref_out[i], (int)my_out[i]);
                    break;
                }
            }
            ++failed;
        } else {
            ++passed;
        }
    }

    /*
     * ── force n_invertible invertible cases ──
     * Retry until we get a polynomial with a valid inverse.
     * (Most weight-286 ternary polys over F3[x]/(x^761-x-1) are invertible.)
     */
    for (int t = 0; t < n_invertible; ++t) {
        int attempts = 0;
        while (1) {
            rand_ternary(in, 143, 143); /* 143 + 143 = 286, sntrup761 weight */
            int r_ref = R3_recip(ref_out, in);
            if (r_ref == 0) break;          /* found an invertible one */
            if (++attempts > 1000) {
                printf("  [R3_recip] could not find invertible poly after "
                       "1000 tries (test %d)\n", t);
                break;
            }
        }
        ++test_num;

        int r_my = R3_inv(my_out, in);
        if (r_my != 0) {
            printf("  [R3_recip] invertible %d: my impl returned failure "
                   "but ref succeeded\n", t);
            ++failed; continue;
        }
        if (memcmp(ref_out, my_out, P * sizeof(F3)) != 0) {
            printf("  [R3_recip] invertible %d: output mismatch\n", t);
            for (int i = 0; i < P; ++i) {
                if (ref_out[i] != my_out[i]) {
                    printf("    first diff at index %d: ref=%d my=%d\n",
                           i, (int)ref_out[i], (int)my_out[i]);
                    break;
                }
            }
            ++failed;
        } else {
            ++passed;
        }
    }

    printf("\nR3_recip  : %d passed, %d failed, %d skipped (not invertible)\n",
           passed, failed, skipped);
    return failed;
}

/*
 * Rq_recip3 test  (same structure, Fq output)
 */
static int test_Rq_recip3(int n_random, int n_invertible)
{
    int passed = 0, failed = 0, skipped = 0;
    F3 in[P];
    Fq ref_out[P], my_out[P];

    /* ── random tests ── */
    for (int t = 0; t < n_random; ++t) {
        rand_ternary(in, 143, 143); /* 143 + 143 = 286, sntrup761 weight */

        int r_ref = Rq_recip3(ref_out, in);
        int r_my  = Rq3_inv(my_out, in);

        if (r_ref != r_my) {
            printf("  [Rq_recip3] random %d: return code mismatch "
                   "(ref=%d my=%d)\n", t, r_ref, r_my);
            ++failed; continue;
        }
        if (r_ref != 0) { ++skipped; continue; }

        if (memcmp(ref_out, my_out, P * sizeof(Fq)) != 0) {
            printf("  [Rq_recip3] random %d: output mismatch\n", t);
            for (int i = 0; i < P; ++i) {
                if (ref_out[i] != my_out[i]) {
                    printf("    first diff at index %d: ref=%d my=%d\n",
                           i, (int)ref_out[i], (int)my_out[i]);
                    break;
                }
            }
            ++failed;
        } else {
            ++passed;
        }
    }

    /* ── forced invertible tests ── */
    for (int t = 0; t < n_invertible; ++t) {
        int attempts = 0;
        while (1) {
            rand_ternary(in, 143, 143); /* 143 + 143 = 286, sntrup761 weight */
            int r_ref = Rq_recip3(ref_out, in);
            if (r_ref == 0) break;
            if (++attempts > 1000) {
                printf("  [Rq_recip3] could not find invertible poly "
                       "after 1000 tries\n");
                break;
            }
        }

        int r_my = Rq3_inv(my_out, in);
        if (r_my != 0) {
            printf("  [Rq_recip3] invertible %d: my impl returned failure "
                   "but ref succeeded\n", t);
            ++failed; continue;
        }
        if (memcmp(ref_out, my_out, P * sizeof(Fq)) != 0) {
            printf("  [Rq_recip3] invertible %d: output mismatch\n", t);
            for (int i = 0; i < P; ++i) {
                if (ref_out[i] != my_out[i]) {
                    printf("    first diff at index %d: ref=%d my=%d\n",
                           i, (int)ref_out[i], (int)my_out[i]);
                    break;
                }
            }
            ++failed;
        } else {
            ++passed;
        }
    }

    printf("Rq_recip3 : %d passed, %d failed, %d skipped (not invertible)\n",
           passed, failed, skipped);
    return failed;
}

#endif


int main() {
    #ifdef POLYUTILS
    int passed = 0, failed = 0;

    // Test every nonzero element in Fq
    for (int a = -(adj); a <= adj; a++) {
        if (a == 0) continue;

        Fq orig = Fq_recip_original((Fq)a);
        Fq fast = Fq_recip((Fq)a);

        // Check both implementations agree
        if (orig != fast) {
            printf("MISMATCH at a=%d: original=%d fast=%d\n", a, orig, fast);
            failed++;
            continue;
        }

        // Check a * a^-1 = 1 (mod Q)
        Fq product = Fq_mod(a * (int32_t)orig);
        if (product != 1) {
            printf("INVERSE WRONG at a=%d: recip=%d, a*recip=%d\n", a, orig, product);
            failed++;
            continue;
        }

        passed++;
    }

    printf("\npolyUtils.c Fq_recip() Results: %d passed, %d failed\n", passed, failed);
    return failed > 0 ? 1 : 0;
    
    #endif

    #ifdef POLYARITHTEST

    srand((unsigned)time(NULL));

    printf("=== sntrup761 polynomial inverse tests ===\n\n");

    int fail = 0;
    fail += test_R3_recip  (/* random */ 200, /* forced invertible */ 50);
    fail += test_Rq_recip3 (/* random */ 200, /* forced invertible */ 50);

    printf("\n%s\n", fail == 0 ? "ALL TESTS PASSED" : "SOME TESTS FAILED");
    return fail ? 1 : 0;

    #endif

}


