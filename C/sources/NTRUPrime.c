#include "../includes/NTRUPrime.h"
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
    Fq g[P];
    Fq finv[P];
    F3 g3[P];

    // loop breaks upon finding an invertible G, which is guaranteed to happen with high probability
    while (true) {
        // step 1
        makeGPoly(g);
        
        for(int i = 0; i < P; i++) {
            g3[i] = g[i];
        }
        if (R3_inv(ginv, g3) == 0) { // step 1a
            break;
        }
    }
    Fq gq[P];
    for(int i = 0; i < P; i++) {
        gq[i] = g[i];
    }
    // step 2
    Short_random(f);
    // step 3
    Rq3_inv(finv, f); // always works
    
    // step 4
    keyGenMult(finv, g, h);
}

/*
    Function call:
    R: randomly sampled small polynomial in R3, not dependent on even distribution of the coefficients in {-1, 0, 1}
    H: Public key in Rq
    C: Ciphertext in Rq - should be blank to start, and will be overwritten with the output of the encryption function
*/
static void Encrypt(Fq *c, const F3 *r, const Fq *h) {
    Fq h1[P];
    keyGenMult(h, r, h1); 
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
static void Decrypt(F3 *out, const Fq *c, const F3 *f, const F3 *ginv) {
    Fq cf[P], cf3[P];
    F3 e[P], ev[P];
    int valid;

    keyGenMult(f, c, cf);
    Rq_scale3(cf3, cf);
    Rq_reduceR3(e, cf3);
    R3Mult(e, P, ginv, P, ev, P);
    valid = isValidPoly(ev);

    reconstruct(out, valid, ev);
}
