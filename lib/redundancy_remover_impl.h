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

#ifndef INCLUDED_DMDL_REDUNDANCY_REMOVER_IMPL_H
#define INCLUDED_DMDL_REDUNDANCY_REMOVER_IMPL_H

#include <dmdl/redundancy_remover.h>

namespace gr {
  namespace dmdl {

    class redundancy_remover_impl : public redundancy_remover
    {
     private:
      int _develop_mode;
      int _block_id;
      int _last_index;
      void check_redundancy(pmt::pmt_t cmd_in);

     public:
      redundancy_remover_impl(int develop_mode, int block_id);
      ~redundancy_remover_impl();
    };

  } // namespace dmdl
} // namespace gr

#endif /* INCLUDED_DMDL_REDUNDANCY_REMOVER_IMPL_H */

