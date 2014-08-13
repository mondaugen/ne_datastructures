#!/bin/sh
gcc -Iinc test/mm_heap_test.c src/mm_heap.c -o \
test/mm_heap_test.elf -ggdb3
