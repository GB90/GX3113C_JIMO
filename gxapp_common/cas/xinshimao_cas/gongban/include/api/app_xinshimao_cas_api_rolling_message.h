#ifndef __APP_XINSHIMAO_CAS_API_ROLLING_MESSAGE_H__
#define __APP_XINSHIMAO_CAS_API_ROLLING_MESSAGE_H__
#ifdef __cplusplus
extern "C" {
#endif

#include "app_common_play.h"
#include <gxos/gxcore_os_core.h>

#define CA_MAX_LEN_OSD          240
#define CA_OSD_TOP              0x01  /* OSD风格：显示在屏幕上方 */

typedef enum
{
	XINSHIMAO_MSG_OSD=0,
	XINSHIMAO_MSG_MSG,
	XINSHIMAO_MSG_MAX
}XINSHIMAO_MSG_TYPE;

typedef struct {
	uint8_t wStatus; /* 0 --不需要显示，1-- 需要显示*/
	uint8_t roll_status; /*0 -- 当前滚动状态，1-- 当前非滚动状态*/
	uint8_t byStyle; /*osd滚动消息在全屏界面位置类型*/
	uint8_t message_type; /* 0 --osd，1-- msg*/
	int32_t wTimes;       /*已滚动次数*/
    handle_t mutex;
	uint8_t content[CA_MAX_LEN_OSD];
} xinshimao_cas_rolling_message_st; 

void xinshimao_cas_sent_osdmsg(uint8_t style,char *msg);
void xinshimao_cas_sent_msg(char *msg);
int8_t app_xinshimao_cas_rolling_message_init(void);
int32_t app_xinshimao_cas_gxmsg_ca_on_event_msg(GxMsgProperty0_OnEvent* event);
int32_t app_xinshimao_cas_gxmsg_ca_on_event_rolling(GxMsgProperty0_OnEvent* event);
int32_t app_xinshimao_cas_rolling_message_exec(void);


#ifdef __cplusplus
}
#endif
#endif 

