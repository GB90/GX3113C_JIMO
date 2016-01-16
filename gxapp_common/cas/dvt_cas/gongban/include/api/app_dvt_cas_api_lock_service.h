/**
 *
 * @file        app_dvt_cas_api_lock_service.h
 * @brief
 * @version     1.1.0
 * @date        11/30/2012 09:47:49 AM
 * @author      zhouhuaming (zhouhm), zhuohm@nationalchip.com
 *
 */
#ifndef __APP_DVT_CAS_LOCK_SERVICE_API_H__
#define __APP_DVT_CAS_LOCK_SERVICE_API_H__
#ifdef __cplusplus
extern "C" {
#endif

#include "DVTCAS_STBDataPublic.h"

void app_dvt_cas_api_lock_service(WORD wOriNetID, WORD wTSID, WORD wServiceID, WORD wDuration);
uint8_t app_dvt_cas_api_set_lock_status(uint8_t lock_status);
uint8_t app_dvt_cas_api_get_lock_status(void);

#ifdef __cplusplus
}
#endif
#endif /*__APP_DVT_CAS_LOCK_SERVICE_API_H__*/

