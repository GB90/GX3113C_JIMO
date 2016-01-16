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
* Purpose   :	Ä£¿éÍ·ÎÄ\u0152?******************************************************************************
* Release History:
VERSION	Date			  AUTHOR         Description
1.0  	2012.12.02		  zhouhm 	 			creation
*****************************************************************************/
#include "app_common_porting_stb_api.h"
#include "app_win_interface.h"
#include "app_cd_cas_3.0_api_rating.h"

SIGNAL_HANDLER  int win_cd_cas30_rating_create(const char* widgetname, void *usrdata)
{
	uint32_t rating = 0;
	ca_get_date_t get_data = {0};
	char* str = NULL;
	char* osd_language=NULL;
	

    osd_language = app_flash_get_config_osd_language();
	if (0 == strcmp(osd_language,LANGUAGE_CHINESE))
    {
 //   	GUI_SetProperty("win_cd_cas30_rating_tip_image_exit", "img", "tips_exit.bmp");
//		GUI_SetProperty("win_cd_cas30_rating_tip_image_ok", "img", "tips_confirm.bmp");
    }
    else if(0 == strcmp(osd_language,LANGUAGE_ENGLISH))
     {
   // 	GUI_SetProperty("win_cd_cas30_rating_tip_image_exit", "img", "tips_exit_e.bmp");
//		GUI_SetProperty("win_cd_cas30_rating_tip_image_ok", "img", "tips_confirm_e2.bmp");
    }
	get_data.date_type = DVB_CA_RATING_INFO;	
	get_data.ID = CD_RATING_LEVEL_ID;
	str = app_cas_api_get_data(&get_data);
	if (NULL !=str )
		{
			rating = atoi(str);
			GUI_SetProperty("win_cd_cas30_rating_boxitem1_value", "select", &rating);			
		}

	return 0;

}

SIGNAL_HANDLER  int win_cd_cas30_rating_destroy(const char* widgetname, void *usrdata)
{
	return 0;
}

SIGNAL_HANDLER  int win_cd_cas30_rating_keypress(const char* widgetname, void *usrdata)
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
				 GUI_EndDialog("win_cd_cas30_rating");
				 return EVENT_TRANSFER_STOP;

			case KEY_OK:
				GUI_GetProperty("win_cd_cas30_rating_boxitem2_edit", "string", &value);
				GUI_GetProperty("win_cd_cas30_rating_boxitem1_value", "select", &ratevalue);
				rating.date_type = DVB_CA_RATING_INFO;
				rating.pin = (char*)value;
				rating.rate = ratevalue;
				str = app_cas_api_change_rating(&rating);
				if (NULL != str)
					{
						printf("str = %s \n",str);
						#ifdef APP_SD
						app_popmsg(210,150,str,POPMSG_TYPE_OK);
						#endif
						#ifdef APP_HD
						app_popmsg(340, 200,str,POPMSG_TYPE_OK);
						#endif
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









