#!/usr/bin/env python2
# -*- coding: utf-8 -*-
##################################################
# GNU Radio Python Flow Graph
# Title: Top Block
# Generated: Tue Feb  6 10:16:34 2018
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
from gnuradio import eng_notation
from gnuradio import gr
from gnuradio.eng_option import eng_option
from gnuradio.filter import firdes
from optparse import OptionParser
import dmdl
import inets
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
        self.inets_run_0 = inets.run(5, 10)
        self.inets_frame_probe_0_0_0 = inets.frame_probe(1, 3, 0, 0, 0.01, 0, "/home/inets/source/gr-inets/results/", "", 1)
        self.inets_frame_probe_0_0 = inets.frame_probe(1, 2, 0, 0, 0.01, 0, "/home/inets/source/gr-inets/results/", "", 1)
        self.inets_frame_probe_0 = inets.frame_probe(1, 1, 0, 0, 0.01, 0, "/home/inets/source/gr-inets/results/", "", 1)
        self.inets_dummy_source_0 = inets.dummy_source(0, 23, 100, 1, 1)
        self.dmdl_replicate_0 = dmdl.replicate(0, 37, 4)
        self.dmdl_redundancy_remover_0 = dmdl.redundancy_remover(1, 49)
        self.dmdl_framing_0 = dmdl.framing(0, 17, 1, 1, 0, 1, 1, 1, 2, 1, 318, 2, 524, 2, 2, 1, 1, 0, ([2, 3]), ([1000, 1000]), 2, 0, 300, 1)



        ##################################################
        # Connections
        ##################################################
        self.msg_connect((self.dmdl_framing_0, 'End'), (self.dmdl_replicate_0, 'Begin'))
        self.msg_connect((self.dmdl_replicate_0, 'End'), (self.dmdl_redundancy_remover_0, 'Begin'))
        self.msg_connect((self.inets_dummy_source_0, 'output'), (self.dmdl_framing_0, 'Begin'))
        self.msg_connect((self.inets_run_0, 'trigger_out'), (self.inets_dummy_source_0, 'trigger'))

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
