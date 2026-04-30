//gcc rng.c run_KAT.c -I"../includes" -Iopenssl_files -Lopenssl_files -llibcrypto-3-x64 -o test.exe
//^ add c files as test expands
#include "../includes/KAT_rng.h"
#include <stdio.h>
#include "../includes/OuterLayer.h"
#define KATNUM 1
#define SEED_LENGTH 10

unsigned char entropy_input[48];
unsigned char seed[KATNUM][48];


int main() {
    printf("started\n");
    FILE                *fp_req, *fp_rsp;
    int                 ret_val;
    int i;
    unsigned char *ct = 0;
    unsigned char *ss = 0;
    unsigned char *ss1 = 0;
    unsigned char *pk = 0;
    unsigned char *sk = 0;

    for (i=0; i<48; i++)
        entropy_input[i] = i;
    randombytes_init(entropy_input, NULL, 256);

    for (i=0; i<KATNUM; i++)
        randombytes(seed[i], 48);
    printf(&seed);
        /*
    fp_req = fdopen(8, "w");
    if (!fp_req)
        return -1;

    for (i=0; i<KATNUM; i++) {

        randombytes_init(seed[i], NULL, 256);

        //fprintf(fp_rsp, "count = %d\n", i);
        //fprintBstr(fp_rsp, "seed = ", seed[i], 48);
        
        if ( (ret_val = OuterKeygen(pk, sk)) != 0) {
            printf("crypto_kem_keypair returned wrong\n");
            return -1;
        }
        /*
        if ( (ret_val = crypto_kem_enc(ct, ss, pk)) != 0) {
            fprintf(stderr, "crypto_kem_enc returned <%d>\n", ret_val);
            return KAT_CRYPTO_FAILURE;
        }
        fprintBstr(fp_rsp, "ct = ", ct, crypto_kem_CIPHERTEXTBYTES);
        fprintBstr(fp_rsp, "ss = ", ss, crypto_kem_BYTES);
        
        fprintf(fp_rsp, "\n");
        
        if ( (ret_val = crypto_kem_dec(ss1, ct, sk)) != 0) {
            fprintf(stderr, "crypto_kem_dec returned <%d>\n", ret_val);
            return KAT_CRYPTO_FAILURE;
        }
        
        if ( memcmp(ss, ss1, crypto_kem_BYTES) ) {
            fprintf(stderr, "crypto_kem_dec returned bad 'ss' value\n");
            return KAT_CRYPTO_FAILURE;
        }
        
    }
*/
    return 0;
}