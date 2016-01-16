/**
 *
 * @file        app_mg_cas_api_pop.h
 * @brief
 * @version     1.1.0
 * @date        04/15/2014 
 * @author      zhouhuaming (zhouhm), zhuohm@nationalchip.com
 *
 */
#ifndef __APP_MG_CAS312_API_POP_H__
#define __APP_MG_CAS312_API_POP_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "app_common_play.h"
#include <gxos/gxcore_os_core.h>

#define MG312_CA_MAX_LEN_OSD          (240)
#define MG312_CA_OSD_TOP              (0x01)
#define MG312_CA_OSD_BOTTOM           (0x02)

typedef enum
{
	MG312_MSG_OSD = 0,
	MG312_MSG_MSG,
	MG312_MSG_MAX
}MG312_MSG_TYPE;

typedef struct 
{
	uint8_t wStatus; /* 0 --不需要显示，1-- 需要显示*/
	uint8_t roll_status; /*0 -- 当前滚动状态，1-- 当前非滚动状态*/
	uint8_t byStyle; /*osd滚动消息在全屏界面位置类型*/
	uint8_t message_type; /* 0 --osd，1-- msg*/
	int32_t wTimes;       /*已滚动次数*/
    handle_t mutex;
	uint8_t content[MG312_CA_MAX_LEN_OSD];
} mg_cas312_rolling_message_st; 

typedef struct 
{
	int32_t status; /*1 -- show, 0 -- hide*/
} mg_cas312_pre_auth_st; 

typedef struct 
{
	int32_t status; /*1 -- show, 0 -- hide*/
} mg_cas312_auth_st; 

typedef enum 
{
	MGCA312_ENTITLE_INFO_ID1 = 0,
    MGCA312_ENTITLE_INFO_ID2,
}MGCA312_ENTITLE_INFO_ID_E;


int32_t app_mg_cas312_gxmsg_ca_on_event_exec(GxMessage *msg);
uint8_t app_mg_cas312_api_is_ca_pop_msg_type(uint8_t type);
int32_t app_mg_cas312_api_pop_msg_exec(uint8_t type);
int32_t app_mg_cas312_api_osd_exec(void);

#ifdef __cplusplus
}
#endif

#endif /*__APP_MG_CAS312_API_POP_H__*/

