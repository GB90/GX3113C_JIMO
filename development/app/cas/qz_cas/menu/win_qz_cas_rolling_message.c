/*****************************************************************************
* 						   CONFIDENTIAL
*        Hangzhou GuoXin Science and Technology Co., Ltd.
*                      (C)2012, All right reserved
******************************************************************************

******************************************************************************
* File Name :	win_qz_cas_rolling_message.c
* Author    : 	chenth
* Project   :	goxceed dvbc
* Type      :
******************************************************************************
* Purpose   :	ģ��ͷ�ļ�
******************************************************************************
* Release History:
VERSION	Date			  AUTHOR         Description
1.0  	2014.05.15		  chenth 	 			creation
*****************************************************************************/
#include "app_common_porting_stb_api.h"
#include "app_win_interface.h"
#include "app_qz_cas_api_rolling_message.h"
#include "app_qz_cas_api_pop.h"

void qz_cas_rolling_message_show(uint8_t byStyle,uint8_t showFlag,char* message)
{
	switch(byStyle)
	{
		case QZ_CA_OSD_TOP:
			if (DVB_CA_ROLLING_FLAG_HIDE == showFlag)
				{
					GUI_SetProperty("win_full_screen_text_roll_top","rolling_stop",NULL);							
					GUI_SetProperty("win_full_screen_text_roll_top","string", message);
					GUI_SetProperty("win_full_screen_text_roll_top","state","hide");
				}
			else
				if (DVB_CA_ROLLING_FLAG_SHOW == showFlag)
					{						
						GUI_SetProperty("win_full_screen_text_roll_top","string", message);
						GUI_SetProperty("win_full_screen_text_roll_top","state","show");
					}
			else
				if (DVB_CA_ROLLING_FLAG_RESET == showFlag)
					{//��λ������ʾ��ʵ�ʻ�δ��ʾ
						GUI_SetProperty("win_full_screen_text_roll_top","string","");
						GUI_SetInterface("flush", NULL);			
						GUI_SetProperty("win_full_screen_text_roll_top","rolling_stop",NULL);										
						GUI_SetProperty("win_full_screen_text_roll_top","reset_rolling","");
						GUI_SetProperty("win_full_screen_text_roll_top","string", message);	
						GUI_SetProperty("win_full_screen_text_roll_top","state","hide");
					}
			break;
		default:
			break;
	}
}
//��ȡ��������
int32_t qz_cas_rolling_message_get_times(uint8_t byStyle)
{
	int32_t wTimes=0;  

	switch(byStyle)
	{
		case QZ_CA_OSD_TOP:
			GUI_GetProperty("win_full_screen_text_roll_top","times",&wTimes);
			break;
		default:
			break;
	}

	return wTimes;
}

