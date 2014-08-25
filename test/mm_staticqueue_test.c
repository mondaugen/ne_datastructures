#include "mm_staticqueue.h"
#include <stdio.h>

int main (void)
{
    MMStaticQueue *sq = MMStaticQueue_new(5);
    char *strs[] = {
        "why",
        "I",
        "Think",
        "That",
        "guy doesn't",
        "actually",
        "know",
        "as much as",
        "his answer",
        "was long" };
    size_t numstrs = sizeof(strs)/sizeof(char*), i;

    for (i = 0; i < 3; i++) {
        MMStaticQueue_push(sq,strs[i]);
    }

    while (1) {
        void *item;
        item = MMStaticQueue_pop(sq);
        if (!item) {
            break;
        }
        printf("%s, ",(char*)item);
    }

    printf("\n");

    for ( ; i < 10; i++) {
        if (MMStaticQueue_push(sq,strs[i])) {
            break;
        }
    }

    while (1) {
        void *item;
        item = MMStaticQueue_pop(sq);
        if (!item) {
            break;
        }
        printf("%s, ",(char*)item);
    }

    printf("\n");

    free(sq);
    return 0;
}
