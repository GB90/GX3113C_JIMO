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
	uint16_t wShowStatus; /* 0 --隐藏状态，1 -- 显示状态*/
	uint8_t *buffer;
	int32_t position;

} desai_cas50_finger_data_st;

typedef struct {
	uint8_t wStatus;
	uint16_t roll_status; /* 0 --隐藏状态，1 -- 显示状态*/
	int32_t wTimes;       /*已滚动次数*/
	uint8_t *buffer;
	uint8_t from;/*1:CA,2:AD*/
} desai_cas50_osd_data_st;
typedef struct {
	int					enDisplayPos; //osd显示位置
	uint8_t				aucBackgroundColor[4];//背景颜色
	uint8_t				aucDisplayColor[4];   //字体颜色
	uint16_t				usContentSize;        //osd内容长度
	uint16_t				usGetBufferSize;
	uint16_t* 			pucDisplayBuffer;     //osd内容
	uint16_t				ucFontType;           //字体类型
	uint16_t				ucFontSize;           //字体大小
	uint16_t				ucShowTimes;          //显示次数
} desai_cas50_osd_detail_st;


typedef struct {
	uint8_t wStatus; /* 需要显示邮件图标类型*/
	uint8_t wShowStatus; /*当前显示状态*/
	uint8_t uCount; /* 显示累计次数，处理闪烁*/
	uint32_t emailID;
} desai_cas50_email_st;

typedef struct {

	uint16_t wStatus; /* 0 --隐藏状态，1 -- 显示状态*/
	int32_t *buffer;

} desai_cas50_exigent_data_st;

typedef struct {
	uint8_t wStatus; /* 要求显示或取消购买框*/
	uint8_t wShowStatus; /*当前购买框显示状态*/
	uint16_t wEcmPid;
	ST_IPPV_INFO ippvBuyInfo;
	uint8_t byMessageType;
} desai_cas50_ippv_buy_st;

typedef enum
{
    DESAI_CAS50_CARD_UPDATE_NONE,
    DESAI_CAS50_CARD_UPDATE_START,      /*升级开始*/
    DESAI_CAS50_CARD_UPDATE_PROCESS,    /*更新进度*/
    DESAI_CAS50_CARD_UPDATE_SUCCESS,    /*升级成功*/
    DESAI_CAS50_CARD_UPDATE_ERR,        /*更新错误*/


} DESAI_CAS50_CARD_UPDATE_STATUS_E;

typedef struct
{
	DESAI_CAS50_CARD_UPDATE_STATUS_E bStatus;
    uint8_t bProgress;

} DESAI_CAS50_CARD_UPDATE_S;

#if 0
typedef struct {
	uint16_t wStatus; /* 0 --清除，1 -- 显示卡号 */
	uint16_t wShowStatus; /* 0 --隐藏状态，1 -- 显示状态*/
	uint16_t wEcmPid;
	uint32_t dwCardID;
	uint32_t position; /*随机显示位置索引*/
//	uint32_t wDuration;
} cd_cas30_finger_data_st;

//#define ROLL_TITLE_MAX (100)
#define ROLL_CONTENT_MAX (1024)

typedef struct {
	uint8_t wStatus; /* 0 --不需要显示，1-- 需要显示*/
	uint8_t roll_status; /*0 -- 当前滚动状态，1-- 当前非滚动状态*/
	uint8_t byStyle; /*osd滚动消息在全屏界面位置类型*/
	uint8_t content[CDCA_MAXLEN_OSD];
} cd_cas30_rolling_message_st;

typedef struct {
	uint8_t wStatus; /* 反授权需要显示图标类型*/
	uint8_t wShowStatus; /*当前显示状态*/
	uint8_t uCount; /* 显示累计次数，处理闪烁*/
} cd_cas30_detitle_st;


typedef struct {
	uint8_t wStatus; /* 要求显示或取消购买框*/
	uint8_t wShowStatus; /*当前购买框显示状态*/
	uint16_t wEcmPid;
	SCDCAIppvBuyInfo ippvBuyInfo;
	uint8_t byMessageType;
} cd_cas30_ippv_buy_st;

typedef struct {
	uint8_t wStatus; /* 需要显示邮件图标类型*/
	uint8_t wShowStatus; /*当前显示状态*/
	uint8_t uCount; /* 显示累计次数，处理闪烁*/
	uint32_t emailID;
} cd_cas30_email_st;
#endif

#ifdef __cplusplus
}
#endif
#endif /*__APP_DSCAS_50_API_POP_H__*/

