#include <stdlib.h>
#include <stdio.h> 
#include <string.h> 

#define MM_HEAP_DEBUG 

#include "mm_heap.h" 

/* reads arguments from command line, puts them in heap, prints heap as it puts
 * them in and then removes each one in turn, printing the heap each time */

int main(int argc, char **argv)
{
    if (!argv[1]) {
        fprintf(stderr,"Usage: %s key-1 ... key-n\n",argv[0]);
        exit (1);
    }

    char **argstart = argv;
    int result = 1;
    MMHeap_Manager hm;
    memset(&hm,0,sizeof(MMHeap_Manager));
    while (*(++argv)) {
        MMHeap_Node *hn = (MMHeap_Node*)malloc(sizeof(MMHeap_Node));
        memset(hn,0,sizeof(MMHeap_Node));
        hn->key = atoi(*argv);
        MMHeap_Manager_insertMaxHeapNode(&hm,hn);
        MMHeap_Node_print(hm.top);
        printf("Is maxheap? ");
        if (MMHeap_Node_testMaxHeap(hm.top)) {
            printf("Yes\n");
            result = result && 1;
        } else {
            printf("No\n");
            result = result && 0;
        }

    }

    while (hm.top) {
        MMHeap_Node *hn = MMHeap_Manager_removeMax(&hm);
        printf("Current max: %d\n",hn->key);
        MMHeap_Node_print(hm.top);
        printf("Is maxheap? ");
        if (MMHeap_Node_testMaxHeap(hm.top)) {
            printf("Yes\n");
            result = result && 1;
        } else {
            printf("No\n");
            result = result && 0;
        }
        free(hn);
    }

    argv = argstart;

    while (*(++argv)) {
        MMHeap_Node *hn = (MMHeap_Node*)malloc(sizeof(MMHeap_Node));
        memset(hn,0,sizeof(MMHeap_Node));
        hn->key = atoi(*argv);
        MMHeap_Manager_insertMinHeapNode(&hm,hn);
        MMHeap_Node_print(hm.top);
        printf("Is minheap? ");
        if (MMHeap_Node_testMinHeap(hm.top)) {
            printf("Yes\n");
            result = result && 1;
        } else {
            printf("No\n");
            result = result && 0;
        }
    }

    while (hm.top) {
        MMHeap_Node *hn = MMHeap_Manager_removeMin(&hm);
        printf("Current min: %d\n",hn->key);
        MMHeap_Node_print(hm.top);
        printf("Is minheap? ");
        if (MMHeap_Node_testMinHeap(hm.top)) {
            printf("Yes\n");
            result = result && 1;
        } else {
            printf("No\n");
            result = result && 0;
        }
        free(hn);
    }

    if (result) {
        exit(0);
    } else {
        exit(1); /* failed the test! */
    }
}

        
