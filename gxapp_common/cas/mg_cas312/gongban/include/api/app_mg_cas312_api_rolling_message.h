/**
 *
 * @file        app_mg_cas312_api_rolling_message.h
 * @brief
 * @version     1.1.0
 * @date        04/16/2014 
 * @author      zhouhuaming (zhouhm), zhuohm@nationalchip.com
 *
 */
#ifndef __APP_MG_CAS312_API_ROLLING_MESSAGE_H__
#define __APP_MG_CAS312_API_ROLLING_MESSAGE_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "app_common_play.h"

int8_t app_mg_cas312_rolling_message_init(void);
int32_t app_mg_cas312_gxmsg_ca_on_event_msg(GxMsgProperty0_OnEvent* event);
int32_t app_mg_cas312_gxmsg_ca_on_event_rolling(GxMsgProperty0_OnEvent* event);
int32_t app_mg_cas312_rolling_message_exec(void);
void app_mg_cas312_rolling_message_hide(void);/*wangjian add on 20141218*/

#ifdef __cplusplus
}
#endif

#endif /*__APP_MG_CAS312_API_ROLLING_MESSAGE_H__*/

