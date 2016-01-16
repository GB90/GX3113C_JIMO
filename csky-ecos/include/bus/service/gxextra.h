#ifndef __GX_EXTRA_H__
#define __GX_EXTRA_H__

#include "gxbus.h"
#include <time.h>

typedef enum {
	TDT_TIME,            
	TOT_TIME
}FOUNTAIN;

typedef struct
{
	int         time_zone;
	time_t      utc_second;
	FOUNTAIN    time_fountain;
}GxMsgProperty_ExtraTimeOk;

typedef struct
{
	uint32_t ts_src;    // 通过哪一路TS去过滤tdt或者tot，TS1-0  TS2-1  TS3-2 ...
}GxMsgProperty_ExtraSyncTime;

#endif /* __GX_EXTRA_H__ */


