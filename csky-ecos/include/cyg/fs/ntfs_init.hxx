/************************************************************************************
	Copyright (C) 2004 Medeli Electric Industry	Co., LTD.
	System Name	:	NationalChip
	File   Name	:	ntfs_init.hxx
	Revision	:	1.00
	Date		:	2010.07.20
    author      :   yuqj
	usage       :   #include <ntfs_init.hxx>
************************************************************************************/
#ifndef	NTFS_INIT_HXX
#define	NTFS_INIT_HXX

extern "C" void ntfs_register(void);

class ntfs_init_class {
 public:
	ntfs_init_class(void) {
		ntfs_register();
}};

#endif				/* End of ntfs_init.hxx */
