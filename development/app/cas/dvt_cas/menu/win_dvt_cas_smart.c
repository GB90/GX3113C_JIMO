/*****************************************************************************
* 						   CONFIDENTIAL
*        Hangzhou GuoXin Science and Technology Co., Ltd.
*                      (C)2012, All right reserved
******************************************************************************

******************************************************************************
* File Name :	win_dvt_cas_smart.c
* Author    : 	zhouhm
* Project   :	goxceed dvbc
* Type      :
******************************************************************************
* Purpose   :	模块头文件
******************************************************************************
* Release History:
VERSION	Date			  AUTHOR         Description
1.0  	2013.12.04		  zhouhm 	 			creation
*****************************************************************************/
#include "app_common_porting_stb_api.h"
#include "app_win_interface.h"
#include "app_dvt_cas_api_baseinfo.h"

SIGNAL_HANDLER  int win_dvt_cas_smart_create(const char* widgetname, void *usrdata)
{
	char* str = NULL;

	ca_get_date_t get_data = {0};

	 char* osd_language=NULL;	
	
	 osd_language = app_flash_get_config_osd_language();
	 if (0 == strcmp(osd_language,LANGUAGE_CHINESE))	
	{
	//	 GUI_SetProperty("win_dvt_cas_smart_title","img","title_ch_Smartcard.bmp");  
	//	 GUI_SetProperty("win_dvt_cas_smart_image_exit", "img", "tips_exit.bmp");
	 }
	 else if(0 == strcmp(osd_language,LANGUAGE_ENGLISH))
	 { 
	//	 GUI_SetProperty("win_dvt_cas_smart_title","img","title_Smartcard_e.bmp");
	//	 GUI_SetProperty("win_dvt_cas_smart_image_exit","img","tips_exit_e.bmp");
	 }

	get_data.date_type = DVB_CA_BASE_INFO;		

	
	get_data.ID = DVT_BASEINFO_SERIAL_ID;
	str = app_cas_api_get_data(&get_data);
	if (NULL !=str )
		{
                GUI_SetProperty("win_dvt_cas_smart_card_num", "string",str);			
		}

	get_data.ID = DVT_BASEINFO_COS_VER_ID;
	str = app_cas_api_get_data(&get_data);
	if (NULL !=str )
		{
                GUI_SetProperty("win_dvt_cas_smart_cos", "string",str);			
		}

	get_data.ID = DVT_BASEINFO_CAS_VER_ID;
	str = app_cas_api_get_data(&get_data);
	if (NULL !=str )
		{
                GUI_SetProperty("win_dvt_cas_smart_module", "string",str);			
		}


	get_data.ID = DVT_BASEINFO_MANU_NAME_ID;
	str = app_cas_api_get_data(&get_data);
	if (NULL !=str )
		{
                GUI_SetProperty("win_dvt_cas_smart_ca", "string",str);			
		}

	get_data.ID = DVT_BASEINFO_MOTHER_ID;
	str = app_cas_api_get_data(&get_data);
	if (NULL !=str )
		{
                GUI_SetProperty("win_dvt_cas_smart_mother", "string",str);			
		}

	get_data.ID = DVT_BASEINFO_MOTHER_NUMBER_ID;
	str = app_cas_api_get_data(&get_data);
	if (NULL !=str )
		{
                GUI_SetProperty("win_dvt_cas_smart_mother_num", "string",str);			
		}

	get_data.ID = DVT_BASEINFO_LOCK_ID;
	str = app_cas_api_get_data(&get_data);
	if (NULL !=str )
		{
                GUI_SetProperty("win_dvt_cas_smart_lock", "string",str);			
		}

	get_data.ID = DVT_BASEINFO_RATING_ID;
	str = app_cas_api_get_data(&get_data);
	if (NULL !=str )
		{
                GUI_SetProperty("win_dvt_cas_smart_rating", "string",str);			
		}

	get_data.ID = DVT_BASEINFO_WORK_TIME_ID;
	str = app_cas_api_get_data(&get_data);
	if (NULL !=str )
		{
                GUI_SetProperty("win_dvt_cas_smart_work_time", "string",str);			
		}

	get_data.ID = DVT_BASEINFO_AREA_ID;
	str = app_cas_api_get_data(&get_data);
	if (NULL !=str )
		{
                GUI_SetProperty("win_dvt_cas_smart_area", "string",str);			
		}

	return 0;
}

SIGNAL_HANDLER  int win_dvt_cas_smart_destroy(const char* widgetname, void *usrdata)
{
	return 0;
}

SIGNAL_HANDLER  int win_dvt_cas_smart_keypress(const char* widgetname, void *usrdata)
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
				GUI_EndDialog("win_dvt_cas_smart");
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

