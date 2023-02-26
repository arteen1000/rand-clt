
#ifndef OUTPUT_H
#define OUTPUT_H

#include <stdbool.h>

bool
writebytes (unsigned long long x, int nbytes);

long
unnecessary_call_write_sys(char*, unsigned int);

#endif
