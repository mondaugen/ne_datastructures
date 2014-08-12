#!/bin/sh
gcc -Iinc test/ne_swaptopbottom_test.c src/ne_maxheap.c -o \
test/ne_swaptopbottom_test.elf -ggdb3
