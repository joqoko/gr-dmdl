/* -*- c++ -*- */
/*
 * Copyright 2008,2010,2012 Free Software Foundation, Inc.
 *
 * This file is part of the GR DMDL, an GNU Radio based implementation
 * of Decomposite MAC Descrption Language which is a tool for
 * MAC protocol design and prototyping
 *
 * GR DMDL is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3, or (at your option)
 * any later version.
 *
 * GR DMDL is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with GNU Radio; see the file COPYING.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street,
 * Boston, MA 02110-1301, USA.
 */

#ifndef INCLUDED_DMDL_BACKOFF_IMPL_H
#define INCLUDED_DMDL_BACKOFF_IMPL_H

#include <dmdl/backoff.h>

namespace gr {
  namespace dmdl {

    class backoff_impl : public backoff
    {
     private:
       int _backoff_time_unit_ms;
       int _develop_mode;
       int _block_id;
       int _min_backoff_ms;
       int _max_backoff_ms;
       int _backoff_type;
       pmt::pmt_t _cmd;
       int _n_backoff;
       int _min_window_size;
       int _apply_cs;
       bool _ch_busy;
       double _start_busy;
       double _end_busy;
       double _cs_threshold;
       int _virtual_cs;
       double _timer_bias_s;
       int _nav_us;
       double _power;
       bool _in_backoff;
       int _system_time_granularity_us;
       void start_backoff(pmt::pmt_t cmd);
       void countdown_exp_backoff_cs();
       void countdown_const_backoff();
       void countdown_random_backoff();
       void start_virtual_cs(pmt::pmt_t power_in);
       void carrier_sensing(pmt::pmt_t rcts);
       void set_cs_threshold(pmt::pmt_t cs_threshold_in);

     public:
      backoff_impl(int develop_mode, int block_id, int backoff_type, int backoff_time_unit_ms, int min_backoff_ms, int max_backoff_ms, int apply_cs, double cs_threshold, int system_time_granularity_us, int virtual_cs, int min_window_size);
      ~backoff_impl();

    };

  } // namespace dmdl
} // namespace gr

#endif /* INCLUDED_DMDL_BACKOFF_IMPL_H */

