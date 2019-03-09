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
#include "cmd_buffer_impl.h"
#include <queue>

namespace gr {
  namespace dmdl {

    cmd_buffer::sptr
    cmd_buffer::make(int develop_mode, int block_id, int buffer_size, int auto_dequeue_first, int keep_dequeue_state, int auto_dequeue_full)
    {
      return gnuradio::get_initial_sptr
        (new cmd_buffer_impl(develop_mode, block_id, buffer_size, auto_dequeue_first, keep_dequeue_state, auto_dequeue_full));
    }

    /*
     * The private constructor
     */
    cmd_buffer_impl::cmd_buffer_impl(int develop_mode, int block_id, int buffer_size, int auto_dequeue_first, int keep_dequeue_state, int auto_dequeue_full)
      : gr::block("cmd_buffer",
              gr::io_signature::make(0, 0, 0),
              gr::io_signature::make(0, 0, 0)),
        _develop_mode(develop_mode),
        _block_id(block_id),
        _buffer_size(buffer_size),
	_auto_dequeue_first(auto_dequeue_first),
	_keep_dequeue_state(keep_dequeue_state),
	_dequeue_when_available(0),
	_dequeue_first(1),
	_show_am_empty(1),
        _auto_dequeue_full(auto_dequeue_full)
  //      _output_U(output_U)
    {
      if(_develop_mode)
        std::cout << "develop_mode of buffer ID: " << _block_id << " is activated." << std::endl;
      message_port_register_in(pmt::mp("Q")); 
      set_msg_handler(pmt::mp("Q"), boost::bind(&cmd_buffer_impl::enqueue, this, _1));
      message_port_register_in(pmt::mp("IE")); 
      set_msg_handler(pmt::mp("IE"), boost::bind(&cmd_buffer_impl::indicate, this, _1));
      message_port_register_in(pmt::mp("D")); 
      set_msg_handler(pmt::mp("D"), boost::bind(&cmd_buffer_impl::dequeue, this, _1));
      message_port_register_in(pmt::mp("H")); 
      set_msg_handler(pmt::mp("H"), boost::bind(&cmd_buffer_impl::flush, this, _1));
      message_port_register_in(pmt::mp("G")); 
      set_msg_handler(pmt::mp("G"), boost::bind(&cmd_buffer_impl::reset_size, this, _1));
      message_port_register_out(pmt::mp("U"));
      message_port_register_out(pmt::mp("NF"));
      message_port_register_out(pmt::mp("L"));
      message_port_register_out(pmt::mp("M"));
    }

    /*
     * Our virtual destructor.
     */
    cmd_buffer_impl::~cmd_buffer_impl()
    {
    }

    void cmd_buffer_impl::reset_size(pmt::pmt_t cmd_in)
    {
      if(pmt::is_integer(cmd_in))
      {
        _buffer_size = pmt::to_long(cmd_in);
        if(_develop_mode)
          std::cout << "buffer_size of cmd_buffer block ID " << _block_id << " is reset to " << _buffer_size << std::endl;
      }
      else   
      {
        std::cout << "error: cmd_buffer block ID " << _block_id << " can only reassign buffer size to a integer number." << std::endl;
      }
    }
    
    void cmd_buffer_impl::indicate(pmt::pmt_t trigger)
    {
      if(_show_am_empty)
      {
        message_port_pub(pmt::mp("NF"), pmt::from_long(1));
        _show_am_empty = 0;
      }
      else
      {
        if(pmt::is_dict(trigger))
          message_port_pub(pmt::mp("NF"), pmt::from_long(1));
      }
    }
    
    void cmd_buffer_impl::enqueue(pmt::pmt_t enqueue_element)
    {
      if(_develop_mode)
        std::cout << "++++++++++++ buffer ID: " << _block_id << " enqueue ++++++++++" << std::endl;
      if(_buffer.size() < _buffer_size)
      {
        _buffer.push(enqueue_element);
        if(_buffer.size() < _buffer_size)
          message_port_pub(pmt::mp("NF"), pmt::from_long(1));
        else
        {
          message_port_pub(pmt::mp("L"), pmt::mp("buffer_is_full"));
          if(_auto_dequeue_full)
          {
            message_port_pub(pmt::mp("U"), _buffer.front());
            _buffer.pop();
          }
        }
        if(_develop_mode)
        {
          std::cout << "buffer ID: " << _block_id << " has " << _buffer.size() << " elements after enqueue, ";
          if(_buffer.size() < _buffer_size)
            std::cout << " and buffer is not full."  << std::endl;
          else
          {
            std::cout << " and buffer is full.";
            message_port_pub(pmt::mp("L"), pmt::from_long(1));
            if(_auto_dequeue_full)
              std::cout << " the first elements is dequeued." << std::endl;
          }
        }
        if(_develop_mode == 2)
        {
          struct timeval t; 
          gettimeofday(&t, NULL);
          double current_time = t.tv_sec - double(int(t.tv_sec/10000)*10000) + t.tv_usec / 1000000.0;
          std::cout << "buffer ID: " << _block_id << " enqueue at time " << current_time << " s" << std::endl;
        }
        /*
         * sometimes elements is dequeued immediately when enqueued. 
         * 1. the first element for the whole execution. Normally it is used as the 'first drive' of the protocol
         * 2. other parts of the protocol require to dequeue but no element is in the queue. Therefore new element is dequeued immediately to fit the demand.  
         */
        if((_dequeue_first && _auto_dequeue_first) || _dequeue_when_available)
        {
          message_port_pub(pmt::mp("U"), _buffer.front());
          _buffer.pop();
          if(_develop_mode == 2)
          {
            struct timeval t; 
            gettimeofday(&t, NULL);
            double current_time = t.tv_sec - double(int(t.tv_sec/10000)*10000) + t.tv_usec / 1000000.0;
            std::cout << "buffer ID: " << _block_id << " dequeue at time " << current_time << " s" <<  std::endl;
          }
          if(_dequeue_first && _auto_dequeue_first)
	  {
            if(_develop_mode)
              std::cout << "buffer ID: " << _block_id << " has " << _buffer.size() << " elements after auto-dequeue the first element." << std::endl;
            _dequeue_first = 0;
	  }
	  else if(_dequeue_when_available)
	  {
            if(_develop_mode)
              std::cout << "buffer ID: " << _block_id << " has " << _buffer.size() << " elements after dequeue (postponed dequeue request)." << std::endl;
	    _dequeue_when_available = 0;
	  }
        }
      }
      else
      {
        if(_develop_mode)
          std::cout << "buffer ID: " << _block_id << " is full. current element is discarded." << std::endl;
      //  message_port_pub(pmt::mp("U"), enqueue_element);
      }
    }
    
    void cmd_buffer_impl::dequeue(pmt::pmt_t dequeue_request)
    {
      if(_develop_mode)
        std::cout << "++++++++++++ buffer ID: " << _block_id << " dequeue ++++++++++" << std::endl;
      // real dequeue request only used for slide window arq which may need multiple frames at the same time. However, the state transition is quite complicated.
      if(pmt::is_real(dequeue_request))
      {
        int n_dequeue = pmt::to_long(dequeue_request);
        // if there are elements in the buffer
        if(_buffer.size() > 0)
        {
          // if the number of elements in the buffer is smaller than the number of requested elements, the number of dequeued elements should be forced to the number of elements in the buffer.
          if(n_dequeue > 0)
          {
            if(_buffer.size() < n_dequeue)
            {
              n_dequeue = _buffer.size();
              if(_develop_mode)
                std::cout << "Only " << n_dequeue << "element is the buffer, all of them are dequeued" << std::endl;
            }
            for(int i = 0; i < n_dequeue; i++)
            {
              message_port_pub(pmt::mp("U"), _buffer.front());
              _buffer.pop();
            }
            // after dequeueing all elements, output a pmt containing the number of frames as a conclusion.
            message_port_pub(pmt::mp("U"), pmt::from_long(n_dequeue));
            if(_develop_mode)
              std::cout << "buffer ID: " << _block_id << " has " << _buffer.size() << " elements after dequeue." << std::endl;
            if(_develop_mode == 2)
            {
              struct timeval t; 
              gettimeofday(&t, NULL);
              double current_time = t.tv_sec - double(int(t.tv_sec/10000)*10000) + t.tv_usec / 1000000.0;
              std::cout << "buffer ID: " << _block_id << " dequeue " << n_dequeue << " elements at time " << current_time << " s" << std::endl;
            }
          }
          else
          {
            // dequeue a integer -2 means that the go-back-n requests no frame (most probably all transmissions are failed). 
            message_port_pub(pmt::mp("U"), pmt::from_long(-2));
            if(_develop_mode)
              std::cout << "0 element is required and no element is dequeued." << std::endl;
          }
        }
        else
        {
          // dequeue a integer -1 means that the buffer is empty
          message_port_pub(pmt::mp("U"), pmt::from_long(-1));
          if(_develop_mode)
            std::cout << "buffer is empty so no element can be dequeued." << std::endl;
        }
      }
      else
      {
        if(_buffer.size() > 0)
        {
          message_port_pub(pmt::mp("U"), _buffer.front());
          _buffer.pop();
          if(_develop_mode)
            std::cout << "buffer ID: " << _block_id << " has " << _buffer.size() << " elements after dequeue." << std::endl;
          if(_develop_mode == 2)
          {
            struct timeval t; 
            gettimeofday(&t, NULL);
            double current_time = t.tv_sec - double(int(t.tv_sec/10000)*10000) + t.tv_usec / 1000000.0;
            std::cout << "buffer ID: " << _block_id << " dequeue at time " << current_time << " s" << std::endl;
          }
//          if(_buffer.size() == 0)
//            message_port_pub(pmt::mp("M"), pmt::from_long(0));
        }
        else
        {
          if(_keep_dequeue_state)
          {
            _dequeue_when_available = 1;
            if(_develop_mode)
              std::cout << "buffer ID: " << _block_id << " has " << _buffer.size() << " elements. dequeue will be executed after next enqueue." << std::endl;
          }
          else
          {
            if(_develop_mode)
              std::cout << "buffer ID: " << _block_id << " is empty. no element is popped." << std::endl;
          }
          message_port_pub(pmt::mp("M"), pmt::make_dict());
        }
      }
    }

/*
    void cmd_buffer_impl::preview(pmt::pmt_t preview_request)
    {
      if(_develop_mode == 1)
        std::cout << "++++++++++++ buffer ID: " << _block_id << " preview ++++++++++" << std::endl;
      if(_buffer.size() > 0)
      {
        message_port_pub(pmt::mp("preview_element"), _buffer.front());
        if(_develop_mode == 1)
          std::cout << "buffer ID: " << _block_id << " has " << _buffer.size() << " elements after preview." << std::endl;
      }
      else
        if(_develop_mode == 1)
          std::cout << "buffer ID: " << _block_id << " is empty. no element is previewed." << std::endl;
    }
*/
    void cmd_buffer_impl::flush(pmt::pmt_t flush)
    {
      if(_develop_mode)
      {
        std::cout << "++++++++++++ buffer ID: " << _block_id << " flush ++++++++++++" << std::endl;
        std::cout << "buffer ID: " << _block_id << " has " << _buffer.size() << " elements before flush." << std::endl;
      }
      int flush_length = _buffer.size();
      if(flush_length > 0)
      {
        for(int i = 0; i < flush_length; i++)
        {
//          message_port_pub(pmt::mp("U"), _buffer.front());
          _buffer.pop();
        }
        if(_develop_mode)
          std::cout << "buffer ID: " << _block_id << " has " << _buffer.size() << " elements after flush." << std::endl;
        if(_develop_mode == 2)
        {
          struct timeval t; 
          gettimeofday(&t, NULL);
          double current_time = t.tv_sec - double(int(t.tv_sec/10000)*10000) + t.tv_usec / 1000000.0;
          std::cout << "buffer ID: " << _block_id << " queue is flushed at time " << current_time << " s" << std::endl;
        }
      }
      else
      {
        if(_develop_mode)
          std::cout << "buffer ID: " << _block_id << " is empty. no element is flushed." << std::endl;
      }
    }

  } /* namespace dmdl */
} /* namespace gr */

