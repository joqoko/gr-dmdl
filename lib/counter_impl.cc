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
#include "counter_impl.h"
#include <iostream>
#include <fstream>
#include <string>
#include <ctime>
#include <queue>

namespace gr {
  namespace dmdl {

    counter::sptr
    counter::make(int develop_mode, int counter_id, int interval_mode, std::string counter_name, int record_on, std::string record_path, int name_with_timestamp)
    {
      return gnuradio::get_initial_sptr
        (new counter_impl(develop_mode, counter_id, interval_mode, counter_name, record_on, record_path, name_with_timestamp));
    }

    /*
     * The private constructor
     */
    counter_impl::counter_impl(int develop_mode, int counter_id, int interval_mode, std::string counter_name, int record_on, std::string record_path, int name_with_timestamp)
      : gr::block("counter",
              gr::io_signature::make(0, 0, 0),
              gr::io_signature::make(0, 0, 0)),
        _develop_mode(develop_mode),
        _counter_id(counter_id),
        _counter_name(counter_name),
        _counter(0),
        _record_on(record_on),
        _name_with_timestamp(name_with_timestamp)
    {
      if(_develop_mode)
        std::cout << "the " << _counter_id << "th message counter is activated." << std::endl;
      message_port_register_in(pmt::mp("B"));
      set_msg_handler(pmt::mp("B"), boost::bind(&counter_impl::counting, this, _1 ));
      message_port_register_in(pmt::mp("RT"));
      set_msg_handler(pmt::mp("RT"), boost::bind(&counter_impl::reset, this, _1 ));
      message_port_register_in(pmt::mp("SC"));
      set_msg_handler(pmt::mp("SC"), boost::bind(&counter_impl::start_counting, this, _1 ));
      message_port_register_in(pmt::mp("S"));
      set_msg_handler(pmt::mp("S"), boost::bind(&counter_impl::stop_counting, this, _1 ));
      if(interval_mode)
        _in_counting = 1;
      else
        _in_counting = 1;
        struct timeval t; 
        gettimeofday(&t, NULL);
        _start_time = t.tv_sec + t.tv_usec / 1000000.0;

      if(_record_on)
      {
        time_t tt = time(0);   // get time now
        struct tm * now = localtime( & tt );
        std::ostringstream file_name;
        if(_name_with_timestamp)
          file_name << "/home/inets/source/gr-inets/results/" << (now->tm_year + 1900) << "_" << (now->tm_mon + 1) << "_" << now->tm_mday << "_" << now->tm_hour << "_" << now->tm_min << "_" << now->tm_sec << "_counter" << counter_id << "_" << _counter_name << ".txt";
        else
          file_name << "/home/inets/source/gr-inets/results/" << _counter_name << ".txt";
        _file_name_str = file_name.str();
      }
    }

    /*
     * Our virtual destructor.
     */
    counter_impl::~counter_impl()
    {
    }
 
    void counter_impl::reset(pmt::pmt_t cmd_in)
    {
      struct timeval t; 
      gettimeofday(&t, NULL);
      double current_time = t.tv_sec - double(int(t.tv_sec/10000)*10000) + t.tv_usec / 1000000.0;
      if(_develop_mode == 3)
        std::cout << "counter results at time " << current_time << " is " << _counter << std::endl;
      std::ofstream ofs (_file_name_str.c_str(), std::ofstream::app);
      ofs << _counter << " " << std::endl;
      ofs.close();
      _counter = 0;
    }

    void counter_impl::stop_counting(pmt::pmt_t message)
    {
      _in_counting = 0;
      struct timeval t; 
      gettimeofday(&t, NULL);
      double current_time = t.tv_sec + t.tv_usec / 1000000.0;
      double current_time_show = t.tv_sec - double(int(t.tv_sec/10000)*10000) + t.tv_usec / 1000000.0;
      double interval = current_time - _start_time;
      if(_develop_mode == 1)
        std::cout << "counter " << _counter_name << " ID " << _counter_id << " stop counting at time " << current_time_show << "s. in " << interval << "s, counter is visited " << _counter << " times" << std::endl;
    }

    void counter_impl::start_counting(pmt::pmt_t message)
    {
      if(_virgin == 0)
      {
        _in_counting = 1;
        _virgin = 1;
        struct timeval t; 
        gettimeofday(&t, NULL);
        _start_time = t.tv_sec + t.tv_usec / 1000000.0;
        double start_time_show = t.tv_sec - double(int(t.tv_sec/10000)*10000) + t.tv_usec / 1000000.0;
        if(_develop_mode == 1)
          std::cout << "counter " << _counter_name << " ID " << _counter_id << " start counting at time " << start_time_show << "s" << std::endl;
      }
    }

    void counter_impl::counting(pmt::pmt_t message)
    {
      if(_in_counting)
      {
        _counter++;
        if(_develop_mode == 1)
          // add the function that user can add name for counter
          std::cout << "the " << _counter_id << "th message counter " << _counter_name << " has been visited " << _counter << " times "; 
        if(_develop_mode == 2)
        {
          struct timeval t; 
          gettimeofday(&t, NULL);
          double current_time = t.tv_sec - double(int(t.tv_sec/10000)*10000) + t.tv_usec / 1000000.0;
          std::cout << " at " << current_time << " s" << std::endl;
        }
        else if(_develop_mode != 2 && _develop_mode == 1)
          std::cout << " " << std::endl;
      }
    }

  } /* namespace dmdl */
} /* namespace gr */

