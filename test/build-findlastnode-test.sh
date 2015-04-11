#!/bin/sh
gcc -Iinc test/ne_findlastnode_test.c src/ne_maxheap.c -o \
test/ne_findlastnode_test.elf -ggdb3
