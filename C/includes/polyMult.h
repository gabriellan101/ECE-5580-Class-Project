#ifndef POLY_MULT
#define POLYMULT
#include <stdio.h>
#include <stdlib.h>

#include "Types.h"


void polyMultHelper(int * f, int f_length, int * g, int g_length, int * h, int h_length);
/**
 * Reduces the polynomial degree by the given amount
 * Assumes that reduction is happening inside a quotient ring with a quotient of x^d-x-1, where d is the degree
 */
void polyMultReduce(int * h, int h_length, int degree);

void RqMult(int * f, int f_length, int * g, int g_length, int * h, int h_length);

void R3Mult(int * f, int f_length, int * g, int g_length, int * h, int h_length);

void keyGenMult(Fq *f, Fq *g, Fq * h);
#endif

