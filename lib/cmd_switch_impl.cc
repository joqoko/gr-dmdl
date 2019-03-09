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
#include "cmd_switch_impl.h"

namespace gr {
  namespace dmdl {

    cmd_switch::sptr
    cmd_switch::make(int develop_mode, int block_id)
    {
      return gnuradio::get_initial_sptr
        (new cmd_switch_impl(develop_mode, block_id));
    }

    /*
     * The private constructor
     */
    cmd_switch_impl::cmd_switch_impl(int develop_mode, int block_id)
      : gr::block("cmd_switch",
              gr::io_signature::make(0, 0, 0),
              gr::io_signature::make(0, 0, 0)),
        _develop_mode(develop_mode),
        _block_id(block_id),
        _switchs(true)
    {
      if(_develop_mode)
        std::cout << "develop_mode of cmd_switch id: " << _block_id << " is activated." << std::endl;
      message_port_register_in(pmt::mp("B"));
      set_msg_handler(pmt::mp("B"), boost::bind(&cmd_switch_impl::in_and_out, this, _1 ));
      message_port_register_in(pmt::mp("OFF"));
      set_msg_handler(pmt::mp("OFF"), boost::bind(&cmd_switch_impl::set_off, this, _1 ));
      message_port_register_in(pmt::mp("ON"));
      set_msg_handler(pmt::mp("ON"), boost::bind(&cmd_switch_impl::set_on, this, _1 ));
      message_port_register_out(pmt::mp("E"));
    }


    /*
     * our virtual destructor.
     */
    cmd_switch_impl::~cmd_switch_impl()
    {
    }

    void
    cmd_switch_impl::in_and_out(pmt::pmt_t cmd_in)
    {
      if(_switchs)
      {
        message_port_pub(pmt::mp("E"), cmd_in);
        if(_develop_mode)
          std::cout << "cmd_switch block id " << _block_id << " forwards cmd" << std::endl;
      }
      else
      {
        if(_develop_mode)
          std::cout << "cmd_switch block id " << _block_id << " cannot forward cmd" << std::endl;
      }
    }

    void
    cmd_switch_impl::set_off(pmt::pmt_t cmd_in)
    {
      _switchs = false;
      if(_develop_mode)
        std::cout << "cmd_switch block id " << _block_id << " is OFF." << std::endl;
    }

    void
    cmd_switch_impl::set_on(pmt::pmt_t cmd_in)
    {
      _switchs = true;
      if(_develop_mode)
        std::cout << "cmd_switch block id " << _block_id << " is ON." << std::endl;
    }

  } /* namespace dmdl */
} /* namespace gr */

