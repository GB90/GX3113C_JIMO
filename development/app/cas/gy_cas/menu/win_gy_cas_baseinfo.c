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
#include "app_gy_cas_api_baseinfo.h"


SIGNAL_HANDLER  int win_gy_cas_baseinfo_create(const char* widgetname, void *usrdata)
{

	ca_get_date_t get_data = {0};
	char* str = NULL;
	
	get_data.date_type = DVB_CA_BASE_INFO;	
	get_data.ID = GY_BASEINFO_OPERATE_ID;
	str = app_cas_api_get_data(&get_data);
	if (NULL !=str )
		{
                GUI_SetProperty("win_gy_cas_baseinfo_operator", "string",str);			
		}

	get_data.ID = GY_BASEINFO_USER_TYPE_ID;
	str = app_cas_api_get_data(&get_data);
	if (NULL !=str )
		{
                GUI_SetProperty("win_gy_cas_baseinfo_usertype", "string",str);			
		}

	get_data.ID = GY_BASEINFO_CARD_STATUS_ID;
	str = app_cas_api_get_data(&get_data);
	if (NULL !=str )
		{
                GUI_SetProperty("win_gy_cas_baseinfo_cardstatus", "string",str);			
		}
	
	get_data.ID = GY_BASEINFO_AREACORD_ID;
	str = app_cas_api_get_data(&get_data);
	if (NULL !=str )
		{
                GUI_SetProperty("win_cd_cas30_baseinfo_areacode", "string",str);			
		}

	get_data.ID = GY_BASEINFO_CARD_TYPE_ID;
	str = app_cas_api_get_data(&get_data);
	if (NULL !=str )
		{
                GUI_SetProperty("win_gy_cas_baseinfo_cardtype", "string",str);			
		}

	get_data.ID = GY_BASEINFO_CARD_ISSUE_TIMER_ID;
	str = app_cas_api_get_data(&get_data);
	if (NULL !=str )
		{
                GUI_SetProperty("win_gy_cas_baseinfo_starttime", "string",str);			
		}
	
	get_data.ID = GY_BASEINFO_CARD_EXPIRE_TIMER_ID;
	str = app_cas_api_get_data(&get_data);
	if (NULL !=str )
		{
                GUI_SetProperty("win_gy_cas_baseinfo_endtime", "string",str);			
		}

	get_data.ID = GY_BASEINFO_CARD_SN_ID;
		str = app_cas_api_get_data(&get_data);
		if (NULL !=str )
			{
					GUI_SetProperty("win_gy_cas_baseinfo_serialnum", "string",str); 		
			}

	get_data.ID = GY_BASEINFO_CARD_MONEY_ID;
	str = app_cas_api_get_data(&get_data);
	if (NULL !=str )
		{
                GUI_SetProperty("win_gy_cas_baseinfo_money", "string",str);			
		}

	get_data.ID = GY_BASEINFO_VERSION_ID;
	str = app_cas_api_get_data(&get_data);
	if (NULL !=str )
		{
                GUI_SetProperty("win_gy_cas_baseinfo_casver", "string",str);			
		}
	return 0;
}

SIGNAL_HANDLER  int win_gy_cas_baseinfo_destroy(const char* widgetname, void *usrdata)
{
	return 0;
}

SIGNAL_HANDLER  int win_gy_cas_baseinfo_keypress(const char* widgetname, void *usrdata)
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
				GUI_EndDialog("win_gy_cas_baseinfo");
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

