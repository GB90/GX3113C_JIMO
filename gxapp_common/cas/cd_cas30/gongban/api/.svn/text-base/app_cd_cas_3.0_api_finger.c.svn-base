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
* Purpose   :	模块头文件
******************************************************************************
* Release History:
VERSION	Date			  AUTHOR         Description
1.0  	2012.12.02		  zhouhm 	 			creation
*****************************************************************************/
#include "app_common_porting_stb_api.h"
#include "app_cd_cas_3.0_api_pop.h"
#include "app_cd_cas_3.0_api_finger.h"
#include "gui_core.h"

extern 	void cd_cas30_finger_show(uint8_t number,uint8_t showFlag,char* message);

static cd_cas30_finger_data_st ca_finger_data = {0};
int32_t app_cd_cas30_gxmsg_ca_on_event_finger(GxMsgProperty0_OnEvent* event)
{
	cd_cas30_finger_data_st* finger_data = NULL;
	if (NULL == event)
		return 0;

	finger_data = (cd_cas30_finger_data_st*)(event->buf);

	if (NULL == finger_data)
		return 0;

	ca_finger_data.dwCardID = finger_data->dwCardID;
	ca_finger_data.wStatus = finger_data->wStatus;
	if(ca_finger_data.wStatus)
	{
		ca_finger_data.wShowStatus = FALSE;
	}
	else
	{
		ca_finger_data.wShowStatus = TRUE;
	}

	return 1;
}

int32_t app_cd_cas30_finger_exec(void)
{
	uint8_t chFingerBuf[20]={0};	
	char* focus_Window = (char*)GUI_GetFocusWindow();

	if((strcasecmp("win_full_screen", focus_Window)  != 0)&&(strcasecmp("win_cd_cas30_ippv_buy", focus_Window)  != 0))
	{
		return TRUE;
	}

// 指纹显示
	if (TRUE == ca_finger_data.wStatus  )	
	{
		if (FALSE == ca_finger_data.wShowStatus)
		{
			if ((NULL != focus_Window)&&(strcasecmp("win_full_screen", focus_Window) == 0))
			{
				ca_finger_data.position = rand()%16;
				
			}
			else
			{
				ca_finger_data.position = rand()%8;
				
			}			
			cd_cas30_finger_show(ca_finger_data.position,DVB_CA_FINGER_FLAG_HIDE,(char*)NULL);
			sprintf((char*)chFingerBuf, "%u", ca_finger_data.dwCardID);
			cd_cas30_finger_show(ca_finger_data.position,DVB_CA_FINGER_FLAG_SHOW,(char*)chFingerBuf);
			ca_finger_data.wShowStatus = TRUE;				
		}
	}	
	else
	{
		if (TRUE == ca_finger_data.wShowStatus)
		{
			if ((NULL != focus_Window)&&(strcasecmp("win_full_screen", focus_Window) == 0))
			{
				ca_finger_data.position = rand()%16;
			}
			else
			{
				ca_finger_data.position = rand()%8;
			}
			cd_cas30_finger_show(ca_finger_data.position,DVB_CA_FINGER_FLAG_HIDE,(char*)NULL);
			ca_finger_data.wShowStatus = FALSE;				
		}
	}
	return TRUE;
}






