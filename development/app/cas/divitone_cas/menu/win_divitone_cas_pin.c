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
* Purpose   :	ģ??ͷ??\u0152?
******************************************************************************
* Release History:
VERSION	Date			  AUTHOR         Description
1.0  	2012.12.02		  zhouhm 	 			creation
*****************************************************************************/
#include "app_common_porting_stb_api.h"
#include "app_win_interface.h"
#include "app_divitone_cas_api_pin.h"
#include "DiviGuardCa.h"
extern UINT8 Repeattime;

SIGNAL_HANDLER  int win_divitone_cas_pin_create(const char* widgetname, void *usrdata)
{
	return 0;
}

SIGNAL_HANDLER  int win_divitone_cas_pin_destroy(const char* widgetname, void *usrdata)
{
	return 0;
}

SIGNAL_HANDLER  int win_divitone_cas_pin_keypress(const char* widgetname, void *usrdata)
{
	GUI_Event *event = NULL;
	event = (GUI_Event *)usrdata;
	ca_pin_t pin={0};
	uint8_t *valueOld;
	uint8_t *valueNew;
	uint8_t *valueNewConfirm;
	char* str = NULL;
	popmsg_ret ret;

	char buf[40];

	if(event->type == GUI_KEYDOWN)
	{
		switch(event->key.sym)
		{
			case KEY_EXIT:
				break;
				return EVENT_TRANSFER_STOP;
			case KEY_RECALL:
				 GUI_EndDialog("win_divitone_cas_pin");
				 return EVENT_TRANSFER_STOP;

			case KEY_OK:
				/*????ƥ??ԭʼ????*/
				GUI_GetProperty("win_divitone_cas_pin_boxitem1_edit", "string", &valueOld);
				GUI_GetProperty("win_divitone_cas_pin_boxitem2_edit", "string", &valueNew);
				GUI_GetProperty("win_divitone_cas_pin_boxitem3_edit", "string", &valueNewConfirm);
				pin.date_type = DVB_CA_CHANGE_PIN_INFO;
				pin.oldpin = (char*)valueOld;
				pin.newpin = (char*)valueNew;
				pin.newconfirmpin = (char*)valueNewConfirm;
				str = app_cas_api_change_pin(&pin);
				if ((NULL != str)&&(0 == strcmp(str,"Input new PIN")))
				{
					GUI_SetProperty("win_divitone_cas_pin_boxitem1_edit","clear",NULL);
					GUI_SetProperty("win_divitone_cas_pin_boxitem2_edit","clear",NULL);
                    		GUI_SetProperty("win_divitone_cas_pin_boxitem3_edit","clear",NULL);
	 	   			ret = app_popmsg(210, 200,str,POPMSG_TYPE_OK);
					return EVENT_TRANSFER_STOP;
				}
				if ((NULL != str)&&(0 == strcmp(str,"PIN Error!")))
				{
					if(Repeattime == 17)
						snprintf(buf,40,"Card Locked!");
					else
						snprintf(buf,40,"???????󣬻????Գ??? %d ??",Repeattime);
					GUI_SetProperty("win_divitone_cas_pin_boxitem1_edit","clear",NULL);
					GUI_SetProperty("win_divitone_cas_pin_boxitem2_edit","clear",NULL);
                    		GUI_SetProperty("win_divitone_cas_pin_boxitem3_edit","clear",NULL);
					app_popmsg(210, 200,buf,POPMSG_TYPE_OK);
					return EVENT_TRANSFER_STOP;
				}
				if (NULL != str)
					{
						ret = app_popmsg(210, 200,str,POPMSG_TYPE_OK);
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

SIGNAL_HANDLER  int win_divitone_cas_pin_box_keypress(const char* widgetname, void *usrdata)
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









