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

#ifndef INCLUDED_DMDL_DUMMY_SOURCE_IMPL_H
#define INCLUDED_DMDL_DUMMY_SOURCE_IMPL_H

#include <dmdl/dummy_source.h>

namespace gr {
  namespace dmdl {

    class dummy_source_impl : public dummy_source
    {
     private:
      int _develop_mode;
      int _block_id;
      int _payload_length;
      int _source_type;
      bool _generating;
      double _packet_rate;
      std::vector<unsigned char> _payload;
      int _start;
      void trigger(pmt::pmt_t trig);
      void stop_generation(pmt::pmt_t trig);
      void constant_source();

     public:
      dummy_source_impl(int develop_mode, int block_id, int payload_length, int source_type, double packet_rate);
      ~dummy_source_impl();
};

  } // namespace dmdl
} // namespace gr

#endif /* INCLUDED_DMDL_DUMMY_SOURCE_IMPL_H */

