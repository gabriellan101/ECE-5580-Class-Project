#powershell -ExecutionPolicy Bypass -File build.ps1
clear
gcc kat_ref.c sources/*.c -o test.exe -Iopenssl_files -Lopenssl_files -llibcrypto-3-x64
echo compilation done
echo running test.exe
./test.exe