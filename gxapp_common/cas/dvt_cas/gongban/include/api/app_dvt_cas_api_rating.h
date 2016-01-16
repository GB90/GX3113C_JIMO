/**
 *
 * @file        app_dvt_cas_api_rating.h
 * @brief
 * @version     1.1.0
 * @date        12/05/2013 
 * @author      zhouhuaming (zhouhm), zhuohm@nationalchip.com
 *
 */
#ifndef __APP_DVT_CAS_API_RATING_H__
#define __APP_DVT_CAS_API_RATING_H__
#ifdef __cplusplus
extern "C" {
#endif

#include "app_common_play.h"

typedef enum 
{
	DVT_RATING_LEVEL_ID=0,
	DVT_RATING_MAX
}DVTCA_RATING_ID_E;


uint8_t app_dvt_cas_api_init_rating_data(void);
char * app_dvt_cas_api_get_rating_data(ca_get_date_t *data);
char* app_dvt_cas_api_change_rating(ca_rating_t* data);


#ifdef __cplusplus
}
#endif
#endif /*__APP_DVT_CAS_API_RATING_H__*/

