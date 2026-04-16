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


void Decode(const unsigned char *encoded, const uint16_t *M,long long len, uint16_t *decoded)
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
}