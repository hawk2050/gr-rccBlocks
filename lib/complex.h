#ifndef __INCLUDED_COMPLEX_H__
#define __INCLUDED_COMPLEX_H__

#include <gnuradio/gr_complex.h>
/*
Christos Komninakis (author of the fading channel code) had 
created his own complex data type. Fortunately this was 
interchangeable with the gr_complexed type (double precession) 
and in the interest of leaving Christos' code unmodified the 
following typedef is used. 
*/
typedef gr_complex		Complex;


#endif // __INCLUDED_COMPLEX_H__
