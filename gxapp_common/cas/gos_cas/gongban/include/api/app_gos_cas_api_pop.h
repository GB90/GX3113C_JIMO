/**
 *
 * @file        app_gos_cas_api_pop.h
 * @brief
 * @version     1.1.0
 * @date        04/22/2014 
 * @author      zhouhuaming (zhouhm), zhuohm@nationalchip.com
 *
 */
#ifndef __APP_GOS_CAS_API_POP_H__
#define __APP_GOS_CAS_API_POP_H__
#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
	uint16_t wStatus; /* 0 --清除，1 -- 显示卡号 */
	uint16_t wShowStatus; /* 0 --隐藏状态，1 -- 显示状态*/
	uint16_t wEcmPid;
	uint32_t dwCardID;
	uint32_t position; /*随机显示位置索引*/
	uint32_t wDuration;
	uint32_t wusedTime;
	uint32_t version;
} gos_cas_finger_data_st; 

//#define ROLL_TITLE_MAX (100)
#define ROLL_CONTENT_MAX (1024)

typedef struct {
	uint8_t wStatus; /* 0 --不需要显示，1-- 需要显示*/
	uint8_t roll_status; /*0 -- 当前滚动状态，1-- 当前非滚动状态*/
	uint8_t byStyle; /*osd滚动消息在全屏界面位置类型*/
	uint8_t content[210];
	uint32_t OSD_version;
	uint32_t wTimes;
} gos_cas_rolling_message_st; 

typedef struct {
	uint8_t wStatus; /* 需要显示邮件图标类型*/
	uint8_t wShowStatus; /*当前显示状态*/
	uint8_t uCount; /* 显示累计次数，处理闪烁*/
	uint32_t emailID;
} gos_cas_email_st; 

typedef enum
{
	CA_GOS_No_OSDPromt=0,			/*无OSD提示*/	
	CA_GOS_SMART_STOP,              /*卡已被禁用*/
    CA_GOS_SMART_AREA_NOPAIR,		/*流卡区域不匹配*/
	CA_GOS_SMART_IPPT_SWITCH,		/*IPPT用户提示*/
    CA_GOS_SMART_STB_NOPAIR,			/*机卡不配对*/
    CA_GOS_SMART_PROG_NOENTITLE,		/*节目类未授权*/
    CA_GOS_SMART_PROG_NOENTITLE_IPPT,/*节目类未授权*/
    CA_GOS_SMART_MOTHER_SON_PAIR,	/*子母卡需从新配对*/
    CA_GOS_SMART_MOTHER_SON_TIMEOUT,	/*子母卡配对超时*/
    CA_GOS_SMART_PAIR_OK,			/*机卡对应OK*/
    CA_GOS_SMART_LAST_DAY,			/*最后提示天数*/	
    CA_GOS_SMART_AREA_LOCK,			/*卡区域锁定 */
	CA_GOS_SMART_CARD_LOCK,			/*卡已被锁定 */
	CA_GOS_SMART_IPPT,				/*IPPT提示*/
	CA_GOS_SMART_LIMIT_WATCH,		/*卡限制收看*/
	CA_GOS_SMART_NO_WORK_TIME,		/*卡不在工作时间段*/
	CA_GOS_SMART_OUT				/*节目加扰，请插卡*/
//	GOSCA_SMART_UNKNOWN_ERROR,		/*未知错误*/
}CA_GOS_SMART_STATUS;


int32_t app_gos_cas_gxmsg_ca_on_event_exec(GxMessage * msg);
uint8_t app_gos_cas_api_is_ca_pop_msg_type(uint8_t type);
 int32_t app_gos_cas_api_pop_msg_exec(uint8_t type);
 int32_t app_gos_cas_api_osd_exec(void);



#ifdef __cplusplus
}
#endif
#endif /*__APP_GOS_CAS_API_POP_H__*/

