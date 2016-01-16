#include "app_common_porting_stb_api.h"
#include "app_win_interface.h"
#include "app_tr_cas_api_baseinfo.h"


SIGNAL_HANDLER  int win_tr_cas_baseinfo_create(const char* widgetname, void *usrdata)
{	
	ca_get_date_t get_data = {0};
	char* str = NULL;
	
	get_data.date_type = DVB_CA_BASE_INFO;		
	get_data.ID = TR_BASEINFO_CAS_VER_ID;
	str = app_cas_api_get_data(&get_data);
	if (NULL != str)
	{
    	GUI_SetProperty("win_tr_cas_baseinfo_operator1", "string", str);			
	}
	
	get_data.date_type = DVB_CA_BASE_INFO;	
	get_data.ID = TR_BASEINFO_SERIAL_ID;
	str = app_cas_api_get_data(&get_data);
	if (NULL != str)
	{
    	GUI_SetProperty("win_tr_cas_baseinfo_operator3", "string", str);			
	}
	
	get_data.date_type = DVB_CA_BASE_INFO;	
	get_data.ID = TR_BASEINFO_COS_VER_ID;
	str = app_cas_api_get_data(&get_data);
	if (NULL != str)
	{
    	GUI_SetProperty("win_tr_cas_baseinfo_operator2", "string", str);			
	}
	
	get_data.date_type = DVB_CA_BASE_INFO;	
	get_data.ID = TR_BASEINFO_RATING_ID;
	str = app_cas_api_get_data(&get_data);
	if (NULL != str)
	{
    	GUI_SetProperty("win_tr_cas_baseinfo_operator4", "string", str);			
	}

	get_data.date_type = DVB_CA_BASE_INFO;	
	get_data.ID = TR_BASEINFO_AREA_ID;
	str = app_cas_api_get_data(&get_data);
	if (NULL != str)
	{
		GUI_SetProperty("win_tr_cas_baseinfo_operator5", "string", str);			
	}

	get_data.date_type = DVB_CA_BASE_INFO;	
	get_data.ID = TR_BASEINFO_MARRY_STATE;
	str = app_cas_api_get_data(&get_data);
	if (NULL != str)
	{
		GUI_SetProperty("win_tr_cas_baseinfo_operator6", "string", str);			
	}
	
	return 0;
}

SIGNAL_HANDLER  int win_tr_cas_baseinfo_destroy(const char* widgetname, void *usrdata)
{
	return 0;
}

SIGNAL_HANDLER  int win_tr_cas_baseinfo_keypress(const char* widgetname, void *usrdata)
{
	GUI_Event *event = NULL;
	
	event = (GUI_Event *)usrdata;
	if (event->type == GUI_KEYDOWN)
	{
		switch(event->key.sym)
		{
			case KEY_EXIT:
				break;
				return EVENT_TRANSFER_STOP;
				
			case KEY_RECALL:
				GUI_EndDialog("win_tr_cas_baseinfo");
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

