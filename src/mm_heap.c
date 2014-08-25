#include "mm_heap.h" 
#include <string.h> 

void MMHeap_swap(MMHeap_Node *a, MMHeap_Node *b)
{
    MM_SWAP(a->data,b->data,void*);
    MM_SWAP(a->key,b->key, MMHeap_Key);
}


void MMHeap_makeMaxHeap(MMHeap_Node *hn)
{
    if (!hn) { return; }
    while(1) {
        MMHeap_Node *l, *r, *largest;
        l = hn->left;
        r = hn->right;
        if (l && (MM_HEAP_KEY_GT(l->key,hn->key))) {
            largest = l;
        } else {
            largest = hn;
        }
        if (r && (MM_HEAP_KEY_GT(r->key,largest->key))) {
            largest = r;
        }
        if (largest != hn) {
            MMHeap_swap(hn,largest);
            hn = largest;
        } else {
            return;
        }
    }
}

void MMHeap_makeMinHeap(MMHeap_Node *hn)
{
    if (!hn) { return; }
    while(1) {
        MMHeap_Node *l, *r, *smallest;
        l = hn->left;
        r = hn->right;
        if (l && (MM_HEAP_KEY_LT(l->key,hn->key))) {
            smallest = l;
        } else {
            smallest = hn;
        }
        if (r && (MM_HEAP_KEY_LT(r->key,smallest->key))) {
            smallest = r;
        }
        if (smallest != hn) {
            MMHeap_swap(hn,smallest);
            hn = smallest;
        } else {
            return;
        }
    }
}

MMHeap_Node **MMHeap_Node_followToBottom(MMHeap_Node *hn, MMHeap_Manager_Size h,
        MMHeap_Manager_Size w, MMStack **stack)
{
    MMHeap_Manager_Size i;
    MMHeap_Node **result;
    result = &hn;
    for (i = MM_HEAP_MANAGER_SIZE_PRE_DEC(h);
            MM_HEAP_MANAGER_SIZE_GTE(i,0);
            MM_HEAP_MANAGER_SIZE_POST_DEC(i)) {
        if (stack) {
            *stack = MMStack_push(*stack,(void*)(*result));
        }
        if (MM_HEAP_MANAGER_SIZE_BITAND(w,MM_HEAP_MANAGER_SIZE_LSHIFT(1,i))) {
            result = &((*result)->right);
        } else {
            result = &((*result)->left);
        }
    }
    return result;
}

/* Given a stack of nodes, move through the stack, calling makeMaxHeap on each
 * node in the stack. */
void MMHeap_floatUp(MMStack **stack)
{
    MMHeap_Node *hn;
    while (1) {
        *stack = MMStack_pop(*stack,(void*)&hn);
        if (!hn) { return; }
        MMHeap_makeMaxHeap(hn);
    }
}

/* Given a stack of nodes, move through the stack, calling makeMaxHeap on each
 * node in the stack. */
void MMHeap_floatDown(MMStack **stack)
{
    MMHeap_Node *hn;
    while (1) {
        *stack = MMStack_pop(*stack,(void*)&hn);
        if (!hn) { return; }
        MMHeap_makeMinHeap(hn);
    }
}


MMHeap_Node *MMHeap_Node_removeLastNode(MMHeap_Node *parent, MMHeap_Node *node)
{
    if ((!node) || (parent == node) || (node->left) || (node->right)) {
        return NULL; /* Don't remove if node is NULL, parent is node or node is not actually last */
    }
    if (parent) {
        if (node == parent->left) {
            parent->left = NULL;
        } else if (node == parent->right) {
            parent->right = NULL;
        } else {
            return NULL; /* What is it then? */
        }
    }
    return node;
}

void MMHeap_Manager_init(MMHeap_Manager *hm)
{
    memset(hm,0,sizeof(hm));
}

void MMHeap_Manager_incHeapParams(MMHeap_Manager_Size *h, MMHeap_Manager_Size *w)
{
    /* The height includes only complete rows so that the width of the current
     * row can be calculated properly */
    if (MM_HEAP_MANAGER_SIZE_GTE(MM_HEAP_MANAGER_SIZE_PRE_INC(*w),
                MM_HEAP_MANAGER_SIZE_LSHIFT(1,*h))) {
        *w = 0;
        MM_HEAP_MANAGER_SIZE_POST_INC(*h);
    }
}

void MMHeap_Manager_decHeapParams(MMHeap_Manager_Size *h,MMHeap_Manager_Size *w)
{
    /* The height includes only complete rows so that the width of the current
     * row can be calculated properly */
    if (MM_HEAP_MANAGER_SIZE_EQ(*w,0)) {
        MM_HEAP_MANAGER_SIZE_POST_DEC(*h);
        *w = MM_HEAP_MANAGER_SIZE_LSHIFT(1,*h);
    }
    MM_HEAP_MANAGER_SIZE_POST_DEC(*w);
}

MMHeap_Node **MMHeap_Manager_findNextEmptyNode(MMHeap_Manager *hm, MMStack **stack)
{
    if (!hm->top) { return &(hm->top); }
    return MMHeap_Node_followToBottom(hm->top, hm->height, hm->width, stack);
}

MMHeap_Node **MMHeap_Manager_findLastNode(MMHeap_Manager *hm, MMStack **stack)
{
    if (!hm->top) { return &(hm->top); }
    MMHeap_Manager_Size height = hm->height;
    MMHeap_Manager_Size width  = hm->width;
    MMHeap_Manager_decHeapParams(&height, &width);
    return MMHeap_Node_followToBottom(hm->top,height,width,stack);
}

void MMHeap_Manager_insertHeapNode(MMHeap_Manager *hm, MMHeap_Node *hn, MMStack **stack)
{
    MMHeap_Node **where = MMHeap_Manager_findNextEmptyNode(hm, stack);
    *where = hn;
    MMHeap_Manager_incHeapParams(&(hm->height), &(hm->width));
}

/* Inserts node and then floats it to its proper position, maintaining a max heap */
void MMHeap_Manager_insertMaxHeapNode(MMHeap_Manager *hm, MMHeap_Node *hn)
{
    MMStack *stack = NULL;
    MMHeap_Manager_insertHeapNode(hm,hn,&stack);
    MMHeap_floatUp(&stack);
    stack = MMStack_free(stack);
}

/* Inserts node and then floats it to its proper position, maintaining a max heap */
void MMHeap_Manager_insertMinHeapNode(MMHeap_Manager *hm, MMHeap_Node *hn)
{
    MMStack *stack = NULL;
    MMHeap_Manager_insertHeapNode(hm,hn,&stack);
    MMHeap_floatDown(&stack);
    stack = MMStack_free(stack);
}

MMHeap_Node *MMHeap_Manager_removeMax(MMHeap_Manager *hm)
{
    MMStack *stack = NULL;
    MMHeap_Node **bottom, *bottomParent, *result;
    bottom = MMHeap_Manager_findLastNode(hm,&stack);
    MMHeap_swap(hm->top,*bottom);
    stack = MMStack_pop(stack,(void*)&bottomParent);
    result = MMHeap_Node_removeLastNode(bottomParent,*bottom);
    if (!bottomParent) {
        /* This was the last node, set hm->top to NULL */
        hm->top = NULL;
    }
    MMHeap_Manager_decHeapParams(&(hm->height), &(hm->width));
    MMHeap_makeMaxHeap(hm->top);
    stack = MMStack_free(stack);
    return result;
}

MMHeap_Node *MMHeap_Manager_removeMin(MMHeap_Manager *hm)
{
    MMStack *stack = NULL;
    MMHeap_Node **bottom, *bottomParent, *result;
    bottom = MMHeap_Manager_findLastNode(hm,&stack);
    MMHeap_swap(hm->top,*bottom);
    stack = MMStack_pop(stack,(void*)&bottomParent);
    result = MMHeap_Node_removeLastNode(bottomParent,*bottom);
    if (!bottomParent) {
        /* This was the last node, set hm->top to NULL */
        hm->top = NULL;
    }
    MMHeap_Manager_decHeapParams(&(hm->height), &(hm->width));
    MMHeap_makeMinHeap(hm->top);
    stack = MMStack_free(stack);
    return result;
}
