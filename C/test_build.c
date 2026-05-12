#include "includes/OuterLayer.h"
#include "includes/NTRUPrime.h"
int main() {
    
    Fq h[761], c[761];
    F3 f[761], ginv[761];
    KeyGen(h,f,ginv);
    //Encrypt(c, const F3 *r, h)

    printf("test ran\n");
}