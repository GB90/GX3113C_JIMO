/**
 *
 * @file        app_dvt_cas_api_ca_info.h
 * @brief
 * @version     1.1.0
 * @date        12/04/2013 
 * @author      zhouhuaming (zhouhm), zhuohm@nationalchip.com
 *
 */
#ifndef __APP_DVT_CAS_API_BASEINFO_H__
#define __APP_DVT_CAS_API_BASEINFO_H__
#ifdef __cplusplus
extern "C" {
#endif

#include "app_common_play.h"

typedef enum 
{
	DVT_BASEINFO_SERIAL_ID=0,
	DVT_BASEINFO_COS_VER_ID,
	DVT_BASEINFO_CAS_VER_ID,
	DVT_BASEINFO_MANU_NAME_ID,
	DVT_BASEINFO_MOTHER_ID,
	DVT_BASEINFO_MOTHER_NUMBER_ID,
	DVT_BASEINFO_LOCK_ID,
	DVT_BASEINFO_RATING_ID,
	DVT_BASEINFO_WORK_TIME_ID,
	DVT_BASEINFO_AREA_ID,
	DVT_BASEINFO_AREA_LOCK,
	DVT_BASEINFO_AREA_TIME,
}DVTCA_BASEIINFO_ID_E;

int32_t app_dvt_cas_api_init_baseinfo_data(void);
char * app_dvt_cas_api_get_baseinfo_data(ca_get_date_t *data);


#ifdef __cplusplus
}
#endif
#endif /*__APP_DVT_CAS_API_BASEINFO_H__*/

