gcc -g -O0 -o gmp_rsa gmp_rsa.c -lgmp
gcc -o fr_modular_exponentiation FR-Modular-Exponentiation.c -lmastik -lbfd -ldwarf -lelf
./fr_modular_exponentiation ./gmp_rsa & ./Modular_Expo 11 5 100
python3 plot_results.py