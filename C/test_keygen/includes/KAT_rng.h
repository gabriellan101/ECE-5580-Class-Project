/**
 * The contents of this file contain code directly adapted from the NTRUPrime reference implmentation submitted to NIST
 * Since NTRU is non-deterministic, any attempt to make an implementation match the Known Answers Test requires using the 
 * exact same rng configuration as the reference implementation. As such, any instance of random number generation used in
 * the reference implementation is copied here, so that the generation of any polynomails will match the KATs
 */

#ifndef KAT_RNG
#define KAT_RNG
#include "Types.h"
#include "polyUtils.h"
#include "rng.h"

/**
 * Generates the bytes used for creating the g polynomial
 */
uint32_t urandom32(void);
/**
 * Generates g, a small polynomial in R
 */
void makeGPoly(F3 * g);


void Short_random(int8_t *out);

void Short_fromlist(int8_t *out,const uint32_t *in);

void crypto_sort_uint32(uint32_t *x,int n);

void minmax(uint32_t *x,uint32_t *y);

#endif