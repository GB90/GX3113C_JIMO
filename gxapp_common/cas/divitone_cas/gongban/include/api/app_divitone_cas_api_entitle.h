/**
 *
 * @file        app_cd_cas_3.0_api_entitle.h
 * @brief
 * @version     1.1.0
 * @date        11/30/2012 09:47:49 AM
 * @author      zhouhuaming (zhouhm), zhuohm@nationalchip.com
 *
 */
#ifndef __APP_DIVICAS_API_ENTITLE_H__
#define __APP_DIVICAS_API_ENTITLE_H__
#ifdef __cplusplus
extern "C" {
#endif

#include "app_common_play.h"

typedef enum 
{
	DIVI_ENTITLE_ENTITLE_ID=0,
	DIVI_ENTITLE_ENTITLE_PRICE,
	DIVI_ENTITLE_ENTITLE_PERID,	
	DIVI_ENTITLE_ENTITLE_SCRIPT,	
}DIVI_ENTITLE_ENTITLE_ID_E;

int32_t app_divitone_cas_api_init_entitle_data(void);
char * app_divitone_cas_api_get_entitle_data(ca_get_date_t *data);
uint8_t app_divitone_cas_api_get_entitle_count(ca_get_count_t *data);

#ifdef __cplusplus
}
#endif
#endif /*__APP_DIVICAS_API_ENTITLE_H__*/

