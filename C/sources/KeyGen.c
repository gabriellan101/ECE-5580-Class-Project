
#include "../includes/Types.h"
#include "../includes/polyMult.h"
#include <stdbool.h>
#define KAT 1

/*
* TODO: Replace with function that checks if G is invertible in R3
*/
bool isInvertible(Fq * g) {
    return true;
}
/**
 * If using Known Answer Tests, use these functions
 * Following functions are adapted directly from the reference implementation,
 * ensuring generation of polynomials match the reference and KATs can be used
 */
#if KAT
static uint32_t urandom32(void)
{
  unsigned char c[4];
  uint32_t out[4];

  randombytes(c,4);
  out[0] = (uint32_t)c[0];
  out[1] = ((uint32_t)c[1])<<8;
  out[2] = ((uint32_t)c[2])<<16;
  out[3] = ((uint32_t)c[3])<<24;
  return out[0]+out[1]+out[2]+out[3];
}

void makeGPoly(Fq * g) {
    for (int i = 0;i < P;++i) g[i] = (((urandom32()&0x3fffffff)*3)>>30)-1;
}
#endif
void KeyGen() {
    
    Fq g[P];
    for(;;) {
    makeGPoly(g);
    if(isInvertible(g))
        break;
    }
    
    Fq f[P];
    

    int f3[P];
    polyMultHelper(&f, P, &threePoly, 1, &f3, P);

    int finv[P];


    /*TODO: replace with actual invert function*/
    //invert(F, finv);

    int h[Q];
    RqMult(g, P, finv, P, h, Q);


    //mult f by 3
    //invert f
    //find h = g/(3f)
}