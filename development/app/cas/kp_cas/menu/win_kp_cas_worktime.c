#include "app_common_porting_stb_api.h"
#include "app_win_interface.h"
#include "app_kp_cas_api_worktime.h"

SIGNAL_HANDLER  int win_kp_cas_worktime_create(const char* widgetname, void *usrdata)
{
	ca_get_date_t get_data = {0};
	char* str = NULL;
	
	get_data.date_type = DVB_CA_WORK_TIME_INFO;	
	get_data.ID = KP_WORKTIME_START_ID;
	str = app_cas_api_get_data(&get_data);
	if (NULL !=str )
	{
            GUI_SetProperty("win_kp_cas_worktime_boxitem1_edit", "string",str);			
	}

	get_data.date_type = DVB_CA_WORK_TIME_INFO;	
	get_data.ID = KP_WORKTIME_END_ID;
	str = app_cas_api_get_data(&get_data);
	if (NULL !=str )
	{	
        GUI_SetProperty("win_kp_cas_worktime_boxitem2_edit", "string",str);			
	}

	return 0;

}

SIGNAL_HANDLER  int win_kp_cas_worktime_destroy(const char* widgetname, void *usrdata)
{
	return 0;
}

SIGNAL_HANDLER  int win_kp_cas_worktime_keypress(const char* widgetname, void *usrdata)
{
	GUI_Event *event = NULL;
	event = (GUI_Event *)usrdata;
	char *pin;
	char *starttime;
	char *endtime;
	char* str = NULL;
	ca_work_time_t worktime = {0};

	if(event->type == GUI_KEYDOWN)
	{
		switch(event->key.sym)
		{
			case KEY_EXIT:
			case KEY_MENU:
				break;
				return EVENT_TRANSFER_STOP;
			case KEY_RECALL:
				 GUI_EndDialog("win_kp_cas_worktime");
				 return EVENT_TRANSFER_STOP;

			case KEY_LEFT:
			case KEY_RIGHT:
				return EVENT_TRANSFER_STOP;
			case KEY_OK:
				GUI_GetProperty("win_kp_cas_worktime_boxitem3_edit", "string", &pin);
           		GUI_GetProperty("win_kp_cas_worktime_boxitem1_edit", "string", &starttime);
				GUI_GetProperty("win_kp_cas_worktime_boxitem2_edit", "string", &endtime);

				worktime.date_type = DVB_CA_WORK_TIME_INFO;
				worktime.pin = (char*)pin;
				worktime.starttime = starttime;
				worktime.endtime = endtime;
				
				str = app_cas_api_change_worktime(&worktime);
				if (str != NULL)
				{
					if (strcmp(str,"Set Successfully"))
					{
						GUI_SetProperty("win_kp_cas_worktime_boxitem3_edit","clear",NULL);
					}
					app_popmsg(POP_DIALOG_X_START, POP_DIALOG_Y_START,str,POPMSG_TYPE_OK);
				}

				return EVENT_TRANSFER_STOP;
			default:
				return EVENT_TRANSFER_KEEPON;
		}
	}

	return EVENT_TRANSFER_KEEPON;

}


