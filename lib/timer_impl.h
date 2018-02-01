/* -*- c++ -*- */
/* 
 * Copyright 2018 <+YOU OR YOUR COMPANY+>.
 * 
 * This is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3, or (at your option)
 * any later version.
 * 
 * This software is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this software; see the file COPYING.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street,
 * Boston, MA 02110-1301, USA.
 */

#ifndef INCLUDED_DMDL_TIMER_IMPL_H
#define INCLUDED_DMDL_TIMER_IMPL_H

#include <dmdl/timer.h>

namespace gr {
  namespace dmdl {

    class timer_impl : public timer
    {
     private:
      int _develop_mode;
      int _block_id;
      int _timer_type;
      int _disable_timer;
      bool _in_active;
      double _duration_ms;
      pmt::pmt_t _frame_info;
      double _timer_bias_s;
      double _start_time;
      int _system_time_granularity_us;
      double _reserved_time_ms;
      std::vector<uint32_t> _duration_list_ms;
      std::vector<int> _node_id_list;
      std::vector<int> _address_check_list;
      void start_timer(pmt::pmt_t trigger);
      void suspend_timer(pmt::pmt_t trigger);
      void resume_timer(pmt::pmt_t trigger);
      void reset_duration(pmt::pmt_t cmd_in);
      void disable_timer(pmt::pmt_t trigger);
      void countdown_oneshot_timer();
      void countdown_oneshot_exp_timer();
      void countdown_periodic_timer();
      void countdown_continual_timer();

     public:
      timer_impl(int develop_mode, int block_id, int timer_type, double duration_ms, int system_time_granularity_us, double reserved_time_ms);
      ~timer_impl();
    };

  } // namespace dmdl
} // namespace gr

#endif /* INCLUDED_DMDL_TIMER_IMPL_H */

