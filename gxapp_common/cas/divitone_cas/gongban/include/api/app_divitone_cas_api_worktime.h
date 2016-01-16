/**
 *
 * @file        app_cd_cas_3.0_api_detitle.h
 * @brief
 * @version     1.1.0
 * @date        11/30/2012 09:47:49 AM
 * @author      zhouhuaming (zhouhm), zhuohm@nationalchip.com
 *
 */
#ifndef __APP_DIVICAS_API_WORKTIME_H__
#define __APP_DIVICAS_API_WORKTIME_H__
#ifdef __cplusplus
extern "C" {
#endif

#include "app_common_play.h"

typedef enum 
{
	DIVI_WORKTIME_START_ID=0,
	DIVI_WORKTIME_END_ID,
	DIVI_WORKTIME_MAX
}DIVICA_WORKTIME_ID_E;


uint8_t app_divitone_cas_api_init_worktime_data(void);
char * app_divitone_cas_api_get_worktime_data(ca_get_date_t *data);
char* app_divitone_cas_api_change_worktime(ca_work_time_t* data);


#ifdef __cplusplus
}
#endif
#endif /*__APP_DIVICAS_API_WORKTIME_H__*/

