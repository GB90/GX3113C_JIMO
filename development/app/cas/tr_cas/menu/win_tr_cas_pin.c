/*****************************************************************************
* 						   CONFIDENTIAL
*        Hangzhou GuoXin Science and Technology Co., Ltd.
*                      (C)2012, All right reserved
******************************************************************************

******************************************************************************
* File Name :	win_tr_cas_pin.c
* Author    : 	zhouhm
* Project   :	goxceed dvbc
* Type      :
******************************************************************************
* Purpose   :	ģ??ͷ??\u0152?
******************************************************************************
* Release History:
VERSION	Date			  AUTHOR         Description
1.0  	2013.12.05		  zhouhm 	 			creation
*****************************************************************************/
#include "app_common_porting_stb_api.h"
#include "app_win_interface.h"
#include "Tr_Cas.h"
#include "app_tr_cas_api_pin.h"


SIGNAL_HANDLER  int win_tr_cas_pin_create(const char* widgetname, void *usrdata)
{
	 char* osd_language = NULL;	
	
	 osd_language = app_flash_get_config_osd_language();
	 
	if (0 == strcmp(osd_language,LANGUAGE_CHINESE))	
	{
	//	GUI_SetProperty("win_tr_cas_pin_title","img","title_ch_Password.bmp");  
	//	GUI_SetProperty("win_tr_cas_pin_tip_image_exit", "img", "tips_exit.bmp");
	//	GUI_SetProperty("win_tr_cas_pin_tip_image_ok", "img", "tips_confirm.bmp");
	}
	else if (0 == strcmp(osd_language,LANGUAGE_ENGLISH))
	{ 
	//	GUI_SetProperty("win_tr_cas_pin_title","img","title_Password_e.bmp");
	//	GUI_SetProperty("win_tr_cas_pin_tip_image_exit","img","tips_exit_e.bmp");
	//	GUI_SetProperty("win_tr_cas_pin_tip_image_ok", "img", "tips_confirm_e2.bmp");
	}

	return 0;
}

SIGNAL_HANDLER  int win_tr_cas_pin_destroy(const char* widgetname, void *usrdata)
{
	return 0;
}

SIGNAL_HANDLER  int win_tr_cas_pin_keypress(const char* widgetname, void *usrdata)
{
	GUI_Event *event = NULL;
	event = (GUI_Event *)usrdata;

	if (event->type == GUI_KEYDOWN)
	{
		switch (event->key.sym)
		{
			case KEY_EXIT:
				break;
				return EVENT_TRANSFER_STOP;
			case KEY_RECALL:
				 GUI_EndDialog("win_tr_cas_pin");
				 return EVENT_TRANSFER_STOP;
			case KEY_POWER:
				return EVENT_TRANSFER_KEEPON;
			default:
				return EVENT_TRANSFER_STOP;
		}
	}

	return EVENT_TRANSFER_KEEPON;
}

SIGNAL_HANDLER  int win_tr_cas_pin_box_keypress(const char* widgetname, void *usrdata)
{
	GUI_Event *event = NULL;
	ca_pin_t pin = {0};
	uint8_t curIdx = 0;
	uint8_t *valueOld;
	uint8_t *valueNew;
	uint8_t *valueNewConfirm;
	char* str = NULL;

	event = (GUI_Event *)usrdata;
	if (event->type == GUI_KEYDOWN)
	{
		switch (event->key.sym)
		{
		case KEY_MENU:
			event->key.sym = KEY_EXIT;
			return EVENT_TRANSFER_KEEPON;
			break;

		case KEY_DOWN:
		{
			GUI_GetProperty("win_tr_cas_pin_box", "select", &curIdx);
			if (0 == curIdx)
			{
				GUI_GetProperty("win_tr_cas_pin_boxitem1_edit", "string", &valueOld);
				if (strlen((char*)valueOld) != TRCAS_PIN_MAXLEN)
				{

					app_popmsg(POP_DIALOG_X_START, POP_DIALOG_Y_START, "Input 6-bit PIN", POPMSG_TYPE_OK);

			        return EVENT_TRANSFER_STOP; 
				}
			}
		}
			break;
			
		case KEY_OK:
			/*????ƥ??ԭʼ????*/
			GUI_GetProperty("win_tr_cas_pin_boxitem1_edit", "string", &valueOld);
			GUI_GetProperty("win_tr_cas_pin_boxitem2_edit", "string", &valueNew);
			GUI_GetProperty("win_tr_cas_pin_boxitem3_edit", "string", &valueNewConfirm);
			pin.date_type = DVB_CA_CHANGE_PIN_INFO;
			pin.oldpin = (char*)valueOld;
			pin.newpin = (char*)valueNew;
			pin.newconfirmpin = (char*)valueNewConfirm;
			str = app_cas_api_change_pin(&pin);
			
			if (NULL != str)
			{

                app_popmsg(POP_DIALOG_X_START, POP_DIALOG_Y_START,str,POPMSG_TYPE_OK);

				return EVENT_TRANSFER_STOP;
			}

			return EVENT_TRANSFER_STOP;

		case KEY_POWER:
			return EVENT_TRANSFER_KEEPON;
		default:
			return EVENT_TRANSFER_KEEPON;
		}
	}

	return EVENT_TRANSFER_KEEPON;
}


