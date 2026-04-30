#!/bin/bash
#powershell -ExecutionPolicy Bypass -File run_kat.ps1 <- runs the script if powershell has permission issues also powershell sucks
cd sources
clear
gcc .\KAT_rng.c .\NTRUPrime.c .\polyArith.c .\polyMult.c .\polyUtils.c .\run_KAT.c .\Types.c .\rng.c -o test.exe -Iopenssl_files -Lopenssl_files -llibcrypto-3-x64 -I"../includes"
echo done