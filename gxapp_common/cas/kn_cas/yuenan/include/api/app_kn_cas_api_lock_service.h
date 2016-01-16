/**
 *
 * @file        app_by_cas_api_lock_service.h
 * @brief
 * @version     1.1.0
 * @date        11/30/2012 09:47:49 AM
 * @author      zhouhuaming (zhouhm), zhuohm@nationalchip.com
 *
 */
#ifndef __APP_KNCAS_LOCK_SERVICE_API_H__
#define __APP_KNCAS_LOCK_SERVICE_API_H__
#ifdef __cplusplus
extern "C" {
#endif

void app_kn_cas_api_lock_service(unsigned short wOriNetID,unsigned short wTSID,unsigned short wServiceID,unsigned char *pUTCTime);
uint8_t app_kn_cas_api_set_lock_status(uint8_t lock_status);
uint8_t app_kn_cas_api_get_lock_status(void);

#ifdef __cplusplus
}
#endif
#endif /*__APP_KNCAS_LOCK_SERVICE_API_H__*/

