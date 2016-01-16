/************************************************************************************
																					
	Copyright (C) 2004 Medeli Electric Industry	Co., LTD.							
																					
	System Name	:	NationalChip													
	File   Name	:	gx_flashio_init.h												
	Revision	:	1.00															
	Date		:	2009.09.22	
    author      :   gaohaibo
	usage       :   #include <cyg/io/gx_flashio_init.hxx>
************************************************************************************/
#ifndef	GX_FLASHIO_INIT_HXX
#define	GX_FLASHIO_INIT_HXX

extern "C" void flashio_init(void);

class cyg_flashio_init_class {		//flash io entry
public:
   cyg_flashio_init_class(void) 
	{ 
        flashio_init(); 
	}
};

#endif	/* End of gx_flashio_init.h */

