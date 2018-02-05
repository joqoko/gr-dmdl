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

#ifndef INCLUDED_DMDL_BACKOFF_H
#define INCLUDED_DMDL_BACKOFF_H

#include <dmdl/api.h>
#include <gnuradio/block.h>

namespace gr {
  namespace dmdl {

    /*!
     * \brief <+description of block+>
     * \ingroup dmdl
     *
     */
    class DMDL_API backoff : virtual public gr::block
    {
     public:
      typedef boost::shared_ptr<backoff> sptr;

      /*!
       * \brief Return a shared_ptr to a new instance of dmdl::backoff.
       *
       * To avoid accidental use of raw pointers, dmdl::backoff's
       * constructor is in a private implementation
       * class. dmdl::backoff::make is the public interface for
       * creating new instances.
       */
      static sptr make(int develop_mode, int block_id, int backoff_type, int backoff_time_unit_ms, int min_backoff_ms, int max_backoff_ms, int apply_cs, double cs_threshold, int system_time_granularity_us, int virtual_cs, int min_window_size);
    };

  } // namespace dmdl
} // namespace gr

#endif /* INCLUDED_DMDL_BACKOFF_H */

