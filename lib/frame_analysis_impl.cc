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
#include <cmath>
#include "frame_analysis_impl.h"
#include <gnuradio/digital/crc32.h>
#include <volk/volk.h>
#include <boost/crc.hpp>

namespace gr {
  namespace dmdl {

    frame_analysis::sptr
    frame_analysis::make(int develop_mode, int block_id, int len_frame_type, int len_frame_index, int len_destination_address, int len_source_address, int len_num_transmission, int len_reserved_field_I, int len_reserved_field_II, int len_payload_length, int apply_self_address_check, int my_address)
    {
      return gnuradio::get_initial_sptr
        (new frame_analysis_impl(develop_mode, block_id, len_frame_type, len_frame_index, len_destination_address, len_source_address, len_num_transmission, len_reserved_field_I, len_reserved_field_II, len_payload_length, apply_self_address_check, my_address));
    }

    /*
     * The private constructor
     */
    frame_analysis_impl::frame_analysis_impl(int develop_mode, int block_id, int len_frame_type, int len_frame_index, int len_destination_address, int len_source_address, int len_num_transmission, int len_reserved_field_I, int len_reserved_field_II, int len_payload_length, int apply_self_address_check, int my_address)
      : gr::block("frame_analysis",
              gr::io_signature::make(0, 0, 0),
              gr::io_signature::make(0, 0, 0)),
        _block_id(block_id),
        _develop_mode(develop_mode),
        _len_frame_type(len_frame_type), // Bytes
        _len_frame_index(len_frame_index), // Bytes
        _len_destination_address(len_destination_address), // Bytes
        _len_source_address(len_source_address), // Bytes
        _len_num_transmission(len_num_transmission), // Bytes
        _len_reserved_field_I(len_reserved_field_I), // Bytes
        _len_reserved_field_II(len_reserved_field_II), // Bytes
        _len_payload_length(len_payload_length), // Bytes
        _apply_self_address_check(apply_self_address_check),
        _my_address(my_address)
    {
      if(_develop_mode == 1)
        std::cout << "develop_mode of frame_analysis ID: " << _block_id << " is activated." << std::endl;
      message_port_register_in(pmt::mp("B"));
      message_port_register_out(pmt::mp("P"));
      set_msg_handler(pmt::mp("B"), boost::bind(&frame_analysis_impl::frame_analysis, this, _1 ));
    }

    /*
     * Our virtual destructor.
     */
    frame_analysis_impl::~frame_analysis_impl()
    {
    }

    void
    frame_analysis_impl::frame_analysis(pmt::pmt_t rx_frame)
    {
      if(_develop_mode == 1)
      {
        std::cout << "++++++++  frame_analysis ID: " << _block_id << "  +++++++" << std::endl;
      }
      if(pmt::is_pair(rx_frame)) 
      {
        bool good_frame;
	pmt::pmt_t not_found;
        pmt::pmt_t meta = pmt::car(rx_frame);
        pmt::pmt_t frame_pmt = pmt::cdr(rx_frame);
        std::vector<unsigned char> frame_array; 
        if(pmt::is_u8vector(frame_pmt))
        {
          std::vector<unsigned char> frame_array = pmt::u8vector_elements(frame_pmt);
          int _frame_length = frame_array.size(); 
          pmt::pmt_t frame_INFO;
          /*
           * frame type check
           */
          int frame_type = static_cast<unsigned>(frame_array[0]);
          if(frame_type <= 5 && frame_type > 0)
          {
            frame_INFO = frame_decompose(frame_pmt, frame_type);
	    if(pmt::to_long(pmt::dict_ref(frame_INFO, pmt::string_to_symbol("self_address_check"), not_found)))
              message_port_pub(pmt::mp("P"), frame_INFO);
	    else
              if(_develop_mode == 1)
                std::cout << "no output self frame." << std::endl;
          }
          else if(frame_type == 8)
          {
            ampdu_decompose(frame_pmt, frame_type);
          }
        }
        else
          std::cout << "pmt is not a u8vector" << std::endl;
      }
      else 
        std::cout << "pmt is not a pair" << std::endl;
    }

    void
    frame_analysis_impl::ampdu_decompose(pmt::pmt_t frame_pmt, int frame_type)
    {
      pmt::pmt_t subframe_INFO;
      pmt::pmt_t not_found;
      int crc32_length = 4;
      std::vector<unsigned char> frame_array = pmt::u8vector_elements(frame_pmt);
      // Create delimiter array
      int delimiter_length = get_ampdu_delimiter_length();
      std::vector<unsigned char> delimiter_array;
      delimiter_array.insert(delimiter_array.end(), frame_array.begin(), frame_array.begin() + delimiter_length);
      // Get reserved field ampdu 
      int reserved_field_ampdu_pos = _len_frame_type;
      std::vector<unsigned char> reserved_field_ampdu_array(delimiter_array.begin() + reserved_field_ampdu_pos, delimiter_array.begin() + reserved_field_ampdu_pos + _len_reserved_field_I);
      int reserved_field_ampdu = BytesToint(reserved_field_ampdu_array);
      // Get mpdu length
      int mpdu_length_pos = reserved_field_ampdu_pos + _len_reserved_field_I;
      std::vector<unsigned char> mpdu_length_array(delimiter_array.begin() + mpdu_length_pos, delimiter_array.begin() + mpdu_length_pos + _len_payload_length);
      int mpdu_length = BytesToint(mpdu_length_array);
      
      if(_develop_mode)
        std::cout << "mpdu length: " << mpdu_length << " delimiter_length: " << delimiter_length << std::endl;
      int number_aggregation = floor(frame_array.size() / (mpdu_length + delimiter_length));
      if(_develop_mode == 1)
      {	
        std::cout << "++++++ AMPDU frame with " << number_aggregation << " mpdu ++++++" << number_aggregation << std::endl;
        std::cout << "reserved_field_ampdu: " << reserved_field_ampdu << std::endl;
        std::cout << "mpdu length: " << mpdu_length << std::endl;
      }
      for(int i = 0; i < number_aggregation; i++)
      {
	if(_develop_mode)
          std::cout << "+++ " << i << "/" << number_aggregation << " subframes in AMPDU +++" << std::endl;
        std::vector<unsigned char> mpdu_array;
        mpdu_array.insert(mpdu_array.end(), frame_array.begin() + delimiter_length + i * (delimiter_length + mpdu_length + crc32_length), frame_array.begin() + (i + 1) * (delimiter_length + mpdu_length + crc32_length) - crc32_length);
        pmt::pmt_t mpdu_pmt = pmt::init_u8vector(mpdu_array.size(), mpdu_array);
        /*
	if(_develop_mode == 1)
	{
          std::cout << "mpdu: ";
          disp_vec(mpdu_array);
	}
	*/
        subframe_INFO = frame_decompose(mpdu_pmt, frame_type);
        if(pmt::to_long(pmt::dict_ref(subframe_INFO, pmt::string_to_symbol("self_address_check"), not_found)))
	{
          message_port_pub(pmt::mp("P"), subframe_INFO);
	}
	else
	  std::cout << "no output self frame." << std::endl;
      } 
    }	     

    int
    frame_analysis_impl::get_frame_header_length()
    {
      return _len_frame_type + _len_frame_index + _len_destination_address + _len_source_address + _len_num_transmission + _len_reserved_field_I + _len_reserved_field_II + _len_payload_length;
    } 

    int
    frame_analysis_impl::get_ampdu_delimiter_length()
    {
      return _len_frame_type + _len_reserved_field_I + _len_payload_length;
    } 

    int 
    frame_analysis_impl::BytesToint(std::vector<unsigned char> bytes)
    {
      int int_num = 0;
      int size = bytes.size();
      for(int i = 0; i < size; ++i)  
      {
        int_num = static_cast<int>(bytes.back() << ((size - i - 1) * 8)) + int_num;
        bytes.pop_back();
      } 
      return int_num;
    }
    
    pmt::pmt_t
    frame_analysis_impl::frame_decompose(pmt::pmt_t frame_pmt, int frame_type)
    {
      if(_develop_mode == 1)
        std::cout << "+++ frame information +++" << std::endl;
      std::vector<unsigned char> frame_array = pmt::u8vector_elements(frame_pmt);
      pmt::pmt_t frame_INFO  = pmt::make_dict();
      int header_length = get_frame_header_length();
      std::vector<unsigned char> frame_header_array;
      frame_header_array.insert(frame_header_array.end(), frame_array.begin(), frame_array.begin() + header_length);
      int is_good_frame = 0;
      // Get frame index
      int index_pos = _len_frame_type;
      std::vector<unsigned char> index_array(frame_header_array.begin() + index_pos, frame_header_array.begin() + index_pos + _len_frame_index);
      // disp_vec(index_array);
      int index = BytesToint(index_array);

      // Get destination address
      int dest_pos = index_pos + _len_frame_index;
      std::vector<unsigned char> dest_array(frame_header_array.begin() + dest_pos, frame_header_array.begin() + dest_pos + _len_destination_address);
      //disp_vec(dest_array);
      int destination_address = BytesToint(dest_array);
      
      // Get source address
      int src_pos = dest_pos + _len_destination_address;
      std::vector<unsigned char> src_array(frame_header_array.begin() + src_pos, frame_header_array.begin() + src_pos + _len_source_address);
      //disp_vec(src_array);
      int source_address = BytesToint(src_array);
      // now we can check the address to make sure that the rx frame is not send by myself
      int self_address_check;
      if(source_address != _my_address)
        self_address_check = 1;
      else
        self_address_check = 0;

      // Get number of transmissions
      int ntrans_pos = src_pos + _len_source_address;
      std::vector<unsigned char> ntrans_array(frame_header_array.begin() + ntrans_pos, frame_header_array.begin() + ntrans_pos + _len_num_transmission);
      //disp_vec(ntrans_array);
      int num_transmission = BytesToint(ntrans_array);
      
      // Get reserved field I
      int re_I_pos = ntrans_pos + _len_num_transmission;
      std::vector<unsigned char> re_I_array(frame_header_array.begin() + re_I_pos, frame_header_array.begin() + re_I_pos + _len_reserved_field_I);
      //disp_vec(re_I_array);
      int reserved_field_I = BytesToint(re_I_array);
      
      // Get reserved field II
      int re_II_pos = re_I_pos + _len_reserved_field_I;
      std::vector<unsigned char> re_II_array(frame_header_array.begin() + re_II_pos, frame_header_array.begin() + re_II_pos + _len_reserved_field_II);
      //disp_vec(re_II_array);
      int reserved_field_II = BytesToint(re_II_array);
      
      // Get payload length
      int payload_length_pos = re_II_pos + _len_reserved_field_II;
      std::vector<unsigned char> payload_length_array(frame_header_array.begin() + payload_length_pos, frame_header_array.begin() + payload_length_pos + _len_payload_length);
      //disp_vec(payload_length_array);
      int payload_length = BytesToint(payload_length_array);
      
      // Get payload
      int payload_pos = payload_length_pos + _len_payload_length;
      std::vector<unsigned char> payload_array(frame_array.begin() + payload_pos, frame_array.begin() + payload_pos + payload_length);
      // payload of rts or cts is the nav time
      int nav_time_us = 0;
      if(frame_type == 4 || frame_type == 5)
      {
        nav_time_us = BytesToint(payload_array);
      }

      // Get CRC
      int crc_pos = payload_pos + payload_length;
      std::vector<unsigned char> crc_array(frame_array.begin() + crc_pos, frame_array.begin() + crc_pos + 4);

      if(_develop_mode == 1)
      {
        std::cout << "Frame header array is: ";
        disp_vec(frame_header_array);
        std::cout << "frame type is: " << frame_type << std::endl;
        std::cout << "frame index is: " << index << std::endl;
        std::cout << "destination address is: " << destination_address << std::endl;
        std::cout << "source address is: " << source_address << std::endl;
        std::cout << "number of transmission is: " << num_transmission << std::endl;
        std::cout << "reserved field I is: " << reserved_field_I << std::endl;
        std::cout << "reserved field II is: " << reserved_field_II << std::endl;
        std::cout << "payload length is: " << payload_length << std::endl;
        std::cout << "frame header length is: " << header_length << std::endl;
        std::cout << "self address check is: " << self_address_check << std::endl;
        if(frame_type == 4 || frame_type == 5)
          std::cout << "nav time is: " << nav_time_us << "us" << std::endl;
        std::cout << "frame verification (good_frame) is initialized to: " << is_good_frame << std::endl;
        std::cout << "payload is: ";
        disp_vec(payload_array);
        std::cout << "crc is: ";
        disp_vec(crc_array);
      }
      pmt::pmt_t not_found;

      if(self_address_check == 0 && _apply_self_address_check == 1)
      {
        frame_INFO  = pmt::dict_add(frame_INFO, pmt::string_to_symbol("self_address_check"), pmt::from_long(0));
        if(_develop_mode == 1)
	std::cout << "Self address check is: " << self_address_check << " and apply self address check is: " << _apply_self_address_check << std::endl;
      }
      else
      {
        pmt::pmt_t meta = pmt::make_dict();
        pmt::pmt_t frame_header_array_u8vector = pmt::init_u8vector(header_length, frame_header_array);
        pmt::pmt_t frame_header_array_pmt = pmt::cons(meta, frame_header_array_u8vector);
      
        frame_INFO = pmt::dict_add(frame_INFO, pmt::string_to_symbol("frame_type"), pmt::from_long(frame_type));
        frame_INFO = pmt::dict_add(frame_INFO, pmt::string_to_symbol("frame_index"), pmt::from_long(index));
        frame_INFO = pmt::dict_add(frame_INFO, pmt::string_to_symbol("destination_address"), pmt::from_long(destination_address));
        frame_INFO = pmt::dict_add(frame_INFO, pmt::string_to_symbol("source_address"), pmt::from_long(source_address));
        frame_INFO = pmt::dict_add(frame_INFO, pmt::string_to_symbol("num_transmission"), pmt::from_long(num_transmission));
        frame_INFO = pmt::dict_add(frame_INFO, pmt::string_to_symbol("reserved_field_I"), pmt::from_long(reserved_field_I));
        frame_INFO = pmt::dict_add(frame_INFO, pmt::string_to_symbol("reserved_field_II"), pmt::from_long(reserved_field_II));
        frame_INFO = pmt::dict_add(frame_INFO, pmt::string_to_symbol("payload_length"), pmt::from_long(payload_length));
        frame_INFO = pmt::dict_add(frame_INFO, pmt::string_to_symbol("header_length"), pmt::from_long(header_length));
        frame_INFO = pmt::dict_add(frame_INFO, pmt::string_to_symbol("self_address_check"),pmt::from_long(1));
        frame_INFO = pmt::dict_add(frame_INFO, pmt::string_to_symbol("address_check"),pmt::from_long(0));
        frame_INFO = pmt::dict_add(frame_INFO, pmt::string_to_symbol("good_frame"),pmt::from_long(is_good_frame));
        frame_INFO = pmt::dict_add(frame_INFO, pmt::string_to_symbol("nav_time"),pmt::from_long(nav_time_us));
        struct timeval t; 
        gettimeofday(&t, NULL);
        double current_time = t.tv_sec + t.tv_usec / 1000000.0;
        frame_INFO = pmt::dict_add(frame_INFO, pmt::string_to_symbol("analysis_time"),pmt::from_double(current_time));
        if(payload_array.size() > 0)
          frame_INFO = pmt::dict_add(frame_INFO, pmt::string_to_symbol("payload"), pmt::init_u8vector(payload_array.size(), payload_array));

        frame_INFO = pmt::dict_add(frame_INFO, pmt::string_to_symbol("frame_pmt"), pmt::cons(meta, frame_pmt));
        if(_develop_mode == 2)
        {
          double current_time = t.tv_sec - double(int(t.tv_sec/101)*100) + t.tv_usec / 1000000.0;
          if(frame_type == 1)
            std::cout << "* header analysis ID: " << _block_id << " get the " << num_transmission <<"th transmission of data frame "<< index << " at time " << current_time << " s" << std::endl;
          else
            std::cout << "* header analysis ID: " << _block_id << " get the ack frame of the " << num_transmission <<"th transmission of data frame "<< index << " at time " << current_time << " s" << std::endl;
        }
      } 
      return frame_INFO;
    }

    void 
    frame_analysis_impl::disp_vec(std::vector<unsigned char> vec)
    {
      for(int i=0; i<vec.size(); ++i)
        std::cout << static_cast<unsigned>(vec[i]) << ' ';
      std::cout << ". total length is: " << vec.size() << std::endl;
    }

  } /* namespace dmdl */
} /* namespace gr */

