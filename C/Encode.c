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
        uint16_t r = R[0];
        uint16_t m = Moduli[0];

        while (m > 1) // exit condition 
        {
            *out++ = r; // output is array value encoded 
            r >>=8;
            m = (m+255)>>8;
        }
    }

    if (arrayLen > 1) //combining nums from array to then encode 
    {
        uint16_t R2[(arrayLen+1)/2]; 
        uint16_t M2[(arrayLen+1)/2];
        long long i;
        for (i = 0;i < arrayLen-1;i += 2) {
            uint32_t m0 = Moduli[i];
            uint32_t r = R[i]+R[i+1]*m0;
            uint32_t m = Moduli[i+1]*m0;
            while (m >= 16384) {
                *out++ = r;
                r >>= 8;
                m = (m+255)>>8;
            }
            R2[i/2] = r;
            M2[i/2] = m;
        }   
        if (i < arrayLen) {
            R2[i/2] = R[i];
            M2[i/2] = Moduli[i];
        }
        Encode(out,R2,M2,(arrayLen+1)/2);
    }
}