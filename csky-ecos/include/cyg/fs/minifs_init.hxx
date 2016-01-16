/************************************************************************************
																					
	Copyright (C) 2010 Nationchip Co., LTD.							
																					
	System Name	:	NationalChip													
	File   Name	:	minifs_init.hxx												
	Revision	:	1.00															
	Date		:	2011.03.18
    author      :   gaohaibo
	usage       :   #include <cyg/fs/minifs_init.hxx>
************************************************************************************/
#ifndef	MINIFS_INIT_HXX
#define	MINIFS_INIT_HXX

extern "C" void minifs_register(void);

class minifs_init_class {		//minifs
public:
   minifs_init_class(void)
	{
        minifs_register();
    }
};

#endif	

