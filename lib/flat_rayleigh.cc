/*****************************************************************************
 * Author: Christos Komninakis                                               *
 * Date: January 4, 2000.                                                    *
 * Content: C++ code file for the CLASS "flat_rayleigh".                     *
 *****************************************************************************/
#include <string>
#include "flat_rayleigh.h"
 using namespace std;

#define PI 3.14159265358979

/**************** Constructor ***************************************/
flat_rayleigh::flat_rayleigh(int32_t seed, float fD, float pwr, bool flag_indep)
{
  chan_seed = seed; //-23;
  PWR = pwr;
  IndepFlag = flag_indep;
  chan_val = Complex(0.0, 0.0);
  K = 7; H = 7; H2 = 2*H;
  if (fD > 0.2) {
	  cout << "Warning: Discrete Doppler fDT > 0.2, handled as fDT=0.2 exactly" << endl;
	  I = 1;
  } else
	
  I = (int32_t) (0.2 / fD);
  last_i = 0; IP = 0;
  int32_t i, t, j, k;
  
  a = new float [K];
  b = new float [K];
  c = new float [K];
  d = new float [K];
  float A_o;
  st = new Complex* [3];
  for (i=0; i<3; i++)
    st[i] = new Complex [K];
  sinc_matrix = new float* [I];
  for (i=0; i<I; i++)
    sinc_matrix[i] = new float [H2];
  buff_f = new Complex [H2];

  /* Following are the values of a designed ROOT DOPPLER filter prototype,
     consisting of K=7 biquads, with fD = 0.2 */

  a[0] = 0.98323218491754;    b[0] = 0.13953807910136;
  c[0] = -0.60134822489663;   d[0] = 0.48938674026073;
  a[1] = -0.58056307877395;   b[1] = 0.99977164701498;
  c[1] = -0.42846077703117;   d[1] = 0.24612477470991;
  a[2] = -0.50498097209321;   b[2] = 0.99986531836969;
  c[2] = -0.63050157648166;   d[2] = 0.9350218659307;
  a[3] = 0.20534422769486;    b[3] = 0.98691526068222;
  c[3] = -0.096351029308223;  d[3] = -0.060828833103004;
  a[4] = 1.2667005604552;     b[4] = 0.93699896920708;
  c[4] = -0.64949215100022;   d[4] = 0.80427856161307;
  a[5] = -0.60545728027338;   b[5] = 0.99998818741754;
  c[5] = -0.61939958040555;   d[5] = 0.99895172368533;
  a[6] = -0.29077222501287;   b[6] = 0.99832970528904;
  c[6] = -0.62124137243814;   d[6] = 0.98111826315356;
  A_o = 0.028241759549181;

  /* now really start */
  Ao = PWR * A_o / sqrt(2.0);
  /* produce the table storing all the values of the interpolating function,
	 windowed and in reversed order, so that filtering is straightforward */
  float scale = 0.0;
  for (i=0; i<I; i++) {
	  for (j=0; j<H2; j++) {
		  if ((i == 0) & (j == H))
			  sinc_matrix[i][j] = 1.0;
		  else {
			  sinc_matrix[i][j] =
				   sin(PI*(j-H)+(PI/I)*i) / (PI*(j-H)+(PI/I)*i) * //;
                   (0.5 - 0.5*cos(2.0*PI*(i+I*j)/(H2*I))); // windowed, Hanning
                   //(0.54 - 0.46*cos(2.0*PI*(i+I*j)/(H2*I))); // windowed, Hamming
                   //(0.42 - 0.5*cos(2.0*PI*(i+I*j)/(H2*I)) + 0.08*cos(4.0*PI*(i+I*j)/(H2*I))); // windowed, Blackman
		  }
		  scale += (sinc_matrix[i][j])*(sinc_matrix[i][j]);
	  }
  }
  scale = sqrt(1.0/scale*I);
  // scale and swap
  float temp;
  for (i=0; i<I; i++) {
	  for (j=0; j<H; j++) {
		  temp = sinc_matrix[i][j];
		  sinc_matrix[i][j] = scale * sinc_matrix[i][H2-1-j];
		  sinc_matrix[i][H2-1-j] = scale * temp;
	  }
  }
  sinc_matrix[0][H] *= scale;

  /* work the channel for some time, to overcome transients */
  for (t=0; t<I*4*(4*K+H2); t++) {
    if (last_i == I-1) {
      last_i = 0;

      /* now produce new element, and insert it into the buffer: buff_f[IP] */
      st[0][0] = Complex(Gaussian(&chan_seed), Gaussian(&chan_seed))*Ao -
                 st[1][0]*c[0] - st[2][0]*d[0];
      for (k=1; k<K; k++)
        st[0][k] = st[0][k-1] + st[1][k-1]*a[k-1] + st[2][k-1]*b[k-1] -
                                st[1][k]*c[k] - st[2][k]*d[k];
      buff_f[IP] = st[0][K-1] + st[1][K-1]*a[K-1] + st[2][K-1]*b[K-1];
      for (k=0; k<K; k++) {
        st[2][k] = st[1][k];
        st[1][k] = st[0][k];
      }
      /* done, produced buff_f[IP], now move IP to next position */
      IP = (IP + 1) % H2;
    } else
      last_i++;
	// stop here, no need to interpolate, since idle run
  }
}



/******* Function pass_through, without providing any CSI ************/
void flat_rayleigh::pass_through(int32_t length, Complex *inp, Complex *outp)
{
  register int32_t k, j, t;

  if (IndepFlag) {
	  chan_seed += 1;	// reset the seed
	  // clean-up and then run idle, to ensure transients are dead
	  for (k=0; k<K; k++) {
		  st[0][k] = Complex(0.0, 0.0);
		  st[1][k] = Complex(0.0, 0.0);
		  st[2][k] = Complex(0.0, 0.0);
	  }
	  for (j=0; j<H2; j++)
		  buff_f[j] = Complex(0.0, 0.0);
	  // and now run idle for a little...
	  for (t=0; t<I*4*(4*K+H2); t++) {
		  if (last_i == I-1) {
			  last_i = 0;
			  /* produce new element, and insert it into buff_f[IP] */
			  st[0][0] = Complex( Gaussian(&chan_seed), Gaussian(&chan_seed) );
			  st[0][0] = st[0][0]*Ao - st[1][0]*c[0] - st[2][0]*d[0];
			  for (k=1; k<K; k++)
				  st[0][k] = st[0][k-1] + st[1][k-1]*a[k-1] + st[2][k-1]*b[k-1] -
						     st[1][k]*c[k] - st[2][k]*d[k];
			  buff_f[IP] = st[0][K-1] + st[1][K-1]*a[K-1] + st[2][K-1]*b[K-1];
			  for (k=0; k<K; k++) {
				  st[2][k] = st[1][k];
				  st[1][k] = st[0][k];
			  }
			  /* done, produced buff_f[IP], now move IP to next position */
			  IP = (IP + 1) % H2;
		  } else
			  last_i++;
		  // end here, don't produce chan_val, since idle run
	  }
  } /* end of dry run for independence */

  /* run for real now */
  for (t=0; t<length; t++) {
    if (last_i == I-1) {
      last_i = 0;

      /* now produce new element, and insert it into the buffer: buff_f[IP] */
      st[0][0] = st[0][0] = Complex( Gaussian(&chan_seed), Gaussian(&chan_seed) );
	  st[0][0] = st[0][0]*Ao - st[1][0]*c[0] - st[2][0]*d[0];
	  for (k=1; k<K; k++)
        st[0][k] = st[0][k-1] + st[1][k-1]*a[k-1] + st[2][k-1]*b[k-1] -
                                st[1][k]*c[k] - st[2][k]*d[k];
      buff_f[IP] = st[0][K-1] + st[1][K-1]*a[K-1] + st[2][K-1]*b[K-1];
      for (k=0; k<K; k++) {
        st[2][k] = st[1][k];
        st[1][k] = st[0][k];
      }
      /* done, produced buff_f[IP], now move IP to next position */
      IP = (IP + 1) % H2;
    } else
      last_i++;

    buff_sinc = sinc_matrix[last_i];
    chan_val = Complex(0.0, 0.0);
    for (j=0; j<H2; j++)
      chan_val += buff_f[(IP + j) % H2] * buff_sinc[j];
    outp[t] = inp[t] * chan_val;
  }
}



/******* Overloaded Function pass_through, providing CSI also ************/
void flat_rayleigh::pass_through(int32_t length, Complex *inp,
                                    Complex *outp, Complex *csi)
{
  register int32_t k, j, t;

  if (IndepFlag) {
	  chan_seed += 1;	// reset the seed
	  // clean-up and then run idle, to ensure transients are dead
	  for (k=0; k<K; k++) {
		  st[0][k] = Complex(0.0, 0.0);
		  st[1][k] = Complex(0.0, 0.0);
		  st[2][k] = Complex(0.0, 0.0);
	  }
	  for (j=0; j<H2; j++)
		  buff_f[j] = Complex(0.0, 0.0);
	  // and now run idle for a little...
	  for (t=0; t<I*4*(4*K+H2); t++) {
		  if (last_i == I-1) {
			  last_i = 0;
			  /* produce new element, and insert it into buff_f[IP] */
			  st[0][0] = Complex( Gaussian(&chan_seed), Gaussian(&chan_seed) );
			  st[0][0] = st[0][0]*Ao - st[1][0]*c[0] - st[2][0]*d[0];
			  for (k=1; k<K; k++)
				  st[0][k] = st[0][k-1] + st[1][k-1]*a[k-1] + st[2][k-1]*b[k-1] -
						     st[1][k]*c[k] - st[2][k]*d[k];
			  buff_f[IP] = st[0][K-1] + st[1][K-1]*a[K-1] + st[2][K-1]*b[K-1];
			  for (k=0; k<K; k++) {
				  st[2][k] = st[1][k];
				  st[1][k] = st[0][k];
			  }
			  /* done, produced buff_f[IP], now move IP to next position */
			  IP = (IP + 1) % H2;
		  } else
			  last_i++;
		  // end here, don't produce chan_val, since idle run
	  }
  } /* end of dry run for independence */
  
  for (t=0; t<length; t++) {
    if (last_i == I-1) {
      last_i = 0;

      /* now produce new element, and insert it into the buffer: buff_f[IP] */
      st[0][0] = Complex( Gaussian(&chan_seed), Gaussian(&chan_seed) )*Ao -
                 st[1][0]*c[0] - st[2][0]*d[0];
      for (k=1; k<K; k++)
        st[0][k] = st[0][k-1] + st[1][k-1]*a[k-1] + st[2][k-1]*b[k-1] -
                                st[1][k]*c[k] - st[2][k]*d[k];
      buff_f[IP] = st[0][K-1] + st[1][K-1]*a[K-1] + st[2][K-1]*b[K-1];
      for (k=0; k<K; k++) {
        st[2][k] = st[1][k];
        st[1][k] = st[0][k];
      }
      /* done, produced buff_f[IP], now move IP to next position */
      IP = (IP + 1) % H2;
    } else
      last_i++;

    buff_sinc = sinc_matrix[last_i];
    chan_val = Complex(0.0, 0.0);
    for (j=0; j<H2; j++)
      chan_val += buff_f[(IP + j) % H2] * buff_sinc[j];
    csi[t] = chan_val;
    outp[t] = inp[t] * chan_val;
  }
}



/***** Overloaded Function pass_through, providing AMPLITUDE CSI only *****/
void flat_rayleigh::pass_through(int32_t length, Complex *inp,
                                 Complex *outp, float *amp_csi)
{
  register int32_t k, j, t;

  if (IndepFlag) {
	  chan_seed += 1;	// reset the seed
	  // clean-up and then run idle, to ensure transients are dead
	  for (k=0; k<K; k++) {
		  st[0][k] = Complex(0.0, 0.0);
		  st[1][k] = Complex(0.0, 0.0);
		  st[2][k] = Complex(0.0, 0.0);
	  }
	  for (j=0; j<H2; j++)
		  buff_f[j] = Complex(0.0, 0.0);
	  // and now run idle for a little...
	  for (t=0; t<I*4*(4*K+H2); t++) {
		  if (last_i == I-1) {
			  last_i = 0;
			  /* produce new element, and insert it into buff_f[IP] */
			  st[0][0] = Complex( Gaussian(&chan_seed), Gaussian(&chan_seed) );
			  st[0][0] = st[0][0]*Ao - st[1][0]*c[0] - st[2][0]*d[0];
			  for (k=1; k<K; k++)
				  st[0][k] = st[0][k-1] + st[1][k-1]*a[k-1] + st[2][k-1]*b[k-1] -
						     st[1][k]*c[k] - st[2][k]*d[k];
			  buff_f[IP] = st[0][K-1] + st[1][K-1]*a[K-1] + st[2][K-1]*b[K-1];
			  for (k=0; k<K; k++) {
				  st[2][k] = st[1][k];
				  st[1][k] = st[0][k];
			  }
			  /* done, produced buff_f[IP], now move IP to next position */
			  IP = (IP + 1) % H2;
		  } else
			  last_i++;
		  // end here, don't produce chan_val, since idle run
	  }
  } /* end of dry run for independence */

  for (t=0; t<length; t++) {
    if (last_i == I-1) {
      last_i = 0;

      /* now produce new element, and insert it into the buffer: buff_f[IP] */
      st[0][0] = Complex( Gaussian(&chan_seed), Gaussian(&chan_seed) )*Ao -
                 st[1][0]*c[0] - st[2][0]*d[0];
      for (k=1; k<K; k++)
        st[0][k] = st[0][k-1] + st[1][k-1]*a[k-1] + st[2][k-1]*b[k-1] -
                                st[1][k]*c[k] - st[2][k]*d[k];
      buff_f[IP] = st[0][K-1] + st[1][K-1]*a[K-1] + st[2][K-1]*b[K-1];
      for (k=0; k<K; k++) {
        st[2][k] = st[1][k];
        st[1][k] = st[0][k];
      }
      /* done, produced buff_f[IP], now move IP to next position */
      IP = (IP + 1) % H2;
    } else
      last_i++;

    buff_sinc = sinc_matrix[last_i];
    chan_val = Complex(0.0, 0.0);
    for (j=0; j<H2; j++)
      chan_val += buff_f[(IP + j) % H2] * buff_sinc[j];
    amp_csi[t] = sqrt(abs(chan_val));
    outp[t] = inp[t] * chan_val;
  }
}
    
    
