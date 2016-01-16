/*****************************************************************************
* 						   CONFIDENTIAL
*        Hangzhou GuoXin Science and Technology Co., Ltd.
*                      (C)2012, All right reserved
******************************************************************************

******************************************************************************
* File Name :	app_ca_api.c
* Author    : 	zhouhm
* Project   :	goxceed dvbc
* Type      :
******************************************************************************
* Purpose   :	ģ��ͷ�ļ�
******************************************************************************
* Release History:
VERSION	Date			  AUTHOR         Description
1.0  	2012.12.02		  zhouhm 	 			creation
*****************************************************************************/
//#include "BYCAca_stb.h"
#include "stb_ca2app.h"
#include "app_common_porting_stb_api.h"
#include "app_qilian_cas_api_pop.h"
#include "app_qilian_cas_api_finger.h"

extern 	void qilian_cas_finger_show(uint8_t number,uint8_t showFlag,char* message);

static qilian_ca_finger_data_st ca_finger_data = {0};
int32_t app_qilian_cas_gxmsg_ca_on_event_finger(GxMsgProperty0_OnEvent* event)
{
	qilian_ca_finger_data_st* finger_data = NULL;
	if (NULL == event)
		return 0;

	finger_data = (qilian_ca_finger_data_st*)(event->buf);

	if (NULL == finger_data)
		return 0;

	if ((0 != ca_finger_data.wStatus)&&(TRUE == ca_finger_data.wShowStatus))
	{
		qilian_cas_finger_show(0,DVB_CA_FINGER_FLAG_HIDE,NULL); 				
	}

	memcpy(&ca_finger_data,finger_data,sizeof(qilian_ca_finger_data_st));

//	ca_finger_data.wStatus = 0;
	ca_finger_data.wShowStatus = 0;

	
	return 1;
}

int32_t app_qilian_cas_finger_exec(void)
{
	uint8_t chFingerBuf[100]={0};	
#if 0
	if (0 == ca_finger_data.wStatus)
		{
			/*���*/
			if (TRUE == ca_finger_data.wShowStatus)
				{
					ca_finger_data.wShowStatus = 0;
					qilian_cas_finger_show(0,DVB_CA_FINGER_FLAG_HIDE,NULL); 				
				}
		}
	else
		if (1 == ca_finger_data.wStatus)
		{
			/*��ʾ����*/
			if (FALSE == ca_finger_data.wShowStatus)
				{
					sprintf((char*)chFingerBuf, "id:%u", ca_finger_data.dwCardID);
					qilian_cas_finger_show(0,DVB_CA_FINGER_FLAG_SHOW,(char*)chFingerBuf); 				
					ca_finger_data.wShowStatus = TRUE;
				}
		}
	else
		if (2 == ca_finger_data.wStatus)
		{
			/*��ʾPID�Ϳ���*/
			if (FALSE == ca_finger_data.wShowStatus)
				{
					sprintf((char*)chFingerBuf, "ecmpid:0x%x ,id:%u", ca_finger_data.wEcmPid,ca_finger_data.dwCardID);
					qilian_cas_finger_show(0,DVB_CA_FINGER_FLAG_SHOW,(char*)chFingerBuf); 
					ca_finger_data.wShowStatus = TRUE;
				}
		}
#else
	if (0 == ca_finger_data.wStatus)
	{
		/*���*/
		if (TRUE == ca_finger_data.wShowStatus)
		{
			ca_finger_data.wShowStatus = 0;
			qilian_cas_finger_show(0,DVB_CA_FINGER_FLAG_HIDE,NULL); 				
		}
	}
	else if (1 == ca_finger_data.wStatus)
	{
		/*��ʾ����*/
		if (FALSE == ca_finger_data.wShowStatus)
		{
			sprintf((char*)chFingerBuf, "%s", ca_finger_data.data);
			qilian_cas_finger_show(0,DVB_CA_FINGER_FLAG_SHOW,(char*)chFingerBuf); 				
			ca_finger_data.wShowStatus = TRUE;
		}
	}

#endif
	return 1;
}






