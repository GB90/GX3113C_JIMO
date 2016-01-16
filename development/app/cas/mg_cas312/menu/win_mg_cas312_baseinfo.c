/*****************************************************************************
* 						   CONFIDENTIAL
*        Hangzhou GuoXin Science and Technology Co., Ltd.
*                      (C)2012, All right reserved
******************************************************************************

******************************************************************************
* File Name :	win_mg_cas_baseinfo.c
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
#include "app_mg_cas312_api_baseinfo.h"

SIGNAL_HANDLER  int win_mg_cas_baseinfo_create(const char* widgetname, void *usrdata)
{
	ca_get_date_t get_data = {0};
	char* str = NULL;

    char* osd_language=NULL;
    osd_language = app_flash_get_config_osd_language();
    if (0 == strcmp(osd_language,LANGUAGE_CHINESE))
    {
 //   	GUI_SetProperty("win_mgcas_baseinfo_tip_image_exit", "img", "tips_exit.bmp");
    }
    else if(0 == strcmp(osd_language,LANGUAGE_ENGLISH))
     {
    //	GUI_SetProperty("win_mgcas_baseinfo_tip_image_exit", "img", "tips_exit_e.bmp");
    }
	
	get_data.date_type = DVB_CA_BASE_INFO;	
	get_data.ID = MG312_BASEINFO_IVS_ID;
	str = app_cas_api_get_data(&get_data);
	if (NULL !=str )
	{
        GUI_SetProperty("win_mg_cas_baseinfo_ivs_value", "string",str);			
	}

	get_data.ID = MG312_BASEINFO_VER_ID;
	str = app_cas_api_get_data(&get_data);
	if (NULL != str )
	{
        GUI_SetProperty("win_mg_cas_baseinfo_casver_value", "string",str);			
	}

    get_data.ID = MG312_BASEINFO_CARDSN_ID;
	str = app_cas_api_get_data(&get_data);
	if (NULL != str )
	{
        GUI_SetProperty("win_mg_cas_baseinfo_cardno_value", "string",str);			
	}
    get_data.ID = MG312_BASEINFO_CARDVER_ID;
	str = app_cas_api_get_data(&get_data);
	if (NULL != str )
	{
        GUI_SetProperty("win_mg_cas_baseinfo_cardver_value", "string",str);			
	}
    get_data.ID = MG312_BASEINFO_BALANCE_ID;
	str = app_cas_api_get_data(&get_data);
	if (NULL != str )
	{
        GUI_SetProperty("win_mg_cas_baseinfo_balance_value", "string",str);			
	}
    get_data.ID = MG312_BASEINFO_OVERDRAFT_ID;
	str = app_cas_api_get_data(&get_data);
	if (NULL != str )
	{
        GUI_SetProperty("win_mg_cas_baseinfo_overdraft_value", "string",str);			
	}
    get_data.ID = MG312_BASEINFO_AREACODE_ID;
	str = app_cas_api_get_data(&get_data);
	if (NULL != str )
	{
        GUI_SetProperty("win_mg_cas_baseinfo_zonecode_value", "string",str);			
	}
    get_data.ID = MG312_BASEINFO_PARTNER_ID;
	str = app_cas_api_get_data(&get_data);
	if (NULL != str )
	{
        GUI_SetProperty("win_mg_cas_baseinfo_partner_value", "string",str);			
	}

	return 0;
}

SIGNAL_HANDLER  int win_mg_cas_baseinfo_destroy(const char* widgetname, void *usrdata)
{
	return 0;
}

SIGNAL_HANDLER  int win_mg_cas_baseinfo_keypress(const char* widgetname, void *usrdata)
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
				GUI_EndDialog("win_mg_cas312_baseinfo");
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

