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


#ifndef INCLUDED_DMDL_ATTRIBUTE_FILTER_H
#define INCLUDED_DMDL_ATTRIBUTE_FILTER_H

#include <dmdl/api.h>
#include <gnuradio/block.h>

namespace gr {
  namespace dmdl {

    /*!
     * \brief <+description of block+>
     * \ingroup dmdl
     *
     */
    class DMDL_API attribute_filter : virtual public gr::block
    {
     public:
      typedef boost::shared_ptr<attribute_filter> sptr;

      /*!
       * \brief Return a shared_ptr to a new instance of dmdl::attribute_filter.
       *
       * To avoid accidental use of raw pointers, dmdl::attribute_filter's
       * constructor is in a private implementation
       * class. dmdl::attribute_filter::make is the public interface for
       * creating new instances.
       */
      static sptr make(int develop_mode, int block_id, std::string field_name, int value);
    };

  } // namespace dmdl
} // namespace gr

#endif /* INCLUDED_DMDL_ATTRIBUTE_FILTER_H */

