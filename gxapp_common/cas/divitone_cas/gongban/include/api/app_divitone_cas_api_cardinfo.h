/**
 *
 * @file        app_by_cas_api_baseinfo.h
 * @brief
 * @version     1.1.0
 * @date        11/30/2012 09:47:49 AM
 * @author      zhouhuaming (zhouhm), zhuohm@nationalchip.com
 *
 */
#ifndef __APP_DIVICAS_API_BASEINFO_H__
#define __APP_DIVICAS_API_BASEINFO_H__
#ifdef __cplusplus
extern "C" {
#endif

#include "app_common_play.h"

typedef enum 
{
	DIVI_BASEINFO_CARDID_ID=0,
	DIVI_BASEINFO_CARDTYPE_ID,
	DIVI_BASEINFO_CARDSTATE_ID,
	DIVI_BASEINFO_REGION_ID,
	DIVI_BASEINFO_WORKTIME_ID,
	DIVI_BASEINFO_WATCHLEVEL_ID,
	DIVI_BASEINFO_PAIR_STATE,
	DIVI_BASEINFO_CAVERSION_ID
}DIVICA_BASEIINFO_ID_E;

int32_t app_divitone_cas_api_init_baseinfo_data(void);
char * app_divitone_cas_api_get_baseinfo_data(ca_get_date_t *data);


#ifdef __cplusplus
}
#endif
#endif /*__APP_DIVICAS_API_BASEINFO_H__*/

