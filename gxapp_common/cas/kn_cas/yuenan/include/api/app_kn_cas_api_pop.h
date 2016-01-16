/**
 *
 * @file        app_by_cas_api_baseinfo.h
 * @brief
 * @version     1.1.0
 * @date        11/30/2012 09:47:49 AM
 * @author      zhouhuaming (zhouhm), zhuohm@nationalchip.com
 *
 */
#ifndef __APP_BYCAS_API_POP_H__
#define __APP_BYCAS_API_POP_H__
#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
	uint16_t wStatus; /* 0 --�����1 -- ��ʾ���� 2 -- ��ʾECMID�Ϳ���*/
	uint16_t wShowStatus; /* 0 --����״̬��1 -- ��ʾ״̬*/
	uint16_t wEcmPid;
	uint32_t dwCardID;
	uint32_t wDuration;
} by_ca_finger_data_st; 

#define ROLL_TITLE_MAX (100)
#define ROLL_CONTENT_MAX (1024)

typedef struct {
	uint8_t wStatus; /* 0 --����Ҫ��ʾ��1-- ��Ҫ��ʾ*/
	uint8_t roll_status; /*0 -- ��ǰ����״̬��1-- ��ǰ�ǹ���״̬*/
	uint8_t title[ROLL_TITLE_MAX];
	uint8_t content[ROLL_CONTENT_MAX];
	uint16_t wDuration;
	int32_t wTimes;       /*�ѹ�������*/
	uint32_t wStartTime; /*��¼��ʼ����ʱ�䣬���Լ������ʱ��*/
} by_ca_rolling_message_st; 


#ifdef __cplusplus
}
#endif
#endif /*__APP_BYCAS_API_POP_H__*/

