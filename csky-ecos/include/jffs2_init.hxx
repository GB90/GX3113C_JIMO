/************************************************************************************
	Copyright (C) 2004 Medeli Electric Industry	Co., LTD.							
	System Name	:	NationalChip													
	File   Name	:	jffs2_init.hxx												
	Revision	:	1.00															
	Date		:	2009.11.26
    author      :   yuqj 
	usage       :   #include <jffs2_init.hxx>
************************************************************************************/
#ifndef	JFFS2_INIT_HXX
#define	JFFS2_INIT_HXX

extern "C" void jffs2_register(void);

class jffs2_init_class {		
public:
   jffs2_init_class(void) 
	{ 
        jffs2_register();
    }
};

#endif	/* End of jffs2_init.hxx */

