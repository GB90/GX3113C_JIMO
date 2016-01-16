/**
 *
 * @file        app_gos_cas_api_pop.h
 * @brief
 * @version     1.1.0
 * @date        04/22/2014 
 * @author      zhouhuaming (zhouhm), zhuohm@nationalchip.com
 *
 */
#ifndef __APP_GOS_CAS_API_POP_H__
#define __APP_GOS_CAS_API_POP_H__
#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
	uint16_t wStatus; /* 0 --�����1 -- ��ʾ���� */
	uint16_t wShowStatus; /* 0 --����״̬��1 -- ��ʾ״̬*/
	uint16_t wEcmPid;
	uint32_t dwCardID;
	uint32_t position; /*�����ʾλ������*/
	uint32_t wDuration;
	uint32_t wusedTime;
	uint32_t version;
} gos_cas_finger_data_st; 

//#define ROLL_TITLE_MAX (100)
#define ROLL_CONTENT_MAX (1024)

typedef struct {
	uint8_t wStatus; /* 0 --����Ҫ��ʾ��1-- ��Ҫ��ʾ*/
	uint8_t roll_status; /*0 -- ��ǰ����״̬��1-- ��ǰ�ǹ���״̬*/
	uint8_t byStyle; /*osd������Ϣ��ȫ������λ������*/
	uint8_t content[210];
	uint32_t OSD_version;
	uint32_t wTimes;
} gos_cas_rolling_message_st; 

typedef struct {
	uint8_t wStatus; /* ��Ҫ��ʾ�ʼ�ͼ������*/
	uint8_t wShowStatus; /*��ǰ��ʾ״̬*/
	uint8_t uCount; /* ��ʾ�ۼƴ�����������˸*/
	uint32_t emailID;
} gos_cas_email_st; 

typedef enum
{
	CA_GOS_No_OSDPromt=0,			/*��OSD��ʾ*/	
	CA_GOS_SMART_STOP,              /*���ѱ�����*/
    CA_GOS_SMART_AREA_NOPAIR,		/*��������ƥ��*/
	CA_GOS_SMART_IPPT_SWITCH,		/*IPPT�û���ʾ*/
    CA_GOS_SMART_STB_NOPAIR,			/*���������*/
    CA_GOS_SMART_PROG_NOENTITLE,		/*��Ŀ��δ��Ȩ*/
    CA_GOS_SMART_PROG_NOENTITLE_IPPT,/*��Ŀ��δ��Ȩ*/
    CA_GOS_SMART_MOTHER_SON_PAIR,	/*��ĸ����������*/
    CA_GOS_SMART_MOTHER_SON_TIMEOUT,	/*��ĸ����Գ�ʱ*/
    CA_GOS_SMART_PAIR_OK,			/*������ӦOK*/
    CA_GOS_SMART_LAST_DAY,			/*�����ʾ����*/	
    CA_GOS_SMART_AREA_LOCK,			/*���������� */
	CA_GOS_SMART_CARD_LOCK,			/*���ѱ����� */
	CA_GOS_SMART_IPPT,				/*IPPT��ʾ*/
	CA_GOS_SMART_LIMIT_WATCH,		/*�������տ�*/
	CA_GOS_SMART_NO_WORK_TIME,		/*�����ڹ���ʱ���*/
	CA_GOS_SMART_OUT				/*��Ŀ���ţ���忨*/
//	GOSCA_SMART_UNKNOWN_ERROR,		/*δ֪����*/
}CA_GOS_SMART_STATUS;


int32_t app_gos_cas_gxmsg_ca_on_event_exec(GxMessage * msg);
uint8_t app_gos_cas_api_is_ca_pop_msg_type(uint8_t type);
 int32_t app_gos_cas_api_pop_msg_exec(uint8_t type);
 int32_t app_gos_cas_api_osd_exec(void);



#ifdef __cplusplus
}
#endif
#endif /*__APP_GOS_CAS_API_POP_H__*/

