/**
 *
 * @file        app_cd_cas_3.0_api_detitle.h
 * @brief
 * @version     1.1.0
 * @date        11/30/2012 09:47:49 AM
 * @author      zhouhuaming (zhouhm), zhuohm@nationalchip.com
 *
 */
#ifndef __APP_GYCAS_API_PIN_H__
#define __APP_GYCAS_API_PIN_H__
#ifdef __cplusplus
extern "C" {
#endif

#include "app_common_play.h"

#define GYCA_MAXLEN_PINCODE  6


char* app_gy_cas_api_change_pin(ca_pin_t* data);

#ifdef __cplusplus
}
#endif
#endif /*__APP_CDCAS_30_API_PIN_H__*/

