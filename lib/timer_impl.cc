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
#include "timer_impl.h"

namespace gr {
  namespace dmdl {

    timer::sptr
    timer::make(int develop_mode, int block_id, int timer_type, double duration_ms, int system_time_granularity_us, double reserved_time_ms)
    {
      return gnuradio::get_initial_sptr
        (new timer_impl(develop_mode, block_id, timer_type, duration_ms, system_time_granularity_us, reserved_time_ms));
    }

    /*
     * The private constructor
     */
    timer_impl::timer_impl(int develop_mode, int block_id, int timer_type, double duration_ms, int system_time_granularity_us, double reserved_time_ms)
      : gr::block("timer",
              gr::io_signature::make(0, 0, 0),
              gr::io_signature::make(0, 0, 0)),
        _develop_mode(develop_mode),
        _block_id(block_id),
        _timer_type(timer_type),
        _duration_ms(duration_ms),
        _system_time_granularity_us(system_time_granularity_us),
        _reserved_time_ms(reserved_time_ms),
        _timer_bias_s(0),
        _in_active(false)
    {
      if(_develop_mode)
        std::cout << "develop_mode of timer ID: " << _block_id << " is activated." << std::endl;
      message_port_register_in(pmt::mp("B"));
      set_msg_handler(pmt::mp("B"), boost::bind(&timer_impl::start_timer, this, _1 ));
      message_port_register_in(pmt::mp("A"));
      set_msg_handler(pmt::mp("A"), boost::bind(&timer_impl::suspend_timer, this, _1 ));
      message_port_register_in(pmt::mp("RS"));
      set_msg_handler(pmt::mp("RS"), boost::bind(&timer_impl::resume_timer, this, _1 ));
      message_port_register_in(pmt::mp("G"));
      set_msg_handler(pmt::mp("G"), boost::bind(&timer_impl::reset_duration, this, _1 ));
      message_port_register_in(pmt::mp("S"));
      set_msg_handler(pmt::mp("S"), boost::bind(&timer_impl::disable_timer, this, _1 ));
      message_port_register_out(pmt::mp("E"));
      message_port_register_out(pmt::mp("address_check_out"));
      _disable_timer = 0;
      _frame_info = pmt::from_long(0);
    }

    /*
     * Our virtual destructor.
     */
    timer_impl::~timer_impl()
    {
    }

    void timer_impl::reset_duration(pmt::pmt_t cmd_in)
    {
      if(pmt::is_number(cmd_in))
      {
        _duration_ms = pmt::to_double(cmd_in);
        if(_develop_mode)
          std::cout << "duration of timer block ID " << _block_id << " is reset to " << _duration_ms << " [ms] " << std::endl;
      }
      else   
      {
        std::cout << "error: timer block ID " << _block_id << " can only reassign duration to a number (in [ms])." << std::endl;
      }
    }

    void
    timer_impl::start_timer(pmt::pmt_t trigger)
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
            std::cout << "timer " << _block_id << " is triggered at time " << current_time << " s" << std::endl;
          _in_active = true;
          if(_timer_type == 0)
          {
            _frame_info = trigger;
            boost::thread thrd(&timer_impl::countdown_oneshot_timer, this);       
          }
          else if(_timer_type == 1)
          {
            boost::thread thrd(&timer_impl::countdown_periodic_timer, this);       
          }
          else if(_timer_type == 3)
          {
            boost::thread thrd(&timer_impl::countdown_oneshot_exp_timer, this);       
          }
          else if(_timer_type == 2)
          {
            int node_id = pmt::to_long(pmt::dict_ref(trigger, pmt::string_to_symbol("destination_address"), not_found));
            double slot_time = pmt::to_double(pmt::dict_ref(trigger, pmt::string_to_symbol("slot_time"), not_found));
            int address_check = pmt::to_long(pmt::dict_ref(trigger, pmt::string_to_symbol("address_check"), not_found));
            if(node_id == 0 && slot_time == 0 && address_check == 0)
            {
              _in_active = false;
              if(_develop_mode)
                std::cout << "timer ID: " << _block_id << " receives new scheduling information" << std::endl;
              _duration_list_ms.clear();
              _node_id_list.clear();
              _address_check_list.clear();
            }
            else
            {
      
              if(_develop_mode)
              { 
                std::cout << "timer ID: " << _block_id << " new waiting request are stored." << std::endl;
                std::cout << " slot_time is: " << slot_time << " and node ID is: " << node_id << std::endl;
              }
              _duration_list_ms.push_back(slot_time);
              _node_id_list.push_back(node_id);
              _address_check_list.push_back(address_check);
              boost::thread thrd(&timer_impl::countdown_continual_timer, this);
            }
          }
        }
        else
        {
          if(_timer_type == 0)
          {
            struct timeval t;
            gettimeofday(&t, NULL);
            _start_time = t.tv_sec + t.tv_usec / 1000000.0;
            if(_develop_mode)
              std::cout << "warning: timer ID: " << _block_id << " is triggered before the last one finish." << std::endl;
          }
          if(_timer_type == 3)
          {
            if(_develop_mode)
              std::cout << "timer ID: " << _block_id << " cannot be triggered before the last one finish." << std::endl;
          }
          else if(_timer_type == 1)
          {
            if(_develop_mode)
              std::cout << "timer id: " << _block_id << " is set to periodic mode so it can only be triggered once." << std::endl;
          }
          else if(_timer_type == 2)
          { 
      
            if(pmt::dict_has_key(trigger, pmt::string_to_symbol("slot_time")))
            {
              double slot_time = pmt::to_double(pmt::dict_ref(trigger, pmt::string_to_symbol("slot_time"), not_found));
              int node_id = pmt::to_double(pmt::dict_ref(trigger, pmt::string_to_symbol("destination_address"), not_found));
              int address_check = pmt::to_double(pmt::dict_ref(trigger, pmt::string_to_symbol("address_check"), not_found));
              if(node_id == 0 && slot_time == 0 && address_check == 0)
              {
                if(_develop_mode)
                  std::cout << "timer ID: " << _block_id << " terminates all scheduled timer because new scheduling information is received" << std::endl;
                _in_active = false;
                _duration_list_ms.clear();
                _node_id_list.clear();
                _address_check_list.clear();
              }
              else
              {
                double slot_time = pmt::to_double(pmt::dict_ref(trigger, pmt::string_to_symbol("slot_time"), not_found));
                double node_id = pmt::to_double(pmt::dict_ref(trigger, pmt::string_to_symbol("destination_address"), not_found));
                int address_check = pmt::to_double(pmt::dict_ref(trigger, pmt::string_to_symbol("address_check"), not_found));
                if(_develop_mode)
                {
                  std::cout << "timer ID: " << _block_id << " new waiting request are stored." << std::endl;
                  std::cout << " slot_time is: " << slot_time << " and node ID is: " << node_id << std::endl;
                }
                _duration_list_ms.push_back(slot_time);
                _node_id_list.push_back(node_id);
                _address_check_list.push_back(address_check);
              }
            }
            else
            {
              std::cout << "timer ID: " << _block_id << ": input pmt has no slot_time field. please check your connections." << std::endl;
            }
          }
        } 
      }
      else
      {
        if(_develop_mode)
          std::cout << "timer ID: " << _block_id << " is disabled." << std::endl;
      }
    }
 
    void
    timer_impl::countdown_continual_timer()
    {
      while(_duration_list_ms.size() > 0 && _in_active)
      {
        pmt::pmt_t address_check = pmt::make_dict();
        address_check = pmt::dict_add(address_check, pmt::string_to_symbol("beacon_address_check"), pmt::from_long(_address_check_list.front()));
        message_port_pub(pmt::mp("address_check_out"), address_check);
        struct timeval t;
        gettimeofday(&t, NULL);
        double current_time = t.tv_sec + t.tv_usec / 1000000.0;
        double start_time = current_time;
        double start_time_show = t.tv_sec - double(int(t.tv_sec/100)*100) + t.tv_usec / 1000000.0;
        double current_time_show = start_time_show;
        double duration_ms = std::max(double(_duration_list_ms.front()) - _reserved_time_ms, double(0));
        if(_develop_mode)
        {
          std::cout << "timer start time: " << start_time_show;
          if(_address_check_list.front())
            std::cout << ", it is my time slot " << std::endl;
          else
            std::cout << ", it is not my time slot " << std::endl;
        }
        while((current_time < start_time + double(duration_ms) / 1000 - _timer_bias_s) && _in_active)
        {
          boost::this_thread::sleep(boost::posix_time::microseconds(_system_time_granularity_us));
          gettimeofday(&t, NULL);
          current_time = t.tv_sec + t.tv_usec / 1000000.0;
          current_time_show = t.tv_sec - double(int(t.tv_sec/100)*100) + t.tv_usec / 1000000.0;
          // std::cout << "timeout is running at: " << current_time_show << std::endl;
        }
        std::cout << " _in_active after waiting is: " << _in_active << std::endl;
        gettimeofday(&t, NULL);
        current_time = t.tv_sec + t.tv_usec / 1000000.0;
        if(_develop_mode)
        {
          current_time_show = t.tv_sec - double(int(t.tv_sec/100)*100) + t.tv_usec / 1000000.0;
          if(_in_active)
          {
            _timer_bias_s = _timer_bias_s + current_time - start_time - duration_ms/1000;
            std::cout << "timer ID: " << _block_id << " is expired at time " << current_time_show << " s. " << " actual duration is: " << current_time - start_time << " [s]" << "and the time bias is: " << _timer_bias_s << std::endl;
          }
          else
            std::cout << "timer ID: " << _block_id << " is killed at time " << current_time_show << " s. " << " actual duration is: " << current_time - start_time << " [s]" << std::endl;
        }
        pmt::pmt_t expire = pmt::make_dict();
        expire = pmt::dict_add(expire, pmt::string_to_symbol("time_stamp"), pmt::from_double(current_time));
        message_port_pub(pmt::mp("E"), expire);
        if(_in_active)
        {
          _duration_list_ms.erase(_duration_list_ms.begin());
          _node_id_list.erase(_node_id_list.begin());
          _address_check_list.erase(_address_check_list.begin());
        }
      }
      if(!_in_active)
      {
        _duration_list_ms.clear();
        _node_id_list.clear();
        _address_check_list.clear();
      }
      _in_active = false;
      pmt::pmt_t address_check = pmt::make_dict();
      address_check = pmt::dict_add(address_check, pmt::string_to_symbol("beacon_address_check"), pmt::from_long(0));
      message_port_pub(pmt::mp("address_check_out"), address_check);
    }

    void
    timer_impl::suspend_timer(pmt::pmt_t trigger)
    {
      if(_in_active)
      {
        _in_active = false;
        std::cout << "timer ID: " << _block_id << " is suspended. " << std::endl;
      }
      else
      {
        if(_develop_mode)
          std::cout << "warning: timer ID: " << _block_id << " is suspended in inactive node. " << std::endl;
       
      }
    }

    void
    timer_impl::resume_timer(pmt::pmt_t trigger)
    {
      if(!_in_active)
      {
        _in_active = true;
      }
      else
      {
        if(_develop_mode)
          std::cout << "timer ID: " << _block_id << " is already in active mode. " << std::endl;
       
      }
    }

    void
    timer_impl::disable_timer(pmt::pmt_t trigger)
    {
      _disable_timer = 1;
      if(_develop_mode)
        std::cout << "timer ID: " << _block_id << " is disabled" << std::endl;
    }
 
 
    void
    timer_impl::countdown_oneshot_exp_timer()
    {
      struct timeval t;
      gettimeofday(&t, NULL);
      double current_time = t.tv_sec + t.tv_usec / 1000000.0;
      _start_time = current_time;
      double start_time_show = t.tv_sec - double(int(t.tv_sec/100)*100) + t.tv_usec / 1000000.0;
      double current_time_show = start_time_show;
      if(_develop_mode)
        std::cout << "timer start time: " << start_time_show << std::endl;
      double exp =  (double)rand()/(double)(RAND_MAX);
      double next_time = - log(exp) * _duration_ms;
      while((current_time < _start_time + double(next_time) / 1000) && _in_active)
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
          _timer_bias_s = _timer_bias_s + current_time - _start_time - double(_duration_ms)/1000;
          std::cout << "timer ID: " << _block_id << " is expired at time " << current_time_show << " s. " << " actual duration is: " << current_time - _start_time << " [s]" << ". the time bias is: " << _timer_bias_s << std::endl;
        }
        else
          std::cout << "* timer ID: " << _block_id << " is killed at time " << current_time_show << " s. " << " actual duration is: " << current_time_show - start_time_show << " s" << std::endl;
      }
      if(pmt::is_dict(_frame_info))
      {
        _frame_info = pmt::dict_add(_frame_info, pmt::string_to_symbol("time_stamp"), pmt::from_double(current_time));
        _in_active = false;
        message_port_pub(pmt::mp("E"), _frame_info);
      }
      else
      {
        pmt::pmt_t expire = pmt::make_dict();
        expire = pmt::dict_add(expire, pmt::string_to_symbol("time_stamp"), pmt::from_double(current_time));
        _in_active = false;
        message_port_pub(pmt::mp("E"), _frame_info);
      }
    }
 
    void
    timer_impl::countdown_oneshot_timer()
    {
      struct timeval t;
      gettimeofday(&t, NULL);
      double current_time = t.tv_sec + t.tv_usec / 1000000.0;
      _start_time = current_time;
      double local_start_time = current_time;
      double start_time_show = t.tv_sec - double(int(t.tv_sec/100)*100) + t.tv_usec / 1000000.0;
      double current_time_show = start_time_show;
      if(_develop_mode)
        std::cout << "timer start time: " << start_time_show << std::endl;
      while((current_time < _start_time + double(_duration_ms) / 1000) && _in_active)
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
          _timer_bias_s = _timer_bias_s + current_time - _start_time - double(_duration_ms)/1000;
          std::cout << "timer ID: " << _block_id << " is expired at time " << current_time_show << " s. " << " actual duration is: " << current_time - local_start_time << " [s]" << ". the time bias is: " << _timer_bias_s << std::endl;
        }
        else
          std::cout << "timer ID: " << _block_id << " is suspended at time " << current_time_show << " s. " << " actual duration is: " << current_time_show - start_time_show << " s" << std::endl;
      }
      if(_in_active)
      {
        if(pmt::is_dict(_frame_info))
        {
          if(_develop_mode)
          {
            std::cout << "* timer ID: " << _block_id << " output input command" << std::endl;
          }
          _frame_info = pmt::dict_add(_frame_info, pmt::string_to_symbol("time_stamp"), pmt::from_double(current_time));
          _in_active = false;
          message_port_pub(pmt::mp("E"), _frame_info);
        }
        else
        {
          if(_develop_mode)
          {
            std::cout << "* timer ID: " << _block_id << " output new command" << std::endl;
          }
          pmt::pmt_t expire = pmt::make_dict();
          expire = pmt::dict_add(expire, pmt::string_to_symbol("time_stamp"), pmt::from_double(current_time));
          _in_active = false;
          message_port_pub(pmt::mp("E"), _frame_info);
        }
      }
      else
        if(_develop_mode)
          std::cout << "timer ID: " << _block_id << " is suspended so no output." << std::endl;
    }
 
    void
    timer_impl::countdown_periodic_timer()
    {
      while(_in_active)
      {
        struct timeval t;
        gettimeofday(&t, NULL);
        double current_time = t.tv_sec + t.tv_usec / 1000000.0;
        double start_time = current_time;
        double start_time_show = t.tv_sec - double(int(t.tv_sec/100)*100) + t.tv_usec / 1000000.0;
        double current_time_show = start_time_show;
        while((current_time < start_time + double(_duration_ms) / 1000 - _timer_bias_s) && _in_active)
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
            _timer_bias_s = _timer_bias_s + current_time - start_time - double(_duration_ms)/1000;
            std::cout << "* timer ID: " << _block_id << " is expired at time " << current_time_show << " s. " << " actual duration is: " << current_time - start_time << " [ms]" << "and the time bias is: " << _timer_bias_s << std::endl;
          }
          else
          {
            std::cout << "* timer ID: " << _block_id << " is killed at time " << current_time_show << " s. " << " actual duration is: " << current_time_show - start_time_show << " s" << std::endl;
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

