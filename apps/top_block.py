#!/usr/bin/env python
##################################################
# Gnuradio Python Flow Graph
# Title: Top Block
# Generated: Tue May  6 19:58:52 2014
##################################################

from PyQt4 import Qt
from gnuradio import analog
from gnuradio import blocks
from gnuradio import eng_notation
from gnuradio import gr
from gnuradio import qtgui
from gnuradio.eng_option import eng_option
from gnuradio.filter import firdes
from optparse import OptionParser
from random import randint
import SimpleXMLRPCServer
import rccBlocks
import sip
import sys
import threading

class top_block(gr.top_block, Qt.QWidget):

    def __init__(self):
        gr.top_block.__init__(self, "Top Block")
        Qt.QWidget.__init__(self)
        self.setWindowTitle("Top Block")
        try:
             self.setWindowIcon(Qt.QIcon.fromTheme('gnuradio-grc'))
        except:
             pass
        self.top_scroll_layout = Qt.QVBoxLayout()
        self.setLayout(self.top_scroll_layout)
        self.top_scroll = Qt.QScrollArea()
        self.top_scroll.setFrameStyle(Qt.QFrame.NoFrame)
        self.top_scroll_layout.addWidget(self.top_scroll)
        self.top_scroll.setWidgetResizable(True)
        self.top_widget = Qt.QWidget()
        self.top_scroll.setWidget(self.top_widget)
        self.top_layout = Qt.QVBoxLayout(self.top_widget)
        self.top_grid_layout = Qt.QGridLayout()
        self.top_layout.addLayout(self.top_grid_layout)

        self.settings = Qt.QSettings("GNU Radio", "top_block")
        self.restoreGeometry(self.settings.value("geometry").toByteArray())


        ##################################################
        # Variables
        ##################################################
        self.fd = fd = 50
        self.chan_rate = chan_rate = 250000
        self.fdTs = fdTs = fd*(1.0/chan_rate)
        self.fadeMode = fadeMode = False

        ##################################################
        # Blocks
        ##################################################
        _fadeMode_check_box = Qt.QCheckBox("Fading Enabled")
        self._fadeMode_choices = {True: True, False: False}
        self._fadeMode_choices_inv = dict((v,k) for k,v in self._fadeMode_choices.iteritems())
        self._fadeMode_callback = lambda i: Qt.QMetaObject.invokeMethod(_fadeMode_check_box, "setChecked", Qt.Q_ARG("bool", self._fadeMode_choices_inv[i]))
        self._fadeMode_callback(self.fadeMode)
        _fadeMode_check_box.stateChanged.connect(lambda i: self.set_fadeMode(self._fadeMode_choices[bool(i)]))
        self.top_layout.addWidget(_fadeMode_check_box)
        self.xmlrpc_server_0 = SimpleXMLRPCServer.SimpleXMLRPCServer(("0.0.0.0", 1234), allow_none=True)
        self.xmlrpc_server_0.register_instance(self)
        threading.Thread(target=self.xmlrpc_server_0.serve_forever).start()
        self.rccBlocks_rayleighChan_cc_0 = rccBlocks.rayleighChan_cc(randint(-10e3, 0), fdTs, 1.0, False, fadeMode)
        self.qtgui_time_sink_x_0 = qtgui.time_sink_f(
        	25000, #size
        	chan_rate, #samp_rate
        	"QT GUI Plot", #name
        	1 #number of inputs
        )
        self.qtgui_time_sink_x_0.set_update_time(0.10)
        self.qtgui_time_sink_x_0.set_y_axis(-50, 20)
        self.qtgui_time_sink_x_0.enable_tags(-1, True)
        self.qtgui_time_sink_x_0.set_trigger_mode(qtgui.TRIG_MODE_FREE, qtgui.TRIG_SLOPE_POS, 0.0, 0, 0, "")
        self._qtgui_time_sink_x_0_win = sip.wrapinstance(self.qtgui_time_sink_x_0.pyqwidget(), Qt.QWidget)
        self.top_layout.addWidget(self._qtgui_time_sink_x_0_win)
        self.qtgui_histogram_sink_x_0 = qtgui.histogram_sink_f(
        	1500000,
        	100,
                0,
                4,
        	"QT GUI Plot",
        	1
        )
        self.qtgui_histogram_sink_x_0.set_update_time(0.10)
        self._qtgui_histogram_sink_x_0_win = sip.wrapinstance(self.qtgui_histogram_sink_x_0.pyqwidget(), Qt.QWidget)
        self.top_layout.addWidget(self._qtgui_histogram_sink_x_0_win)
        self._fd_tool_bar = Qt.QToolBar(self)
        self._fd_tool_bar.addWidget(Qt.QLabel("Doppler Rate, Hz"+": "))
        self._fd_line_edit = Qt.QLineEdit(str(self.fd))
        self._fd_tool_bar.addWidget(self._fd_line_edit)
        self._fd_line_edit.returnPressed.connect(
        	lambda: self.set_fd(eng_notation.str_to_num(self._fd_line_edit.text().toAscii())))
        self.top_layout.addWidget(self._fd_tool_bar)
        self.blocks_throttle_0 = blocks.throttle(gr.sizeof_gr_complex*1, chan_rate,True)
        self.blocks_nlog10_ff_0 = blocks.nlog10_ff(1, 1, 0)
        self.blocks_multiply_const_vxx_0 = blocks.multiply_const_vff((20, ))
        self.blocks_complex_to_mag_0 = blocks.complex_to_mag(1)
        self.analog_const_source_x_0 = analog.sig_source_c(0, analog.GR_CONST_WAVE, 0, 0, 1+1j)

        ##################################################
        # Connections
        ##################################################
        self.connect((self.analog_const_source_x_0, 0), (self.blocks_throttle_0, 0))
        self.connect((self.blocks_throttle_0, 0), (self.rccBlocks_rayleighChan_cc_0, 0))
        self.connect((self.rccBlocks_rayleighChan_cc_0, 0), (self.blocks_complex_to_mag_0, 0))
        self.connect((self.blocks_complex_to_mag_0, 0), (self.blocks_nlog10_ff_0, 0))
        self.connect((self.blocks_nlog10_ff_0, 0), (self.blocks_multiply_const_vxx_0, 0))
        self.connect((self.blocks_complex_to_mag_0, 0), (self.qtgui_histogram_sink_x_0, 0))
        self.connect((self.blocks_multiply_const_vxx_0, 0), (self.qtgui_time_sink_x_0, 0))


# QT sink close method reimplementation
    def closeEvent(self, event):
        self.settings = Qt.QSettings("GNU Radio", "top_block")
        self.settings.setValue("geometry", self.saveGeometry())
        event.accept()

    def get_fd(self):
        return self.fd

    def set_fd(self, fd):
        self.fd = fd
        self.set_fdTs(self.fd*(1.0/self.chan_rate))
        Qt.QMetaObject.invokeMethod(self._fd_line_edit, "setText", Qt.Q_ARG("QString", eng_notation.num_to_str(self.fd)))

    def get_chan_rate(self):
        return self.chan_rate

    def set_chan_rate(self, chan_rate):
        self.chan_rate = chan_rate
        self.set_fdTs(self.fd*(1.0/self.chan_rate))
        self.blocks_throttle_0.set_sample_rate(self.chan_rate)
        self.qtgui_time_sink_x_0.set_samp_rate(self.chan_rate)

    def get_fdTs(self):
        return self.fdTs

    def set_fdTs(self, fdTs):
        self.fdTs = fdTs
        self.rccBlocks_rayleighChan_cc_0.set_dopplerFreq(self.fdTs)

    def get_fadeMode(self):
        return self.fadeMode

    def set_fadeMode(self, fadeMode):
        self.fadeMode = fadeMode
        self._fadeMode_callback(self.fadeMode)
        self.rccBlocks_rayleighChan_cc_0.set_fadeMode(self.fadeMode)

if __name__ == '__main__':
    import ctypes
    import sys
    if sys.platform.startswith('linux'):
        try:
            x11 = ctypes.cdll.LoadLibrary('libX11.so')
            x11.XInitThreads()
        except:
            print "Warning: failed to XInitThreads()"
    parser = OptionParser(option_class=eng_option, usage="%prog: [options]")
    (options, args) = parser.parse_args()
    Qt.QApplication.setGraphicsSystem(gr.prefs().get_string('qtgui','style','raster'))
    qapp = Qt.QApplication(sys.argv)
    tb = top_block()
    tb.start()
    tb.show()
    def quitting():
        tb.stop()
        tb.wait()
    qapp.connect(qapp, Qt.SIGNAL("aboutToQuit()"), quitting)
    qapp.exec_()
    tb = None #to clean up Qt widgets

