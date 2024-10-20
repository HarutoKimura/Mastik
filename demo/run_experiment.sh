gcc -g -O0 -o gmp_rsa gmp_rsa.c multiply.c reduce.c -lgmp
gcc -o fr_modular_exponentiation FR-Modular-Exponentiation.c -lmastik -lbfd -ldwarf -lelf
./fr_modular_exponentiation ./gmp_rsa
python3 plot_results.py