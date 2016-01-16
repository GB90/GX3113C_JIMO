/**
 *
 * @file        app_by_cas_api_entitle.h
 * @brief
 * @version     1.1.0
 * @date        11/30/2012 09:47:49 AM
 * @author      zhouhuaming (zhouhm), zhuohm@nationalchip.com
 *
 */
#ifndef __APP_DIVICAS_API_EMAIL_H__
#define __APP_DIVICAS_API_EMAIL_H__
#ifdef __cplusplus
extern "C" {
#endif

#include "app_common_play.h"



typedef enum 
{
	DIVI_EMAIL_FLAG_ID =0,
	DIVI_EMAIL_LEVEL_ID,
	DIVI_EMAIL_IS_NEW_ID,
	DIVI_EMAIL_TITLE_ID ,
	DIVI_EMAIL_TIME_ID ,
	DIVI_EMAIL_CONTENT_ID
}DIVICA_EMAIL_ID_E;


int32_t app_divitone_cas_api_init_email_data(void);
char * app_divitone_cas_api_get_email_data(ca_get_date_t *data);
uint8_t app_divitone_cas_api_get_email_count(ca_get_count_t *data);

bool app_divitone_cas_api_delete_email(ca_delete_data_t *data);
bool app_divitone_cas_api_delete_all_email(ca_delete_data_t *data);

int32_t app_divitone_cas_gxmsg_ca_on_event_email(GxMsgProperty0_OnEvent* event);
int32_t app_divitone_cas_email_flag_exec(void);



#ifdef __cplusplus
}
#endif
#endif /*__APP_DIVICAS_API_EMAIL_H__*/

