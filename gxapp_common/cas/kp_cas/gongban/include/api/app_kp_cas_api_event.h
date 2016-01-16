#ifndef __APP_KP_CAS_API_EVENT_H__
#define __APP_KP_CAS_API_EVENT_H__
#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
	uint16_t wStatus; /* 0 --�����1 -- ��ʾ���� 2 -- ��ʾECMID�Ϳ���*/
	uint16_t wShowStatus; /* 0 --����״̬��1 -- ��ʾ״̬*/
	char  msg[64];
} kp_ca_finger_data_st; 

void KPCA_toShowFinger(int OnOff,char *msg);
int32_t app_kp_cas_gxmsg_ca_on_event_finger(GxMsgProperty0_OnEvent* event);
int32_t app_kp_cas_finger_exec(void);

#ifdef __cplusplus
}
#endif
#endif 

