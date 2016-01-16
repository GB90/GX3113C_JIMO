/**
 *
 * @file        app_cd_cas_3.0_api_ippv_purse.h
 * @brief
 * @version     1.1.0
 * @date        11/30/2012 09:47:49 AM
 * @author      zhouhuaming (zhouhm), zhuohm@nationalchip.com
 *
 */
#ifndef __APP_CDCAS_30_API_IPPV_INFO_H__
#define __APP_CDCAS_30_API_IPPV_INFO_H__
#ifdef __cplusplus
extern "C" {
#endif

#include "app_common_play.h"

typedef enum 
{
	CD_IPPV_INFO_TVS_ID=0,
	CD_IPPV_INFO_SLOT_ID,
	CD_IPPV_INFO_PROGRAM_ID, 
	CD_IPPV_INFO_STATUS_ID,	
	CD_IPPV_INFO_PRICE_ID,
	CD_IPPV_INFO_TAPEABLE_ID,
	CD_IPPV_INFO_ENDTIME_ID, 
	
}CDCA_30_IPPV_INFO_ID_E;


int32_t app_cd_cas30_api_init_ippv_info_data(void);
char * app_cd_cas30_api_get_ippv_info_data(ca_get_date_t *data);
uint8_t app_cd_cas30_api_get_ippv_info_count(ca_get_count_t *data);


#ifdef __cplusplus
}
#endif
#endif /*__APP_CDCAS_30_API_IPPV_SLOT_H__*/

