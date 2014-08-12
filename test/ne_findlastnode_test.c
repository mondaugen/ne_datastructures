#include <stdlib.h> 
#include <stdio.h> 
#include "ne_maxheap.h" 

#define NUM_ARGS 2 

static void buildHeapManager(HeapManager *hm, int *keys, size_t size)
{
    size_t i;
    HeapManager_init(hm);
    for (i = 0; i < size; ++i) {
        HeapNode *hn = (HeapNode*)malloc(sizeof(HeapNode));
        HeapNode_init(hn,keys[i]);
        HeapManager_insertHeapNode(hm,hn,NULL);
    }
}

int main (int argc, char **argv)
{
    /*
    if (argc != NUM_ARGS) {
        perror("Args are %s size-of-heap\n", argv[0]);
        exit(1);
    }
    */
/*     size_t heapSize = atoi(argv[1]); */
    int keys[] = {1,2,3,4,5,6,7};
    HeapManager hm;
    HeapNode **where;
    buildHeapManager(&hm,keys,sizeof(keys)/sizeof(int));
    HeapNode_print(hm.top);
    where = HeapManager_findLastNode(&hm,NULL);
    printf("Last key: %d\n", (*where)->key);

    int keys2[] = {};
    HeapManager hm2;
    buildHeapManager(&hm2,keys2,sizeof(keys2)/sizeof(int));
    HeapNode_print(hm2.top);
    where = HeapManager_findLastNode(&hm2,NULL);
    if (*where) {
        printf("Last key: %d\n", (*where)->key);
    } else {
        printf("Last node is NULL\n");
    }

    int keys3[] = {1};
    HeapManager hm3;
    buildHeapManager(&hm3,keys3,sizeof(keys3)/sizeof(int));
    HeapNode_print(hm3.top);
    MMStack *stack = NULL;
    where = HeapManager_findLastNode(&hm3,&stack);
    if (*where) {
        printf("Last key: %d\n", (*where)->key);
    }
    HeapNode *popOffStack;
    stack = MMStack_pop(stack,(void*)&popOffStack);
    if (popOffStack) {
        /* What is it then? */
        HeapNode_print(popOffStack);
    } else {
        printf("Stack is empty as it should be.\n");
    }

    return 0;
}
