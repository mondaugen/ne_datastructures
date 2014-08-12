#ifndef NE_MAXHEAP_H
#define NE_MAXHEAP_H 

#include <stdlib.h>
#include <stdio.h> 
#include <time.h>
#include <string.h> 
#include "mm_stack.h" 

typedef struct __HeapNode       HeapNode;
typedef struct __HeapManager HeapManager;

struct __HeapNode {
    HeapNode  *left;
    HeapNode *right;
    int         key;
};

struct __HeapManager {
    HeapNode  *top;
    int     height;
    int      width;
};

void HeapManager_init(HeapManager *hm);
void HeapNode_init(HeapNode *hn, int key);
void HeapNode_copy(HeapNode *dest, HeapNode *src);
HeapNode *HeapNode_swapLeft(HeapNode *grandparent, HeapNode *parent);
HeapNode *HeapNode_swapRight(HeapNode *grandparent, HeapNode *parent);
HeapNode *HeapNode_swapTopBottom(HeapNode *top, HeapNode *bottomParent, HeapNode *bottom);
HeapNode *HeapNode_maxHeapify(HeapNode *grandparent, HeapNode *parent);
HeapNode **HeapNode_followToBottom(HeapNode *hn, int h, int w, MMStack **stack);
HeapNode *HeapNode_floatUp(MMStack **stack);
int HeapNode_freeLastNode(HeapNode *parent, HeapNode *node);
void HeapManager_incHeapParams(int *h, int *w);
void HeapManager_decHeapParams(int *h, int *w);
HeapNode **HeapManager_findNextEmptyNode(HeapManager *hm, MMStack **stack);
HeapNode **HeapManager_findLastNode(HeapManager *hm, MMStack **stack);
void HeapManager_insertHeapNode(HeapManager *hm, HeapNode *hn, MMStack **stack);
int HeapManager_freeLastNode(HeapManager *hm, MMStack **stack);
void HeapNode_print(HeapNode *hn);
void HeapManager_print(HeapManager *hm);

#endif /* NE_MAXHEAP_H */
