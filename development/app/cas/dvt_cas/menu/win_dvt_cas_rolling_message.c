/*****************************************************************************
* 						   CONFIDENTIAL
*        Hangzhou GuoXin Science and Technology Co., Ltd.
*                      (C)2012, All right reserved
******************************************************************************

******************************************************************************
* File Name :	win_dvt_cas_rolling_message.c
* Author    : 	zhouhm
* Project   :	goxceed dvbc
* Type      :
******************************************************************************
* Purpose   :	模块头文件
******************************************************************************
* Release History:
VERSION	Date			  AUTHOR         Description
1.0  	2013.12.02		  zhouhm 	 			creation
*****************************************************************************/
#include "app_common_porting_stb_api.h"
#include "app_win_interface.h"
#include "app_dvt_cas_api_rolling_message.h"
#include "app_dvt_cas_api_pop.h"

void dvt_cas_rolling_message_show(uint8_t byStyle,uint8_t showFlag,char* message)
{
	if (DVB_CA_ROLLING_FLAG_HIDE == showFlag)
		{	
			GUI_SetProperty("win_full_screen_text_roll_bottom","state","hide");
		}
	else
		if (DVB_CA_ROLLING_FLAG_SHOW == showFlag)
			{
				printf("%s %s show \n",__FILE__,__func__);
				GUI_SetProperty("win_full_screen_text_roll_bottom","state","show");
				printf("%s %s message=%s \n",__FILE__,__func__,message);
				GUI_SetProperty("win_full_screen_text_roll_bottom","string", message);
			}
	else
		if (DVB_CA_ROLLING_FLAG_RESET == showFlag)
			{
				GUI_SetProperty("win_full_screen_text_roll_bottom","rolling_stop","");							
				GUI_SetProperty("win_full_screen_text_roll_bottom","reset_rolling","");
				GUI_SetProperty("win_full_screen_text_roll_bottom","string", message);			
				GUI_SetProperty("win_full_screen_text_roll_bottom","state","hide");													
			}
	return;

}




