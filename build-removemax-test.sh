#!/bin/sh
gcc -Iinc test/ne_removemax_test.c src/ne_maxheap.c -o \
test/ne_removemax_test.elf -ggdb3
