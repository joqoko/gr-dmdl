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
#include "timeout_impl.h"

namespace gr {
  namespace dmdl {

    timeout::sptr
    timeout::make(int develop_mode, int block_id, float timeout_duration_ms, int system_time_granularity_us, int llc_protocol)
    {
      return gnuradio::get_initial_sptr
        (new timeout_impl(develop_mode, block_id, timeout_duration_ms, system_time_granularity_us, llc_protocol));
    }

    /*
     * The private constructor
     */
    timeout_impl::timeout_impl(int develop_mode, int block_id, float timeout_duration_ms, int system_time_granularity_us, int llc_protocol)
      : gr::block("timeout",
              gr::io_signature::make(0, 0, 0),
              gr::io_signature::make(0, 0, 0)),
        _develop_mode(develop_mode),
        _block_id(block_id),
        _timeout_duration_ms(float(timeout_duration_ms)),
        _in_timeout(false),
        _system_time_granularity_us(system_time_granularity_us),
        _llc_protocol(llc_protocol)
    {
      if(_develop_mode)
        std::cout << "develop_mode of timeout ID: " << _block_id << " is activated." << std::endl;
      struct timeval t;
      gettimeofday(&t, NULL);
      _start_time = t.tv_sec + t.tv_usec / 1000000.0;
      message_port_register_out(pmt::mp("E"));
      message_port_register_in(pmt::mp("B"));
      message_port_register_in(pmt::mp("I"));
      set_msg_handler(
        pmt::mp("B"),
        boost::bind(&timeout_impl::start_timeout, this, _1)
      );
      set_msg_handler(
        pmt::mp("I"),
        boost::bind(&timeout_impl::kill_timeout, this, _1)
      );
    }

    /*
     * Our virtual destructor.
     */
    timeout_impl::~timeout_impl()
    {
    }

    void timeout_impl::kill_timeout(pmt::pmt_t ack_frame_info) 
    {
      if(_develop_mode)
        std::cout << "+++  timeout ID: " << _block_id << " kill timeout  +++";
      print_time();
      if(_develop_mode == 2)
      {
        struct timeval t; 
        gettimeofday(&t, NULL);
        double current_time = t.tv_sec - double(int(t.tv_sec/10000)*10000) + t.tv_usec / 1000000.0;
        std::cout << "* timeout ID: " << _block_id << " start killing the timer at time " << current_time << " s" << std::endl;
      }
      // stop and wait arq
      if(_llc_protocol == 0)
      {
        if(pmt::is_dict(ack_frame_info))
        {
          pmt::pmt_t not_found;
          // ack frame info
          int frame_type = pmt::to_long(pmt::dict_ref(ack_frame_info, pmt::string_to_symbol("frame_type"), not_found));
          int ack_dest = pmt::to_long(pmt::dict_ref(ack_frame_info, pmt::string_to_symbol("destination_address"), not_found));
          int ack_src = pmt::to_long(pmt::dict_ref(ack_frame_info, pmt::string_to_symbol("source_address"), not_found));
          int ack_index = pmt::to_long(pmt::dict_ref(ack_frame_info, pmt::string_to_symbol("frame_index"), not_found));
          // waiting frame info
          int wait_dest = pmt::to_long(pmt::dict_ref(_waiting_frame_info, pmt::string_to_symbol("destination_address"), not_found));
          int wait_src = pmt::to_long(pmt::dict_ref(_waiting_frame_info, pmt::string_to_symbol("source_address"), not_found));
          int wait_index = pmt::to_long(pmt::dict_ref(_waiting_frame_info, pmt::string_to_symbol("frame_index"), not_found));
          if(_in_timeout)
          {
            //std::cout << "frame type: " << frame_type << " ack_dest: " << ack_dest << " wait_src: " << wait_src << " ack_src: " << ack_src << "wait_dest: " << wait_dest << " ack_index: " << ack_index << " wait_index: " << wait_index << std::endl;
            if((frame_type == 2 || frame_type == 5) && (ack_dest == wait_src) && (ack_src == wait_dest) && (ack_index == wait_index))
            { 
              double gen_time = pmt::to_double(pmt::dict_ref(_waiting_frame_info, pmt::string_to_symbol("generation_time"), not_found));
              int num_transmission = pmt::to_double(pmt::dict_ref(_waiting_frame_info, pmt::string_to_symbol("num_transmission"), not_found));
              ack_frame_info = pmt::dict_add(ack_frame_info, pmt::string_to_symbol("generation_time"),pmt::from_double(gen_time));
              ack_frame_info = pmt::dict_add(ack_frame_info, pmt::string_to_symbol("num_transmission"),pmt::from_long(num_transmission));
              _in_timeout = false;
              message_port_pub(pmt::mp("E"), ack_frame_info);
              if(_develop_mode)
                std::cout << "timeout is terminated by correctly received ack frame." << std::endl;
            }
            else if(frame_type != 2 && frame_type != 5)
              if(_develop_mode)
                std::cout << "timeout ID " << _block_id << "error: not an ack frame or rts frame." << std::endl;
            else if((ack_dest != wait_src) && (ack_src != wait_dest))
              if(_develop_mode)
                std::cout << "timeout ID " << _block_id << "address not correct." << std::endl;
            else
              if(_develop_mode)
                std::cout << "timeout ID " << _block_id << "expecting the ack/cts frame of the " << wait_index << "th frame but received the ack/cts frame the " << ack_index << "th data/rts frame." << std::endl;
          }
          else
          {
            if(_develop_mode)
              std::cout << "timeout ID " << _block_id << " received a frame out of timeout interval." << std::endl;
          }
        }
        else
          std::cout << "timeout ID " << _block_id << " error: wrong data type. please check your connection." << std::endl;
      }
      // go-back-n arq 
      if(_llc_protocol == 1)
      {
        if(pmt::is_dict(ack_frame_info))
        {
          pmt::pmt_t not_found;
          // ack frame info
          int frame_type = pmt::to_long(pmt::dict_ref(ack_frame_info, pmt::string_to_symbol("frame_type"), not_found));
          int ack_dest = pmt::to_long(pmt::dict_ref(ack_frame_info, pmt::string_to_symbol("destination_address"), not_found));
          int ack_src = pmt::to_long(pmt::dict_ref(ack_frame_info, pmt::string_to_symbol("source_address"), not_found));
          int ack_index = pmt::to_long(pmt::dict_ref(ack_frame_info, pmt::string_to_symbol("frame_index"), not_found));
          // waiting frame info
          int wait_dest = pmt::to_long(pmt::dict_ref(_window.front(), pmt::string_to_symbol("destination_address"), not_found));
          int wait_src = pmt::to_long(pmt::dict_ref(_window.front(), pmt::string_to_symbol("source_address"), not_found));
          int wait_index = pmt::to_long(pmt::dict_ref(_window.front(), pmt::string_to_symbol("frame_index"), not_found));
          if(_in_timeout)
          {
            if(_develop_mode)
              std::cout << "frame type: " << frame_type << " ack_dest: " << ack_dest << " wait_src: " << wait_src << " ack_src: " << ack_src << "wait_dest: " << wait_dest << " ack_index: " << ack_index << " wait_index: " << wait_index << std::endl;
            if((frame_type == 2) && (ack_dest == wait_src) && (ack_src == wait_dest) && (ack_index >= wait_index))
            { 
              message_port_pub(pmt::mp("E"), ack_frame_info);
              if(_window.size() - (ack_index - wait_index) == 1)
              {
                _in_timeout = false;
              }
              else
              {
                if(_develop_mode)
                  std::cout << "frame from " << ack_src << " in window is acked with frame index: " << ack_index << std::endl;
                  
              }
              for(int i = 0; i < ack_index - wait_index + 1; i++)
                _window.pop(); 
            }
            else if(frame_type != 2)
              if(_develop_mode)
                std::cout << "Not an ack_frame_info dict." << std::endl;
            else if((ack_dest != wait_src) && (ack_src != wait_dest))
              if(_develop_mode)
                std::cout << "address not correct." << std::endl;
            else
              if(_develop_mode)
                std::cout << "expecting the ack of the " << wait_index << "th frame but received the ack of the " << ack_index << "th frame." << std::endl;
          }
          else
          {
            if(_develop_mode)
              std::cout << "Receive a pmt dict out of timeout interval." << std::endl;
          }
        }
        else
          std::cout << "ack_frame_info: wrong data type. please check your connection." << std::endl;
      }
      // selective repeat arq 
      if(_llc_protocol == 2)
      {
         
      }
    }

    void timeout_impl::start_timeout(pmt::pmt_t data_frame_info) 
    {
      if(_develop_mode)
        std::cout << "+++ timeout ID: " << _block_id << " start timeout +++";
      print_time();
      if(_llc_protocol == 0)
      {
        if(pmt::is_dict(data_frame_info))
        {
          if(!_in_timeout)
          {
            pmt::pmt_t not_found;
            int data_type = pmt::to_long(pmt::dict_ref(data_frame_info, pmt::string_to_symbol("frame_type"), not_found));
            if(data_type == 1 || data_type == 4)
            {
//              if(_develop_mode)
 //             {
 //               struct timeval t; 
 //               gettimeofday(&t, NULL);
 //               double current_time = t.tv_sec - double(int(t.tv_sec/100)*100) + t.tv_usec / 1000000.0;
 //               std::cout << "* timeout ID: " << _block_id << " timeout timer is triggered at time " << current_time << " s" << std::endl;
 //             }
              _in_timeout = true;
              _waiting_frame_info = data_frame_info;
              // std::cout << "When timeout is started, the index is: " << pmt::to_long(pmt::dict_ref(_waiting_frame_info, pmt::string_to_symbol("frame_index"), not_found)) << std::endl;
              boost::thread thrd(&timeout_impl::countdown_timeout, this);       
            }
            else
              std::cout << "timeout ID " << _block_id << " error: not a data frame or rts frame. frame type is: " << data_type << std::endl;
          }
          else
            std::cout << "timeout ID " << _block_id << " error: cannot trigger the timeout timer before finishing the last one." << std::endl;
        }
        else
          std::cout << "timeout ID " << _block_id << ". error: wrong data type. please check your connection." << std::endl;
      }
      // go-back-n
      else if(_llc_protocol == 1)
      {
        if(pmt::is_dict(data_frame_info))
        {
          pmt::pmt_t not_found;
          if(!_in_timeout)
          {
            int data_type = pmt::to_long(pmt::dict_ref(data_frame_info, pmt::string_to_symbol("frame_type"), not_found));
            if(data_type == 1)
            {
 //             if(_develop_mode)
 //             {
 //               struct timeval t; 
 //               gettimeofday(&t, NULL);
 //               double current_time = t.tv_sec - double(int(t.tv_sec/10000)*10000) + t.tv_usec / 1000000.0;
 //               std::cout << "* timeout ID: " << _block_id << " timeout timer is triggered at time " << current_time << " s" << std::endl;
 //             }
              _in_timeout = true;
              _window.push(data_frame_info);
              if(_develop_mode)
                std::cout << "after pushing the first frame, window size is: " << _window.size() << std::endl;
              // std::cout << "When timeout is started, the index is: " << pmt::to_long(pmt::dict_ref(_waiting_frame_info, pmt::string_to_symbol("frame_index"), not_found)) << std::endl;
              boost::thread thrd(&timeout_impl::countdown_timeout, this);       
            }
            else
              std::cout << "timeout ID " << _block_id << " error: llc protocol only support data frames." << std::endl;
          }
          else
          {
            int data_type = pmt::to_long(pmt::dict_ref(data_frame_info, pmt::string_to_symbol("frame_type"), not_found));
            if(data_type == 1)
            {
              int frame_index = pmt::to_long(pmt::dict_ref(data_frame_info, pmt::string_to_symbol("frame_index"), not_found));

              //if(_window.size() > 0 && (frame_index == (pmt::to_long(pmt::dict_ref(_window.back(), pmt::string_to_symbol("frame_index"), not_found)) + 1)) || frame_index == (pmt::to_long(pmt::dict_ref(_window.back(), pmt::string_to_symbol("frame_index"), not_found)) + 256))
              //{
                reset_timeout();
                _window.push(data_frame_info);
                if(_develop_mode)
                {
                  struct timeval t; 
                  gettimeofday(&t, NULL);
                  double current_time = t.tv_sec - double(int(t.tv_sec/100)*100) + t.tv_usec / 1000000.0;
                  std::cout << "* timeout ID: " << _block_id << " timeout timer is reset at time " << current_time << " s" << std::endl;
                }
              //}
              //else
              //  std::cout << "go-back-n timeout cannot receive discontinued data frame, current window size is: " << _window.size() << " current frame index is: " << frame_index << " and the expected one is: " << pmt::to_long(pmt::dict_ref(_window.back(), pmt::string_to_symbol("frame_index"), not_found)) + 1 << ". please check your connections." << std::endl;
            }
          }
        }
        else
          std::cout << "data_frame_info: wrong data type. please check your connection." << std::endl;
      }
      // selective repeat
      else if(_llc_protocol == 2)
      {
      }
    }
 
    void timeout_impl::reset_timeout()
    {
      struct timeval t;
      gettimeofday(&t, NULL);
      _start_time = t.tv_sec + t.tv_usec / 1000000.0;
    }

    void timeout_impl::countdown_timeout()
    {
      struct timeval t;
      gettimeofday(&t, NULL);
      double current_time = t.tv_sec + t.tv_usec / 1000000.0;
      double start_time_show = t.tv_sec - double(int(t.tv_sec/100)*100) + t.tv_usec / 1000000.0;
      _start_time = current_time;
      double current_time_show = start_time_show;
      if(_develop_mode)
        std::cout << "timeout timer start time: " << start_time_show << std::endl;
      while((current_time < _start_time + _timeout_duration_ms / 1000) && _in_timeout)
      {
        gettimeofday(&t, NULL);
        current_time = t.tv_sec + t.tv_usec / 1000000.0;
        boost::this_thread::sleep(boost::posix_time::microseconds(_system_time_granularity_us));
        current_time_show = t.tv_sec - double(int(t.tv_sec/100)*100) + t.tv_usec / 1000000.0;
        // std::cout << "timeout is running at: " << current_time_show << std::endl;
      }
      if(_develop_mode)
      {
        gettimeofday(&t, NULL);
        double current_time_show = t.tv_sec - double(int(t.tv_sec/100)*100) + t.tv_usec / 1000000.0;
        if(_in_timeout)
        std::cout << "* timeout ID: " << _block_id << " timeout timer is expired at time " << current_time_show << " s. " << " timeout duration is: " << _timeout_duration_ms << " [ms]" << std::endl;
        else
        std::cout << "* timeout ID: " << _block_id << " timeout timer is killed  at time " << current_time_show << " s. " << " actual timeout duration is: " << current_time_show - start_time_show << " s" << std::endl;
      }
      if(_in_timeout)
      {
        // stop and wait
        if(_llc_protocol == 0)
          message_port_pub(pmt::mp("E"), _waiting_frame_info);
        else if(_llc_protocol == 1)
        {
          message_port_pub(pmt::mp("E"), _window.front());
          flush_window();
        }
        else if(_llc_protocol == 2)
        {
          message_port_pub(pmt::mp("E"), _window.front());
          flush_window();
        }
          
      }
      _in_timeout = false;
    }
  
    void timeout_impl::flush_window()
    {
      int flush_length = _window.size();
      if(flush_length > 0)
      {
        for(int i = 0; i < flush_length; i++)
        {
          _window.pop();
        }
        if(_develop_mode)
          std::cout << "timeout ID: " << _block_id << " has " << _window.size() << " frames in the window after flush." << std::endl;
      }
    }  
 
    void
    timeout_impl::print_time()
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

