#ifndef POLYUTILS_H
#define POLYUTILS_H

#include "Types.h"


/*
This function is based off Fermat's little theorem, which states that since q is prime, for any nonzero element x in Fq, we have x^(q-1) = 1. (mod Q)
Therefore, x^(q-2) = x^(-1) (mod Q).
*/ 
static Fq Fq_recip(Fq x);

/*
 This function is used to reduce from Rq to R3 in the decryption process.
 This step is necessary because the decryption process involves multiplying the ciphertext
 (which is in Rq) by the secret key (which is in R3), and we need to ensure that the result
 is in R3 to correctly recover the message.
*/
static void RqtoR3(F3 *out,const Fq *in);



#include "polyUtils.c"

#endif