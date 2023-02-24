// software implementation of generator

#include "rand64-sw.h"
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>

/* Software implementation.  */

/* Input stream containing random bytes.  */
static FILE *urandstream;

// filename to use
extern char *filename;

// buffer to use
static struct drand48_data buffer;

void
software_rand64_init (void)
{
  urandstream = fopen (filename, "r");
  if (! urandstream)
    abort ();
}

unsigned long long
software_rand64 (void)
{
  unsigned long long int x;
  if (fread (&x, sizeof x, 1, urandstream) != 1)
    abort ();
  return x;
}

void
software_rand64_fini (void)
{
  fclose (urandstream);
}


// initialize mrand48_r
void
mrand48_init (void)
{
  buffer = seed48_r();
}

// generate values
unsigned long long
mrand48_wrapper (void)
{
  long result;
  errno = 0;
  if (-1 == mrand48_r(&buffer,&result)){
    perror("mrand48_r");
    abort();
  }
  return (unsigned long long) result;
}

void
mrand48_fini (void)
{
}
