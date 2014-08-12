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

void HeapManager_init(HeapManager *hm)
{
    hm->top = NULL;
    hm->height = 0;
    hm->width = 0;
}

void HeapNode_init(HeapNode *hn, int key)
{
    hn->left = NULL;
    hn->right = NULL;
    hn->key = key;
}

void HeapNode_copy(HeapNode *dest, HeapNode *src)
{
    memcpy(dest,src,sizeof(HeapNode));
}

/* Swaps parent with its left child. This returns the address of the new parent
 * so that out-of-tree references may be updated, for example in the
 * HeapManager, which stores the top of a tree. It's important to check that the
 * value isn't NULL before updating the reference, because you'll lose your
 * address. Do something like this:
 * HeapNode *tmp;
 * if(tmp = HeapNode_swapLeft(grandparent,reference)) { reference = tmp; }
 */
HeapNode *HeapNode_swapLeft(HeapNode *grandparent, HeapNode *parent)
{
    HeapNode *child = parent->left;
    if (!child) { return NULL; /* child is NULL */ }
    if (grandparent) {
        if (grandparent->right == parent) { grandparent->right = child; }
        else if (grandparent->left == parent) { grandparent->left = child; }
        else { return NULL; /* bad parent */ }
    }
    HeapNode *tmp = parent->right;
    parent->left = child->left;
    parent->right = child->right;
    child->left = parent;
    child->right = tmp;
    return child;
}


HeapNode *HeapNode_swapRight(HeapNode *grandparent, HeapNode *parent)
{
    HeapNode *child = parent->right;
    if (!child) { return NULL; /* child is NULL */ }
    if (grandparent) {
        if (grandparent->right == parent) { grandparent->right = child; }
        else if (grandparent->left == parent) { grandparent->left = child; }
        else { return NULL; /* bad parent */ }
    }
    HeapNode *tmp = parent->left;
    parent->left = child->left;
    parent->right = child->right;
    child->right= parent;
    child->left = tmp;
    return child;
}

/* Because *parent may not be the parent it once was (after getting swapped),
 * and it may not be the "top" anymore, this function returns a pointer to the
 * new top of the HeapNode tree.
 */
HeapNode *HeapNode_maxHeapify(HeapNode *grandparent, HeapNode *parent)
{
    HeapNode *top = NULL;
    while (1) {
        HeapNode *l, *r, *largest;
        l = parent->left;
        r = parent->right;
        if (l && (l->key > parent->key)) { largest = l; }
        else { largest = parent; }
        if (r && (r->key > largest->key)) { largest = r; }
        if (largest != parent) {
            HeapNode *tmp;
            if (largest == l) {
                if(tmp = HeapNode_swapLeft(grandparent,parent)) {
                    if (!top) { top = tmp; }
                    parent = tmp;
                    largest = parent->left;
                }
                else {
                    return tmp;
                }
            }
            else {
                if(tmp = HeapNode_swapRight(grandparent,parent)) {
                    if (!top) { top = tmp; }
                    parent = tmp;
                    largest = parent->right;
                }
                else {
                    return tmp;
                }
            }
            grandparent = parent; parent = largest;
        } else {
            return top;
        }
    }
}

HeapNode **HeapNode_followToBottom(HeapNode *hn, int h, int w)
{
    int i;
    HeapNode **result;
    result = &hn;
    for (i = h - 1; i >= 0; i--) {
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

#define __swap(a,b,type) do { type __tmp; __tmp = a; a = b; b = __tmp; } while (0)

void HeapNode_print(HeapNode *hn)
{
    /* sloppy sloppy sloppy, Just don't make too big a tree! */
    HeapNode **curQ = (HeapNode**)malloc(sizeof(HeapNode*) * 100);
    int curQSize = 0;
    HeapNode **nextQ = (HeapNode**)malloc(sizeof(HeapNode*) * 100);
    int nextQSize = 0;
    curQ[curQSize++] = hn;
    while (1) {
        size_t i;
        for (i = 0; i < curQSize; i++) {
            printf("%d ", curQ[i]->key);
            if (curQ[i]->left) { nextQ[nextQSize++] = curQ[i]->left; }
            if (curQ[i]->right) { nextQ[nextQSize++] = curQ[i]->right; }
            curQ[i] = NULL;
        }
        curQSize = 0;
        printf("\n");
        __swap(curQ,nextQ,HeapNode **);
        __swap(curQSize,nextQSize,int);
        if (curQSize == 0) { break; }
    }
    printf("\n");
    free(curQ); free(nextQ);
}

void HeapManager_print(HeapManager *hm)
{
    printf("HeapManager:\n");
    printf("\tTop: %p\n", hm->top);
    printf("\tHeight: %d\n", hm->height);
    printf("\tWidth: %d\n", hm->width);
}

#define NUM_NODES 10 

int main(void)
{
    int keys[] = {16, 4, 10, 14, 7, 9, 3, 2, 8, 1};
    int keys2[] = {4, 16, 10, 14, 7, 9, 3, 2, 8, 1};
    int keys3[] = {4, 1, 3, 2, 16, 9, 10, 14, 8, 7};
    HeapManager hm;
    HeapManager_init(&hm);
    srand(time(NULL));
    size_t i;
    for (i = 0; i < NUM_NODES; ++i) {
        HeapNode *hn = (HeapNode*)malloc(sizeof(HeapNode));
        hn->key = keys[i];
        hn->left = NULL;
        hn->right = NULL;
        HeapManager_insertHeapNode(&hm,hn);
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
        HeapManager_insertHeapNode(&hm2,hn);
    }

    HeapNode_print(hm2.top);
    if (tmp = HeapNode_maxHeapify(NULL,hm2.top)) { hm2.top = tmp; }
    HeapNode_print(hm2.top);
    return 0;
}

