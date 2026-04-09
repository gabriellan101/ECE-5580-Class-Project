#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "Types.h"
#include "polyUtils.h"
//#include "polyArith.h"

#define Q 4591
#define adj ((Q-1)/2)

// testing definitions for individual files - comment out which aren't being tested
#define POLYUTILS

typedef int16_t Fq;

#ifdef POLYUTILS

// Original O(Q) implementation
static Fq Fq_recip_original(Fq a1) {
    int i = 1;
    Fq ai = a1;
    while (i < Q - 2) {
        ai = Fq_freeze(a1 * (int32_t)ai);
        i += 1;
    }
    return ai;
}

#endif

#ifdef POLYARITHTEST



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
        Fq product = Fq_freeze(a * (int32_t)orig);
        if (product != 1) {
            printf("INVERSE WRONG at a=%d: recip=%d, a*recip=%d\n", a, orig, product);
            failed++;
            continue;
        }

        passed++;
    }

    printf("\npolyUtils.c Results: %d passed, %d failed\n", passed, failed);
    return failed > 0 ? 1 : 0;
    
    #endif


}


