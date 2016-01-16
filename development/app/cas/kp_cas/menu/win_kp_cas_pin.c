#include "app_common_porting_stb_api.h"
#include "app_win_interface.h"
#include "app_kp_cas_api_pin.h"

SIGNAL_HANDLER  int win_kp_cas_pin_create(const char* widgetname, void *usrdata)
{
	return 0;
}

SIGNAL_HANDLER  int win_kp_cas_pin_destroy(const char* widgetname, void *usrdata)
{
	return 0;
}

SIGNAL_HANDLER  int win_kp_cas_pin_keypress(const char* widgetname, void *usrdata)
{
	GUI_Event *event = NULL;
	event = (GUI_Event *)usrdata;
	ca_pin_t pin={0};
	uint8_t *valueOld;
	uint8_t *valueNew;
	uint8_t *valueNewConfirm;
	char* str = NULL;
	popmsg_ret ret;

	if(event->type == GUI_KEYDOWN)
	{
		switch(event->key.sym)
		{
			case KEY_EXIT:
				break;
				return EVENT_TRANSFER_STOP;
			case KEY_RECALL:
				 GUI_EndDialog("win_kp_cas_pin");
				 return EVENT_TRANSFER_STOP;

			case KEY_OK:
				/*首先匹配原始密码*/
				GUI_GetProperty("win_kp_cas_pin_boxitem1_edit", "string", &valueOld);
				GUI_GetProperty("win_kp_cas_pin_boxitem2_edit", "string", &valueNew);
				GUI_GetProperty("win_kp_cas_pin_boxitem3_edit", "string", &valueNewConfirm);
				pin.date_type = DVB_CA_CHANGE_PIN_INFO;
				pin.oldpin = (char*)valueOld;
				pin.newpin = (char*)valueNew;
				pin.newconfirmpin = (char*)valueNewConfirm;
				str = app_cas_api_change_pin(&pin);
				if ((NULL != str)&& strcmp(str,"Set PIN Succeed"))
				{
					GUI_SetProperty("win_kp_cas_pin_boxitem1_edit","clear",NULL);
					GUI_SetProperty("win_kp_cas_pin_boxitem2_edit","clear",NULL);
                    GUI_SetProperty("win_kp_cas_pin_boxitem3_edit","clear",NULL);
	 	   			ret = app_popmsg(POP_DIALOG_X_START, POP_DIALOG_Y_START,str,POPMSG_TYPE_OK);
					return EVENT_TRANSFER_STOP;
				}
				if (NULL != str)
				{
					ret = app_popmsg(POP_DIALOG_X_START, POP_DIALOG_Y_START,str,POPMSG_TYPE_OK);
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

SIGNAL_HANDLER  int win_kp_cas_pin_box_keypress(const char* widgetname, void *usrdata)
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


