#ifndef __APP_KPCAS_API_ENTITLE_H__
#define __APP_KPCAS_API_ENTITLE_H__
#ifdef __cplusplus
extern "C" {
#endif

#include "app_common_play.h"

typedef enum 
{
	KP_ENTITLE_ID =0,
	KP_ENTITLE_BEGIN ,
	KP_ENTITLE_END ,	
}KPCA_ENTITLE_ID_E;


int32_t app_kp_cas_api_init_entitle_data(void);
char * app_kp_cas_api_get_entitle_data(ca_get_date_t *data);
uint8_t app_kp_cas_api_get_entitle_count(ca_get_count_t *data);


#ifdef __cplusplus
}
#endif
#endif

