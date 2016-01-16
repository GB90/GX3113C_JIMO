/**
 *
 * @file        app_cd_cas_3.0_api_detitle.h
 * @brief
 * @version     1.1.0
 * @date        11/30/2012 09:47:49 AM
 * @author      zhouhuaming (zhouhm), zhuohm@nationalchip.com
 *
 */
#ifndef __APP_GYCAS_API_RATING_H__
#define __APP_GYCAS_API_RATING_H__
#ifdef __cplusplus
extern "C" {
#endif

#include "app_common_play.h"

typedef enum 
{
	GY_RATING_LEVEL_ID=0,
	GY_RATING_MAX
}GYCA_RATING_ID_E;


uint8_t app_gy_cas_api_init_rating_data(void);
char * app_gy_cas_api_get_rating_data(ca_get_date_t *data);
char* app_gy_cas_api_change_rating(ca_rating_t* data);


#ifdef __cplusplus
}
#endif
#endif /*__APP_CDCAS_30_API_RATING_H__*/

