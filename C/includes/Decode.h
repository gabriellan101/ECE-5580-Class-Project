#ifndef DECODE_H
#define DECODE_H
#include <stdint.h>
#include <stdlib.h>
#include "Types.h"

void Decode(uint16_t *decoded, const unsigned char *encoded, const uint16_t *M, long long len);

void decodeRq(Fq *r,const unsigned char *s);

void decodeR3(F3 *f,const unsigned char *s);

#endif