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
#include "attribute_editor_impl.h"
#include <string>
#include <iostream>

namespace gr {
  namespace dmdl {

    attribute_editor::sptr
    attribute_editor::make(int develop_mode, int block_id, std::string field_name, int operation, double value)
    {
      return gnuradio::get_initial_sptr
        (new attribute_editor_impl(develop_mode, block_id, field_name, operation, value));
    }

    /*
     * The private constructor
     */
    attribute_editor_impl::attribute_editor_impl(int develop_mode, int block_id, std::string field_name, int operation, double value)
      : gr::block("attribute_editor",
              gr::io_signature::make(0, 0, 0),
              gr::io_signature::make(0, 0, 0)),
        _develop_mode(develop_mode),
        _block_id(block_id),
        _operation(operation),
        _value(value),
        _field_name(field_name)
    {
      if(_develop_mode)
        std::cout << "develop_mode of attribute_editor ID: " << _block_id << " is activated." << std::endl;
      message_port_register_in(pmt::mp("B"));
      message_port_register_out(pmt::mp("E"));
      set_msg_handler(pmt::mp("B"), boost::bind(&attribute_editor_impl::catagorizing, this, _1 ));
    }


    /*
     * Our virtual destructor.
     */
    attribute_editor_impl::~attribute_editor_impl()
    {
    }

    void
    attribute_editor_impl::catagorizing(pmt::pmt_t cmd_in)
    {
      if(_field_name.compare("num_transmission") == 0)
      {
        if(pmt::dict_has_key(cmd_in, pmt::mp(_field_name)))
        { 
          pmt::pmt_t not_found;
          int num_transmission = pmt::to_long(pmt::dict_ref(cmd_in, pmt::string_to_symbol("num_transmission"), not_found));
          num_transmission = operating(num_transmission);
          cmd_in = pmt::dict_delete(cmd_in, pmt::string_to_symbol("num_transmission"));
          cmd_in = pmt::dict_add(cmd_in, pmt::string_to_symbol("num_transmission"), pmt::from_long(num_transmission));
          message_port_pub(pmt::mp("E"), cmd_in);
        }
        else
          std::cout << "warning: attribute_editor ID " << _block_id << " input cmd  does not have such field. please ensure your connection is correct. " << std::endl;
      }
      else
      {
        std::cout << "warning: attribute_editor ID " << _block_id << " does not support such field. please ensure your connection is correct. " << std::endl;
      }
    }
 
    int
    attribute_editor_impl::operating(int n)
    {
      switch(_operation)
      {
        case 1:
        {
          n = n + int(_value);
          break;
        } 
        case 2:
        {
          n = n - int(_value);
          break;
        } 
        default: 
        {
          std::cout << "warning: attribute_editor ID " << _block_id << " does not support such operation. please ensure your connection is correct. " << std::endl;
          break;
        }
      }  
      return n;
    }

  } /* namespace dmdl */
} /* namespace gr */

