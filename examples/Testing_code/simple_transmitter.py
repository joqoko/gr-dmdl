#!/usr/bin/env python2
# -*- coding: utf-8 -*-
##################################################
# GNU Radio Python Flow Graph
# Title: Simple Transmitter
# Generated: Tue Mar 12 18:28:01 2019
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
from gnuradio.qtgui import Range, RangeWidget
from optparse import OptionParser
import dmdl
import gnuradio
import sys
from gnuradio import qtgui


class simple_transmitter(gr.top_block, Qt.QWidget):

    def __init__(self):
        gr.top_block.__init__(self, "Simple Transmitter")
        Qt.QWidget.__init__(self)
        self.setWindowTitle("Simple Transmitter")
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

        self.settings = Qt.QSettings("GNU Radio", "simple_transmitter")
        self.restoreGeometry(self.settings.value("geometry").toByteArray())


        ##################################################
        # Variables
        ##################################################
        self.sps = sps = 4
        self.range_tx_gain = range_tx_gain = 15
        self.range_rx_gain = range_rx_gain = 0
        self.range_mu = range_mu = 0.6
        self.usrp_device_address = usrp_device_address = "addr=192.168.10.4"
        self.tx_gain = tx_gain = range_tx_gain
        self.tx_center_frequency = tx_center_frequency = 400000000
        self.source_address = source_address = 2
        self.samp_rate = samp_rate = 1000000
        self.rx_gain = rx_gain = range_rx_gain
        self.rx_center_frequency = rx_center_frequency = 400000000

        self.rrc = rrc = firdes.root_raised_cosine(1.0, sps, 1, 0.5, 11*sps)

        self.preamble_detector_threshold = preamble_detector_threshold = 30
        self.mu = mu = range_mu
        self.initial_frame_index = initial_frame_index = 1
        self.diff_preamble_128 = diff_preamble_128 = [1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 0, 1, 0, 1, 1, 0, 1, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 1, 1, 1, 0, 1, 0, 0, 1, 1, 0, 1, 1, 1, 1, 0, 1, 0, 1, 1, 1, 0, 1, 1, 0, 1, 1, 0, 1, 0, 1, 0, 1, 0, 1, 1, 0,0, 1, 1, 0, 1, 0, 0, 0, 1, 0, 1, 1, 1, 0, 0, 0, 1, 0, 0, 1, 1, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1, 0,0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 1, 1, 1, 1, 0, 1, 1, 0, 0, 0, 1, 0, 1, 0, 0, 1, 1, 1, 0, 0, 0, 0, 1, 1, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 1, 0, 0, 0, 1, 1, 0, 0, 0, 1, 1, 0, 1, 1, 0, 0, 1, 0, 1, 0, 1, 1, 1, 1, 0, 0, 1, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 1,1, 1, 1, 0, 0, 0, 1, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 1, 0, 1, 1, 1, 0, 0, 1, 0, 1, 1, 0, 0, 0, 0, 1, 0, 1, 1, 0, 1, 1, 1, 0, 1, 0, 1, 0, 0, 1, 0, 0][0:128]
        self.destination_address = destination_address = 1
        self.cs_threshold = cs_threshold = 0.005

        ##################################################
        # Blocks
        ##################################################
        self.tabs = Qt.QTabWidget()
        self.tabs_widget_0 = Qt.QWidget()
        self.tabs_layout_0 = Qt.QBoxLayout(Qt.QBoxLayout.TopToBottom, self.tabs_widget_0)
        self.tabs_grid_layout_0 = Qt.QGridLayout()
        self.tabs_layout_0.addLayout(self.tabs_grid_layout_0)
        self.tabs.addTab(self.tabs_widget_0, 'General')
        self.tabs_widget_1 = Qt.QWidget()
        self.tabs_layout_1 = Qt.QBoxLayout(Qt.QBoxLayout.TopToBottom, self.tabs_widget_1)
        self.tabs_grid_layout_1 = Qt.QGridLayout()
        self.tabs_layout_1.addLayout(self.tabs_grid_layout_1)
        self.tabs.addTab(self.tabs_widget_1, 'MAC')
        self.tabs_widget_2 = Qt.QWidget()
        self.tabs_layout_2 = Qt.QBoxLayout(Qt.QBoxLayout.TopToBottom, self.tabs_widget_2)
        self.tabs_grid_layout_2 = Qt.QGridLayout()
        self.tabs_layout_2.addLayout(self.tabs_grid_layout_2)
        self.tabs.addTab(self.tabs_widget_2, 'PHY and Radio')
        self.tabs_widget_3 = Qt.QWidget()
        self.tabs_layout_3 = Qt.QBoxLayout(Qt.QBoxLayout.TopToBottom, self.tabs_widget_3)
        self.tabs_grid_layout_3 = Qt.QGridLayout()
        self.tabs_layout_3.addLayout(self.tabs_grid_layout_3)
        self.tabs.addTab(self.tabs_widget_3, 'System')
        self.top_grid_layout.addWidget(self.tabs, 0, 0, 1, 1)
        for r in range(0, 1):
            self.top_grid_layout.setRowStretch(r, 1)
        for c in range(0, 1):
            self.top_grid_layout.setColumnStretch(c, 1)
        self._usrp_device_address_tool_bar = Qt.QToolBar(self)
        self._usrp_device_address_tool_bar.addWidget(Qt.QLabel('USRP address'+": "))
        self._usrp_device_address_line_edit = Qt.QLineEdit(str(self.usrp_device_address))
        self._usrp_device_address_tool_bar.addWidget(self._usrp_device_address_line_edit)
        self._usrp_device_address_line_edit.returnPressed.connect(
        	lambda: self.set_usrp_device_address(str(str(self._usrp_device_address_line_edit.text().toAscii()))))
        self.tabs_grid_layout_0.addWidget(self._usrp_device_address_tool_bar)
        self._tx_center_frequency_tool_bar = Qt.QToolBar(self)
        self._tx_center_frequency_tool_bar.addWidget(Qt.QLabel('TX center frequency [Hz]'+": "))
        self._tx_center_frequency_line_edit = Qt.QLineEdit(str(self.tx_center_frequency))
        self._tx_center_frequency_tool_bar.addWidget(self._tx_center_frequency_line_edit)
        self._tx_center_frequency_line_edit.returnPressed.connect(
        	lambda: self.set_tx_center_frequency(int(str(self._tx_center_frequency_line_edit.text().toAscii()))))
        self.tabs_grid_layout_2.addWidget(self._tx_center_frequency_tool_bar)
        self._sps_tool_bar = Qt.QToolBar(self)
        self._sps_tool_bar.addWidget(Qt.QLabel('Sample per Symbol'+": "))
        self._sps_line_edit = Qt.QLineEdit(str(self.sps))
        self._sps_tool_bar.addWidget(self._sps_line_edit)
        self._sps_line_edit.returnPressed.connect(
        	lambda: self.set_sps(int(str(self._sps_line_edit.text().toAscii()))))
        self.tabs_grid_layout_2.addWidget(self._sps_tool_bar)
        self._source_address_tool_bar = Qt.QToolBar(self)
        self._source_address_tool_bar.addWidget(Qt.QLabel('Node address'+": "))
        self._source_address_line_edit = Qt.QLineEdit(str(self.source_address))
        self._source_address_tool_bar.addWidget(self._source_address_line_edit)
        self._source_address_line_edit.returnPressed.connect(
        	lambda: self.set_source_address(int(str(self._source_address_line_edit.text().toAscii()))))
        self.tabs_grid_layout_0.addWidget(self._source_address_tool_bar)
        self._samp_rate_tool_bar = Qt.QToolBar(self)
        self._samp_rate_tool_bar.addWidget(Qt.QLabel('Sampling rate'+": "))
        self._samp_rate_line_edit = Qt.QLineEdit(str(self.samp_rate))
        self._samp_rate_tool_bar.addWidget(self._samp_rate_line_edit)
        self._samp_rate_line_edit.returnPressed.connect(
        	lambda: self.set_samp_rate(int(str(self._samp_rate_line_edit.text().toAscii()))))
        self.tabs_grid_layout_2.addWidget(self._samp_rate_tool_bar)
        self._diff_preamble_128_tool_bar = Qt.QToolBar(self)
        self._diff_preamble_128_tool_bar.addWidget(Qt.QLabel('Preamble 128'+": "))
        self._diff_preamble_128_line_edit = Qt.QLineEdit(str(self.diff_preamble_128))
        self._diff_preamble_128_tool_bar.addWidget(self._diff_preamble_128_line_edit)
        self._diff_preamble_128_line_edit.returnPressed.connect(
        	lambda: self.set_diff_preamble_128(eval(str(self._diff_preamble_128_line_edit.text().toAscii()))))
        self.tabs_grid_layout_2.addWidget(self._diff_preamble_128_tool_bar)
        self._destination_address_tool_bar = Qt.QToolBar(self)
        self._destination_address_tool_bar.addWidget(Qt.QLabel('Destination address'+": "))
        self._destination_address_line_edit = Qt.QLineEdit(str(self.destination_address))
        self._destination_address_tool_bar.addWidget(self._destination_address_line_edit)
        self._destination_address_line_edit.returnPressed.connect(
        	lambda: self.set_destination_address(int(str(self._destination_address_line_edit.text().toAscii()))))
        self.tabs_grid_layout_0.addWidget(self._destination_address_tool_bar)
        self._rx_center_frequency_tool_bar = Qt.QToolBar(self)
        self._rx_center_frequency_tool_bar.addWidget(Qt.QLabel('RX center frequency [Hz]'+": "))
        self._rx_center_frequency_line_edit = Qt.QLineEdit(str(self.rx_center_frequency))
        self._rx_center_frequency_tool_bar.addWidget(self._rx_center_frequency_line_edit)
        self._rx_center_frequency_line_edit.returnPressed.connect(
        	lambda: self.set_rx_center_frequency(int(str(self._rx_center_frequency_line_edit.text().toAscii()))))
        self.tabs_grid_layout_2.addWidget(self._rx_center_frequency_tool_bar)
        self._range_tx_gain_range = Range(0, 25, 1, 15, 200)
        self._range_tx_gain_win = RangeWidget(self._range_tx_gain_range, self.set_range_tx_gain, 'Tx Gain', "counter_slider", float)
        self.tabs_grid_layout_2.addWidget(self._range_tx_gain_win)
        self._range_rx_gain_range = Range(0, 25, 1, 0, 200)
        self._range_rx_gain_win = RangeWidget(self._range_rx_gain_range, self.set_range_rx_gain, 'Rx Gain', "counter_slider", float)
        self.tabs_grid_layout_2.addWidget(self._range_rx_gain_win)
        self._range_mu_range = Range(0, 1, 0.01, 0.6, 200)
        self._range_mu_win = RangeWidget(self._range_mu_range, self.set_range_mu, 'BB Derotation Gain', "counter_slider", float)
        self.tabs_grid_layout_2.addWidget(self._range_mu_win)
        self._preamble_detector_threshold_tool_bar = Qt.QToolBar(self)
        self._preamble_detector_threshold_tool_bar.addWidget(Qt.QLabel('Preamble Detector Threshold'+": "))
        self._preamble_detector_threshold_line_edit = Qt.QLineEdit(str(self.preamble_detector_threshold))
        self._preamble_detector_threshold_tool_bar.addWidget(self._preamble_detector_threshold_line_edit)
        self._preamble_detector_threshold_line_edit.returnPressed.connect(
        	lambda: self.set_preamble_detector_threshold(int(str(self._preamble_detector_threshold_line_edit.text().toAscii()))))
        self.tabs_grid_layout_2.addWidget(self._preamble_detector_threshold_tool_bar)
        self._initial_frame_index_tool_bar = Qt.QToolBar(self)
        self._initial_frame_index_tool_bar.addWidget(Qt.QLabel('Initial frame index'+": "))
        self._initial_frame_index_line_edit = Qt.QLineEdit(str(self.initial_frame_index))
        self._initial_frame_index_tool_bar.addWidget(self._initial_frame_index_line_edit)
        self._initial_frame_index_line_edit.returnPressed.connect(
        	lambda: self.set_initial_frame_index(int(str(self._initial_frame_index_line_edit.text().toAscii()))))
        self.tabs_grid_layout_1.addWidget(self._initial_frame_index_tool_bar)
        self.dmdl_start_0 = dmdl.start(5, 10)
        self.dmdl_sending_0 = dmdl.sending(0, 11, gnuradio.digital.constellation_qpsk().base(), diff_preamble_128, samp_rate, sps, 10, usrp_device_address, tx_center_frequency, 0.005, 0.05, "t1TXs", "Tfr", 0, 1, 0)
        self.dmdl_probe_0 = dmdl.probe(0, 100, 0, 0, 0.01, 0, "/home/inets/source/gr-inets/results/", "", 1)
        self.dmdl_framing_0 = dmdl.framing(0, 17, 1, 1, 0, 1, destination_address, 1, source_address, 1, 318, 2, 524, 2, 2, 1, 1, 0, ([2, 3]), ([1000, 1000]), 2, 0, 300, 1)
        self.dmdl_dummy_source_0 = dmdl.dummy_source(0, 23, 100, 2, 1)
        self._cs_threshold_tool_bar = Qt.QToolBar(self)
        self._cs_threshold_tool_bar.addWidget(Qt.QLabel('Carrier Sensing Threshold'+": "))
        self._cs_threshold_line_edit = Qt.QLineEdit(str(self.cs_threshold))
        self._cs_threshold_tool_bar.addWidget(self._cs_threshold_line_edit)
        self._cs_threshold_line_edit.returnPressed.connect(
        	lambda: self.set_cs_threshold(eng_notation.str_to_num(str(self._cs_threshold_line_edit.text().toAscii()))))
        self.tabs_grid_layout_2.addWidget(self._cs_threshold_tool_bar)



        ##################################################
        # Connections
        ##################################################
        self.msg_connect((self.dmdl_dummy_source_0, 'E'), (self.dmdl_framing_0, 'B'))
        self.msg_connect((self.dmdl_framing_0, 'E'), (self.dmdl_sending_0, 'B'))
        self.msg_connect((self.dmdl_sending_0, 'E'), (self.dmdl_probe_0, 'B'))
        self.msg_connect((self.dmdl_start_0, 'B'), (self.dmdl_dummy_source_0, 'B'))

    def closeEvent(self, event):
        self.settings = Qt.QSettings("GNU Radio", "simple_transmitter")
        self.settings.setValue("geometry", self.saveGeometry())
        event.accept()

    def get_sps(self):
        return self.sps

    def set_sps(self, sps):
        self.sps = sps
        Qt.QMetaObject.invokeMethod(self._sps_line_edit, "setText", Qt.Q_ARG("QString", str(self.sps)))

    def get_range_tx_gain(self):
        return self.range_tx_gain

    def set_range_tx_gain(self, range_tx_gain):
        self.range_tx_gain = range_tx_gain
        self.set_tx_gain(self.range_tx_gain)

    def get_range_rx_gain(self):
        return self.range_rx_gain

    def set_range_rx_gain(self, range_rx_gain):
        self.range_rx_gain = range_rx_gain
        self.set_rx_gain(self.range_rx_gain)

    def get_range_mu(self):
        return self.range_mu

    def set_range_mu(self, range_mu):
        self.range_mu = range_mu
        self.set_mu(self.range_mu)

    def get_usrp_device_address(self):
        return self.usrp_device_address

    def set_usrp_device_address(self, usrp_device_address):
        self.usrp_device_address = usrp_device_address
        Qt.QMetaObject.invokeMethod(self._usrp_device_address_line_edit, "setText", Qt.Q_ARG("QString", str(self.usrp_device_address)))

    def get_tx_gain(self):
        return self.tx_gain

    def set_tx_gain(self, tx_gain):
        self.tx_gain = tx_gain

    def get_tx_center_frequency(self):
        return self.tx_center_frequency

    def set_tx_center_frequency(self, tx_center_frequency):
        self.tx_center_frequency = tx_center_frequency
        Qt.QMetaObject.invokeMethod(self._tx_center_frequency_line_edit, "setText", Qt.Q_ARG("QString", str(self.tx_center_frequency)))

    def get_source_address(self):
        return self.source_address

    def set_source_address(self, source_address):
        self.source_address = source_address
        Qt.QMetaObject.invokeMethod(self._source_address_line_edit, "setText", Qt.Q_ARG("QString", str(self.source_address)))

    def get_samp_rate(self):
        return self.samp_rate

    def set_samp_rate(self, samp_rate):
        self.samp_rate = samp_rate
        Qt.QMetaObject.invokeMethod(self._samp_rate_line_edit, "setText", Qt.Q_ARG("QString", str(self.samp_rate)))

    def get_rx_gain(self):
        return self.rx_gain

    def set_rx_gain(self, rx_gain):
        self.rx_gain = rx_gain

    def get_rx_center_frequency(self):
        return self.rx_center_frequency

    def set_rx_center_frequency(self, rx_center_frequency):
        self.rx_center_frequency = rx_center_frequency
        Qt.QMetaObject.invokeMethod(self._rx_center_frequency_line_edit, "setText", Qt.Q_ARG("QString", str(self.rx_center_frequency)))

    def get_rrc(self):
        return self.rrc

    def set_rrc(self, rrc):
        self.rrc = rrc

    def get_preamble_detector_threshold(self):
        return self.preamble_detector_threshold

    def set_preamble_detector_threshold(self, preamble_detector_threshold):
        self.preamble_detector_threshold = preamble_detector_threshold
        Qt.QMetaObject.invokeMethod(self._preamble_detector_threshold_line_edit, "setText", Qt.Q_ARG("QString", str(self.preamble_detector_threshold)))

    def get_mu(self):
        return self.mu

    def set_mu(self, mu):
        self.mu = mu

    def get_initial_frame_index(self):
        return self.initial_frame_index

    def set_initial_frame_index(self, initial_frame_index):
        self.initial_frame_index = initial_frame_index
        Qt.QMetaObject.invokeMethod(self._initial_frame_index_line_edit, "setText", Qt.Q_ARG("QString", str(self.initial_frame_index)))

    def get_diff_preamble_128(self):
        return self.diff_preamble_128

    def set_diff_preamble_128(self, diff_preamble_128):
        self.diff_preamble_128 = diff_preamble_128
        Qt.QMetaObject.invokeMethod(self._diff_preamble_128_line_edit, "setText", Qt.Q_ARG("QString", repr(self.diff_preamble_128)))

    def get_destination_address(self):
        return self.destination_address

    def set_destination_address(self, destination_address):
        self.destination_address = destination_address
        Qt.QMetaObject.invokeMethod(self._destination_address_line_edit, "setText", Qt.Q_ARG("QString", str(self.destination_address)))

    def get_cs_threshold(self):
        return self.cs_threshold

    def set_cs_threshold(self, cs_threshold):
        self.cs_threshold = cs_threshold
        Qt.QMetaObject.invokeMethod(self._cs_threshold_line_edit, "setText", Qt.Q_ARG("QString", eng_notation.num_to_str(self.cs_threshold)))


def main(top_block_cls=simple_transmitter, options=None):

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
