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
#include "address_check_impl.h"

namespace gr {
  namespace dmdl {

    address_check::sptr
    address_check::make(int develop_mode, int block_id, int my_address, int mode)
    {
      return gnuradio::get_initial_sptr
        (new address_check_impl(develop_mode, block_id, my_address, mode));
    }

    /*
     * The private constructor
     */
    address_check_impl::address_check_impl(int develop_mode, int block_id, int my_address, int mode)
      : gr::block("address_check",
              gr::io_signature::make(0, 0, 0),
              gr::io_signature::make(0, 0, 0)),
        _block_id(block_id),
        _develop_mode(develop_mode),
        _mode(mode),
        _my_address(my_address)
    {
      if(_develop_mode)
        std::cout << "develop_mode of address_check ID: " << _block_id << " is activated." << std::endl;
      message_port_register_in(pmt::mp("B"));
      message_port_register_out(pmt::mp("P"));
      message_port_register_out(pmt::mp("PO"));
      message_port_register_out(pmt::mp("F"));
      set_msg_handler(pmt::mp("B"), boost::bind(&address_check_impl::check_address, this, _1 ));
    }

    /*
     * Our virtual destructor.
     */
    address_check_impl::~address_check_impl()
    {
    }

    void
    address_check_impl::check_address(pmt::pmt_t frame_info)
    {
      if(_develop_mode == 1)
      {
        std::cout << "++++++++   address_check ID: " << _block_id << "   +++++++++" << std::endl;
      }
      pmt::pmt_t not_found;
      // mode == 0, check the destination address of coming frame. Used for unicast
      // mode != 0, check the source address of coming frame. Used for debugging and special MAC
      int received_frame_address = 0;
      if(_mode == 0)
        received_frame_address = pmt::to_long(pmt::dict_ref(frame_info, pmt::string_to_symbol("destination_address"), not_found));
      else
        received_frame_address = pmt::to_long(pmt::dict_ref(frame_info, pmt::string_to_symbol("source_address"), not_found));
      int is_my_address = (_my_address == received_frame_address);
      if(_develop_mode == 1)
        std::cout << "My address is " << _my_address << " and rx frame address is " << received_frame_address << ". Frame check is: " << is_my_address << " (1: passed, 2: failed)." << std::endl;
      frame_info = pmt::dict_delete(frame_info, pmt::string_to_symbol("address_check"));
      frame_info = pmt::dict_add(frame_info, pmt::string_to_symbol("address_check"), pmt::from_long(is_my_address));
      if(is_my_address)
      {
        message_port_pub(pmt::mp("P"), frame_info);
        message_port_pub(pmt::mp("PO"), frame_info);
      }
      else
      {
        message_port_pub(pmt::mp("F"), frame_info);
      }
      if(_develop_mode == 2)
      {
        struct timeval t; 
        gettimeofday(&t, NULL);
        double current_time = t.tv_sec - double(int(t.tv_sec/100)*100) + t.tv_usec / 1000000.0;
        std::cout << "* address check ID: " << _block_id << " finish header analysis at time " << current_time << " s" << std::endl;
      }
    }

  } /* namespace dmdl */
} /* namespace gr */

