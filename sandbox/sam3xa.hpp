// ============================================================================
//
// file : target-sam3xa-chip.hpp
//
// HAL for atsam3x, common for the bare chip and the arduino due
//
// ============================================================================

#include "hwcpp-all.hpp"

// the atmel header files use 'register', which is removed in C++17
#define register 
#include "sam.h"
#undef register

// the implementation of CPU clock speed switching
#include "sam3xa.inc"

namespace hwcpp::sam3xa {
    
static void init(){
   static bool done = false;
   if( done ){
      return; 
   }       
   done = true;
    
   // disable the watchdog     
   WDT->WDT_MR = WDT_MR_WDDIS;     
   
   // enable the clocks to all GPIO blocks
   PMC->PMC_PCER0 = ( 0x3F << 11 );
}    
	
enum class pio {
   a = 0x400E0E00U,
   b = 0x400E1000U,
   c = 0x400E1200U,
   d = 0x400E1400U
};

// ========= pin_in_out ==========

template< pio P, uint32_t pin >
struct _pin_in_out {
	
   static void HWLIB_INLINE init(){
      hwcpp::sam3xa::init();
   }
   
   static void HWLIB_INLINE direction_set_direct( direction d ){
      ( ( d == direction::input )
         ?  ((Pio*)P)->PIO_ODR 
         :  ((Pio*)P)->PIO_OER 
      )  = ( 0x1U << pin );
   }
   
   static void HWLIB_INLINE set_direct( bool v ){
      ( v 
         ?  ((Pio*)P)->PIO_SODR 
         :  ((Pio*)P)->PIO_CODR 
      )  = ( 0x1U << pin );	   
   }

   static void HWLIB_INLINE get_direct( bool v ){
      ( v 
         ?  ((Pio*)P)->PIO_SODR 
         :  ((Pio*)P)->PIO_CODR 
      )  = ( 0x1U << pin );	   
   }
};

template< pio P, uint32_t pin >
using pin_in_out = pin_in_out_direct_base< _pin_in_out< P, pin > >;	

// is this any use? all pins are in-out (except the OC ones)
// for those, create a reinterpret-cast
template< pio P, uint32_t pin >
using pin_out = pin_out< pin_in_out< P, pin > >;	

}; // namespace hwcpp::sam3xa





