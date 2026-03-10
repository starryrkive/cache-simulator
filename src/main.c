#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cache.h"

void printResult(const char *name, int hits, int total) {
    double ratio = (double)hits / total;
    printf("%s | Hits: %d | Misses: %d | Hit Ratio: %.3f\n",
           name, hits, total - hits, ratio);
}

int main(int argc, char *argv[]) {

    if (argc < 4) {
        printf("Usage:\n");
        printf("./cache_sim POLICY CAPACITY TRACE...\n");
        printf("Example:\n");
        printf("./cache_sim LRU 3 1 2 3 1 4 2\n");
        return 1;
    }

    char *policy = argv[1];
    int capacity = atoi(argv[2]);

    int n = argc - 3;
    int *trace = malloc(sizeof(int) * n);

    for (int i = 0; i < n; i++)
        trace[i] = atoi(argv[i + 3]);

    int hits = 0;

    /* FIFO */
    if (strcmp(policy, "FIFO") == 0) {

        FIFOCache *c = createFIFOCache(capacity);

        for (int i = 0; i < n; i++)
            if (accessFIFO(c, trace[i])) hits++;

        printResult("FIFO", hits, n);
        freeFIFOCache(c);
    }

    /* LRU */
    else if (strcmp(policy, "LRU") == 0) {

        LRUCache *c = createLRUCache(capacity, 100);

        for (int i = 0; i < n; i++)
            if (accessLRU(c, trace[i])) hits++;

        printResult("LRU", hits, n);
        freeLRUCache(c);
    }

    /* LFU */
    else if (strcmp(policy, "LFU") == 0) {

        LFUCache *c = createLFUCache(capacity);

        for (int i = 0; i < n; i++)
            if (accessLFU(c, trace[i])) hits++;

        printResult("LFU", hits, n);
        freeLFUCache(c);
    }

    /* OPT */
    else if (strcmp(policy, "OPT") == 0) {

        OPTCache *c = createOPTCache(capacity, trace, n);

        for (int i = 0; i < n; i++)
            if (accessOPT(c, trace[i])) hits++;

        printResult("OPT", hits, n);
        freeOPTCache(c);
    }

    /* COMPARE */
    else if (strcmp(policy, "COMPARE") == 0) {

        printf("Policy Comparison\n");
        printf("------------------\n");

        FIFOCache *f = createFIFOCache(capacity);
        hits = 0;
        for (int i = 0; i < n; i++)
            if (accessFIFO(f, trace[i])) hits++;
        printResult("FIFO", hits, n);
        freeFIFOCache(f);

        LRUCache *l = createLRUCache(capacity, 100);
        hits = 0;
        for (int i = 0; i < n; i++)
            if (accessLRU(l, trace[i])) hits++;
        printResult("LRU", hits, n);
        freeLRUCache(l);

        LFUCache *lf = createLFUCache(capacity);
        hits = 0;
        for (int i = 0; i < n; i++)
            if (accessLFU(lf, trace[i])) hits++;
        printResult("LFU", hits, n);
        freeLFUCache(lf);

        OPTCache *o = createOPTCache(capacity, trace, n);
        hits = 0;
        for (int i = 0; i < n; i++)
            if (accessOPT(o, trace[i])) hits++;
        printResult("OPT", hits, n);
        freeOPTCache(o);
    }

    else {
        printf("Invalid policy.\n");
    }

    free(trace);
    return 0;
}