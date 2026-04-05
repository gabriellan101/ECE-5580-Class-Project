#ifndef POLYARITH_H
#define POLYARITH_H

#include "types.h"

// f + g = h in R3, mod3
void F3_add(F3 *h,const F3 *f,const F3 *g);
// f - g = h in R3, mod3
void F3_sub(F3 *h,const F3 *f,const F3 *g);

// same in Rq, modq
void Fq_add(Fq *h,const Fq *f,const Fq *g);
void Fq_sub(Fq *h,const Fq *f,const Fq *g);

static int R3_inv(F3 *out,const F3 *in);
static int Rq_inv(Fq *out,const Fq *in);

#endif