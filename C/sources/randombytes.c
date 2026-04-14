#include "../includes/randombytes.h"
// The following 2 includes are for randomization functions on Windows
// Alternative versions can be used, but I'm on a windows machine so I used these
// finish later after poly arithmetic is done
#include <windows.h>
#include <bcrypt.h>

void randombytes_init() {
    unsigned char entropy_input[48];
    BCryptGenRandom(NULL, entropy_input, sizeof(entropy_input), BCRYPT_USE_SYSTEM_PREFERRED_RNG);
    randombytes_init(entropy_input, NULL, 256);
}