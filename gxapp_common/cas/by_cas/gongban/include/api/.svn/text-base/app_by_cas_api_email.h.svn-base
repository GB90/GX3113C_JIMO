/**
 *
 * @file        app_by_cas_api_entitle.h
 * @brief
 * @version     1.1.0
 * @date        11/30/2012 09:47:49 AM
 * @author      zhouhuaming (zhouhm), zhuohm@nationalchip.com
 *
 */
#ifndef __APP_BYCAS_API_EMAIL_H__
#define __APP_BYCAS_API_EMAIL_H__
#ifdef __cplusplus
extern "C" {
#endif

#include "app_common_play.h"

typedef struct {
	uint8_t wStatus; /* ÐèÒªÏÔÊ¾ÓÊ¼þÍ¼±êÀàÐÍ*/
	uint8_t wShowStatus; /*µ±Ç°ÏÔÊ¾×´Ì¬*/
	uint8_t uCount; /* ÏÔÊ¾ÀÛ¼Æ´ÎÊý£¬´¦ÀíÉÁË¸*/
} by_cas_email_st; 

typedef enum 
{
	BY_EMAIL_FLAG_ID =0,
	BY_EMAIL_TITLE_ID ,
	BY_EMAIL_SENDTIME_ID ,
	BY_EMAIL_VALIDTIME_ID,
	BY_EMAIL_CONTENT_ID
}BYCA_EMAIL_ID_E;


int32_t app_by_cas_api_init_email_data(void);
char * app_by_cas_api_get_email_data(ca_get_date_t *data);
uint8_t app_by_cas_api_get_email_count(ca_get_count_t *data);

bool app_by_cas_api_delete_email(ca_delete_data_t *data);
bool app_by_cas_api_delete_all_email(ca_delete_data_t *data);

int32_t app_by_cas_gxmsg_ca_on_event_email(GxMsgProperty0_OnEvent* event);
int32_t app_by_cas_email_flag_exec();



#ifdef __cplusplus
}
#endif
#endif /*__APP_BYCAS_API_EMAIL_H__*/

