/**
 *
 * @file        
 * @brief
 * @version     1.1.0
 * @date        11/30/2012 09:47:49 AM
 * @author      zhouhuaming (zhouhm), zhuohm@nationalchip.com
 *
 */
#ifndef __WIN_DS_CAS50_EXIGENT_H__
#define __WIN_DS_CAS50_EXIGENT_H__
#ifdef __cplusplus
extern "C" {
#endif

uint8_t app_desai_cas50_api_set_lock_status(uint8_t lock_status);
uint8_t app_desai_cas50_api_get_lock_status(void);
int32_t app_desai_cas50_gxmsg_ca_on_event_exigent(GxMsgProperty0_OnEvent* event);


#ifdef __cplusplus
}
#endif
#endif /*__WIN_DS_CAS50_EXIGENT_H__*/

