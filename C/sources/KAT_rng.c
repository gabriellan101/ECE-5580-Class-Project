#include "../includes/KAT_rng.h"
#include "../includes/rng.h"
#include "rng.c"


uint32_t urandom32(void)
{
  unsigned char c[4];
  uint32_t out[4];

  randombytes(c,4);
  out[0] = (uint32_t)c[0];
  out[1] = ((uint32_t)c[1])<<8;
  out[2] = ((uint32_t)c[2])<<16;
  out[3] = ((uint32_t)c[3])<<24;
  return out[0]+out[1]+out[2]+out[3];
}

void makeGPoly(F3 * g) {
    for (int i = 0;i < P;++i) g[i] = (((urandom32()&0x3fffffff)*3)>>30)-1;
}


void Short_random(int8_t *out)
{
  uint32_t L[P];
  int i;

  for (i = 0;i < P;++i) L[i] = urandom32();
  Short_fromlist(out,L);
}

void Short_fromlist(int8_t *out,const uint32_t *in)
{
  uint32_t L[P];
  int i;

  for (i = 0;i < W;++i) L[i] = in[i]&(uint32_t)-2;
  for (i = W;i < P;++i) L[i] = (in[i]&(uint32_t)-3)|1;
  crypto_sort_uint32(L,P);
  for (i = 0;i < P;++i) out[i] = (L[i]&3)-1;
}

void crypto_sort_uint32(uint32_t *x,int n)
{
  int top,p,q,i;

  if (n < 2) return;
  top = 1;
  while (top < n - top) top += top;

  for (p = top;p > 0;p >>= 1) {
    for (i = 0;i < n - p;++i)
      if (!(i & p))
        minmax(x + i,x + i + p);
    for (q = top;q > p;q >>= 1)
      for (i = 0;i < n - q;++i)
        if (!(i & p))
          minmax(x + i + p,x + i + q);
  }
}

void minmax(uint32_t *x,uint32_t *y)
{
  uint32_t xi = *x;
  uint32_t yi = *y;
  uint32_t xy = xi ^ yi;
  uint32_t c = yi - xi;
  c ^= xy & (c ^ yi ^ 0x80000000);
  c >>= 31;
  c = -c;
  c &= xy;
  *x = xi ^ c;
  *y = yi ^ c;
}
