/**
 *
 * @file        app_cd_cas_3.0_api_detitle.h
 * @brief
 * @version     1.1.0
 * @date        11/30/2012 09:47:49 AM
 * @author      zhouhuaming (zhouhm), zhuohm@nationalchip.com
 *
 */
#ifndef __APP_CDCAS_30_API_EMAIL_H__
#define __APP_CDCAS_30_API_EMAIL_H__
#ifdef __cplusplus
extern "C" {
#endif

#include "app_common_play.h"

typedef enum 
{
	CD_EMAIL_IS_NEW_ID=0,
	CD_EMAIL_HEAD_ID,
	CD_EMAIL_DATE_ID,	
	CD_EMAIL_IMPORTANCE,
	CD_EMAIL_CONTENT,
	CD_EMAIL_MAX
}CDCA_30_EMAIL_ID_E;

uint8_t app_cd_cas30_api_init_email_data(void);
char * app_cd_cas30_api_get_email_data(ca_get_date_t *data);

uint8_t app_cd_cas30_api_get_email_count(ca_get_count_t *data);

bool app_cd_cas30_api_delete_email(ca_delete_data_t *data);

bool app_cd_cas30_api_delete_all_email(ca_delete_data_t *data);
int32_t app_cd_cas30_gxmsg_ca_on_event_email(GxMsgProperty0_OnEvent* event);
int32_t app_cd_cas30_email_flag_exec();







#ifdef __cplusplus
}
#endif
#endif /*__APP_CDCAS_30_API_EMAIL_H__*/

