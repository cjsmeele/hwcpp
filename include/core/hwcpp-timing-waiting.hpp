// ==========================================================================
//
// file : hwcpp-timing-waiting.hpp
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
// root class and concept for the wait timing service
//
// ==========================================================================

template< typename _ticks_frequency > 
struct _timing_waiting_root :
   not_instantiable
{
   static constexpr bool _is_timing_waiting_tag = true; 
};

template< typename T >
concept bool is_waiting(){ 
   return T::_is_timing_waiting_tag;
   // ticks_from_ns
   // ns_from_ticks
}


// ==========================================================================
//
// LIBRARY-INTERNAL
//
// this is the foundation provided by a HAL
//
// ==========================================================================

template< typename _ticks_frequency >
struct _timing_waiting_foundation :
   not_instantiable
{
   static constexpr bool _is_timing_waiting_foundation_tag = true;
   
   using ticks_frequency = _ticks_frequency;
};

template< typename T >
concept bool _is_timing_waiting_foundation(){ 
   return T::_is_timing_waiting_foundation_tag;
   
   // + waiting
   // now_ticks()
}


// ==========================================================================
//
// LIBRARY-INTERNAL
//
// builder for a waiting service
//
// ==========================================================================


template< _is_timing_waiting_foundation T >
struct _timing_waiting_builder : 
   _timing_waiting_root< T >
{
   using _root = _timing_waiting_root< T >;	
   using ticks_frequency = typename T::ticks_frequency;	
   
   static void init(){
      T::init();
   }	  
   
   // ========== ticks <-> ns conversion   
   
   static constexpr ticks_type ticks_from_ns( ns_type n ){ 
	  return 1 + (
	     ( n * ticks_frequency::num ) 
		 / ( 1'000'000'000 * ticks_frequency::den ));
   }	

   static constexpr ns_type ns_from_ticks( ticks_type n ){ 
	  return 1 + ( 
	     ( n* 1'000'000'000 * ticks_frequency::den )
	     / ( ticks_frequency::num ));
   }     
   
   // ========== dummy exclusive
   
   template< typename Dummy >
   struct exclusive {};
     
   // ========== wait a run-time known amount of time 
   
   static void wait_ticks( ticks_type t ){
      T::wait_ticks_function( t );	    	   
   }   
   
   static void wait_ns( ns_type n ){
      wait_ticks( ticks_from_ns( n ) ); 
   }
   
   static void wait_us( ns_type n ){
      wait_ticks( ticks_from_ns( n * 1'000ULL ) );	    	   
   }

   static void wait_ms( ns_type n ){
      wait_ticks( ticks_from_ns( n * 1'000'000ULL ) );	    	   
   }

   static void wait_s( ns_type n ){
      wait_ticks( ticks_from_ns( n * 1'000'000'000ULL ) );	    	   
   }   
   
   //========== duration template
   
   template< 
      ticks_type _ticks,
      ns_type    _ns
   > 
   struct duration :
      duration_root< _ticks, _ns >
   {
      static void init(){
         T::init();		  
	  }	   
	  
	  static void HWCPP_INLINE wait(){
         T::template wait_ticks_template< _ticks >();
	  }
	   
   };		  
   
   //========== durations  

   template< ticks_type t >
   using ticks = duration< t, ns_from_ticks( t ) >;
   
   template< ns_type n >
   using ns = duration< ticks_from_ns( n ), n >;
   
   template< ns_type n >
   using us = ns< n * 1'000ULL >;

   template< ns_type n >
   using ms = ns< n * 1'000'000ULL >;

   template< ns_type n >
   using s = ns< n * 1'000'000'000ULL >;     
   
};
