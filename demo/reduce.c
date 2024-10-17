#include <stdio.h>
#include <gmp.h>
void reduce(mpz_t result, const mpz_t x, const mpz_t p) {
    // printf("Reduce function address: %p\n", (void*)reduce);
    mpz_mod(result, x, p);
}