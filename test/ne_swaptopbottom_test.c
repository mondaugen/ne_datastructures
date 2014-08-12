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
    int keys[] = {1,2};
    HeapManager hm;
    buildHeapManager(&hm,keys,sizeof(keys)/sizeof(int));
    HeapNode_print(hm.top);
    hm.top = HeapNode_swapTopBottom(hm.top,hm.top,hm.top->left);
    HeapNode_print(hm.top);

    int keys2[] = {1,2,3};
    HeapManager hm2;
    buildHeapManager(&hm2,keys2,sizeof(keys2)/sizeof(int));
    HeapNode_print(hm2.top);
    hm2.top = HeapNode_swapTopBottom(hm2.top,hm2.top,hm2.top->right);
    if (!HeapNode_swapTopBottom(hm2.top,hm2.top->left,hm2.top->right)) {
        fputs("Bad parent/child combination\n",stderr);
    }
    if (!HeapNode_swapTopBottom(hm2.top,hm2.top->left,hm2.top->left)) {
        fputs("Bad parent/child combination\n",stderr);
    }
    HeapNode_print(hm2.top);

    int keys3[] = {1,2,3,4,5,6};
    HeapManager hm3;
    buildHeapManager(&hm3,keys3,sizeof(keys3)/sizeof(int));
    HeapNode_print(hm3.top);
    hm3.top = HeapNode_swapTopBottom(hm3.top,hm3.top->right,hm3.top->right->left);
    HeapNode_print(hm3.top);

    return 0;
}
