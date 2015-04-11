#!/bin/sh
gcc -Iinc test/ne_freelastnode_test.c src/ne_maxheap.c -o \
test/ne_freelastnode_test.elf -ggdb3
