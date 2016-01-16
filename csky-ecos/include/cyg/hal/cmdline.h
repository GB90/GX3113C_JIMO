/*=============================================================================
//
//      cmdline.h
//
//      cmdline functions
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
// Date:         2011-05-11
// Purpose:      Support cmdline
// Description:  This file contains cmdline info
// Usage:        #include <cyg/hal/cmdline.h>
//
//####DESCRIPTIONEND####
//
//===========================================================================*/


#define TOLOWER(x) ((x) | 0x20)

#define BANKS_MAX  10

struct gx_mem {
        unsigned int start;
        unsigned int size;
};

struct gx_mem_hole_info {
	int mem_num;
    struct gx_mem bank[BANKS_MAX];
};

struct mem_info
{
	struct gx_mem_hole_info *gxmem;
	cyg_uint32 mem_size;
	cyg_uint32 surface_size;
};

extern struct mem_info *hal_mem_get(char *str);
extern void string_info(char *str ,char *mem_str ,char *gxmem_str, char *videomem_str, char *fbmem_str, char *av_extend_str, char *surface);
extern struct gx_mem_hole_info *early_mem(char **p);
extern unsigned long memparse(char *ptr, char **retptr);
extern unsigned long simple_strtoull(const char *cp,char **endp,unsigned int base);
