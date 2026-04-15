#include "includes/NTRUPrime.h"
#include "../includes/KAT_rng.h"

/*
Steps for key generation:
1: Generate a random small polynomial G in R3 and check if it is invertible in R3
    1a: if G is not invertible, repeat step 1
2: Generate a random small polynomial F in R3
3: Find the inverse of F in Rq, which always exists
4: Compute H = F^-1 * G in Rq
5: (implicit based on function definition) Output public key H and secret key (F, G^-1)
*/
static void KeyGen(Fq *h, F3 *f, F3 *ginv) {
    F3 g[P];
    Fq finv[P];
    

    // loop breaks upon finding an invertible G, which is guaranteed to happen with high probability
    while (true) {
        // step 1
        makeGPoly(g);
        if (R3_inv(ginv, g) == 0) { // step 1a
            break;
        }
    }

    // step 2
    Short_random(f);
    // step 3
    Rq3_inv(finv, f); // always works
    
    // step 4
    keyGenMult(finv, g, h);    // replace with our mult function instead of placeholder
}

/*
    Function call:
    R: randomly sampled small polynomial in R3, not dependent on even distribution of the coefficients in {-1, 0, 1}
    H: Public key in Rq
    C: Ciphertext in Rq - should be blank to start, and will be overwritten with the output of the encryption function
*/
static void Encrypt(Fq *c, const F3 *r, const Fq *h) {
    Fq h1[P];
    FqF3_mult(h1, h, r); // replace with our mult funciton
    roundR3(c, h1);
}

/*
    Steps for decryption:
    Inputs are ciphertext, G inverse (called 'v' in spec), and f
    1: Compute 3*f*c in Rq
    2: Coefficients of 3fc should be between +/- (q-1)/2 - reduce mod 3 (rounding in encryption important for this step)
    3: Multiply past result by Ginv in R/3
    4: construct small polynomial 
*/
static void Decrypt(F3 *r, const Fq *c, const F3 *ginv, const F3 f) {
    Fq cf[P], cf3[P];
    F3 e[P], ev[P];
    int valid;

    FqF3_mult(cf, c, f); // replace
    Rq_scale3(cf3, cf);
    Rq_reduceR3(e, cf3);
    F3F3_mult(ev, e, ginv);
    valid = isValidPoly(ev);

    reconstruct(r, valid, ev);
}
