/************************************************************************************
	Copyright (C) 2004 Medeli Electric Industry	Co., LTD.							
	System Name	:	NationalChip													
	File   Name	:	yaffs_init.hxx												
	Revision	:	1.00															
	Date		:	2009.11.26
    author      :   yuqj 
	usage       :   #include <yaffs_init.hxx>
************************************************************************************/
#ifndef	YAFFS_INIT_HXX
#define	YAFFS_INIT_HXX

extern "C" void yaffs_register(void);

class yaffs_init_class {		
public:
   yaffs_init_class(void) 
	{ 
        yaffs_register();
    }
};

#endif	/* End of yaffs_init.hxx */

