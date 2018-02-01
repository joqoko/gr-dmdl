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


#ifndef INCLUDED_DMDL_TIMER_H
#define INCLUDED_DMDL_TIMER_H

#include <dmdl/api.h>
#include <gnuradio/block.h>

namespace gr {
  namespace dmdl {

    /*!
     * \brief <+description of block+>
     * \ingroup dmdl
     *
     */
    class DMDL_API timer : virtual public gr::block
    {
     public:
      typedef boost::shared_ptr<timer> sptr;

      /*!
       * \brief Return a shared_ptr to a new instance of dmdl::timer.
       *
       * To avoid accidental use of raw pointers, dmdl::timer's
       * constructor is in a private implementation
       * class. dmdl::timer::make is the public interface for
       * creating new instances.
       */
      static sptr make(int develop_mode, int block_id, int timer_type, double duration_ms, int system_time_granularity_us, double reserved_time_ms);
    };

  } // namespace dmdl
} // namespace gr

#endif /* INCLUDED_DMDL_TIMER_H */

