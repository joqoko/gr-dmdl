/* -*- c++ -*- */
/* 
 * Copyright 2016 <+YOU OR YOUR COMPANY+>.
 * 
 * This is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3, or (at your option)
 * any later version.
 * 
 * This software is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this software; see the file COPYING.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street,
 * Boston, MA 02110-1301, USA.
 */

#ifndef INCLUDED_DMDL_TX_CONTROL_SIMPHY_IMPL_H
#define INCLUDED_DMDL_TX_CONTROL_SIMPHY_IMPL_H

#include <dmdl/tx_control_simphy.h>

namespace gr {
  namespace dmdl {

    class tx_control_simphy_impl : public tx_control_simphy
    {
     private:
      // Nothing to declare in this block.
      int _develop_mode;
      int _block_id;
      int _record_on;
      std::string _file_name_str;
      std::string _file_name_extension;
      int _name_with_timestamp;
      double _last_tx_time;
      double _bps;
      double _t_pretx_interval_s;
      tag_t _packet_len_tag;
      int process_tags_info(std::vector <tag_t> tags);


     public:
      tx_control_simphy_impl(int develop_mode, int block_id, double bps, double t_pretx_interval_s, int record_on, std::string fime_name_extension, int name_with_timestamp);
      ~tx_control_simphy_impl();

      // Where all the action really happens
      void forecast (int noutput_items, gr_vector_int &ninput_items_required);

      int general_work(int noutput_items,
           gr_vector_int &ninput_items,
           gr_vector_const_void_star &input_items,
           gr_vector_void_star &output_items);
    };

  } // namespace dmdl
} // namespace gr

#endif /* INCLUDED_DMDL_TX_CONTROL_SIMPHY_IMPL_H */

