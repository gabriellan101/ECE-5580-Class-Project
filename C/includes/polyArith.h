#ifndef POLYARITH_H
#define POLYARITH_H

#include "Types.h"
#include "polyUtils.h"

/*
A difference between cryptol and C is the necessity of defining a discrete add and subtract function for polynomials.
Multiplication and inversion still need to be defined.
*/

// Multiplication of Fq and F3 polynomials
// f is in Rq, g is in R3, h (output) is in Rq
static void FqF3_mult(Fq *h,const Fq *f,const F3 *g);

// Multiplication of F3 and F3 polynomials
// f and g are in R3, h (output) is in R3
static void F3F3_mult(F3 *h,const F3 *f,const F3 *g);

/*
For these two functions the output is a mask representing whether the inverse exists or not.
If the inverse exists, the output is 0, and if it does not exist, the output is -1.
This is done to avoid branching in the code, which can be a source of side-channel attacks.
*/
static int R3_inv(F3 *out,const F3 *in);
static int Rq3_inv(Fq *out,const F3 *in);

// adding and subtracting polynomials in Rq and R3
// index_adj is used to specify the length of the polynomials, which is necessary for the inverse function which uses polynomials of length P+1
// F_scale and G_scale are used to specify the scaling of the input polynomials, which is necessary for inverse and for the decryption process 
// where we need to add a scaled version of the ciphertext to a scaled version of the secret key
static void Fq_add(Fq *h,const Fq *f,const Fq *g, int F_scale, int G_scale, int index_adj);
static void Fq_sub(Fq *h,const Fq *f,const Fq *g, int F_scale, int G_scale, int index_adj);

static void F3_add(F3 *h,const F3 *f,const F3 *g, int F_scale, int G_scale, int index_adj);
static void F3_sub(F3 *h,const F3 *f,const F3 *g, int F_scale, int G_scale, int index_adj);


#include "../sources/polyArith.c"

#endif