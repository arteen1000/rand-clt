
// send bytes to output

#include "output.h"
#include <stdio.h>
#include <limits.h>
#include <unistd.h>

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

long
unnecessary_call_write_sys(char* buf, unsigned int num_output_bytes)
{
  return write(STDOUT_FILENO, buf, num_output_bytes);
}
