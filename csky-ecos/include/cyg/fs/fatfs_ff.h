/************************************************************************************
																					
	Copyright (C) 2004 Medeli Electric Industry	Co., LTD.							
																					
	System Name	:	NationalChip													
	File   Name	:	fatfs_ff.h												
	Revision	:	1.00															
	Date		:	2010.07.28	
    author      :   Haibo Gao
	usage       :   #include <cyg/fs/fatfs_ff.h>
************************************************************************************/
#ifndef FATFS_FF_H
#define FATFS_FF_H
#include <cyg/infra/cyg_type.h>
#include <pkgconf/system.h>
#include <cyg/infra/diag.h>
#include <cyg/error/codes.h>
#ifdef CYGPKG_HAL_CK
#define __BIG_ENDIAN__  1
static inline unsigned short swab16(unsigned short x)
{
	return (x<<8 | x>>8);
}
#endif

typedef   cyg_uint16    WCHAR;
typedef   cyg_uint32    UINT;
typedef   unsigned char	BYTE;
typedef unsigned long	DWORD;
typedef unsigned short	WORD;
/*
/  unicode code page reference:
/   437   U.S. (OEM)
/   720   Arabic (OEM)
/   1256  Arabic (Windows)
/   737   Greek (OEM)
/   1253  Greek (Windows)
/   1250  Central Europe (Windows)
/   775   Baltic (OEM)
/   1257  Baltic (Windows)
/   850   Multilingual Latin 1 (OEM)
/   852   Latin 2 (OEM)
/   1252  Latin 1 (Windows)
/   855   Cyrillic (OEM)
/   1251  Cyrillic (Windows)
/   866   Russian (OEM)
/   857   Turkish (OEM)
/   1254  Turkish (Windows)
/   858   Multilingual Latin 1 + Euro (OEM)
/   862   Hebrew (OEM)
/   1255  Hebrew (Windows)
/   874   Thai (OEM, Windows)
/   1258  Vietnam (OEM, Windows)
/   932   Japanese Shift-JIS
/   936   Simplified Chinese GBK
/   949   Korean
/   950   Traditional Chinese Big5
*/

externC void *
memcpy( void *s1, const void *s2, size_t n );

typedef struct {
	int _DF1S;	/* DBC 1st byte range 1 start */
	int _DF1E;	/* DBC 1st byte range 1 end */
	int _DF2S;	/* DBC 1st byte range 2 start */
	int _DF2E;	/* DBC 1st byte range 2 end */
	int _DS1S;	/* DBC 2nd byte range 1 start */
	int _DS1E;	/* DBC 2nd byte range 1 end */
	int _DS2S;	/* DBC 2nd byte range 2 start */
	int _DS2E;	/* DBC 2nd byte range 2 end */
	int _DS3S;	/* DBC 2nd byte range 3 start */
	int _DS3E;	/* DBC 2nd byte range 3 end */
} fs_oem_bound;
typedef struct {
		 WCHAR (*ff_convert) (	/* Converted code, 0 means conversion error */
			WCHAR	src,	/* Character code to be converted */
			UINT	dir		/* 0: Unicode to OEMCP, 1: OEMCP to Unicode */
		);
		WCHAR (*ff_wtoupper) (	/* Upper converted character */
			WCHAR chr		/* Input character */
		);
} fs_oem_uni_convert;
extern fs_oem_uni_convert 	local_oem_uni_convert;
extern fs_oem_bound 		local_oem_bound;
extern BYTE *_EXCVT;
extern UINT _CODE_PAGE;

int Is_chinese(BYTE *c);
int Is_japanese(BYTE *c);
int Is_Korean(BYTE *c);
BYTE IsDBCS1(BYTE c);
BYTE IsDBCS2(BYTE c);
int Is_UTF8(void);
void CCSBC_init(UINT _CODE_PAGE);
externC void CCSBC_US_init(void);
void fs_oemuni_init(UINT _CODE_PAGE);
externC WCHAR ff_convert (	/* Converted code, 0 means conversion error */
	WCHAR	src,	/* Character code to be converted */
	UINT	dir		/* 0: Unicode to OEMCP, 1: OEMCP to Unicode */
);
externC WCHAR ff_wtoupper (	/* Upper converted character */
	WCHAR chr		/* Input character */
);
externC WCHAR ff_convert_cp932 (	/* Converted code, 0 means conversion error */
	WCHAR	src,	/* Character code to be converted */
	UINT	dir		/* 0: Unicode to OEMCP, 1: OEMCP to Unicode */
);
externC WCHAR ff_wtoupper_cp932 (	/* Upper converted character */
	WCHAR chr		/* Input character */
);
externC WCHAR ff_convert_cp936 (	/* Converted code, 0 means conversion error */
	WCHAR	src,	/* Character code to be converted */
	UINT	dir		/* 0: Unicode to OEMCP, 1: OEMCP to Unicode */
);
externC WCHAR ff_wtoupper_cp936 (	/* Upper converted character */
	WCHAR chr		/* Input character */
);
externC WCHAR ff_convert_cp949 (	/* Converted code, 0 means conversion error */
	WCHAR	src,	/* Character code to be converted */
	UINT	dir		/* 0: Unicode to OEMCP, 1: OEMCP to Unicode */
);
externC WCHAR ff_wtoupper_cp949 (	/* Upper converted character */
	WCHAR chr		/* Input character */
);
externC WCHAR ff_convert_cp950 (	/* Converted code, 0 means conversion error */
	WCHAR	src,	/* Character code to be converted */
	UINT	dir		/* 0: Unicode to OEMCP, 1: OEMCP to Unicode */
);
externC WCHAR ff_wtoupper_cp950 (	/* Upper converted character */
	WCHAR chr		/* Input character */
);
externC void fs_SBCS_language_select(UINT _CODE_PAGE);
externC void fs_Japanese_language_select(void);
externC void fs_Simplified_Chanese_language_select(void);
externC void fs_Korean_language_select(void);
externC void fs_Traditional_Chanese_language_select(void);
externC void fs_SBCS_US_select(void);
externC Cyg_ErrNo f_mkfs (
	const char *drv,			/* device name */
	cyg_uint8 partition,		/* Partitioning rule 0:FDISK, 1:SFD.we usually use: 1 */
	cyg_uint16 allocsize		/* Allocation unit size [bytes] */
);
externC void fs_utf8_select(void);

#endif
