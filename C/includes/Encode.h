#ifndef ENCODE_H
#define ENCODE_H
#include <stdint.h>
#include "Types.h"

void Encode(long long len, const uint16_t *R, const uint16_t *Moduli, unsigned char *out);

void encodeR3(unsigned char *s,const F3 *f);

void encodeRq(unsigned char *s,const Fq *r);

#include "../sources/Encode.c"

#endif