/**
 *
 * @file        app_cd_cas_3.0_api_ippv_purse.h
 * @brief
 * @version     1.1.0
 * @date        11/30/2012 09:47:49 AM
 * @author      zhouhuaming (zhouhm), zhuohm@nationalchip.com
 *
 */
#ifndef __APP_CDCAS_30_API_MOTHER_CHILD_H__
#define __APP_CDCAS_30_API_MOTHER_CHILD_H__
#ifdef __cplusplus
extern "C" {
#endif

#include "app_common_play.h"

typedef enum 
{
	CD_MOTHER_CHILD_TVS_ID=0,
	CD_MOTHER_CHILD_CARD_TYPE_ID,
	CD_MOTHER_CHILD_LAST_FEED_TIME_ID,
	CD_MOTHER_CHILD_FEED_PERIOD_ID,	
	CD_MOTHER_CHILD_FEED_MOTHER_CARD_ID
}CDCA_30_MOTHER_CHILID_ID_E;


int32_t app_cd_cas30_api_init_mother_child_data(void);
char * app_cd_cas30_api_get_mother_child_data(ca_get_date_t *data);
uint8_t app_cd_cas30_api_get_mother_child_count(ca_get_count_t *data);
char* app_cd_cas30_api_feed_mother_child_card(ca_mother_child_card_t *data);


#ifdef __cplusplus
}
#endif
#endif /*__APP_CDCAS_30_API_FEATURES_H__*/

