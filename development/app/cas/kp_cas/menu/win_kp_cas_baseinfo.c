#include "app_common_porting_stb_api.h"
#include "app_win_interface.h"
#include "app_kp_cas_api_baseinfo.h"

SIGNAL_HANDLER  int win_kp_cas_baseinfo_create(const char* widgetname, void *usrdata)
{
    
	ca_get_date_t get_data = {0};
	char* str = NULL;

    char* osd_language=NULL;
    osd_language = app_flash_get_config_osd_language();
    if (0 == strcmp(osd_language,LANGUAGE_CHINESE))
    {
 //   	GUI_SetProperty("win_kp_cas_baseinfo_tip_image_exit", "img", "tips_exit.bmp");
    }
    else if(0 == strcmp(osd_language,LANGUAGE_ENGLISH))
     {
    //	GUI_SetProperty("win_kp_cas_baseinfo_tip_image_exit", "img", "tips_exit_e.bmp");
    }

    //GUI_SetProperty("win_kp_cas_baseinfo_time_text", "string", app_win_get_local_date_string());
	
	get_data.date_type = DVB_CA_BASE_INFO;	
	get_data.ID = KP_BASEINFO_CA_ID;
	str = app_cas_api_get_data(&get_data);
	if (NULL !=str )
	{
        GUI_SetProperty("win_kp_cas_baseinfo_op0", "string",str);			
	}

	get_data.ID = KP_BASEINFO_VER_ID;
	str = app_cas_api_get_data(&get_data);
	if (NULL != str )
	{
        GUI_SetProperty("win_kp_cas_baseinfo_op1", "string",str);			
	}

    get_data.ID = KP_BASEINFO_LICENCE_ID;
	str = app_cas_api_get_data(&get_data);
	if (NULL != str )
	{
        GUI_SetProperty("win_kp_cas_baseinfo_op2", "string",str);			
	}
    get_data.ID = KP_BASEINFO_UNIQUE_ID;
	str = app_cas_api_get_data(&get_data);
	if (NULL != str )
	{
        GUI_SetProperty("win_kp_cas_baseinfo_op3", "string",str);			
	}
    get_data.ID = KP_BASEINFO_AREACODE_ID;
	str = app_cas_api_get_data(&get_data);
	if (NULL != str )
	{
        GUI_SetProperty("win_kp_cas_baseinfo_op4", "string",str);			
	}
    get_data.ID = KP_BASEINFO_ATTIBUTE_ID;
	str = app_cas_api_get_data(&get_data);
	if (NULL != str )
	{
        GUI_SetProperty("win_kp_cas_baseinfo_op5", "string",str);			
	}
	return 0;
}

SIGNAL_HANDLER  int win_kp_cas_baseinfo_destroy(const char* widgetname, void *usrdata)
{
	return 0;
}

SIGNAL_HANDLER  int win_kp_cas_baseinfo_keypress(const char* widgetname, void *usrdata)
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
				GUI_EndDialog("win_kp_cas_baseinfo");
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

