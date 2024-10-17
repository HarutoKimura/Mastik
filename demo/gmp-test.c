#include <gmp.h>
#include <stdio.h>

int main() {
    mpz_t n;
    mpz_init(n);
    mpz_set_str(n, "12345678901234567890", 10);
    gmp_printf("n = %Zd\n", n);
    mpz_clear(n);
    return 0;
}