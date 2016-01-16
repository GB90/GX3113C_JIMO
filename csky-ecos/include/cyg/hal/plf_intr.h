#ifndef CYGONCE_HAL_PLF_INTR_H
#define CYGONCE_HAL_PLF_INTR_H

//==========================================================================
//
//      plf_intr.h
//
//      Atlas Interrupt and clock support
//
//==========================================================================
// ####ECOSGPLCOPYRIGHTBEGIN####                                            
// -------------------------------------------                              
// This file is part of eCos, the Embedded Configurable Operating System.   
// Copyright (C) 1998, 1999, 2000, 2001, 2002 Free Software Foundation, Inc.
//
// eCos is free software; you can redistribute it and/or modify it under    
// the terms of the GNU General Public License as published by the Free     
// Software Foundation; either version 2 or (at your option) any later      
// version.                                                                 
//
// eCos is distributed in the hope that it will be useful, but WITHOUT      
// ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or    
// FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License    
// for more details.                                                        
//
// You should have received a copy of the GNU General Public License        
// along with eCos; if not, write to the Free Software Foundation, Inc.,    
// 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.            
//
// As a special exception, if other files instantiate templates or use      
// macros or inline functions from this file, or you compile this file      
// and link it with other works to produce a work based on this file,       
// this file does not by itself cause the resulting work to be covered by   
// the GNU General Public License. However the source code for this file    
// must still be made available in accordance with section (3) of the GNU   
// General Public License v2.                                               
//
// This exception does not invalidate any other reasons why a work based    
// on this file might be covered by the GNU General Public License.         
// -------------------------------------------                              
// ####ECOSGPLCOPYRIGHTEND####                                              
//==========================================================================
//#####DESCRIPTIONBEGIN####
//
// Author(s):    nickg
// Contributors: nickg, jskov,
//               gthomas, jlarmour, dmoseley, michael anburaj <michaelanburaj@hotmail.com>
// Date:         2000-06-06
// Purpose:      Define Interrupt support
// Description:  The macros defined here provide the HAL APIs for handling
//               interrupts and the clock for the Atlas board.
//              
// Usage:
//              #include <cyg/hal/plf_intr.h>
//              ...
//              
//
//####DESCRIPTIONEND####
//
//==========================================================================

#include <pkgconf/hal.h>
// First an assembly safe part

//--------------------------------------------------------------------------
// Interrupt vectors.

#ifndef CYGHWR_HAL_INTERRUPT_VECTORS_DEFINED

// These are decoded via the IP bits of the cause
// register when an external interrupt is delivered.

#define CYGNUM_HAL_INTERRUPT_SER                    0
#define CYGNUM_HAL_INTERRUPT_TIM0                   1
#define CYGNUM_HAL_INTERRUPT_2                      2
#define CYGNUM_HAL_INTERRUPT_3                      3
#define CYGNUM_HAL_INTERRUPT_FPGA_RTC               4
#define CYGNUM_HAL_INTERRUPT_COREHI                 5
#define CYGNUM_HAL_INTERRUPT_CORELO                 6
#define CYGNUM_HAL_INTERRUPT_7                      7
#define CYGNUM_HAL_INTERRUPT_PCIA                   8
#define CYGNUM_HAL_INTERRUPT_PCIB                   9
#define CYGNUM_HAL_INTERRUPT_PCIC                   10
#define CYGNUM_HAL_INTERRUPT_PCID                   11
#define CYGNUM_HAL_INTERRUPT_ENUM                   12
#define CYGNUM_HAL_INTERRUPT_DEG                    13
#define CYGNUM_HAL_INTERRUPT_ATXFAIL                14
#define CYGNUM_HAL_INTERRUPT_INTA                   15
#define CYGNUM_HAL_INTERRUPT_INTB                   16
#define CYGNUM_HAL_INTERRUPT_INTC                   17
#define CYGNUM_HAL_INTERRUPT_INTD                   18
#define CYGNUM_HAL_INTERRUPT_SERR                   19
#define CYGNUM_HAL_INTERRUPT_HW1                    20
#define CYGNUM_HAL_INTERRUPT_HW2                    21
#define CYGNUM_HAL_INTERRUPT_HW3                    22
#define CYGNUM_HAL_INTERRUPT_HW4                    23
#define CYGNUM_HAL_INTERRUPT_HW5                    24



#define CYGNUM_HAL_INTERRUPT_DEBUG_UART             CYGNUM_HAL_INTERRUPT_SER

#define CYGNUM_HAL_INTERRUPT_RTC                    CYGNUM_HAL_INTERRUPT_HW5

#define CYGHWR_HAL_INTERRUPT_VECTORS_DEFINED

#endif

//==========================================================================

// These are interrupts on the 3001--6107 chip

// The vector used by the Real time clock 
     
#define CYGNUM_HAL_INTERRUPT_BMIINTR      			63

#define CYGNUM_HAL_INTERRUPT_NONE    				-1

#define CYGNUM_HAL_ISR_MIN            				0
#define CYGNUM_HAL_ISR_MAX            				(CYGNUM_HAL_INTERRUPT_BMIINTR)

#define CYGNUM_HAL_ISR_COUNT          				(CYGNUM_HAL_ISR_MAX-CYGNUM_HAL_ISR_MIN+1)

//--------------------------------------------------------------------------
#ifndef __ASSEMBLER__

#include <cyg/infra/cyg_type.h>

//--------------------------------------------------------------------------
// Control-C support.

#if defined(CYGDBG_HAL_CK_DEBUG_GDB_CTRLC_SUPPORT)

# define CYGHWR_HAL_GDB_PORT_VECTOR CYGNUM_HAL_INTERRUPT_SER

externC cyg_uint32 hal_ctrlc_isr(CYG_ADDRWORD vector, CYG_ADDRWORD data);

# define HAL_CTRLC_ISR hal_ctrlc_isr

#endif


//----------------------------------------------------------------------------
// Reset.
#ifndef CYGHWR_HAL_RESET_DEFINED
extern void hal_atlas_reset( void );
#define CYGHWR_HAL_RESET_DEFINED
//#define HAL_PLATFORM_RESET()                      hal_atlas_reset()

#define HAL_PLATFORM_RESET_ENTRY                    0xbfc00000

#endif // CYGHWR_HAL_RESET_DEFINED

#endif // __ASSEMBLER__

//--------------------------------------------------------------------------
#endif // ifndef CYGONCE_HAL_PLF_INTR_H
// End of plf_intr.h
