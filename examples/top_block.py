#!/usr/bin/env python2
# -*- coding: utf-8 -*-
##################################################
# GNU Radio Python Flow Graph
# Title: Top Block
# Generated: Sat Mar  9 20:58:25 2019
##################################################

if __name__ == '__main__':
    import ctypes
    import sys
    if sys.platform.startswith('linux'):
        try:
            x11 = ctypes.cdll.LoadLibrary('libX11.so')
            x11.XInitThreads()
        except:
            print "Warning: failed to XInitThreads()"

from PyQt4 import Qt
from gnuradio import blocks
from gnuradio import eng_notation
from gnuradio import gr
from gnuradio.eng_option import eng_option
from gnuradio.filter import firdes
from optparse import OptionParser
import dmdl
import pmt
import sys
from gnuradio import qtgui


class top_block(gr.top_block, Qt.QWidget):

    def __init__(self):
        gr.top_block.__init__(self, "Top Block")
        Qt.QWidget.__init__(self)
        self.setWindowTitle("Top Block")
        qtgui.util.check_set_qss()
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
        self.samp_rate = samp_rate = 32000

        ##################################################
        # Blocks
        ##################################################
        self.dmdl_probe_1 = dmdl.probe(0, 100, 0, 0, 0.01, 0, "/home/inets/source/gr-inets/results/", "", 1)
        self.dmdl_probe_0 = dmdl.probe(0, 100, 0, 0, 0.01, 0, "/home/inets/source/gr-inets/results/", "", 1)
        self.dmdl_framing_0 = dmdl.framing(0, 17, 1, 1, 0, 1, 1, 1, 2, 1, 318, 2, 524, 2, 2, 1, 1, 0, ([2, 3]), ([1000, 1000]), 2, 0, 300, 1)
        self.dmdl_dummy_source_0 = dmdl.dummy_source(0, 23, 100, 3, 1)
        self.dmdl_cmd_switch_0 = dmdl.cmd_switch(1, 47)
        self.blocks_message_strobe_0 = blocks.message_strobe(pmt.intern("TEST"), 1000)

        ##################################################
        # Connections
        ##################################################
        self.msg_connect((self.blocks_message_strobe_0, 'strobe'), (self.dmdl_cmd_switch_0, 'OFF'))
        self.msg_connect((self.blocks_message_strobe_0, 'strobe'), (self.dmdl_dummy_source_0, 'Begin'))
        self.msg_connect((self.dmdl_cmd_switch_0, 'E'), (self.dmdl_probe_1, 'info_in'))
        self.msg_connect((self.dmdl_dummy_source_0, 'End'), (self.dmdl_framing_0, 'Begin'))
        self.msg_connect((self.dmdl_framing_0, 'End'), (self.dmdl_cmd_switch_0, 'B'))

    def closeEvent(self, event):
        self.settings = Qt.QSettings("GNU Radio", "top_block")
        self.settings.setValue("geometry", self.saveGeometry())
        event.accept()

    def get_samp_rate(self):
        return self.samp_rate

    def set_samp_rate(self, samp_rate):
        self.samp_rate = samp_rate


def main(top_block_cls=top_block, options=None):

    from distutils.version import StrictVersion
    if StrictVersion(Qt.qVersion()) >= StrictVersion("4.5.0"):
        style = gr.prefs().get_string('qtgui', 'style', 'raster')
        Qt.QApplication.setGraphicsSystem(style)
    qapp = Qt.QApplication(sys.argv)

    tb = top_block_cls()
    tb.start()
    tb.show()

    def quitting():
        tb.stop()
        tb.wait()
    qapp.connect(qapp, Qt.SIGNAL("aboutToQuit()"), quitting)
    qapp.exec_()


if __name__ == '__main__':
    main()
