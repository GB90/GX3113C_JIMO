/*****************************************************************************
* 						   CONFIDENTIAL
*        Hangzhou GuoXin Science and Technology Co., Ltd.
*                      (C)2012, All right reserved
******************************************************************************

******************************************************************************
* File Name :	win_dvt_cas_rating.c
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
#include "app_dvt_cas_api_rating.h"
#include "DVTCAS_STBInf.h"

SIGNAL_HANDLER  int win_dvt_cas_rating_create(const char* widgetname, void *usrdata)
{
	uint32_t rating = 0;
	ca_get_date_t get_data = {0};
	char* str = NULL;

	 char* osd_language=NULL;	
	
	 osd_language = app_flash_get_config_osd_language();
	 if (0 == strcmp(osd_language,LANGUAGE_CHINESE))	
	{
	//	 GUI_SetProperty("win_dvt_cas_rating_title","img","title_ch_AgeLock.bmp");  
	//	 GUI_SetProperty("win_dvt_cas_rating_tip_image_exit", "img", "tips_exit.bmp");
	//	 GUI_SetProperty("win_dvt_cas_rating_tip_image_ok", "img", "tips_confirm.bmp");

	 }
	 else if(0 == strcmp(osd_language,LANGUAGE_ENGLISH))
	 { 
	//	 GUI_SetProperty("win_dvt_cas_rating_title","img","title_AgeLock_e.bmp");
	//	 GUI_SetProperty("win_dvt_cas_rating_tip_image_exit","img","tips_exit_e.bmp");
	//	 GUI_SetProperty("win_dvt_cas_rating_tip_image_ok", "img", "tips_confirm_e2.bmp");
	 }

	
	get_data.date_type = DVB_CA_RATING_INFO;	
	get_data.ID = DVT_RATING_LEVEL_ID;
	str = app_cas_api_get_data(&get_data);
	if (NULL !=str )
		{
			rating = atoi(str);
			GUI_SetProperty("win_dvt_cas_rating_boxitem2_value", "select", &rating);			
		}

	return 0;

}

SIGNAL_HANDLER  int win_dvt_cas_rating_destroy(const char* widgetname, void *usrdata)
{
	return 0;
}

SIGNAL_HANDLER  int win_dvt_cas_rating_keypress(const char* widgetname, void *usrdata)
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
				 GUI_EndDialog("win_dvt_cas_rating");
				 return EVENT_TRANSFER_STOP;
			case KEY_POWER:
				return EVENT_TRANSFER_KEEPON;
			default:
				return EVENT_TRANSFER_STOP;
		}
	}

	return EVENT_TRANSFER_KEEPON;
}

SIGNAL_HANDLER  int win_dvt_cas_rating_box_keypress(const char* widgetname, void *usrdata)
{
	GUI_Event *event = NULL;
	uint32_t ratevalue;
	uint8_t *value;
	char* str = NULL;
	ca_rating_t rating = {0};
//	uint32_t nSelect;

	event = (GUI_Event *)usrdata;

	if(event->type == GUI_KEYDOWN)
	{
		switch(event->key.sym)
		{
		case KEY_MENU:
			event->key.sym = KEY_EXIT;
			return EVENT_TRANSFER_KEEPON;
			break;
		case KEY_DOWN:
			GUI_GetProperty("win_dvt_cas_rating_box", "select", &value);
			if(value == 0)
			{
				GUI_GetProperty("win_dvt_cas_rating_boxitem1_edit", "string", &value);
				if(strlen((char*)value)!=DVTCA_MAXLEN_PINCODE)
				{
                    #ifdef APP_SD
    				app_popmsg(210,150,"Input 8-bit PIN",POPMSG_TYPE_OK);
    				#endif
    				#ifdef APP_HD
    				app_popmsg(340,200,"Input 8-bit PIN",POPMSG_TYPE_OK);
    				#endif
                    return EVENT_TRANSFER_STOP; 
				}
			}
			break;

		case KEY_OK:
			/*????ƥ??ԭʼ????*/
			GUI_GetProperty("win_dvt_cas_rating_boxitem1_edit", "string", &value);
			GUI_GetProperty("win_dvt_cas_rating_boxitem2_value", "select", &ratevalue);
			rating.date_type = DVB_CA_RATING_INFO;
			rating.pin = (char*)value;
			rating.rate = ratevalue;
			str = app_cas_api_change_rating(&rating);
			if (NULL != str)
			{
				printf("str = %s \n",str);
                #ifdef APP_SD
                app_popmsg(210, 150,str,POPMSG_TYPE_OK);
				#endif
				#ifdef APP_HD
				app_popmsg(340, 200,str,POPMSG_TYPE_OK);
				#endif
				
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








