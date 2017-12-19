// ==========================================================================
//
// file : hwcpp-port-adapters.hpp
//
// adapters that transform a port to another kind of port
//
// ==========================================================================
//
// This file is part of HwCpp, 
// a C++ library for close-to-the-hardware programming.
//
// Copyright Wouter van Ooijen 2017
// 
// Distributed under the Boost Software License, Version 1.0.
// (See the accompanying LICENSE_1_0.txt in the root directory of this
// library, or a copy at http://www.boost.org/LICENSE_1_0.txt)
//
// ==========================================================================


// ==========================================================================
//
// PUBLIC
//
// adapter that creates a port out from another port
//
// ==========================================================================

template< can_port_out port > 
struct port_out< port > :
   _port_out_root< port::n_pins >,
   _box_out< port >
{};
  
   
   
/* 
// ==========================================================================
//
// PUBLIC
//
// adapter that creates a port in from another port
//
// ==========================================================================


template< is_port_in port > 
struct port_in< port > :
   port
{};
   
template< is_port_in_out port > 
struct port_in< port > :
   port_in_root< port::n_pins > 
{
   	
   using value_type = typename port::value_type;
   
   static void init() { 
      port::init();
	  port::direction_set_direct( pin_direction::output ); 
   }
      
   static value_type set_direct( value_type v ) {
      return port::get_direct();
   }   
	
};

      
// ==========================================================================
//
// PUBLIC
//
// adapter that creates a port in out from another port
//
// ==========================================================================


   
// ==========================================================================
//
// PUBLIC
//
// adapter that creates a port oc from another port
//
// ==========================================================================

*/