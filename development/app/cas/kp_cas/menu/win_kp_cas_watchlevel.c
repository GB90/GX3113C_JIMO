#include "app_common_porting_stb_api.h"
#include "app_win_interface.h"
#include "app_kp_cas_api_watchlevel.h"

SIGNAL_HANDLER  int win_kp_cas_watchlevel_create(const char* widgetname, void *usrdata)
{
	uint32_t rating = 0;
	ca_get_date_t get_data = {0};
	char* str = NULL;
	
	get_data.date_type = DVB_CA_RATING_INFO;	
	get_data.ID = KP_RATING_LEVEL_ID;
	str = app_cas_api_get_data(&get_data);
	if (NULL !=str )
	{
		rating = atoi(str);
		GUI_SetProperty("win_kp_cas_watchlevel_boxitem1_value", "select", &rating);			
	}

	return 0;

}

SIGNAL_HANDLER  int win_kp_cas_watchlevel_destroy(const char* widgetname, void *usrdata)
{
	return 0;
}

SIGNAL_HANDLER  int win_kp_cas_watchlevel_keypress(const char* widgetname, void *usrdata)
{
	GUI_Event *event = NULL;
	event = (GUI_Event *)usrdata;
	uint32_t ratevalue;
	uint8_t *value;
	char* str = NULL;
	ca_rating_t rating = {0};

	if(event->type == GUI_KEYDOWN)
	{
		switch(event->key.sym)
		{
			case KEY_EXIT:
				break;
				return EVENT_TRANSFER_STOP;
			case KEY_RECALL:
				 GUI_EndDialog("win_kp_cas_watchlevel");
				 return EVENT_TRANSFER_STOP;

			case KEY_OK:
				GUI_GetProperty("win_kp_cas_watchlevel_boxitem2_edit", "string", &value);
				GUI_GetProperty("win_kp_cas_watchlevel_boxitem1_value", "select", &ratevalue);
				rating.date_type = DVB_CA_RATING_INFO;
				rating.pin = (char*)value;
				rating.rate = ratevalue;
				str = app_cas_api_change_rating(&rating);
				if (str != NULL)
				{
					if (strcmp(str,"Set Successfully"))
					{
						GUI_SetProperty("win_kp_cas_watchlevel_boxitem2_edit","clear",NULL);
					}
					app_popmsg(POP_DIALOG_X_START, POP_DIALOG_Y_START,str,POPMSG_TYPE_OK);
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









