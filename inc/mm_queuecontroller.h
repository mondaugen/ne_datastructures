#ifndef MM_QUEUE_CONTROLLER_H
#define MM_QUEUE_CONTROLLER_H
#include <stddef.h> 
#include <string.h> 

/* This kind of Queue can be set up with some array of pointers and manage queue
 * access */
typedef struct __MMQueueController MMQueueController;

struct __MMQueueController {
    size_t size;
    size_t pushAddr;
    size_t popAddr;
    void **items;
};

/* Make the array items into a queue controlled by this queue controller */
static inline void MMQueueController_init(MMQueueController *qc, void **items, size_t size)
{
    memset(qc, 0, sizeof(MMQueueController));
    memset(items, 0, sizeof(void*) * size);
    qc->size = size;
    qc->items = items;
}

static inline int MMQueueController_push(MMQueueController *q, void *item)
{
    if (q->items[q->pushAddr] == NULL) {
        q->items[q->pushAddr] = item;
        q->pushAddr = (q->pushAddr + 1) % q->size;
        return 0;
    }
    return 1; /* Queue is full */
}

static inline void *MMQueueController_pop(MMQueueController *q)
{
    void *result = q->items[q->popAddr];
    if (!result) {
        return NULL; /* Queue empty */
    }
    q->items[q->popAddr] = NULL;
    q->popAddr = (q->popAddr + 1) % q->size;
    return result;
}

#endif /* MM_QUEUE_CONTROLLER_H */
