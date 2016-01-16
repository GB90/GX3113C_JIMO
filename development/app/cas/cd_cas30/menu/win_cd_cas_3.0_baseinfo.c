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
#include "app_cd_cas_3.0_api_baseinfo.h"


SIGNAL_HANDLER  int win_cd_cas30_baseinfo_create(const char* widgetname, void *usrdata)
{

	ca_get_date_t get_data = {0};
	char* str = NULL;
	char* osd_language=NULL;


    osd_language = app_flash_get_config_osd_language();
	if (0 == strcmp(osd_language,LANGUAGE_CHINESE))
    {
 //   	GUI_SetProperty("win_cd_cas30_baseinfo_tip_image_exit", "img", "tips_exit.bmp");
    }
    else if(0 == strcmp(osd_language,LANGUAGE_ENGLISH))
     {
    //	GUI_SetProperty("win_cd_cas30_baseinfo_tip_image_exit", "img", "tips_exit_e.bmp");
    }
	get_data.date_type = DVB_CA_BASE_INFO;	
	get_data.ID = CD_BASEINFO_OPERATE1_ID;
	str = app_cas_api_get_data(&get_data);
	if (NULL !=str )
		{
                GUI_SetProperty("win_cd_cas30_baseinfo_operator1", "string",str);			
 		}

	get_data.ID = CD_BASEINFO_OPERATE2_ID;
	str = app_cas_api_get_data(&get_data);
	if (NULL !=str )
		{
                GUI_SetProperty("win_cd_cas30_baseinfo_operator2", "string",str);			
		}

	get_data.ID = CD_BASEINFO_OPERATE3_ID;
	str = app_cas_api_get_data(&get_data);
	if (NULL !=str )
		{
                GUI_SetProperty("win_cd_cas30_baseinfo_operator3", "string",str);			
		}

	get_data.ID = CD_BASEINFO_OPERATE4_ID;
	str = app_cas_api_get_data(&get_data);
	if (NULL !=str )
		{
                GUI_SetProperty("win_cd_cas30_baseinfo_operator4", "string",str);			
		}

	get_data.ID = CD_BASEINFO_PARIED_ID;
	str = app_cas_api_get_data(&get_data);
	if (NULL !=str )
		{
                GUI_SetProperty("win_cd_cas30_baseinfo_paried", "string",str);			
		}

	get_data.ID = CD_BASEINFO_WORK_TIME_ID;
	str = app_cas_api_get_data(&get_data);
	if (NULL !=str )
		{
                GUI_SetProperty("win_cd_cas30_baseinfo_worktime", "string",str);			
		}

	get_data.ID = CD_BASEINFO_RATING_ID;
	str = app_cas_api_get_data(&get_data);
	if (NULL !=str )
		{
                GUI_SetProperty("win_cd_cas30_baseinfo_level", "string",str);			
		}

	get_data.ID = CD_BASEINFO_CARD_SN_ID;
	str = app_cas_api_get_data(&get_data);
	if (NULL !=str )
		{
                GUI_SetProperty("win_cd_cas30_baseinfo_id", "string",str);			
		}

	get_data.ID = CD_BASEINFO_VERSION_ID;
	str = app_cas_api_get_data(&get_data);
	if (NULL !=str )
		{
                GUI_SetProperty("win_cd_cas30_baseinfo_version", "string",str);			
		}

	get_data.ID = CD_BASEINFO_STB_ID_ID;
	str = app_cas_api_get_data(&get_data);
	if (NULL !=str )
		{
                GUI_SetProperty("win_cd_cas30_baseinfo_unique", "string",str);			
		}
	
	return 0;
}

SIGNAL_HANDLER  int win_cd_cas30_baseinfo_destroy(const char* widgetname, void *usrdata)
{
	return 0;
}

SIGNAL_HANDLER  int win_cd_cas30_baseinfo_keypress(const char* widgetname, void *usrdata)
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
				GUI_EndDialog("win_cd_cas30_baseinfo");
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

