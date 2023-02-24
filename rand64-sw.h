
#ifndef RAND64_SW_H
#define RAND64_SW_H

/* Initialize the software rand64 implementation.  */
void
software_rand64_init (void);

/* Return a random value, using software operations.  */
unsigned long long
software_rand64 (void);

/* Finalize the software rand64 implementation.  */
void
software_rand64_fini (void);

void
mrand48_init (void);

unsigned long long
mrand48_wrapper (void);

void
mrand48_fini (void);
#endif
