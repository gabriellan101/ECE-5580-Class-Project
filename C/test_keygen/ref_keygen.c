#include <stdint.h>
#include "includes/rng.h"

#ifndef Fq
#define Fq int16_t
#endif

typedef uint8_t uint8;
typedef int8_t int8;

typedef uint16_t uint16;
typedef int16_t int16;

typedef uint32_t uint32;
typedef int32_t int32;

typedef uint64_t uint64;
typedef int64_t int64;
typedef int8 small;

#define p (761)
#define q (4591)
#define q12 ((q-1)/2)
#define w 286

static uint32 r_urandom32(void)
{
  unsigned char c[4];
  uint32 out[4];

  randombytes(c,4);
  out[0] = (uint32)c[0];
  out[1] = ((uint32)c[1])<<8;
  out[2] = ((uint32)c[2])<<16;
  out[3] = ((uint32)c[3])<<24;
  return out[0]+out[1]+out[2]+out[3];
}

 void r_Small_random(small *out)
{
  int i;

  for (i = 0;i < p;++i) out[i] = (((r_urandom32()&0x3fffffff)*3)>>30)-1;
}

 int r_int16_negative_mask(int16 x)
{
  uint16 u = x;
  u >>= 15;
  return -(int) u;
  /* alternative with gcc -fwrapv: */
  /* x>>15 compiles to CPU's arithmetic right shift */
}

 int r_int16_nonzero_mask(int16 x)
{
  uint16 u = x; /* 0, else 1...65535 */
  uint32 v = u; /* 0, else 1...65535 */
  v = -v; /* 0, else 2^32-65535...2^32-1 */
  v >>= 31; /* 0, else 1 */
  return -v; /* 0, else -1 */
}


void r_uint32_divmod_uint14(uint32 *q_in,uint16 *r,uint32 x,uint16 m)
{
  uint32 v = 0x80000000;
  uint32 qpart;
  uint32 mask;

  v /= m;
  *q_in = 0;

  qpart = (x*(uint64)v)>>31;

  x -= qpart*m; *q_in += qpart;
  /* x <= 49146 */

  qpart = (x*(uint64)v)>>31;

  x -= qpart*m; *q_in += qpart;
  x -= m; *q_in += 1;
  mask = -(x>>31);
  x += mask&(uint32)m; *q_in += mask;
  *r = x;
}

void r_int32_divmod_uint14(int32 *q_in,uint16 *r,int32 x,uint16 m)
{
  uint32 uq,uq2;
  uint16 ur,ur2;
  uint32 mask;

  r_uint32_divmod_uint14(&uq,&ur,0x80000000+(uint32)x,m);
  r_uint32_divmod_uint14(&uq2,&ur2,0x80000000,m);
  ur -= ur2; uq -= uq2;
  mask = -(uint32)(ur>>15);
  ur += mask&m; uq += mask;
  *r = ur; *q_in = uq;
}

uint16 r_int32_mod_uint14(int32 x,uint16 m)
{
  int32 q_in;
  uint16 r;
  r_int32_divmod_uint14(&q_in,&r,x,m);
  return r;
}

 small r_F3_freeze(int16 x)
{
  return r_int32_mod_uint14(x+1,3)-1;
}

 int r_R3_recip(small *out,const small *in)
{ 
  small f[p+1],g[p+1],v[p+1],r[p+1];
  int i,loop,delta;
  int sign,swap,t;
  
  for (i = 0;i < p+1;++i) v[i] = 0;
  for (i = 0;i < p+1;++i) r[i] = 0;
  r[0] = 1;
  for (i = 0;i < p;++i) f[i] = 0;
  f[0] = 1; f[p-1] = f[p] = -1;
  for (i = 0;i < p;++i) g[p-1-i] = in[i];
  g[p] = 0;
    
  delta = 1; 

  for (loop = 0;loop < 2*p-1;++loop) {
    for (i = p;i > 0;--i) v[i] = v[i-1];
    v[0] = 0;
    
    sign = -g[0]*f[0];
    swap = r_int16_negative_mask(-delta) & r_int16_nonzero_mask(g[0]);
    delta ^= swap&(delta^-delta);
    delta += 1;
    
    for (i = 0;i < p+1;++i) {
      t = swap&(f[i]^g[i]); f[i] ^= t; g[i] ^= t;
      t = swap&(v[i]^r[i]); v[i] ^= t; r[i] ^= t;
    }
  
    for (i = 0;i < p+1;++i) g[i] = r_F3_freeze(g[i]+sign*f[i]);
    for (i = 0;i < p+1;++i) r[i] = r_F3_freeze(r[i]+sign*v[i]);

    for (i = 0;i < p;++i) g[i] = g[i+1];
    g[p] = 0;
  }
  
  sign = f[0];
  for (i = 0;i < p;++i) out[i] = sign*v[p-1-i];
  
  return r_int16_nonzero_mask(delta);
} 

 Fq r_Fq_freeze(int32 x)
{
  return r_int32_mod_uint14(x+q12,q)-q12;
}

 void r_Rq_mult_small(Fq *h,const Fq *f,const small *g)
{
  Fq fg[p+p-1];
  Fq result;
  int i,j;

  for (i = 0;i < p;++i) {
    result = 0;
    for (j = 0;j <= i;++j) result = r_Fq_freeze(result+f[j]*(int32)g[i-j]);
    fg[i] = result;
  }
  for (i = p;i < p+p-1;++i) {
    result = 0;
    for (j = i-p+1;j < p;++j) result = r_Fq_freeze(result+f[j]*(int32)g[i-j]);
    fg[i] = result;
  }

  for (i = p+p-2;i >= p;--i) {
    fg[i-p] = r_Fq_freeze(fg[i-p]+fg[i]);
    fg[i-p+1] = r_Fq_freeze(fg[i-p+1]+fg[i]);
  }

  for (i = 0;i < p;++i) h[i] = fg[i];
}

Fq r_Fq_recip(Fq a1)
{ 
  int i = 1;
  Fq ai = a1;

  while (i < q-2) {
    ai = r_Fq_freeze(a1*(int32)ai);
    i += 1;
  }
  return ai;
} 

 int r_Rq_recip3(Fq *out,const small *in)
{ 
  Fq f[p+1],g[p+1],v[p+1],r[p+1];
  int i,loop,delta;
  int swap,t;
  int32 f0,g0;
  Fq scale;

  for (i = 0;i < p+1;++i) v[i] = 0;
  for (i = 0;i < p+1;++i) r[i] = 0;
  r[0] = r_Fq_recip(3);
  for (i = 0;i < p;++i) f[i] = 0;
  f[0] = 1; f[p-1] = f[p] = -1;
  for (i = 0;i < p;++i) g[p-1-i] = in[i];
  g[p] = 0;

  delta = 1;

  for (loop = 0;loop < 2*p-1;++loop) {
    for (i = p;i > 0;--i) v[i] = v[i-1];
    v[0] = 0;

    swap = r_int16_negative_mask(-delta) & r_int16_nonzero_mask(g[0]);
    delta ^= swap&(delta^-delta);
    delta += 1;

    for (i = 0;i < p+1;++i) {
      t = swap&(f[i]^g[i]); f[i] ^= t; g[i] ^= t;
      t = swap&(v[i]^r[i]); v[i] ^= t; r[i] ^= t;
    }

    f0 = f[0];
    g0 = g[0];
    for (i = 0;i < p+1;++i) g[i] = r_Fq_freeze(f0*g[i]-g0*f[i]);
    for (i = 0;i < p+1;++i) r[i] = r_Fq_freeze(f0*r[i]-g0*v[i]);

    for (i = 0;i < p;++i) g[i] = g[i+1];
    g[p] = 0;
  }

  scale = r_Fq_recip(f[0]);
  for (i = 0;i < p;++i) out[i] = r_Fq_freeze(scale*(int32)v[p-1-i]);

  return r_int16_nonzero_mask(delta);
}


static void r_minmax(uint32 *x,uint32 *y)
{
  uint32 xi = *x;
  uint32 yi = *y;
  uint32 xy = xi ^ yi;
  uint32 c = yi - xi;
  c ^= xy & (c ^ yi ^ 0x80000000);
  c >>= 31;
  c = -c;
  c &= xy;
  *x = xi ^ c;
  *y = yi ^ c;
}
#undef p
#undef q
void r_crypto_sort_uint32(uint32 *x,int n)
{
  int top,p,q,i;

  if (n < 2) return;
  top = 1;
  while (top < n - top) top += top;

  for (p = top;p > 0;p >>= 1) {
    for (i = 0;i < n - p;++i)
      if (!(i & p))
        r_minmax(x + i,x + i + p);
    for (q = top;q > p;q >>= 1)
      for (i = 0;i < n - q;++i)
        if (!(i & p))
          r_minmax(x + i + p,x + i + q);
  }
}
#define p (761)

static void r_Short_fromlist(small *out,const uint32 *in)
{
  uint32 L[p];
  int i;

  for (i = 0;i < w;++i) L[i] = in[i]&(uint32)-2;
  for (i = w;i < p;++i) L[i] = (in[i]&(uint32)-3)|1;
  r_crypto_sort_uint32(L,p);
  for (i = 0;i < p;++i) out[i] = (L[i]&3)-1;
}

static void r_Short_random(small *out)
{
  uint32 L[p];
  int i;

  for (i = 0;i < p;++i) L[i] = r_urandom32();
  r_Short_fromlist(out,L);
}

 void r_KeyGen(Fq *h,small *f,small *ginv)
{
  small g[p];
  Fq finv[p];
  
  for (;;) {
    r_Small_random(g);
    if (r_R3_recip(ginv,g) == 0) break;
  }
  r_Short_random(f);
  r_Rq_recip3(finv,f); /* always works */
  r_Rq_mult_small(h,finv,g);
}

#undef p
#undef w