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

#ifndef INCLUDED_DMDL_START_PENDING_IMPL_H
#define INCLUDED_DMDL_START_PENDING_IMPL_H

#include <dmdl/start_pending.h>

namespace gr {
  namespace dmdl {

    class start_pending_impl : public start_pending
    {
     private:
      bool _started;
      int _start_next_time_s;
      int _system_time_granularity_us;
      void check_start(pmt::pmt_t msg);

     public:
      start_pending_impl(int start_next_time_s, int system_time_granularity_us);
      ~start_pending_impl();
    };

  } // namespace dmdl
} // namespace gr

#endif /* INCLUDED_DMDL_START_PENDING_IMPL_H */

