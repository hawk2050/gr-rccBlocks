#!/usr/bin/env python
##################################################
# Gnuradio Python Flow Graph
# Title: Static RF or Single Path Rayleigh Faded RF
# Author: Richard Clarke
# Description: Provides either static RF or single path Rayleigh faded RF at specified doppler spread
# Generated: Tue May  6 20:16:35 2014
##################################################

from gnuradio import analog
from gnuradio import blocks
from gnuradio import eng_notation
from gnuradio import gr
from gnuradio import uhd
from gnuradio.eng_option import eng_option
from gnuradio.filter import firdes
from optparse import OptionParser
from random import randint
import SimpleXMLRPCServer
import rccBlocks
import threading
import time

class pc_fading_vaunix_xmlrpc(gr.top_block):

    def __init__(self):
        gr.top_block.__init__(self, "Static RF or Single Path Rayleigh Faded RF")

        ##################################################
        # Variables
        ##################################################
        self.usrpRate = usrpRate = 250e3
        self.fd = fd = 20
        self.fdTs = fdTs = fd*(1.0/usrpRate)
        self.fadeMode = fadeMode = 0
        self.centreFreq = centreFreq = 1e6
        self.baseband_multiplier = baseband_multiplier = 0.25
        self.atten = atten = 0

        ##################################################
        # Blocks
        ##################################################
        self.xmlrpc_server_0 = SimpleXMLRPCServer.SimpleXMLRPCServer(("0.0.0.0", 1234), allow_none=True)
        self.xmlrpc_server_0.register_instance(self)
        threading.Thread(target=self.xmlrpc_server_0.serve_forever).start()
        self.uhd_usrp_sink_0_0_0 = uhd.usrp_sink(
        	device_addr="",
        	stream_args=uhd.stream_args(
        		cpu_format="fc32",
        		channels=range(1),
        	),
        )
        self.uhd_usrp_sink_0_0_0.set_subdev_spec("B:AB", 0)
        self.uhd_usrp_sink_0_0_0.set_samp_rate(usrpRate)
        self.uhd_usrp_sink_0_0_0.set_center_freq(centreFreq, 0)
        self.uhd_usrp_sink_0_0_0.set_gain(0, 0)
        self.rccBlocks_rayleighChan_cc_0 = rccBlocks.rayleighChan_cc(randint(-10e3, 0), fdTs, 1.0, False, bool(fadeMode))
        self.rccBlocks_VNXLabBrick_0 = rccBlocks.VNXLabBrick(atten)
        self.blocks_throttle_0 = blocks.throttle(gr.sizeof_float*1, usrpRate,True)
        self.blocks_multiply_const_vxx_1 = blocks.multiply_const_vcc((baseband_multiplier, ))
        self.analog_const_source_x_1 = analog.sig_source_f(0, analog.GR_CONST_WAVE, 0, 0, 1)
        self.analog_const_source_x_0 = analog.sig_source_c(0, analog.GR_CONST_WAVE, 0, 0, 1+1j)

        ##################################################
        # Connections
        ##################################################
        self.connect((self.blocks_multiply_const_vxx_1, 0), (self.uhd_usrp_sink_0_0_0, 0))
        self.connect((self.blocks_throttle_0, 0), (self.rccBlocks_VNXLabBrick_0, 0))
        self.connect((self.analog_const_source_x_1, 0), (self.blocks_throttle_0, 0))
        self.connect((self.rccBlocks_rayleighChan_cc_0, 0), (self.blocks_multiply_const_vxx_1, 0))
        self.connect((self.analog_const_source_x_0, 0), (self.rccBlocks_rayleighChan_cc_0, 0))


# QT sink close method reimplementation

    def get_usrpRate(self):
        return self.usrpRate

    def set_usrpRate(self, usrpRate):
        self.usrpRate = usrpRate
        self.set_fdTs(self.fd*(1.0/self.usrpRate))
        self.uhd_usrp_sink_0_0_0.set_samp_rate(self.usrpRate)
        self.blocks_throttle_0.set_sample_rate(self.usrpRate)

    def get_fd(self):
        return self.fd

    def set_fd(self, fd):
        self.fd = fd
        self.set_fdTs(self.fd*(1.0/self.usrpRate))

    def get_fdTs(self):
        return self.fdTs

    def set_fdTs(self, fdTs):
        self.fdTs = fdTs
        self.rccBlocks_rayleighChan_cc_0.set_dopplerFreq(self.fdTs)

    def get_fadeMode(self):
        return self.fadeMode

    def set_fadeMode(self, fadeMode):
        self.fadeMode = fadeMode
        self.rccBlocks_rayleighChan_cc_0.set_fadeMode(bool(self.fadeMode))

    def get_centreFreq(self):
        return self.centreFreq

    def set_centreFreq(self, centreFreq):
        self.centreFreq = centreFreq
        self.uhd_usrp_sink_0_0_0.set_center_freq(self.centreFreq, 0)

    def get_baseband_multiplier(self):
        return self.baseband_multiplier

    def set_baseband_multiplier(self, baseband_multiplier):
        self.baseband_multiplier = baseband_multiplier
        self.blocks_multiply_const_vxx_1.set_k((self.baseband_multiplier, ))

    def get_atten(self):
        return self.atten

    def set_atten(self, atten):
        self.atten = atten
        self.rccBlocks_VNXLabBrick_0.set_atten(self.atten)

if __name__ == '__main__':
    parser = OptionParser(option_class=eng_option, usage="%prog: [options]")
    (options, args) = parser.parse_args()
    tb = pc_fading_vaunix_xmlrpc()
    tb.start()
    raw_input('Press Enter to quit: ')
    tb.stop()
    tb.wait()

