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
#include "carrier_sensing_impl.h"
#include <boost/thread/thread.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

namespace gr {
  namespace dmdl {

    carrier_sensing::sptr
    carrier_sensing::make(int develop_mode, int block_id, int cs_mode, double cs_duration, float cs_threshold, int system_time_granularity_us, int nf_initial_n, int rx_sensitivity_dB)
    {
      return gnuradio::get_initial_sptr
        (new carrier_sensing_impl(develop_mode, block_id, cs_mode, cs_duration, cs_threshold, system_time_granularity_us, nf_initial_n, rx_sensitivity_dB));
    }

    /*
     * The private constructor
     */
    carrier_sensing_impl::carrier_sensing_impl(int develop_mode, int block_id, int cs_mode, double cs_duration, float cs_threshold, int system_time_granularity_us, int nf_initial_n, int rx_sensitivity_dB)
      : gr::block("carrier_sensing",
              gr::io_signature::make(0, 0, 0),
              gr::io_signature::make(0, 0, 0)),
        _develop_mode(develop_mode),
        _block_id(block_id),
        _cs_mode(cs_mode),
        _cs_duration(cs_duration),
        _nf_initial_n(nf_initial_n),
        _system_time_granularity_us(system_time_granularity_us),
        _rx_sensitivity_dB(rx_sensitivity_dB),
        _cs_threshold(cs_threshold)
    {
      _in_cca = false;
      _cs_time = 0;
      _cca = false;
      _stop_sensing = false;
      if(_develop_mode == 1)
        std::cout << "develop_mode of carrier sensing is activated." << std::endl;
      message_port_register_in(pmt::mp("B"));
      message_port_register_out(pmt::mp("F"));
      message_port_register_out(pmt::mp("P"));
      message_port_register_out(pmt::mp("T"));
      set_msg_handler(
        pmt::mp("B"),
        boost::bind(&carrier_sensing_impl::start_sensing, this, _1)
      );
      message_port_register_in(pmt::mp("S"));
      set_msg_handler(
        pmt::mp("S"),
        boost::bind(&carrier_sensing_impl::stop_sensing, this, _1)
      );
      message_port_register_in(pmt::mp("R"));
      set_msg_handler(
        pmt::mp("R"),
        boost::bind(&carrier_sensing_impl::sensing, this, _1)
      );
      message_port_register_in(pmt::mp("G"));
      set_msg_handler(
        pmt::mp("G"),
        boost::bind(&carrier_sensing_impl::reset_duration, this, _1)
      );
    }

    /*
     * our virtual destructor.
     */
    carrier_sensing_impl::~carrier_sensing_impl()
    {
    }

    void carrier_sensing_impl::reset_duration(pmt::pmt_t cmd_in)
    {
      if(pmt::is_integer(cmd_in))
      {
        _cs_duration = pmt::to_long(cmd_in);
        if(_develop_mode)
          std::cout << "cs duration of carrier_sensing block ID " << _block_id << " is reset to " << _cs_duration << std::endl;
      }
      else if(pmt::dict_has_key(cmd_in, pmt::mp("CCA1_ms")))
      {
        pmt::pmt_t not_found;
        _cs_duration = pmt::to_long(pmt::dict_ref(cmd_in, pmt::string_to_symbol("CCA1_ms"), not_found));
        if(_develop_mode)
          std::cout << "cs duration of carrier_sensing block ID " << _block_id << " is reset to " << _cs_duration << " according to CogMAC protocol " << std::endl;
      }
      else   
      {
        std::cout << "error: carrier_sensing block ID " << _block_id << " can only reassign cs_duration to a integer number (in [ms])." << std::endl;
      }
    }

    void carrier_sensing_impl::sensing(pmt::pmt_t power_in)
    {
      if(pmt::is_real(power_in))
      {
        double power = pmt::to_double(power_in);
        // _cca true means the channel is free 
        if(_nf_initial_n > 0)
        {
          _nf_initial_n--;
          _noise_floor.insert(_noise_floor.begin(), power);
        }
        else if(_nf_initial_n == 0)
        {
          int len = _noise_floor.size();
          for(std::vector<double>::iterator it = _noise_floor.begin(); it != _noise_floor.end(); ++it)
          {
            if(_develop_mode)
              std::cout << "the save noise level is: " << _noise_floor.back() << std::endl;
            _cs_threshold = _cs_threshold + _noise_floor.back();
            _noise_floor.pop_back();
          }
          double rx_sens = pow(10, double(_rx_sensitivity_dB)/10);
          _cs_threshold = _cs_threshold / len * rx_sens;
          if(_develop_mode)
            std::cout << " rx_sensitivity is " << rx_sens << " and the noise floor is " << _cs_threshold << std::endl;
          message_port_pub(pmt::mp("T"), pmt::from_double(_cs_threshold));
          _nf_initial_n = -1;
        }
        else
        {
          _cca = (_cs_threshold > power);
        }
        if(_develop_mode == 3)
        {
          struct timeval t;
          gettimeofday(&t, NULL);
          double current_time = t.tv_sec - double(int(t.tv_sec/100)*100) + t.tv_usec / 1000000.0;
          std::cout << "in carrier sensing, average rx power is: " << power << ", received at " << current_time << " s" << std::endl;
        }
      }
      else
        std::cout << "carrier_sensing ID " << _block_id << " error: not valid power signal" << std::endl;
    }
    

    void carrier_sensing_impl::stop_sensing(pmt::pmt_t cmd_in)
    {
      _stop_sensing = true;
    }

    void carrier_sensing_impl::start_sensing(pmt::pmt_t cmd_in)
    {
      struct timeval t;
      gettimeofday(&t, NULL);
      double current_time = t.tv_sec - double(int(t.tv_sec/100)*100) + t.tv_usec / 1000000.0;

      if(_cs_mode == 4)
      {
        _cmd = cmd_in;
        // continuous carrier sensing
        if(pmt::is_dict(cmd_in))
        {
          if(_develop_mode == 1 || _develop_mode == 2)
            std::cout << "+++++++++ cs ID: " << _block_id << " in mode continuous  +++++++++" << std::endl;    
          _stop_sensing = false;
          boost::thread thrd(&carrier_sensing_impl::continuous_sensing, this);
        }
        else
        {
          // not a dict pmt, most likely an import error
          std::cout << "Warning: is not a valid input pmt. Please check your connections." << std::endl;
        }
      }
      else if(_cs_mode == 1)
      {
        // Oneshot carrier sensing
        if(pmt::is_dict(cmd_in))
        {
          if(_develop_mode == 1 || _develop_mode == 2)
            std::cout << "+++++++++ cs ID: " << _block_id << " in mode Oneshot  +++++++++" << std::endl;    
          // this function is fired
          pmt::pmt_t not_found;
          int frame_type = pmt::to_long(pmt::dict_ref(cmd_in, pmt::string_to_symbol("frame_type"), not_found)); 
          if(frame_type == 1)
          {
            _cmd = cmd_in;
            if(_develop_mode == 1)
              std::cout << "before sending a data frame, start sensing" << std::endl;
            boost::thread thrd(&carrier_sensing_impl::oneshot_sensing, this);
          }
          else
          {
            if(_develop_mode == 1)
              std::cout << "before sending a ack frame, no sensing" << std::endl;
            message_port_pub(pmt::mp("P"), cmd_in);
          }
        }
        else
        {
          // not a dict pmt, most likely an import error
          std::cout << "Warning: is not a valid input pmt. Please check your connections." << std::endl;
        }
      }
      // Fix duration carrier sensing
      else if(_cs_mode == 2)
      {
        if(pmt::is_dict(cmd_in))
        {
          if(_develop_mode == 1 || _develop_mode == 2)
            std::cout << "carrier_sensing ID: " << _block_id << " start sensing in mode fix duration at " << "time " << current_time << "s" << std::endl;    
          // this function is fired
          pmt::pmt_t not_found;
          if(pmt::dict_has_key(cmd_in, pmt::string_to_symbol("frame_type")))
          {
            int frame_type = pmt::to_long(pmt::dict_ref(cmd_in, pmt::string_to_symbol("frame_type"), not_found)); 
            if(frame_type == 1)
            {
              _cmd = cmd_in;
              _in_cca = true;
              if(_develop_mode == 1)
                std::cout << "before sending a data frame, start sensing" << std::endl;
              boost::thread thrd(&carrier_sensing_impl::countdown_sensing, this);
            }
            else
            {
              if(_develop_mode == 1)
                std::cout << "before sending a ack frame, no sensing" << std::endl;
              message_port_pub(pmt::mp("P"), cmd_in);
            }
          }
          else
          {
            _cmd = cmd_in;
            _in_cca = true;
            if(_develop_mode == 1 || _develop_mode == 2)
              std::cout << "start sensing" << std::endl;
            boost::thread thrd(&carrier_sensing_impl::countdown_sensing, this);
          }
        }
        else
        {
          // not a dict pmt, most likely an import error
          std::cout << "Warning: is not a valid input pmt. Please check your connections." << std::endl;
        }
      }
      // Unlimited carrier sensing
      else if(_cs_mode == 3)
      {
        if(pmt::is_dict(cmd_in))
        {
          if(_develop_mode == 1 || _develop_mode == 2)
            std::cout << "+++++++++ cs ID: " << _block_id << " in mode unlimited +++++++++" << std::endl;    
          // this function is fired
          pmt::pmt_t not_found;
          int frame_type = pmt::to_long(pmt::dict_ref(cmd_in, pmt::string_to_symbol("frame_type"), not_found)); 
          if(frame_type == 1)
          {
            _cmd = cmd_in;
            if(_develop_mode == 1)
              std::cout << "before sending a data frame, start unlimited sensing" << std::endl;
            boost::thread thrd(&carrier_sensing_impl::unlimited_sensing, this);
          }
          else
          {
            if(_develop_mode == 1)
              std::cout << "before sending a ack frame, no sensing" << std::endl;
            message_port_pub(pmt::mp("P"), cmd_in);
          }
        }
        else
        {
          // not a dict pmt, most likely an import error
          std::cout << "Warning: is not a valid input pmt. Please check your connections." << std::endl;
        }
      }
    }

    void carrier_sensing_impl::oneshot_sensing()
    {
      _in_cca = true;
      struct timeval t;
      gettimeofday(&t, NULL);
      double current_time = t.tv_sec + t.tv_usec / 1000000.0;
      double start_time = t.tv_sec + t.tv_usec / 1000000.0;
      while(_in_cca)
      {
        boost::this_thread::sleep(boost::posix_time::microseconds(_system_time_granularity_us)); 
        gettimeofday(&t, NULL);
        current_time = t.tv_sec + t.tv_usec / 1000000.0;
      }
      if(_cca)
      {
        if(_develop_mode == 1)
          std::cout << "Carrier sensing passed. " << std::endl;
        message_port_pub(pmt::mp("P"), _cmd);
      }
      else
      {
        if(_develop_mode == 1)
          std::cout << "Carrier sensing failed. " << std::endl;
        message_port_pub(pmt::mp("F"), _cmd);
      }
    }

    void carrier_sensing_impl::continuous_sensing()
    {
      struct timeval t;
      gettimeofday(&t, NULL);
      double current_time = t.tv_sec + t.tv_usec / 1000000.0;
      double start_time = t.tv_sec + t.tv_usec / 1000000.0;
      while(!_stop_sensing && _cca)
      {
        boost::this_thread::sleep(boost::posix_time::microseconds(_system_time_granularity_us)); 
        gettimeofday(&t, NULL);
        current_time = t.tv_sec + t.tv_usec / 1000000.0;
      }
      if(_cca)
      {
        if(_develop_mode == 1)
          std::cout << "Carrier sensing passed. " << std::endl;
        message_port_pub(pmt::mp("P"), _cmd);
      }
      else
      {
        if(_develop_mode == 1)
          std::cout << "Carrier sensing failed. " << std::endl;
        message_port_pub(pmt::mp("F"), _cmd);
      }
      _cs_time = current_time - start_time;
      if(_develop_mode == 1)
        std::cout << "Carrier sensing time is: " << _cs_time << " s" << std::endl;
    }

    void carrier_sensing_impl::unlimited_sensing()
    {
      struct timeval t;
      gettimeofday(&t, NULL);
      double current_time = t.tv_sec + t.tv_usec / 1000000.0;
      double start_time = t.tv_sec + t.tv_usec / 1000000.0;
      while(1)
      {
        boost::this_thread::sleep(boost::posix_time::microseconds(_system_time_granularity_us)); 
        gettimeofday(&t, NULL);
        current_time = t.tv_sec + t.tv_usec / 1000000.0;
        if(_cca)
          break;
      }
      if(_develop_mode == 1)
        std::cout << "Carrier sensing passed. " << std::endl;
      message_port_pub(pmt::mp("P"), _cmd);
      _cs_time = current_time - start_time;
      if(_develop_mode == 1)
        std::cout << "Carrier sensing time is: " << _cs_time << " s" << std::endl;
    }

    void carrier_sensing_impl::countdown_sensing()
    {
      struct timeval t;
      gettimeofday(&t, NULL);
      double current_time = t.tv_sec + t.tv_usec / 1000000.0;
      double start_time = t.tv_sec + t.tv_usec / 1000000.0;
//      std::cout << "start_countdown" << std::endl;
//      std::cout << "cs duration is: " << _cs_duration << " in second: " << _cs_duration /1000 << std::endl;
//      std::cout << "current time is: " << current_time << std::endl;
//      std::cout << "_in_cca is: " << _in_cca << std::endl;
//      std::cout << "time condition is: " << (start_time + (_cs_duration / 1000) - current_time) << std::endl;
//      std::cout << ((current_time < start_time + _cs_duration / 1000) && _in_cca) << std::endl;
      while((current_time < start_time + double(_cs_duration) / 1000) && _cca)
      {
        boost::this_thread::sleep(boost::posix_time::microseconds(_system_time_granularity_us)); 
        gettimeofday(&t, NULL);
        current_time = t.tv_sec + t.tv_usec / 1000000.0;
//        std::cout << "current sensing time is: " << current_time - start_time - _cs_duration / 1000 << std::endl;
        // std::cout << "sensing status is: " << _in_cca << std::endl;
//        std::cout << "in sensing " << start_time + _cs_duration / 1000 - current_time << std::endl;
      }
      if(_cca)
      {
        if(_develop_mode == 1)
          std::cout << "carrier sensing passed. " << std::endl;
        message_port_pub(pmt::mp("P"), _cmd);
      }
      else
      {
        if(_develop_mode == 1)
          std::cout << "Carrier sensing failed. " << std::endl;
        message_port_pub(pmt::mp("F"), _cmd);
      }
      _cs_time = current_time - start_time;
      if(_develop_mode == 1)
        std::cout << "Carrier sensing time is: " << _cs_time << " s" << std::endl;
    }

  } /* namespace dmdl */
} /* namespace gr */

