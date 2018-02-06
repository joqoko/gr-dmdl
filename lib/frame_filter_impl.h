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

#ifndef INCLUDED_DMDL_FRAME_FILTER_IMPL_H
#define INCLUDED_DMDL_FRAME_FILTER_IMPL_H

#include <dmdl/frame_filter.h>

namespace gr {
  namespace dmdl {

    class frame_filter_impl : public frame_filter
    {
     private:
      int _develop_mode;
      int _block_id;
      int _drop_type;
      int _source_address;
      int _destination_address;
      int _frame_index;
      int _frame_type;
      int _reserved_field_I;
      int _reserved_field_II; 
      int _number_of_filtering;
      void filtering(pmt::pmt_t frame_in);

     public:
      frame_filter_impl(int develop_mode, int block_id, int drop_type, int frame_type, int source_address, int destination_address, int frame_index, int reserved_field_I, int reserved_field_II, int number_of_filtering);
      ~frame_filter_impl();

};

  } // namespace dmdl
} // namespace gr

#endif /* INCLUDED_DMDL_FRAME_FILTER_IMPL_H */

