#include <stdio.h>
#include <gmp.h>

void multiply(mpz_t result, const mpz_t x, const mpz_t b) {
    mpz_mul(result, x, b);
}