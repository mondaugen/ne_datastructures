#!/bin/sh
gcc -Iinc test/ne_maxheap_test.c src/ne_maxheap.c -o \
test/ne_maxheap_test.elf -ggdb3
