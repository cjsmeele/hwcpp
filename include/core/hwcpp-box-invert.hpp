// ==========================================================================
//
// file : hwcpp-box-invert.hpp
//
// inverting functionality for box classes
//
// An inverted box reads or writes the inverted value.
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
// LIBRARY-INTERNAL
//
// mixin class and concept for invertible boxes
//
// a class T must be a box and inherit from 
// invertible (for bitwise inversion) or from
// invertible_interval (for numeric interval inversion) 
// to be accepted by invert< T >. 
// It is the responsibility of the using class to do both.
//
// ==========================================================================

struct invertible {
   static constexpr bool _is_invertible = true;
};

template< typename T >
concept bool is_invertible = requires {  
   T::_is_box_tag;
   T::_is_invertible; 
};

// ========== mixin class for bitwise invertible boxes

struct invertible_bitwise :
   invertible
{ 
   static bool HWCPP_INLINE invert_value( bool v ){ 
      return ! v; 
   }
   
   static auto HWCPP_INLINE invert_value( auto v ){ 
      return ~ v; 
   }
};

// ========= mixin class for numeric-range invertible boxes

template< typename T >
struct invertible_interval :
   invertible
{ 
   static auto HWCPP_INLINE invert_value( auto v ){ 
      return T::lowest + ( T::highest - v ); 
   }
};


// ==========================================================================
//
// FILE-INTERNAL
//
// class wrappere that inverts the set functions (when present)
//
// ==========================================================================

template< typename T > struct _invert_set_wrapper : T {};

template< _has_box_out_functions T >
struct _invert_set_wrapper< T > : T {
	
   using _vt = typename T::value_type;
    
   static void HWCPP_INLINE set( _vt v ){ 
      T::set( T::invert_value( v ) ); }
       
   static void HWCPP_INLINE set_direct( _vt v ){ 
      T::set_direct( T::invert_value( v ) ); }
      
   static void HWCPP_INLINE set_buffered( _vt v ){ 
      T::set_buffered( T::invert_value( v ) ); }
};	


// ==========================================================================
//
// FILE-INTERNAL
//
// class filter that inverts the get functions (when present)
//
// ==========================================================================

template< typename T > struct _invert_get_wrapper : T {};

template< _has_box_in_functions T >
struct _invert_get_wrapper< T > : T {
    
   	static auto HWCPP_INLINE get(){ 
       return T::invert_value( T::get() ); }
       
   	static auto HWCPP_INLINE get_direct(){ 
       return T::invert_value( T::get_direct() ); }
	   
   	static auto HWCPP_INLINE get_buffered(){ 
       return T::invert_value( T::get_buffered() ); }
};	


// ==========================================================================
//
// PUBLIC
//
// invert decorator
//
// ==========================================================================

template< is_invertible T > 
using invert =
   _invert_get_wrapper< 
   _invert_set_wrapper< T > >;
