#include <gmp.h>
void square(mpz_t result, const mpz_t x) {
    mpz_mul(result, x, x);
}