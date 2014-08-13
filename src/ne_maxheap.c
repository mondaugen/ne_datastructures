#include "ne_maxheap.h" 

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

/* Because this swaps the top and bottom, the address of the bottom will no
 * longer be the bottom after the function is called. Furthermore, if the top is
 * also the bottom's parent, which could happen on a heap of height 2, the
 * bottomParent will no longer be correct after swapping. The address of the new
 * top is returned, the address of the new bottomParent and new bottom are put
 * in the respective pointers.
 */
HeapNode *HeapNode_swapTopBottom(HeapNode *top, HeapNode *bottomParent, HeapNode **newBottomParent,
        HeapNode *bottom, HeapNode **newBottom)
{
    if ((!top) || (!bottom) || (bottom->left) || (bottom->right)
            || (bottomParent == bottom)) {
        return NULL;
    }
    if (top == bottom) {
        *newBottomParent = bottomParent;
        *newBottom = top;
        return top;
    }
    if (top == bottomParent) {
        if (bottomParent->left == bottom) {
            bottom->left = top;
            bottom->right = top->right;
        } else if (bottomParent->right == bottom) {
            bottom->right = top;
            bottom->left = top->left;
        } else {
            return NULL; /* bad parent-bottom combination */
        }
        *newBottomParent = bottom;
    } else {
        HeapNode *tmpleft = top->left;
        HeapNode *tmpright = top->right;
        if (bottomParent->left == bottom) {
            bottomParent->left = top;
        } else if (bottomParent->right == bottom) {
            bottomParent->right = top;
        } else {
            return NULL; /* bad parent-bottom combination */
        }
        bottom->left = tmpleft;
        bottom->right = tmpright;
        *newBottomParent = bottomParent;
    }
    top->left = NULL;
    top->right = NULL;
    *newBottom = top;
    return bottom;
}

/* Because *parent may not be the parent it once was (after getting swapped),
 * and it may not be the "top" anymore, this function returns a pointer to the
 * new top of the HeapNode tree.
 */
HeapNode *HeapNode_maxHeapify(HeapNode *grandparent, HeapNode *parent)
{
    HeapNode *top = NULL;
    while (1) {
        if (!parent) {
            return top;
        }
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
            if (!top) { top = parent; }
            return top;
        }
    }
}

HeapNode **HeapNode_followToBottom(HeapNode *hn, int h, int w, MMStack **stack)
{
    int i;
    HeapNode **result;
    result = &hn;
    for (i = h - 1; i >= 0; i--) {
        if (stack) {
            *stack = MMStack_push(*stack,(void*)(*result));
        }
        if (w & (1 << i)) {
            result = &((*result)->right);
        } else {
            result = &((*result)->left);
        }
    }
    return result;
}

/* This will float up the stack, all the way to the top of the heap, even when
 * it is not swapping the places of the nodes (because the tree is already in
 * heap order). */
void HeapNode_floatUp(HeapNode **hn, MMStack **stack)
{
    /* If there's nothing in the stack, we can't do anything. */
    if (!stack || (!*stack)) {
        return;
    }
    HeapNode *parent, *grandparent;
    *stack = MMStack_pop(*stack,(void*) &parent);
    *stack = MMStack_pop(*stack,(void*) &grandparent);
    while (1) {
        parent = HeapNode_maxHeapify(grandparent, parent);
        if (!grandparent) { *hn = parent; return; }
        parent = grandparent;
        *stack = MMStack_pop(*stack,(void**)&grandparent);
    }
}

HeapNode *HeapNode_removeLastNode(HeapNode *parent, HeapNode *node)
{
    if ((!node) || (parent == node)) {
        return NULL; /* please NO */
    }
    if (parent) {
        if (node == parent->left) {
            parent->left = NULL;
        } else if (node == parent->right) {
            parent->right = NULL;
        } else {
            return NULL; /* WTF? */
        }
    }
    return node;
}

int HeapNode_freeLastNode(HeapNode *parent, HeapNode *node)
{
    node = HeapNode_removeLastNode(parent,node);
    if (node) {
        free(node);
    }
    return 0;
}

void HeapManager_incHeapParams(int *h, int *w)
{
    /* The height includes only complete rows so that the width of the current
     * row can be calculated properly */
    if (++(*w) >= (1 << *h)) {
        *w = 0;
        (*h)++;
    }
}

void HeapManager_decHeapParams(int *h, int *w)
{
    /* The height includes only complete rows so that the width of the current
     * row can be calculated properly */
    if (*w == 0) {
        (*h)--;
        *w = (1 << *h) - 1;
    } else {
        (*w)--;
    }
}

HeapNode **HeapManager_findNextEmptyNode(HeapManager *hm, MMStack **stack)
{
    if (!hm->top) { return &(hm->top); }
    return HeapNode_followToBottom(hm->top, hm->height, hm->width, stack);
}

HeapNode **HeapManager_findLastNode(HeapManager *hm, MMStack **stack)
{
    if (!hm->top) { return &(hm->top); }
    int height = hm->height;
    int width  = hm->width;
    HeapManager_decHeapParams(&height, &width);
    return HeapNode_followToBottom(hm->top,height,width,stack);
}

void HeapManager_insertHeapNode(HeapManager *hm, HeapNode *hn, MMStack **stack)
{
    HeapNode **where = HeapManager_findNextEmptyNode(hm, stack);
    *where = hn;
    HeapManager_incHeapParams(&(hm->height), &(hm->width));
}

/* Inserts node and then floats it to its proper position, maintaining a max heap */
void HeapManager_insertMaxHeapNode(HeapManager *hm, HeapNode *hn)
{
    MMStack *stack = NULL;
    HeapManager_insertHeapNode(hm,hn,&stack);
    HeapNode_floatUp(&(hm->top),&stack);
    stack = MMStack_free(stack);
}

int HeapManager_freeLastNode(HeapManager *hm, MMStack **stack)
{
    if (!stack) {
        return 1; /* we need a stack */
    }
    HeapNode **where = HeapManager_findLastNode(hm,stack);
    HeapNode *parent;
    *stack = MMStack_pop(*stack,(void*)&parent);
    if(HeapNode_freeLastNode(parent,*where)) {
        return 1; /* error freeing the node */
    }
    if (!parent) {
        /* node is the top, so now the top is gone */
        hm->top = NULL;
    }
    HeapManager_decHeapParams(&(hm->height),&(hm->width));
    return 0;
}

HeapNode *HeapManager_removeMax(HeapManager *hm)
{
    /* TODO: works pretty well, but hm->top should be set to NULL if it is the last
     * node being removed */
    HeapNode **bottom = NULL,
             *bottomParent = NULL,
             *newBottomParent = NULL,
             *result = NULL,
             *newBottom = NULL;
    MMStack *stack = NULL;
    bottom = HeapManager_findLastNode(hm,&stack);
    stack = MMStack_pop(stack,(void*)&bottomParent);
    hm->top = HeapNode_swapTopBottom(hm->top, bottomParent, &newBottomParent,
            *bottom, &newBottom);
    result = HeapNode_removeLastNode(newBottomParent,newBottom);
    if (!newBottomParent) {
        /* This was the last node, set hm->top to NULL */
        hm->top = NULL;
    }
    HeapManager_decHeapParams(&(hm->height),&(hm->width));
    hm->top = HeapNode_maxHeapify(NULL,hm->top);
    stack = MMStack_free(stack);
    return result;
}

#define __swap(a,b,type) do { type __tmp; __tmp = a; a = b; b = __tmp; } while (0)

void HeapNode_print(HeapNode *hn)
{
    if (!hn) { return; }
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
