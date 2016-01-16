/************************************************************************************
																					
	Copyright (C) 2004 Medeli Electric Industry	Co., LTD.							
																					
	System Name	:	NationalChip													
	File   Name	:	gx3110_sd_init.h												
	Revision	:	1.00															
	Date		:	2009.09.22	
    author      :   gaohaibo
	usage       :   #include <cyg/io/gx3110_sd_init.hxx>
************************************************************************************/
#ifndef	GX3110_SD_INIT_HXX
#define	GX3110_SD_INIT_HXX

extern "C" void gx3110_sd_mod_init(void);

class cyg_sd_init_class {		//sd card
public:
   cyg_sd_init_class(void) 
	{ 
        gx3110_sd_mod_init();
        
	}
};

#endif	/* End of gx3110_sd_init.h */

