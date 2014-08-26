#ifndef MM_HEAP_H
#define MM_HEAP_H 

#include "mm_heap_conf.h" 
#include "mm_stack.h" 

#ifndef MM_HEAP_KEY_TYPE
 #define MM_HEAP_KEY_TYPE int
#endif

#ifndef MM_HEAP_MANAGER_SIZE_TYPE
 #define MM_HEAP_MANAGER_SIZE_TYPE int
#endif

#ifndef MM_HEAP_KEY_GT
 #define MM_HEAP_KEY_GT(a,b) ((a) > (b))
#endif 

#ifndef MM_HEAP_KEY_GTE
 #define MM_HEAP_KEY_GTE(a,b) ((a) >= (b))
#endif 

#ifndef MM_HEAP_KEY_LT
 #define MM_HEAP_KEY_LT(a,b) ((a) < (b))
#endif

#ifndef MM_HEAP_KEY_LTE
 #define MM_HEAP_KEY_LTE(a,b) ((a) <= (b))
#endif 

#ifndef MM_HEAP_MANAGER_SIZE_PRE_DEC 
 #define MM_HEAP_MANAGER_SIZE_PRE_DEC(a) (--(a))
#endif 

#ifndef MM_HEAP_MANAGER_SIZE_POST_DEC 
 #define MM_HEAP_MANAGER_SIZE_POST_DEC(a) ((a)--)
#endif 

#ifndef MM_HEAP_MANAGER_SIZE_PRE_INC 
 #define MM_HEAP_MANAGER_SIZE_PRE_INC(a) (++(a))
#endif 

#ifndef MM_HEAP_MANAGER_SIZE_POST_INC 
 #define MM_HEAP_MANAGER_SIZE_POST_INC(a) ((a)++)
#endif

#ifndef MM_HEAP_MANAGER_SIZE_LSHIFT 
 #define MM_HEAP_MANAGER_SIZE_LSHIFT(a,b) ((a) << (b))
#endif

#ifndef MM_HEAP_MANAGER_SIZE_GTE
 #define MM_HEAP_MANAGER_SIZE_GTE(a,b) ((a) >= (b))
#endif

#ifndef MM_HEAP_MANAGER_SIZE_EQ
 #define MM_HEAP_MANAGER_SIZE_EQ(a,b) ((a) == (b))
#endif

#ifndef MM_HEAP_MANAGER_SIZE_BITAND
 #define MM_HEAP_MANAGER_SIZE_BITAND(a,b) ((a) & (b))
#endif

#ifndef MM_SWAP
 #define MM_SWAP(a,b,type) do { type __tmp; __tmp = a; a = b; b = __tmp; } while (0) 
#endif  

typedef MM_HEAP_KEY_TYPE                    MMHeap_Key;
typedef struct __MMHeap_Node                 MMHeap_Node;
typedef MM_HEAP_MANAGER_SIZE_TYPE  MMHeap_Manager_Size;
typedef struct __MMHeap_Manager           MMHeap_Manager;

struct __MMHeap_Node {
    MMHeap_Node   *left;
    MMHeap_Node  *right;
    MMHeap_Key key;
    void       *data;
};

struct __MMHeap_Manager {
    MMHeap_Node           *top;
    MMHeap_Manager_Size height;
    MMHeap_Manager_Size  width;
}; 

void MMHeap_freeAll(MMHeap_Node *top);
void MMHeap_Manager_insertMaxHeapNode(MMHeap_Manager *hm, MMHeap_Node *hn);
MMHeap_Node *MMHeap_Manager_removeMax(MMHeap_Manager *hm);
void MMHeap_Manager_insertMinHeapNode(MMHeap_Manager *hm, MMHeap_Node *hn);
MMHeap_Node *MMHeap_Manager_removeMin(MMHeap_Manager *hm);

#ifdef MM_HEAP_DEBUG 
static inline void MMHeap_Node_print(MMHeap_Node *hn)
{
    if (!hn) { return; }
    /* sloppy sloppy sloppy, Just don't make too big a tree! */
    MMHeap_Node **curQ = (MMHeap_Node**)malloc(sizeof(MMHeap_Node*) * 100);
    int curQSize = 0;
    MMHeap_Node **nextQ = (MMHeap_Node**)malloc(sizeof(MMHeap_Node*) * 100);
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
        MM_SWAP(curQ,nextQ,MMHeap_Node **);
        MM_SWAP(curQSize,nextQSize,int);
        if (curQSize == 0) { break; }
    }
    printf("\n");
    free(curQ); free(nextQ);
}

static inline int MMHeap_Node_testMaxHeap(MMHeap_Node *hn)
{
    int result = 1;
    if (!hn) { return result; }
    if (hn->left) {
        result = result && (MM_HEAP_KEY_GTE(hn->key,hn->left->key))
            && (MMHeap_Node_testMaxHeap(hn->left));
    }
    if (hn->right) {
        result = result && (MM_HEAP_KEY_GTE(hn->key,hn->right->key))
            && (MMHeap_Node_testMaxHeap(hn->right));
    }
    return result;
}

static inline int MMHeap_Node_testMinHeap(MMHeap_Node *hn)
{
    int result = 1;
    if (!hn) { return result; }
    if (hn->left) {
        result = result && (MM_HEAP_KEY_LTE(hn->key,hn->left->key))
            && (MMHeap_Node_testMinHeap(hn->left));
    }
    if (hn->right) {
        result = result && (MM_HEAP_KEY_LTE(hn->key,hn->right->key))
            && (MMHeap_Node_testMinHeap(hn->right));
    }
    return result;
}

static inline void MMHeap_Manager_print(MMHeap_Manager *hm)
{
    printf("MMHeap_Manager:\n");
    printf("\tTop: %p\n", hm->top);
    printf("\tHeight: %d\n", hm->height);
    printf("\tWidth: %d\n", hm->width);
}
#endif  

#endif /* MM_HEAP_H */
