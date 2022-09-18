// See LICENSE for license details.

#include <stdlib.h>
#include <stdio.h>
#include "util.h"

#pragma GCC optimize ("unroll-loops")

/* printf output seems to be influenced by:
    - barrier(nc); // make appear/disappear/change core
    - following printfs // make previous printf appear
    if printf is called only for one core in many cases it will not be shown in stdout at all
    ...
*/


void thread_entry(int cid, int nc)
{
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
  exit(0);
}
