#include <stdio.h>
#include <stdlib.h>
#include <gmp.h>
#include <emmintrin.h>

// extern void reduce(mpz_t result, const mpz_t x, const mpz_t p);
// volatile char padding1[16384] = {0};

void square(mpz_t result, const mpz_t x) {
    // printf("Square function address: %p\n", (void*)square);
    static int count = 0;
    printf("Square called: %d times\n", ++count);
    mpz_mul(result, x, x);
}

// volatile char padding2[16384] = {0};


// void multiply(mpz_t result, const mpz_t x, const mpz_t b) {
//     printf("Multiply function address: %p\n", (void*)multiply);
//     mpz_mul(result, x, b);
// }

// volatile char padding2[16384] = {0};

// void reduce(mpz_t result, const mpz_t x, const mpz_t p) {
//     printf("Reduce function address: %p\n", (void*)reduce);
//     mpz_mod(result, x, p);
// }

// void square_and_reduce(mpz_t result, const mpz_t x, const mpz_t p) {
//     mpz_t temp;
//     mpz_init(temp);

//     square(temp, x);
//     reduce(result, temp, p);

//     mpz_clear(temp);
// }

// extern void square(mpz_t result, const mpz_t x);
extern void multiply(mpz_t result, const mpz_t x, const mpz_t b);
extern void reduce(mpz_t result, const mpz_t x, const mpz_t p);

// extern void multiply_and_reduce(mpz_t result, const mpz_t x, const mpz_t b, const mpz_t p);

// vulnerable modular exponentiation function
// square and multiply functions
void modular_exponentiation(mpz_t result, const mpz_t b, const mpz_t d, const mpz_t p) {
    mpz_t x, temp;
    mpz_init_set_ui(x, 1);
    mpz_init(temp);

    size_t num_bits = mpz_sizeinbase(d, 2);

    for (int i = num_bits - 1; i >= 0; i--) {
        // Square
        square(temp, x);
        _mm_mfence();  // Memory barrier
        reduce(x, temp, p);
         _mm_mfence();  // Memory barrier
        // square_and_reduce(temp, x, p);

        // Multiply (if the bit is set)
        if (mpz_tstbit(d, i)) {
            multiply(temp, x, b);
            _mm_mfence();  // Memory barrier
            reduce(x, temp, p);
            // multiply_and_reduce(temp, x, b, p);
        }
    }

    mpz_set(result, x);
    mpz_clear(x);
    mpz_clear(temp);
}


typedef struct {
    mpz_t n;  /* modulus */
    mpz_t e;  /* public exponent */
} RSA_public_key;

typedef struct {
    mpz_t n;  /* modulus */
    mpz_t e;  /* public exponent */
    mpz_t d;  /* private exponent */
    mpz_t p;  /* prime factor p */
    mpz_t q;  /* prime factor q */
    mpz_t u;  /* inverse of p mod q */
} RSA_secret_key;

void rsa_generate_key(RSA_secret_key *sk, int nbits, const char *d_str) {
    gmp_randstate_t state;
    mpz_t phi, p_minus_1, q_minus_1, gcd;

    gmp_randinit_default(state);
    mpz_init(phi);
    mpz_init(p_minus_1);
    mpz_init(q_minus_1);
    mpz_init(gcd);

    // Generate p and q
    mpz_init(sk->p);
    mpz_init(sk->q);
    mpz_urandomb(sk->p, state, nbits/2);
    mpz_nextprime(sk->p, sk->p);
    do {
        mpz_urandomb(sk->q, state, nbits/2);
        mpz_nextprime(sk->q, sk->q);
    } while (mpz_cmp(sk->p, sk->q) == 0);

    // Compute n = p * q
    mpz_init(sk->n);
    mpz_mul(sk->n, sk->p, sk->q);

    // Compute phi = (p-1)(q-1)
    mpz_sub_ui(p_minus_1, sk->p, 1);
    mpz_sub_ui(q_minus_1, sk->q, 1);
    mpz_mul(phi, p_minus_1, q_minus_1);

    // Choose e
    mpz_init_set_ui(sk->e, 3);

    // Instead of computing d, set it to the provided value
    mpz_init_set_str(sk->d, d_str, 0);  // 0 means auto-detect base


    // Compute d = e^(-1) mod phi
    // mpz_init(sk->d);
    mpz_invert(sk->d, sk->e, phi);

    // Compute u = p^(-1) mod q
    mpz_init(sk->u);
    mpz_invert(sk->u, sk->p, sk->q);

    mpz_clear(phi);
    mpz_clear(p_minus_1);
    mpz_clear(q_minus_1);
    mpz_clear(gcd);
    gmp_randclear(state);
}

// RSA encryption
void rsa_encrypt(mpz_t result, const mpz_t message, const RSA_public_key *pk) {
    // mpz_powm(result, message, pk->e, pk->n);
    modular_exponentiation(result, message, pk->e, pk->n);
}

// RSA decryption
void rsa_decrypt(mpz_t result, const mpz_t ciphertext, const RSA_secret_key *sk) {
    mpz_t m1, m2, h;
    mpz_init(m1);
    mpz_init(m2);
    mpz_init(h);

    // m1 = c^(d mod (p-1)) mod p
    mpz_sub_ui(h, sk->p, 1);
    mpz_mod(h, sk->d, h);
    // mpz_powm(m1, ciphertext, h, sk->p);
    modular_exponentiation(m1, ciphertext, h, sk->p);

    // m2 = c^(d mod (q-1)) mod q
    mpz_sub_ui(h, sk->q, 1);
    mpz_mod(h, sk->d, h);
    // mpz_powm(m2, ciphertext, h, sk->q);
    modular_exponentiation(m2, ciphertext, h, sk->q);

    // h = (m2 - m1) * u mod q
    mpz_sub(h, m2, m1);
    mpz_mul(h, h, sk->u);
    mpz_mod(h, h, sk->q);

    // result = m1 + h * p
    mpz_mul(h, h, sk->p);
    mpz_add(result, m1, h);

    mpz_clear(m1);
    mpz_clear(m2);
    mpz_clear(h);
}

// RSA signature generation
void rsa_sign(mpz_t signature, const mpz_t message, const RSA_secret_key *sk) {
    // Use vulnerable modular exponentiation for signing
    printf("Signing (using private key d):\n");
    modular_exponentiation(signature, message, sk->d, sk->n);
}

// RSA signature verification
int rsa_verify(const mpz_t message, const mpz_t signature, const RSA_public_key *pk) {
    mpz_t calculated_message;
    mpz_init(calculated_message);
    
    mpz_powm(calculated_message, signature, pk->e, pk->n); // Use standard GMP function
    // printf("Verifying (using public key e):\n");
    // modular_exponentiation(calculated_message, signature, pk->e, pk->n);
    
    int result = (mpz_cmp(message, calculated_message) == 0);
    mpz_clear(calculated_message);
    return result;
}


int main() {
    RSA_secret_key sk;
    RSA_public_key pk;
    mpz_t message, ciphertext, decrypted, signature;

    // Generate a key with d = 2^2047 (for a 2048-bit key)
    // This is a 2048-bit number with only the most significant bit set
    // const char* d_str = "20000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000";

    const char *d_str = "0x100000000000000";  

    rsa_generate_key(&sk, 2048, d_str);

    mpz_init_set(pk.n, sk.n);
    mpz_init_set(pk.e, sk.e);


    // // Display public key
    // gmp_printf("Public Key:\n");
    // gmp_printf("n (modulus): %Zd\n", pk.n);
    // gmp_printf("e (public exponent): %Zd\n\n", pk.e);

    // // Display private key
    // gmp_printf("Private Key:\n");
    // gmp_printf("n (modulus): %Zd\n", sk.n);
    // gmp_printf("e (public exponent): %Zd\n", sk.e);
    // gmp_printf("d (private exponent): %Zd\n", sk.d);
    // gmp_printf("p (prime factor): %Zd\n", sk.p);
    // gmp_printf("q (prime factor): %Zd\n", sk.q);
    // gmp_printf("u (inverse of p mod q): %Zd\n\n", sk.u);


    mpz_init_set_str(message, "1234567890", 10);
    mpz_init(ciphertext);
    mpz_init(decrypted);

    // rsa_encrypt(ciphertext, message, &pk);
    // rsa_decrypt(decrypted, ciphertext, &sk);

    // gmp_printf("Original : %Zd\n", message);
    // gmp_printf("Decrypted: %Zd\n", decrypted);


    rsa_sign(signature, message, &sk);
    gmp_printf("Signature: %Zd\n", signature);

    int verified = rsa_verify(message, signature, &pk);
    printf("Signature verified: %s\n", verified ? "Yes" : "No");


    // Clean up
    mpz_clear(message);
    mpz_clear(ciphertext);
    mpz_clear(decrypted);
    // Clear key structures...

    return 0;
}