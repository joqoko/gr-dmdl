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

#ifndef INCLUDED_DMDL_COUNTER_IMPL_H
#define INCLUDED_DMDL_COUNTER_IMPL_H

#include <dmdl/counter.h>

namespace gr {
  namespace dmdl {

    class counter_impl : public counter
    {
     private:
      int _develop_mode;
      int _counter_id;
      int _counter;
      int _in_counting;
      int _virgin;
      int _record_on;
      int _name_with_timestamp;
      double _start_time; 
      std::string _counter_name;
      std::string _file_name_str;
      std::string _file_name_extension;
      void counting(pmt::pmt_t message);
      void reset(pmt::pmt_t cmd_in);
      void start_counting(pmt::pmt_t message);
      void stop_counting(pmt::pmt_t message);

     public:
      counter_impl(int develop_mode, int counter_id, int interval_mode, std::string counter_name, int record_on, std::string record_path, int name_with_timestamp);
      ~counter_impl();

    };

  } // namespace dmdl
} // namespace gr

#endif /* INCLUDED_DMDL_COUNTER_IMPL_H */

