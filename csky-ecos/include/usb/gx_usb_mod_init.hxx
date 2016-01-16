/************************************************************************************
																					
	Copyright (C) 2004 Medeli Electric Industry	Co., LTD.							
																					
	System Name	:	NationalChip													
	File   Name	:	gx_usb_mod_init.hxx												
	Revision	:	1.00															
	Date		:	2009.11.25	
    author      :   gaohb
	usage       :   #include <usb/gx_usb_mod_init.hxx>
************************************************************************************/
#ifndef	GX_USB_MOD_INIT_HXX
#define	GX_USB_MOD_INIT_HXX

extern "C" void gx_usb_mod_init(void);

class cyg_usb_init_class {		//usb
public:
   cyg_usb_init_class(void) 
	{ 
        gx_usb_mod_init();
        
	}
};

#endif	/* End of gx_usb_mod_init.hxx */

