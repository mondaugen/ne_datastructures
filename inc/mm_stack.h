#ifndef MM_STACK_H
#define MM_STACK_H 

#include <stdlib.h> 

typedef struct __MMStack MMStack;

struct __MMStack {
    MMStack *next;
    void    *item;
};

static inline MMStack *MMStack_new(void *item)
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

/* The item that is popped off with the top stack item is put in "item" if item
 * not NULL */
static inline MMStack *MMStack_pop(MMStack *stack, void **item)
{
    if (!stack) {
        if (item) {
            *item = NULL;
        }
        return stack;
    }
    MMStack *result = stack->next;
    if (item) {
        *item = stack->item;
    }
    free(stack);
    return result;
}

static inline MMStack *MMStack_free(MMStack *stack)
{
    while (stack) {
        stack = MMStack_pop(stack,NULL);
    }
    return stack;
}

/* This is ugly. */
static inline MMStack *MMStack_freeDeep(MMStack *stack)
{
    while (stack) {
        void *bunk;
        stack = MMStack_pop(stack,&bunk);
        if (bunk) {
            free(bunk);
        }
    }
    return stack;
}

#endif /* MM_STACK_H */
