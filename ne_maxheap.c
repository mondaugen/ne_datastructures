#include <stdlib.h>
#include <stdio.h> 
#include <time.h> 

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

void HeapManager_init(HeapManager *hm)
{
    hm->top = NULL;
    hm->height = 0;
    hm->width = 0;
}

HeapNode **HeapNode_followToBottom(HeapNode *hn, int h, int w)
{
    int i;
    HeapNode **result;
    result = &hn;
    for (i = 0; i < h; ++i) {
        if (w & (1 << i)) {
            result = &((*result)->right);
        } else {
            result = &((*result)->left);
        }
    }
    return result;
}

void HeapManager_incHeapParams(int *h, int *w)
{
    if (++(*w) >= (1 << *h)) {
        *w = 0;
        (*h)++;
    }
}

HeapNode **HeapManager_findNextEmptyNode(HeapManager *hm)
{
    if (!hm->top) { return &(hm->top); }
    return HeapNode_followToBottom(hm->top, hm->height, hm->width);
}

void HeapManager_insertHeapNode(HeapManager *hm, HeapNode *hn)
{
    HeapNode **where = HeapManager_findNextEmptyNode(hm);
    *where = hn;
    HeapManager_incHeapParams(&(hm->height), &(hm->width));
}

#define __swap(a,b,type) do { type tmp; tmp = a; a = b; b = tmp; } while (0)

void HeapNode_print(HeapNode *hn)
{
    /* sloppy sloppy sloppy, Just don't make too big a tree! */
    HeapNode **curQ = (HeapNode**)malloc(sizeof(HeapNode*) * 100);
    int curQSize = 0;
    HeapNode **nextQ = (HeapNode**)malloc(sizeof(HeapNode*) * 100);
    int nextQSize = 0;
    curQ[curQSize++] = hn;
    while (1) {
        while (curQSize) {
            curQSize--;
            printf("%d ", curQ[curQSize]->key);
            if (curQ[curQSize]->left) { nextQ[nextQSize++] = curQ[curQSize]->left; }
            if (curQ[curQSize]->right) { nextQ[nextQSize++] = curQ[curQSize]->right; }
            curQ[curQSize] = NULL;
        }
        printf("\n");
        __swap(curQ,nextQ,HeapNode **);
        __swap(curQSize,nextQSize,int);
        if (curQSize == 0) { break; }
    }
    free(curQ); free(nextQ);
}

void HeapManager_print(HeapManager *hm)
{
    printf("HeapManager:\n");
    printf("\tTop: %p\n", hm->top);
    printf("\tHeight: %d\n", hm->height);
    printf("\tWidth: %d\n", hm->width);
}

#define NUM_NODES 100

int main(void)
{
    HeapManager hm;
    HeapManager_init(&hm);
    srand(time(NULL));
    size_t i;
    for (i = 0; i < NUM_NODES; ++i) {
        HeapNode *hn = (HeapNode*)malloc(sizeof(HeapNode));
        hn->key = rand() % 100;
        hn->left = NULL;
        hn->right = NULL;
        HeapManager_insertHeapNode(&hm,hn);
    }
    HeapNode_print(hm.top);
    HeapManager_print(&hm);
    return 0;
}

