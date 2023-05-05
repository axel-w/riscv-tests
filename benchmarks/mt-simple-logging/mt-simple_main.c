// See LICENSE for license details.

#include <stdlib.h>
#include <stdio.h>
#include "util.h"
#include "msg_logger.h"

#pragma GCC optimize ("unroll-loops")

/* printf output seems to be influenced by:
    - barrier(nc); // make appear/disappear/change core
    - following printfs // make previous printf appear
    if printf is called only for one core in many cases it will not be shown in stdout at all
    ...
*/


void thread_entry(int cid, int nc)
{
  if(cid == 0) {
    start_logger();
  }
  barrier(nc);
  asm volatile("fence");
  barrier(nc);
  printf("C: %d \t", cid);
  barrier(nc);
  printf("C: %d \t", cid);
  barrier(nc);
  printf("C: %d second dummy text\n", cid);
  barrier(nc);
  printf("C: %d third dummy text\n", cid);
  barrier(nc);
  printf("C: %d fourth\n", cid);
  barrier(nc);
  if(cid == 0) {
    stop_logger();
  }
  barrier(nc);
  print_log();

  barrier(nc);
  exit(0);
}
