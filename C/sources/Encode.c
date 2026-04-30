#include "../includes/Encode.h"
#include <stdint.h>
/*
Primary encode/encapsulation function

long long used for overflow safety

Compute → c=rh+m (mod q)
*/

void Encode( const uint16_t *R, const uint16_t *M, long long len, unsigned char *encoded)
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

    
}

/* these are the only functions that rely on p mod 4 = 1 */

static void Small_encode(unsigned char *s,const small *f)
{
  small x;
  int i;

  for (i = 0;i < p/4;++i) {
    x = *f++ + 1;
    x += (*f++ + 1)<<2;
    x += (*f++ + 1)<<4;
    x += (*f++ + 1)<<6;
    *s++ = x;
  }
  x = *f++ + 1;
  *s++ = x;
}

