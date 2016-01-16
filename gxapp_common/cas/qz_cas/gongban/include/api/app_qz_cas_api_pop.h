/**
 *
 * @file        app_qz_cas_api_pop.h
 * @brief
 * @version     1.1.0
 * @date        04/15/2014 
 * @author      chenth, chenth@nationalchip.com
 *
 */
#ifndef __APP_QZ_CAS_API_POP_H__
#define __APP_QZ_CAS_API_POP_H__
#ifdef __cplusplus
extern "C" {
#endif
#include "app_common_play.h"
#include <gxos/gxcore_os_core.h>


#define QZ_CA_MAX_LEN_OSD          240
#define QZ_CA_OSD_TOP              0x01  /* OSD风格：显示在屏幕上方 */
#define QZ_CA_OSD_NUM              1


typedef enum
{
	QZ_MSG_OSD=0,
	QZ_MSG_MAX
}QZ_MSG_TYPE;

typedef struct {
	uint8_t wStatus; /* 0 --不需要显示，1-- 需要显示*/
	uint8_t roll_status; /*0 -- 当前滚动状态，1-- 当前非滚动状态*/
	uint8_t byStyle; /*osd滚动消息在全屏界面位置类型*/
	uint8_t message_type; /* 0 --osd，1-- msg*/
	int32_t wTimes;       /*已滚动次数*/
    handle_t    mutex;    /*互斥锁*/
	uint8_t content[QZ_CA_MAX_LEN_OSD];
} qz_cas_rolling_message_st; 

#define rolling_message_cpy(pdst,psrc) \
        (pdst)->wStatus = (psrc)->wStatus; \
        (pdst)->roll_status = (psrc)->roll_status; \
        (pdst)->byStyle = (psrc)->byStyle; \
        (pdst)->message_type = (psrc)->message_type; \
        (pdst)->wTimes = (psrc)->wTimes; \
        memcpy((pdst)->content,(psrc)->content,QZ_CA_MAX_LEN_OSD);


int32_t app_qz_cas_gxmsg_ca_on_event_exec(GxMessage * msg);
uint8_t app_qz_cas_api_is_ca_pop_msg_type(uint8_t type);
int32_t app_qz_cas_api_pop_msg_exec(uint8_t type);
int32_t app_qz_cas_api_osd_exec(void);

int32_t qzcas_get_config_hs_mode(void);
int32_t qzcas_set_config_hs_mode(int32_t config);

#ifdef __cplusplus
}
#endif
#endif /*__APP_QZ_CAS_API_POP_H__*/

