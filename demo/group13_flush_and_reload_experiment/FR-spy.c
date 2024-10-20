#include "/mastik/demo/config.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mastik/symbol.h>
#include <mastik/fr.h>
#include <mastik/util.h>

#define SAMPLES 5000
#define SLOT 2000
#define THRESHOLD 100

char *monitor[] = {
    "vulnerable_rsa.c:5",  // square 
    "multiply.c:4",  // multiply
    "reduce.c:4"   // reduce
};

int nmonitor = sizeof(monitor)/sizeof(monitor[0]);

void usage(const char *prog) {
    fprintf(stderr, "Usage: %s Modular_Exponentiation\n", prog);
    exit(1);
}

int main(int ac, char **av) {
#ifdef HAVE_DEBUG_SYMBOLS
    char *binary = av[1];
    if (binary == NULL)
        usage(av[0]);

    fr_t fr = fr_prepare();
    for (int i = 0; i < nmonitor; i++) {
        uint64_t offset = sym_getsymboloffset(binary, monitor[i]);
        if (offset == ~0ULL) {
            fprintf(stderr, "Cannot find %s in %s\n", monitor[i], binary);
            exit(1);
        } 
        fr_monitor(fr, map_offset(binary, offset));
    }

    uint16_t *res = malloc(SAMPLES * nmonitor * sizeof(uint16_t));
    for (int i = 0; i < SAMPLES * nmonitor ; i+= 4096/sizeof(uint16_t))
        res[i] = 1;
    fr_probe(fr, res);

    int l = fr_trace(fr, SAMPLES, res, SLOT, THRESHOLD, 500);
    
    // Output results to a CSV file for plotting
    // monitoring results
    FILE *fp = fopen("probe_result.csv", "w");
    fprintf(fp, "Slot,Square,Multiply,Reduce\n");
    for (int i = 0; i < l; i++) {
        fprintf(fp, "%d,%d,%d,%d\n", i, res[i*nmonitor], res[i*nmonitor+1], res[i*nmonitor+2]);
    }
    fclose(fp);

    free(res);
    fr_release(fr);
#else // HAVE_DEBUG_SYMBOLS
    fprintf(stderr, "%s: No support for debug symbols\n", av[0]);
    exit(1);
#endif
}