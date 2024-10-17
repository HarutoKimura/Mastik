#include <stdio.h>
#include <gmp.h>
void multiply(mpz_t result, const mpz_t x, const mpz_t b) {
    // printf("Multiply function address: %p\n", (void*)multiply);
    static int count = 0;
    printf("Multiply called: %d times\n", ++count);
    mpz_mul(result, x, b);
}

// void reduce(mpz_t result, const mpz_t x, const mpz_t p) {
//     mpz_mod(result, x, p);
// }

// void multiply_and_reduce(mpz_t result, const mpz_t x, const mpz_t b, const mpz_t p) {
//     mpz_t temp;
//     mpz_init(temp);

//     multiply(temp, x, b);
//     reduce(result, temp, p);

//     mpz_clear(temp);
// }