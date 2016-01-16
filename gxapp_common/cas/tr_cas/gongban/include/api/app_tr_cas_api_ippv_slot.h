/**
 *
 * @file        app_tr_cas_api_ippv_slot.h
 * @brief
 * @version     1.1.0
 * @date        11/30/2012 09:47:49 AM
 * @author      zhouhuaming (zhouhm), zhuohm@nationalchip.com
 *
 */
#ifndef __APP_TR_CAS_API_IPPV_SLOT_H__
#define __APP_TR_CAS_API_IPPV_SLOT_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "app_common_play.h"

typedef enum 
{
	TR_SLOT_ID = 0,
	TR_BALANCE_ID, 
	TR_CREDIT_LINE_ID,
}TR_CAS_SLOT_ID_E;


int32_t app_tr_cas_api_init_ippv_slot_data(void);
char* app_tr_cas_api_get_ippv_slot_data(ca_get_date_t *data);
uint8_t app_tr_cas_api_get_ippv_slot_count(ca_get_count_t *data);


#ifdef __cplusplus
}
#endif

#endif /*__APP_TR_CAS_API_IPPV_SLOT_H__*/

