#!/bin/bash
cd sources
clear
gcc .\KAT_rng.c OuterLayer.c .\NTRUPrime.c .\polyArith.c .\polyMult.c .\polyUtils.c .\run_KAT.c .\Types.c .\rng.c -o test.exe -Iopenssl_files -Lopenssl_files -llibcrypto-3-x64 -I"../includes"
echo done