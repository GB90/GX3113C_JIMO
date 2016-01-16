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
#include "app_wf_cas_api_baseinfo.h"


SIGNAL_HANDLER  int win_wf_cas_baseinfo_create(const char* widgetname, void *usrdata)
{

	ca_get_date_t get_data = {0};
	char* str = NULL;
	
	get_data.date_type = DVB_CA_BASE_INFO;		
	get_data.ID = WF_BASEINFO_CARDID_ID;
	str = app_cas_api_get_data(&get_data);
	if (NULL !=str )
	{
            GUI_SetProperty("win_wf_cas_baseinfo_cardno_id_value", "string",str);			
	}

	get_data.ID = WF_BASEINFO_PURSEINFO_ID;
	str = app_cas_api_get_data(&get_data);
	if (NULL !=str )
	{
            GUI_SetProperty("win_wf_cas_baseinfo_purseinfo_id_value", "string",str);			
	}

	get_data.ID = WF_BASEINFO_STBNO_ID;
	str = app_cas_api_get_data(&get_data);
	if (NULL !=str )
	{
            GUI_SetProperty("win_wf_cas_baseinfo_stbno_id_value", "string",str);			
	}

	get_data.ID = WF_BASEINFO_CASID_ID;
	str = app_cas_api_get_data(&get_data);
	if (NULL !=str )
	{
            GUI_SetProperty("win_wf_cas_baseinfo_casid_id_value", "string",str);			
	}

	get_data.ID = WF_BASEINFO_CASVER_ID;
	str = app_cas_api_get_data(&get_data);
	if (NULL !=str )
	{
            GUI_SetProperty("win_wf_cas_baseinfo_casver_id_value", "string",str);			
	}

	get_data.ID = WF_BASEINFO_CASPRO_ID;
	str = app_cas_api_get_data(&get_data);
	if (NULL !=str )
	{
            GUI_SetProperty("win_wf_cas_baseinfo_caspro_id_value", "string",str);			
	}

	get_data.ID = WF_BASEINFO_CASDES_ID;
	str = app_cas_api_get_data(&get_data);
	if (NULL !=str )
	{
            GUI_SetProperty("win_wf_cas_baseinfo_casdes_id_value", "string",str);			
	}

	get_data.ID = WF_BASEINFO_CARDVALID_ID;
	str = app_cas_api_get_data(&get_data);
	if (NULL !=str )
	{
            GUI_SetProperty("win_wf_cas_baseinfo_cardvalid_id_value", "string",str);			
	}

	get_data.ID = WF_BASEINFO_DATATIME_ID;
	str = app_cas_api_get_data(&get_data);
	if (NULL !=str )
	{
            GUI_SetProperty("win_wf_cas_baseinfo_datatime_id_value", "string",str);			
	}

	return 0;
}

SIGNAL_HANDLER  int win_wf_cas_baseinfo_destroy(const char* widgetname, void *usrdata)
{
	return 0;
}

SIGNAL_HANDLER  int win_wf_cas_baseinfo_keypress(const char* widgetname, void *usrdata)
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
				GUI_EndDialog("win_wf_cas_baseinfo");
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

