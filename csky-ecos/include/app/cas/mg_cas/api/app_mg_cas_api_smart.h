/**
 *
 * @file        app_mg_cas_api_smart.h
 * @brief
 * @version     1.1.0
 * @date        04/11/2014 
 * @author      zhouhuaming (zhouhm), zhuohm@nationalchip.com
 *
 */
#ifndef __APP_MG_CAS_POP_MSG_API_H__
#define __APP_MG_CAS_POP_MSG_API_H__
#ifdef __cplusplus
extern "C" {
#endif

#include "app_common_play.h"

uint8_t app_mg_cas_api_smart_check_valid(void);
uint8_t app_mg_cas_api_card_in(char* atr,uint8_t len);
uint8_t app_mg_cas_api_card_out(void);
uint8_t app_mg_cas_api_is_ca_pop_msg_type(uint8_t type);
int32_t app_mg_cas_api_pop_msg_exec(uint8_t type);
uint8_t app_mg_cas_api_smart_reset(uint8_t* pbyATR, uint8_t *pbyLen );
void app_mg_cas_api_smart_init(void);
#ifdef __cplusplus
}
#endif
#endif /*__APP_MG_CAS_POP_MSG_API_H__*/

