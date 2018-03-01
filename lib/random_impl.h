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

#ifndef INCLUDED_DMDL_RANDOM_IMPL_H
#define INCLUDED_DMDL_RANDOM_IMPL_H

#include <dmdl/random.h>

namespace gr {
  namespace dmdl {

    class random_impl : public random
    {
     private:
      int _develop_mode;
      int _block_id;
      double _threshold;
      void rolling(pmt::pmt_t pmt_in);
      void set_threshold(pmt::pmt_t cmd_in);

     public:
      random_impl(int develop_mode, int block_id, double threshold);
      ~random_impl();
    };

  } // namespace dmdl
} // namespace gr

#endif /* INCLUDED_DMDL_RANDOM_IMPL_H */

