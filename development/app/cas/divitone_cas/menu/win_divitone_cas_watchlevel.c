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
#include "app_divitone_cas_api_watchlevel.h"
#include "DiviGuardCa.h"
extern UINT8 Repeattime;


SIGNAL_HANDLER  int win_divitone_cas_watchlevel_create(const char* widgetname, void *usrdata)
{
	uint32_t rating = 0;
	ca_get_date_t get_data = {0};
	char* str = NULL;
	
	get_data.date_type = DVB_CA_RATING_INFO;	
	get_data.ID = DIVI_RATING_LEVEL_ID;
	str = app_cas_api_get_data(&get_data);
	if (NULL !=str )
		{
			rating = atoi(str);
			
			GUI_SetProperty("win_divitone_cas_watchlevel_boxitem1_value", "select", &rating);			
		}

	return 0;

}

SIGNAL_HANDLER  int win_divitone_cas_watchlevel_destroy(const char* widgetname, void *usrdata)
{
	return 0;
}

SIGNAL_HANDLER  int win_divitone_cas_watchlevel_keypress(const char* widgetname, void *usrdata)
{
	GUI_Event *event = NULL;
	event = (GUI_Event *)usrdata;
	uint32_t ratevalue;
	uint8_t *value;
	char* str = NULL;
	ca_rating_t rating = {0};

	char buf[40];

	if(event->type == GUI_KEYDOWN)
	{
		switch(event->key.sym)
		{
			case KEY_EXIT:
			case KEY_RECALL:
			case KEY_MENU:	
		       		GUI_EndDialog("win_divitone_cas_watchlevel");
				 return EVENT_TRANSFER_STOP;

			case KEY_OK:
				GUI_GetProperty("win_divitone_cas_watchlevel_boxitem2_edit", "string", &value);
				GUI_GetProperty("win_divitone_cas_watchlevel_boxitem1_value", "select", &ratevalue);
				rating.date_type = DVB_CA_RATING_INFO;
				rating.pin = (char*)value;
				rating.rate = ratevalue;
				str = app_cas_api_change_rating(&rating);
				if (str != NULL)
					{
						if (0 == strcmp(str,"Input new PIN"))
							{
								GUI_SetProperty("win_divitone_cas_watchlevel_boxitem2_edit","clear",NULL);
							}
						if (0 == strcmp(str,"PIN Error!"))
							{
								if(Repeattime == 17)
									snprintf(buf,40,"Card Locked!");
								else
									snprintf(buf,40,"PIN码错误，你还可以试 %d 次",Repeattime);
								GUI_SetProperty("win_divitone_cas_watchlevel_boxitem2_edit","clear",NULL);
								app_popmsg(210, 200,buf,POPMSG_TYPE_OK);
								return EVENT_TRANSFER_STOP;
							}
						app_popmsg(210, 200,str,POPMSG_TYPE_OK);
					}
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









