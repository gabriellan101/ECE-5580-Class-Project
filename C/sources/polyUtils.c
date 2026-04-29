#include "../includes/polyUtils.h"
#define KAT 1
// this function has been tested against the original O(Q) implementation and produces the same output for every nonzero element in Fq
Fq Fq_recip(Fq x) {
    int32_t result = 1;
    int32_t base = x;
    int exp = Q - 2;

    while (exp > 0) {
        if (exp & 1)
            result = Fq_mod(base * result);
        base = Fq_mod(base * base);
        exp >>= 1;
    }
    return result;
}

bool isSmall(int * f, int f_length) {
    for(int i = 0; i < f_length; i++) {
        if(f[i] > 1) 
            return false;
        if(f[i] < -1)
            return false;
    }
    return true;
}

bool isTSmall(int * f, int f_length) {
    int bigCount = 0;
    for(int i = 0; i < f_length; i++) {
        if(f[i] > 1) 
            bigCount++;
        if(f[i] < -1)
            bigCount++;
    }
    return (bigCount > W) ? false: true;
}

int isNonZero(int16_t f){
    return (f != 0) ? -1 : 0;
}

int isNegative(int16_t f){
    return (f < 0) ? -1 : 0;
}

void roundR3(Fq *out, const Fq *in) {
    for (int i = 0; i < P; i++) out[i] = in[i] - F3_mod(in[i]);
}


void RqtoR3(F3 *out, const Fq *in) {
    for (int i = 0; i < P; i++) {
        out[i] = F3_mod(in[i]);
    }
}

//
void Rq_scale3(Fq *out, const Fq *in) {
    for (int i = 0; i < P; i++) out[i] = Fq_mod(3 * (int32_t)in[i]);
}

// counts nonzero coefficients in R3 poly and compares them to the weight
// Should have 286 nonzero coefficients
int isValidPoly(F3 *x) {
    int count = 0;
    for (int i = 0; i < P; i++) count += x[i]&1; // uses a bitmask here to detect nonzero. Coefficients are already in  {-1, 0, 1}
    return isNonZero(count - W);
}

void reconstruct(F3 *out, int valid, F3 *in){
    for (int i = 0; i < P; i++){
        if (valid == 0) out[i] = in[i];
        else if (i < W) out[i] = 1;
        else out[i] = 0;
    }
}

