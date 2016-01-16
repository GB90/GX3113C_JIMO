/**
 *
 * @file        app_tr_cas_api_smart.h
 * @brief
 * @version     1.1.0
 * @date        11/30/2012 09:47:49 AM
 * @author     
 *
 */
#ifndef __APP_TRCAS_SMARTCARD_API_H__
#define __APP_TRCAS_SMARTCARD_API_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "app_common_play.h"

#if 1
#define RUNTIME_ETU   			(372)
#define RUNTIME_BAUD_RATE 	 	(9600 * RUNTIME_ETU)
#define RUNTIME_EGT    			(RUNTIME_ETU * 30)

#define RUNTIME_WDT   			(200 * RUNTIME_ETU)
#define RUNTIME_TGT   			(0)
#define RUNTIME_TWDT   			(3 * 9600 * RUNTIME_ETU)
#else
#define RUNTIME_ETU                         (372)
#define RUNTIME_BAUD_RATE             (3570000)//(9600*RUNTIME_ETU)
#define RUNTIME_EGT                   (RUNTIME_ETU * 30)
#define RUNTIME_WDT                   (400 * RUNTIME_ETU)
#define RUNTIME_TGT                         (0)
#define RUNTIME_TWDT                   (0x1ffffff)
#endif
uint8_t app_tr_cas_api_smart_check_valid(void);
uint8_t app_tr_cas_api_card_in(char *atr, uint8_t len);
uint8_t app_tr_cas_api_card_out(void);

void TRDRV_SCARD_In(char *pbyATR);
void TRDRV_SCARD_Remove(char *pbyATR);

#ifdef __cplusplus
}
#endif

#endif /*__APP_TR_CAS_SMARTCARD_API_H__*/

