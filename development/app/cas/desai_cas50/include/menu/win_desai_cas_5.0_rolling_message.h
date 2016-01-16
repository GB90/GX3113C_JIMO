/**
 *
 * @file        win_by_cas_rolling_message.h
 * @brief
 * @version     1.1.0
 * @date        11/30/2012 09:47:49 AM
 * @author      zhouhuaming (zhouhm), zhuohm@nationalchip.com
 *
 */
#ifndef __WIN_DS_CAS50_ROLLING_MESSAGE_H__
#define __WIN_DS_CAS50_ROLLING_MESSAGE_H__
#ifdef __cplusplus
extern "C" {
#endif

#include "app_common_play.h"


int8_t app_desai_cas50_rolling_message_init(void);
int32_t app_desai_cas50_gxmsg_ca_on_event_rolling(GxMsgProperty0_OnEvent* event);
int32_t app_desai_cas50_rolling_message_exec(void);

#ifdef __cplusplus
}
#endif
#endif /*__WIN_DS_CAS50_ROLLING_MESSAGE_H__*/

