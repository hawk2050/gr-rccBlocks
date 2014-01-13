/*definitions used by the random functions*/
#ifndef RANDOM
#define RANDOM

#include <inttypes.h>
#include <math.h>


#define Ia   (int32_t)16807
#define Im   (int32_t)2147483647
#define Am   (float)(1.0/Im)
#define Iq   (int32_t)127773
#define Ir   (int32_t)2836
#define Ntab (int32_t)32
#define Ndiv (1+(Im-1)/Ntab)
#define Eps  1.2e-7
#define Rnmx (1.0-Eps)
 
// extern float drand48();
 
float Uniform(int32_t *idum);
/*Returns a uniform RV in (0,1) - Any seed<-1 can be used*/
 
float Gaussian(int32_t *idum); 
/*Returns a Gaussian RV ~ N(0,1) - Uses  Uniform from above - 
                                 - Any seed<-1 can be used */

#endif

