/**
 *
 * @file
 * @brief
 * @version     1.1.0
 * @date        11/30/2012 09:47:49 AM
 * @author      zhouhuaming (zhouhm), zhuohm@nationalchip.com
 *
 */
#ifndef __WIN_DSCAS_50_POP_MSG_H__
#define __WIN_DSCAS_50_POP_MSG_H__
#ifdef __cplusplus
extern "C" {
#endif


int32_t app_desai_cas50_gxmsg_ca_on_event_exec(GxMessage * msg);
uint8_t app_desai_cas50_api_is_ca_pop_msg_type(uint8_t type);
 int32_t app_desai_cas50_api_pop_msg_exec(uint8_t type);
 int32_t app_desai_cas50_api_osd_exec(void);
/*
* 2014-11-27，qm,获取卡升级状态
*/
int32_t app_desai_cas50_api_get_card_update_status(void);
#ifdef __cplusplus
}
#endif
#endif

