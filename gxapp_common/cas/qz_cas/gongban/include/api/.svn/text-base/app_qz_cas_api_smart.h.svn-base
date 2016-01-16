/**
 *
 * @file        app_qz_cas_api_smart.h
 * @brief
 * @version     1.1.0
 * @date        05/6/2014 
 * @author      chenth, chenth@nationalchip.com
 *
 */
#ifndef __APP_QZ_CAS_API_SMART_H__
#define __APP_QZ_CAS_API_SMART_H__
#ifdef __cplusplus
extern "C" {
#endif

#include "app_common_play.h"

typedef struct
{
    unsigned char super_cas_id[4];
    unsigned char smartcard_id[3];
    unsigned char cw_mask[8];
    unsigned char area_code[4];
}SMARTCARD_INFO;

uint8_t app_qz_cas_api_smart_check_valid(void);
uint8_t app_qz_cas_api_card_in(char* atr,uint8_t len);
uint8_t app_qz_cas_api_card_out(void);
uint8_t app_qz_cas_api_is_ca_pop_msg_type(uint8_t type);
int32_t app_qz_cas_api_pop_msg_exec(uint8_t type);
uint8_t app_qz_cas_api_smart_reset(uint8_t* pbyATR, uint8_t *pbyLen );
void app_qz_cas_api_smart_init(void);
#ifdef __cplusplus
}
#endif
#endif /*__APP_QZ_CAS_API_SMART_H__*/

