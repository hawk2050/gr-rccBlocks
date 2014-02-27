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


#ifndef INCLUDED_RCCBLOCKS_RAYLEIGHCHAN_CC_H
#define INCLUDED_RCCBLOCKS_RAYLEIGHCHAN_CC_H

#include <rccBlocks/api.h>
#include <gnuradio/block.h>

namespace gr {
  namespace rccBlocks {

    /*!
     * \brief <+description of block+>
     * \ingroup rccBlocks
     *
     */
    class RCCBLOCKS_API rayleighChan_cc : virtual public gr::sync_block
    {
     public:
      typedef boost::shared_ptr<rayleighChan_cc> sptr;

      /*!
       * \brief Return a shared_ptr to a new instance of rccBlocks::rayleighChan_cc.
       *
       * To avoid accidental use of raw pointers, rccBlocks::rayleighChan_cc's
       * constructor is in a private implementation
       * class. rccBlocks::rayleighChan_cc::make is the public interface for
       * creating new instances.
       */
      static sptr make(int32_t seed, float fD, float pwr, bool flag_indep, bool mode);
      virtual void set_dopplerFreq(float fD) = 0;
      virtual void set_fadeMode(bool mode) = 0;
    };

  } // namespace rccBlocks
} // namespace gr

#endif /* INCLUDED_RCCBLOCKS_RAYLEIGHCHAN_CC_H */

