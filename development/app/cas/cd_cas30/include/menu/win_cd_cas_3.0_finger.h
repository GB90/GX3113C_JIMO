/**
 *
 * @file        app_by_ca_api_finger.h
 * @brief
 * @version     1.1.0
 * @date        11/30/2012 09:47:49 AM
 * @author      zhouhuaming (zhouhm), zhuohm@nationalchip.com
 *
 */
#ifndef __WIN_CD_CAS30_FINGER_H__
#define __WIN_CD_CAS30_FINGER_H__
#ifdef __cplusplus
extern "C" {
#endif

#include "app_common_porting_stb_api.h"
	
int32_t app_cd_cas30_gxmsg_ca_on_event_finger(GxMsgProperty0_OnEvent* event);
int32_t app_cd_cas30_finger_exec(void);

#ifdef __cplusplus
}
#endif
#endif /*__WIN_CD_CAS30_FINGER_H__*/

