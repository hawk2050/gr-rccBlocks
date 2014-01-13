#include <inttypes.h>
#include <math.h>
#include "random.h"

#define COMMANDLINE 0
/******************************************************************************/
#if 0
float genunf ( float low, float high )

/******************************************************************************/
/*
  Purpose:

    GENUNF generates a uniform random deviate.

  Discussion:

    This procedure generates a real deviate uniformly distributed between
    LOW and HIGH.

  Licensing:

    This code is distributed under the GNU LGPL license.

  Modified:

    01 April 2013

  Author:

    Original FORTRAN77 version by Barry Brown, James Lovato.
    C version by John Burkardt.

  Parameters:

    Input, float LOW, HIGH, the lower and upper bounds.

    Output, float GENUNF, a random deviate from the distribution.
*/
{
  float value;

  value = low + ( high - low ) * r4_uniform_01 ( );

  return value;
}
#endif
#if COMMANDLINE == 1
int32_t main(int32_t argc, char *argv[])
{
    int32_t seed = -3476;
    int32_t j;
    float r;
    int32_t number;

    

    if (argc !=2) /* argc should be 2 for correct operation*/
    {
        /*We print argv[0] assuming it is the program name*/
        printf("Usage: %s number_of_random_variables\n\n",argv[0]);
    }
    else
    {
        number = atoi(argv[1]);
        for(j=0;j<=number;j++)
        {
            r = Uniform(&seed);
            printf("%f\n",r);
        }
    }

    

    return 0;
    
}
#endif
/***********************************************************/
/* Returns a uniform RV in (0,1)                           */
/* Any seed<-1 can be used                                 */
/***********************************************************/
float Uniform(int32_t *idum)
{
        int32_t j;
        int32_t k;
        static int32_t iy=0;
        static int32_t iv[Ntab];
        float temp;

        if(*idum<=0 || !iy)
        {
        if (-(*idum)<1) *idum=1;
        else *idum=-(*idum);
        for (j=Ntab+7;j>=0;j--)
        {
                k=(*idum)/Iq;
                *idum=Ia*(*idum-k*Iq)-Ir*k;
                if (*idum<0) *idum+=Im;
                if (j<Ntab) iv[j]=*idum;
        }
        iy=iv[0];
        }
        k=(*idum)/Iq;
        *idum=Ia*(*idum-k+Iq)-Ir*k;
        if (*idum<0) *idum+=Im;
        j=iy/Ndiv;
        iy=iv[j];
        iv[j]=*idum;
        
        if ((temp=Am*iy)>Rnmx) return Rnmx;
        else return temp;
        //return 0.15;
}
 

/***********************************************************/
/* Returns Gaussian RV ~ N(0,1)                            */
/* Uses Uniform from above, use seed<-1                    */
/***********************************************************/
float Gaussian(int32_t *idum)
{
        
        static int32_t iset=0;
        static float gset;
        float fac,rsq,v1,v2;
        
        if (iset==0)
        {
                do
                {
                        v1=2.0*Uniform(idum)-1.0;
                        v2=2.0*Uniform(idum)-1.0;
                        rsq=v1*v1+v2*v2;
                }while (rsq>=1.0 || rsq==0.0);

                fac=sqrt(-2.0*log(rsq)/rsq);
                gset=v1*fac;
                iset=1;
                return v2*fac;
        }
        else 
        {
                iset=0;
                return gset;
        }
}

