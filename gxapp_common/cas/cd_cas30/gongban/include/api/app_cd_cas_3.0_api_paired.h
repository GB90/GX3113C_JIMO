/**
 *
 * @file        app_cd_cas_3.0_api_paired.h
 * @brief
 * @version     1.1.0
 * @date        11/30/2012 09:47:49 AM
 * @author      zhouhuaming (zhouhm), zhuohm@nationalchip.com
 *
 */
#ifndef __APP_CDCAS_30_API_PAIRED_H__
#define __APP_CDCAS_30_API_PAIRED_H__
#ifdef __cplusplus
extern "C" {
#endif

#include "app_common_play.h"

typedef enum 
{
	CD_PAIRED_INFO_ID=0,
	CD_PAIRED_STBID_LIST1_ID,
	CD_PAIRED_STBID_LIST2_ID,
	CD_PAIRED_STBID_LIST3_ID,
	CD_PAIRED_STBID_LIST4_ID,
	CD_PAIRED_STBID_LIST5_ID	
}CDCA_30_PAIRED_ID_E;

int32_t app_cd_cas30_api_init_paired_data(void);
char * app_cd_cas30_api_get_paired_data(ca_get_date_t *data);

#ifdef __cplusplus
}
#endif
#endif /*__APP_CDCAS_30_API_PAIRED_H__*/

