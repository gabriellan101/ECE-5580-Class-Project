#ifndef ENCODE_H
#define ENCODE_H
#include <stdint.h>
#include "Types.h"

// encode.h
    unsigned char *EncodeRecursive(unsigned char *restrict encoded, const uint16_t *restrict R, const uint16_t *restrict M, long long len);

    void Encode(unsigned char *encoded, const uint16_t *R, const uint16_t *M, long long len);

void encodeR3(unsigned char *s,const F3 *f);

void encodeRq(unsigned char *s,const Fq *r);

#endif