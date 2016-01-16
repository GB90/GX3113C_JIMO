#ifndef __APP_KPCAS_API_BASEINFO_H__
#define __APP_KPCAS_API_BASEINFO_H__
#ifdef __cplusplus
extern "C" {
#endif

#include "app_common_play.h"

typedef enum 
{
    KP_BASEINFO_CA_ID= 0,
	KP_BASEINFO_VER_ID,
    KP_BASEINFO_LICENCE_ID,
	KP_BASEINFO_UNIQUE_ID,
	KP_BASEINFO_AREACODE_ID,
	KP_BASEINFO_ATTIBUTE_ID,
}KPCA_BASEIINFO_ID_E;

int32_t app_kp_cas_api_init_baseinfo_data(void);
char * app_kp_cas_api_get_baseinfo_data(ca_get_date_t *data);


#ifdef __cplusplus
}
#endif
#endif

