/**
 *
 * @file        app_mg_cas312_api_lock_service.h
 * @brief
 * @version     1.1.0
 * @date        04/16/2014 
 * @author      zhouhuaming (zhouhm), zhuohm@nationalchip.com
 *
 */
#ifndef __APP_MG_CAS312_LOCK_SERVICE_API_H__
#define __APP_MG_CAS312_LOCK_SERVICE_API_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "mg312def.h"

void app_mg_cas312_api_lock_service(uint16_t wServiceID);
uint8_t app_mg_cas312_api_set_lock_status(uint8_t lock_status);
uint8_t app_mg_cas312_api_get_lock_status(void);

#ifdef __cplusplus
}
#endif

#endif /*__APP_MG_CAS312_LOCK_SERVICE_API_H__*/

