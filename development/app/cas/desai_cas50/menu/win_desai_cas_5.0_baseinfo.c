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
#include "app_desai_cas_5.0_api_baseinfo.h"

SIGNAL_HANDLER  int win_desai_cas50_baseinfo_create(const char* widgetname, void *usrdata)
{

	ca_get_date_t get_data = {0};
//       unsigned int      Row    = 0;
//       unsigned int      ID    = 0;
	char* str = NULL;
	
	get_data.date_type = DVB_CA_BASE_INFO;
	
	get_data.ID = DS_BASEINFO_OPERATE_ID;
	str = app_cas_api_get_data(&get_data);
	if (NULL !=str )
		{
                GUI_SetProperty("win_desai_cas50_baseinfo_operator1", "string",str);			
		}

	get_data.ID = DS_BASEINFO_CARD_SN_ID;
	str = app_cas_api_get_data(&get_data);
	if (NULL !=str )
		{
                GUI_SetProperty("win_desai_cas50_baseinfo_operator2", "string",str);			
		}

	get_data.ID = DS_BASEINFO_GROUP_ID;
	str = app_cas_api_get_data(&get_data);
	if (NULL !=str )
		{
                GUI_SetProperty("win_desai_cas50_baseinfo_operator3", "string",str);			
		}

	get_data.ID = DS_BASEINFO_LCO_ID;
	str = app_cas_api_get_data(&get_data);
	if (NULL !=str )
		{
                GUI_SetProperty("win_desai_cas50_baseinfo_operator4", "string",str);			
		}

	get_data.ID = DS_BASEINFO_CARDREG_ID;
	str = app_cas_api_get_data(&get_data);
	if (NULL !=str )
		{
                GUI_SetProperty("win_desai_cas50_baseinfo_paried", "string",str);			
		}

	get_data.ID = DS_BASEINFO_CARDSTATE_ID;
	str = app_cas_api_get_data(&get_data);
	if (NULL !=str )
		{
                GUI_SetProperty("win_desai_cas50_baseinfo_worktime", "string",str);			
		}

	get_data.ID = DS_BASEINFO_VERSION_ID;
	str = app_cas_api_get_data(&get_data);
	if (NULL !=str )
		{
                GUI_SetProperty("win_desai_cas50_baseinfo_level", "string",str);			
		}

	get_data.ID = DS_BASEINFO_WORK_TIME_ID;
	str = app_cas_api_get_data(&get_data);
	if (NULL !=str )
		{
                GUI_SetProperty("win_desai_cas50_baseinfo_id", "string",str);			
		}

	get_data.ID = DS_BASEINFO_RATTING_ID;
	str = app_cas_api_get_data(&get_data);
	if (NULL !=str )
		{
                GUI_SetProperty("win_desai_cas50_baseinfo_version", "string",str);			
		}

	get_data.ID = DS_BASEINFO_CARDVER_ID;
	str = app_cas_api_get_data(&get_data);
	if (NULL !=str )
		{
                GUI_SetProperty("win_desai_cas50_baseinfo_unique", "string",str);			
		}
	get_data.ID = DS_BASEINFO_MOTHER_CARD_ID;
	str = app_cas_api_get_data(&get_data);
	if (NULL !=str )
	{
            GUI_SetProperty("win_desai_cas50_baseinfo_mothercard", "string",str);			
	}
	return 0;
}

SIGNAL_HANDLER  int win_desai_cas50_baseinfo_destroy(const char* widgetname, void *usrdata)
{
	return 0;
}

SIGNAL_HANDLER  int win_desai_cas50_baseinfo_keypress(const char* widgetname, void *usrdata)
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
			case KEY_MENU:
				GUI_EndDialog("win_desai_cas50_baseinfo");
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


