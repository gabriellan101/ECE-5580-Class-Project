#include "Encode.h"
#include <stdint.h>
/*
Primary encode/encapsulation function

long long used for overflow safety
*/
void Encode(long long arrayLen, const uint16_t *R, const uint16_t *Moduli, unsigned char *out){

    //base case 
    if (arrayLen == 1) // only encoding one num
    {
        uint16_t r = R[0]; //encoding a single number r with limit m 
        uint16_t m = Moduli[0];

        //take the lowest 8 bits of r and output a byte 
        //shift r right to remove those bits 
        //shrink m accordingly 
        //basically --> write r in base 256 (bytes) but only as many bytes as needed for m 
        while (m > 1) // exit condition  
        {
            *out++ = r; // output is array value encoded 
            r >>=8;
            m = (m+255)>>8;
        }
    }
    //recursive packing 
    if (arrayLen > 1) //combining nums from array to then encode 
    {
        uint16_t R2[(arrayLen+1)/2]; 
        uint16_t M2[(arrayLen+1)/2];
        long long i;
        for (i = 0;i < arrayLen-1;i += 2) {
            uint32_t m0 = Moduli[i];
            //mixed-radix encoding; instead of storing R[i], R[i+1], we combine them 
            uint32_t r = R[i]+R[i+1]*m0;
            uint32_t m = Moduli[i+1]*m0;
            //if combind range m is big --> emit bytes 
            //shrink both r and m 
            while (m >= 16384) {
                *out++ = r;
                r >>= 8;
                m = (m+255)>>8;
            }
            //storing reduced values; creating smaller problem 
            R2[i/2] = r;
            M2[i/2] = m;
        }   
        if (i < arrayLen) {
            //if there is an extra element, carry it forward 
            R2[i/2] = R[i];
            M2[i/2] = Moduli[i];
        }
        Encode(out,R2,M2,(arrayLen+1)/2);
    }
}


void Encode(unsigned char *out, const uint16_t *R, const uint16_t *M, long long len)
{
    /* We will repeatedly shrink the problem size */
    while (len > 1) {
        long long newlen = (len + 1) / 2;

        uint16_t R2[newlen];
        uint16_t M2[newlen];

        long long i;
        for (i = 0; i < len - 1; i += 2) {
            uint32_t m0 = M[i];
            uint32_t r = R[i] + (uint32_t)R[i + 1] * m0;
            uint32_t m = (uint32_t)M[i + 1] * m0;

            /* Output bytes while range is large */
            while (m >= 16384) {
                *out++ = (unsigned char) r;
                r >>= 8;
                m = (m + 255) >> 8;
            }

            R2[i / 2] = (uint16_t) r;
            M2[i / 2] = (uint16_t) m;
        }

        /* Handle odd element */
        if (i < len) {
            R2[i / 2] = R[i];
            M2[i / 2] = M[i];
        }

        /* Move to next level */
        for (i = 0; i < newlen; i++) {
            ((uint16_t *)R)[i] = R2[i];
            ((uint16_t *)M)[i] = M2[i];
        }

        len = newlen;
    }

    /* Final base case (len == 1) */
    if (len == 1) {
        uint16_t r = R[0];
        uint16_t m = M[0];

        while (m > 1) {
            *out++ = (unsigned char) r;
            r >>= 8;
            m = (m + 255) >> 8;
        }
    }
}