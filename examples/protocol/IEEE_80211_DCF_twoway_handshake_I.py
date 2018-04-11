#!/usr/bin/env python2
# -*- coding: utf-8 -*-
##################################################
# GNU Radio Python Flow Graph
# Title: Ieee 80211 Dcf Twoway Handshake I
# Generated: Fri Apr  6 16:55:40 2018
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
import gnuradio.digital
import sys
from gnuradio import qtgui


class IEEE_80211_DCF_twoway_handshake_I(gr.top_block, Qt.QWidget):

    def __init__(self, constellation=gnuradio.digital.constellation_qpsk().base(), preamble=[]):
        gr.top_block.__init__(self, "Ieee 80211 Dcf Twoway Handshake I")
        Qt.QWidget.__init__(self)
        self.setWindowTitle("Ieee 80211 Dcf Twoway Handshake I")
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

        self.settings = Qt.QSettings("GNU Radio", "IEEE_80211_DCF_twoway_handshake_I")
        self.restoreGeometry(self.settings.value("geometry").toByteArray())


        ##################################################
        # Parameters
        ##################################################
        self.constellation = constellation
        self.preamble = preamble

        ##################################################
        # Variables
        ##################################################
        self.sps = sps = 4
        self.usrp_device_address = usrp_device_address = "addr=192.168.10.2"
        self.tx_center_frequency = tx_center_frequency = 3.9e8
        self.timeout_duration_ms = timeout_duration_ms = 100
        self.system_time_granularity_us = system_time_granularity_us = 10
        self.source_address = source_address = 1
        self.slot_time = slot_time = 8
        self.samp_rate = samp_rate = 400000
        self.rx_gain = rx_gain = 0
        self.rx_center_frequency = rx_center_frequency = 3.9e8

        self.rrc = rrc = firdes.root_raised_cosine(1.0, sps, 1, 0.5, 11*sps)

        self.mu = mu = 0.6
        self.min_window_size = min_window_size = 1
        self.diff_preamble_128 = diff_preamble_128 = [1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 0, 1, 0, 1, 1, 0, 1, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 1, 1, 1, 0, 1, 0, 0, 1, 1, 0, 1, 1, 1, 1, 0, 1, 0, 1, 1, 1, 0, 1, 1, 0, 1, 1, 0, 1, 0, 1, 0, 1, 0, 1, 1, 0,0, 1, 1, 0, 1, 0, 0, 0, 1, 0, 1, 1, 1, 0, 0, 0, 1, 0, 0, 1, 1, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1, 0,0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 1, 1, 1, 1, 0, 1, 1, 0, 0, 0, 1, 0, 1, 0, 0, 1, 1, 1, 0, 0, 0, 0, 1, 1, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 1, 0, 0, 0, 1, 1, 0, 0, 0, 1, 1, 0, 1, 1, 0, 0, 1, 0, 1, 0, 1, 1, 1, 1, 0, 0, 1, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 1,1, 1, 1, 0, 0, 0, 1, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 1, 0, 1, 1, 1, 0, 0, 1, 0, 1, 1, 0, 0, 0, 0, 1, 0, 1, 1, 0, 1, 1, 1, 0, 1, 0, 1, 0, 0, 1, 0, 0][0:128]
        self.destination_address = destination_address = 2
        self.cs_threshold = cs_threshold = 0.001
        self.Single_rxp_time_s = Single_rxp_time_s = 60
        self.SIFS = SIFS = 10

        ##################################################
        # Blocks
        ##################################################
        self.dmdl_type_check_0_0 = dmdl.type_check(0, 25, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1)
        self.dmdl_type_check_0 = dmdl.type_check(0, 25, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1)
        self.dmdl_timer_0_0 = dmdl.timer(0, 5, 0, 20, 10, 0)
        self.dmdl_timer_0 = dmdl.timer(0, 5, 0, 50, 10, 0)
        self.dmdl_timeout_0 = dmdl.timeout(0, 10, 100, 1000, 0)
        self.dmdl_start_0 = dmdl.start(1, 10)
        self.dmdl_sending_0 = dmdl.sending(0, 11, gnuradio.digital.constellation_qpsk().base(), diff_preamble_128, samp_rate, sps, system_time_granularity_us, usrp_device_address, tx_center_frequency, 0.005, 0.05, "t1TXs", "Tfr", 0, 1, 0)
        self.dmdl_resend_check_0 = dmdl.resend_check(0, 24, 6)
        self.dmdl_receiving_0 = dmdl.receiving(1, 21, gnuradio.digital.constellation_qpsk().base(), rrc, mu, diff_preamble_128, rx_gain, samp_rate, sps, 30, usrp_device_address, rx_center_frequency)
        self.dmdl_probe_0_0 = dmdl.probe(1, 2, 0, 0, 0.01, 0, "/home/inets/source/gr-inets/results/", "", 1)
        self.dmdl_path_0 = dmdl.path(0, 44, 1, "")
        self.dmdl_framing_0_0 = dmdl.framing(0, 17, 2, 1, 0, 1, destination_address, 1, source_address, 1, 318, 2, 524, 2, 2, 1, 1, 0, ([2, 3]), ([1000, 1000]), 2, 0, 300, 1)
        self.dmdl_framing_0 = dmdl.framing(0, 17, 1, 1, 1, 1, destination_address, 1, source_address, 1, 318, 2, 524, 2, 2, 1, 1, 0, ([2, 3]), ([1000, 1000]), 2, 0, 300, 1)
        self.dmdl_frame_analysis_0 = dmdl.frame_analysis(0, 7, 1, 1, 1, 1, 1, 2, 2, 2, 1, source_address)
        self.dmdl_error_detection_0 = dmdl.error_detection(0, 9)
        self.dmdl_dummy_source_0 = dmdl.dummy_source(0, 23, 100, 1, 1)
        self.dmdl_carrier_sensing_0 = dmdl.carrier_sensing(0, 11, 2, 100, cs_threshold, system_time_granularity_us, 100, 20)
        self.dmdl_backoff_0_0 = dmdl.backoff(0, 11, 1, 10, 100, 400, 0, 0.005, 10, 1, 6)
        self.dmdl_backoff_0 = dmdl.backoff(0, 3, 1, 10, 100, 400, 0, 0.005, 10, 1, 2)
        self.dmdl_attribute_editor_0 = dmdl.attribute_editor(0, 16, "num_transmission", 1, 1)
        self.dmdl_address_check_0 = dmdl.address_check(0, 17, source_address, 0)



        ##################################################
        # Connections
        ##################################################
        self.msg_connect((self.dmdl_address_check_0, 'Pass'), (self.dmdl_type_check_0, 'Begin'))
        self.msg_connect((self.dmdl_attribute_editor_0, 'cmd_out'), (self.dmdl_path_0, 'Begin'))
        self.msg_connect((self.dmdl_backoff_0, 'End'), (self.dmdl_resend_check_0, 'Begin'))
        self.msg_connect((self.dmdl_backoff_0_0, 'End'), (self.dmdl_dummy_source_0, 'Begin'))
        self.msg_connect((self.dmdl_carrier_sensing_0, 'Fail'), (self.dmdl_backoff_0, 'Begin'))
        self.msg_connect((self.dmdl_carrier_sensing_0, 'cs_Threshold'), (self.dmdl_backoff_0, 'cs_Threshold'))
        self.msg_connect((self.dmdl_carrier_sensing_0, 'cs_Threshold'), (self.dmdl_backoff_0_0, 'cs_Threshold'))
        self.msg_connect((self.dmdl_carrier_sensing_0, 'Pass'), (self.dmdl_receiving_0, 'rx_switch_in'))
        self.msg_connect((self.dmdl_dummy_source_0, 'End'), (self.dmdl_framing_0, 'Begin'))
        self.msg_connect((self.dmdl_error_detection_0, 'Pass'), (self.dmdl_address_check_0, 'Begin'))
        self.msg_connect((self.dmdl_frame_analysis_0, 'Pass'), (self.dmdl_error_detection_0, 'Begin'))
        self.msg_connect((self.dmdl_frame_analysis_0, 'Pass'), (self.dmdl_probe_0_0, 'info_in'))
        self.msg_connect((self.dmdl_framing_0, 'End'), (self.dmdl_path_0, 'Begin'))
        self.msg_connect((self.dmdl_framing_0_0, 'End'), (self.dmdl_timer_0_0, 'Begin'))
        self.msg_connect((self.dmdl_path_0, 'End'), (self.dmdl_carrier_sensing_0, 'Begin'))
        self.msg_connect((self.dmdl_path_0, 'End'), (self.dmdl_timer_0, 'Begin'))
        self.msg_connect((self.dmdl_receiving_0, 'rx_power_out'), (self.dmdl_backoff_0, 'Rssi'))
        self.msg_connect((self.dmdl_receiving_0, 'rx_power_out'), (self.dmdl_backoff_0_0, 'Rssi'))
        self.msg_connect((self.dmdl_receiving_0, 'rx_power_out'), (self.dmdl_carrier_sensing_0, 'Rssi'))
        self.msg_connect((self.dmdl_receiving_0, 'rx_frame_out'), (self.dmdl_frame_analysis_0, 'Begin'))
        self.msg_connect((self.dmdl_receiving_0, 'rx_switch_out'), (self.dmdl_sending_0, 'in'))
        self.msg_connect((self.dmdl_resend_check_0, 'Pass'), (self.dmdl_attribute_editor_0, 'cmd_in'))
        self.msg_connect((self.dmdl_resend_check_0, 'Fail'), (self.dmdl_dummy_source_0, 'Begin'))
        self.msg_connect((self.dmdl_sending_0, 'rx_control_out'), (self.dmdl_receiving_0, 'rx_switch_in'))
        self.msg_connect((self.dmdl_sending_0, 'DATA'), (self.dmdl_timeout_0, 'Begin_DATA'))
        self.msg_connect((self.dmdl_start_0, 'Begin'), (self.dmdl_dummy_source_0, 'Begin'))
        self.msg_connect((self.dmdl_timeout_0, 'End'), (self.dmdl_type_check_0_0, 'Begin'))
        self.msg_connect((self.dmdl_timer_0, 'End'), (self.dmdl_carrier_sensing_0, 'Stop'))
        self.msg_connect((self.dmdl_timer_0_0, 'End'), (self.dmdl_receiving_0, 'rx_switch_in'))
        self.msg_connect((self.dmdl_type_check_0, 'DATA'), (self.dmdl_framing_0_0, 'Begin'))
        self.msg_connect((self.dmdl_type_check_0, 'ACK'), (self.dmdl_timeout_0, 'confIrm_ACK'))
        self.msg_connect((self.dmdl_type_check_0_0, 'DATA'), (self.dmdl_backoff_0, 'Begin'))
        self.msg_connect((self.dmdl_type_check_0_0, 'ACK'), (self.dmdl_backoff_0_0, 'Begin'))

    def closeEvent(self, event):
        self.settings = Qt.QSettings("GNU Radio", "IEEE_80211_DCF_twoway_handshake_I")
        self.settings.setValue("geometry", self.saveGeometry())
        event.accept()

    def get_constellation(self):
        return self.constellation

    def set_constellation(self, constellation):
        self.constellation = constellation

    def get_preamble(self):
        return self.preamble

    def set_preamble(self, preamble):
        self.preamble = preamble

    def get_sps(self):
        return self.sps

    def set_sps(self, sps):
        self.sps = sps

    def get_usrp_device_address(self):
        return self.usrp_device_address

    def set_usrp_device_address(self, usrp_device_address):
        self.usrp_device_address = usrp_device_address

    def get_tx_center_frequency(self):
        return self.tx_center_frequency

    def set_tx_center_frequency(self, tx_center_frequency):
        self.tx_center_frequency = tx_center_frequency

    def get_timeout_duration_ms(self):
        return self.timeout_duration_ms

    def set_timeout_duration_ms(self, timeout_duration_ms):
        self.timeout_duration_ms = timeout_duration_ms

    def get_system_time_granularity_us(self):
        return self.system_time_granularity_us

    def set_system_time_granularity_us(self, system_time_granularity_us):
        self.system_time_granularity_us = system_time_granularity_us

    def get_source_address(self):
        return self.source_address

    def set_source_address(self, source_address):
        self.source_address = source_address

    def get_slot_time(self):
        return self.slot_time

    def set_slot_time(self, slot_time):
        self.slot_time = slot_time

    def get_samp_rate(self):
        return self.samp_rate

    def set_samp_rate(self, samp_rate):
        self.samp_rate = samp_rate

    def get_rx_gain(self):
        return self.rx_gain

    def set_rx_gain(self, rx_gain):
        self.rx_gain = rx_gain

    def get_rx_center_frequency(self):
        return self.rx_center_frequency

    def set_rx_center_frequency(self, rx_center_frequency):
        self.rx_center_frequency = rx_center_frequency

    def get_rrc(self):
        return self.rrc

    def set_rrc(self, rrc):
        self.rrc = rrc

    def get_mu(self):
        return self.mu

    def set_mu(self, mu):
        self.mu = mu

    def get_min_window_size(self):
        return self.min_window_size

    def set_min_window_size(self, min_window_size):
        self.min_window_size = min_window_size

    def get_diff_preamble_128(self):
        return self.diff_preamble_128

    def set_diff_preamble_128(self, diff_preamble_128):
        self.diff_preamble_128 = diff_preamble_128

    def get_destination_address(self):
        return self.destination_address

    def set_destination_address(self, destination_address):
        self.destination_address = destination_address

    def get_cs_threshold(self):
        return self.cs_threshold

    def set_cs_threshold(self, cs_threshold):
        self.cs_threshold = cs_threshold

    def get_Single_rxp_time_s(self):
        return self.Single_rxp_time_s

    def set_Single_rxp_time_s(self, Single_rxp_time_s):
        self.Single_rxp_time_s = Single_rxp_time_s

    def get_SIFS(self):
        return self.SIFS

    def set_SIFS(self, SIFS):
        self.SIFS = SIFS


def argument_parser():
    parser = OptionParser(usage="%prog: [options]", option_class=eng_option)
    return parser


def main(top_block_cls=IEEE_80211_DCF_twoway_handshake_I, options=None):
    if options is None:
        options, _ = argument_parser().parse_args()

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
