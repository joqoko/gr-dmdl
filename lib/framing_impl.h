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

#ifndef INCLUDED_DMDL_FRAMING_IMPL_H
#define INCLUDED_DMDL_FRAMING_IMPL_H

#include <dmdl/framing.h>
#include <gnuradio/digital/crc32.h>
#include <boost/crc.hpp>

namespace gr {
  namespace dmdl {

    class framing_impl : public framing
    {
     private:
      // Nothing to declare in this block.
      int _develop_mode;
      int _block_id;
      int _frame_type;
      int _len_frame_type; // Bytes
      int _frame_index;
      int _ack_index;
      int _len_frame_index; // Bytes
      int _destination_address;
      int _ack_address;
      int _len_destination_address; // Bytes
      int _source_address; 
      int _len_source_address; // Bytes
      int _reserved_field_I;
      int _len_reserved_field_I; // Bytes
      int _reserved_field_II;
      int _len_reserved_field_II; // Bytes
      int _reserved_field_ampdu; // Bytes
      int _payload_length;
      int _len_payload_length; // Bytes
      int _increase_index;
      int _len_num_transmission;
      int _len_slot_time_beacon;
      int _default_payload;
      int _default_index;
      int _default_payload_length;
      int _internal_index;
      std::vector<unsigned int> _node_list;
      std::vector<unsigned int> _slot_list_ms;
      boost::crc_optimal<32, 0x04C11DB7, 0xFFFFFFFF, 0xFFFFFFFF, true, true> _crc_impl; 
      pmt::pmt_t crc32_bb_calc(pmt::pmt_t msg);
      void catagorization(pmt::pmt_t data_in);
      void reset_frame_index(pmt::pmt_t pmt_in);
      void reset_initial_frame_index(pmt::pmt_t pmt_in);
      pmt::pmt_t data_frame_formation(pmt::pmt_t rx_payload);
      pmt::pmt_t ampdu_subframe_formation(pmt::pmt_t rx_payload);
      pmt::pmt_t amsdu_subframe_formation(pmt::pmt_t rx_payload);
      pmt::pmt_t ack_frame_formation(pmt::pmt_t rx_data);
      pmt::pmt_t beacon_frame_formation(pmt::pmt_t rx_beacon_info);
      pmt::pmt_t frame_header_formation(std::vector<unsigned char> *frame_header, int frame_type, int frame_index, int destination_address, int source_address, int reserved_field_I, int reserved_field_II, int payload_length, int num_transmission);
      pmt::pmt_t ampdu_delimiter_formation(std::vector<unsigned char> *delimiter, int reserved_field_I, int payload_length, int frame_type);
      void intToByte(int i, std::vector<unsigned char> *bytes, int size);
      void disp_vec(std::vector<unsigned char> vec);
      int get_frame_header_length();

     public:
      framing_impl(int develop_mode, int block_id, int frame_type, int len_frame_type, int frame_index, int len_frame_index, int destination_address, int len_destination_address, int source_address, int len_source_address, int reserved_field_I, int len_reserved_field_I, int reserved_field_II, int len_reserved_field_II, int len_payload_length, int increase_index, int len_num_transmission, int reserved_field_ampdu, std::vector<unsigned int> node_list, std::vector<unsigned int> slot_list_ms, int len_slot_time_beacon, int default_payload, int default_payload_length, int internal_index);
      ~framing_impl();

    };

  } // namespace dmdl
} // namespace gr

#endif /* INCLUDED_DMDL_FRAMING_IMPL_H */

