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
#include "path_impl.h"

namespace gr {
  namespace dmdl {

    path::sptr
    path::make(int develop_mode, int block_id, int switch_s, std::string display)
    {
      return gnuradio::get_initial_sptr
        (new path_impl(develop_mode, block_id, switch_s, display));
    }

    /*
     * The private constructor
     */
    path_impl::path_impl(int develop_mode, int block_id, int switch_s, std::string display)
      : gr::block("path",
              gr::io_signature::make(0, 0, 0),
              gr::io_signature::make(0, 0, 0)),
        _develop_mode(develop_mode),
        _block_id(block_id),
        _display(display),
        _switch_s(switch_s)
    {
      if(_develop_mode)
        std::cout << "develop_mode of path ID: " << _block_id << " is activated." << std::endl;
      message_port_register_in(pmt::mp("B"));
      message_port_register_out(pmt::mp("E"));
      set_msg_handler(pmt::mp("B"), boost::bind(&path_impl::control, this, _1 ));
    }

    /*
     * Our virtual destructor.
     */
    path_impl::~path_impl()
    {
    }

    void
    path_impl::control(pmt::pmt_t cmd_in)
    {
      if(_switch_s)
      {
        if(_develop_mode)
          std::cout << " path block ID " << _block_id << " is set to ON node. input cmd is exported." << std::endl;
        message_port_pub(pmt::mp("E"), cmd_in);
      }
      else
      {
        if(_develop_mode)
          std::cout << " path block ID " << _block_id << " is set to OFF node. input cmd is discarded." << std::endl;
      }
      if(_display.size() > 0)
        std::cout << "path ID " << _block_id << " " << _display << std::endl;
    }

  } /* namespace dmdl */
} /* namespace gr */

