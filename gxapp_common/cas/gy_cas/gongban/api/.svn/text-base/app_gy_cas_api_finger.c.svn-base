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
#include "app_gy_cas_api_pop.h"
#include "app_gy_cas_api_finger.h"
#include "gui_core.h"
#include "app_common_porting_ca_os.h"

extern 	void gy_cas_finger_show(uint8_t number,uint8_t showFlag,char* message);
static handle_t gy_ca_finger_sem = 0;

static  uint8_t fullscreenFlag = FALSE;
static gy_cas_finger_data_st ca_finger_data = {0};
int32_t app_gy_cas_finger_init(void)
{
	app_porting_ca_os_sem_create(&gy_ca_finger_sem,1);
	memset(&ca_finger_data,0,sizeof(gy_cas_finger_data_st));
	return 0;
}
int32_t app_gy_cas_gxmsg_ca_on_event_finger(GxMsgProperty0_OnEvent* event)
{
	gy_cas_finger_data_st* finger_data = NULL;
	char* focus_Window = (char*)GUI_GetFocusWindow();
	if (NULL == event)
		return 0;
	if ((NULL != focus_Window)&&(strcasecmp("win_full_screen", focus_Window) != 0))
		return 0;
	
	finger_data = (gy_cas_finger_data_st*)(event->buf);

	if (NULL == finger_data)
		return 0;
	app_porting_ca_os_sem_wait(gy_ca_finger_sem);
	
	ca_finger_data.wStatus = finger_data->wStatus;
#if 0
	if ((0 != ca_finger_data.wStatus)&&(TRUE == ca_finger_data.wShowStatus))
	{
		if(!memcmp(ca_finger_data.str,finger_data->str,sizeof(finger_data->str)))
		{
			app_porting_ca_os_sem_signal(gy_ca_finger_sem);
			return 1;
		}

	}
#endif
	memcpy(ca_finger_data.str,finger_data->str,sizeof(finger_data->str));
	gy_cas_finger_show(ca_finger_data.position,DVB_CA_FINGER_FLAG_HIDE,NULL);
	ca_finger_data.wShowStatus = FALSE;

	app_porting_ca_os_sem_signal(gy_ca_finger_sem);
	
	return 1;
}

int32_t app_gy_cas_finger_exec(void)
{
	uint8_t chFingerBuf[20]={0};	
	char* focus_Window = (char*)GUI_GetFocusWindow();

	app_porting_ca_os_sem_wait(gy_ca_finger_sem);

	if (TRUE == ca_finger_data.wStatus  )	
	{
		if (FALSE == ca_finger_data.wShowStatus)
		{
			focus_Window = (char*)GUI_GetFocusWindow();
			if ((NULL != focus_Window)&&(strcasecmp("win_full_screen", focus_Window) == 0))
			{
				ca_finger_data.position = 10;//rand()%16;
				fullscreenFlag = TRUE;
				
			}
			else
			{
				ca_finger_data.position = 10;//rand()%3;
				fullscreenFlag = FALSE;
				
			}
			sprintf((char*)chFingerBuf, "%s", ca_finger_data.str);
			gy_cas_finger_show(ca_finger_data.position,DVB_CA_FINGER_FLAG_SHOW,(char*)chFingerBuf);

			ca_finger_data.wShowStatus = TRUE;				
		}
		else
		{
			if (ca_finger_data.position  >= 3)
			{
				if (TRUE == fullscreenFlag)
				{
					focus_Window = (char*)GUI_GetFocusWindow();
					if ((NULL != focus_Window)&&(strcasecmp("win_full_screen", focus_Window) == 0))
					{
						fullscreenFlag = TRUE;
					}
					else
					{
						/*
						* 由全屏状态进入菜单界面，指纹显示在上端，避免界面错乱
						*/
						fullscreenFlag = FALSE;
						gy_cas_finger_show(ca_finger_data.position,DVB_CA_FINGER_FLAG_HIDE,NULL);											
						ca_finger_data.position= 10;//rand()%3;
						sprintf((char*)chFingerBuf, "%s", ca_finger_data.str);
						gy_cas_finger_show(ca_finger_data.position,DVB_CA_FINGER_FLAG_SHOW,(char*)chFingerBuf);														
					}
				}							
			}

		}
		
	}	
	else
	{
		if (TRUE == ca_finger_data.wShowStatus)
		{
			memset(ca_finger_data.str,0,sizeof(ca_finger_data.str));
			gy_cas_finger_show(ca_finger_data.position,DVB_CA_FINGER_FLAG_HIDE,NULL);
			ca_finger_data.wShowStatus = FALSE;
		}
	}
	
	app_porting_ca_os_sem_signal(gy_ca_finger_sem);
	return TRUE;
}






