#include"randlib.h"
#include<immintrin.h>     //ADDED for _rdrand64_step


/* Return a random value, using hardware operations.  */
extern unsigned long long     // CHANGED to match randlib.h
hardware_rand64 (void)
{
  unsigned long long int x;
  while (! _rdrand64_step (&x))
    continue;
  return x;
}
