
#include "options.h"
#include <stdlib.h>
#include <errno.h>
#include <stdio.h>
#include <stdbool.h>

#include <unistd.h>
#include <string.h>


// Note: strcmp() returns 0 when the two strings are equal :/
// can treat as return true when not equal -> strne()

int strings_eq(char* a, char* b){
  return !strcmp(a, b);
}

long long
parse_args (int argc, char **argv, int *argv_i_index_ptr, unsigned int *num_output_bytes_ptr){
  //  argv_o_index_ptr and num_output_bytes_ptr will not be set
  // if corresponding options not encountered
  // num_output_bytes must be > 0
  // nbytes must be >= 0
  
  int it = 1;
    
  bool prev_i = false;
  bool prev_o = false;
  int i_count = 0;
  int o_count = 0;
  
  long long nbytes;
  bool nbytes_specified = false;

  // report first parsing error encountered and terminate
  while (it < argc)
    {
      if (prev_i || prev_o)
        {
          if (prev_i)
            {
              if (strings_eq (argv[it], "rdrand") || strings_eq (argv[it], "mrand48_r")
                  || argv[it][0] == '/')
                {
                  *argv_i_index_ptr = it;
                }
              else
                {
                  fprintf(stderr, "%s: Invalid argument\n", argv[it]);
                  return -1;
                }
              
              prev_i = false;
            }
          else if (prev_o)
            {
              if (!strings_eq (argv[it], "stdio"))
                {
                  char* endptr;
                  errno = 0;
                  *num_output_bytes_ptr = strtoll (argv[it], &endptr, 10);
                  if (errno)
                    {
                    perror(argv[it]);
                    return -1;
                    }
                  else if (*endptr || *num_output_bytes_ptr < 1)
                    {
                      fprintf(stderr, "%s: Invalid argument\n", argv[it]);
                      return -1;
                    }
                }
              prev_o = false;
            }
        }
      else if (strings_eq (argv[it], "-i"))
        {
          if (++i_count > 1)
            {
              fprintf(stderr, "%s: Duplicate option specifier\n", argv[it]);
              return -1;
            }

          prev_i = true;
        }
      else if (strings_eq (argv[it], "-o"))
        {
          if (++o_count > 1)
            {
              fprintf(stderr, "%s: Duplicate option specifier \n", argv[it]);
              return -1;
            }

          prev_o = true;
        }
      else
        {
          if (nbytes_specified)
            {
              fprintf(stderr, "%s: Invalid argument", argv[it]);
              return -1;
            }
          
          char *endptr;
          errno = 0;
          nbytes = strtoll (argv[it], &endptr, 10);
          if (errno)
            {
              perror (argv[it]);
              return -1;
            }
          else if (*endptr || nbytes < 0)
            {
              fprintf (stderr, "%s: Invalid argument\n", argv[it]);
              return -1;
            }

          nbytes_specified = true;
        }
      
      it++;
    }

  if (!nbytes_specified || prev_i || prev_o)
    {
      if (prev_i)
        fprintf(stderr, "-i: Missing argument\n");
      else if (prev_o)
        fprintf(stderr, "-o: Missing argument\n");
      else if (!nbytes_specified)
        fprintf(stderr,"NBYTES: Missing argument\n");
      


      return -1;
    }

  return nbytes;
}
