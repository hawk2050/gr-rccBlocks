#!/usr/bin/env python
# -*- coding: utf-8 -*-
# 
# Copyright 2014 <+YOU OR YOUR COMPANY+>.
# 
# This is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 3, or (at your option)
# any later version.
# 
# This software is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
# 
# You should have received a copy of the GNU General Public License
# along with this software; see the file COPYING.  If not, write to
# the Free Software Foundation, Inc., 51 Franklin Street,
# Boston, MA 02110-1301, USA.
# 

from gnuradio import gr
import rccBlocks
import numpy as np
import sys

class multipath_rayleigh_cc(gr.hier_block2):
    """
    Applies multipath Rayleigh fadding to a complex stream input.

    @param fg:          flowgraph
    @param src:         Block which forms the head of the multipath channel.
                        This is a temporary work around. Ideally this head block would not need
                        to be passed to this class.
    @param vehicle_speed:   This is the speed at which the vehicle is travling in km/h.
    @type vehicle_speed:    float
    @param carrier_freq:    Transmission carrier frequancy in Hz.
    @type carrier_freq:     float
    @param chan_rate:   The sample rate at which the channel is operating (Samples/second).
    @type chan_rate:    float
    @param chan_seed:   Used to seed the channels. Each new channel is seeded with chan_seed++.
    @type chan_seed:    interger
    @param chan_pwrs:   Array containing the square root power of the fading waveform for each path
                        (e.g. pwr = 5 would produce a fading waveform with an 
                        output power of 25).
    @type chan_pwrs:    array of floats
    @param path_delays: Array of path delays in micro-seconds. Note the delay time resolution is
                        dependent on the sample rate of the channel. For example, a channel operating at 
                        256k samples/second, delays will be nearest integer multiples of 0.256.
    @type path_delays:  array of ints
    @param flag_indep:  Determines whether individual blocks processed by the channel should 
                        be treated as independent blocks or as one continuous block.
                        1 if blocks are independent, 0 if continuous across blocks.
                        By default blocks are not treated independently.
    @type flag_indep:   bool
    @param flag_norm:   Determines whether the total output power should be normalized.
                        If this is true then the values of chan_pwrs are relative only.
                        By default the channel is normalized.
    @type flag_norm:    bool
    
    """
    
    def __init__(self, seed,fD,chan_pwrs,path_delays,flag_indep=False,flag_norm=True,flag_fadeEnable=False):
        gr.hier_block2.__init__(self,
            "multipath_rayleigh_cc",
            gr.io_signature(1, 1, gr.sizeof_gr_complex*1),  # Input signature
            gr.io_signature(1, 1, gr.sizeof_gr_complex*1)) # Output signature

        ##################################################
        # Parameters
        ##################################################
        self.fD = fD #Doppler Bandwidth = fd*(1.0/chan_rate)
        self.chan_pwrs = chan_pwrs
        self.path_delays_samples = path_delays
        self.chan_seed = seed
        self.mode = flag_fadeEnable #Enables fading for underlaying single path fading block

        # Checks that there is the same number of delays as there are powers.
        if len(self.chan_pwrs) != len(self.path_delays_us):
            raise ValueError, "The vector length of chan_pwrs does not match the vector length of path_delays."
            # Could this be improved?
            sys.exit(1)

        self.c2f_blks = []      # for list of gr.complex_to_float().
        self.delay_blks = []    # for list of gr.filter_delay_fc ().
        self.chan_blks = []     # for list of tait.flat_rayleigh_channel_cc().

        # Normalizes the channel powers if required
        if flag_norm is True:
            self.chan_pwrs = 1.0*np.array(chan_pwrs)/np.sqrt((chan_pwrs ** 2).sum(-1))

        # Populate the lists above with the correct number of blocks.
        for i in range (len(self.path_delays_samples)):
            print "create delay block %d" %(i)
            
            # Delay block is required.
            self.delay_blks.append(gr.delay(gr.sizeof_gr_complex*1, int(self.path_delays_samples[i])))
                
            self.chan_blks.append(rccBlocks.rayleighChan_cc(chan_seed + i, self.fD, self.chan_pwrs[i], flag_indep,self.mode))

        self.sum = gr.add_vcc(1)  

        # Create multiple instances of the "src -> delay -> channel" connection.
        for i in range (len(self.chan_blks)):
            print i
            self.connect( (self,0), (self.chan_blks[i],0) )
            self.connect( (self.chan_blks[i],0), (self.delay_blks[i],0) )
            self.connect( (self.delay_blks[i],0), (self.sum, i) )
        #self.connect( (self,0), (self.chan_blks[0],0) )
        #self.connect( (self.chan_blks[0],0), (self.delay_blks[0],0) )
        #self.connect( (self.delay_blks[0],0), (self, 0) )  
        self.connect((self.sum, 0), (self,0) )         
