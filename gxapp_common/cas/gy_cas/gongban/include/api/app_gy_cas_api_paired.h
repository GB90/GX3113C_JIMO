/**
 *
 * @file        app_cd_cas_3.0_api_paired.h
 * @brief
 * @version     1.1.0
 * @date        11/30/2012 09:47:49 AM
 * @author      zhouhuaming (zhouhm), zhuohm@nationalchip.com
 *
 */
#ifndef __APP_GYCAS_API_PAIRED_H__
#define __APP_GYCAS_API_PAIRED_H__
#ifdef __cplusplus
extern "C" {
#endif

#include "app_common_play.h"

typedef enum 
{
	GY_PAIRED_INFO_ID=0,
}GYCA_PAIRED_ID_E;

int32_t app_gy_cas_api_init_paired_data(void);
char * app_gy_cas_api_get_paired_data(ca_get_date_t *data);

#ifdef __cplusplus
}
#endif
#endif /*__APP_CDCAS_30_API_PAIRED_H__*/

