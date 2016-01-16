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
#include "app_win_interface.h"
#include "app_cd_cas_3.0_api_rolling_message.h"
#include "app_cd_cas_3.0_api_pop.h"

static void set_osd_showing_speed(char *text_handle)
{ 
	int ucOsdTempo = 1;
	char buf[5];
	sprintf((void*)buf,"%d",ucOsdTempo);
	GUI_SetProperty(text_handle,"step",buf);
	return;
}

void cd_cas30_rolling_message_show(uint8_t byStyle,uint8_t showFlag,char* message)
{
	switch(byStyle)
		{
			case CDCA_OSD_TOP:
				if (DVB_CA_ROLLING_FLAG_HIDE == showFlag)
				{	
					GUI_SetProperty("win_full_screen_text_roll_top","rolling_stop",NULL);	
					GUI_SetProperty("win_full_screen_text_roll_top","string", message);
					GUI_SetProperty("win_full_screen_text_roll_top","state","hide");
					GUI_SetProperty("win_full_screen_text_roll_top_back","state","hide");
				}
				else if (DVB_CA_ROLLING_FLAG_SHOW == showFlag)
				{
					GUI_SetProperty("win_full_screen_text_roll_top","string", message);
					GUI_SetProperty("win_full_screen_text_roll_top","state","show");
					GUI_SetProperty("win_full_screen_text_roll_top_back","state","show");
				}
				else if (DVB_CA_ROLLING_FLAG_RESET == showFlag)
				{
					GUI_SetProperty("win_full_screen_text_roll_top","rolling_stop",NULL);	
	set_osd_showing_speed("win_full_screen_text_roll_top");
					GUI_SetProperty("win_full_screen_text_roll_top","string", message);
					GUI_SetProperty("win_full_screen_text_roll_top","state","hide");
					GUI_SetProperty("win_full_screen_text_roll_top_back","state","hide");
				}

				break;
			case CDCA_OSD_BOTTOM:
				if (DVB_CA_ROLLING_FLAG_HIDE == showFlag)
				{	
					GUI_SetProperty("win_full_screen_text_roll_bottom","rolling_stop",NULL);							
					GUI_SetProperty("win_full_screen_text_roll_bottom","string", " ");			
					GUI_SetProperty("win_full_screen_text_roll_bottom","state","hide");
					GUI_SetProperty("win_full_screen_text_roll_bottom_back","state","hide");
				}
				else if (DVB_CA_ROLLING_FLAG_SHOW == showFlag)
				{
					GUI_SetProperty("win_full_screen_text_roll_bottom","string", message);	
					GUI_SetProperty("win_full_screen_text_roll_bottom","state","show");
					GUI_SetProperty("win_full_screen_text_roll_bottom_back","state","show");
				}
				else if (DVB_CA_ROLLING_FLAG_RESET == showFlag)
				{
					GUI_SetProperty("win_full_screen_text_roll_bottom","rolling_stop",NULL);							
					set_osd_showing_speed("win_full_screen_text_roll_bottom");
					GUI_SetProperty("win_full_screen_text_roll_bottom","string", " ");			
					GUI_SetProperty("win_full_screen_text_roll_bottom","state","hide");													
					GUI_SetProperty("win_full_screen_text_roll_bottom_back","state","hide");
				}	
				break;
			default:
				break;
		}
}




