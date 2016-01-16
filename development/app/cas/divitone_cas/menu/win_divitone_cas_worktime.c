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
#include "app_divitone_cas_api_worktime.h"
#include "DiviGuardCa.h"
extern UINT8 Repeattime;

SIGNAL_HANDLER  int win_divitone_cas_worktime_create(const char* widgetname, void *usrdata)
{
	ca_get_date_t get_data = {0};
	char* str = NULL;
	
	get_data.date_type = DVB_CA_WORK_TIME_INFO;	
	get_data.ID = DIVI_WORKTIME_START_ID;
	str = app_cas_api_get_data(&get_data);
	if (NULL !=str )
		{
                GUI_SetProperty("win_divitone_cas_worktime_boxitem1_edit", "string",str);			
		}

	get_data.date_type = DVB_CA_WORK_TIME_INFO;	
	get_data.ID = DIVI_WORKTIME_END_ID;
	str = app_cas_api_get_data(&get_data);
	if (NULL !=str )
		{	
            GUI_SetProperty("win_divitone_cas_worktime_boxitem2_edit", "string",str);			
		}

	return 0;

}

SIGNAL_HANDLER  int win_divitone_cas_worktime_destroy(const char* widgetname, void *usrdata)
{
	return 0;
}

SIGNAL_HANDLER  int win_divitone_cas_worktime_keypress(const char* widgetname, void *usrdata)
{
	GUI_Event *event = NULL;
	event = (GUI_Event *)usrdata;
    	char *pin;
    	char *starttime;
    	char *endtime;
	char* str = NULL;
	ca_work_time_t worktime = {0};

	char buf[70];

	if(event->type == GUI_KEYDOWN)
	{
		switch(event->key.sym)
		{
			case KEY_EXIT:
			case KEY_MENU:
			case KEY_RECALL:
				 GUI_EndDialog("win_divitone_cas_worktime");
				 return EVENT_TRANSFER_STOP;

			case KEY_LEFT:
			case KEY_RIGHT:
				return EVENT_TRANSFER_STOP;
			case KEY_OK:
				GUI_GetProperty("win_divitone_cas_worktime_boxitem3_edit", "string", &pin);
           		GUI_GetProperty("win_divitone_cas_worktime_boxitem1_edit", "string", &starttime);
				GUI_GetProperty("win_divitone_cas_worktime_boxitem2_edit", "string", &endtime);

				worktime.date_type = DVB_CA_WORK_TIME_INFO;
				worktime.pin = (char*)pin;
				worktime.starttime = starttime;
				worktime.endtime = endtime;
				
				str = app_cas_api_change_worktime(&worktime);
				if (str != NULL)
				{
					if (0 == strcmp(str,"Input new PIN"))
					{
						GUI_SetProperty("win_divitone_cas_worktime_boxitem3_edit","clear",NULL);
					}
					if (0 == strcmp(str,"PIN Error!"))
					{
						if(Repeattime == 17)
							snprintf(buf,70,"Card Locked!");
						else
							snprintf(buf,70,"PIN码错误，你还可以试 %d 次",Repeattime);
						GUI_SetProperty("win_divitone_cas_worktime_boxitem3_edit","clear",NULL);
						app_popmsg(210, 200,buf,POPMSG_TYPE_OK);
						return EVENT_TRANSFER_STOP;
					}
					app_popmsg(210, 200,str,POPMSG_TYPE_OK);
				}

				return EVENT_TRANSFER_STOP;
			default:
				return EVENT_TRANSFER_KEEPON;
		}
	}

	return EVENT_TRANSFER_KEEPON;

}








