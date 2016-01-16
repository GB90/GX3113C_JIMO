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
#include "app_by_cas_api_baseinfo.h"


SIGNAL_HANDLER  int win_by_cas_baseinfo_create(const char* widgetname, void *usrdata)
{

	ca_get_date_t get_data = {0};
	char* str = NULL;
	
	get_data.date_type = DVB_CA_BASE_INFO;		
	get_data.ID = BY_BASEINFO_VERSION_ID;
	str = app_cas_api_get_data(&get_data);
	if (NULL !=str )
		{
                GUI_SetProperty("win_by_cas_baseinfo_version", "string",str);			
		}

	get_data.ID = BY_BASEINFO_SYS_SP_ID;
	str = app_cas_api_get_data(&get_data);
	if (NULL !=str )
		{
                GUI_SetProperty("win_by_cas_baseinfo_sysspid", "string",str);			
		}

	get_data.ID = BY_BASEINFO_SYS_CT_ID;
	str = app_cas_api_get_data(&get_data);
	if (NULL !=str )
		{
                GUI_SetProperty("win_by_cas_baseinfo_sysctid", "string",str);			
		}

	get_data.ID = BY_BASEINFO_CARDID_ID;
	str = app_cas_api_get_data(&get_data);
	if (NULL !=str )
		{
                GUI_SetProperty("win_by_cas_baseinfo_cardid", "string",str);			
		}

	
	return 0;
}

SIGNAL_HANDLER  int win_by_cas_baseinfo_destroy(const char* widgetname, void *usrdata)
{
	return 0;
}

SIGNAL_HANDLER  int win_by_cas_baseinfo_keypress(const char* widgetname, void *usrdata)
{
	GUI_Event *event = NULL;
	event = (GUI_Event *)usrdata;
	if(event->type == GUI_KEYDOWN)
	{
		switch(event->key.sym)
		{
			case KEY_EXIT:
				break;
				return EVENT_TRANSFER_STOP;
			case KEY_RECALL:
				GUI_EndDialog("win_by_cas_baseinfo");
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

