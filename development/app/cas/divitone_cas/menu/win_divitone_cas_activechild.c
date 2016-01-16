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
#include "DiviGuardCa.h"

void app_divitone_cas_activechild_show(uint8_t showFlag,char* message)
{	
	if (0 == showFlag)
		{	
			GUI_SetProperty("win_divitone_cas_activechild_text","state","hide");
		}
	else
		if (1 == showFlag)
			{
				GUI_SetProperty("win_divitone_cas_activechild_text", "string", message);
				GUI_SetProperty("win_divitone_cas_activechild_text","state","show");
			}
	return;		
}

SIGNAL_HANDLER  int win_divitone_cas_activechild_create(const char* widgetname, void *usrdata)
{
	DIVI_Active_ChildCard();  
	app_divitone_cas_activechild_show(1,"Child CARD is activing");
	return 0;
}

SIGNAL_HANDLER  int win_divitone_cas_activechild_destroy(const char* widgetname, void *usrdata)
{
	return 0;
}

SIGNAL_HANDLER  int win_divitone_cas_activechild_keypress(const char* widgetname, void *usrdata)
{
	GUI_Event *event = NULL;
	event = (GUI_Event *)usrdata;
	if(event->type == GUI_KEYDOWN)
	{
		switch(event->key.sym)
		{
			case KEY_EXIT:
			case KEY_MENU:
				DIVI_Finish_ChildCard();
				GUI_EndDialog("win_divitone_cas_activechild");				
				break;
				return EVENT_TRANSFER_STOP;
			case KEY_RECALL:
				DIVI_Finish_ChildCard();
				GUI_EndDialog("win_divitone_cas_activechild");
				return EVENT_TRANSFER_STOP;
			case KEY_OK:
				return EVENT_TRANSFER_STOP;
			case KEY_POWER:
				return EVENT_TRANSFER_KEEPON;
			default:
				return EVENT_TRANSFER_STOP;
		}
	}

	return EVENT_TRANSFER_KEEPON;
}



