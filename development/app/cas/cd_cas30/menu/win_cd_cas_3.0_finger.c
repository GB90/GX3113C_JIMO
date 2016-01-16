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
#include "app_win_interface.h"
#include "win_cd_cas_3.0_finger.h"
#include "app_cd_cas_3.0_api_pop.h"

void cd_cas30_finger_show(uint8_t number,uint8_t showFlag,char* message)
{
	char	 sTextBuffer[40]={0};
	char* focus_Window = (char*)GUI_GetFocusWindow();

	if((strcasecmp("win_full_screen", focus_Window)  != 0)&&(strcasecmp("win_cd_cas30_ippv_buy", focus_Window)  != 0))
	{
		return ;
	}
	
	if (DVB_CA_FINGER_FLAG_HIDE == showFlag)
	{	
		if((focus_Window)&&(strcasecmp("win_full_screen", focus_Window)  == 0))
		{
			GUI_SetProperty("win_full_screen_text_finger0","state","hide");
			GUI_SetProperty("win_full_screen_text_finger1","state","hide");
			GUI_SetProperty("win_full_screen_text_finger2","state","hide");
			GUI_SetProperty("win_full_screen_text_finger3","state","hide");
			GUI_SetProperty("win_full_screen_text_finger4","state","hide");
			GUI_SetProperty("win_full_screen_text_finger5","state","hide");
			GUI_SetProperty("win_full_screen_text_finger6","state","hide");
			GUI_SetProperty("win_full_screen_text_finger7","state","hide");
			GUI_SetProperty("win_full_screen_text_finger8","state","hide");
			GUI_SetProperty("win_full_screen_text_finger9","state","hide");
			GUI_SetProperty("win_full_screen_text_finger10","state","hide");
			GUI_SetProperty("win_full_screen_text_finger11","state","hide");
			GUI_SetProperty("win_full_screen_text_finger12","state","hide");
			GUI_SetProperty("win_full_screen_text_finger13","state","hide");
			GUI_SetProperty("win_full_screen_text_finger14","state","hide");
			GUI_SetProperty("win_full_screen_text_finger15","state","hide");
		}
		if((focus_Window)&&(strcasecmp("win_cd_cas30_ippv_buy", focus_Window)  == 0))
		{
			GUI_SetProperty("win_cd_cas30_ippv_buy_text_finger0","state","hide");
			GUI_SetProperty("win_cd_cas30_ippv_buy_text_finger1","state","hide");
			GUI_SetProperty("win_cd_cas30_ippv_buy_text_finger2","state","hide");
			GUI_SetProperty("win_cd_cas30_ippv_buy_text_finger3","state","hide");
			GUI_SetProperty("win_cd_cas30_ippv_buy_text_finger4","state","hide");
			GUI_SetProperty("win_cd_cas30_ippv_buy_text_finger5","state","hide");
			GUI_SetProperty("win_cd_cas30_ippv_buy_text_finger6","state","hide");
			GUI_SetProperty("win_cd_cas30_ippv_buy_text_finger7","state","hide");
		}
	}
	else if (DVB_CA_FINGER_FLAG_TRANS_HIDE == showFlag)
	{
		sprintf( sTextBuffer, "win_full_screen_text_finger%d", number);
		GUI_SetProperty(sTextBuffer,"state","osd_trans_hide");				
	}
	else if (DVB_CA_FINGER_FLAG_SHOW == showFlag)
	{
		if((focus_Window)&&(strcasecmp("win_cd_cas30_ippv_buy", focus_Window)  == 0))
		{
			sprintf( sTextBuffer, "win_cd_cas30_ippv_buy_text_finger%d", number);	
		}
		else
		{
			sprintf( sTextBuffer, "win_full_screen_text_finger%d", number); 
		}
		GUI_SetProperty(sTextBuffer,"string", message); 				
		GUI_SetProperty(sTextBuffer,"state","show");	
		if((focus_Window)&&(strcasecmp("win_cd_cas30_ippv_buy", focus_Window)  == 0))
		{
			GUI_SetProperty("win_cd_cas30_ippv_buy_box","update",NULL);	
		}
	}
	return;
}









