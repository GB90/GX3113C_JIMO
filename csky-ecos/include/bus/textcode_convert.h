#ifndef __TEXTCODE_CONVERT_H__
#define __TEXTCODE_CONVERT_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "gxcore.h"

uint16_t convert_unicode2gbk(uint16_t uncs);
int  convert_gbk2utf8(const char  * s, int len, char * pbuf, int bufsize);
int  convert_gbk2unicode (const char *s,int len, uint16_t *pBuffer,int BufferSize);
int  convert_unicode2utf8(const uint16_t  * s, int len, char * pbuf, int bufsize);
int  convert_utf82unicode(const char * s, int Len, uint16_t * pbuf, int bufsize);

#ifdef __cplusplus
}
#endif

#endif

