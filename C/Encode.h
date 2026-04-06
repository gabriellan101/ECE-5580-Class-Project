#ifndef ENCODE_H
#define ENCODE_H
#include <stdint.h>

void Encode(long long len, const uint16_t *R, const uint16_t *Moduli, unsigned char *out);