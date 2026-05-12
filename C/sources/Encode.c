#include "../includes/Encode.h"
#include <stdint.h>
/*
Primary encode/encapsulation function

long long used for overflow safety

Compute → c=rh+m (mod q)
*/
// comment out our implementation of encode and decode for KAT
/*
void Encode(unsigned char *encoded, const uint16_t *R, const uint16_t *M, long long len)
{
    // Base case (length of message to encode is 1)
    if (len == 1) 
    {

        // accessing arrays 
        uint16_t r = R[0]; // single number being encoded into bytes 
        uint16_t m = M[0]; // bounded moduli

        // mod is not bound at 1
        
        while (m > 1) 
        {
            // storing a 32-bit value into 8-bit slot 
            // only writing the lowest 8 bits to the output buffer 
            *encoded = (unsigned char) r;
            // shift for byte emitted 
            r >>= 8;
            // need to advance location of output 
            encoded++; // moving pointer 
            // calculating upper bound of mod divided by 256 
            m = (m + 255) >> 8;
        }
    }
    

    // continuously half the problem by combining pairs 
    while (len > 1) 
    {
        // halfing length of array (due to combination of pairs)
        long long halfLen = (len + 1) / 2;
        long long i; // index variable
        
        uint16_t newR[halfLen]; // new integer array 
        uint16_t newM[halfLen]; // new moduli array

         

        // loop through 
        for (i = 0; i < len - 1; i += 2) 
        {
            uint32_t m0 = M[i]; // bound
            // mixed radix packing to combine pairs 
            // R[i+1]∈[0,M[i+1])
            uint32_t r = R[i] + (uint32_t)R[i + 1] * m0;
            uint32_t m = (uint32_t)M[i + 1] * m0;

            // when the mod range is large --> r safe to emit byte
            // divide r & m by 256, round up for m, and emit lowest byte for r 
            while (m >= 16384) 
            {
                *encoded = (unsigned char) r; // byte to emit
                // shift 
                r >>= 8; 
                // need to advance location of output 
                encoded++; // moving pointer
                // calculating upper bound of mod divided by 256 
                m = (m + 255) >> 8;
            }
            // store reduced values calculated 
            // store new integers
            newR[i / 2] = (uint16_t) r;
            // store new moduli 
            newM[i / 2] = (uint16_t) m;
        }

        // handling unpaired elements --> carrying forward 
        if (i < len) 
        {
            newR[i / 2] = R[i];
            newM[i / 2] = M[i];
        }

        // overwriting original input arrays 
        uint16_t *rPoint = (uint16_t *)R;
        uint16_t *mPoint = (uint16_t *)M;

        for (i = 0; i < halfLen; i++) 
        {
            *rPoint++ = newR[i];
            *mPoint++ = newM[i];
        }
        // starts the new smaller problem; base case will not hit this
        len = halfLen;
    }

    
}*/

// encode function from the reference implementation

/* 0 <= R[i] < M[i] < 16384 */
void Encode(unsigned char *out,const uint16_t *R,const uint16_t *M,long long len)
{
  if (len == 1) {
    uint16_t r = R[0];
    uint16_t m = M[0];
    while (m > 1) {
      *out++ = r;
      r >>= 8;
      m = (m+255)>>8;
    }
  }
  if (len > 1) {
    uint16_t R2[(len+1)/2];
    uint16_t M2[(len+1)/2];
    long long i;
    for (i = 0;i < len-1;i += 2) {
      uint32_t m0 = M[i];
      uint32_t r = R[i]+R[i+1]*m0;
      uint32_t m = M[i+1]*m0;
      while (m >= 16384) {
        *out++ = r;
        r >>= 8;
        m = (m+255)>>8;
      }
      R2[i/2] = r;
      M2[i/2] = m;
    }
    if (i < len) {
      R2[i/2] = R[i];
      M2[i/2] = M[i];
    }
    Encode(out,R2,M2,(len+1)/2);
  }
}

/* these are the only functions that rely on p mod 4 = 1 */

void encodeR3(unsigned char *s,const F3 *f)
{
  F3 x;
  int i;

  for (i = 0;i < P/4;++i) {
    x = *f++ + 1;
    x += (*f++ + 1)<<2;
    x += (*f++ + 1)<<4;
    x += (*f++ + 1)<<6;
    *s++ = x;
  }
  x = *f++ + 1;
  *s++ = x;
}

void encodeRq(unsigned char *s,const Fq *r)
{
  Fq R[P],M[P];
  int i;
  
  for (i = 0;i < P;++i) R[i] = r[i]+adj;
  for (i = 0;i < P;++i) M[i] = Q;
  Encode(s,R,M,P);
}
