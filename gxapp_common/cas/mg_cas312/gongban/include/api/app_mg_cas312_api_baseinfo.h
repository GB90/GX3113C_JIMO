#ifndef __APP_MGCAS312_API_BASEINFO_H__
#define __APP_MGCAS312_API_BASEINFO_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "app_common_play.h"

typedef enum 
{
	MG312_BASEINFO_IVS_ID = 0,
	MG312_BASEINFO_VER_ID,
	MG312_BASEINFO_CARDSN_ID,
	MG312_BASEINFO_CARDVER_ID,
	MG312_BASEINFO_BALANCE_ID,
	MG312_BASEINFO_OVERDRAFT_ID,
	MG312_BASEINFO_AREACODE_ID,
	MG312_BASEINFO_PARTNER_ID,
	
	MG312_BASEINFO_FEATURESCODE_V1_ID,
	MG312_BASEINFO_FEATURESCODE_V2_ID,
	MG312_BASEINFO_SUBMAINCARD_TYPE_ID,
	MG312_BASEINFO_SUBMAINCARD_MAINID_ID,
	MG312_BASEINFO_SUBMAINCARD_TIME_ID, /*son card active period.*/
	MG312_BASEINFO_SUBMAINCARD_ACTIVE_ID/*son card remainder active period.*/
}MGCA_BASEIINFO_ID_E;

int32_t app_mg_cas312_api_init_baseinfo_data(void);
char * app_mg_cas312_api_get_baseinfo_data(ca_get_date_t *data);

#ifdef __cplusplus
}
#endif

#endif

