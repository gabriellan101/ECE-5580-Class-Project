#ifndef NTRUPRIME_H
#define NTRUPRIME_H

#include "Types.h"
#include "polyArith.h"
#include "randombytes.h"
#include "polyUtils.h"
#include "polyMult.h"

/*
This file defines functions used in the inner layer of the streamlined NTRUPrime core
It includes encrypt, decrypt, and key generation
These functions are simple at a high level and share some aspects with the reference implementation,
such as Encrypt(), which only consists of three lines, but the complexity comes from the helper functions.
*/

static void KeyGen(Fq *h, F3 *f, F3 *ginv);
static void Encrypt(Fq *c, const F3 *r, const Fq *h);
static void Decrypt(F3 *out, const Fq *c, const F3 *f, const F3 *ginv);


#endif