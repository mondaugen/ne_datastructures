#ifndef MM_STACK_H
#define MM_STACK_H 

#include <stdlib.h> 

typedef struct __MMStack MMStack;

struct __MMStack {
    MMStack *next;
    void    *item;
};

MMStack *MMStack_new(void *item)
{
    MMStack *result = (MMStack*)malloc(sizeof(MMStack));
    result->item = item;
    return result;
}

static inline MMStack *MMStack_push(MMStack *stack, void *item)
{
    MMStack *result = MMStack_new(item);
    result->next = stack;
    return result;
}

/* The item that is popped off with the top stack item is put in "item" */
static inline MMStack *MMStack_pop(MMStack *stack, void **item)
{
    if (!stack) {
        *item = NULL;
        return stack;
    }
    MMStack *result = stack->next;
    *item = stack->item;
    free(stack);
    return result;
}

#endif /* MM_STACK_H */
