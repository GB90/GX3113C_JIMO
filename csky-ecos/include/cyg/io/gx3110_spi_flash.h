#ifndef GX3110_SPI_FLASH_H
#define GX3110_SPI_FLASH_H
//=============================================================================
//
//      gx3110_spi_flash.h
//
//      SPI flash driver for gx3110 demo board.
//
//=============================================================================
// ####ECOSGPLCOPYRIGHTBEGIN####                                            
// -------------------------------------------                              
// This file is part of eCos, the Embedded Configurable Operating System.   
// Copyright (C) 2008, 2009 Free Software Foundation, Inc.
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
//=============================================================================
//#####DESCRIPTIONBEGIN####
//
// Author(s):   Haibo Gao
// Date:        2009-10-23
// Purpose:     SPI flash driver implementation
// usage: 		#include <cyg/io/gx3110_spi_flash.h>
//####DESCRIPTIONEND####
//
//=============================================================================

// Required data structures.
#include <cyg/io/flash_dev.h>
#include <cyg/io/gx3110_spi.h>

// Exported handle on the driver function table.
externC struct cyg_flash_dev_funs cyg_devs_gxspi_flash_funs;

struct dummy_cycle{
	cyg_uint32 keep_value;
	cyg_uint32 exit_value;
};

//enum xip_enum{
//	STANDARD,	//无需进行任何配置
//	VCR,		//需配置VCR
//	A3H			//需写0xa3:GD25Q64系列
//};

#define VCR		0x1
#define QE		0x2
#define A3H		0x4

struct gxspi_xip {
	//enum xip_enum		xip_mode;			//vcr 
	cyg_uint32 			xip_mode;
	struct dummy_cycle 	dummy_value;
};

typedef struct gxspi_flash_params {
    cyg_uint32 sector_size;   	// Number of pages in a sector.
    cyg_uint32 sector_count;  	// Number of sectors on device.  
    cyg_uint32 addr_width;		// 3byte or 4bytes addr
    cyg_uint32 jedec_id;      	// 3 byte JEDEC identifier for this device.
    cyg_uint32 ext_id;      	// 2 byte ext identifier for this device.
	void *priv;					//private parameter
    struct gxspi_range *prange;//pointer to spi_writeprotect_addrrange
    //cyg_uint32 protect_size[8];	//bp2 bp1 bp0 :protect size of the flash.
    cyg_uint32 count;		 //sizeof(prange)/sizeof(struct gxspi_range) 
    cyg_uint32 param;		 //bottom or top protect support: &0x01 bottom;&0x08 top support.
    /*if you want open following struct, you must modify flash.h make both agree.*/
	//struct gxspi_xip xip_param;    
} gxspi_params;

int new_spi_flash_support(gxspi_params new_flash);
//-----------------------------------------------------------------------------
// Macro used to generate a flash device object with the default M25PXX 
// settings.  Even though the block info data structure is declared here, the 
// details are not filled in until the device type is inferred during 
// initialisation.  This also applies to the 'end' field which is calculated 
// using the _start_ address and the inferred size of the device.
// _name_   is the root name of the instantiated data structures.
// _start_  is the base address of the device - for SPI based devices this can
//          have an arbitrary value, since the device is not memory mapped.
// _spidev_ is a pointer to a SPI device object of type cyg_spi_device.  This
//          is not typechecked during compilation so be careful!

#define CYG_GX31XX_SPI_FLASH_DRIVER(_name_, _start_, _spidev_)  \
struct cyg_flash_block_info _name_ ##_block_info; 				\
CYG_FLASH_DRIVER(_name_, &cyg_devs_gxspi_flash_funs, 0,			\
  _start_, _start_, 1, & _name_ ##_block_info, (void*) _spidev_)
//=============================================================================
#endif // GX3110_SPI_FLASH_H
