/*=============================================================================
//
//      gxmtc.h
//			
//      mtc functions
//
//=============================================================================
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
//=============================================================================
//#####DESCRIPTIONBEGIN####
//
// Author(s):    baiwx 
// Contributors: gthomas, jskov
// Date:         2011-04-19
// Purpose:      Support mtc 
// Description:  This file contains mtc ENCRYPT and DECRYPT
// Usage:        #include <cyg/hal/gxmtc.h>
//
//####DESCRIPTIONEND####
//
//===========================================================================*/

#include <stdio.h>
#include <cyg/hal/hal_cache.h>

enum
{
    GXMTC_DES_ARITH	= 0,
    GXMTC_3DES_ARITH,
    GXMTC_AES128_ARITH,
    GXMTC_AES192_ARITH,
    GXMTC_AES256_ARITH,
    GXMTC_MULTI2_ARITH
};

enum
{
    GXMTC_ENCRYPT	=0,
    GXMTC_DECRYPT	=1
};

enum
{
    GXMTC_ECB_MODE      = 0,
    GXMTC_CBC_MODE      = 1,
    GXMTC_CFB_MODE      = 2,
    GXMTC_OFB_MODE      = 3,
    GXMTC_CTR_MODE      = 4
};

typedef struct gx_mtc_s
{
    unsigned int                 Arithmetic;
    unsigned int                 CipherMode;
    unsigned int                 DecryptSel;     //TRUE:decrypt,FALSE:encrypt
    unsigned int                 RoundNumber;    //from 2 to 32
    void                         *pKey;
    unsigned int                 MtcBaseAddress;
    unsigned int                 Handle;
    unsigned int                 DataLen;
    unsigned int                 KeyLen;
}gx_mtc_t;

typedef struct mtc_info_s
{
	unsigned char *key;
	unsigned int key_len;
	unsigned char *input;
	unsigned int input_len;
	unsigned char *output;
	unsigned int arithmetic; 
	unsigned int ciphermode; 
	unsigned int decryptsel;
}mtc_info;

typedef struct gxmtc_reg_s
{
    unsigned int                 rMTC_CTRL1;
    unsigned int                 rMTC_KEY1_H;
    unsigned int                 rMTC_KEY1_L;
    unsigned int                 rMTC_KEY2_H;
    unsigned int                 rMTC_KEY2_L;
    unsigned int                 rMTC_KEY3_H;
    unsigned int                 rMTC_KEY3_L;
    unsigned int                 rMTCR_SDR_START_ADDR;
    unsigned int                 rMTCW_SDR_START_ADDR;
    unsigned int                 rMTCR_SDR_DATA_LEN;
    unsigned int                 rMTC_CTRL2;
    unsigned int                 rMTC_KEY4_H;
    unsigned int                 rMTC_KEY4_L;
    unsigned int                 rCTR1;
    unsigned int                 rCTR2;
    unsigned int                 rCTR3;
    unsigned int                 rCTR4;
    unsigned int                 rIV1;
    unsigned int                 rIV2;
    unsigned int                 rIV3;
    unsigned int                 rIV4;
    unsigned int                 rMULT_DATA_KEY_H;
    unsigned int                 rMULT_DATA_KEY_L;
}gxmtc_reg_t;


#define MAX_BUFFER_SIZE 64
#define GXMTC_BASEADDR      0x01014000+0xd0000000

extern void swapkeydata(unsigned char *input, unsigned char *output);
extern void swapdata(unsigned char *input, unsigned char *output);
extern int gx_mtc_init(mtc_info *gx_mtc_info);
extern int gxmtc_read(unsigned int handle , unsigned char   *pDataOut);
extern int gxmtc_write(unsigned int handle, unsigned char  *pdatain);
extern int gxmtc_open(gx_mtc_t *pgxmtc, unsigned int *phandle);
