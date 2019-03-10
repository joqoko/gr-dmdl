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

#ifndef INCLUDED_DMDL_DIVIDER_IMPL_H
#define INCLUDED_DMDL_DIVIDER_IMPL_H

#include <dmdl/divider.h>

namespace gr {
  namespace dmdl {

    class divider_impl : public divider
    {
     private:
      int _develop_mode;
      int _block_id;
      int _current_count;
      int _counts;
      int _mode;
      void counting(pmt::pmt_t pmt_in);
      void reset(pmt::pmt_t pmt_in);
      void set_counts(pmt::pmt_t pmt_in);

     public:
      divider_impl(int develop_mode, int block_id, int counts, int mode);
      ~divider_impl();

    };

  } // namespace dmdl
} // namespace gr

#endif /* INCLUDED_DMDL_DIVIDER_IMPL_H */

