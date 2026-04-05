#ifndef TYPES_H
#define TYPES_H

#include <stdint.h>

#define P 761
#define Q 4591
#define W 286


typedef struct {
  uint16_t coeffs[P];
} Fq;

typedef struct {
  int8_t coeffs[P];
} F3;




#endif