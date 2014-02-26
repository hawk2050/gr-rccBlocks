#ifndef FLAT_RAYLEIGH
#define FLAT_RAYLEIGH

#include "random.h"
/*****************************************************************************
 * Author: Christos Komninakis                                               *
 * Date: January 4, 2000.                                                    *
 * Content: header file for the CLASS "flat_rayleigh".                       *
 *****************************************************************************/
#include <iostream>
#include <fstream>
#include <inttypes.h>
#include "complex.h"

using namespace std;

class flat_rayleigh
{
  protected:
  int32_t chan_seed;	/* the random channel seed */
  bool IndepFlag;	/* 1 if i.i.d. blocks, 0 if continuous across blocks */
  float PWR;		/* the power of the fading waveform */
  bool fadeMode; /* Fading is either disabled (0) or enabled (1). If disabled then the input to the block is passed straight through.*/
  Complex chan_val;	/* the flat fading complex coefficient of the channel */
  int32_t K;        /* the number of biquads for my ellipsoid-algorithm design */
  int32_t H, H2;    /* number of interpolating coefs (one sided). H2=2*H */
  int32_t I;        /* the interpolation factor, given by 0.2/fD */
  int32_t last_i, IP;   /* helpful indices: IP=Insertion Point into buff_f */
  float *a, *b, *c, *d, Ao;  /* biquad coefs, and gain */
  float **sinc_matrix;
  Complex **st;    /* state of the K biquads */
  Complex *buff_f; /* the rarely sampled output of the 0.2-Doppler prototype */
  float *buff_sinc; /* pointer to the pertinent row of the interp. matrix */
  
  public:
  flat_rayleigh(int32_t seed, float fD, float pwr, bool flag_indep, bool mode);
  ~flat_rayleigh()
    {
      delete [] a;
      delete [] b;
      delete [] c;
      delete [] d;
      delete [] st;
      delete [] buff_f;
      delete [] sinc_matrix;
    }

  void set_dopplerFreq(float fD);
  bool get_fadeMode(void);
  void set_fadeMode(bool mode);
  void no_fading(int32_t length, Complex *inp, Complex *outp);
  void pass_through(int32_t length, Complex *inp, Complex *outp);
  void pass_through(int32_t length, Complex *inp, Complex *outp, Complex *csi);
  void pass_through(int32_t length, Complex *inp, Complex *outp, float *amp_csi);
};

#endif
