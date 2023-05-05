// #include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include "util.h"
#include "msg_logger.h"

#define TESTSIZE 32
#define BASE 0x2000

volatile int test1[TESTSIZE];
volatile int test2[TESTSIZE];

void thread_entry(int cid, int nc)
{

    asm volatile("fence");
    barrier(nc);
    if (cid == 0)
    {
        start_logger();
    }
    barrier(nc);

    if (cid == 0) {
        for (int i = 0; i < TESTSIZE; i++)
        {
            test1[i] = 0x1000 + i;
        }
    }
    if (cid == 1) {
        for (int i = 0; i < TESTSIZE; i++)
        {
            test2[i] = 0x3000 + i;
        }
    }
    if (cid == 0) {
        for (int j = 0; j < TESTSIZE; j++)
        {
            test1[j] = 0x2000 + j;
        }
    }

    if (cid == 1) {
        for (int j = 0; j < TESTSIZE; j++)
        {
            test2[j] = 0x4000 + j;
        }
    }

    barrier(nc);
    if (cid == 0)
    {
        stop_logger();
    }
    barrier(nc);
    if (cid == 0)
    {
        print_log();
    }
    barrier(nc);
    exit(0);
}