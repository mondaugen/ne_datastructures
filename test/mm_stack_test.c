#include <stdio.h> 
#include "mm_stack.h" 

int main (void)
{
    int nums[] = {1,-1,2,-2,3,-3,4,-4};
    MMStack *stack = NULL;
    size_t i;
    for (i = 0; i < 8; ++i) {
        if (nums[i] < 0) {
            stack = MMStack_push(stack, (void*)(nums + i));
        }
    }
    while (stack) {
        void * presult;
        stack = MMStack_pop(stack, &presult);
        printf("%d\n",*((int*)presult));
    }
    return 0;
}
