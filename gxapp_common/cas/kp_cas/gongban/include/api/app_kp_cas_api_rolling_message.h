#ifndef __APP_MG_CAS_API_ROLLING_MESSAGE_H__
#define __APP_MG_CAS_API_ROLLING_MESSAGE_H__
#ifdef __cplusplus
extern "C" {
#endif

#include "app_common_play.h"
#include <gxos/gxcore_os_core.h>

#define CA_MAX_LEN_OSD          256
#define CA_OSD_TOP              0x01  /* OSD�����ʾ����Ļ�Ϸ� */
#define CA_OSD_BOTTOM           0x02  /* OSD�����ʾ����Ļ�·� */

typedef enum
{
	CA_OSD_CTRL_TYPE_R2L = 0,
    CA_OSD_CTRL_TYPE_L2R,
}CA_OSD_CTRL_TYPE;


typedef struct {
	uint8_t wStatus; /* 0 --����Ҫ��ʾ��1-- ��Ҫ��ʾ*/
	uint8_t roll_status; /*0 -- ��ǰ����״̬��1-- ��ǰ�ǹ���״̬*/
	uint8_t byStyle; /*osd������Ϣ��ȫ������λ������*/
	uint8_t message_type; /* 0 --osd��1-- msg*/
	int32_t wTimes;       /*�ѹ�������*/
    handle_t mutex;
	uint8_t content[CA_MAX_LEN_OSD];
} kp_cas_rolling_message_st; 

void kp_cas_rolling_test();
int Osd_Analyze(char *text,int len,uint8_t *style,uint8_t *times_min,uint8_t *time_num,char *cMsg);
void kp_cas_sent_osdmsg(uint8_t style,uint8_t times_min,uint8_t time_num,char *msg);
int8_t app_kp_cas_rolling_message_init(void);
int32_t app_kp_cas_rolling_message_exec(void);
int32_t app_kp_cas_gxmsg_ca_on_event_msg(GxMsgProperty0_OnEvent* event);



#ifdef __cplusplus
}
#endif
#endif /*__APP_KP_CAS_API_ROLLING_MESSAGE_H__*/




