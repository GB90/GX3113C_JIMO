/**
 *
 * @file        app_by_cas_api_entitle.h
 * @brief
 * @version     1.1.0
 * @date        11/30/2012 09:47:49 AM
 * @author      zhouhuaming (zhouhm), zhuohm@nationalchip.com
 *
 */
#ifndef __APP_WFCAS_API_ENTITLE_H__
#define __APP_WFCAS_API_ENTITLE_H__
#ifdef __cplusplus
extern "C" {
#endif

#include "app_common_play.h"

typedef enum 
{
	WF_ENTITLE_PRODUCT_NO_ID = 0,
	WF_ENTITLE_PRODUCT_ID,
	WF_ENTITLE_PRODUCT_STARTTIME_ID,
	WF_ENTITLE_PRODUCT_ENDTIME_ID 	
}WFCA_ENTITLE_ID_E;

int32_t app_wf_cas_api_init_entitle_data(void);
char * app_wf_cas_api_get_entitle_data(ca_get_date_t *data);
uint8_t app_wf_cas_api_get_entitle_count(ca_get_count_t *data);


#ifdef __cplusplus
}
#endif
#endif /*__APP_WFCAS_API_ENTITLE_H__*/

