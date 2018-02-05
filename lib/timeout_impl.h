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

#ifndef INCLUDED_DMDL_TIMEOUT_IMPL_H
#define INCLUDED_DMDL_TIMEOUT_IMPL_H

#include <dmdl/timeout.h>
#include <queue>

namespace gr {
  namespace dmdl {

    class timeout_impl : public timeout
   {
      private:
      int _develop_mode;
      int _block_id;
      int _in_timeout;
      double _start_time;
      float _timeout_duration_ms;
      int _system_time_granularity_us;
      int _llc_protocol;
      std::queue<pmt::pmt_t> _window;
      pmt::pmt_t _waiting_frame_info;
      void start_timeout(pmt::pmt_t tx_frame_info);
      void kill_timeout(pmt::pmt_t ack_info);
      void reset_timeout();
      void flush_window();
      void countdown_timeout();
      void print_time();

     public:
      timeout_impl(int develop_mode, int block_id, float timeout_duration_ms, int system_time_granularity_us, int llc_protocol);
      ~timeout_impl();

    };

  } // namespace dmdl
} // namespace gr

#endif /* INCLUDED_DMDL_TIMEOUT_IMPL_H */

