/**
 *
 * @file        app_tr_cas_api_finger.h
 * @brief
 * @version     1.1.0
 * @date        11/29/2013 
 * @author      zhouhuaming (zhouhm), zhuohm@nationalchip.com
 *
 */
#ifndef __APP_TR_CAS_API_FINGER_H__
#define __APP_TR_CAS_API_FINGER_H__

#ifdef __cplusplus
extern "C" {
#endif
#include "Tr_Cas.h"
#include "app_common_play.h"


typedef struct 
{
	uint8_t  bHashedNumLen;
	uint8_t pbHashedNum[FP_SCNUM_MAX_LENGTH];
	uint16_t wStatus; /* 0 --Çå³ý£¬1 -- ÏÔÊ¾¿¨ºÅ */
	uint16_t wShowStatus; /* 0 --Òþ²Ø×´Ì¬£¬1 -- ÏÔÊ¾×´Ì¬*/
	uint32_t position; /*Ëæ»úÏÔÊ¾Î»ÖÃË÷Òý*/
	uint32_t wDuration;
	uint32_t wusedTime;
} tr_cas_finger_data_st; 


int32_t app_tr_cas_gxmsg_ca_on_event_finger(GxMsgProperty0_OnEvent* event);
int32_t app_tr_cas_finger_exec(void);


#ifdef __cplusplus
}
#endif

#endif /*__APP_DVT_CAS_API_FINGER_H__*/

