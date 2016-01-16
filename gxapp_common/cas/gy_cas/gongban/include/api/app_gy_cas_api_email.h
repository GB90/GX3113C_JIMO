/**
 *
 * @file        app_cd_cas_3.0_api_detitle.h
 * @brief
 * @version     1.1.0
 * @date        11/30/2012 09:47:49 AM
 * @author      zhouhuaming (zhouhm), zhuohm@nationalchip.com
 *
 */
#ifndef __APP_GYCAS_API_EMAIL_H__
#define __APP_GYCAS_API_EMAIL_H__
#ifdef __cplusplus
extern "C" {
#endif

#include "gyca_def.h"
#include "app_common_play.h"

typedef struct {
	uint8_t wStatus; /* 需要显示邮件图标类型*/
	uint8_t wShowStatus; /*当前显示状态*/
	uint8_t uCount; /* 显示累计次数，处理闪烁*/
	uint32_t emailID;
} gy_ca_email_st; 

typedef enum 
{
	GY_EMAIL_IS_NEW_ID=0,
	GY_EMAIL_HEAD_ID,
	GY_EMAIL_DATE_ID,	
	GY_EMAIL_IMPORTANCE,
	GY_EMAIL_CONTENT,
	GY_EMAIL_MAX
}GYCA_EMAIL_ID_E;

typedef struct _GxEmail_t_
{
	unsigned char id; 
	unsigned char readFlag; // 已读标志. 0未读, 1已读. 
	T_CAMailInfo email; 
} GxEmail_t; 


void app_gy_cas_api_add_email(T_CAMailInfo *pMailInfo); 
uint8_t app_gy_cas_api_init_email_data(void);
char * app_gy_cas_api_get_email_data(ca_get_date_t *data);

uint8_t app_gy_cas_api_get_email_count(ca_get_count_t *data);

bool app_gy_cas_api_delete_email(ca_delete_data_t *data);

bool app_gy_cas_api_delete_all_email(ca_delete_data_t *data);
int32_t app_gy_cas_gxmsg_ca_on_event_email(GxMsgProperty0_OnEvent* event);
int32_t app_gy_cas_email_flag_exec();







#ifdef __cplusplus
}
#endif
#endif /*__APP_GYCAS_API_EMAIL_H__*/

