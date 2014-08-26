#ifndef MM_STATICQUEUE_H
#define MM_STATICQUEUE_H 

#include <stdlib.h>

typedef struct __MMStaticQueue MMStaticQueue;

struct __MMStaticQueue {
    size_t size;
    size_t pushAddr;
    size_t popAddr;
    void * items[];
};

static inline MMStaticQueue *MMStaticQueue_new(size_t size)
{
    MMStaticQueue *result = (MMStaticQueue*)calloc(1,sizeof(MMStaticQueue) + size * sizeof(void*));
    result->size = size;
    return result;
}

static inline int MMStaticQueue_push(MMStaticQueue *q, void *item)
{
    if (q->items[q->pushAddr] == NULL) {
        q->items[q->pushAddr] = item;
        q->pushAddr = (q->pushAddr + 1) % q->size;
        return 0;
    }
    return 1; /* Queue is full */
}

static inline void *MMStaticQueue_pop(MMStaticQueue *q)
{
    void *result = q->items[q->popAddr];
    if (!result) {
        return NULL; /* Queue empty */
    }
    q->items[q->popAddr] = NULL;
    q->popAddr = (q->popAddr + 1) % q->size;
    return result;
}

#endif /* MM_STATICQUEUE_H */
