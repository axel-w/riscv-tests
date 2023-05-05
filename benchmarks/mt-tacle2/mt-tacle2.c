// See LICENSE for license details.

#include <stdlib.h>
#include <stdio.h>
#include "util.h"
#include "msg_counter.h"
#include "tasks.h"

#pragma GCC optimize ("unroll-loops")

void thread_entry(int cid, int nc)
{
  asm volatile("fence");
  barrier(nc);
  if (cid == 0) {
    start_counter();
  }
  barrier(nc);
  
  if (cid == 0) {
    binarysearch_task();
  }
  if (cid == 1) {
    bitcount_task();
  }

  barrier(nc);
  if (cid == 0) {
    stop_counter();
  }
  barrier(nc);
  // if (cid == 0) {
  //   print_counts();
  // }
  // barrier(nc);
  exit(0);
}
