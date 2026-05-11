#!/bin/bash
# ` is the line continuation character for PowerShell, makes the script prettier
clear
gcc -Bsources -Iincludes sources/OuterLayer.c sources/OuterLayer.h includes/Types.h sources/Types.c `
sources/KAT_rng.c includes/NTRUPrime.h sources/NTRUPrime.c sources/polyArith.c includes/polyArith.h  sources/Encode.c includes/Encode.h `
sources/polyMult.c sources/polyUtils.c includes/polyUtils.h run_KAT.c sources/Types.c sources/rng.c -o test.exe `
-Iopenssl_files -Lopenssl_files -llibcrypto-3-x64
echo done