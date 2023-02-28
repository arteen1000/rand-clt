
#ifndef OUTPUT_H
#define OUTPUT_H

#include <stdbool.h>
#include <unistd.h>

bool
writebytes (unsigned long long x, int nbytes);

inline long
unnecessary_call_write_sys(char* buf, unsigned int num_output_bytes)
{
  return write(STDOUT_FILENO, buf, num_output_bytes);
}



#endif
