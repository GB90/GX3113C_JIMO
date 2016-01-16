#ifndef __APP_MGCAS_API_PPV_H__
#define __APP_MGCAS_API_PPV_H__
#ifdef __cplusplus
extern "C" {
#endif

#include "app_common_play.h"

typedef enum 
{
    MG_PPV_ISNO = 0,
    MG_PPV_ORDER_STATUS_ID,
    MG_PPV_ORDER_STATUS_STRING_ID,
    MG_PPV_PRO_COST_ID,
    MG_PPV_CARD_BLE,
}MGCA_PPV_ID_E;

int32_t app_mg_cas_api_init_ppv_data(void);
char * app_mg_cas_api_get_ppv_data(ca_get_date_t *data);
uint8_t app_mg_cas_api_buy_ppv_data(ca_buy_ipp_t *data);

#ifdef __cplusplus
}
#endif
#endif

