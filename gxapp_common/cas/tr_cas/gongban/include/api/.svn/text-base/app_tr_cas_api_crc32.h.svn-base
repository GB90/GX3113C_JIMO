/**
 *
 * @file        app_tr_cas_api_crc32.h
 * @brief
 * @version     1.1.0
 * @date        12/29/2014 
 * @author      wangjian (wj), wangjian@nationalchip.com
 *
 */
#ifndef __APP_TR_CAS_CRC32_H__
#define __APP_TR_CAS_CRC32_H__
   
#include "app_common_play.h"

#ifdef __cplusplus
extern "C" {
#endif 

void app_tr_cas_crc32_init(int32_t width, uint32_t poly, uint32_t init, 
     						uint32_t refin, uint32_t refout, uint32_t xorot);
void app_tr_cas_crc32_reset(void);
void app_tr_cas_crc32_nextbyte(uint8_t ch);
uint32_t app_tr_cas_crc32_get_result(void);

#ifdef __cplusplus
}
#endif

#endif

