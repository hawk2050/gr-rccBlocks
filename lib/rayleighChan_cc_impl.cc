/* -*- c++ -*- */
/* 
 * Copyright 2014 <+YOU OR YOUR COMPANY+>.
 * 
 * This is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3, or (at your option)
 * any later version.
 * 
 * This software is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this software; see the file COPYING.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street,
 * Boston, MA 02110-1301, USA.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <inttypes.h>
#include <gnuradio/io_signature.h>
#include "rayleighChan_cc_impl.h"

namespace gr {
  namespace rccBlocks {

    rayleighChan_cc::sptr
    rayleighChan_cc::make(int32_t seed, float fd, float pwr, bool flag_indep)
    {
      return gnuradio::get_initial_sptr (new rayleighChan_cc_impl(seed, fd, pwr, flag_indep));
    }

    static const int32_t MIN_IN = 1;  // mininum number of input streams
    static const int32_t MAX_IN = 1;  // maximum number of input streams
    static const int32_t MIN_OUT = 1; // minimum number of output streams
    static const int32_t MAX_OUT = 1; // maximum number of output streams

    /*
     * The private constructor
     */
    rayleighChan_cc_impl::rayleighChan_cc_impl(int32_t seed, float fd, float pwr, bool flag_indep)
      : gr::block("rayleighChan_cc",
              gr::io_signature::make(MIN_IN, MAX_IN, sizeof (gr_complex)),
              gr::io_signature::make(MIN_OUT, MAX_OUT, sizeof (gr_complex)))
    {
      mychan = new flat_rayleigh(seed, fd, pwr, flag_indep);
      set_dopplerFreq(fd);
    }

    /*
     * Our virtual destructor.
     */
    rayleighChan_cc_impl::~rayleighChan_cc_impl()
    {
      // frees the allocated memory.
      delete mychan;
    }

    void rayleighChan_cc_impl::set_dopplerFreq(float fD)
    {
       mychan->set_dopplerFreq(fD);
    }

    void
    rayleighChan_cc_impl::forecast (int noutput_items, gr_vector_int &ninput_items_required)
    {
        /* <+forecast+> e.g. ninput_items_required[0] = noutput_items */
      ninput_items_required[0] = noutput_items;
    }

    int32_t
    rayleighChan_cc_impl::general_work (int noutput_items,
                       gr_vector_int &ninput_items,
                       gr_vector_const_void_star &input_items,
                       gr_vector_void_star &output_items)
    {
        const gr_complex *in = (const gr_complex *) input_items[0];
        gr_complex *out = (gr_complex *) output_items[0];

        // Do <+signal processing+>
        // Performs the channel fading.
        mychan->pass_through(noutput_items, in, out);
        // Tell runtime system how many input items we consumed on
        // each input stream.
        consume_each (noutput_items);

        // Tell runtime system how many output items we produced.
        return noutput_items;
    }

  } /* namespace rccBlocks */
} /* namespace gr */

