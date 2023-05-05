#include <stdlib.h>
#include <stdio.h>
#include "util.h"
#include "msg_counter.h"
#include "mt-tasks.h"

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
    task_0();
  }
  if (cid == 1) {
    // task_1();
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
