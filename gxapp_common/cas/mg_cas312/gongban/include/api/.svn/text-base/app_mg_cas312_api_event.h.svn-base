#ifndef __APP_MG_CAS312_API_EVENT_H__
#define __APP_MG_CAS312_API_EVENT_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "app_common_play.h"
#include "app_common_porting_stb_api.h"
#include "app_mg_cas312_api_pop.h"

typedef struct 
{
	uint16_t wStatus; /* 0 --Çå³ý£¬1 -- ÏÔÊ¾¿¨ºÅ 2 -- ÏÔÊ¾ECMIDºÍ¿¨ºÅ*/
	uint16_t wShowStatus; /* 0 --Òþ²Ø×´Ì¬£¬1 -- ÏÔÊ¾×´Ì¬*/
	char  msg[64];
}mg_ca312_finger_data_st; 


typedef struct 
{
	u_int32_t time;//µ¥Î»s
	char  msg[64];
}mg_ca312_ppv_data_st; 

int32_t app_mg_cas312_gxmsg_ca_on_event_finger(GxMsgProperty0_OnEvent* event);
int32_t app_mg_cas312_finger_exec(void);
int32_t app_mg_cas312_gxmsg_ca_on_event_ippv_logo(GxMsgProperty0_OnEvent* event);
int32_t app_mg_cas312_msg_show(GxMsgProperty0_OnEvent* event);

#ifdef __cplusplus
}
#endif

#endif 

