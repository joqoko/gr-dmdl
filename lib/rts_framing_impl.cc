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
#include "rts_framing_impl.h"
#include <gnuradio/digital/crc32.h> 
#include <volk/volk.h>
#include <boost/crc.hpp>

namespace gr {
  namespace dmdl {

    rts_framing::sptr
    rts_framing::make(int develop_mode, int block_id, int len_frame_type, int len_frame_index, int destination_address, int len_destination_address, int source_address, int len_source_address, int reserved_field_I, int len_reserved_field_I, int reserved_field_II, int len_reserved_field_II, int len_payload_length, int len_num_transmission, int len_rts_cts_payload, int padding, std::vector<unsigned char> preamble, double bps, int SIFS, int slot_time)
    {
      return gnuradio::get_initial_sptr
        (new rts_framing_impl(develop_mode, block_id, len_frame_type, len_frame_index, destination_address, len_destination_address, source_address, len_source_address, reserved_field_I, len_reserved_field_I, reserved_field_II, len_reserved_field_II, len_payload_length, len_num_transmission, len_rts_cts_payload, padding, preamble, bps, SIFS, slot_time));
    }

    /*
     * The private constructor
     */
    rts_framing_impl::rts_framing_impl(int develop_mode, int block_id, int len_frame_type, int len_frame_index, int destination_address, int len_destination_address, int source_address, int len_source_address, int reserved_field_I, int len_reserved_field_I, int reserved_field_II, int len_reserved_field_II, int len_payload_length, int len_num_transmission, int len_rts_cts_payload, int padding, std::vector<unsigned char> preamble, double bps, int SIFS, int slot_time)
      : gr::block("rts_framing",
              gr::io_signature::make(0, 0, 0),
              gr::io_signature::make(0, 0, 0)),
        _develop_mode(develop_mode),
        _block_id(block_id),
        _len_frame_type(len_frame_type), // Bytes
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
        _padding(padding),
        _preamble(preamble),
        _bps(bps),
        _len_rts_cts_payload(len_rts_cts_payload),
	_len_num_transmission(len_num_transmission),
        _SIFS(SIFS),
        _slot_time(slot_time)
    {
      if(_develop_mode)
        std::cout << "develop_mode of rts_framing ID: " << _block_id << " is activated." << std::endl;
      message_port_register_in(pmt::mp("DATA"));
      set_msg_handler(pmt::mp("DATA"), boost::bind(&rts_framing_impl::framing, this, _1 ));
      message_port_register_in(pmt::mp("CTS"));
      set_msg_handler(pmt::mp("CTS"), boost::bind(&rts_framing_impl::send_data, this, _1 ));
      message_port_register_out(pmt::mp("RTS"));
      // only in develop_mode
      message_port_register_out(pmt::mp("PF"));
      if(_frame_index > 255 || _frame_index < 0)
      {
        if(_develop_mode)
          std::cout << "frame index should in range [0, 255]. Frame index is set to 0." << std::endl;
        _frame_index = 0;
      }
      _preamble_length = _preamble.size() / 8;
      _data_frame = pmt::from_long(0);
      _rts_frame = pmt::from_long(0);
    }

    /*
     * Our virtual destructor.
     */
    rts_framing_impl::~rts_framing_impl()
    {
     
    }


    void
    rts_framing_impl::send_data(pmt::pmt_t cts_frame)
    {
      pmt::pmt_t not_found;
      if(!pmt::is_dict(_rts_frame))
        std::cout << " error: rts_framing ID " << _block_id <<  " receives cts_frame before transmitting a rts_frame. " << std::endl;
      else
      {
        int rts_index = pmt::to_long(pmt::dict_ref(_rts_frame, pmt::string_to_symbol("frame_index"), not_found));
        int cts_index = pmt::to_long(pmt::dict_ref(cts_frame, pmt::string_to_symbol("frame_index"), not_found));
        if(rts_index == cts_index)
        {
          if(pmt::is_dict(_data_frame))
          {
            message_port_pub(pmt::mp("RTS"), _data_frame);
            pmt::pmt_t frame_after_crc = pmt::dict_ref(_data_frame, pmt::string_to_symbol("frame_pmt"), not_found);
            message_port_pub(pmt::mp("PF"), frame_after_crc);
            _data_frame = pmt::from_long(0);
            _rts_frame = pmt::from_long(0);
            if(_develop_mode)
              std::cout << "rts frame is correctly responsed by a cts frame. data frame is transmitting. " << std::endl;
          }
          else
            std::cout << " error: rts_framing ID " << _block_id <<  " rts frame is correctly responsed by a cts_frame but no data frame are buffered. " << std::endl;

            
        } 
        else
        {
          std::cout << " error: rts_framing ID " << _block_id <<  " received cts_frame does not fit the transmitted rts_frame. " << std::endl;
        }
      } 
    }
 
    void
    rts_framing_impl::framing(pmt::pmt_t data_frame)
    {
      pmt::pmt_t not_found;
      if(_develop_mode)
      {
        std::cout << "+++ rts_framing ID: " << _block_id << " +++" << std::endl;
      }
      if(pmt::is_dict(_data_frame))
        std::cout << "Warning: rts_framing ID " << _block_id <<  " receives a data frame before transmitting/dropping the previous one. " << std::endl;
      _data_frame = data_frame; 
      /*
       * data frame duration
       */
      int header_length = pmt::to_long(pmt::dict_ref(data_frame, pmt::string_to_symbol("header_length"), not_found));
      int payload_length = pmt::to_long(pmt::dict_ref(data_frame, pmt::string_to_symbol("payload_length"), not_found));
      int data_mpdu_length = header_length + payload_length + 4;
      int data_ppdu_length = _padding + 4 + _preamble_length + _padding + data_mpdu_length;
      int data_tx_time_us = data_ppdu_length * 8 * (1000000 / double(_bps));
      if(_develop_mode)
        std::cout << "calculated data frame mpdu length is: " << data_mpdu_length << " and measured mpdu length is: " << pmt::u8vector_elements(pmt::cdr(pmt::dict_ref(data_frame, pmt::string_to_symbol("frame_pmt"), not_found))).size() << std::endl;
      if(_develop_mode)
        std::cout << "data frame ppdu length is: " << data_ppdu_length << ". with bitrate: " << _bps << ", the transmission time is: " << data_tx_time_us << "us" << std::endl;
      /*
       * cts frame duration
       */
      int cts_mpdu_length = header_length + _len_rts_cts_payload + 4;
      int cts_ppdu_length = _padding + 4 + _preamble_length + _padding + cts_mpdu_length;
      int cts_tx_time_us = cts_ppdu_length * 8 * (1000000 / _bps);
      if(_develop_mode)
        std::cout << "cts frame ppdu length is: " << cts_ppdu_length << ". with bitrate: " << _bps << ", the transmission time is: " << cts_tx_time_us << "us" << std::endl;
      // then calculate tx time of an ack frame
      int ack_mpdu_length = header_length + 4;
      int ack_ppdu_length = _padding + 4 + _preamble_length + _padding + ack_mpdu_length;
      int ack_tx_time_us = ack_ppdu_length * 8 * (1000000 / _bps);
      if(_develop_mode)
        std::cout << "ack frame ppdu length is: " << ack_ppdu_length << ". with bitrate: " << _bps << ", the transmission time is: " << ack_tx_time_us << "us" << std::endl;
      // plus three SIFS durations
       
      int nav_rts_us = _SIFS * 3 + data_tx_time_us + cts_tx_time_us + ack_tx_time_us;
      if(_develop_mode)
        std::cout << "overall transmission time is: " << nav_rts_us << "us" << std::endl;
      std::vector<unsigned char> vec_nav;
      intToByte(nav_rts_us, &vec_nav, _len_rts_cts_payload);
      /*
       * generating the frame 
       */
      int data_index = pmt::to_long(pmt::dict_ref(data_frame, pmt::string_to_symbol("frame_index"), not_found));
      pmt::pmt_t frame_info;
      pmt::pmt_t meta = pmt::make_dict();
      std::vector<unsigned char> frame_header;
      frame_info = frame_header_formation(&frame_header, 4, data_index, _destination_address, _source_address, _reserved_field_I, _reserved_field_II, _len_rts_cts_payload, 1);
      std::vector<unsigned char> frame;
      frame.insert(frame.end(), frame_header.begin(), frame_header.end());
      frame.insert(frame.end(), vec_nav.begin(), vec_nav.end());
      if(_develop_mode)
        std::cout << "rts header with payload length " << frame.size() << std::endl;
      // crc
      // crc32_bb_calc(&frame);
      // change frame to pmt::pmt_t
      pmt::pmt_t frame_before_crc_u8vector = pmt::init_u8vector(frame.size(), frame);
      pmt::pmt_t frame_before_crc = pmt::cons(meta, frame_before_crc_u8vector); 
      pmt::pmt_t frame_after_crc = crc32_bb_calc(frame_before_crc);
      frame_info = pmt::dict_add(frame_info, pmt::string_to_symbol("frame_pmt"), frame_after_crc);
      frame_info = pmt::dict_add(frame_info, pmt::string_to_symbol("nav_time"), pmt::from_long(nav_rts_us));
      // std::vector<unsigned char> frame_after_crc_vector = pmt::u8vector_elements(pmt::cdr(frame_after_crc));
      // if(_develop_mode)
        // std::cout << "ack frame with crc (no payload), length " << frame_after_crc_vector.size() << std::endl;
      _rts_frame = frame_info;
      message_port_pub(pmt::mp("RTS"), _rts_frame);
      message_port_pub(pmt::mp("PF"), frame_after_crc);
      if(_develop_mode == 2)
      {
        struct timeval t; 
        gettimeofday(&t, NULL);
        double current_time = t.tv_sec - double(int(t.tv_sec/100)*100) + t.tv_usec / 1000000.0;
        std::cout << "framing ID: " << _block_id << " rts frame is generated at time " << current_time << " s" << std::endl;
      }
    }

    pmt::pmt_t
    rts_framing_impl::frame_header_formation(std::vector<unsigned char> *frame_header, int frame_type, int frame_index, int destination_address, int source_address, int reserved_field_I, int reserved_field_II, int payload_length, int num_transmission)
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
      return frame_info;
    }

    void 
    rts_framing_impl::intToByte(int i, std::vector<unsigned char> *bytes, int size)
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
    rts_framing_impl::crc32_bb_calc(pmt::pmt_t msg)
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

    int
    rts_framing_impl::get_frame_header_length()
    {
      return _len_frame_type + _len_frame_index + _len_destination_address + _len_source_address + _len_num_transmission + _len_reserved_field_I + _len_reserved_field_II + _len_payload_length;
    } 

  } /* namespace dmdl */
} /* namespace gr */

