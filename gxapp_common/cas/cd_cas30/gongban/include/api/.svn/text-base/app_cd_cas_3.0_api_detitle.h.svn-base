/**
 *
 * @file        app_cd_cas_3.0_api_detitle.h
 * @brief
 * @version     1.1.0
 * @date        11/30/2012 09:47:49 AM
 * @author      zhouhuaming (zhouhm), zhuohm@nationalchip.com
 *
 */
#ifndef __APP_CDCAS_30_API_DETITLE_H__
#define __APP_CDCAS_30_API_DETITLE_H__
#ifdef __cplusplus
extern "C" {
#endif

#include "app_common_play.h"

typedef enum 
{
	CD_DETITLE_OPERATE_ID=0,
	CD_DETITLE_CHK_NUM_ID,
	CD_DETITLE_CHK_NUM_READED_ID,	
	CD_DETITLE_MAX
}CDCA_30_DETITLE_ID_E;

typedef enum 
{
	CD_DETITLE_FLAG_NONE=0,
	CD_DETITLE_FLAG_HIDE,
	CD_DETITLE_FLAG_SHOW,
	CD_DETITLE_FLAG_TRANS_HIDE,
	CD_DETITLE_FLAG_MAX
}CDCA_30_DETITLE_FLAG_E;


uint8_t app_cd_cas30_api_init_detitle_data(void);
char * app_cd_cas30_api_get_detitle_data(ca_get_date_t *data);

uint8_t app_cd_cas30_api_get_detitle_count(ca_get_count_t *data);

bool app_cd_cas30_api_delete_detitle(ca_delete_data_t *data);

bool app_cd_cas30_api_delete_all_detitle(ca_delete_data_t *data);

int32_t app_cd_cas30_gxmsg_ca_on_event_detitle(GxMsgProperty0_OnEvent* event);
int32_t app_cd_cas30_detitle_flag_exec();







#ifdef __cplusplus
}
#endif
#endif /*__APP_CDCAS_30_API_DETITLE_H__*/

