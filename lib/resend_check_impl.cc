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
#include "resend_check_impl.h"

namespace gr {
  namespace dmdl {

    resend_check::sptr
    resend_check::make(int develop_mode, int block_id, int max_resend)
    {
      return gnuradio::get_initial_sptr
        (new resend_check_impl(develop_mode, block_id, max_resend));
    }

    /*
     * The private constructor
     */
    resend_check_impl::resend_check_impl(int develop_mode, int block_id, int max_resend)
      : gr::block("resend_check",
              gr::io_signature::make(0, 0, 0),
              gr::io_signature::make(0, 0, 0)),
        _develop_mode(develop_mode),
        _block_id(block_id),
        _max_resend(max_resend)
    {
      if(_develop_mode)
        std::cout << "develop mode of resend_check ID: " << _block_id << " is activated." << std::endl;
      message_port_register_in(pmt::mp("B"));
      message_port_register_out(pmt::mp("P"));
      message_port_register_out(pmt::mp("F"));
      set_msg_handler(pmt::mp("B"), boost::bind(&resend_check_impl::check_resend, this, _1 ));
    }

    /*
     * Our virtual destructor.
     */
    resend_check_impl::~resend_check_impl()
    {
    }

    void
    resend_check_impl::check_resend(pmt::pmt_t frame_in)
    {
      pmt::pmt_t not_found;
      int n_transmission = pmt::to_long(pmt::dict_ref(frame_in, pmt::string_to_symbol("num_transmission"), not_found));
      if(n_transmission >= _max_resend)
      {
        if(_develop_mode)
          std::cout <<"frame: " << pmt::to_long(pmt::dict_ref(frame_in, pmt::string_to_symbol("frame_index"), not_found)) <<  " from node: " << pmt::to_long(pmt::dict_ref(frame_in, pmt::string_to_symbol("source_address"), not_found)) << " is dropped because its max retransmission counter is reached. " << std::endl;
        message_port_pub(pmt::mp("F"), frame_in);
      }
      else
      {
        int frame_type = pmt::to_long(pmt::dict_ref(frame_in, pmt::string_to_symbol("frame_type"), not_found));
        if(frame_type == 1)
        {
          if(_develop_mode)
            std::cout << "a data frame with index " << pmt::to_long(pmt::dict_ref(frame_in, pmt::string_to_symbol("frame_index"), not_found)) << " from node: " << pmt::to_long(pmt::dict_ref(frame_in, pmt::string_to_symbol("source_address"), not_found))  << " has been transmitted " << n_transmission  << "th time" << std::endl;
        } 
        else if(frame_type == 2)
        {
          if(_develop_mode)
            std::cout << "warning: resend_check ID " << _block_id << ", an ack frame should not be retransmitted. (it is still transmitted out)" << std::endl;
        } 
        else if(frame_type == 3)
        {
          if(_develop_mode)
            std::cout << "a beacon frame with index " << pmt::to_long(pmt::dict_ref(frame_in, pmt::string_to_symbol("frame_index"), not_found)) << " from node: " << pmt::to_long(pmt::dict_ref(frame_in, pmt::string_to_symbol("source_address"), not_found))  << " has been transmitted " << n_transmission  << "th time" << std::endl;
        } 
        else if(frame_type == 4)
        {
          if(_develop_mode)
            std::cout << "a rts frame represents original data frame index " << pmt::to_long(pmt::dict_ref(frame_in, pmt::string_to_symbol("frame_index"), not_found)) << " from node: " << pmt::to_long(pmt::dict_ref(frame_in, pmt::string_to_symbol("source_address"), not_found))  << " has been transmitted " << n_transmission  << "th time" << std::endl;
        } 
        else if(frame_type == 5)
        {
          if(_develop_mode)
            std::cout << "warning: resend_check ID " << _block_id << ", a cts frame should not be retransmitted. (it is still transmitted out)" << std::endl;
        } 
        else if(frame_type == 6)
        {
          if(_develop_mode)
            std::cout << "an ampdu frame with index " << pmt::to_long(pmt::dict_ref(frame_in, pmt::string_to_symbol("frame_index"), not_found)) << " from node: " << pmt::to_long(pmt::dict_ref(frame_in, pmt::string_to_symbol("source_address"), not_found))  << " has been transmitted " << n_transmission  << "th time" << std::endl;
        } 
        else if(frame_type == 7)
        {
          if(_develop_mode)
            std::cout << "an amsdu frame with index " << pmt::to_long(pmt::dict_ref(frame_in, pmt::string_to_symbol("frame_index"), not_found)) << " from node: " << pmt::to_long(pmt::dict_ref(frame_in, pmt::string_to_symbol("source_address"), not_found))  << " has been transmitted " << n_transmission  << "th time" << std::endl;
        } 
        else if(frame_type == 8)
        {
          if(_develop_mode)
            std::cout << "warning: resend_check ID " << _block_id << ", an ampdu subframe should not be retransmitted. (it is still transmitted out)" << std::endl;
        } 
        else if(frame_type == 9)
        {
          if(_develop_mode)
            std::cout << "warning: resend_check ID " << _block_id << ", an amsdu subframe should not be retransmitted. (it is still transmitted out)" << std::endl;
          if(_develop_mode)
            std::cout << "an amsdu subframe" << std::endl;
        } 
        else
        {
          if(_develop_mode)
            std::cout << "an unknown type frame with index " << pmt::to_long(pmt::dict_ref(frame_in, pmt::string_to_symbol("frame_index"), not_found)) << " from node: " << pmt::to_long(pmt::dict_ref(frame_in, pmt::string_to_symbol("source_address"), not_found))  << " has been transmitted " << n_transmission  << "th time" << std::endl;
        } 
        message_port_pub(pmt::mp("P"),frame_in);
      }
    }

  } /* namespace dmdl */
} /* namespace gr */

