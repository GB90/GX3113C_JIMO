#ifndef CYGONCE_DEVS_SPI_ARM_GX3110_H
#define CYGONCE_DEVS_SPI_ARM_GX3110_H

//==========================================================================
//
//      spi_flash.h
//
//      SPI driver for GX3110
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
// Author(s):     Haibo Gao <ghb20081@163.com>
// Contributors: 
// Date:         2009-09-12
// Purpose:      
// Description:  
// Usage:        #include <cyg/io/gx3110_spi.h>             
//####DESCRIPTIONEND####
//
//==========================================================================

#include <pkgconf/hal.h>
#include <pkgconf/io_spi.h>
#include <pkgconf/devs_gx3110_spi.h>

#include <cyg/infra/cyg_type.h>
#include <cyg/hal/drv_api.h>
#include <cyg/io/spi.h>
#include <pkgconf/system.h>
#include <cyg/kernel/kapi.h>

#define GXSPI_FLASH_DROP_CS 	(1<<0)
#define GXSPI_FLASH_READ 		(1<<1)
#define GXSPI_FLASH_FAST_READ	(1<<2)
#define GXSPI_FLASH_READ_STATUS	(1<<3)
#define GXSPI_FLASH_PP			(1<<4)
#define GXSPI_FLASH_PP_NOT_BIT_SET (1<<5)

typedef struct {
	cyg_uint32 SPI_1_BASE_ADDR ;
	cyg_uint32 SPI_2_BASE_ADDR ;
	cyg_uint32 rPIN_FUNCTION_SEL_1;//pin function configure
	cyg_uint32 CYGNUM_GX3110_INTERRUPT_SPI0;	//interrupt num
	cyg_uint32 CYGNUM_GX3110_INTERRUPT_SPI1;
	cyg_uint32 CHIP_TYPE;
}gxchip_correlation;

struct spi_dev {
	volatile cyg_uint32 spcr;	//control register
	volatile cyg_uint32 spsr;	//status register
	volatile cyg_uint32 sptdr;	//transfer data register
	volatile cyg_uint32 sprdr;	//recive data register
	volatile cyg_uint32 spcfg;	//cfg register.gx3201
};

typedef struct {
	cyg_spi_bus     spi_bus;
	cyg_interrupt   spi_intr;
	cyg_handle_t    spi_hand;
	cyg_vector_t    spi_vect;
	cyg_priority_t  spi_prio;
	cyg_drv_mutex_t spi_lock;
	cyg_drv_cond_t  spi_wait;

	volatile  struct spi_dev *spi_dev;

	volatile cyg_uint32       count;
	volatile const cyg_uint8 *tx;
	volatile cyg_uint8       *rx;
} cyg_spi_gx3110_bus_t;

typedef struct {
	cyg_spi_device  spi_device;

	cyg_uint8       spi_cpha;
	cyg_uint8       spi_cpol;
	cyg_uint8       spi_lsbf;
	cyg_uint32      spi_baud;
} cyg_spi_gx3110_dev_t;

#ifdef CYGPKG_DEVS_SPI_ARM_GX3110_BUS0
	externC cyg_spi_gx3110_bus_t cyg_spi_gx3110_bus0;
#endif

#ifdef CYGPKG_DEVS_SPI_ARM_GX3110_BUS1
	externC cyg_spi_gx3110_bus_t cyg_spi_gx3110_bus1;
#endif

extern cyg_spi_gx3110_dev_t gx3110_spi0_dev;
#endif

