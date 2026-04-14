
#include "../includes/Types.h"
#include "../includes/polyMult.h"
#include <stdbool.h>


/*
* TODO: Replace with function that checks if G is invertible in R3
*/
bool isInvertible(Fq * g) {
    return true;
}


void KeyGen() {
    
    //TODO: Generate g, a random small element polynomial in R and check if its invertible
    Fq g[P];
    do {
    for (int i = 0; i < P; i++) {
        g[P] = 0; //TODO: replace with random number generation
    }
    } while (!isInvertible(g));




        //TODO: Generate f, a random t-small element polynomial in R
    Fq f[P];
    do {
    for (int i = 0; i < P; i++) {
        g[P] = 0; //TODO: replace with random number generation
    }
    } while (isTSmall(&f));

    int threePoly = {3};

    int f3[P];
    polyMultHelper(&f, P, &threePoly, 1, &f3, P);


    
    //mult f by 3
    //invert f
    //find h = g/(3f)
}