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
#include "attribute_splitter_impl.h"

namespace gr {
  namespace dmdl {

    attribute_splitter::sptr
    attribute_splitter::make(int develop_mode, int block_id, std::string command_name)
    {
      return gnuradio::get_initial_sptr
        (new attribute_splitter_impl(develop_mode, block_id, command_name));
    }

    /*
     * The private constructor
     */
    attribute_splitter_impl::attribute_splitter_impl(int develop_mode, int block_id, std::string command_name)
      : gr::block("attribute_splitter",
              gr::io_signature::make(0, 0, 0),
              gr::io_signature::make(0, 0, 0)),
        _develop_mode(develop_mode),
        _block_id(block_id),
        _command_name(command_name)
    {
      message_port_register_out(pmt::mp("E"));
      message_port_register_in(pmt::mp("B"));
      set_msg_handler(
        pmt::mp("B"),
        boost::bind(&attribute_splitter_impl::splitting, this, _1)
      ); 
    }

    /*
     * Our virtual destructor.
     */
    attribute_splitter_impl::~attribute_splitter_impl()
    {
    }

    void
    attribute_splitter_impl::splitting(pmt::pmt_t cmd_in)
    {
      if(pmt::dict_has_key(cmd_in, pmt::mp(_command_name)))
      {
        pmt::pmt_t not_found;
        message_port_pub(pmt::mp("E"), pmt::dict_ref(cmd_in, pmt::string_to_symbol(_command_name), not_found));
        if(_develop_mode)
          std::cout << "attribute_splitter block ID " << _block_id << " has field " << _command_name << ". the field is exported." << std::endl;
      }
      else
      {
        if(_develop_mode)
          std::cout << "attribute_splitter block ID " << _block_id << " has no field " << _command_name << "." << std::endl;
      }
    }

  } /* namespace dmdl */
} /* namespace gr */

