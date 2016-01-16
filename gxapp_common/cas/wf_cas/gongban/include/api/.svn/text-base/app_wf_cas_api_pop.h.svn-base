/**
 *
 * @file        app_by_cas_api_baseinfo.h
 * @brief
 * @version     1.1.0
 * @date        11/30/2012 09:47:49 AM
 * @author      zhouhuaming (zhouhm), zhuohm@nationalchip.com
 *
 */
#ifndef __APP_WFCAS_API_POP_H__
#define __APP_WFCAS_API_POP_H__
#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
	uint8_t wStatus; /* 需要显示邮件图标类型*/
	uint8_t wShowStatus; /*当前显示状态*/
	uint8_t uCount; /* 显示累计次数，处理闪烁*/
	uint32_t emailID;
	uint8_t *buffer;
} wf_cas_email_st; 


#define ROLL_CONTENT_MAX (1024)

typedef struct {
	uint8_t wStatus; /* 0 --不需要显示，1-- 需要显示*/
	uint8_t roll_status; /*0 -- 当前滚动状态，1-- 当前非滚动状态*/
	int32_t wTimes;       /*已滚动次数*/
	uint32_t emailID;
	uint8_t *buffer;
} wf_ca_rolling_message_st; 


typedef struct {
	uint8_t wStatus; /* 要求显示或取消购买框*/
	uint8_t wShowStatus; /*当前购买框显示状态*/
	uint8_t IsPPV;
	uint16_t Interval;
	uint16_t UnitPrice;
} wf_cas_pptv_buy_st; 


#ifdef __cplusplus
}
#endif
#endif /*__APP_WFCAS_API_POP_H__*/

