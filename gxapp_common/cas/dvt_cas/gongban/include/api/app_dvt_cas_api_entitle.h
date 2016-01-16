/**
 *
 * @file        app_dvt_cas_api_entitle.h
 * @brief
 * @version     1.1.0
 * @date        12/03/2013 
 * @author      zhouhuaming (zhouhm), zhuohm@nationalchip.com
 *
 */
#ifndef __APP_DVT_CAS_API_ENTITLE_H__
#define __APP_DVT_CAS_API_ENTITLE_H__
#ifdef __cplusplus
extern "C" {
#endif

#include "app_common_play.h"

typedef enum 
{
	DVT_ENTITLE_NAME_ID =0,
	DVT_ENTITLE_ID_ID ,
	DVT_ENTITLE_AUTHORIZE_TIME_ID ,
	DVT_ENTITLE_START_TIME_ID ,
	DVT_ENTITLE_END_TIME_ID 
}DVTCA_ENTITLE_ID_E;


int32_t app_dvt_cas_api_init_entitle_data(void);
char * app_dvt_cas_api_get_entitle_data(ca_get_date_t *data);
uint8_t app_dvt_cas_api_get_entitle_count(ca_get_count_t *data);


#ifdef __cplusplus
}
#endif
#endif /*__APP_DVT_CAS_API_ENTITLE_H__*/

