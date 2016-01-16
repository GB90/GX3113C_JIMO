/************************************************************************************
	Copyright (C) 2004 Medeli Electric Industry	Co., LTD.							
	System Name	:	NationalChip													
	File   Name	:	ramfs_init.hxx												
	Revision	:	1.00															
	Date		:	2009.11.26
    author      :   yuqj 
	usage       :   #include <cyg/ramfs/ramfs_init.hxx>
************************************************************************************/
#ifndef	RAMFS_INIT_HXX
#define	RAMFS_INIT_HXX

extern "C" void ramfs_register(void);

class ramfs_init_class {		
public:
   ramfs_init_class(void) 
	{ 
        ramfs_register();
    }
};

#endif	/* End of ramfs_init.hxx */

