#ifndef WALLCLOCK_H
#define WALLCLOCK_H
/*==========================================================================
name	:    wallclock.h
Author  :    Haibo Gao 
Date    :    2010-10-13
usage   :    #include <cyg/io/wallclock.h>
==========================================================================*/
#include <cyg/infra/cyg_type.h>         // Common type definitions and support

externC 
cyg_uint32 simple_mktime(cyg_uint32 year, cyg_uint32 month, cyg_uint32 mday, cyg_uint32 hour, cyg_uint32 minute, cyg_uint32 second);
#endif
