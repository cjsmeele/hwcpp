// ==========================================================================
//
// file : hwcpp-box-fanout.hpp
//
// fanout behaviour for box classes
//
// A fanout combination of output boxes writes the same value
// to all boxes.
// 
// Fanout functionality for a specific type is provided
// by that type by deferring to _box_fanout< adapter, list... >,
// check the pins for an example.
//
// LIBRARY-INTERNAL
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


// ========== base, not implemented, never used

template< 
   template< typename > class adapt, 
   typename... pins 
> struct _box_fanout;


// ========== recursion endpoint

template< 
   template< typename > class adapt 
> struct _box_fanout< adapt > {
   
   static void init(){}
   static void set( auto v ){}
   static void set_direct( auto v ){}
   static void set_buffered( auto v ){}
   static void flush(){}
};

// ========== handle one box and recurse

template<  
   template< typename > class adapt, 
   typename _box, 
   typename... tail >
struct _box_fanout< adapt, _box, tail... > :
   _box_fanout< adapt, tail... >
{
	
   using _value_type = typename _box::value_type;	
   using box = adapt< _box >;	
   using tail_boxes = _box_fanout< adapt, tail... >;
	
   static void init() { 
      box::init();
      tail_boxes::init(); 
   }
      
   static void set( _value_type v ) {
      box::set_buffered( v );
      tail_boxes::set_buffered( v );
	  flush();
   }
      
   static void set_direct( _value_type v ) {
      box::set_buffered( v );
      tail_boxes::set_buffered( v );
	  flush();
   }
      
   static void set_buffered( _value_type v ) {
      box::set_buffered( v );
      tail_boxes::set_buffered( v );
   }
      
   static void flush() {
      box::flush();
      tail_boxes::flush();
   }
   
};