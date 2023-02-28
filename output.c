
// send bytes to output

#include "output.h"
#include <stdio.h>
#include <limits.h>

bool
writebytes (unsigned long long x, int nbytes)
{
  do
    {
      if (putchar ( (unsigned char) x ) < 0)
	return false;
      x >>= CHAR_BIT;
      nbytes--;
    }
  while (0 < nbytes);

  return true;
}

