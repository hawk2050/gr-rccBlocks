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

import numpy as np
from gnuradio import gr
from instruments import VNX_Lab_Brick

class VNXLabBrick(gr.sync_block):
    """
    docstring for block VNXLabBrick
    """
    def __init__(self, atten):
        gr.sync_block.__init__(self,
            name="VNXLabBrick",
            in_sig=[np.float32],
            out_sig=None)
        self.vnx = VNX_Lab_Brick.VNXDigitalAttenuator()
        self.vnx.connect()
        self.vnx.setAttenuation(atten)
        self.currentAttenSetting = atten


    def work(self, input_items, output_items):
        inum_input_items = len(input_items[0])
        in0 = input_items[0]
        return num_input_items

    def set_atten(self,atten):
        self.vnx.setAttenuation(atten)
        self.currentAttenSetting = atten

