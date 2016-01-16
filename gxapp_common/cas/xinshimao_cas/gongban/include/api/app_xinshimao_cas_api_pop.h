/**
 *
 * @file        app_xinshimao_ca_api_pop.h
 * @brief
 * @version     1.1.0
 * @date        09/17/2013 09:47:49 AM
 * @author      zhouhuaming (zhouhm), zhuohm@nationalchip.com
 *
 */
#ifndef __APP_XINSHIMAO_CAS_API_POP_H__
#define __APP_XINSHIMAO_CAS_API_POP_H__
#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
	uint16_t wStatus; /* 0 --清除，1 -- 显示卡号 */
	uint16_t wShowStatus; /* 0 --隐藏状态，1 -- 显示状态*/
	uint16_t wEcmPid;
	uint32_t dwCardID;
	uint32_t position; /*随机显示位置索引*/
//	uint32_t wDuration;
} xinshimao_cas_finger_data_st; 


typedef struct {
	uint8_t wStatus; /* 需要显示邮件图标类型*/
	uint8_t wShowStatus; /*当前显示状态*/
	uint8_t uCount; /* 显示累计次数，处理闪烁*/
	uint32_t emailID;
} xinshimao_cas_email_st;  

int32_t app_xinshimao_cas_gxmsg_ca_on_event_exec(GxMessage * msg);
uint8_t app_xinshimao_cas_api_is_ca_pop_msg_type(uint8_t type);
 int32_t app_xinshimao_cas_api_pop_msg_exec(uint8_t type);
 int32_t app_xinshimao_cas_api_osd_exec(void);



#ifdef __cplusplus
}
#endif
#endif /*__APP_XINSHIMAO_CAS_API_POP_H__*/

