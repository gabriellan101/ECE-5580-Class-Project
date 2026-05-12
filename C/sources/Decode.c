#include "../includes/Decode.h"
#include <stdint.h>
/*
Primary decode/decapsulation function

must use reverse iteration 

Steps: 

1. read combined value in bytes from encoded 
2. split back into separate values 
3. repeat 
*/

/*
void Decode(uint16_t *decoded, const unsigned char *encoded, const uint16_t *M, long long len)
{
    // working buffer for decoded val
    //uint16_t R[len];
    // need buffer that we can modify --> M const 
    // buffer for moduli (non constant)
    //uint16_t Mbuf[len]; 
    uint16_t *R = malloc(len * sizeof(uint16_t));
    uint16_t *Mbuf = malloc(len * sizeof(uint16_t));

    if (!R || !Mbuf)
    {
      free(R);
      free(Mbuf);
      return;
    }
    // index 
    long long i;
    //current length -- number of values 
    long long numVal = len;

    // need to copy initial mod for reorg later
    
    // populating mod buffer by copying moduli from M passed in
    for (i = 0; i < len; i++) 
    {
      Mbuf[i] = M[i];
    }
    // main loop -- byte reconstruction
    /*
      Need to do: 
      1. read comp val
      2. expand 
      3. reduce prob
    */
   /* COMMENT OUT HERE
    // more than one val pair to decode 
    while (numVal > 1) 
    {
      // calculate half len
      // each pair --> 1 val 
      // pass over odd el
      long long halfLen = (numVal + 1) / 2;
      // length for iteration 
      long long halfLen1 = halfLen - 1;

      //reconstruction
      // working right to left -- reverse iteration
      for (i = halfLen1; i >= 0; i--) 
      {
        // current mod --> bound 
        uint32_t currMod = Mbuf[i];
        //inititialize for reconstruct int 
        uint32_t r = 0;
        // tracker for power 256
        uint32_t powTrack = 1;

        // condition to check --> if more bytes are needed 
        while (currMod > 1) 
        {
          // shrinking mod --> determine bytes 
          // need val for currMod + 255 
          uint32_t currMod1 = currMod + 255;
          // shrinking mod --> dividing by 256
          // maybe >> 8 ?
          currMod = (currMod1) / 256;
          // rounding next byte from input 
          // multiplying by current power of 256 
          // add into r 
          // advance encoded pointer
          uint32_t outByte = *encoded;
          r += outByte * powTrack;
          encoded++;
          // multply shift by 256 for next byte
          powTrack = powTrack * 256;
        }
        
        // need to cast & store in array -- come back 
        // storing new val 
        R[i] = (uint16_t)r;
      }

      // now expanding pairs 
      // processing pairs as (i-1, i)
      // need to avoid overwriting 
      for (long long i = numVal - 1; i > 0; i -= 2) 
      {
        // mod for first num in pair 
        uint32_t modNum1 = Mbuf[i - 1];
        // combined val of pair 
        // pair index 
        long long indexPair = (i - 1) / 2;
        uint32_t r  = R[indexPair];
        
        // need to split 
        // div ? 
        // grabbing first val 
        uint32_t firstVal = r % modNum1;
        // storing val 
        R[i - 1] = firstVal; 
        // grabbing second val  
        uint32_t secondVal = r / modNum1; 
        // storing second val 
        R[i] = secondVal;
        // storing both mods 
        // mod for first val
        Mbuf[i - 1] = modNum1;
        // mod corresponding second val 
        // shouldn't change ?
        // Mbuf[i] = Mbuf[i];       
      }
      // checking for odd el
      // not paired --> need to copy
      // mod should be fine 
      if (numVal % 2 != 0) 
      {
        // index of last el
        long long i = numVal - 1;
        // copy not paired value (that should be the odd el)
        // store in array
        R[i] = R[i / 2];
      }
      // move to next smaller prob with updated len
      numVal = halfLen;
    }
      //final mod 
      uint32_t finalMod = Mbuf[0];
      // final val 
      uint32_t finalVal = 0;
      // shifter for 256 pow
      uint32_t pow256 = 1;
      while (finalMod > 1) 
      {
        // calc bte count
        finalMod = (finalMod + 255) / 256;
        // read byte & grow
        uint32_t byte = *encoded;
        finalVal += byte * pow256;
        encoded++;
        // increase pow
        pow256 = pow256 * 256;
      }
      // store final val
      R[0] = (uint16_t)finalVal;
    // copy val to output array 
    for (long long i = 0; i < len; i++) 
    {
      // final encoded val
      decoded[i] = R[i];
    }
}*/

// from reference implementation, used for KAT
void uint32_divmod_uint14(uint32_t *q,uint16_t *r,uint32_t x,uint16_t m)
{
  uint32_t v = 0x80000000;
  uint32_t qpart;
  uint32_t mask;

  v /= m;

  /* caller guarantees m > 0 */
  /* caller guarantees m < 16384 */
  /* vm <= 2^31 <= vm+m-1 */
  /* xvm <= 2^31 x <= xvm+x(m-1) */

  *q = 0;

  qpart = (x*(uint64_t)v)>>31;
  /* 2^31 qpart <= xv <= 2^31 qpart + 2^31-1 */
  /* 2^31 qpart m <= xvm <= 2^31 qpart m + (2^31-1)m */
  /* 2^31 qpart m <= 2^31 x <= 2^31 qpart m + (2^31-1)m + x(m-1) */
  /* 0 <= 2^31 newx <= (2^31-1)m + x(m-1) */
  /* 0 <= newx <= (1-1/2^31)m + x(m-1)/2^31 */
  /* 0 <= newx <= (1-1/2^31)(2^14-1) + (2^32-1)((2^14-1)-1)/2^31 */

  x -= qpart*m; *q += qpart;
  /* x <= 49146 */

  qpart = (x*(uint64_t)v)>>31;
  /* 0 <= newx <= (1-1/2^31)m + x(m-1)/2^31 */
  /* 0 <= newx <= m + 49146(2^14-1)/2^31 */
  /* 0 <= newx <= m + 0.4 */
  /* 0 <= newx <= m */

  x -= qpart*m; *q += qpart;
  /* x <= m */

  x -= m; *q += 1;
  mask = -(x>>31);
  x += mask&(uint32_t)m; *q += mask;
  /* x < m */

  *r = x;
}

uint32_t uint32_div_uint14(uint32_t x,uint16_t m)
{
  uint32_t q;
  uint16_t r;
  uint32_divmod_uint14(&q,&r,x,m);
  return q;
}

uint16_t uint32_mod_uint14(uint32_t x,uint16_t m)
{
  uint32_t q;
  uint16_t r;
  uint32_divmod_uint14(&q,&r,x,m);
  return r;
}


void Decode(uint16_t *out,const unsigned char *S,const uint16_t *M,long long len)
{
  if (len == 1) {
    if (M[0] == 1)
      *out = 0;
    else if (M[0] <= 256)
      *out = uint32_mod_uint14(S[0],M[0]);
    else
      *out = uint32_mod_uint14(S[0]+(((uint16_t)S[1])<<8),M[0]);
  }
  if (len > 1) {
    uint16_t R2[(len+1)/2];
    uint16_t M2[(len+1)/2];
    uint16_t bottomr[len/2];
    uint32_t bottomt[len/2];
    long long i;
    for (i = 0;i < len-1;i += 2) {
      uint32_t m = M[i]*(uint32_t) M[i+1];
      if (m > 256*16383) {
        bottomt[i/2] = 256*256;
        bottomr[i/2] = S[0]+256*S[1];
        S += 2;
        M2[i/2] = (((m+255)>>8)+255)>>8;
      } else if (m >= 16384) {
        bottomt[i/2] = 256;
        bottomr[i/2] = S[0];
        S += 1;
        M2[i/2] = (m+255)>>8;
      } else {
        bottomt[i/2] = 1;
        bottomr[i/2] = 0;
        M2[i/2] = m;
      }
    }
    if (i < len)
      M2[i/2] = M[i];
    Decode(R2,S,M2,(len+1)/2);
    for (i = 0;i < len-1;i += 2) {
      uint32_t r = bottomr[i/2];
      uint32_t r1;
      uint16_t r0;
      r += bottomt[i/2]*R2[i/2];
      uint32_divmod_uint14(&r1,&r0,r,M[i]);
      r1 = uint32_mod_uint14(r1,M[i+1]); /* only needed for invalid inputs */
      *out++ = r0;
      *out++ = r1;
    }
    if (i < len)
      *out++ = R2[i/2];
  }
}



void decodeRq(Fq *r,const unsigned char *s)
{
  Fq R[P],M[P];
  int i;

  for (i = 0;i < P;++i) M[i] = Q;
  Decode(R,s,M,P);
  for (i = 0;i < P;++i) r[i] = ((Fq)R[i])-adj;
}

void decodeR3(F3 *f,const unsigned char *s)
{
  unsigned char x;
  int i;

  for (i = 0;i < P/4;++i) {
    x = *s++;
    *f++ = ((F3)(x&3))-1; x >>= 2;
    *f++ = ((F3)(x&3))-1; x >>= 2;
    *f++ = ((F3)(x&3))-1; x >>= 2;
    *f++ = ((F3)(x&3))-1;
  }
  x = *s++;
  *f++ = ((F3)(x&3))-1;
}