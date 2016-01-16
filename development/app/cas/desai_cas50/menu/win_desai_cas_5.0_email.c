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
#include "app_common_play.h"
#include "app_common_porting_stb_api.h"
//#include "app_win_interface.h"
#include "win_desai_cas_5.0_email.h"
#include "app_common_porting_ca_flash.h"
#include "app_common_porting_ca_smart.h"
#include "app_common_porting_ca_demux.h"
#include "app_common_porting_ca_descramble.h"
#include "app_desai_cas_5.0_api_pop.h"
#include "desai_ca_api.h"
#include "app.h"

static desai_cas50_email_st ca_email_flag = {0};
int32_t app_desai_cas50_gxmsg_ca_on_event_email(GxMsgProperty0_OnEvent* event)
{
	desai_cas50_email_st* email_flag = NULL;
	uint8_t email_Flag = FALSE;

	char* focus_Window = (char*)GUI_GetFocusWindow();
	
	if (NULL == event)
		return 0;

	email_flag = (desai_cas50_email_st*)(event->buf);

	if (NULL == email_flag)
		return 0;

	if ((NULL != focus_Window)&&(strcasecmp("win_full_screen", focus_Window) != 0))
	{
		email_Flag = ca_email_flag.wShowStatus;
		memcpy(&ca_email_flag,email_flag,sizeof(desai_cas50_email_st));
		ca_email_flag.wShowStatus = email_Flag;
		return 1;
	}


	if ((0 != ca_email_flag.wStatus)&&(TRUE == ca_email_flag.wShowStatus))
	{
		/*
		* 关闭之前显示
		*/
		GUI_SetProperty("full_screen_img_mail","state","hide");
		ca_email_flag.wStatus = 0;
		ca_email_flag.wShowStatus = 0;
		ca_email_flag.uCount = 0;
	}

	memcpy(&ca_email_flag,email_flag,sizeof(desai_cas50_email_st));


	
	return 1;
}

/*
*邮件图标显示、隐藏
*/
int32_t app_desai_cas50_email_flag_exec()
{
	uint8_t detitle_status = ca_email_flag.wStatus;

	char* focus_Window = (char*)GUI_GetFocusWindow();
	if ((NULL != focus_Window)&&(strcasecmp("win_full_screen", focus_Window) != 0))
	{	
		return 1;
	}
	
	if ( MESSAGE_NEWEMAIL_TYPE == detitle_status)
	{	
		if (FALSE == ca_email_flag.wShowStatus)
		{
			GUI_SetProperty("full_screen_img_mail","img","MAIL_200ff.bmp");
			GUI_SetProperty("full_screen_img_mail","state","show");
		}
		ca_email_flag.wShowStatus = TRUE;
	}
	else
		if ( MESSAGE_OVEREMAIL_TYPE == detitle_status)
		{
			ca_email_flag.uCount++;
			ca_email_flag.uCount = ca_email_flag.uCount%30;

			if (0 != ca_email_flag.uCount)
			{
				GUI_SetProperty("full_screen_img_mail","img","mail_full.bmp");
				GUI_SetProperty("full_screen_img_mail","state","show");	
				ca_email_flag.wShowStatus = TRUE;
			}
			else
			{
				GUI_SetProperty("full_screen_img_mail","state","osd_trans_hide");
				ca_email_flag.wShowStatus = FALSE;
			}
			
			GUI_SetProperty("full_screen_img_mail", "draw_now", NULL);
			
		}
		else
			if (MESSAGE_HIDE_NEWEMAIL_TYPE == detitle_status)
			{	
				/* 要求隐藏邮件图标 */
				if (TRUE == ca_email_flag.wShowStatus)
				{
					GUI_SetProperty("full_screen_img_mail","state","hide");
				}

				ca_email_flag.wShowStatus = FALSE;
			}
		return 1;
	
}




