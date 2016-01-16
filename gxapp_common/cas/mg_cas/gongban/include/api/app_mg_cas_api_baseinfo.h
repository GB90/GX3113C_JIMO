#ifndef __APP_MGCAS_API_BASEINFO_H__
#define __APP_MGCAS_API_BASEINFO_H__
#ifdef __cplusplus
extern "C" {
#endif

#include "app_common_play.h"

typedef enum 
{
	MG_BASEINFO_IVS_ID = 0,
	MG_BASEINFO_VER_ID,
	MG_BASEINFO_CARDSN_ID,
	MG_BASEINFO_STATE_ID,
	MG_BASEINFO_AREACODE_ID,
	MG_BASEINFO_CTRLCODE_ID,
	MG_BASEINFO_TOTAL_ID,
	MG_BASEINFO_CONSUME_ID,
	MG_BASEINFO_EXPIRES_ID,
	MG_BASEINFO_SUBMAINCARD_TYPE_ID,
	MG_BASEINFO_SUBMAINCARD_MAINID_ID,
	MG_BASEINFO_SUBMAINCARD_TIME_ID,
	MG_BASEINFO_FEATURESCODE_V1_ID,
	MG_BASEINFO_FEATURESCODE_V2_ID,
}MGCA_BASEIINFO_ID_E;

int32_t app_mg_cas_api_init_baseinfo_data(void);
char * app_mg_cas_api_get_baseinfo_data(ca_get_date_t *data);


#ifdef __cplusplus
}
#endif
#endif

