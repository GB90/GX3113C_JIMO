/**
 *
 * @file        app_by_cas_api_baseinfo.h
 * @brief
 * @version     1.1.0
 * @date        11/30/2012 09:47:49 AM
 * @author      zhouhuaming (zhouhm), zhuohm@nationalchip.com
 *
 */
#ifndef __APP_DIVICAS_API_POP_H__
#define __APP_DIVICAS_API_POP_H__
#ifdef __cplusplus
extern "C" {
#endif

#include "DiviGuardCa.h"

typedef struct {
	uint8_t wStatus; /* 0 --不需要显示，1-- 需要显示*/
	uint8_t roll_status; /*0 -- 当前滚动状态，1-- 当前非滚动状态*/
	int32_t wTimes;       /*已滚动次数*/
	UINT32 param1;
	UINT32 param2;	
} divitone_cas_rolling_message_st; 

typedef struct {
	uint8_t wStatus; 
	uint8_t wShowStatus;
	uint8_t uCount;
} divitone_cas_email_st; 

typedef struct {
	UINT32 param1;
	UINT32 param2;	
} divitone_cas_active_child_st; 

//int32_t app_divitone_cas_gxmsg_ca_on_event_exec(GxMessage * msg);
//uint8_t app_divitone_cas_api_is_ca_pop_msg_type(uint8_t type);
// int32_t app_divitone_cas_api_pop_msg_exec(uint8_t type);
// int32_t app_divitone_cas_api_osd_exec(void);

#ifdef __cplusplus
}
#endif
#endif /*__APP_DIVICAS_API_POP_H__*/

