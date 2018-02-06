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

#ifndef INCLUDED_DMDL_REPLICATE_IMPL_H
#define INCLUDED_DMDL_REPLICATE_IMPL_H

#include <dmdl/replicate.h>

namespace gr {
  namespace dmdl {

    class replicate_impl : public replicate
    {
     private:
      int _develop_mode;
      int _block_id;
      int _replicate_number;
      void do_replicate(pmt::pmt_t frame_in);
      void reset_number(pmt::pmt_t number_in);

     public:
      replicate_impl(int develop_mode, int block_id, int replicate_number);
      ~replicate_impl();


    };

  } // namespace dmdl
} // namespace gr

#endif /* INCLUDED_DMDL_REPLICATE_IMPL_H */

