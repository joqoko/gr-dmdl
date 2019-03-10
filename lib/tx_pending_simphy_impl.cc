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
#include <gnuradio/io_signature.h>
#include "tx_pending_simphy_impl.h"

namespace gr {
  namespace dmdl {

    tx_pending_simphy::sptr
    tx_pending_simphy::make(int develop_mode, int block_id, int system_time_granularity_us, float sample_rate, const std::string &lengthtagname, double interframe_interval_s, int record_on, std::string file_name_extension, int name_with_timestamp)
    {
      return gnuradio::get_initial_sptr
        (new tx_pending_simphy_impl(develop_mode, block_id, system_time_granularity_us, sample_rate, lengthtagname, interframe_interval_s, record_on, file_name_extension, name_with_timestamp));
    }

    /*
     * The private constructor
     */
    tx_pending_simphy_impl::tx_pending_simphy_impl(int develop_mode, int block_id, int system_time_granularity_us, float sample_rate, const std::string &lengthtagname, double interframe_interval_s, int record_on, std::string file_name_extension, int name_with_timestamp)
      : gr::sync_block("tx_pending_simphy",
              gr::io_signature::make(1, 1, sizeof(gr_complex)),
              gr::io_signature::make(0, 0, 0)),
        _sample_rate(sample_rate),
        _d_lengthtagname(pmt::string_to_symbol(lengthtagname)),
        _develop_mode(develop_mode),
        _block_id(block_id),
        _system_time_granularity_us(system_time_granularity_us),
        _countdown_bias_s(0),
        _record_on(record_on),
        _file_name_extension(file_name_extension),
        _name_with_timestamp(name_with_timestamp),
        _interframe_interval_s(interframe_interval_s)
    {
      if(_develop_mode == 1)
        std::cout << "develop_mode of tx_pending_simphy ID: " << _block_id << " is activated." << std::endl;
      _wait_time = 0;
      message_port_register_in(pmt::mp("B"));
      set_msg_handler(pmt::mp("B"), boost::bind(&tx_pending_simphy_impl::buffer_tx_frame_info, this, _1 ));
      message_port_register_out(pmt::mp("DATA"));
      message_port_register_out(pmt::mp("ACK"));
      message_port_register_out(pmt::mp("BEACON"));
      message_port_register_out(pmt::mp("RTS"));
      message_port_register_out(pmt::mp("CTS"));
      message_port_register_out(pmt::mp("AMPDU"));
      message_port_register_out(pmt::mp("AMSDU"));
      message_port_register_out(pmt::mp("UKN"));
      message_port_register_out(pmt::mp("RXO"));

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
    tx_pending_simphy_impl::~tx_pending_simphy_impl()
    {
    }

    int
    tx_pending_simphy_impl::work(int noutput_items,
        gr_vector_const_void_star &input_items,
        gr_vector_void_star &output_items)
    {
      const gr_complex *in = (const gr_complex *) input_items[0];

      std::vector <tag_t> tags;
      get_tags_in_range(tags, 0, nitems_read(0), nitems_read(0) + noutput_items);// + packet_length);
      // If tag(s) is detected, we need to wait then send the spark signal.
      if(process_tags_info(tags))
      { 
       if(_record_on)
       {
          struct timeval t;
          gettimeofday(&t, NULL);
          std::ofstream ofs (_file_name_str.c_str(), std::ofstream::app);
          ofs << _wait_time << "\n";
          ofs.close();
        }
        boost::thread thrd(&tx_pending_simphy_impl::countdown_waiting, this);
      }

      // Tell runtime system how many output items we produced.
      return noutput_items;
    }

    void
    tx_pending_simphy_impl::buffer_tx_frame_info(pmt::pmt_t tx_frame_info)
    {
      if(pmt::is_dict(tx_frame_info))
      {
        _tx_queue.push(tx_frame_info);
        if(_develop_mode)
          std::cout << "tx_frame_info received." << std::endl;
      }
      else
        std::cout << "tx_frame_info is not a dict. Please check your connection." << std::endl;
    }

    int
    tx_pending_simphy_impl::process_tags_info(std::vector <tag_t> tags)
    {
      if(tags.size() > 0)
      {
        for(int i = 0; i < tags.size(); i++)
        {
//          if(_develop_mode)
//          {
 //           std::cout << "Index of tags: " << i << std::endl;
   //         std::cout << "Offset: " << tags[i].offset << std::endl;
       //     std::cout << "Key: " << tags[i].key << std::endl;
     //       std::cout << "Value: " << tags[i].value << std::endl;
         //   std::cout << "Srcid: " << tags[i].srcid << std::endl;
          //}
          
          // std::cout << "string comapre: " << pmt::symbol_to_string(tags[i].key) << "packet_len" <<  (pmt::symbol_to_string(tags[i].key) == "packet_len") << std::endl;
          if(pmt::symbol_to_string(tags[i].key) == "packet_len")
          {
            if(_develop_mode)
            {
              std::cout << "++++  tx_pending_simphy ID: " << _block_id << " ";
            }
            _wait_time = pmt::to_double(tags[i].value) / _sample_rate;     
            if(_develop_mode == 2)
            {
              std::cout << " number of samples: " << pmt::to_double(tags[i].value) << " in " << _wait_time << "s. ";
            }
            break;
          }
        }
      }
      return tags.size();
    }

    void tx_pending_simphy_impl::countdown_waiting()
    {
      struct timeval t;
      gettimeofday(&t, NULL);
      double current_time = t.tv_sec + t.tv_usec / 1000000.0;
      double start_time = t.tv_sec + t.tv_usec / 1000000.0;
      double start_time_show = t.tv_sec - double(int(t.tv_sec/10)*10) + t.tv_usec / 1000000.0;
      if(_develop_mode == 2)
        std::cout << "pending is started at: " << start_time_show << "s. ";
      while(current_time < start_time + _wait_time + _interframe_interval_s)
      {
        boost::this_thread::sleep(boost::posix_time::microseconds(_system_time_granularity_us));
        gettimeofday(&t, NULL);
        current_time = t.tv_sec + t.tv_usec / 1000000.0;
      }
      if(_tx_queue.size() > 0)
      {
        gettimeofday(&t, NULL);
        current_time = t.tv_sec + t.tv_usec / 1000000.0;
        double current_time_show = t.tv_sec - double(int(t.tv_sec/10)*10) + t.tv_usec / 1000000.0;
        pmt::pmt_t tx_frame_info = _tx_queue.front();
        pmt::pmt_t not_found;
        int frame_type = pmt::to_long(pmt::dict_ref(tx_frame_info, pmt::string_to_symbol("frame_type"), not_found));
        
        if(frame_type == 1)
        {
          if(_develop_mode)
            std::cout << "frame type: data";
          message_port_pub(pmt::mp("DATA"), tx_frame_info);
        } 
        else if(frame_type == 2)
        {
          if(_develop_mode)
            std::cout << "frame type: ack";
          message_port_pub(pmt::mp("ACK"), tx_frame_info);
        } 
        else if(frame_type == 3)
        {
          if(_develop_mode)
            std::cout << "frame type: beacon";
          message_port_pub(pmt::mp("BEACON"), tx_frame_info);
        } 
        else if(frame_type == 4)
        {
          if(_develop_mode)
            std::cout << "frame type: rts";
          message_port_pub(pmt::mp("RTS"), tx_frame_info);
        } 
        else if(frame_type == 5)
        {
          if(_develop_mode)
            std::cout << "frame type: cts";
          message_port_pub(pmt::mp("CTS"), tx_frame_info);
        } 
        else if(frame_type == 6)
        {
          if(_develop_mode)
            std::cout << "frame type: ampdu";
          message_port_pub(pmt::mp("AMPDU"), tx_frame_info);
        } 
        else if(frame_type == 7)
        {
          if(_develop_mode)
            std::cout << "frame type: amsdu";
          message_port_pub(pmt::mp("AMSDU"), tx_frame_info);
        } 
        else
        {
          if(_develop_mode)
            std::cout << "frame type: unknown";
          message_port_pub(pmt::mp("UKN"), tx_frame_info);
        } 
        if(_develop_mode == 2)
          std::cout << ", starting tx time " << start_time_show << "s and finish time " << current_time_show << "s. the actual transmitting duration is " << current_time - start_time << "s" << std::endl; 
        _tx_queue.pop();
      } 
      else
        std::cout << "pending_tx: tx_queue is empty. " << std::endl;
      _wait_time = 0;
      message_port_pub(pmt::mp("RXO"), pmt::from_bool(true));
    }

  } /* namespace dmdl */
} /* namespace gr */

