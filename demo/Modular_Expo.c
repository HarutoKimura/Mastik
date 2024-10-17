#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <unistd.h>

int square(int x, int p) {
    int result1 = x * x;
    return result1;
}

int multiply(int x, int b) {
    int result2 = x * b;
    return result2;
}

int reduce(int x, int p) {
    int result3 = x % p;
    return result3;
}

int modular_exponentiation(int b, int d, int p) {
    int x = 1;
    int num_bits = (int)floor(log2(d)) + 1;
    for (int i = num_bits - 1; i >= 0; i--) {
        x = square(x, p);
        // printf("suare: %d\n", x);
        x = reduce(x, p);
        // printf("reduce: %d\n", x);
        if (d & (1 << i)){
            x = multiply(x, b);
            // printf("multiply: %d\n", x);
            x = reduce(x, p);
            // printf("reduce: %d\n", x);
        }
    }
    return x;
}

int main(int argc, char *argv[]) {
    if (argc != 4) {
        fprintf(stderr, "Usage: %s <base> <exponent> <modulus>\n", argv[0]);
        return 1;
    }

    int b = atoi(argv[1]);
    int d = atoi(argv[2]);
    int p = atoi(argv[3]);

    int result = 0;

    result = modular_exponentiation(b, d, p);
    printf("result: %d\n", result);

    return 0;
}


// // Volatile array to prevent optimization
// volatile int marker[3] = {0};

// int square(int x, int p) {
//     marker[0] = 1; // Square marker
//     marker[0] = 0;
//     return (x * x);
// }

// int multiply(int x, int b) {
//     marker[1] = 1; // Multiply marker
//     marker[1] = 0;
//     return (x * b);
// }

// int reduce(int x, int p) {
//     marker[2] = 1; // Reduce marker
//     marker[2] = 0;
//     return x % p;
// }

// int modular_exponentiation(int b, int d, int p) {
//     int x = 1;
//     int num_bits = (int)floor(log2(d)) + 1;
//     for (int i = num_bits - 1; i >= 0; i--) {
//         x = square(x, p);
//         x = reduce(x, p);
//         if (d & (1 << i)){
//             x = multiply(x, b);
//             x = reduce(x, p);
//         }
//     }
//     return x;
// }

// int main(int argc, char *argv[]) {
//     if (argc != 4) {
//         fprintf(stderr, "Usage: %s <base> <exponent> <modulus>\n", argv[0]);
//         return 1;
//     }

//     int b = atoi(argv[1]);
//     int d = atoi(argv[2]);
//     int p = atoi(argv[3]);

//     // // Add a delay and print a message before starting
//     // printf("Starting modular exponentiation in 3 seconds...\n");
//     // fflush(stdout);
//     // sleep(3);

//     int result = modular_exponentiation(b, d, p);
    
//     printf("Result: %d\n", result);

//     return 0;
// }
