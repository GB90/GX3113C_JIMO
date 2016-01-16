/**
 *
 * @file        app_cd_cas_3.0_api_smart.h
 * @brief
 * @version     1.1.0
 * @date        11/30/2012 09:47:49 AM
 * @author      zhouhuaming (zhouhm), zhuohm@nationalchip.com
 *
 */
#ifndef __APP_CDCAS_30_LOCK_SERVICE_API_H__
#define __APP_CDCAS_30_LOCK_SERVICE_API_H__
#ifdef __cplusplus
extern "C" {
#endif

#include "CDCASS.h"
typedef struct
{
    uint8_t                   wStatus;
    uint8_t 		     wShowStatus;
    uint16_t                   wCurtainCode;
    char *				str;
}App_CaCurtainNotify; 


void app_cd_cas30_api_lock_service(const SCDCALockService* pLockService);
uint8_t app_cd_cas30_api_set_lock_status(uint8_t lock_status);
uint8_t app_cd_cas30_api_get_lock_status(void);

#ifdef __cplusplus
}
#endif
#endif /*__APP_CDCAS_30_LOCK_SERVICE_API_H__*/

