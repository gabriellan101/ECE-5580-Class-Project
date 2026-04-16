
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

void RqMult(int * f, int f_length, int * g, int g_length, int * h, int h_length) {
    polyMultHelper(f, f_length, g, g_length, h, h_length);
    polyMultReduce(h, h_length, Q);
    for(int i = 0; i < h_length; i++) {
        h[i] = Fq_mod(h[i]);
    }
}

void R3Mult(int * f, int f_length, int * g, int g_length, int * h, int h_length) {
    int * h_temp = (int*)malloc((f_length+g_length)*sizeof(int));
    polyMultHelper(f, f_length, g, g_length, h_temp, f_length+g_length);
    polyMultReduce(h_temp, f_length+g_length, 3);
    for(int i = 0; i < h_length; i++) {
        h[i] = F3_mod(h_temp[i]);
    }
    free(h_temp);
}


void keyGenMult(Fq *f, Fq *g, Fq * h) {
    for(int i = 0; i < 2*P-1; i++) {
        h[i] = 0;
    }
    for(int fi = 0; fi < P; fi++) {
        for(int gi = 0; gi < P; gi++) {
            h[fi+gi] = Fq_mod(h[fi+gi]+f[fi]*g[gi]);
        }
    }
    for(int i = P; i < 2*P-1; i++) {
        h[i-P] = Fq_mod(h[i-P] + h[i]);
        h[i-P+1] = Fq_mod(h[i-P+1] + h[i+1]);
    }
}
