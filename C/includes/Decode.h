#ifndef DECODE_H
#define DECODE_H
#include <stdint.h>
#include "Types.h"

void Decode(const unsigned char *encoded, const uint16_t *M,long long len, uint16_t *decoded);

void Rq_decode(Fq *r,const unsigned char *s);

void R3_decode(F3 *f,const unsigned char *s);

#include "../sources/Decode.c"

#endif