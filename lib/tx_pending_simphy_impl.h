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

#ifndef INCLUDED_DMDL_TX_PENDING_SIMPHY_IMPL_H
#define INCLUDED_DMDL_TX_PENDING_SIMPHY_IMPL_H

#include <dmdl/tx_pending_simphy.h>
#include <queue>

namespace gr {
  namespace dmdl {

    class tx_pending_simphy_impl : public tx_pending_simphy
    {
     private:
      // Nothing to declare in this block.
      int _develop_mode;
      int _block_id;
      float _sample_rate;
      const pmt::pmt_t _d_lengthtagname;
      float _wait_time;
      int _system_time_granularity_us;
      int _record_on;
      std::string _file_name_extension;
      int _name_with_timestamp;
      std::string _file_name_str;
      double _countdown_bias_s;
      double _interframe_interval_s;
      std::queue<pmt::pmt_t> _tx_queue;         
      int process_tags_info(std::vector <tag_t> tags);
      void buffer_tx_frame_info(pmt::pmt_t tx_frame_info);
      void countdown_waiting();

     public:
      tx_pending_simphy_impl(int develop_mode, int block_id, int system_time_granularity_us, float sample_rate, const std::string &lengthtagname, double interframe_interval_s, int record_on, std::string file_name_extension, int name_with_timestamp);
      ~tx_pending_simphy_impl();

      // Where all the action really happens
      int work(int noutput_items,
         gr_vector_const_void_star &input_items,
         gr_vector_void_star &output_items);
    };

  } // namespace dmdl
} // namespace gr

#endif /* INCLUDED_DMDL_TX_PENDING_SIMPHY_IMPL_H */

