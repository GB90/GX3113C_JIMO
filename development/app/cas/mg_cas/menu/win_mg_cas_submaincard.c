/*****************************************************************************
* 						   CONFIDENTIAL
*        Hangzhou GuoXin Science and Technology Co., Ltd.
*                      (C)2012, All right reserved
******************************************************************************

******************************************************************************
* File Name :	win_xunchi_cas_submaincard.c
* Author    : 	chenth
* Project   :	goxceed dvbc
* Type      :
******************************************************************************
* Purpose   :	Ñ¸³ÛCA
******************************************************************************
* Release History:
VERSION	Date			  AUTHOR         Description
1.0  	2014.04.14		  chenth 	 			creation
*****************************************************************************/
#include "app_common_porting_stb_api.h"
#include "app_win_interface.h"
#include "app_mg_cas_api_baseinfo.h"

SIGNAL_HANDLER  int win_mg_cas_submaincard_create(const char* widgetname, void *usrdata)
{
    ca_get_date_t get_data = {0};
	char* str = NULL;

    char* osd_language=NULL;
    osd_language = app_flash_get_config_osd_language();
    if (0 == strcmp(osd_language,LANGUAGE_CHINESE))
    {
   // 	GUI_SetProperty("win_mgcas_submaincard_tip_image_exit", "img", "tips_exit.bmp");
    }
    else if(0 == strcmp(osd_language,LANGUAGE_ENGLISH))
    { 
    //	GUI_SetProperty("win_mgcas_submaincard_tip_image_exit", "img", "tips_exit_e.bmp");
    }

    
	get_data.date_type = DVB_CA_BASE_INFO;	
	get_data.ID = MG_BASEINFO_SUBMAINCARD_TYPE_ID;
	str = app_cas_api_get_data(&get_data);
	if (NULL !=str )
	{
        GUI_SetProperty("win_mg_cas_submaincard_type", "string",str);			
	}

    get_data.ID = MG_BASEINFO_SUBMAINCARD_MAINID_ID;
	str = app_cas_api_get_data(&get_data);
	if (NULL != str )
	{
        GUI_SetProperty("win_mg_cas_submaincard_mainid", "string",str);			
	}
    get_data.ID = MG_BASEINFO_SUBMAINCARD_TIME_ID;
	str = app_cas_api_get_data(&get_data);
	if (NULL != str )
	{
        GUI_SetProperty("win_mg_cas_submaincard_time", "string",str);			
	}
	return 0;
}

SIGNAL_HANDLER  int win_mg_cas_submaincard_destroy(const char* widgetname, void *usrdata)
{
	return 0;
}

SIGNAL_HANDLER  int win_mg_cas_submaincard_keypress(const char* widgetname, void *usrdata)
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
				GUI_EndDialog("win_mg_cas_submaincard");
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


