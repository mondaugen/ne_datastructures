#include "mm_queuecontroller.h"
#include <stdio.h>

int main (void)
{
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
    char *new_strs[numstrs];
    MMQueueController qc;
    MMQueueController_init(&qc,(void**)new_strs,numstrs); 
    for (i = 0; i < 3; i++) {
        MMQueueController_push(&qc,strs[i]);
    }

    while (1) {
        void *item;
        item = MMQueueController_pop(&qc);
        if (!item) {
            break;
        }
        printf("%s, ",(char*)item);
    }

    printf("\n");

    for ( ; i < 10; i++) {
        if (MMQueueController_push(&qc,strs[i])) {
            break;
        }
    }

    /* try pushing to see if it's full */
    char *someMore[] = {"abba", "rabbi", "kona gold", "bonghits69", "douche bag"};
    i = 0;
    while (1) {
        if (MMQueueController_push(&qc,someMore[i++])) {
            printf("QUEUE FULL!\n");
            break;
        }
    }

    while (1) {
        void *item;
        item = MMQueueController_pop(&qc);
        if (!item) {
            break;
        }
        printf("%s, ",(char*)item);
    }

    printf("\n");

    return 0;
}
