
#include "options.h"
#include <stdlib.h>
#include <errno.h>
#include <stdio.h>

#include <unistd.h>

long long
parse_args (bool *valid_ptr, int argc, char **argv){
  long long nbytes;
  if (argc == 2)
    {
      char *endptr;
      errno = 0;
      nbytes = strtoll (argv[1], &endptr, 10);
      if (errno)
        perror (argv[1]);
      else
        *valid_ptr = !*endptr && 0 <= nbytes;
    } else return -1;
  return nbytes;
}
