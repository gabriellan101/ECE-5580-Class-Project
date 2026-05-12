#powershell -ExecutionPolicy Bypass -File run_kat.ps1
clear
gcc test_build.c sources/*.c -o test.exe -Iopenssl_files -Lopenssl_files -llibcrypto-3-x64
echo compilation done
echo running test.exe
./test.exe