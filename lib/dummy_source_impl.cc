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
#include "dummy_source_impl.h"

namespace gr {
  namespace dmdl {

    dummy_source::sptr
    dummy_source::make(int develop_mode, int block_id, int payload_length, int source_type, double packet_rate)
    {
      return gnuradio::get_initial_sptr
        (new dummy_source_impl(develop_mode, block_id, payload_length, source_type, packet_rate));
    }

    /*
     * The private constructor
     */
    dummy_source_impl::dummy_source_impl(int develop_mode, int block_id, int payload_length, int source_type, double packet_rate)
      : gr::block("dummy_source",
              gr::io_signature::make(0, 0, 0),
              gr::io_signature::make(0, 0, 0)),
        _develop_mode(develop_mode),
        _block_id(block_id),
        _payload_length(payload_length),
        _source_type(source_type),
        _packet_rate(packet_rate),
        _start(0)
    {
      if(_develop_mode)
        std::cout << "develop_mode of dummy source ID: " << _block_id << " is activated." << std::endl;
      _generating = false;
      message_port_register_in(pmt::mp("B")); 
      set_msg_handler(pmt::mp("B"), boost::bind(&dummy_source_impl::trigger, this, _1));
      message_port_register_in(pmt::mp("S")); 
      set_msg_handler(pmt::mp("S"), boost::bind(&dummy_source_impl::stop_generation, this, _1));
      message_port_register_out(pmt::mp("E"));
      std::srand(std::time(0));
      for (unsigned int i = 0; i < _payload_length; i++)
      {
        _payload.push_back(std::rand() % 255);
      }  
    }

    /*
     * Our virtual destructor.
     */
    dummy_source_impl::~dummy_source_impl()
    {
    }

    void
    dummy_source_impl::stop_generation(pmt::pmt_t trig)
    {
      _generating = false;
      if(_develop_mode)
        std::cout << "dummy_source ID " << _block_id << " stops generating frame." << std::endl;
      std::cout << "dummy_source ID " << _block_id << " dummy frame generation is set to false" << std::endl;
    }

    void
    dummy_source_impl::trigger (pmt::pmt_t trig)
    {
      _generating = true;
      if(_source_type == 1)
      {
        if(pmt::is_integer(trig))
        {
          for(int i = 0; i < pmt::to_long(trig); i++)
          {
            std::vector<unsigned char> payload(_payload_length);
            message_port_pub(pmt::mp("E"), pmt::cons(pmt::make_dict(), pmt::init_u8vector(payload.size(), payload)));
            if(_develop_mode)
              std::cout << "dummy infinite source ID: " << _block_id << " generate a payload." << std::endl;
          }
          if(_develop_mode)
            std::cout << "dummy infinite source ID: " << _block_id << " generate and E " << pmt::to_long(trig) << " payloads in a row." << std::endl;
//          message_port_pub(pmt::mp("E"), trig);
        }
        else
        {
          message_port_pub(pmt::mp("E"), pmt::cons(pmt::make_dict(), pmt::init_u8vector(_payload.size(), _payload)));
        }
      }
      /*
       * source_type 2: constant rate source. 
       */
      else if(_source_type == 2)
      {  
        if(_develop_mode)
          std::cout << "dummy source ID: " << _block_id << "starts generating payload with constant data rate." << std::endl;
        struct timeval t;
        while(_generating)
        {
          boost::this_thread::sleep(boost::posix_time::microseconds(1 / _packet_rate * 1000000));
          gettimeofday(&t, NULL);
          double current_time = t.tv_sec + t.tv_usec / 1000000.0;
          message_port_pub(pmt::mp("E"), pmt::cons(pmt::make_dict(), pmt::init_u8vector(_payload.size(), _payload)));
          if(_develop_mode)
            std::cout << "dummy constant rate source ID: " << _block_id << " generate a payload." << std::endl;
        }
      }
      else if(_source_type == 3)
      {  
        if(_develop_mode)
          std::cout << "dummy source ID: " << _block_id << " generates an oneshot payload." << std::endl;
        message_port_pub(pmt::mp("E"), pmt::cons(pmt::make_dict(), pmt::init_u8vector(_payload.size(), _payload)));
      }
      else
        std::cout << "The chosen source is not supported yet. Your contribution is welcome." << std::endl; 
    }

  } /* namespace dmdl */
} /* namespace gr */

