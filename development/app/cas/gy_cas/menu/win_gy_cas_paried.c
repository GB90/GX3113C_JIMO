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
* Purpose   :	模块头文件
******************************************************************************
* Release History:
VERSION	Date			  AUTHOR         Description
1.0  	2012.12.02		  zhouhm 	 			creation
*****************************************************************************/
#include "app_common_porting_stb_api.h"
#include "app_win_interface.h"
#include "app_gy_cas_api_paired.h"


SIGNAL_HANDLER  int win_gy_cas_paried_create(const char* widgetname, void *usrdata)
{

	ca_get_date_t get_data = {0};
	char* str = NULL;
	uint32_t i = 0;
	unsigned int				  chStbIdIndex = 0;


	get_data.date_type = DVB_CA_PAIRED_INFO;	
	get_data.ID = GY_PAIRED_INFO_ID;
	str = app_cas_api_get_data(&get_data);
	if (NULL !=str )
	{
        GUI_SetProperty("win_gy_cas_paried_info_text", "string",str);			
	}

	
	return 0;
}

SIGNAL_HANDLER  int win_gy_cas_paried_destroy(const char* widgetname, void *usrdata)
{
	return 0;
}


SIGNAL_HANDLER  int win_gy_cas_paried_keypress(const char* widgetname, void *usrdata)
{
	GUI_Event *event = NULL;
	uint8_t *value,*str;
	event = (GUI_Event *)usrdata;
	if(event->type == GUI_KEYDOWN)
	{
		switch(event->key.sym)
		{
			case KEY_EXIT:
				break;
				return EVENT_TRANSFER_STOP;
			case KEY_RECALL:
				GUI_EndDialog("win_gy_cas_paried");
				return EVENT_TRANSFER_STOP;
			case KEY_OK:
				GUI_GetProperty("win_gy_cas_paried_password", "string",&value);
				str = (unsigned char *)app_gy_cas_api_set_paired_data(value);

				if (NULL != str)
				{
					if(0 == strcmp(str,"Setting Paired Error, Please Check Pin"))
						GUI_SetProperty("win_gy_cas_paried_password","clear",NULL);
					
	 	   			app_popmsg(210, 200,str,POPMSG_TYPE_OK);
					return EVENT_TRANSFER_STOP;
				}

				return EVENT_TRANSFER_STOP;
				return EVENT_TRANSFER_STOP;
			case KEY_POWER:
				return EVENT_TRANSFER_KEEPON;
			default:
				return EVENT_TRANSFER_STOP;
		}
	}

	return EVENT_TRANSFER_KEEPON;
}

