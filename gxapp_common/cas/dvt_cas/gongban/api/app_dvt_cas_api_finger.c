/*****************************************************************************
* 						   CONFIDENTIAL
*        Hangzhou GuoXin Science and Technology Co., Ltd.
*                      (C)2012, All right reserved
******************************************************************************

******************************************************************************
* File Name :	app_dvt_cas_api_finger.c
* Author    : 	zhouhm
* Project   :	goxceed dvbc
* Type      :
******************************************************************************
* Purpose   :	模块头文件
******************************************************************************
* Release History:
VERSION	Date			  AUTHOR         Description
1.0  	2013.11.29		  zhouhm 	 			creation
*****************************************************************************/
#include "app_common_porting_stb_api.h"
#include "app_dvt_cas_api_pop.h"
#include "app_dvt_cas_api_finger.h"
#include "gui_core.h"


extern 	void dvt_cas_finger_show(uint8_t number,uint8_t showFlag,char* message);

static  uint8_t fullscreenFlag = FALSE;
static dvt_cas_finger_data_st ca_finger_data = {0};
int32_t app_dvt_cas_gxmsg_ca_on_event_finger(GxMsgProperty0_OnEvent* event)
{
	dvt_cas_finger_data_st* finger_data = NULL;
	if (NULL == event)
		return 0;

	finger_data = (dvt_cas_finger_data_st*)(event->buf);

	if (NULL == finger_data)
		return 0;

	if ((0 != ca_finger_data.wStatus)&&(TRUE == ca_finger_data.wShowStatus))
	{
		/*
		* 关闭上一个指纹显示
		*/

		if (ca_finger_data.position >= 3)
			{
				dvt_cas_finger_show(ca_finger_data.position,DVB_CA_FINGER_FLAG_HIDE,NULL);
			}
		else
			{
				dvt_cas_finger_show(ca_finger_data.position,DVB_CA_FINGER_FLAG_TRANS_HIDE,NULL);
			}
		ca_finger_data.wStatus = 0;
		ca_finger_data.wShowStatus = 0;
		ca_finger_data.wDuration = 0;
		ca_finger_data.wusedTime = 0;

	}

	memcpy(&ca_finger_data,finger_data,sizeof(dvt_cas_finger_data_st));


	
	return 1;
}

int32_t app_dvt_cas_finger_exec(void)
{
	uint8_t chFingerBuf[20]={0};	
//	char* focus_Window = (char*)GUI_GetFocusWindow();



// 指纹显示
	if (TRUE == ca_finger_data.wStatus  )	
	{
			if (FALSE == ca_finger_data.wShowStatus)
			{
//				focus_Window = (char*)GUI_GetFocusWindow();

				ca_finger_data.position = rand()%3;
				fullscreenFlag = FALSE;
						
				
					sprintf((char*)chFingerBuf, "%u", ca_finger_data.dwCardID);
					dvt_cas_finger_show(ca_finger_data.position,DVB_CA_FINGER_FLAG_SHOW,(char*)chFingerBuf);

				ca_finger_data.wShowStatus = TRUE;				
			}
			else
				{
					if (ca_finger_data.wusedTime <
						ca_finger_data.wDuration*1000)
						{
							ca_finger_data.wusedTime+=200;	
						}
					else
						{
							dvt_cas_finger_show(ca_finger_data.position,DVB_CA_FINGER_FLAG_HIDE,NULL);
							ca_finger_data.wStatus = 0;
							ca_finger_data.wShowStatus = 0;
							ca_finger_data.wDuration = 0;
							ca_finger_data.wusedTime = 0;
						}
				}
		
	}	
	return TRUE;
}






