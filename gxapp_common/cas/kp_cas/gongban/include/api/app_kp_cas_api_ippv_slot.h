#ifndef __APP_CDCAS_30_API_IPPV_SLOT_H__
#define __APP_CDCAS_30_API_IPPV_SLOT_H__
#ifdef __cplusplus
extern "C" {
#endif

#include "app_common_play.h"

typedef enum 
{
	KP_IPPV_SLOT_TVS_ID = 0,
    KP_IPPV_SLOT_TVS_NAME,
	KP_IPPV_SLOT_TOTAL_ID,
	KP_IPPV_SLOT_USED_ID,
	
}KPCA_IPPV_SLOT_ID_E; 


int32_t app_kp_cas_api_init_ippv_slot_data(void);
char * app_kp_cas_api_get_ippv_slot_data(ca_get_date_t *data);
uint8_t app_kp_cas_api_get_ippv_slot_count(ca_get_count_t *data);


#ifdef __cplusplus
}
#endif
#endif

