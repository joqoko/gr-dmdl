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

#ifndef INCLUDED_DMDL_RX_SWITCH_SIMPHY_IMPL_H
#define INCLUDED_DMDL_RX_SWITCH_SIMPHY_IMPL_H

#include <dmdl/rx_switch_simphy.h>

namespace gr {
  namespace dmdl {

    class rx_switch_simphy_impl : public rx_switch_simphy
    {
     private:
     int _develop_mode;
     int _block_id;
     int _is_receiving;
     int _num_fetch_per_cs;
     double _stop_time;
     std::vector<double> _vec_average_pow;
     void kai_guan(pmt::pmt_t spark);

     public:
      rx_switch_simphy_impl(int develop_mode, int block_id, int num_fetch_per_cs);
      ~rx_switch_simphy_impl();

      // Where all the action really happens
      int work(int noutput_items,
         gr_vector_const_void_star &input_items,
         gr_vector_void_star &output_items);
    };

  } // namespace dmdl
} // namespace gr

#endif /* INCLUDED_DMDL_RX_SWITCH_SIMPHY_IMPL_H */

