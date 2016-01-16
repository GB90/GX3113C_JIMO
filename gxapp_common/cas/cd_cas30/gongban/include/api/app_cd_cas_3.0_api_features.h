/**
 *
 * @file        app_cd_cas_3.0_api_ippv_purse.h
 * @brief
 * @version     1.1.0
 * @date        11/30/2012 09:47:49 AM
 * @author      zhouhuaming (zhouhm), zhuohm@nationalchip.com
 *
 */
#ifndef __APP_CDCAS_30_API_FEATURES_H__
#define __APP_CDCAS_30_API_FEATURES_H__
#ifdef __cplusplus
extern "C" {
#endif

#include "app_common_play.h"

typedef enum 
{
	CD_FEATURES_TVS_ID=0,
	CD_FEATURES_AREACODE_ID,
	CD_FEATURES_BOUQUETID_ID,
	CD_FEATURES_ID1_ID,	
	CD_FEATURES_ID2_ID,
	CD_FEATURES_ID3_ID
}CDCA_30_FEATURES_ID_E;


int32_t app_cd_cas30_api_init_features_data(void);
char * app_cd_cas30_api_get_features_data(ca_get_date_t *data);
uint8_t app_cd_cas30_api_get_features_count(ca_get_count_t *data);


#ifdef __cplusplus
}
#endif
#endif /*__APP_CDCAS_30_API_FEATURES_H__*/

