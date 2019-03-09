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
#include "absolute_timer_impl.h"

namespace gr {
  namespace dmdl {

    absolute_timer::sptr
    absolute_timer::make(int develop_mode, int block_id, double duration_ms, int system_time_granularity_us)
    {
      return gnuradio::get_initial_sptr
        (new absolute_timer_impl(develop_mode, block_id, duration_ms, system_time_granularity_us));
    }

    /*
     * The private constructor
     */
    absolute_timer_impl::absolute_timer_impl(int develop_mode, int block_id, double duration_ms, int system_time_granularity_us)
      : gr::block("absolute_timer",
              gr::io_signature::make(0, 0, 0),
              gr::io_signature::make(0, 0, 0)),
        _develop_mode(develop_mode),
        _block_id(block_id),
        _duration_ms(duration_ms),
        _in_active(false),
        _system_time_granularity_us(system_time_granularity_us)
    {
      if(_develop_mode)
        std::cout << "develop_mode of absolute_timer ID: " << _block_id << " is activated." << std::endl;
      message_port_register_in(pmt::mp("B"));
      set_msg_handler(pmt::mp("B"), boost::bind(&absolute_timer_impl::start_timer, this, _1 ));
      message_port_register_in(pmt::mp("A"));
      set_msg_handler(pmt::mp("A"), boost::bind(&absolute_timer_impl::suspend_timer, this, _1 ));
      message_port_register_in(pmt::mp("RS"));
      set_msg_handler(pmt::mp("RS"), boost::bind(&absolute_timer_impl::resume_timer, this, _1 ));
      message_port_register_in(pmt::mp("G"));
      set_msg_handler(pmt::mp("G"), boost::bind(&absolute_timer_impl::reset_duration, this, _1 ));
      message_port_register_in(pmt::mp("S"));
      set_msg_handler(pmt::mp("S"), boost::bind(&absolute_timer_impl::disable_timer, this, _1 ));
      message_port_register_out(pmt::mp("E"));
      _disable_timer = 0;
      _frame_info = pmt::from_long(0);
    }


    /*
     * Our virtual destructor.
     */
    absolute_timer_impl::~absolute_timer_impl()
    {
    }

    void absolute_timer_impl::reset_duration(pmt::pmt_t cmd_in)
    {
      if(pmt::is_number(cmd_in))
      {
        _duration_ms = pmt::to_double(cmd_in);
        if(_develop_mode)
          std::cout << "duration of absolute_timer block ID " << _block_id << " is reset to " << _duration_ms << " [ms] " << std::endl;
      }
      else   
      {
        std::cout << "error: absolute_timer block ID " << _block_id << " can only reassign duration to a number (in [ms])." << std::endl;
      }
    }

    void
    absolute_timer_impl::start_timer(pmt::pmt_t trigger)
    {
      if(!_disable_timer)
      {
        if(pmt::is_dict(trigger))
          _frame_info = trigger;
        pmt::pmt_t not_found;
        struct timeval t; 
        gettimeofday(&t, NULL);
        double current_time = t.tv_sec - double(int(t.tv_sec/100)*100) + t.tv_usec / 1000000.0;
        if(!_in_active)
        {
          if(_develop_mode)
            std::cout << "absolute_timer " << _block_id << " is triggered at time " << current_time << " s" << std::endl;
          _in_active = true;
          boost::thread thrd(&absolute_timer_impl::countdown_absolute_timer, this);       
        }
        else
        {
          if(_develop_mode)
            std::cout << "absolute_timer id: " << _block_id << " is set to periodic mode so it can only be triggered once." << std::endl;
        } 
      }
      else
      {
        if(_develop_mode)
          std::cout << "absolute_timer ID: " << _block_id << " is disabled." << std::endl;
      }
    }

    void
    absolute_timer_impl::suspend_timer(pmt::pmt_t trigger)
    {
      if(_in_active)
      {
        _in_active = false;
        std::cout << "absolute_timer ID: " << _block_id << " is suspended. " << std::endl;
      }
      else
      {
        if(_develop_mode)
          std::cout << "warning: absolute_timer ID: " << _block_id << " is suspended in inactive node. " << std::endl;
       
      }
    }

    void
    absolute_timer_impl::resume_timer(pmt::pmt_t trigger)
    {
      if(!_in_active)
      {
        _in_active = true;
      }
      else
      {
        if(_develop_mode)
          std::cout << "absolute_timer ID: " << _block_id << " is already in active mode. " << std::endl;
       
      }
    }

    void
    absolute_timer_impl::disable_timer(pmt::pmt_t trigger)
    {
      _disable_timer = 1;
      if(_develop_mode)
        std::cout << "absolute_timer ID: " << _block_id << " is disabled" << std::endl;
    }
 
    void
    absolute_timer_impl::countdown_absolute_timer()
    {
      while(_in_active && (_disable_timer == 0))
      {
        struct timeval t;
        gettimeofday(&t, NULL);
        double current_time = t.tv_sec + t.tv_usec / 1000000.0;
        double start_time = current_time;
        double start_time_show = t.tv_sec - double(int(t.tv_sec/100)*100) + t.tv_usec / 1000000.0;
        double current_time_show = start_time_show;
        double next_ct = ceil((start_time + double(_duration_ms) / 1000 / 2) / (double(_duration_ms)/1000)) * (double(_duration_ms)/1000);
        double gap = next_ct - start_time;
        while((current_time < next_ct) && _in_active && (_disable_timer == 0))
        {
          boost::this_thread::sleep(boost::posix_time::microseconds(_system_time_granularity_us));
          gettimeofday(&t, NULL);
          current_time = t.tv_sec + t.tv_usec / 1000000.0;
          current_time_show = t.tv_sec - double(int(t.tv_sec/100)*100) + t.tv_usec / 1000000.0;
          // std::cout << "timeout is running at: " << current_time_show << std::endl;
        }
        gettimeofday(&t, NULL);
        current_time = t.tv_sec + t.tv_usec / 1000000.0;
        if(_develop_mode)
        {
          current_time_show = t.tv_sec - double(int(t.tv_sec/100)*100) + t.tv_usec / 1000000.0;
          if(_in_active)
          {
            std::cout << "* general timer ID: " << _block_id << " is expired at time " << current_time_show << " s. " << " actual duration is: " << current_time - start_time << " [ms]" << std::endl;
          }
          else
          {
            std::cout << "* general timer ID: " << _block_id << " is killed at time " << current_time_show << " s. " << " actual duration is: " << current_time_show - start_time_show << " s" << std::endl;
          }
        }
        if(pmt::is_dict(_frame_info))
        {
          _frame_info = pmt::dict_add(_frame_info, pmt::string_to_symbol("time_stamp"), pmt::from_double(current_time));
          message_port_pub(pmt::mp("E"), _frame_info);
        }
        else
        {
          pmt::pmt_t expire = pmt::make_dict();
          expire = pmt::dict_add(expire, pmt::string_to_symbol("time_stamp"), pmt::from_double(current_time));
          message_port_pub(pmt::mp("E"), expire);
        }
      }
    }

  } /* namespace dmdl */
} /* namespace gr */

