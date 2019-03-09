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
#include "attribute_assignment_impl.h"

namespace gr {
  namespace dmdl {

    attribute_assignment::sptr
    attribute_assignment::make(int develop_mode, int block_id, std::string field_name, int value)
    {
      return gnuradio::get_initial_sptr
        (new attribute_assignment_impl(develop_mode, block_id, field_name, value));
    }

    /*
     * The private constructor
     */
    attribute_assignment_impl::attribute_assignment_impl(int develop_mode, int block_id, std::string field_name, int value)
      : gr::block("attribute_assignment",
              gr::io_signature::make(0, 0, 0),
              gr::io_signature::make(0, 0, 0)),
        _develop_mode(develop_mode),
        _block_id(block_id),
        _field_name(field_name),
        _value(value)
    {
      if(_develop_mode)
        std::cout << "develop_mode of attribute editor ID: " << _block_id << " is activated." << std::endl;
      message_port_register_in(pmt::mp("B"));
      set_msg_handler(pmt::mp("B"), boost::bind(&attribute_assignment_impl::reassign, this, _1 ));
      message_port_register_in(pmt::mp("G"));
      set_msg_handler(pmt::mp("G"), boost::bind(&attribute_assignment_impl::reset, this, _1 ));
      message_port_register_out(pmt::mp("E"));
    }

    /*
     * Our virtual destructor.
     */
    attribute_assignment_impl::~attribute_assignment_impl()
    {
    }

    void
    attribute_assignment_impl::reassign(pmt::pmt_t cmd_in)
    {
      pmt::pmt_t not_found;
      if(pmt::is_dict(cmd_in))
      {
        if(pmt::dict_has_key(cmd_in, pmt::string_to_symbol(_field_name)))
        {
          int attribute = pmt::to_long(pmt::dict_ref(cmd_in, pmt::string_to_symbol(_field_name), not_found));
          cmd_in = pmt::dict_delete(cmd_in, pmt::string_to_symbol(_field_name));
          cmd_in = pmt::dict_add(cmd_in, pmt::string_to_symbol(_field_name), pmt::from_long(_value));
          std::cout << "Attribute: " << _field_name << " value: " << attribute << " in the arrived cmd in attribute editor ID: " << _block_id<< " is replaced by " << _value << std::endl;
          message_port_pub(pmt::mp("E"), cmd_in);
        }
        else
        {
          if(_develop_mode == 1)
            std::cout << "Arrived cmd in attribute editor ID: " << _block_id<< " has no attribute " << _field_name << std::endl;
        }
      } 
      else
      {
        if(_develop_mode == 1)
          std::cout << "Arrived cmd in attribute editor ID: " << _block_id<< " is not a pmt" << std::endl;
      }
    }

    void
    attribute_assignment_impl::reset(pmt::pmt_t cmd_in)
    {
      pmt::pmt_t not_found;
      if(pmt::is_dict(cmd_in))
      {
	if(pmt::is_number(cmd_in))
        {
          _value = pmt::to_long(cmd_in);
          if(_develop_mode == 1)
            std::cout << "Default value of attribute editor ID: " << _block_id<< " is changed to " << _value << std::endl;
        }         
        else
        {
          if(_develop_mode == 1)
            std::cout << "Error: input of attribute editor ID: " << _block_id<< " is not a integer" << std::endl;
        }
      }
      else
      {
        if(_develop_mode == 1)
          std::cout << "Error: input of attribute editor ID: " << _block_id<< " is not a pmt" << std::endl;
      }
    }

  } /* namespace dmdl */
} /* namespace gr */

