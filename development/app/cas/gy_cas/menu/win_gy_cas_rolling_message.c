/*****************************************************************************
* 						   CONFIDENTIAL
*        Hangzhou GuoXin Science and Technology Co., Ltd.
*                      (C)2012, All right reserved
******************************************************************************

******************************************************************************
* File Name :	app_ca_api.c
* Author    : 	bingyu
* Project   :	goxceed dvbc
* Type      :
******************************************************************************
* Purpose   :	模块头文件
******************************************************************************
* Release History:
VERSION	Date			  AUTHOR         Description
1.0  	2012.12.02		  bingyu 	 			creation
*****************************************************************************/
#include "app_common_porting_stb_api.h"
#include "app_win_interface.h"
#include "app_gy_cas_api_rolling_message.h"
#include "app_gy_cas_api_pop.h"

void gy_cas_rolling_message_show(uint8_t byStyle,uint8_t showFlag,char* message)
{

	CAS_Dbg("%s>>style=%d,showFlag=%d,memssage=%s\n",__FUNCTION__,byStyle,showFlag,message);
	switch(byStyle)
	{
		case GYCA_OSD_TOP:
			if (DVB_CA_ROLLING_FLAG_HIDE == showFlag)
			{	
				GUI_SetProperty("win_full_screen_text_roll_top","rolling_stop",NULL);	
				GUI_SetProperty("win_full_screen_text_roll_top","string", message);
				GUI_SetProperty("win_full_screen_text_roll_top","state","hide");
				//GUI_SetProperty("win_full_screen_text_roll_top_back","state","hide");
			}
			else if (DVB_CA_ROLLING_FLAG_SHOW == showFlag)
			{
				//GUI_SetProperty("win_full_screen_text_roll_top_back","state","show");
				GUI_SetProperty("win_full_screen_text_roll_top","state","show");
				GUI_SetInterface("flush", NULL);
			}
			else if (DVB_CA_ROLLING_FLAG_RESET == showFlag)
			{
				GUI_SetProperty("win_full_screen_text_roll_top","string","");
				GUI_SetInterface("flush", NULL);			
				GUI_SetProperty("win_full_screen_text_roll_top","rolling_stop",NULL);
				GUI_SetProperty("win_full_screen_text_roll_top","string", message);
				//GUI_SetProperty("win_full_screen_text_roll_top_back","state","show");
				GUI_SetProperty("win_full_screen_text_roll_top","reset_rolling", "");
				GUI_SetProperty("win_full_screen_text_roll_top","state","hide");
			}
			break;
		case GYCA_OSD_BOTTOM:
			if (DVB_CA_ROLLING_FLAG_HIDE == showFlag)
			{	
				GUI_SetProperty("win_full_screen_text_roll_bottom","rolling_stop",NULL);							
				GUI_SetProperty("win_full_screen_text_roll_bottom","string", message);			
				GUI_SetProperty("win_full_screen_text_roll_bottom","state","hide");
				//GUI_SetProperty("win_full_screen_text_roll_bottom_back","state","hide");
			}
			else if (DVB_CA_ROLLING_FLAG_SHOW == showFlag)
			{
				//GUI_SetProperty("win_full_screen_text_roll_bottom_back","state","show");
				GUI_SetProperty("win_full_screen_text_roll_bottom","state","show");
				GUI_SetInterface("flush", NULL);
			}
			else if (DVB_CA_ROLLING_FLAG_RESET == showFlag)
			{
				GUI_SetProperty("win_full_screen_text_roll_bottom","string","");
				GUI_SetInterface("flush", NULL);			
				GUI_SetProperty("win_full_screen_text_roll_bottom","rolling_stop",NULL);				
				GUI_SetProperty("win_full_screen_text_roll_bottom","string", message);
				//GUI_SetProperty("win_full_screen_text_roll_bottom_back","state","show");
				GUI_SetProperty("win_full_screen_text_roll_bottom","reset_rolling", "");
				GUI_SetProperty("win_full_screen_text_roll_bottom","state","hide"); 												
			}	
			break;
		default:
			break;
	}
	CAS_Dbg("%s %d>>exit\n",__FUNCTION__,__LINE__);
}





