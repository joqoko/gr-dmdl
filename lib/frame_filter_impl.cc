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
#include "frame_filter_impl.h"

namespace gr {
  namespace dmdl {

    frame_filter::sptr
    frame_filter::make(int develop_mode, int block_id, int drop_type, int frame_type, int source_address, int destination_address, int frame_index, int reserved_field_I, int reserved_field_II, int number_of_filtering)
    {
      return gnuradio::get_initial_sptr
        (new frame_filter_impl(develop_mode, block_id, drop_type, frame_type, source_address, destination_address, frame_index, reserved_field_I, reserved_field_II, number_of_filtering));
    }

    /*
     * The private constructor
     */
    frame_filter_impl::frame_filter_impl(int develop_mode, int block_id, int drop_type, int frame_type, int source_address, int destination_address, int frame_index, int reserved_field_I, int reserved_field_II, int number_of_filtering)
      : gr::block("frame_filter",
              gr::io_signature::make(0, 0, 0),
              gr::io_signature::make(0, 0, 0)),
        _develop_mode(develop_mode),
        _block_id(block_id),
        _drop_type(drop_type),
        _source_address(source_address),
        _destination_address(destination_address),
        _frame_index(frame_index),
        _frame_type(frame_type),
        _reserved_field_I(reserved_field_I),
        _reserved_field_II(reserved_field_II),
        _number_of_filtering(number_of_filtering)
    {
      if(_develop_mode)
        std::cout << "develop_mode of frame_filter id: " << _block_id << " is activated." << std::endl;
      message_port_register_in(pmt::mp("Begin")); 
      set_msg_handler(pmt::mp("Begin"), boost::bind(&frame_filter_impl::filtering, this, _1));
      message_port_register_out(pmt::mp("End_unfiltered"));
      message_port_register_out(pmt::mp("End"));
    }

    /*
     * Our virtual destructor.
     */
    frame_filter_impl::~frame_filter_impl()
    {
    }

    void
    frame_filter_impl::filtering(pmt::pmt_t frame_in)
    {
      pmt::pmt_t not_found;
      if(_number_of_filtering > 0)
      {
        //if(_develop_mode)
         // std::cout << "+++++++ frame_filter ID: " << _block_id << " get a frame +++++++" << std::endl;
        switch(_drop_type)
        {
          // case 0: frame_index
          case 0 :  
            {
              if(pmt::to_long(pmt::dict_ref(frame_in, pmt::string_to_symbol("frame_index"), not_found)) != _frame_index)
              {
                message_port_pub(pmt::mp("End_unfiltered"), frame_in);
              }
              else
              {
                if(_develop_mode)
                std::cout << "successfully filtered one targeted frame." << std::endl;
                _number_of_filtering--;
                message_port_pub(pmt::mp("End"), frame_in);
              }
              break;
            }
          case 1 : 
            {
              if(pmt::to_long(pmt::dict_ref(frame_in, pmt::string_to_symbol("frame_type"), not_found)) != _frame_type)
              {
                message_port_pub(pmt::mp("End_unfiltered"), frame_in);
              }
              else
              {
                if(_develop_mode)
                std::cout << "successfully filtered one targeted frame." << std::endl;
                _number_of_filtering--;
                message_port_pub(pmt::mp("End"), frame_in);
              }
              break;
            }
          case 2 :  
            {
              if(pmt::to_long(pmt::dict_ref(frame_in, pmt::string_to_symbol("source_address"), not_found)) != _source_address)
              {
                message_port_pub(pmt::mp("End_unfiltered"), frame_in);
              }
              else
              {
                if(_develop_mode)
                std::cout << "successfully filtered one targeted frame." << std::endl;
                _number_of_filtering--;
                message_port_pub(pmt::mp("End"), frame_in);
              }
              break;
            }
          case 3 :  
            {
              if(pmt::to_long(pmt::dict_ref(frame_in, pmt::string_to_symbol("destination_address"), not_found)) != _source_address)
              {
                message_port_pub(pmt::mp("End_unfiltered"), frame_in);
              }
              else
              {
                if(_develop_mode)
                std::cout << "successfully filtered one targeted frame." << std::endl;
                _number_of_filtering--;
                message_port_pub(pmt::mp("End"), frame_in);
              }
              break;
            }
          case 4 :  
            {
              if(pmt::to_long(pmt::dict_ref(frame_in, pmt::string_to_symbol("reserved_field_I"), not_found)) != _source_address)
              {
                message_port_pub(pmt::mp("End_unfiltered"), frame_in);
              }
              else
              {
                if(_develop_mode)
                std::cout << "successfully filtered one targeted frame." << std::endl;
                _number_of_filtering--;
                message_port_pub(pmt::mp("End"), frame_in);
              }
              break;
            }
          case 5 :  
            {
              if(pmt::to_long(pmt::dict_ref(frame_in, pmt::string_to_symbol("reserved_field_II"), not_found)) != _source_address)
              {
                message_port_pub(pmt::mp("End_unfiltered"), frame_in);
              }
              else
              {
                if(_develop_mode)
                std::cout << "successfully filtered one targeted frame." << std::endl;
                _number_of_filtering--;
                message_port_pub(pmt::mp("End"), frame_in);
              }
              break;
            }
          default:
            {
              if(_develop_mode)
                std::cout << "No frame is filtered. " << std::endl;
              message_port_pub(pmt::mp("End_unfiltered"), frame_in);
            }    
        }
      }
      else
        message_port_pub(pmt::mp("End_unfiltered"), frame_in);
    }

  } /* namespace dmdl */
} /* namespace gr */

