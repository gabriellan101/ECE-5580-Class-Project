#include "../includes/Types.h"
#include "../includes/polyArith.h"
#include "../includes/randombytes.h"
#include "../includes/polyUtils.h"
#include "../includes/polyMult.h"
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
    FqF3_mult(h, finv, g);    
}

/*
    Function inputs:

*/
static void Encrypt(Fq *c, const F3 *msg, const Fq *h) {
    // TODO: implement encryption function
}


int main(int argc, char *argv[])
{

    

    return 0;
}