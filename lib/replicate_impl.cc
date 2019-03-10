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
#include "replicate_impl.h"

namespace gr {
  namespace dmdl {

    replicate::sptr
    replicate::make(int develop_mode, int block_id, int replicate_number)
    {
      return gnuradio::get_initial_sptr
        (new replicate_impl(develop_mode, block_id, replicate_number));
    }

    /*
     * The private constructor
     */
    replicate_impl::replicate_impl(int develop_mode, int block_id, int replicate_number)
      : gr::block("replicate",
              gr::io_signature::make(0, 0, 0),
              gr::io_signature::make(0, 0, 0)),
        _develop_mode(develop_mode),
        _block_id(block_id),
        _replicate_number(replicate_number)
    {
      if(_develop_mode)
        std::cout << "develop_mode of replicate id: " << _block_id << " is activated." << std::endl;
      message_port_register_out(pmt::mp("E"));
      message_port_register_out(pmt::mp("CLO"));
      message_port_register_in(pmt::mp("B"));
      set_msg_handler(
        pmt::mp("B"),
        boost::bind(&replicate_impl::do_replicate, this, _1)
      );
      message_port_register_in(pmt::mp("G"));
      set_msg_handler(
        pmt::mp("G"),
        boost::bind(&replicate_impl::reset_number, this, _1)
      );
    }

    /*
     * Our virtual destructor.
     */
    replicate_impl::~replicate_impl()
    {
    }

    void
    replicate_impl::reset_number(pmt::pmt_t number_in)
    {
      if(pmt::is_integer(number_in))
      {
        _replicate_number = pmt::to_long(number_in);
        if(_develop_mode)
          std::cout << "replicate block ID " << _block_id << " is reset to " << _replicate_number << std::endl;
      }
      else if(pmt::dict_has_key(number_in, pmt::mp("N_Re_fr")))
      {
        pmt::pmt_t not_found;
        _replicate_number = pmt::to_long(pmt::dict_ref(number_in, pmt::string_to_symbol("N_Re_fr"), not_found));
        if(_develop_mode)
          std::cout << "replicate block ID " << _block_id << " is reset to " << _replicate_number << " according to CogMAC protocol " << std::endl;
      }
      else   
      {
        std::cout << "error: replicate block ID " << _block_id << " can only reassign number of replicates to a integer number." << std::endl;
      }
    }

    void
    replicate_impl::do_replicate(pmt::pmt_t cmd_in)
    {
      if(pmt::dict_has_key(cmd_in, pmt::string_to_symbol("frame_pmt")))
      {
        for(int i = 0; i < _replicate_number; i++)
          message_port_pub(pmt::mp("E"), cmd_in);
        message_port_pub(pmt::mp("complete_out"), pmt::from_long(0));
      }
      else
        std::cout << "error: replicate ID " << _block_id << " cannot replicate input pmt." << std::endl;
    }

  } /* namespace dmdl */
} /* namespace gr */

