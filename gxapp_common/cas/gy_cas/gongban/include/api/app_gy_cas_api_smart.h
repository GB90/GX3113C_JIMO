/**
 *
 * @file        app_gy_cas_api_smart.h
 * @brief
 * @version     1.1.0
 * @date        11/30/2012 09:47:49 AM
 * @author      bingyu , yubing@nationalchip.com
 *
 */
#ifndef __APP_GYCAS_SMARTCARD_API_H__
#define __APP_GYCAS_SMARTCARD_API_H__
#ifdef __cplusplus
extern "C" {
#endif

#include "app_common_play.h"

uint8_t app_gy_cas_api_smart_check_valid(void);
uint8_t app_gy_cas_api_card_in(char * atr,uint8_t len);
uint8_t app_gy_cas_api_card_out(void);

#ifdef __cplusplus
}
#endif
#endif /*__APP_GYCAS_SMARTCARD_API_H__*/

