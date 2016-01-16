/*
 *   This file has been modified by Evatronix. 
 */
#ifndef _LINUX_LINKAGE_H
#define _LINUX_LINKAGE_H
#include <usb/arch/linkage.h>

#ifdef __cplusplus
#define CPP_ASMLINKAGE extern "C"
#else
#define CPP_ASMLINKAGE
#endif
#ifndef asmlinkage
#define asmlinkage CPP_ASMLINKAGE
#endif

#ifndef __ALIGN
#define __ALIGN         .align 4,0x90
#define __ALIGN_STR     ".align 4,0x90"
#endif

#ifndef FASTCALL
#define FASTCALL(x)     x
#define fastcall
#endif 
#endif

