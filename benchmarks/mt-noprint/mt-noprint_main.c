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
  if (nc == 0) {
    int x = 0;
  } else if (nc == 1)
  {
    int y = 0;
  }
  asm volatile("fence");
  int z = 1;
  z += 1;
  asm volatile("fence");
  int a = 2;
  a += 1;
  barrier(nc);
  exit(0);
}
