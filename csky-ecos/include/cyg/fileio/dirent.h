#ifndef CYGONCE_FILEIO_DIRENT_H
#define CYGONCE_FILEIO_DIRENT_H
/*========================================================================
//
//      dirent.h
//
//      POSIX directory functions
//
//========================================================================
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
//========================================================================
//#####DESCRIPTIONBEGIN####
//
// Author(s):     nickg
// Contributors:  
// Date:          2000-06-26
// Purpose:       This file provides the macros, types and functions
//                for directory operations required by POSIX 1003.1.
// Description:   Implementations of the types needed for directory
//                reading.
// Usage:         #include <dirent.h>
//
//####DESCRIPTIONEND####
//
//======================================================================
*/

#include <pkgconf/isoinfra.h>
#include <cyg/fileio/fileio.h>
#include <sys/types.h>
#include <limits.h>    
#include <time.h>        /* time_t */

//======================================================================

#ifdef __cplusplus
extern "C" {
#endif

//======================================================================
// dirent structure.
    
struct dirent
{
#ifdef CYGPKG_FILEIO_DIRENT_DTYPE
  
    mode_t      d_type; // Only supported with FATFS, RAMFS, ROMFS,
                        // and JFFS2.
                        // d_type is not part of POSIX so
                        // should be used with caution.
#endif
    char        d_name[NAME_MAX+1];
  	off_t   	st_size;     /* File size (regular files only) */
    time_t  	st_atime;    /* Last access time */
    time_t  	st_mtime;    /* Last data modification time */
    time_t  	st_ctime;    /* Last file status change time */
	cyg_fs_attrib_t    attrib;     // Attribute bits for DOS compatability

};

//======================================================================
// DIR pointer object. 
    
typedef void *DIR;

//======================================================================
    
#ifdef __cplusplus
}   /* extern "C" */
#endif

//======================================================================
#endif /* CYGONCE_FILEIO_DIRENT_H multiple inclusion protection */
/* EOF dirent.h */
