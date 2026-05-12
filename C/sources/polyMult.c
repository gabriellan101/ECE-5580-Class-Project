
#include <stdio.h>
#include <stdlib.h>

#include "../includes/polyMult.h"

void polyMultHelper(int * f, int f_length, int * g, int g_length, int * h, int h_length) {
    for(int i = 0; i < h_length; i++) {
        h[i] = 0;
    }
    for (int i_f = 0; i_f < f_length; i_f++) {
        for(int i_g = 0; i_g < g_length; i_g++) {
            h[i_f+i_g] += f[i_f]*g[i_g];
        }
    }
}


void polyMultHelperR3(F3 * f, int f_length, F3 * g, int g_length, F3 * h, int h_length) {
    for(int i = 0; i < h_length; i++) {
        h[i] = 0;
    }
    for (int i_f = 0; i_f < f_length; i_f++) {
        for(int i_g = 0; i_g < g_length; i_g++) {
            h[i_f+i_g] += f[i_f]*g[i_g];
        }
    }
}
/**
 * Reduces the polynomial degree by the given amount
 * Assumes that reduction is happening inside a quotient ring with a quotient of x^d-x-1, where d is the degree
 */
void polyMultReduce(int * h, int h_length, int degree)  {
    for(int i = degree; i < h_length; i++) {
        h[i-degree] += h[i];
        h[i-degree+1] += h[i+1];
    }
}

void polyMultReduceR3(F3 * h, int h_length, int degree)  {
    for(int i = degree; i < h_length; i++) {
        h[i-degree] += h[i];
        h[i-degree+1] += h[i+1];
    }
}

void RqMult(int * f, int f_length, int * g, int g_length, int * h, int h_length) {
    polyMultHelper(f, f_length, g, g_length, h, h_length);
    polyMultReduce(h, h_length, Q);
    for(int i = 0; i < h_length; i++) {
        h[i] = Fq_mod(h[i]);
    }
}

void R3Mult( F3 * f, int f_length, F3 * g, int g_length, F3 * h, int h_length) {
    F3 * h_temp = (F3*)malloc((f_length+g_length)*sizeof(int));
    polyMultHelperR3(f, f_length, g, g_length, h_temp, f_length+g_length);
    polyMultReduceR3(h_temp, f_length+g_length, 3);
    for(int i = 0; i < h_length; i++) {
        h[i] = F3_mod(h_temp[i]);
    }
    free(h_temp);
}

void i_uint32_divmod_uint14(uint32_t *q_in,uint16_t *r,uint32_t x,uint16_t m)
{
  uint32_t v = 0x80000000;
  uint32_t qpart;
  uint32_t mask;

  v /= m;
  *q_in = 0;

  qpart = (x*(uint64_t)v)>>31;

  x -= qpart*m; *q_in += qpart;
  /* x <= 49146 */

  qpart = (x*(uint64_t)v)>>31;

  x -= qpart*m; *q_in += qpart;
  x -= m; *q_in += 1;
  mask = -(x>>31);
  x += mask&(uint32_t)m; *q_in += mask;
  *r = x;
}

void i_int32_divmod_uint14(int32_t *q_in,uint16_t *r,int32_t x,uint16_t m)
{
  uint32_t uq,uq2;
  uint16_t ur,ur2;
  uint32_t mask;

  i_uint32_divmod_uint14(&uq,&ur,0x80000000+(uint32_t)x,m);
  i_uint32_divmod_uint14(&uq2,&ur2,0x80000000,m);
  ur -= ur2; uq -= uq2;
  mask = -(uint32_t)(ur>>15);
  ur += mask&m; uq += mask;
  *r = ur; *q_in = uq;
}

uint16_t i_int32_mod_uint14(int32_t x,uint16_t m)
{
  int32_t q_in;
  uint16_t r;
  i_int32_divmod_uint14(&q_in,&r,x,m);
  return r;
}

#define q12 ((Q-1)/2)

 Fq i_Fq_freeze(int32_t x)
{
  return i_int32_mod_uint14(x+q12,Q)-q12;
}


void keyGenMult(const Fq *f, const F3 *g, Fq * h) {
    /*
    Fq h_temp[2*P];
    for(int i = 0; i < 2*P-1; i++) {
        h_temp[i] = 0;
    }
    
    for(int fi = 0; fi < P; fi++) {
        for(int gi = 0; gi < P; gi++) {
            h_temp[fi+gi] = Fq_mod(h_temp[fi+gi]+f[fi]*g[gi]);
        }
    }
    for(int i = P; i < 2*P-1; i++) {
        h_temp[i-P] = Fq_mod(h_temp[i-P] + h_temp[i]);
        h_temp[i-P+1] = Fq_mod(h_temp[i-P+1] + h_temp[i+1]);
    }
    for(int i = 0; i < P; i++) {
        h[i] = h_temp[i];
    }
        */
    #warning "Using Reference KeygenMult"
    Fq fg[P+P-1];
  Fq result;
  int i,j;

  for (i = 0;i < P;++i) {
    result = 0;
    for (j = 0;j <= i;++j) result = i_Fq_freeze(result+f[j]*(int32_t)g[i-j]);
    fg[i] = result;
  }
  for (i = P;i < P+P-1;++i) {
    result = 0;
    for (j = i-P+1;j < P;++j) result = i_Fq_freeze(result+f[j]*(int32_t)g[i-j]);
    fg[i] = result;
  }

  for (i = P+P-2;i >= P;--i) {
    fg[i-P] = i_Fq_freeze(fg[i-P]+fg[i]);
    fg[i-P+1] = i_Fq_freeze(fg[i-P+1]+fg[i]);
  }

  for (i = 0;i < P;++i) h[i] = fg[i];
}
