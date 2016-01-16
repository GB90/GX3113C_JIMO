/**
 *
 * @file        app_dvt_cas_api_pop.h
 * @brief
 * @version     1.1.0
 * @date        11/29/2013 
 * @author      zhouhuaming (zhouhm), zhuohm@nationalchip.com
 *
 */
#ifndef __APP_DVT_CAS_API_POP_H__
#define __APP_DVT_CAS_API_POP_H__
#ifdef __cplusplus
extern "C" {
#endif
//#include "CDCASS.h"

typedef struct {
	uint16_t wStatus; /* 0 --清除，1 -- 显示卡号 */
	uint16_t wShowStatus; /* 0 --隐藏状态，1 -- 显示状态*/
	uint16_t wEcmPid;
	uint32_t dwCardID;
	uint32_t position; /*随机显示位置索引*/
	uint32_t wDuration;
	uint32_t wusedTime;
} dvt_cas_finger_data_st; 

//#define ROLL_TITLE_MAX (100)
#define ROLL_CONTENT_MAX (1024)

typedef struct {
	uint8_t wStatus; /* 0 --不需要显示，1-- 需要显示*/
	uint8_t roll_status; /*0 -- 当前滚动状态，1-- 当前非滚动状态*/
	uint8_t byStyle; /*osd滚动消息在全屏界面位置类型*/
	uint8_t content[210];
} dvt_cas_rolling_message_st; 

typedef struct {
	uint8_t wStatus; /* 需要显示邮件图标类型*/
	uint8_t wShowStatus; /*当前显示状态*/
	uint8_t uCount; /* 显示累计次数，处理闪烁*/
	uint32_t emailID;
} dvt_cas_email_st; 

int32_t app_dvt_cas_gxmsg_ca_on_event_exec(GxMessage * msg);
uint8_t app_dvt_cas_api_is_ca_pop_msg_type(uint8_t type);
 int32_t app_dvt_cas_api_pop_msg_exec(uint8_t type);
 int32_t app_dvt_cas_api_osd_exec(void);



#ifdef __cplusplus
}
#endif
#endif /*__APP_DVT_CAS_API_POP_H__*/

