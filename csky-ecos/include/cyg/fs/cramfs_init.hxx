/************************************************************************************
																					
	Copyright (C) 2010 Nationchip Co., LTD.							
																					
	System Name	:	NationalChip													
	File   Name	:	cramfs_init.hxx												
	Revision	:	1.00															
	Date		:	2010.04.16	
    author      :   gaohaibo
	usage       :   #include <cyg/fs/cramfs_init.hxx>
************************************************************************************/
#ifndef	CRAMFS_INIT_HXX
#define	CRAMFS_INIT_HXX

extern "C" void cramfs_register(void);

class cramfs_init_class {		//cramfs
public:
   cramfs_init_class(void) 
	{ 
        cramfs_register();
    }
};

#endif	/* End of gx3110_sd_init.h */

