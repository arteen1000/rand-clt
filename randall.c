/* Generate N bytes of random output.  */

/* When generating output this program uses the x86-64 RDRAND
   instruction if available to generate random numbers, falling back
   on /dev/random and stdio otherwise.

   This program is not portable.  Compile it with gcc -mrdrnd for a
   x86-64 machine.

   Copyright 2015, 2017, 2020 Paul Eggert

   This program is free software: you can redistribute it and/or
   modify it under the terms of the GNU General Public License as
   published by the Free Software Foundation, either version 3 of the
   License, or (at your option) any later version.

   This program is distributed in the hope that it will be useful, but
   WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <assert.h>

#include "options.h"
#include "output.h"
#include "rand64-hw.h"
#include "rand64-sw.h"

char *filename = 0;

static_assert(sizeof(char) == 1);

/* Main program, which outputs N bytes of random data.  */

int
main (int argc, char **argv)
{
  unsigned int num_output_bytes = 0; // if set, will be positive integer
  int argv_i_index = -1;
  
  long long nbytes = parse_args(argc, argv, // natural number
                                &argv_i_index,
                                &num_output_bytes);
  
  if (nbytes == -1)
    {
      fprintf (stderr, "%s: usage: %s NBYTES "
               "[ -i [ rdrand | mrand48_r | /absolute/file/path ] ]"
               " [ -o [ stdio | N ] ]\n", argv[0], argv[0]);
      return 1;
    }

  if (nbytes == 0)
    return 0;

  void (*initialize) (void);
  unsigned long long (*rand64) (void);
  void (*finalize) (void);

  if (argv_i_index == -1 || strings_eq (argv[argv_i_index], "rdrand"))
    {
      if (! rdrand_supported ())
        return 1;
      initialize = hardware_rand64_init;
      rand64 = hardware_rand64;
      finalize = hardware_rand64_fini;
    }
  else if (argv[argv_i_index][0] == '/')
    {
      filename = argv[argv_i_index];
      initialize = software_rand64_init;
      rand64 = software_rand64;
      finalize = software_rand64_fini;
    }
  else if (strings_eq (argv[argv_i_index], "mrand48_r"))
    {
      initialize = mrand48_init;
      rand64 = mrand48_wrapper;
      finalize = mrand48_fini;
    }
  else return 1;
  
  initialize ();
  
  int wordsize = sizeof rand64 ();
  int output_errno = 0;

  if (num_output_bytes == 0) // stdio
    {
      
      do
    {
      unsigned long long x = rand64 ();
      int outbytes = nbytes < wordsize ? nbytes : wordsize;
      if (!writebytes (x, outbytes))
	{
	  output_errno = errno;
	  break;
	}
      nbytes -= outbytes;
    }
      while (0 < nbytes);
      
    }
  else { // guaranteed to be > 0 integer
    char *buf = (char*) malloc(num_output_bytes);
    if (!buf){
      fprintf(stderr, "Allocation failure");
      return 1;
    }
    int size_of_buf = num_output_bytes;
    memset(buf, 0, num_output_bytes);
    do {
      
      if (nbytes < num_output_bytes)
	num_output_bytes = nbytes;

      unsigned int bytes_written_buffer = 0;
      char *traverser = buf;
      while (bytes_written_buffer < num_output_bytes && traverser != buf + size_of_buf)
	{
	  unsigned long long x = rand64 ();
	  int bytes_to_write =
	    sizeof(unsigned long long) > num_output_bytes ?
	    num_output_bytes : sizeof(unsigned long long);
	  
	  memcpy(traverser, &x, bytes_to_write);
	  traverser += bytes_to_write;
	  bytes_written_buffer += num_output_bytes;
	}
	
      long bytes_written_stdout = 0;
      
      if (-1 == (bytes_written_stdout =
		 write(STDOUT_FILENO, buf, num_output_bytes))){
	output_errno = errno;
	break;
      }
      
      nbytes -= bytes_written_stdout;
    }
    while (0 < nbytes);

    free(buf);
  }
  
  if (fclose (stdout) != 0)
    output_errno = errno;

  if (output_errno)
    {
      errno = output_errno;
      perror ("output");
    }

  finalize ();
  return !!output_errno;
}
