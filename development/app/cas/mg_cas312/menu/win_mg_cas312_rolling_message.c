/*****************************************************************************
* 						   CONFIDENTIAL
*        Hangzhou GuoXin Science and Technology Co., Ltd.
*                      (C)2012, All right reserved
******************************************************************************

******************************************************************************
* File Name :	win_mg_cas_rolling_message.c
* Author    : 	zhouhm
* Project   :	goxceed dvbc
* Type      :
******************************************************************************
* Purpose   :	模块头文件
******************************************************************************
* Release History:
VERSION	Date			  AUTHOR         Description
1.0  	2014.04.16		  zhouhm 	 			creation
*****************************************************************************/
#include "app_common_porting_stb_api.h"
#include "app_win_interface.h"
#include "app_mg_cas312_api_rolling_message.h"
#include "app_mg_cas312_api_pop.h"


void mg_cas312_rolling_message_show(uint8_t byStyle, uint8_t showFlag, char* message)
{
	switch (byStyle)
	{
		case MG312_CA_OSD_TOP:
			if (DVB_CA_ROLLING_FLAG_HIDE == showFlag)
			{
				GUI_SetProperty("win_full_screen_text_roll_top","rolling_stop",NULL);							
				GUI_SetProperty("win_full_screen_text_roll_top","string", message);
				GUI_SetProperty("win_full_screen_text_roll_top","state","hide");
			}
			else if (DVB_CA_ROLLING_FLAG_SHOW == showFlag)
			{						
				GUI_SetProperty("win_full_screen_text_roll_top","string", message);
				GUI_SetProperty("win_full_screen_text_roll_top","state","show");
			}
			else if (DVB_CA_ROLLING_FLAG_RESET == showFlag)
			{
				GUI_SetProperty("win_full_screen_text_roll_top","string","");
				GUI_SetInterface("flush", NULL);			
				GUI_SetProperty("win_full_screen_text_roll_top","rolling_stop",NULL);										
				GUI_SetProperty("win_full_screen_text_roll_top","reset_rolling","");
				GUI_SetProperty("win_full_screen_text_roll_top","string", message);	
				GUI_SetProperty("win_full_screen_text_roll_top","state","hide");
			}
			break;
			
		case MG312_CA_OSD_BOTTOM:
			if (DVB_CA_ROLLING_FLAG_HIDE == showFlag)
			{
				GUI_SetProperty("win_full_screen_text_roll_bottom","rolling_stop",NULL);							
				GUI_SetProperty("win_full_screen_text_roll_bottom","string", message);
				GUI_SetProperty("win_full_screen_text_roll_bottom","state","hide");
			}
			else if (DVB_CA_ROLLING_FLAG_SHOW == showFlag)
			{
				GUI_SetProperty("win_full_screen_text_roll_bottom","string", message);	
				GUI_SetProperty("win_full_screen_text_roll_bottom","state","show");
			}
			else if (DVB_CA_ROLLING_FLAG_RESET == showFlag)
			{
				GUI_SetProperty("win_full_screen_text_roll_bottom","string","");
				GUI_SetInterface("flush", NULL);			
				GUI_SetProperty("win_full_screen_text_roll_bottom","rolling_stop",NULL);						
				GUI_SetProperty("win_full_screen_text_roll_bottom","reset_rolling","");
				GUI_SetProperty("win_full_screen_text_roll_bottom","string", message);			
				GUI_SetProperty("win_full_screen_text_roll_bottom","state","hide");	
			}				
			break;
			
		default:
			break;
	}

	return;
}

int32_t mg_cas312_rolling_message_get_times(uint8_t byStyle)
{
	int32_t wTimes = 0;  

	switch (byStyle)
	{
		case MG312_CA_OSD_TOP:
			GUI_GetProperty("win_full_screen_text_roll_top","times",&wTimes);
			break;
			
		case MG312_CA_OSD_BOTTOM:
			GUI_GetProperty("win_full_screen_text_roll_bottom","times",&wTimes);			
			break;
			
		default:
			break;
	}

	return wTimes;
}


