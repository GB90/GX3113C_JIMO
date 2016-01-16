/**
 *
 * @file        app_dvt_cas_api_email.h
 * @brief
 * @version     1.1.0
 * @date        12/02/2013
 * @author      zhouhuaming (zhouhm), zhuohm@nationalchip.com
 *
 */
#ifndef __APP_DVT_CAS_API_EMAIL_H__
#define __APP_DVT_CAS_API_EMAIL_H__
#ifdef __cplusplus
extern "C" {
#endif

#include "app_common_play.h"


typedef enum 
{
	DVT_EMAIL_STATE_ID = 0,
	DVT_EMAIL_TITLE_ID,
	DVT_EMAIL_DESPATCHER_ID,
	DVT_EMAIL_CONTENT_ID,
	DVT_EMAIL_MAX
}DVTCA_EMAIL_ID_E;

uint8_t app_dvt_cas_api_init_email_data(void);
char * app_dvt_cas_api_get_email_data(ca_get_date_t *data);

uint8_t app_dvt_cas_api_get_email_count(ca_get_count_t *data);

bool app_dvt_cas_api_delete_email(ca_delete_data_t *data);

bool app_dvt_cas_api_delete_all_email(ca_delete_data_t *data);
int32_t app_dvt_cas_gxmsg_ca_on_event_email(GxMsgProperty0_OnEvent* event);
int32_t app_dvt_cas_email_flag_exec(void);

#ifdef __cplusplus
}
#endif
#endif /*__APP_DVT_CAS_API_EMAIL_H__*/

