#include "../includes/Decode.h"
#include <stdint.h>
/*
Primary decode/decapsulation function

*/

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
    uint16_t bottomt[len/2];
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
      uint32_t r0;
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



void Decode(uint16_t *out, const unsigned char *in, const uint16_t *M, long long len)
{
    /* Temporary working buffers */
    uint16_t R[2][len];
    uint16_t Mbuf[2][len];

    int cur = 0, next = 1;

    long long i;

    /* Initialize M buffer */
    for (i = 0; i < len; i++) {
        Mbuf[cur][i] = M[i];
    }

    long long curlen = len;

    /* Step 1: Build combined values from bytes (reverse of Encode shrinking) */
    while (curlen > 1) {
        long long newlen = (curlen + 1) / 2;

        for (i = 0; i < newlen; i++) {
            R[cur][i] = 0;
        }

        /* Reconstruct combined values */
        for (i = newlen - 1; i >= 0; i--) {
            uint32_t m = Mbuf[cur][i];
            uint32_t r = 0;
            uint32_t shift = 1;

            /* Rebuild r from bytes */
            while (m > 1) {
                m = (m + 255) >> 8;
                r += (*in++) * shift;
                shift <<= 8;
            }

            R[cur][i] = (uint16_t) r;
        }

        /* Step 2: Split combined values into pairs */
        for (i = 0; i < curlen - 1; i += 2) {
            uint32_t m0 = Mbuf[cur][i];
            uint32_t r = R[cur][i / 2];

            R[next][i]     = r % m0;
            R[next][i + 1] = r / m0;

            Mbuf[next][i]     = m0;
            Mbuf[next][i + 1] = Mbuf[cur][i + 1];
        }

        /* Handle odd element */
        if (i < curlen) {
            R[next][i] = R[cur][i / 2];
            Mbuf[next][i] = Mbuf[cur][i];
        }

        /* Swap buffers */
        cur ^= 1;
        next ^= 1;

        curlen = newlen;
    }

    /* Final step: last element */
    {
        uint32_t m = Mbuf[cur][0];
        uint32_t r = 0;
        uint32_t shift = 1;

        while (m > 1) {
            m = (m + 255) >> 8;
            r += (*in++) * shift;
            shift <<= 8;
        }

        R[cur][0] = (uint16_t) r;
    }

    /* Output final values */
    for (i = 0; i < len; i++) {
        out[i] = R[cur][i];
    }
}