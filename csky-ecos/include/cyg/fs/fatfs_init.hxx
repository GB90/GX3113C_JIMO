/************************************************************************************
																					
	Copyright (C) 2004 Medeli Electric Industry	Co., LTD.							
																					
	System Name	:	NationalChip													
	File   Name	:	fatfs_init.hxx												
	Revision	:	1.00															
	Date		:	2009.09.22	
    author      :   Haibo Gao
	usage       :   #include <cyg/fs/fatfs_init.hxx>
************************************************************************************/
#ifndef	FATFS_INIT_HXX
#define	FATFS_INIT_HXX
#include <cyg/fs/fatfs_ff.h>
extern "C" void fatfs_register(void);

class fs_cp_select_US_class {					//cp437
public:
	fs_cp_select_US_class(void)
	{
		fs_SBCS_US_select();
		_CODE_PAGE=437;
	}
};

class fs_cp_select_SBCS_class {					//others SBCS cp
public:
	fs_cp_select_SBCS_class(unsigned int _CODE_PAGE_t)
	{
		fs_SBCS_language_select(_CODE_PAGE_t);
		_CODE_PAGE=_CODE_PAGE_t;
	}
};

class fs_cp_select_Simplified_Chanese_class {	//cp936
public:
	fs_cp_select_Simplified_Chanese_class(void)
	{
		fs_Simplified_Chanese_language_select();
		_CODE_PAGE=936;
	}
};

class fs_cp_select_Traditional_Chanese_class {	//cp950
public:
	fs_cp_select_Traditional_Chanese_class(void)
	{
		fs_Traditional_Chanese_language_select();
		_CODE_PAGE=950;
	}
};

class fs_cp_select_Japanese_class {				//cp932
public:
	fs_cp_select_Japanese_class(void)
	{
		fs_Japanese_language_select();
		_CODE_PAGE=932;
	}
};

class fs_cp_select_Korean_class {				//cp949
public:
	fs_cp_select_Korean_class(void)
	{
		fs_Korean_language_select();
		_CODE_PAGE=949;
	}
};

class fatfs_init_class {						//fatfs init
public:
   fatfs_init_class(void) 
	{ 
        fatfs_register();
    }
};


class fs_utf8_class {					//utf8
public:
	fs_utf8_class(void)
	{
		fs_utf8_select();
	}
};


#endif	/* End of gx3110_sd_init.h */

