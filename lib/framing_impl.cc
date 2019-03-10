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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <gnuradio/io_signature.h>
#include "framing_impl.h"
#include <gnuradio/digital/crc32.h> 
#include <volk/volk.h>
#include <boost/crc.hpp>

namespace gr {
  namespace dmdl {

    framing::sptr
    framing::make(int develop_mode, int block_id, int frame_type, int len_frame_type, int frame_index, int len_frame_index, int destination_address, int len_destination_address, int source_address, int len_source_address, int reserved_field_I, int len_reserved_field_I, int reserved_field_II, int len_reserved_field_II, int len_payload_length, int increase_index, int len_num_transmission, int reserved_field_ampdu, std::vector<unsigned int> node_list, std::vector<unsigned int> slot_list_ms, int len_slot_time_beacon, int default_payload, int default_payload_length, int internal_index)
    {
      return gnuradio::get_initial_sptr
        (new framing_impl(develop_mode, block_id, frame_type, len_frame_type, frame_index, len_frame_index, destination_address, len_destination_address, source_address, len_source_address, reserved_field_I, len_reserved_field_I, reserved_field_II, len_reserved_field_II, len_payload_length, increase_index, len_num_transmission, reserved_field_ampdu, node_list, slot_list_ms, len_slot_time_beacon, default_payload, default_payload_length, internal_index));
    }

    /*
     * The private constructor
     */
    framing_impl::framing_impl(int develop_mode, int block_id, int frame_type, int len_frame_type, int frame_index, int len_frame_index, int destination_address, int len_destination_address, int source_address, int len_source_address, int reserved_field_I, int len_reserved_field_I, int reserved_field_II, int len_reserved_field_II, int len_payload_length, int increase_index, int len_num_transmission, int reserved_field_ampdu, std::vector<unsigned int> node_list, std::vector<unsigned int> slot_list_ms, int len_slot_time_beacon, int default_payload, int default_payload_length, int internal_index)
      : gr::block("framing",
              gr::io_signature::make(0, 0, 0),
              gr::io_signature::make(0, 0, 0)),
        _develop_mode(develop_mode),
        _block_id(block_id),
	_frame_type(frame_type),
        _len_frame_type(len_frame_type), // Bytes
        _frame_index(frame_index),
        _len_frame_index(len_frame_index), // Bytes
        _destination_address(destination_address),
        _len_destination_address(len_destination_address), // Bytes
        _source_address(source_address), 
        _len_source_address(len_source_address), // Bytes
        _reserved_field_I(reserved_field_I), 
        _len_reserved_field_I(len_reserved_field_I), // Bytes
        _reserved_field_II(reserved_field_II),
        _len_reserved_field_II(len_reserved_field_II), // Bytes
        _len_payload_length(len_payload_length), // Bytes
        _increase_index(increase_index),
	_len_num_transmission(len_num_transmission),
        _reserved_field_ampdu(reserved_field_ampdu),
        _node_list(node_list),
        _slot_list_ms(slot_list_ms),
        _len_slot_time_beacon(len_slot_time_beacon),
        _default_payload(default_payload),
        _default_payload_length(default_payload_length),
        _internal_index(internal_index)
    {
      if(_develop_mode)
        std::cout << "develop_mode of framing ID: " << _block_id << " is activated." << std::endl;
      message_port_register_in(pmt::mp("B"));
      set_msg_handler(pmt::mp("B"), boost::bind(&framing_impl::catagorization, this, _1 ));
      message_port_register_in(pmt::mp("RT"));
      set_msg_handler(pmt::mp("RT"), boost::bind(&framing_impl::reset_frame_index, this, _1 ));
      message_port_register_in(pmt::mp("RTI"));
      set_msg_handler(pmt::mp("RTI"), boost::bind(&framing_impl::reset_initial_frame_index, this, _1 ));
      message_port_register_out(pmt::mp("E"));
      // only in develop_mode
      message_port_register_out(pmt::mp("PF"));
      _default_index = _frame_index;
      if(_frame_index > 255 || _frame_index < 0)
      {
        std::cout << "frame index should in range [0, 255]. Frame index is set to 0." << std::endl;
        _frame_index = 0;
      }
    }

    /*
     * Our virtual destructor.
     */
    framing_impl::~framing_impl()
    {
    }

    void 
    framing_impl::reset_initial_frame_index(pmt::pmt_t pmt_in)
    {
      _default_index = int(pmt::to_double(pmt_in));
      _frame_index = _default_index;
    }

    void 
    framing_impl::reset_frame_index(pmt::pmt_t pmt_in)
    {
      _frame_index = _default_index;
    }

    void 
    framing_impl::catagorization(pmt::pmt_t data_in)
    {
      if(_develop_mode)
        std::cout << "frame type: "<< _frame_type << std::endl;
      pmt::pmt_t generated_frame;
      if(pmt::is_dict(data_in))
      {
        if(_frame_type == 1)
        {
          generated_frame = data_frame_formation(data_in);
        }
        else if(_frame_type == 2)
        {
          generated_frame = ack_frame_formation(data_in);
        }
        else if(_frame_type == 3)
          generated_frame = beacon_frame_formation(data_in);
        else if(_frame_type == 4)
          std::cout << "frame type 4 is rts frame. It is not generated here" << std::endl;
        else if(_frame_type == 5)
          std::cout << "frame type 5 is cts frame. It is not generated here" << std::endl;
        else if(_frame_type == 6)
          std::cout << "frame type 6 is ampdu frame. It is not aggregated here" << std::endl;
        else if(_frame_type == 7)
          std::cout << "frame type 7 is amsdu frame. It is not aggregated here" << std::endl;
        else if(_frame_type == 8)
        {
          generated_frame = ampdu_subframe_formation(data_in);
        }
        else if(_frame_type == 9)
          generated_frame = amsdu_subframe_formation(data_in);
        else
          std::cout << "Error. wrong frame type in framing ID: " << _block_id << ". please check your connections." << std::endl;
        if(pmt::dict_has_key(generated_frame, pmt::string_to_symbol("frame_pmt")))
        {
          message_port_pub(pmt::mp("E"), generated_frame);
        }
      }
      else if(pmt::is_integer(data_in) && !_internal_index && _frame_type == 1)
      {
        generated_frame = data_frame_formation(data_in);
        if(pmt::dict_has_key(generated_frame, pmt::string_to_symbol("frame_pmt")))
        {
          message_port_pub(pmt::mp("E"), generated_frame);
        }
      } 
      else
      {
        if(_develop_mode)
          std::cout << "framing ID: " << _block_id << " cannot framing input pmt. it is passed to the next blocks." << std::endl;
        message_port_pub(pmt::mp("E"), data_in);
        
      }

    }

    pmt::pmt_t
    framing_impl::ampdu_delimiter_formation(std::vector<unsigned char> *delimiter, int reserved_field_ampdu, int payload_length, int frame_type)
    {
      std::vector< unsigned char > vec_signiture;
      std::vector< unsigned char > vec_reserved_field_ampdu;
      std::vector< unsigned char > vec_payload_length;
      // signiture is the frame type of ampdu subframe, i.e., 8 (N, 0x4E in the original 802.11n
      /* 
        delimiter (5 Bytes)
        delimiter signature (1 Bytes)
        Reserved field ampdu (2 Bytes, equal to reserved field I)
        Payload length (2 Bytes)
       */
      // signuture
      intToByte(frame_type, &vec_signiture, _len_frame_type);
      // Reserved field I
      intToByte(reserved_field_ampdu, &vec_reserved_field_ampdu, _len_reserved_field_I);
      // Payload length
      intToByte(payload_length, &vec_payload_length, _len_payload_length);

      //std::cout  << "delimiter length before signiture: " << delimiter->size() << std::endl;
      delimiter->insert(delimiter->end(), vec_signiture.begin(), vec_signiture.begin() + _len_frame_type);
      //std::cout  << "Frame header length after signiture: " << frame_header->size() << std::endl;
      delimiter->insert(delimiter->end(), vec_reserved_field_ampdu.begin(), vec_reserved_field_ampdu.begin() + _len_reserved_field_I);
      //std::cout  << "delimiter length after reserved_field: " << delimiter->size() << std::endl;
      delimiter->insert(delimiter->end(), vec_payload_length.begin(), vec_payload_length.begin() + _len_payload_length);

      pmt::pmt_t frame_info  = pmt::make_dict();
      frame_info  = pmt::dict_add(frame_info, pmt::string_to_symbol("ampdu_delimiter_signiture"), pmt::from_long(frame_type));
      frame_info  = pmt::dict_add(frame_info, pmt::string_to_symbol("reserved_field_ampdu"), pmt::from_long(reserved_field_ampdu));
      frame_info  = pmt::dict_add(frame_info, pmt::string_to_symbol("payload_length"), pmt::from_long(payload_length));
      return frame_info;
    }

    pmt::pmt_t
    framing_impl::ampdu_subframe_formation(pmt::pmt_t rx_payload)
    {
      if(_develop_mode)
      {
        std::cout << "+++ Framing ID: " << _block_id << " ampdu subframe +++" << std::endl;
      }
      /*
       * Generate a ampdu subframe
       */
      pmt::pmt_t ampdu_subframe_info;
      pmt::pmt_t mpdu_info;
      if(pmt::is_dict(rx_payload)) 
      {
        pmt::pmt_t meta = pmt::car(rx_payload);
        pmt::pmt_t payload_pmt = pmt::cdr(rx_payload);
        std::vector<unsigned char> payload_array; 
        int payload_length;
        if(pmt::is_u8vector(payload_pmt))
        {
          _frame_type = 8;
          payload_array = pmt::u8vector_elements(payload_pmt);
          payload_length = payload_array.size(); 
          std::vector<unsigned char> mpdu_header;
          if(_increase_index)
            _frame_index++;
          mpdu_info = frame_header_formation(&mpdu_header, _frame_type, _frame_index, _destination_address, _source_address, _reserved_field_I, _reserved_field_II, payload_length, 1);
          std::vector<unsigned char> mpdu;
          mpdu.insert(mpdu.end(), mpdu_header.begin(), mpdu_header.end());
          if(_develop_mode)
            std::cout << "MAC header, length " <<mpdu.size() << std::endl;
          mpdu.insert(mpdu.end(), payload_array.begin(), payload_array.end());
          if(_develop_mode)
            std::cout << "MAC header + msdu, length " << mpdu.size() << std::endl;
          // crc
          // crc32_bb_calc(&frame);
          // change frame to pmt::pmt_t
          pmt::pmt_t mpdu_before_crc_u8vector = pmt::init_u8vector(mpdu.size(), mpdu);
          pmt::pmt_t mpdu_before_crc = pmt::cons(meta, mpdu_before_crc_u8vector); 
          pmt::pmt_t mpdu_after_crc = crc32_bb_calc(mpdu_before_crc);
          payload_array = pmt::u8vector_elements(pmt::cdr(mpdu_after_crc));
          if(_develop_mode)
            std::cout << "MAC header + msdu + crc32, length " <<payload_array.size() << std::endl;
          payload_length = payload_array.size(); 
          std::vector<unsigned char> delimiter;
	  ampdu_subframe_info = ampdu_delimiter_formation(&delimiter, _reserved_field_ampdu, payload_length, _frame_type);
          std::vector<unsigned char> ampdu_subframe;
          ampdu_subframe.insert(ampdu_subframe.end(), delimiter.begin(), delimiter.end());
          if(_develop_mode)
            std::cout << "ampdu delimiter, length " << ampdu_subframe.size() << std::endl;
          ampdu_subframe.insert(ampdu_subframe.end(), payload_array.begin(), payload_array.end());
          if(_develop_mode)
            std::cout << "ampdu delimiter + mpdu (MAC header + msdu + crc32), length " << ampdu_subframe.size() << std::endl;
          pmt::pmt_t subframe_before_crc_u8vector = pmt::init_u8vector(ampdu_subframe.size(), ampdu_subframe);
          pmt::pmt_t subframe_before_crc = pmt::cons(meta, subframe_before_crc_u8vector); 
          pmt::pmt_t subframe_after_crc = crc32_bb_calc(subframe_before_crc);
	  payload_array = pmt::u8vector_elements(pmt::cdr(subframe_after_crc));
          if(_develop_mode)
            std::cout << "ampdu delimiter + mpdu + crc32, length " << payload_array.size() << std::endl;
          ampdu_subframe_info = pmt::dict_add(ampdu_subframe_info, pmt::string_to_symbol("subframe_pmt"), subframe_after_crc);
          ampdu_subframe_info = pmt::dict_add(ampdu_subframe_info, pmt::string_to_symbol("mpdu_info"), mpdu_info);
          message_port_pub(pmt::mp("PF"), subframe_after_crc);
          if(_develop_mode == 2)
          {
            struct timeval t; 
            gettimeofday(&t, NULL);
            double current_time = t.tv_sec - double(int(t.tv_sec/10000)*10000) + t.tv_usec / 1000000.0;
            std::cout << "framing ID: " << _block_id << " ampdu subframe is generated at time " << current_time << " s" << std::endl;
          }
        }
        else
          std::cout << "pmt is not a u8vector" << std::endl;
      }
      else 
        std::cout << "pmt is not a pair" << std::endl;
      return ampdu_subframe_info;
    }

    pmt::pmt_t
    framing_impl::amsdu_subframe_formation(pmt::pmt_t rx_payload)
    {
      std::cout << "Under construction" << std::endl;
      return rx_payload;
    }

    pmt::pmt_t
    framing_impl::data_frame_formation(pmt::pmt_t rx_payload)
    {
      if(_develop_mode)
      {
        std::cout << "+++ Framing ID: " << _block_id << " data frame +++" << std::endl;
      }
      /*
       * Generate a data frame
       */
      pmt::pmt_t frame_info;
      pmt::pmt_t frame_after_crc;
      if(pmt::is_pair(rx_payload) || _default_payload) 
      {
        _frame_type = 1;
        pmt::pmt_t payload_pmt;
        std::vector<unsigned char> payload_array; 
        if(!_default_payload)
        {
          pmt::pmt_t meta = pmt::car(rx_payload);
          payload_pmt = pmt::cdr(rx_payload);
        }
        else
        {
          std::vector<unsigned char> payload(_default_payload_length);
          payload_pmt = pmt::init_u8vector(payload.size(), payload); 
        }
        if(pmt::is_u8vector(payload_pmt))
        {
          payload_array = pmt::u8vector_elements(payload_pmt);
          _payload_length = payload_array.size(); 
          std::vector<unsigned char> frame_header;
          if(!_internal_index)
          {
            _frame_index = pmt::to_long(rx_payload);
          }
          frame_info = frame_header_formation(&frame_header, 1, _frame_index, _destination_address, _source_address, _reserved_field_I, _reserved_field_II, _payload_length, 1);
          std::vector<unsigned char> frame;
          frame.insert(frame.end(), frame_header.begin(), frame_header.end());
          if(_develop_mode)
            std::cout << "frame header, length " << frame.size() << std::endl;
          frame.insert(frame.end(), payload_array.begin(), payload_array.end());
          if(_develop_mode)
            std::cout << "frame header with payload, length " << frame.size() << std::endl;
          // crc
          pmt::pmt_t frame_before_crc_u8vector = pmt::init_u8vector(frame.size(), frame);
          pmt::pmt_t frame_before_crc = pmt::cons(pmt::make_dict(), frame_before_crc_u8vector); 
          frame_after_crc = crc32_bb_calc(frame_before_crc);
          frame_info = pmt::dict_add(frame_info, pmt::string_to_symbol("frame_pmt"), frame_after_crc);
          std::vector<unsigned char> frame_after_crc_vector = pmt::u8vector_elements(pmt::cdr(frame_after_crc));
          message_port_pub(pmt::mp("PF"), frame_after_crc);
          if(_develop_mode)
            std::cout << "frame header with payload with crc, length " << frame_after_crc_vector.size() << std::endl;
          if(_develop_mode == 2)
          {
            struct timeval t; 
            gettimeofday(&t, NULL);
            double current_time = t.tv_sec - double(int(t.tv_sec/10000)*10000) + t.tv_usec / 1000000.0;
            std::cout << "framing ID: " << _block_id << " data frame is generated at time " << current_time <<" s" <<  std::endl;
          }
          if(_internal_index)
          {
            if(_increase_index == 1)
            {
              _frame_index++;
              if(_frame_index > 255)
                _frame_index = 0;
            }
            else if(_increase_index == -1)
            {
              _frame_index--;
              if(_frame_index < 0)
                _frame_index = 255;
            }
          }
        }
        else
          std::cout << "pmt is not a u8vector" << std::endl;
      }
      else 
        std::cout << "pmt is not a pair" << std::endl;
      return frame_info;
    }

    pmt::pmt_t
    framing_impl::ack_frame_formation(pmt::pmt_t rx_data)
    {
      if(_develop_mode)
      {
        std::cout << "+++ Framing ID: " << _block_id << " ack frame +++" << std::endl;
      }
      /*
       * Generate an ack frame
       */
      pmt::pmt_t frame_info;
      pmt::pmt_t frame_after_crc;
      if(pmt::is_dict(rx_data))
      {
        pmt::pmt_t meta = pmt::make_dict();
        pmt::pmt_t not_found;
        // generate an ack frame
        int ack_address = pmt::to_long(pmt::dict_ref(rx_data, pmt::string_to_symbol("source_address"), not_found));
        //int src_address = pmt::to_long(pmt::dict_ref(rx_payload, pmt::string_to_symbol("destination_address"), not_found));
        int ack_index = pmt::to_long(pmt::dict_ref(rx_data, pmt::string_to_symbol("frame_index"), not_found));
        int ack_num_transmission = pmt::to_long(pmt::dict_ref(rx_data, pmt::string_to_symbol("num_transmission"), not_found));
        std::vector<unsigned char> frame_header;
        frame_info = frame_header_formation(&frame_header, 2, ack_index, ack_address, _source_address, _reserved_field_I, _reserved_field_II, 0, ack_num_transmission);
        std::vector<unsigned char> frame;
        frame.insert(frame.end(), frame_header.begin(), frame_header.end());
        if(_develop_mode)
          std::cout << "frame header, length " << frame.size() << std::endl;
        // crc
        // crc32_bb_calc(&frame);
        // change frame to pmt::pmt_t
        pmt::pmt_t frame_before_crc_u8vector = pmt::init_u8vector(frame.size(), frame);
        pmt::pmt_t frame_before_crc = pmt::cons(meta, frame_before_crc_u8vector); 
        pmt::pmt_t frame_after_crc = crc32_bb_calc(frame_before_crc);
        frame_info = pmt::dict_add(frame_info, pmt::string_to_symbol("frame_pmt"), frame_after_crc);
        // std::vector<unsigned char> frame_after_crc_vector = pmt::u8vector_elements(pmt::cdr(frame_after_crc));
        // if(_develop_mode)
          // std::cout << "ack frame with crc (no payload), length " << frame_after_crc_vector.size() << std::endl;
      }
      else 
        std::cout << "Error: pmt is not a dict, cannot generate an ack frame. please check your connections." << std::endl;
      message_port_pub(pmt::mp("PF"), frame_after_crc);
      if(_develop_mode == 2)
      {
        struct timeval t; 
        gettimeofday(&t, NULL);
        double current_time = t.tv_sec - double(int(t.tv_sec/10000)*10000) + t.tv_usec / 1000000.0;
        std::cout << "framing ID: " << _block_id << " ACK frame is generated at time " << current_time << " s" << std::endl;
      }
      return frame_info;
    }
 
    pmt::pmt_t
    framing_impl::beacon_frame_formation(pmt::pmt_t rx_beacon_info)
    {
      if(_develop_mode)
      {
        std::cout << "+++ Framing ID: " << _block_id << " beacon frame +++" << std::endl;
      }
      pmt::pmt_t frame_info;
      /*
       * Generate a data frame
       */
      if(_node_list.size() == _slot_list_ms.size())
      {
        pmt::pmt_t meta = pmt::make_dict();
        pmt::pmt_t frame_after_crc;
        std::vector<unsigned char> payload_array; 
        _frame_type = 3;
        std::vector<unsigned char> frame_header;
        if(_increase_index)
        {
          _frame_index++;
          if(_frame_index > 255)
            _frame_index = 0;
        }
        std::vector<unsigned char> vec_node;
        std::vector<unsigned char> vec_slot;
        for(int i = 0; i < _node_list.size(); i++)
        {
          intToByte(_node_list[i], &vec_node, _len_destination_address);
          intToByte(_slot_list_ms[i], &vec_slot, _len_slot_time_beacon);
          // continue from here 
          payload_array.insert(payload_array.end(), vec_node.begin(), vec_node.begin() + _len_destination_address);
          payload_array.insert(payload_array.end(), vec_slot.begin(), vec_slot.begin() + _len_slot_time_beacon);
          vec_node.erase(vec_node.begin(), vec_node.begin() + _len_destination_address);
          vec_slot.erase(vec_slot.begin(), vec_slot.begin() + _len_slot_time_beacon);
        }
        _payload_length = payload_array.size();

        frame_info = frame_header_formation(&frame_header, _frame_type, _frame_index, _destination_address, _source_address, _reserved_field_I, _reserved_field_II, _payload_length, 1);
        std::vector<unsigned char> frame;
        frame.insert(frame.end(), frame_header.begin(), frame_header.end());
        if(_develop_mode)
          std::cout << "frame header, length " << frame.size() << std::endl;
        frame.insert(frame.end(), payload_array.begin(), payload_array.end());
        if(_develop_mode)
          std::cout << "frame header with payload, length " << frame.size() << std::endl;
        // crc
        pmt::pmt_t frame_before_crc_u8vector = pmt::init_u8vector(frame.size(), frame);
        pmt::pmt_t frame_before_crc = pmt::cons(meta, frame_before_crc_u8vector); 
        frame_after_crc = crc32_bb_calc(frame_before_crc);
        frame_info = pmt::dict_add(frame_info, pmt::string_to_symbol("frame_pmt"), frame_after_crc);
        std::vector<unsigned char> frame_after_crc_vector = pmt::u8vector_elements(pmt::cdr(frame_after_crc));
     
         if(_develop_mode)
           std::cout << "frame header with payload with crc, length " << frame_after_crc_vector.size() << std::endl;
        message_port_pub(pmt::mp("PF"), frame_after_crc);
        if(_develop_mode == 2)
        {
          struct timeval t; 
          gettimeofday(&t, NULL);
          double current_time = t.tv_sec - double(int(t.tv_sec/10000)*10000) + t.tv_usec / 1000000.0;
          std::cout << "framing ID: " << _block_id << " data frame is generated at time " << current_time <<" s" <<  std::endl;
        }
      }
      else
      {
        std::cout << "framing ID: " << _block_id << " node_list and the slot list should have the same length. " << std::endl;
      }
      return frame_info;
    }

    pmt::pmt_t
    framing_impl::frame_header_formation(std::vector<unsigned char> *frame_header, int frame_type, int frame_index, int destination_address, int source_address, int reserved_field_I, int reserved_field_II, int payload_length, int num_transmission)
    {
      std::vector< unsigned char > vec_frame_header;
      std::vector< unsigned char > vec_frame_type;
      std::vector< unsigned char > vec_frame_index;
      std::vector< unsigned char > vec_destination_address;
      std::vector< unsigned char > vec_source_address;
      std::vector< unsigned char > vec_transmission;
      std::vector< unsigned char > vec_reserved_field_I;
      std::vector< unsigned char > vec_reserved_field_II;
      std::vector< unsigned char > vec_payload_length;
      /* 
        frame type (1 Bytes)
        frame index (1 Bytes)
        Destination address (1 Bytes)
        Source address (1 Bytes)
        Reserved field 1 (2 Bytes)
        Reserved field 2 (2 Bytes)
        Payload length (1 Bytes)
       */
      // Frame type 
      intToByte(frame_type, &vec_frame_type, _len_frame_type);
      // Frame index
      intToByte(frame_index, &vec_frame_index, _len_frame_index);
      // Destination address
      intToByte(destination_address, &vec_destination_address, _len_destination_address);
      // Payload length
      intToByte(payload_length, &vec_payload_length, _len_payload_length);
      // Source address
      intToByte(source_address, &vec_source_address, _len_source_address);
      // num_transmission 
      intToByte(num_transmission, &vec_transmission, _len_num_transmission);
      // Reserved field I
      intToByte(reserved_field_I, &vec_reserved_field_I, _len_reserved_field_I);
      // Reserved field II
      intToByte(reserved_field_II, &vec_reserved_field_II, _len_reserved_field_II);

      //std::cout  << "Frame header length before frame type: " << frame_header->size() << std::endl;
      frame_header->insert(frame_header->end(), vec_frame_type.begin(), vec_frame_type.begin() + _len_frame_type);
      //std::cout  << "Frame header length after frame type: " << frame_header->size() << std::endl;
      frame_header->insert(frame_header->end(), vec_frame_index.begin(), vec_frame_index.begin() + _len_frame_index);
      //std::cout  << "Frame header length after frame index: " << frame_header->size() << std::endl;
      frame_header->insert(frame_header->end(), vec_destination_address.begin(), vec_destination_address.begin() + _len_destination_address);
      //std::cout  << "Frame header length after dest: " << frame_header->size() << std::endl;
      frame_header->insert(frame_header->end(), vec_source_address.begin(), vec_source_address.begin() + _len_source_address);
      //std::cout  << "Frame header length after src: " << frame_header->size() << std::endl;
      frame_header->insert(frame_header->end(), vec_transmission.begin(), vec_transmission.begin() + _len_num_transmission);
      //std::cout  << "Frame header length after transmission: " << frame_header->size() << std::endl;
      frame_header->insert(frame_header->end(), vec_reserved_field_I.begin(), vec_reserved_field_I.begin() + _len_reserved_field_I);
      //std::cout  << "Frame header length after re1: " << frame_header->size() << std::endl;
      frame_header->insert(frame_header->end(), vec_reserved_field_II.begin(), vec_reserved_field_II.begin() + _len_reserved_field_II);
      //std::cout  << "Frame header length after re2: " << frame_header->size() << std::endl;
      frame_header->insert(frame_header->end(), vec_payload_length.begin(), vec_payload_length.begin() + _len_payload_length);

      pmt::pmt_t frame_info  = pmt::make_dict();
      frame_info  = pmt::dict_add(frame_info, pmt::string_to_symbol("frame_type"), pmt::from_long(frame_type));
      frame_info  = pmt::dict_add(frame_info, pmt::string_to_symbol("frame_index"), pmt::from_long(frame_index));
      frame_info  = pmt::dict_add(frame_info, pmt::string_to_symbol("destination_address"), pmt::from_long(destination_address));
      frame_info  = pmt::dict_add(frame_info, pmt::string_to_symbol("source_address"), pmt::from_long(source_address));
      frame_info  = pmt::dict_add(frame_info, pmt::string_to_symbol("num_transmission"), pmt::from_long(1));
      frame_info  = pmt::dict_add(frame_info, pmt::string_to_symbol("reserved_field_I"), pmt::from_long(reserved_field_I));
      frame_info  = pmt::dict_add(frame_info, pmt::string_to_symbol("reserved_field_II"), pmt::from_long(reserved_field_II));
      frame_info  = pmt::dict_add(frame_info, pmt::string_to_symbol("payload_length"), pmt::from_long(payload_length));
      frame_info  = pmt::dict_add(frame_info, pmt::string_to_symbol("header_length"), pmt::from_long(get_frame_header_length()));
      frame_info  = pmt::dict_add(frame_info, pmt::string_to_symbol("address_check"),pmt::from_long(0));
      frame_info  = pmt::dict_add(frame_info, pmt::string_to_symbol("good_frame"),pmt::from_long(0));
      struct timeval t; 
      gettimeofday(&t, NULL);
      double gen_time = t.tv_sec + t.tv_usec / 1000000.0;
      frame_info  = pmt::dict_add(frame_info, pmt::string_to_symbol("generation_time"),pmt::from_double(gen_time));
      return frame_info;
    }

    void 
    framing_impl::intToByte(int i, std::vector<unsigned char> *bytes, int size)
    {
//      std::cout << "Type is about to converted" << std::endl;
      bytes->insert(bytes->end(), (unsigned char) (0xff & i));
//      std::cout << "First byte is converted" << std::endl;
      if(size > 1)
      {
        bytes->insert(bytes->end(), (unsigned char) ((0xff00 & i) >> 8));
        if(size > 2)
        {
          bytes->insert(bytes->end(), (unsigned char) ((0xff0000 & i) >> 16));
          if(size > 3)
          {
            bytes->insert(bytes->end(), (unsigned char) ((0xff000000 & i) >> 24));
          }
        }
      }
    }
    
    pmt::pmt_t
    framing_impl::crc32_bb_calc(pmt::pmt_t msg)
    {
      // extract input pdu
      pmt::pmt_t meta(pmt::car(msg));
      pmt::pmt_t bytes(pmt::cdr(msg));

      unsigned int crc;
      size_t pkt_len(0);
      const uint8_t* bytes_in = pmt::u8vector_elements(bytes, pkt_len);
      uint8_t* bytes_out = (uint8_t*)volk_malloc(4 + pkt_len*sizeof(uint8_t),
                                                 volk_get_alignment());

      _crc_impl.reset();
      _crc_impl.process_bytes(bytes_in, pkt_len);
      crc = _crc_impl();
      memcpy((void*)bytes_out, (const void*)bytes_in, pkt_len);
      memcpy((void*)(bytes_out + pkt_len), &crc, 4); // FIXME big-endian/little-endian, this might be wrong

      pmt::pmt_t output = pmt::init_u8vector(pkt_len+4, bytes_out); // this copies the values from bytes_out into the u8vector
      return pmt::cons(meta, output);
    } 

    void 
    framing_impl::disp_vec(std::vector<unsigned char> vec)
    {
      for(int i=0; i<vec.size(); ++i)
        std::cout << static_cast<unsigned>(vec[i]) << ' ';
      std::cout << "total length is :" << vec.size() << " bytes." << std::endl;
    }

    int
    framing_impl::get_frame_header_length()
    {
      return _len_frame_type + _len_frame_index + _len_destination_address + _len_source_address + _len_num_transmission + _len_reserved_field_I + _len_reserved_field_II + _len_payload_length;
    } 

  } /* namespace dmdl */
} /* namespace gr */

