#include "mm_stack.h" 
#include "ne_maxheap.h"

#define NUM_NODES 10 

int main(void)
{
    int keys[] = {16, 4, 10, 14, 7, 9, 3, 2, 8, 1};
    int keys2[] = {4, 16, 10, 14, 7, 9, 3, 2, 8, 1};
    int keys3[] = {16, 14, 10, 8, 7, 9, 3, 2, 4, 1};
    int keys4[] = {20, 11, 3, 12};
    HeapManager hm;
    HeapManager_init(&hm);
    srand(time(NULL));
    size_t i;
    for (i = 0; i < NUM_NODES; ++i) {
        HeapNode *hn = (HeapNode*)malloc(sizeof(HeapNode));
        hn->key = keys[i];
        hn->left = NULL;
        hn->right = NULL;
        HeapManager_insertHeapNode(&hm,hn,NULL);
    }
    HeapNode_print(hm.top);
    /* Try out max heapify */
    HeapNode *tmp;
    if (!(tmp = HeapNode_maxHeapify(hm.top,hm.top->left))) { printf("error heapifying\n"); }
    HeapNode_print(hm.top);
    if (tmp = HeapNode_swapLeft(NULL,hm.top)) { hm.top = tmp; }
    /* We should be able to ignore the value here */
    HeapNode_swapLeft(hm.top,hm.top->right);
    HeapNode_print(hm.top);
    hm.top->left = HeapNode_swapRight(hm.top,hm.top->left);
    HeapNode_print(hm.top);
    if(!(tmp = HeapNode_swapRight(hm.top,hm.top->left->right))) {
        printf("error swapping\n");
    }
    HeapNode_print(hm.top);
    if(!(tmp = HeapNode_swapRight(hm.top,hm.top->left->left->left))) {
        printf("error swapping\n");
    }
    HeapManager_print(&hm);


    printf("Some other stuff\n");

    HeapManager hm2;
    HeapManager_init(&hm2);
    for (i = 0; i < NUM_NODES; ++i) {
        HeapNode *hn = (HeapNode*)malloc(sizeof(HeapNode));
        hn->key = keys2[i];
        hn->left = NULL;
        hn->right = NULL;
        HeapManager_insertHeapNode(&hm2,hn,NULL);
    }

    HeapNode_print(hm2.top);
    if (tmp = HeapNode_maxHeapify(NULL,hm2.top)) { hm2.top = tmp; }
    HeapNode_print(hm2.top);
    if (!HeapNode_maxHeapify(NULL,NULL)) {
        printf("Parent is NULL on call to maxHeapify.\n");
    }

    printf("Now the real deal: can we insert properly?\n");

    HeapManager hm3;
    HeapManager_init(&hm3);
    /*
    for (i = 0; i < NUM_NODES; ++i) {
        HeapNode *hn = (HeapNode*)malloc(sizeof(HeapNode));
        hn->key = keys3[i];
        hn->left = NULL;
        hn->right = NULL;
        HeapManager_insertHeapNode(&hm3,hn,NULL);
    }
    HeapNode_print(hm3.top);
    */
    for (i = 0; i < 4; ++i) {
        MMStack *stack = NULL;
        HeapNode *bighn = (HeapNode*)malloc(sizeof(HeapNode));
        bighn->key = keys4[i];
        bighn->left = NULL;
        bighn->right = NULL;
        HeapManager_insertHeapNode(&hm3,bighn,&stack);
        HeapNode_print(hm3.top);
        HeapNode_floatUp(&(hm3.top),(&stack));
        HeapNode_print(hm3.top);
    }

    /* TODO: remove last node and then remove maximum node. */
    return 0;
}

