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
#include "start_pending_impl.h"
#include "math.h"

namespace gr {
  namespace dmdl {

    start_pending::sptr
    start_pending::make(int start_next_time_s, int system_time_granularity_us)
    {
      return gnuradio::get_initial_sptr
        (new start_pending_impl(start_next_time_s, system_time_granularity_us));
    }

    /*
     * The private constructor
     */
    start_pending_impl::start_pending_impl(int start_next_time_s, int system_time_granularity_us)
      : gr::block("start_pending",
              gr::io_signature::make(0, 0, 0),
              gr::io_signature::make(0, 0, 0)),
        _start_next_time_s(start_next_time_s),
        _system_time_granularity_us(system_time_granularity_us)
    {
      message_port_register_in(pmt::mp("MSG"));
      message_port_register_out(pmt::mp("B"));
      set_msg_handler(pmt::mp("MSG"), boost::bind(&start_pending_impl::check_start, this, _1 ));
      _started = false;
    }

    /*
     * Our virtual destructor.
     */
    start_pending_impl::~start_pending_impl()
    {
    }

    void
    start_pending_impl::check_start(pmt::pmt_t msg)
    {
      if(!_started)
      {
        _started = true;
        struct timeval t; 
        gettimeofday(&t, NULL);
        double current_time_s = t.tv_sec;
        double start_time_s = t.tv_sec;
       
        while(current_time_s - _start_next_time_s * floor(start_time_s / _start_next_time_s) < _start_next_time_s)
        {
          boost::this_thread::sleep(boost::posix_time::microseconds(_system_time_granularity_us));
          gettimeofday(&t, NULL);
          current_time_s = t.tv_sec + t.tv_usec / 1000000.0;
        }
        gettimeofday(&t, NULL);
        double current_time_show = t.tv_sec - double(int(t.tv_sec/100)*100) + t.tv_usec / 1000000.0;
        std::cout << "node starts running at " << current_time_show << std::endl;
        message_port_pub(pmt::mp("B"), msg);
      }
    }

  } /* namespace dmdl */
} /* namespace gr */

