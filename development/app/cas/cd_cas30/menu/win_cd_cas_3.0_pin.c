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
* Purpose   :	模块头文\u0152?******************************************************************************
* Release History:
VERSION	Date			  AUTHOR         Description
1.0  	2012.12.02		  zhouhm 	 			creation
*****************************************************************************/
#include "app_common_porting_stb_api.h"
#include "app_win_interface.h"
#include "app_cd_cas_3.0_api_pin.h"


SIGNAL_HANDLER  int win_cd_cas30_pin_create(const char* widgetname, void *usrdata)
{
	char* osd_language=NULL;
    osd_language = app_flash_get_config_osd_language();
	if (0 == strcmp(osd_language,LANGUAGE_CHINESE))
    {
 //   	GUI_SetProperty("win_cd_cas30_pin_tip_image_exit", "img", "tips_exit.bmp");
//		GUI_SetProperty("win_cd_cas30_pin_tip_image_ok", "img", "tips_confirm.bmp");
    }
    else if(0 == strcmp(osd_language,LANGUAGE_ENGLISH))
     {
   // 	GUI_SetProperty("win_cd_cas30_pin_tip_image_exit", "img", "tips_exit_e.bmp");
//		GUI_SetProperty("win_cd_cas30_pin_tip_image_ok", "img", "tips_confirm_e2.bmp");
    }

	return 0;
}

SIGNAL_HANDLER  int win_cd_cas30_pin_destroy(const char* widgetname, void *usrdata)
{
	return 0;
}

SIGNAL_HANDLER  int win_cd_cas30_pin_keypress(const char* widgetname, void *usrdata)
{
	GUI_Event *event = NULL;
	event = (GUI_Event *)usrdata;
	ca_pin_t pin={0};
	uint8_t *valueOld;
	uint8_t *valueNew;
	uint8_t *valueNewConfirm;
	char* str = NULL;


	if(event->type == GUI_KEYDOWN)
	{
		switch(event->key.sym)
		{
			case KEY_EXIT:
				break;
				return EVENT_TRANSFER_STOP;
			case KEY_RECALL:
				 GUI_EndDialog("win_ca_pinset");
				 return EVENT_TRANSFER_STOP;

			case KEY_OK:
				/*首先匹配原始密码*/
				GUI_GetProperty("win_cd_cas30_pin_boxitem1_edit", "string", &valueOld);
				GUI_GetProperty("win_cd_cas30_pin_boxitem2_edit", "string", &valueNew);
				GUI_GetProperty("win_cd_cas30_pin_boxitem3_edit", "string", &valueNewConfirm);
				pin.date_type = DVB_CA_CHANGE_PIN_INFO;
				pin.oldpin = (char*)valueOld;
				pin.newpin = (char*)valueNew;
				pin.newconfirmpin = (char*)valueNewConfirm;
				pin.errorCode = 1;
				str = app_cas_api_change_pin(&pin);
				if ((NULL != str)&&(1 == pin.errorCode))
				{
					GUI_SetProperty("win_cd_cas30_pin_boxitem1_edit","clear",NULL);
					GUI_SetProperty("win_cd_cas30_pin_boxitem2_edit","clear",NULL);
		                    GUI_SetProperty("win_cd_cas30_pin_boxitem3_edit","clear",NULL);
	 	   			#ifdef APP_SD
					app_popmsg(210,150,str,POPMSG_TYPE_OK);
					#endif
					#ifdef APP_HD
					app_popmsg(340, 200,str,POPMSG_TYPE_OK);
					#endif
					return EVENT_TRANSFER_STOP;
				}

				if (NULL != str)
					{
						#ifdef APP_SD
						app_popmsg(210,150,str,POPMSG_TYPE_OK);
						#endif
						#ifdef APP_HD
						app_popmsg(340, 200,str,POPMSG_TYPE_OK);
						#endif
						return EVENT_TRANSFER_STOP;
					}

				return EVENT_TRANSFER_STOP;
			case KEY_POWER:
				return EVENT_TRANSFER_KEEPON;
			default:
				return EVENT_TRANSFER_STOP;
		}
	}

	return EVENT_TRANSFER_KEEPON;
}

SIGNAL_HANDLER  int win_cd_cas30_pin_box_keypress(const char* widgetname, void *usrdata)
{
	GUI_Event *event = NULL;
	event = (GUI_Event *)usrdata;

	if(event->type == GUI_KEYDOWN)
	{
		switch(event->key.sym)
		{
		case KEY_MENU:
			event->key.sym = KEY_EXIT;
			return EVENT_TRANSFER_KEEPON;
			break;
		case KEY_POWER:
			return EVENT_TRANSFER_KEEPON;
		default:
			return EVENT_TRANSFER_KEEPON;
		}
	}

	return EVENT_TRANSFER_KEEPON;
}









