#ifndef __APP_KP_CAS_API_EVENT_H__
#define __APP_KP_CAS_API_EVENT_H__
#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
	uint16_t wStatus; /* 0 --Çå³ý£¬1 -- ÏÔÊ¾¿¨ºÅ 2 -- ÏÔÊ¾ECMIDºÍ¿¨ºÅ*/
	uint16_t wShowStatus; /* 0 --Òþ²Ø×´Ì¬£¬1 -- ÏÔÊ¾×´Ì¬*/
	char  msg[64];
} kp_ca_finger_data_st; 

void KPCA_toShowFinger(int OnOff,char *msg);
int32_t app_kp_cas_gxmsg_ca_on_event_finger(GxMsgProperty0_OnEvent* event);
int32_t app_kp_cas_finger_exec(void);

#ifdef __cplusplus
}
#endif
#endif 

