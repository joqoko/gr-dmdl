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

#ifndef INCLUDED_DMDL_PACKETIZER_SIMPHY_IMPL_H
#define INCLUDED_DMDL_PACKETIZER_SIMPHY_IMPL_H

#include <dmdl/packetizer_simphy.h>
#include <gnuradio/digital/packet_header_default.h>

namespace gr {
  namespace dmdl {

    class packetizer_simphy_impl : public packetizer_simphy
    {
      private:
        gr::digital::packet_header_default::sptr _header_generator;
        std::vector< unsigned char > _random;
        std::vector< unsigned char > _preamble;
        std::vector< unsigned char > _preamble_packed;
        int _develop_mode;
        int _block_id; 
        int _padding;
        double _last_tx_time;
        double _bps;
        static const unsigned char _random_array[128];
      public:
        packetizer_simphy_impl(int develop_mode, int block_id, const std::vector< unsigned char > &preamble, int padding, double bps);
        ~packetizer_simphy_impl();
        void receive(pmt::pmt_t msg);
    };

  } // namespace dmdl
} // namespace gr

#endif /* INCLUDED_DMDL_PACKETIZER_SIMPHY_IMPL_H */

