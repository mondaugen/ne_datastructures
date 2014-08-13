#include <stdlib.h> 
#include <stdio.h> 
#include <assert.h> 
#include "ne_maxheap.h" 

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
    HeapManager hm;
    HeapManager_init(&hm);
    int keys[] = {1,2,3,4,5,6,7};
    size_t i;
    for (i = 0; i < sizeof(keys)/sizeof(int); i++) {
        HeapNode *hn = (HeapNode*)malloc(sizeof(HeapNode));
        HeapNode_init(hn,keys[i]);
        HeapManager_insertMaxHeapNode(&hm,hn);
        HeapNode_print(hm.top);
    }
    while (hm.top) {
        HeapNode *hn = HeapManager_removeMax(&hm);
        printf("Current max: %d\n",hn->key);
        HeapNode_print(hm.top);
        free(hn);
    }

    return 0;
}
