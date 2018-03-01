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

#ifndef INCLUDED_DMDL_ATTRIBUTE_SPLITTER_IMPL_H
#define INCLUDED_DMDL_ATTRIBUTE_SPLITTER_IMPL_H

#include <dmdl/attribute_splitter.h>

namespace gr {
  namespace dmdl {

    class attribute_splitter_impl : public attribute_splitter
    {
     private:
      int _develop_mode;
      int _block_id;
      std::string _command_name;
      void splitting(pmt::pmt_t cmd_in_);

     public:
      attribute_splitter_impl(int develop_mode, int block_id, std::string command_name);
      ~attribute_splitter_impl();
    };

  } // namespace dmdl
} // namespace gr

#endif /* INCLUDED_DMDL_ATTRIBUTE_SPLITTER_IMPL_H */

