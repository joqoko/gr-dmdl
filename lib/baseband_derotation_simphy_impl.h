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

#ifndef INCLUDED_DMDL_BASEBAND_DEROTATION_SIMPHY_IMPL_H
#define INCLUDED_DMDL_BASEBAND_DEROTATION_SIMPHY_IMPL_H

#include <dmdl/baseband_derotation_simphy.h>

namespace gr {
  namespace dmdl {

    class baseband_derotation_simphy_impl : public baseband_derotation_simphy
    {
     private:
      float _mu;
      float _error;
      gr::digital::constellation_sptr _constellation;
     public:
      baseband_derotation_simphy_impl(float mu, gr::digital::constellation_sptr con);
      ~baseband_derotation_simphy_impl();

      void set_mu(float mu){ _mu = mu; };
      float mu() const { return _mu; };
      // Where all the action really happens
      int work(int noutput_items,
	       gr_vector_const_void_star &input_items,
	       gr_vector_void_star &output_items);
    };

  } // namespace dmdl
} // namespace gr

#endif /* INCLUDED_DMDL_BASEBAND_DEROTATION_SIMPHY_IMPL_H */

