gcc -g -O0 -o vulnerable_rsa vulnerable_rsa.c multiply.c reduce.c -lgmp # Compile the vulnerable RSA implementation, this is the victim process
nm -l vulnerable_rsa | grep -E 'square|multiply|reduce' #Check the square, multiply and reduce's memory addresses
gcc -o FR-spy FR-spy.c -lmastik -lbfd -ldwarf -lelf # Compile the FR-spy, this is the spy process
./FR-spy ./vulnerable_rsa # do the flush and reload attack, FR-spy monitors the vulnerable_rsa execution
python3 plot_result.py # plot the results