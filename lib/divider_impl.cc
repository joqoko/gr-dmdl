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
#include "divider_impl.h"

namespace gr {
  namespace dmdl {

    divider::sptr
    divider::make(int develop_mode, int block_id, int counts, int mode)
    {
      return gnuradio::get_initial_sptr
        (new divider_impl(develop_mode, block_id, counts, mode));
    }

    /*
     * The private constructor
     */
    divider_impl::divider_impl(int develop_mode, int block_id, int counts, int mode)
      : gr::block("divider",
              gr::io_signature::make(0, 0, 0),
              gr::io_signature::make(0, 0, 0)),
        _develop_mode(develop_mode),
        _block_id(block_id),
        _counts(counts),
        _mode(mode),
        _current_count(0)
    {
      if(_develop_mode)
        std::cout << "develop_mode of divider id: " << _block_id << " is activated." << std::endl;
      message_port_register_out(pmt::mp("K"));
      message_port_register_out(pmt::mp("RE"));
      message_port_register_in(pmt::mp("B"));
      set_msg_handler(
        pmt::mp("B"),
        boost::bind(&divider_impl::counting, this, _1)
      );
      message_port_register_in(pmt::mp("RT"));
      set_msg_handler(
        pmt::mp("RT"),
        boost::bind(&divider_impl::reset, this, _1)
      );
      message_port_register_in(pmt::mp("G"));
      set_msg_handler(
        pmt::mp("G"),
        boost::bind(&divider_impl::set_counts, this, _1)
      );
    }

    /*
     * Our virtual destructor.
     */
    divider_impl::~divider_impl()
    {
    }

    void
    divider_impl::set_counts(pmt::pmt_t pmt_in)
    {
      if(pmt::is_integer(pmt_in))
      {
        _counts = pmt::to_long(pmt_in);
        if(_develop_mode)
          std::cout << "the number of counts in divider block ID " << _block_id << " is reset to " << _counts << std::endl;
      }
      else if(pmt::dict_has_key(pmt_in, pmt::mp("N_PU")))
      {
        pmt::pmt_t not_found;
        _counts = pmt::to_long(pmt::dict_ref(pmt_in, pmt::string_to_symbol("N_PU"), not_found));
        if(_develop_mode)
          std::cout << "the number of counts in divider block ID " << _block_id << " is reset to " << _counts << " according to CogMAC protocol " << std::endl;
      }
      else   
      {
        std::cout << "error: divider block ID " << _block_id << " can only reassign number of replicates to a integer number." << std::endl;
      }
    }

    void
    divider_impl::reset(pmt::pmt_t pmt_in)
    {
      if(_mode == 0)
      {
        if(_develop_mode)
          std::cout << "current counts is reset to 0 " << std::endl;
        _current_count = 0;
      }
    }

    void
    divider_impl::counting(pmt::pmt_t pmt_in)
    {
      // normal mode
      if(_mode == 0)
      {
        _current_count++;
        if(_current_count < _counts)
        {
          if(_develop_mode)
            std::cout << "current counts is :" << _current_count << ". target is " << _counts << std::endl;
          message_port_pub(pmt::mp("RE"), pmt_in);
        }
        else
        {
          if(_develop_mode == 1)
            std::cout << "current counts is :" << _current_count << ". target is " << _counts << std::endl;
          message_port_pub(pmt::mp("K"), pmt_in);
        } 
      }
      // frame index mode
      else
      {
        if(_develop_mode == 1)
          std::cout << "frame index mode is under construction now. " << std::endl;
      }
    }

  } /* namespace dmdl */
} /* namespace gr */

