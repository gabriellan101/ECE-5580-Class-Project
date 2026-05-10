#ifndef OUTERLAYER_H
#define OUTERLAYER_H

#include "NTRUPrime.h"
#include "Types.h"
#include "Encode.h"
#include "Decode.h"
#include "polyArith.h"
#include "polyMult.h"
#include "polyMult.h"
#include "polyUtils.h"
#include <stdlib.h>
#include <openssl/sha.h>
#include <stddef.h>
#include <string.h>
#include "../includes/rng.h"

/*
This file defines the outer layer of NTRUPrime, which interfaces with real world applications. The secret key is sent
out of band in practice. This file also includes definitions of hash functions used in the reference implementation to allow
comparisons with the KATs. We chose to do this instead of finding an external library for hash functions for this reason.

Note - while working on this code I realized that Tame Impala sounds like Stewie Griffin
*/

// This function handles pk and sk generation while 
static void OuterKeyGen(unsigned char *pk, unsigned char *sk);

// The following function calls the above function in the NTRUPrime process and the outputs are used for encap and decap
// It is a wrapper on top of OuterKeyGen() that intefaces with the hash functions
static void KEM_keygen(unsigned char *pk, unsigned char *sk);

static void OuterEncrypt(unsigned char *CT, const F3 r, const unsigned char *pk);

static void OuterDecrypt(F3 *r, const unsigned char *CT, const unsigned char *sk);

static void Encap(unsigned char *CT, unsigned char *k, const unsigned char *pk);

static void Decap(unsigned char *k, const unsigned char *CT, const unsigned char *sk);

static int confirm(const unsigned char *ct, const unsigned char *ct2);

static void Rounded_encode(unsigned char *s,const Fq *r);

static void Rounded_decode(Fq *r,const unsigned char *s);

/*
Hash functions from reference implementation below this line
*/

// these definitions copied from the parameter definitions in the reference implemention
#define Ciphertexts_bytes 1007
#define Hash_bytes 32
#define Confirm_bytes 32
#define Inputs_bytes ((P+3)/4)
#define Small_bytes Inputs_bytes
#define SK_bytes (2*Small_bytes)
#define Rq_bytes 1158
#define PK_bytes Rq_bytes

extern int crypto_hash_sha512(unsigned char *,const unsigned char *,unsigned long long);

// I had to edit this function to remove errors
static void Hash_prefix(unsigned char *out,int b,const unsigned char *in,int inlen);

/* h = HashConfirm(r,pk,cache); cache is Hash4(pk) */
static void HashConfirm(unsigned char *h,const unsigned char *r,const unsigned char *pk,const unsigned char *cache);

/* ----- session-key hash */

/* k = HashSession(b,y,z) */
static void HashSession(unsigned char *k,int b,const unsigned char *y,const unsigned char *z);

#include "../sources/OuterLayer.c"

#endif