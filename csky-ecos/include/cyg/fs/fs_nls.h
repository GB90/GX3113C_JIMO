/************************************************************************************
																					
	Copyright (C) 2004 Medeli Electric Industry	Co., LTD.							
																					
	System Name	:	NationalChip													
	File   Name	:	fatfs_nls.h												
	Revision	:	1.00															
	Date		:	2010.07.28	
    author      :   Haibo Gao
	usage       :  #include <cyg/fs/fatfs_nls.h>
************************************************************************************/
#ifndef NLS_H
#define NLS_H	

/* unicode character */

struct nls_table {
	char *charset;
	char *alias;
	int (*uni2char) (unsigned short uni, unsigned char *out, int boundlen);
	int (*char2uni) (const unsigned char *rawstring, int boundlen,
			unsigned short *uni);
	unsigned char *charset2lower;
	unsigned char *charset2upper;
	struct nls_table *next;
};
struct shortname_info {
	unsigned char lower:1,
		      upper:1,
		      valid:1;
};
/* this value hold the maximum octet of charset */
#define NLS_MAX_CHARSET_SIZE 6 /* for UTF-8 */
#define INIT_SHORTNAME_INFO(x)	do {		\
	(x)->lower = 1;				\
	(x)->upper = 1;				\
	(x)->valid = 1;				\
} while (0)

/* nls.c */
extern int register_nls(struct nls_table *);
extern int unregister_nls(struct nls_table *);
extern struct nls_table *load_nls(char *);
extern void unload_nls(struct nls_table *);
extern struct nls_table *load_nls_default(void);
extern struct nls_table *load_nls_utf8(void);
extern void unload_nls_utf8(void);
extern int utf8_mbtowc(unsigned short *, const unsigned char *, int);
extern int utf8_mbstowcs(unsigned short *, const unsigned char *, int);
extern int utf8_wctomb(unsigned char *, unsigned short, int);
extern int utf8_wcstombs(unsigned char *, const unsigned short *, int);
extern cyg_uint32 to_shortname_char(struct nls_table *nls,
				    cyg_uint8 *buf, cyg_uint32 buf_size,
				    cyg_uint16 *src, struct shortname_info *info);

cyg_uint16 fat_bad_char(cyg_uint16 w);
cyg_uint16 fat_replace_char( cyg_uint16 w);
cyg_uint16 fat_skip_char( cyg_uint16 w);
int fat_valid_longname(const char *name, cyg_uint32 len);
int fat_is_used_badchars(const cyg_uint16 *s, int len);

static inline unsigned char nls_tolower(struct nls_table *t, unsigned char c)
{
	unsigned char nc = t->charset2lower[c];

	return nc ? nc : c;
}

static inline unsigned char nls_toupper(struct nls_table *t, unsigned char c)
{
	unsigned char nc = t->charset2upper[c];

	return nc ? nc : c;
}

static inline int nls_strnicmp(struct nls_table *t, const unsigned char *s1,
		const unsigned char *s2, int len)
{
	while (len--) {
		if (nls_tolower(t, *s1++) != nls_tolower(t, *s2++))
			return 1;
	}

	return 0;
}

#endif 

