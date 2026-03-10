#include <stdio.h>
#include <stdlib.h>
#include "cache.h"

/* ================= FIFO ================= */

FIFOCache* createFIFOCache(int capacity) {
    FIFOCache *c = malloc(sizeof(FIFOCache));
    c->capacity = capacity;
    c->size = 0;
    c->front = 0;
    c->rear = -1;
    c->blocks = malloc(sizeof(int) * capacity);
    return c;
}

int accessFIFO(FIFOCache *c, int block) {
    for (int i = 0; i < c->size; i++) {
        int idx = (c->front + i) % c->capacity;
        if (c->blocks[idx] == block) return 1;
    }
    if (c->size < c->capacity) {
        c->rear = (c->rear + 1) % c->capacity;
        c->blocks[c->rear] = block;
        c->size++;
    } else {
        c->front = (c->front + 1) % c->capacity;
        c->rear = (c->rear + 1) % c->capacity;
        c->blocks[c->rear] = block;
    }
    return 0;
}

void freeFIFOCache(FIFOCache *c) {
    free(c->blocks);
    free(c);
}

/* ================= LRU ================= */

Node* createNode(int block) {
    Node *n = malloc(sizeof(Node));
    n->block = block;
    n->prev = n->next = NULL;
    return n;
}

LRUCache* createLRUCache(int capacity, int hashSize) {
    LRUCache *c = malloc(sizeof(LRUCache));
    c->capacity = capacity;
    c->size = 0;
    c->head = c->tail = NULL;
    c->hashSize = hashSize;
    c->hash = calloc(hashSize, sizeof(Node*));
    return c;
}

void removeNode(LRUCache *c, Node *n) {
    if (n->prev) n->prev->next = n->next;
    else c->head = n->next;
    if (n->next) n->next->prev = n->prev;
    else c->tail = n->prev;
}

void insertFront(LRUCache *c, Node *n) {
    n->prev = NULL;
    n->next = c->head;
    if (c->head) c->head->prev = n;
    c->head = n;
    if (!c->tail) c->tail = n;
}

int accessLRU(LRUCache *c, int block) {
    int idx = block % c->hashSize;
    Node *n = c->hash[idx];
    if (n) {
        removeNode(c, n);
        insertFront(c, n);
        return 1;
    }
    Node *newNode = createNode(block);
    if (c->size == c->capacity) {
        Node *lru = c->tail;
        int li = lru->block % c->hashSize;
        removeNode(c, lru);
        c->hash[li] = NULL;
        free(lru);
    } else c->size++;
    insertFront(c, newNode);
    c->hash[idx] = newNode;
    return 0;
}

void freeLRUCache(LRUCache *c) {
    Node *cur = c->head;
    while (cur) {
        Node *tmp = cur;
        cur = cur->next;
        free(tmp);
    }
    free(c->hash);
    free(c);
}

/* ================= LFU ================= */

LFUCache* createLFUCache(int capacity) {
    LFUCache *c = malloc(sizeof(LFUCache));
    c->capacity = capacity;
    c->size = 0;
    c->nodes = malloc(sizeof(LFUNode) * capacity);
    return c;
}

int accessLFU(LFUCache *c, int block) {
    for (int i = 0; i < c->size; i++) {
        if (c->nodes[i].block == block) {
            c->nodes[i].freq++;
            return 1;
        }
    }
    if (c->size < c->capacity) {
        c->nodes[c->size++] = (LFUNode){block, 1};
    } else {
        int min = 0;
        for (int i = 1; i < c->capacity; i++)
            if (c->nodes[i].freq < c->nodes[min].freq)
                min = i;
        c->nodes[min] = (LFUNode){block, 1};
    }
    return 0;
}

void freeLFUCache(LFUCache *c) {
    free(c->nodes);
    free(c);
}

/* ================= OPT ================= */

OPTCache* createOPTCache(int capacity, int *trace, int traceLen) {
    OPTCache *c = malloc(sizeof(OPTCache));
    c->capacity = capacity;
    c->size = 0;
    c->blocks = malloc(sizeof(int) * capacity);
    c->trace = trace;
    c->traceLen = traceLen;
    c->currentIndex = 0;
    return c;
}

int findNextUse(OPTCache *c, int block) {
    for (int i = c->currentIndex + 1; i < c->traceLen; i++)
        if (c->trace[i] == block) return i;
    return 1e9;
}

int accessOPT(OPTCache *c, int block) {
    for (int i = 0; i < c->size; i++)
        if (c->blocks[i] == block) {
            c->currentIndex++;
            return 1;
        }

    if (c->size < c->capacity) {
        c->blocks[c->size++] = block;
    } else {
        int farthest = -1, idx = -1;
        for (int i = 0; i < c->capacity; i++) {
            int next = findNextUse(c, c->blocks[i]);
            if (next > farthest) {
                farthest = next;
                idx = i;
            }
        }
        c->blocks[idx] = block;
    }
    c->currentIndex++;
    return 0;
}

void freeOPTCache(OPTCache *c) {
    free(c->blocks);
    free(c);
}
