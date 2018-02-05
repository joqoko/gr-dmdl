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

#ifndef INCLUDED_DMDL_ERROR_DETECTION_IMPL_H
#define INCLUDED_DMDL_ERROR_DETECTION_IMPL_H

#include <dmdl/error_detection.h>
#include <gnuradio/digital/crc32.h>
#include <boost/crc.hpp>

namespace gr {
  namespace dmdl {

    class error_detection_impl : public error_detection
    {
     private:
      int _develop_mode;
      int _block_id;
      int _frame_type; 
      boost::crc_optimal<32, 0x04C11DB7, 0xFFFFFFFF, 0xFFFFFFFF, true, true> _crc_impl; 
      pmt::pmt_t crc32_bb_calc(pmt::pmt_t msg);
      void check_frame(pmt::pmt_t frame_info);
      void check_frame_v0(pmt::pmt_t rx_frame);
      void disp_vec(std::vector<unsigned char> vec);

     public:
      error_detection_impl(int develop_mode, int block_id);
      ~error_detection_impl();
    };

  } // namespace dmdl
} // namespace gr

#endif /* INCLUDED_DMDL_ERROR_DETECTION_IMPL_H */

