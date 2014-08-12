#include <stdlib.h> 
#include <stdio.h> 
#include <assert.h> 
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
    MMStack *stack = NULL;
    HeapManager_freeLastNode(&hm,&stack);
    HeapNode_print(hm.top);
    stack = MMStack_free(stack);
    HeapManager_freeLastNode(&hm,&stack);
    HeapNode_print(hm.top);
    while (hm.top) {
        stack = MMStack_free(stack);
        HeapManager_freeLastNode(&hm,&stack);
        HeapNode_print(hm.top);
    }
    printf("Done freeing\n");
    stack = MMStack_free(stack);

    int keys2[] = {};
    HeapManager hm2;
    buildHeapManager(&hm2,keys2,sizeof(keys2)/sizeof(int));
    HeapNode_print(hm2.top);
    HeapManager_freeLastNode(&hm2,&stack);
    HeapNode_print(hm2.top);
    stack = MMStack_free(stack);

    int keys3[] = {1};
    HeapManager hm3;
    buildHeapManager(&hm3,keys3,sizeof(keys3)/sizeof(int));
    HeapNode_print(hm3.top);
    HeapManager_freeLastNode(&hm3,&stack);
    HeapNode_print(hm3.top);
    stack = MMStack_free(stack);

    return 0;
}
