// See LICENSE for license details.

#include <stdlib.h>
#include <stdio.h>
#include "util.h"
#include "msg_counter.h"
#include "mmio.h"

#pragma GCC optimize ("unroll-loops")


volatile int count0 = -1;
volatile int count1 = -1;

void iir_init( void );
int iir_return( void );
void iir_main( void );
int task_iir( void );
 

// iir bench:

/*
  Declaration of global variables
*/

volatile float iir_wi[ 2 * 4 ];
volatile float iir_coefficients[ 5 * 4 ];
float iir_x;


/*
  Initialization- and return-value-related functions
*/

void iir_init( void )
{
  int f;
  unsigned int i;
  unsigned char *p;
  volatile char bitmask = 0;


  _Pragma( "loopbound min 20 max 20" )
  for ( f = 0 ; f < 5 * 4; f++ )
    iir_coefficients[ f ] = 7;

  _Pragma( "loopbound min 8 max 8" )
  for ( f = 0 ; f < 2 * 4; f++ )
    iir_wi[ f ] = 0;

  iir_x = ( float ) 1;

  /*
    Apply volatile XOR-bitmask to entire input array.
  */
  p = ( unsigned char * ) &iir_coefficients[ 0 ];
  _Pragma( "loopbound min 80 max 80" )
  for ( i = 0; i < sizeof( iir_coefficients ); ++i, ++p )
    *p ^= bitmask;

  p = ( unsigned char * ) &iir_wi[ 0 ];
  _Pragma( "loopbound min 32 max 32" )
  for ( i = 0; i < sizeof( iir_wi ); ++i, ++p )
    *p ^= bitmask;
}


int iir_return( void )
{
  float checksum = 0.0;
  int f;


  _Pragma( "loopbound min 8 max 8" )
  for ( f = 0 ; f < 2 * 4; f++ )
    checksum += iir_wi[ f ];

  return ( ( int ) checksum );
}


/*
  Main functions
*/

void _Pragma( "entrypoint" ) iir_main( void )
{
  register float w;
  int f;
  register volatile float *ptr_coeff, *ptr_wi1, *ptr_wi2;
  register float y;


  ptr_coeff = &iir_coefficients[ 0 ];
  ptr_wi1 = &iir_wi[ 0 ];
  ptr_wi2 = &iir_wi[ 1 ];

  y = iir_x ;

  _Pragma( "loopbound min 4 max 4" )
  for ( f = 0 ; f < 4 ; f++ ) {
    w = y - *ptr_coeff++ * *ptr_wi1;
    w -= *ptr_coeff++ * *ptr_wi2;

    y = *ptr_coeff++ * w;
    y += *ptr_coeff++ * *ptr_wi1;
    y += *ptr_coeff++ * *ptr_wi2;

    *ptr_wi2++ = *ptr_wi1;
    *ptr_wi1++ = w;

    ptr_wi2++;
    ptr_wi1++;
  }
}


int task_iir( void )
{
  iir_init();
  iir_main();

  return ( iir_return() - 400 != 0 );
}


// fac bench:

int fac_fac( int n );
void fac_init();
int fac_return();
void fac_main();
int task_fac( void );
/*
  Declaration of global variables
*/

int fac_s;
volatile int fac_n;


/*
  Initialization- and return-value-related functions
*/


void fac_init()
{
  fac_s = 0;
  fac_n = 5;
}


int fac_return()
{
  int expected_result = 154;
  return fac_s - expected_result;
}


/*
  Arithmetic math functions
*/


int fac_fac ( int n )
{
  if ( n == 0 )
    return 1;
  else
    return ( n * fac_fac ( n - 1 ) );
}


/*
  Main functions
*/


void _Pragma( "entrypoint" ) fac_main ()
{
  int i;

  _Pragma( "loopbound min 6 max 6" )
  for ( i = 0;  i <= fac_n; i++ ) {
    _Pragma( "marker recursivecall" )
    fac_s += fac_fac ( i );
    _Pragma( "flowrestriction 1*fac_fac <= 6*recursivecall" )
  }
}


int task_fac ( void )
{
  fac_init();
  fac_main();

  return ( fac_return() );
}


void thread_entry(int cid, int nc)
{

  asm volatile("fence");
  barrier(nc);
  if (cid == 0) {
    start_counter();
  }
  barrier(nc);
  
  if (cid == 0) {
    task_fac();
  }
  if (cid == 1) {
    task_iir();
  }

  barrier(nc);
  if (cid == 0) {
    stop_counter();
  }
  barrier(nc);

  printf("number of cores: %d \n", nc);
  for(int i = 0; i<5; i++) {
    barrier(nc);
    if (cid == 0) {
      reg_write32(0x2000+0x8,i);  
    }
    barrier(nc);

    printf("(%d) %c: ", cid, i+97);
    for(int j = 0; j<8; j++) {
        barrier(nc);
        if (cid == 0) {
          count0 = reg_read32(0x2000 + 0xc + j * 0x4);
          printf("%u ", count0);
        }
        barrier(nc);
        if (cid == 1) {
          count1 = reg_read32(0x2000 + 0xc + j * 0x4);
          printf("%u ", count1);
        }
        barrier(nc);
    }
    printf("\n");
  }

  barrier(nc);
  exit(0);
}
