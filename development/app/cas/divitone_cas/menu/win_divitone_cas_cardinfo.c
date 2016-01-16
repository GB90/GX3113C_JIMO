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
#include "app_divitone_cas_api_cardinfo.h"


SIGNAL_HANDLER  int win_divitone_cas_cardinfo_create(const char* widgetname, void *usrdata)
{

	ca_get_date_t get_data = {0};
	char* str = NULL;
	
	get_data.date_type = DVB_CA_BASE_INFO;		
	get_data.ID = DIVI_BASEINFO_CARDID_ID;
	str = app_cas_api_get_data(&get_data);
	if (NULL !=str )
	{
            GUI_SetProperty("win_divitone_cas_card_id_value", "string",str);			
	}

	get_data.ID = DIVI_BASEINFO_CARDTYPE_ID;
	str = app_cas_api_get_data(&get_data);
	if (NULL !=str )
	{
            GUI_SetProperty("win_divitone_cas_cardtype_value", "string",str);			
	}

	get_data.ID = DIVI_BASEINFO_CARDSTATE_ID;
	str = app_cas_api_get_data(&get_data);
	if (NULL !=str )
	{
            GUI_SetProperty("win_divitone_cas_cardstate_value", "string",str);			
	}

	get_data.ID = DIVI_BASEINFO_REGION_ID;
	str = app_cas_api_get_data(&get_data);
	if (NULL !=str )
	{
            GUI_SetProperty("win_divitone_cas_cardarea_value", "string",str);			
	}

	get_data.ID = DIVI_BASEINFO_WORKTIME_ID;
	str = app_cas_api_get_data(&get_data);
	if (NULL !=str )
	{
            GUI_SetProperty("win_divitone_cas_worktime_value", "string",str);			
	}

	get_data.ID = DIVI_BASEINFO_WATCHLEVEL_ID;
	str = app_cas_api_get_data(&get_data);
	if (NULL !=str )
	{
            GUI_SetProperty("win_divitone_cas_watchlevel_value", "string",str);			
	}

	get_data.ID = DIVI_BASEINFO_PAIR_STATE;
	str = app_cas_api_get_data(&get_data);
	if (NULL !=str )
	{
            GUI_SetProperty("win_divitone_cas_pairstate_value", "string",str);			
	}

	get_data.ID = DIVI_BASEINFO_CAVERSION_ID;
	str = app_cas_api_get_data(&get_data);
	if (NULL !=str )
	{
            GUI_SetProperty("win_divitone_cas_caversion_value", "string",str);			
	}
	
	return 0;
}

SIGNAL_HANDLER  int win_divitone_cas_cardinfo_destroy(const char* widgetname, void *usrdata)
{
	return 0;
}

SIGNAL_HANDLER  int win_divitone_cas_cardinfo_keypress(const char* widgetname, void *usrdata)
{
	GUI_Event *event = NULL;
	event = (GUI_Event *)usrdata;
	if(event->type == GUI_KEYDOWN)
	{
		switch(event->key.sym)
		{
			case KEY_EXIT:
			case KEY_RECALL:
			case KEY_MENU:
				GUI_EndDialog("win_divitone_cas_cardinfo");
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

