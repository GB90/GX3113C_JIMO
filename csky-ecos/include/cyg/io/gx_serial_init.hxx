/************************************************************************************
																					
	Copyright (C) 2004 Medeli Electric Industry	Co., LTD.							
																					
	System Name	:	NationalChip													
	File   Name	:	gx3110_serial_init.hxx												
	Revision	:	1.00															
	Date		:	2009.09.22	
    author      :   gaohb
	usage       :   #include <cyg/io/gx3110_serial_init.hxx>
************************************************************************************/
#ifndef	GX3110_SERIAL_INIT_H
#define	GX3110_SERIAL_INIT_H

extern "C" void serial_mod_init(void);
class cyg_serial_init_class {
public:
   cyg_serial_init_class(void)
	{
        serial_mod_init();
	}
};

#endif	/* End of gx3110_serial_init.hxx */
