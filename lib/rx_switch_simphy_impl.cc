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
#include "rx_switch_simphy_impl.h"
#include <numeric>

namespace gr {
  namespace dmdl {

    rx_switch_simphy::sptr
    rx_switch_simphy::make(int develop_mode, int block_id, int num_fetch_per_cs)
    {
      return gnuradio::get_initial_sptr
        (new rx_switch_simphy_impl(develop_mode, block_id, num_fetch_per_cs));
    }

    /*
     * The private constructor
     */
    rx_switch_simphy_impl::rx_switch_simphy_impl(int develop_mode, int block_id, int num_fetch_per_cs)
      : gr::sync_block("rx_switch_simphy",
              gr::io_signature::make(1, 1, sizeof(gr_complex)),
              gr::io_signature::make(1, 1, sizeof(gr_complex))),
        _block_id(block_id),
        _develop_mode(develop_mode),
        _num_fetch_per_cs(num_fetch_per_cs),
        _is_receiving(1)
    {
      if(_develop_mode)
        std::cout << "develop_mode of rx_switch_simphy ID: " << _block_id << " is activated." << std::endl;
      message_port_register_in(pmt::mp("RXI"));
      message_port_register_out(pmt::mp("R"));
      message_port_register_out(pmt::mp("RXO"));
      set_msg_handler(pmt::mp("RXI"), boost::bind(&rx_switch_simphy_impl::kai_guan, this, _1 ));
    }

    /*
     * Our virtual destructor.
     */
    rx_switch_simphy_impl::~rx_switch_simphy_impl()
    {
    }

    int
    rx_switch_simphy_impl::work(int noutput_items,
        gr_vector_const_void_star &input_items,
        gr_vector_void_star &output_items)
    {
      const gr_complex *in = (const gr_complex *) input_items[0];
      gr_complex *out = (gr_complex *) output_items[0];
      gr_complex complex_zero(0, 0);
        
      /* 
       * signal is not changed if _is_receiving is true, otherwise just all zeros.
       */
      double pow_sum = 0;
      if(_is_receiving)
      {
        for(int i = 0; i < noutput_items; i++)
        {
          out[i] = in[i];
          pow_sum = pow_sum + abs(out[i]);
        }
      }
      else
      {
        for(int i = 0; i < noutput_items; i++)
        {
          //out[i] = in[i] / 10000;
          out[i] = complex_zero;
        }
      }
      _vec_average_pow.push_back(pow_sum / noutput_items);

      if(_vec_average_pow.size() >= _num_fetch_per_cs)
      {
        double pow_average_all_fetch = std::accumulate(_vec_average_pow.begin(), _vec_average_pow.end(), 0.0) / _num_fetch_per_cs;
        message_port_pub(pmt::mp("R"), pmt::from_double(pow_average_all_fetch));
        //std::cout << "average power at rx switch is: " << pow_average_all_fetch << std::endl;
        _vec_average_pow.clear();
      }
      // Tell runtime system how many output items we produced.
      return noutput_items;
    }

    void
    rx_switch_simphy_impl::kai_guan(pmt::pmt_t spark)
    {
      if(_develop_mode)
      {
	std::cout << "++++ rx_switch ID: " << _block_id;
      }
      struct timeval t;
      gettimeofday(&t, NULL);
      double start_time_show = t.tv_sec - double(int(t.tv_sec/10)*10) + t.tv_usec / 1000000.0;
      if(pmt::is_bool(spark))
      {
        // true: receiving
        if(pmt::to_bool(spark))
        {
          if(_develop_mode)
          {
            if(_is_receiving)
              std::cout << " continue receiving at time ";
            else 
              std::cout << " start receiving at time ";
          }
          _is_receiving = 1;
          struct timeval t;
          gettimeofday(&t, NULL);
          double start_rx_time = t.tv_sec + t.tv_usec / 1000000.0;
          double tx_gap = start_rx_time - _stop_time;
          if(_develop_mode)
            std::cout << "tx_gap is: " << tx_gap << "s" << std::endl;
        }
        // false: not receiving
        else
        {
          if(_develop_mode)
          {
            if(_is_receiving)
              std::cout << " stop receiving at time ";
            else 
              std::cout << " stay inactive mode at time ";
          }
          _is_receiving = 0;
        }
      }
      // if spark is not a boolean, it should be a dict of frame_info. In this case, rececption should be stopped and output the frame immediately
      else
      {
        // tx mode, stop receiving
        if(_develop_mode)
        {
          if(_is_receiving)
            std::cout << " stop receiving at time ";
          else 
            std::cout << " continues not to receive at time ";
        }
        _is_receiving = 0;
        struct timeval t;
        gettimeofday(&t, NULL);
        _stop_time = t.tv_sec + t.tv_usec / 1000000.0;
        message_port_pub(pmt::mp("RXO"), spark);
      }
      if(_develop_mode)
        std::cout << start_time_show << "s" << std::endl;
    }

  } /* namespace dmdl */
} /* namespace gr */

