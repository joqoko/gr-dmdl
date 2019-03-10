/* -*- c++ -*- */
/* 
 * Copyright 2018 <RWTH INETS & KTH RSLAB>.
 * 
 * This is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3, or (at your option)
 * any later version.
  
 * This software is distributed in the hope that it will be useful,
 e but WITHOUT ANY WARRANTY; without even the implied warranty of
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

#include <gnuradio/io_signature.h>
#include "type_check_impl.h"

namespace gr {
  namespace dmdl {

    type_check::sptr
    type_check::make(int develop_mode, int block_id, int data_frame, int ack_frame, int beacon_frame, int rts_frame, int cts_frame, int ampdu_frame, int amsdu_frame,int ampdu_subframe, int amsdu_subframe, int other_frame)
    {
      return gnuradio::get_initial_sptr
        (new type_check_impl(develop_mode, block_id, data_frame, ack_frame, beacon_frame, rts_frame, cts_frame, ampdu_frame, amsdu_frame, ampdu_subframe, amsdu_subframe, other_frame));
    }

    /*
     * The private constructor
     */
    type_check_impl::type_check_impl(int develop_mode, int block_id, int data_frame, int ack_frame, int beacon_frame, int rts_frame, int cts_frame, int ampdu_frame, int amsdu_frame,int ampdu_subframe, int amsdu_subframe, int other_frame)
      : gr::block("type_check",
              gr::io_signature::make(0, 0, 0),
              gr::io_signature::make(0, 0, 0)),
        _develop_mode(develop_mode),
        _block_id(block_id)
    {
      if(_develop_mode)
        std::cout << "develop_mode of frame_type check ID: " << _block_id << " is activated." << std::endl;
      message_port_register_in(pmt::mp("B"));
      message_port_register_out(pmt::mp("DATA"));
      message_port_register_out(pmt::mp("ACK"));
      message_port_register_out(pmt::mp("BEACON"));
      message_port_register_out(pmt::mp("RTS"));
      message_port_register_out(pmt::mp("CTS"));
      message_port_register_out(pmt::mp("AMPDU"));
      message_port_register_out(pmt::mp("AMSDU"));
      message_port_register_out(pmt::mp("AMPDUs"));
      message_port_register_out(pmt::mp("AMSDUs"));
      message_port_register_out(pmt::mp("OTHER"));
      set_msg_handler(pmt::mp("B"), boost::bind(&type_check_impl::selector, this, _1 ));
    }

    /*
     * Our virtual destructor.
     */
    type_check_impl::~type_check_impl()
    {
    }

    void
    type_check_impl::selector(pmt::pmt_t info)
    {  
      if(_develop_mode == 2)
      {
        struct timeval t; 
        gettimeofday(&t, NULL);
        double current_time = t.tv_sec - double(int(t.tv_sec/100)*100) + t.tv_usec / 1000000.0;
        std::cout << "at time " << current_time << " s, ";
      }
      if(_develop_mode)
        std::cout << "type_check ID: " << _block_id << " detects ";
      pmt::pmt_t not_found;
      int frame_type = pmt::to_long(pmt::dict_ref(info, pmt::string_to_symbol("frame_type"), not_found));
      // data frame info
      if(frame_type == 1)
      {
        if(_develop_mode)
          std::cout << "a data frame" << std::endl;
        message_port_pub(pmt::mp("DATA"), info);
      } 
      else if(frame_type == 2)
      {
        if(_develop_mode)
          std::cout << "an ack frame" << std::endl;
        message_port_pub(pmt::mp("ACK"), info);
      } 
      else if(frame_type == 3)
      {
        if(_develop_mode)
          std::cout << "a beacon frame" << std::endl;
        message_port_pub(pmt::mp("BEACON"), info);
      } 
      else if(frame_type == 4)
      {
        if(_develop_mode)
          std::cout << "a rts frame" << std::endl;
        message_port_pub(pmt::mp("RTS"), info);
      } 
      else if(frame_type == 5)
      {
        if(_develop_mode)
          std::cout << "a cts frame" << std::endl;
        message_port_pub(pmt::mp("CTS"), info);
      } 
      else if(frame_type == 6)
      {
        if(_develop_mode)
          std::cout << "an ampdu frame" << std::endl;
        message_port_pub(pmt::mp("AMPDU"), info);
      } 
      else if(frame_type == 7)
      {
        if(_develop_mode)
          std::cout << "an amsdu frame" << std::endl;
        message_port_pub(pmt::mp("AMSDU"), info);
      } 
      else if(frame_type == 8)
      {
        if(_develop_mode)
          std::cout << "an ampdu subframe" << std::endl;
        message_port_pub(pmt::mp("AMPDUs"), info);
      } 
      else if(frame_type == 9)
      {
        if(_develop_mode)
          std::cout << "an amsdu subframe" << std::endl;
        message_port_pub(pmt::mp("AMSDUs"), info);
      } 
      else
      {
        if(_develop_mode)
          std::cout << "an unknown frame" << std::endl;
        message_port_pub(pmt::mp("OTHER"), info);
      } 
    }

  } /* namespace dmdl */
} /* namespace gr */

