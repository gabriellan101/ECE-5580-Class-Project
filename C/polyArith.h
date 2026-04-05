#ifndef POLYARITH_H
#define POLYARITH_H

#include "Types.h"

/*
A difference between cryptol and C is the necessity of defining a discrete add and subtract function for polynomials.
Multiplication and inversion still need to be defined.
*/


/*
For these two functions the output is a mask representing whether the inverse exists or not.
If the inverse exists, the output is 0, and if it does not exist, the output is -1.
This is done to avoid branching in the code, which can be a source of side-channel attacks.
*/
static int R3_inv(F3 *out,const F3 *in);
static int Rq_inv(Fq *out,const Fq *in);

#endif