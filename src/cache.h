#ifndef CACHE_H
#define CACHE_H

/* ================= POLICY ENUM ================= */

typedef enum {
    FIFO,
    LRU,
    LFU,
    OPT
} Policy;

/* ================= FIFO ================= */

typedef struct {
    int capacity, size;
    int front, rear;
    int *blocks;
} FIFOCache;

FIFOCache* createFIFOCache(int capacity);
int accessFIFO(FIFOCache *cache, int block);
void freeFIFOCache(FIFOCache *cache);

/* ================= LRU ================= */

typedef struct Node {
    int block;
    struct Node *prev;
    struct Node *next;
} Node;

typedef struct {
    int capacity, size;
    Node *head, *tail;
    Node **hash;
    int hashSize;
} LRUCache;

LRUCache* createLRUCache(int capacity, int hashSize);
int accessLRU(LRUCache *cache, int block);
void freeLRUCache(LRUCache *cache);

/* ================= LFU ================= */

typedef struct {
    int block;
    int freq;
} LFUNode;

typedef struct {
    int capacity, size;
    LFUNode *nodes;
} LFUCache;

LFUCache* createLFUCache(int capacity);
int accessLFU(LFUCache *cache, int block);
void freeLFUCache(LFUCache *cache);

/* ================= OPT ================= */

typedef struct {
    int capacity, size;
    int *blocks;
    int *trace;
    int traceLen;
    int currentIndex;
} OPTCache;

OPTCache* createOPTCache(int capacity, int *trace, int traceLen);
int accessOPT(OPTCache *cache, int block);
void freeOPTCache(OPTCache *cache);

#endif
