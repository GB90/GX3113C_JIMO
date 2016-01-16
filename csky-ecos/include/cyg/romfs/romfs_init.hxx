/************************************************************************************
	Copyright (C) 2004 Medeli Electric Industry	Co., LTD.							
	System Name	:	NationalChip													
	File   Name	:	romfs_init.hxx												
	Revision	:	1.00															
	Date		:	2009.11.26
    author      :   yuqj 
	usage       :   #include <romfs_init.hxx>
************************************************************************************/
#ifndef	ROMFS_INIT_HXX
#define	ROMFS_INIT_HXX

extern "C" void romfs_register(void);

class romfs_init_class {		
public:
   romfs_init_class(void) 
	{ 
        romfs_register();
    }
};

#endif	/* End of romfs_init.hxx */

