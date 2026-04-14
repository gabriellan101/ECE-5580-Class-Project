#include "../include/Encode.h"
#include <stdint.h>
/*
Primary encode/encapsulation function

long long used for overflow safety

Compute → c=rh+m (mod q)
*/

void Encode(unsigned char *out, const uint16_t *R, const uint16_t *M, long long len)
{
    // Base case (length of message to encode is 1)
    if (len == 1) {

        // accessing arrays 
        uint16_t r = R[0]; // single number being encoded into bytes 
        uint16_t m = M[0]; // bounded moduli

        // mod is not bound at 1
        
        while (m > 1) {
            // storing a 32-bit value into 8-bit slot 
            // only writing the lowest 8 bits to the output buffer 
            *out = (unsigned char) r; // need to advance location of output 
            out++; // moving pointer 
            // shift for byte emitted 
            r >>= 8;
            // calculating upper bound of mod divided by 256 
            m = (m + 255) >> 8;
        }
    }
    

    // continuously half the problem by combining pairs 
    while (len > 1) {

        // halfing length of array (due to combination of pairs)
        long long newlen = (len + 1) / 2;
        
        uint16_t R2[newlen]; // new integer array 
        uint16_t M2[newlen]; // new moduli array

        long long i; // index variable 

        // loop through 
        for (i = 0; i < len - 1; i += 2) {
            uint32_t m0 = M[i]; // bound
            // mixed radix packing to combine pairs 
            // R[i+1]∈[0,M[i+1])
            uint32_t r = R[i] + (uint32_t)R[i + 1] * m0;
            uint32_t m = (uint32_t)M[i + 1] * m0;

            // when the mod range is large --> r safe to emit byte
            // divide r & m by 256, round up for m, and emit lowest byte for r 
            while (m >= 16384) {
                *out++ = (unsigned char) r;
                // shift 
                r >>= 8; 
                // calculating upper bound of mod divided by 256 
                m = (m + 255) >> 8;
            }

            // store reduced values calculated 
            // store new integers
            R2[i / 2] = (uint16_t) r;
            // store new moduli 
            M2[i / 2] = (uint16_t) m;
        }

        // handling unpaired elements --> carrying forward 
        if (i < len) {
            R2[i / 2] = R[i];
            M2[i / 2] = M[i];
        }

        // overwriting original input arrays 
        for (i = 0; i < newlen; i++) {
            ((uint16_t *)R)[i] = R2[i];
            ((uint16_t *)M)[i] = M2[i];
        }
        // starts the new smaller problem; base case will not hit this
        len = newlen;
    }

    
}