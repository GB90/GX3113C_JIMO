#ifndef __APP_MGCAS312_API_PPV_H__
#define __APP_MGCAS312_API_PPV_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "app_common_play.h"

typedef enum 
{
    MG312_PPV_ISNO = 0,
    MG312_PPV_ORDER_STATUS_ID,
    MG312_PPV_ORDER_STATUS_STRING_ID,
    MG312_PPV_PRO_COST_ID,
    MG312_PPV_CARD_BLE,/*card balance.*/
}MGCA312_PPV_ID_E;

int32_t app_mg_cas312_api_init_ppv_data(void);
char * app_mg_cas312_api_get_ppv_data(ca_get_date_t *data);
uint8_t app_mg_cas312_api_buy_ppv_data(ca_buy_ipp_t *data);

#ifdef __cplusplus
}
#endif

#endif

