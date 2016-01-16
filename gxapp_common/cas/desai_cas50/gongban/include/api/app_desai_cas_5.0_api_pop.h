/**
 *
 * @file        app_by_cas_api_baseinfo.h
 * @brief
 * @version     1.1.0
 * @date        11/30/2012 09:47:49 AM
 * @author      zhouhuaming (zhouhm), zhuohm@nationalchip.com
 *
 */
#ifndef __APP_DSCAS_50_API_POP_H__
#define __APP_DSCAS_50_API_POP_H__
#ifdef __cplusplus
extern "C" {
#endif
#include "desai_ca_api.h"
typedef struct {
	uint8_t wStatus;
	uint16_t wShowStatus; /* 0 --����״̬��1 -- ��ʾ״̬*/
	uint8_t *buffer;
	int32_t position;

} desai_cas50_finger_data_st;

typedef struct {
	uint8_t wStatus;
	uint16_t roll_status; /* 0 --����״̬��1 -- ��ʾ״̬*/
	int32_t wTimes;       /*�ѹ�������*/
	uint8_t *buffer;
	uint8_t from;/*1:CA,2:AD*/
} desai_cas50_osd_data_st;
typedef struct {
	int					enDisplayPos; //osd��ʾλ��
	uint8_t				aucBackgroundColor[4];//������ɫ
	uint8_t				aucDisplayColor[4];   //������ɫ
	uint16_t				usContentSize;        //osd���ݳ���
	uint16_t				usGetBufferSize;
	uint16_t* 			pucDisplayBuffer;     //osd����
	uint16_t				ucFontType;           //��������
	uint16_t				ucFontSize;           //�����С
	uint16_t				ucShowTimes;          //��ʾ����
} desai_cas50_osd_detail_st;


typedef struct {
	uint8_t wStatus; /* ��Ҫ��ʾ�ʼ�ͼ������*/
	uint8_t wShowStatus; /*��ǰ��ʾ״̬*/
	uint8_t uCount; /* ��ʾ�ۼƴ�����������˸*/
	uint32_t emailID;
} desai_cas50_email_st;

typedef struct {

	uint16_t wStatus; /* 0 --����״̬��1 -- ��ʾ״̬*/
	int32_t *buffer;

} desai_cas50_exigent_data_st;

typedef struct {
	uint8_t wStatus; /* Ҫ����ʾ��ȡ�������*/
	uint8_t wShowStatus; /*��ǰ�������ʾ״̬*/
	uint16_t wEcmPid;
	ST_IPPV_INFO ippvBuyInfo;
	uint8_t byMessageType;
} desai_cas50_ippv_buy_st;

typedef enum
{
    DESAI_CAS50_CARD_UPDATE_NONE,
    DESAI_CAS50_CARD_UPDATE_START,      /*������ʼ*/
    DESAI_CAS50_CARD_UPDATE_PROCESS,    /*���½���*/
    DESAI_CAS50_CARD_UPDATE_SUCCESS,    /*�����ɹ�*/
    DESAI_CAS50_CARD_UPDATE_ERR,        /*���´���*/


} DESAI_CAS50_CARD_UPDATE_STATUS_E;

typedef struct
{
	DESAI_CAS50_CARD_UPDATE_STATUS_E bStatus;
    uint8_t bProgress;

} DESAI_CAS50_CARD_UPDATE_S;

#if 0
typedef struct {
	uint16_t wStatus; /* 0 --�����1 -- ��ʾ���� */
	uint16_t wShowStatus; /* 0 --����״̬��1 -- ��ʾ״̬*/
	uint16_t wEcmPid;
	uint32_t dwCardID;
	uint32_t position; /*�����ʾλ������*/
//	uint32_t wDuration;
} cd_cas30_finger_data_st;

//#define ROLL_TITLE_MAX (100)
#define ROLL_CONTENT_MAX (1024)

typedef struct {
	uint8_t wStatus; /* 0 --����Ҫ��ʾ��1-- ��Ҫ��ʾ*/
	uint8_t roll_status; /*0 -- ��ǰ����״̬��1-- ��ǰ�ǹ���״̬*/
	uint8_t byStyle; /*osd������Ϣ��ȫ������λ������*/
	uint8_t content[CDCA_MAXLEN_OSD];
} cd_cas30_rolling_message_st;

typedef struct {
	uint8_t wStatus; /* ����Ȩ��Ҫ��ʾͼ������*/
	uint8_t wShowStatus; /*��ǰ��ʾ״̬*/
	uint8_t uCount; /* ��ʾ�ۼƴ�����������˸*/
} cd_cas30_detitle_st;


typedef struct {
	uint8_t wStatus; /* Ҫ����ʾ��ȡ�������*/
	uint8_t wShowStatus; /*��ǰ�������ʾ״̬*/
	uint16_t wEcmPid;
	SCDCAIppvBuyInfo ippvBuyInfo;
	uint8_t byMessageType;
} cd_cas30_ippv_buy_st;

typedef struct {
	uint8_t wStatus; /* ��Ҫ��ʾ�ʼ�ͼ������*/
	uint8_t wShowStatus; /*��ǰ��ʾ״̬*/
	uint8_t uCount; /* ��ʾ�ۼƴ�����������˸*/
	uint32_t emailID;
} cd_cas30_email_st;
#endif

#ifdef __cplusplus
}
#endif
#endif /*__APP_DSCAS_50_API_POP_H__*/

