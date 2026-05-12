
#include <stdio.h>
#include <stdlib.h>

#include "../includes/polyMult.h"

void polyMultHelper(int * f, int f_length, int * g, int g_length, int * h, int h_length) {
    for(int i = 0; i < h_length; i++) {
        h[i] = 0;
    }
    for (int i_f = 0; i_f < f_length; i_f++) {
        for(int i_g = 0; i_g < g_length; i_g++) {
            h[i_f+i_g] += f[i_f]*g[i_g];
        }
    }
}


void polyMultHelperR3(F3 * f, int f_length, F3 * g, int g_length, F3 * h, int h_length) {
    for(int i = 0; i < h_length; i++) {
        h[i] = 0;
    }
    for (int i_f = 0; i_f < f_length; i_f++) {
        for(int i_g = 0; i_g < g_length; i_g++) {
            h[i_f+i_g] += f[i_f]*g[i_g];
        }
    }
}
/**
 * Reduces the polynomial degree by the given amount
 * Assumes that reduction is happening inside a quotient ring with a quotient of x^d-x-1, where d is the degree
 */
void polyMultReduce(int * h, int h_length, int degree)  {
    for(int i = degree; i < h_length; i++) {
        h[i-degree] += h[i];
        h[i-degree+1] += h[i+1];
    }
}

void polyMultReduceR3(F3 * h, int h_length, int degree)  {
    for(int i = degree; i < h_length; i++) {
        h[i-degree] += h[i];
        h[i-degree+1] += h[i+1];
    }
}

void RqMult(int * f, int f_length, int * g, int g_length, int * h, int h_length) {
    polyMultHelper(f, f_length, g, g_length, h, h_length);
    polyMultReduce(h, h_length, Q);
    for(int i = 0; i < h_length; i++) {
        h[i] = Fq_mod(h[i]);
    }
}

void R3Mult( F3 * f, int f_length, F3 * g, int g_length, F3 * h, int h_length) {
    F3 * h_temp = (F3*)malloc((f_length+g_length)*sizeof(int));
    polyMultHelperR3(f, f_length, g, g_length, h_temp, f_length+g_length);
    polyMultReduceR3(h_temp, f_length+g_length, 3);
    for(int i = 0; i < h_length; i++) {
        h[i] = F3_mod(h_temp[i]);
    }
    free(h_temp);
}


void keyGenMult(const Fq *f, const F3 *g, Fq * h) {
    Fq h_temp[2*P];
    for(int i = 0; i < 2*P-1; i++) {
        h_temp[i] = 0;
    }
    
    for(int fi = 0; fi < P; fi++) {
        for(int gi = 0; gi < P; gi++) {
            h_temp[fi+gi] = Fq_mod(h_temp[fi+gi]+f[fi]*g[gi]);
        }
    }
    for(int i = P; i < 2*P-1; i++) {
        h_temp[i-P] = Fq_mod(h_temp[i-P] + h_temp[i]);
        h_temp[i-P+1] = Fq_mod(h_temp[i-P+1] + h_temp[i+1]);
    }
    for(int i = 0; i < P; i++) {
        h[i] = h_temp[i];
    }
}
