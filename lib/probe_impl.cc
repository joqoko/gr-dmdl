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

#include <iostream>
#include <fstream>
#include <string>
#include <ctime>
#include <queue>
#include <gnuradio/io_signature.h>
#include "probe_impl.h"

namespace gr {
  namespace dmdl {

    probe::sptr
    probe::make(int develop_mode, int block_id, int print_frame, int cs_mode, double cs_threshold, int record_on, std::string record_path, std::string file_name_extension, int name_with_timestamp)
    {
      return gnuradio::get_initial_sptr
        (new probe_impl(develop_mode, block_id, print_frame, cs_mode, cs_threshold, record_on, record_path, file_name_extension, name_with_timestamp));
    }

    /*
     * The private constructor
     */
    probe_impl::probe_impl(int develop_mode, int block_id, int print_frame, int cs_mode, double cs_threshold, int record_on, std::string record_path, std::string file_name_extension, int name_with_timestamp)
      : gr::block("probe",
              gr::io_signature::make(0, 0, 0),
              gr::io_signature::make(0, 0, 0)),
        _develop_mode(develop_mode),
        _block_id(block_id),
	_print_frame(print_frame),
        _cs_mode(cs_mode),
        _record_on(record_on),
        _last_cs_status(0),
        _temp_n(0),
        _file_name_extension(file_name_extension),
        _name_with_timestamp(name_with_timestamp),
        _cs_threshold(cs_threshold)
    {
      message_port_register_out(pmt::mp("E"));
      message_port_register_in(pmt::mp("B"));
      set_msg_handler(
        pmt::mp("B"),
        boost::bind(&probe_impl::read_info, this, _1)
      ); 
      message_port_register_in(pmt::mp("FN"));
      set_msg_handler(
        pmt::mp("FN"),
        boost::bind(&probe_impl::change_file_name, this, _1)
      ); 
      struct timeval t;
      gettimeofday(&t, NULL);
      _last_time = t.tv_sec + t.tv_usec / 1000000.0;

      if(_record_on)
      {
        time_t tt = time(0);   // get time now
        struct tm * now = localtime( & tt );
        std::ostringstream file_name;
        if(_name_with_timestamp)
          file_name << "/home/inets/source/gr-inets/results/" << (now->tm_year + 1900) << "_" << (now->tm_mon + 1) << "_" << now->tm_mday << "_" << now->tm_hour << "_" << now->tm_min << "_" << now->tm_sec << "_block" << _block_id << "_" << _file_name_extension << ".txt";
        else
          file_name << "/home/inets/source/gr-inets/results/" << _file_name_extension << ".txt";
        _file_name_str = file_name.str();
      }
    }

    /*
     * Our virtual destructor.
     */
    probe_impl::~probe_impl()
    {
    }

    void
    probe_impl::change_file_name(pmt::pmt_t new_name)
    { 
      // currently only accept double as input
      if(pmt::is_real(new_name))
      {
        time_t tt = time(0);   // get time now
        struct tm * now = localtime( & tt );
        std::ostringstream file_name;
        if(_name_with_timestamp)
          file_name << "/home/inets/source/gr-inets/results/" << (now->tm_year + 1900) << "_" << (now->tm_mon + 1) << "_" << now->tm_mday << "_" << now->tm_hour << "_" << now->tm_min << "_" << now->tm_sec << "_block" << _block_id << "_" << _file_name_extension << new_name << ".txt";
        else
          file_name << "/home/inets/source/gr-inets/results/" << _file_name_extension << new_name << ".txt";
        _file_name_str = file_name.str();
      }
    }

    void
    probe_impl::read_info(pmt::pmt_t frame_info)
    { 
      struct timeval t;
      if(_develop_mode == 2)
      { 
        gettimeofday(&t, NULL);
        double current_time = t.tv_sec + t.tv_usec / 1000000.0;
        double current_time_show = t.tv_sec - double(int(t.tv_sec/100)*100) + t.tv_usec / 1000000.0;
        pmt::pmt_t time_info;
        time_info = pmt::from_double(current_time);
        message_port_pub(pmt::mp("E"), time_info);
        if(_cs_mode)
        {
          if(pmt::is_real(frame_info))
          {
            double power = pmt::to_double(frame_info);
            int cs_status;
            if(power > _cs_threshold)
            {
              cs_status = 1;
              std::cout << "rx power is " << power << ", received at " << current_time_show << " s, detection gap is " << current_time - _last_time << std::endl;
            }
            else
              cs_status = 0;
            if(_record_on)
            {
              if(_last_cs_status == 1 && cs_status == 0) 
              { 
                std::ofstream ofs (_file_name_str.c_str(), std::ofstream::app);
                ofs << t.tv_sec << " " << t.tv_usec << "\n";
                ofs.close();
              }
              else if(_last_cs_status == 0 && cs_status == 1)
              { 
                std::ofstream ofs (_file_name_str.c_str(), std::ofstream::app);
                ofs << t.tv_sec << " " << t.tv_usec << " ";
                ofs.close();
              }
            }
            _last_time = current_time;
            _last_cs_status = cs_status;
          }
          else
            std::cout << "probe ID " << _block_id << " error: not valid power signal" << std::endl;
        }
        else
        {
          std::cout << "++++ probe ID: " << _block_id << " receives a frame at time " << current_time_show << "s ++++" << std::endl;   
          if(_record_on)
          {
            std::ofstream ofs (_file_name_str.c_str(), std::ofstream::app);
            ofs << t.tv_sec << " " << t.tv_usec << "\n";
            ofs.close();
          }
        }
      } 
      else
      {
        if(_cs_mode)
        {
          if(pmt::is_real(frame_info))
          {
            double power = pmt::to_double(frame_info);
            gettimeofday(&t, NULL);
            double current_time_show = t.tv_sec - double(int(t.tv_sec/10)*10) + t.tv_usec / 1000000.0;
            double current_time = t.tv_sec + t.tv_usec / 1000000.0;
            if(power > _cs_threshold)
            {
              std::cout << "rx power is " << power << ", received at " << current_time_show << " s, detection gap is " << current_time - _last_time << std::endl;
            }
            if(_record_on)
            {
              if(power > _cs_threshold)
              {
                if(_last_power.size() > 0)
                {
                  for(int i = 0; i < _last_power.size(); i++)
                  {
                  std::ofstream ofs (_file_name_str.c_str(), std::ofstream::app);
                    ofs << _last_power.front() << " " << _last_tv_sec.front() << " " << _last_tv_usec.front() << "\n";
                  ofs.close();
                    _last_power.pop();
                    _last_tv_sec.pop();
                    _last_tv_usec.pop();
                  }
                  std::cout << "size of queue is: " << _last_power.size() << std::endl;
                }
                std::ofstream ofs (_file_name_str.c_str(), std::ofstream::app);
                ofs << power << " " << t.tv_sec << " " << t.tv_usec << "\n";
                ofs.close();
                _temp_n = 15;
              }
              else
              {
                if(_temp_n > 0)
                {
                  _temp_n--;
                  std::ofstream ofs (_file_name_str.c_str(), std::ofstream::app);
                  ofs << power << " " << t.tv_sec << " " << t.tv_usec << "\n";
                  ofs.close();
                } 
                _last_power.push(power);
                _last_tv_sec.push(t.tv_sec);
                _last_tv_usec.push(t.tv_usec);
                if(_last_power.size() > 1)
                {
                  _last_power.pop();
                  _last_tv_sec.pop();
                  _last_tv_usec.pop();
                }
              }
            }
            else
            {
            }
            _last_time = current_time;
          }
          else
            std::cout << "carrier_sensing ID " << _block_id << " error: not valid power signal" << std::endl;
        }
        else if(!_cs_mode && pmt::is_bool(frame_info))
        {
          std::cout << "+++++++++ probe ID: " << _block_id << " +++++++++";    
          std::cout << "receives boolean " << pmt::to_bool(frame_info) << std::endl;
        }
        else if(!_cs_mode && pmt::is_integer(frame_info))
        {
          std::cout << "+++++++++ probe ID: " << _block_id << " +++++++++";    
          std::cout << "receives number " << pmt::to_long(frame_info) << std::endl;
        }
        else if(!_cs_mode && pmt::is_dict(frame_info))
        {
          std::cout << "+++++++++ probe ID: " << _block_id << " +++++++++";    
          pmt::pmt_t not_found;
          int find_frame = 0;
          print_time();
          int frame_type = 0;
          if(pmt::dict_has_key(frame_info, pmt::string_to_symbol("frame_type")))
          {
            frame_type = pmt::to_long(pmt::dict_ref(frame_info, pmt::string_to_symbol("frame_type"), not_found));
            if(frame_type == 1)
              std::cout << " ---- data frame detected ---- ";
            else if(frame_type == 2)
              std::cout << " ---- ack frame detected ---- ";
            else if(frame_type == 3)
              std::cout << " ---- beacon frame detected ---- ";
            else if(frame_type == 4)
              std::cout << " ---- rts frame detected ---- ";
            else if(frame_type == 5)
              std::cout << " ---- cts frame detected ---- ";
            else if(frame_type == 6)
              std::cout << " ---- ampdu frame detected ---- ";
            else if(frame_type == 7)
              std::cout << " ---- amsdu frame detected ---- ";
            else if(frame_type == 8)
              std::cout << " ---- ampdu subframe detected ---- ";
            else if(frame_type == 9)
              std::cout << " ---- amsdu subframe detected ---- ";
            else
              std::cout << "Unknown frame type" << std::endl;
            // show detail of DATA, ACK, BEACON, RTS, CTS
            if(frame_type <= 5 && frame_type > 0)
            {
              find_frame = 1;
              show_detail(frame_info);
            }
            else if(frame_type == 6)
            { 
              find_frame = 1;
              show_detail(frame_info);
            }
            else if(frame_type == 8)
            { 
              find_frame = 1;
              show_detail(frame_info);
            }
          }
          if(pmt::dict_has_key(frame_info, pmt::string_to_symbol("slot_time")))
          {
            find_frame = 1;
            std::cout << "tdma scheduling info detected. " << std::endl;
            if(!pmt::is_integer(pmt::dict_ref(frame_info, pmt::string_to_symbol("destination_address"), not_found)))
            {
              std::vector<uint32_t> node_id_list = pmt::u32vector_elements(pmt::dict_ref(frame_info, pmt::string_to_symbol("node_id"), not_found)); 
              std::vector<uint32_t> slot_time_list = pmt::u32vector_elements(pmt::dict_ref(frame_info, pmt::string_to_symbol("slot_time"), not_found)); 
              for(int i = 0; i < node_id_list.size(); i++)
              {
                std::cout << "time slot of node " << node_id_list[i] << " is " << slot_time_list[i] << " [ms]" << std::endl;
              }
            }
            else
            {
              int node_id = pmt::to_long(pmt::dict_ref(frame_info, pmt::string_to_symbol("destination_address"), not_found)); 
              double slot_time = pmt::to_double(pmt::dict_ref(frame_info, pmt::string_to_symbol("slot_time"), not_found)); 
              int address_check = pmt::to_double(pmt::dict_ref(frame_info, pmt::string_to_symbol("address_check"), not_found)); 
              std::cout << "time slot of node " << node_id << " is " << slot_time << " [ms]" << " and address check";
              if(address_check)
                std::cout << " passed" << std::endl;
              else
                std::cout << " failed" << std::endl;
            }
          }
          // show detail of ampdu subframe from the transmitter side
          if(pmt::dict_has_key(frame_info, pmt::string_to_symbol("mpdu_info")))
          {
            find_frame = 1;
            pmt::pmt_t mpdu_info = pmt::dict_ref(frame_info, pmt::string_to_symbol("mpdu_info"), not_found);
            show_detail(mpdu_info);
            if(pmt::dict_has_key(frame_info, pmt::string_to_symbol("subframe_pmt")))
            {
              pmt::pmt_t subframe_pmt = pmt::dict_ref(frame_info, pmt::string_to_symbol("subframe_pmt"), not_found);
              std::vector<unsigned char> frame_array = pmt::u8vector_elements(pmt::cdr(subframe_pmt));
              std::cout << "subframe info contains a subframe with length " << frame_array.size() << " bytes: ";
              if(_print_frame) 
                disp_vec(frame_array);
            }
          }
          // pure number
          if(pmt::dict_has_key(frame_info, pmt::string_to_symbol("beacon_address_check")))
          {
            pmt::pmt_t address_check_pmt = pmt::dict_ref(frame_info, pmt::string_to_symbol("beacon_address_check"), not_found);
            std::cout << " input pmt is: " << pmt::to_long(address_check_pmt) << std::endl;
            find_frame = 1;
          }
          // show pure vector pmt
          if(pmt::is_u8vector(pmt::cdr(frame_info)))
          {
            find_frame = 1;
            std::vector<unsigned char> array = pmt::u8vector_elements(pmt::cdr(frame_info));
            std::cout << "Input mac frame has length: " << array.size() << std::endl;
            if(_print_frame) 
              disp_vec(array);
          }  
          if(pmt::dict_has_key(frame_info, pmt::string_to_symbol("time_stamp")))
          {
            find_frame = 1;
            double time = pmt::to_double(pmt::dict_ref(frame_info, pmt::string_to_symbol("time_stamp"), not_found));
            std::cout << "input time_stamp is: " << time - double(int(time/100)*100);
          }
          if(find_frame == 0)
            std::cout << "++++ probe ID: " << _block_id << " error. Unknow frame type. Please check your connections." << std::endl;
          std::cout << std::endl;
        }
        else
          std::cout << "++++ probe ID: " << _block_id << " error. Input is not a frame_info structure or carrier sensing reading. Please check your connections." << std::endl;
      }
    }
  
    void
    probe_impl::show_detail(pmt::pmt_t frame_info)
    {
      pmt::pmt_t not_found;
      pmt::pmt_t frame_pmt;
      int frame_type = pmt::to_long(pmt::dict_ref(frame_info, pmt::string_to_symbol("frame_type"), not_found));
      if(pmt::dict_has_key(frame_info, pmt::string_to_symbol("frame_index")))
        std::cout << "frame index is:            " << pmt::dict_ref(frame_info, pmt::string_to_symbol("frame_index"), not_found) << ";    ";
      if(pmt::dict_has_key(frame_info, pmt::string_to_symbol("destination_address")))
        std::cout << "destination address is:    " << pmt::dict_ref(frame_info, pmt::string_to_symbol("destination_address"), not_found) << ";    ";
      if(pmt::dict_has_key(frame_info, pmt::string_to_symbol("source_address")))
        std::cout << "source address is:         " << pmt::dict_ref(frame_info, pmt::string_to_symbol("source_address"), not_found) << ";    ";
      if(pmt::dict_has_key(frame_info, pmt::string_to_symbol("num_transmission")))
        std::cout << "number of transmission is: " << pmt::dict_ref(frame_info, pmt::string_to_symbol("num_transmission"), not_found) << ";    ";
      if(pmt::dict_has_key(frame_info, pmt::string_to_symbol("reserved_field_I")))
        std::cout << "reserved field I is:       " << pmt::dict_ref(frame_info, pmt::string_to_symbol("reserved_field_I"), not_found) << ";    ";
      if(pmt::dict_has_key(frame_info, pmt::string_to_symbol("reserved_field_II")))
        std::cout << "reserved field II is:      " << pmt::dict_ref(frame_info, pmt::string_to_symbol("reserved_field_II"), not_found) << ";    ";
      if(pmt::dict_has_key(frame_info, pmt::string_to_symbol("payload_length")))
        std::cout << "payload length is:           " << pmt::dict_ref(frame_info, pmt::string_to_symbol("payload_length"), not_found) << ";    ";
      if(pmt::dict_has_key(frame_info, pmt::string_to_symbol("header_length")))
        std::cout << "header length is:          " << pmt::dict_ref(frame_info, pmt::string_to_symbol("header_length"), not_found) << ";    ";
      if(frame_type == 4 || frame_type == 5)
        std::cout << "nav time      is:          " << pmt::dict_ref(frame_info, pmt::string_to_symbol("nav_time"), not_found) << ";    ";
     
      if(pmt::dict_has_key(frame_info, pmt::string_to_symbol("address_check")))
        std::cout << "address check is:          " << pmt::dict_ref(frame_info, pmt::string_to_symbol("address_check"), not_found) << ";    ";
      if(pmt::dict_has_key(frame_info, pmt::string_to_symbol("good_frame")))
        std::cout << "good frame is:             " << pmt::dict_ref(frame_info, pmt::string_to_symbol("good_frame"), not_found) << ";    ";
      if(pmt::dict_has_key(frame_info, pmt::string_to_symbol("analysis_time")))
        std::cout << "analysis time is:             " << pmt::dict_ref(frame_info, pmt::string_to_symbol("analysis_time"), not_found) << ";    ";
      if(pmt::dict_has_key(frame_info, pmt::string_to_symbol("frame_pmt")))
        frame_pmt = pmt::dict_ref(frame_info, pmt::string_to_symbol("frame_pmt"), not_found);
      if(pmt::dict_has_key(frame_info, pmt::string_to_symbol("subframe_pmt")))
        frame_pmt = pmt::dict_ref(frame_info, pmt::string_to_symbol("subframe_pmt"), not_found);
      if(pmt::dict_has_key(frame_info, pmt::string_to_symbol("ampdu_frame_pmt")))
        frame_pmt = pmt::dict_ref(frame_info, pmt::string_to_symbol("ampdu_frame_pmt"), not_found);
      if((pmt::dict_has_key(frame_info, pmt::string_to_symbol("frame_pmt"))) || (pmt::dict_has_key(frame_info, pmt::string_to_symbol("subframe_pmt"))) || (pmt::dict_has_key(frame_info, pmt::string_to_symbol("ampdu_frame_pmt"))))
      {
        std::vector<unsigned char> frame_array = pmt::u8vector_elements(pmt::cdr(frame_pmt));
        std::cout << "frame info contains a frame with length " << frame_array.size() << " bytes: ";
        if(_print_frame) 
          disp_vec(frame_array);
      }
    }

    void 
    probe_impl::disp_int_vec(std::vector<uint32_t> vec)
    {
      for(int i=0; i<vec.size(); ++i)
      {
        std::cout << vec[i] << ' ';
      }
      std::cout << ". total length is :" << vec.size() << "." << std::endl;
    }
 
    void 
    probe_impl::disp_vec(std::vector<unsigned char> vec)
    {
      for(int i=0; i<vec.size(); ++i)
        std::cout << static_cast<unsigned>(vec[i]) << ' ';
      std::cout << ". total length is: " << vec.size() << std::endl;
    }
 
    void
    probe_impl::print_time()
    {
      if(_develop_mode == 2)
      {
        struct timeval t; 
        gettimeofday(&t, NULL);
        double current_time_show = t.tv_sec - double(int(t.tv_sec/100)*100) + t.tv_usec / 1000000.0;
        std::cout << " at " << _block_id <<  current_time_show << "s " << std::endl;
      }
    }

  } /* namespace dmdl */
} /* namespace gr */

