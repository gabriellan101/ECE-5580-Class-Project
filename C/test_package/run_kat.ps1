#!/bin/bash
clear
gcc -Bsources -Iincludes sources/KAT_rng.c sources/NTRUPrime.c sources/polyArith.c sources/polyMult.c sources/polyUtils.c run_KAT.c sources/Types.c sources/rng.c -o test.exe -Iopenssl_files -Lopenssl_files -llibcrypto-3-x64
echo done