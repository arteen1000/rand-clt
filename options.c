
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
parse_args (int argc, char **argv){
  
  int it = 1;
  bool parsing_error = false;
  
  bool prev_i = false;
  bool prev_o = false;
  int i_count = 0;
  int o_count = 0;
  
  long long nbytes;
  bool nbytes_specified = false;
  
  long long num_output_bytes = 1; // default
    
  while (it < argc)
    {
      if (prev_i || prev_o)
        {
          if (prev_i)
            {
              if (strings_eq (argv[it], "rdrand"))
                {
                  // set up call to rdrand
                }
              else if (strings_eq (argv[it], "mrand48_r"))
                {
                  // set up call to mrand48_r
                }
              else if (argv[it][0] == '/')
                {
                  // set up file path option
                }
              else
                {
                  fprintf(stderr, "%s: Invalid argument\n", argv[it]);
                  parsing_error = true;
                }
              
              prev_i = false;
            }
          else if (prev_o)
            {
              if (strings_eq (argv[it], "stdio"))
                {
                  // use stdio output
                }
              else 
                {
                  char* endptr;
                  errno = 0;
                  num_output_bytes = strtoll (argv[it], &endptr, 10);
                  if (errno)
                    {
                    perror(argv[it]);
                    parsing_error = true;
                    }
                  else if (*endptr || num_output_bytes < 1)
                    parsing_error = true;
                }
              prev_o = false;
            }
        }
      else if (strings_eq (argv[it], "-i"))
        {
          i_count++;
          if (prev_i || prev_o)
              parsing_error = true;
          else
            prev_i = true;
        }
      else if (strings_eq (argv[it], "-o"))
        {
          o_count++;
          if (prev_i || prev_o)
              parsing_error = true;
          else
            prev_o = true;
        }
      else
        {
          if (nbytes_specified)
            parsing_error = true;
          else
            {
               char *endptr;
               errno = 0;
               nbytes = strtoll (argv[it], &endptr, 10);
               if (errno)
                 {
                 perror (argv[it]);
                 parsing_error = true;
                 }
               else if (*endptr || nbytes < 0)
                 parsing_error = true;
               
               nbytes_specified = true;
            }
        }
      
      it++;
    }

  if (parsing_error || !nbytes_specified || prev_i || prev_o || o_count > 1 || i_count > 1)
    {
      if (!nbytes_specified)
        fprintf(stderr,"NBYTES must be specified: Missing argument\n");
      if (prev_i || prev_o)
        fprintf(stderr, "Must specify argument for option: Missing argument\n");
      if (o_count > 1 || i_count > 1)
        fprintf(stderr, "Duplicate options specifiers: Ambiguous declaration\n");
        
      return -1;
    } else return nbytes;
}
