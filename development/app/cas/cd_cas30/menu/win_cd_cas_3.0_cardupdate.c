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

SIGNAL_HANDLER  int win_cd_cas30_cardupdate_create(const char* widgetname, void *usrdata)
{
	int32_t config = 0;
    uint8_t buf[50] = {0};

	char* osd_language=NULL;
    osd_language = app_flash_get_config_osd_language();
	if (0 == strcmp(osd_language,LANGUAGE_CHINESE))
    {
 //   	GUI_SetProperty("win_cd_cas30_cardupdate_tip_image_exit", "img", "tips_exit.bmp");
		//GUI_SetProperty("win_cd_cas30_email_confirm_flag", "img", "tips_confirm.bmp");
    }
    else if(0 == strcmp(osd_language,LANGUAGE_ENGLISH))
     {
    //	GUI_SetProperty("win_cd_cas30_cardupdate_tip_image_exit", "img", "tips_exit_e.bmp");
		//GUI_SetProperty("win_cd_cas30_email_confirm_flag", "img", "tips_confirm_e2.bmp");
    }

	
	GxBus_ConfigGetInt(CARD_UPDATE_FLAG,&config,CARD_UPDATE_FLAG_DV);
	GxBus_ConfigGet(CARD_UPDATE_TIME, (char*)buf,17,CARD_UPDATE_TIME_DV);	
	switch(config)
		{
			case 0:	
				break;
			case 1:
				break;
			case 2:
				/*升级成功*/
				GUI_SetProperty("win_cd_cas30_cardupdate_record_value", "string", "Card Update Success");
				GUI_SetProperty("win_cd_cas30_cardupdate_time_value", "string", buf);						
				break;
			default:
				break;
		}
	return 0;
}

SIGNAL_HANDLER  int win_cd_cas30_cardupdate_destroy(const char* widgetname, void *usrdata)
{
	return 0;
}

SIGNAL_HANDLER  int win_cd_cas30_cardupdate_keypress(const char* widgetname, void *usrdata)
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
				 GUI_EndDialog("win_cd_cas30_cardupdate");
				 return EVENT_TRANSFER_STOP;
			case KEY_OK:
				break;
				return EVENT_TRANSFER_STOP;
			case KEY_POWER:
				return EVENT_TRANSFER_KEEPON;
			default:
				return EVENT_TRANSFER_STOP;
		}
	}

	return EVENT_TRANSFER_KEEPON;
}


