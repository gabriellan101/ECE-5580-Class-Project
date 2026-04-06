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

    }
}