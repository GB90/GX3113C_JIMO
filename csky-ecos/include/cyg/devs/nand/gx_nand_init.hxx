/************************************************************************************
	Copyright (C) 2004 Medeli Electric Industry	Co., LTD.							
	System Name	:	NationalChip													
	File   Name	:	nand_init.hxx												
	Revision	:	1.00															
	Date		:	2009.11.26
    author      :   yuqj 
	usage       :   #include <nand_init.hxx>
************************************************************************************/
#ifndef	NAND_INIT_HXX
#define	NAND_INIT_HXX
#include <cyg/nand/nand.h>
#include <cyg/nand/nand_devtab.h>

extern "C" void gx_nand_mod_init(void);
__externC void cyg_nand_initx(cyg_nand_printf pf);
class nand_init_class {		
public:
   nand_init_class(void) 
	{ 
        gx_nand_mod_init();
		#if defined(CYGSEM_IO_NAND_DEBUG_FN_DEFAULT)
            cyg_nand_initx(CYGSEM_IO_NAND_DEBUG_FN_DEFAULT);
		#else
            cyg_nand_initx(NULL);
		#endif
    }
};

#endif	/* End of nand_init.hxx */

