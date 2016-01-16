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
#include "app_cd_cas_3.0_api_paired.h"


SIGNAL_HANDLER  int win_cd_cas30_paried_create(const char* widgetname, void *usrdata)
{

	ca_get_date_t get_data = {0};
	char* str = NULL;
	uint32_t i = 0;
	unsigned int				  chStbIdIndex = 0;
	char* osd_language=NULL;
    osd_language = app_flash_get_config_osd_language();
	if (0 == strcmp(osd_language,LANGUAGE_CHINESE))
    {
 //   	GUI_SetProperty("win_cd_cas30_paried_tip_image_exit", "img", "tips_exit.bmp");
		//GUI_SetProperty("win_cd_cas30_pin_tip_image_ok", "img", "tips_confirm.bmp");
    }
    else if(0 == strcmp(osd_language,LANGUAGE_ENGLISH))
     {
    //	GUI_SetProperty("win_cd_cas30_paried_tip_image_exit", "img", "tips_exit_e.bmp");
		//GUI_SetProperty("win_cd_cas30_pin_tip_image_ok", "img", "tips_confirm_e2.bmp");
    }


	get_data.date_type = DVB_CA_PAIRED_INFO;	
	get_data.ID = CD_PAIRED_INFO_ID;
	str = app_cas_api_get_data(&get_data);
	if (NULL !=str )
	{
            GUI_SetProperty("win_cd_cas30_paried_info_text", "string",str);			
	}

	for (i =CD_PAIRED_STBID_LIST1_ID; i<=CD_PAIRED_STBID_LIST5_ID;i++ )
	{
		get_data.date_type = DVB_CA_PAIRED_INFO;	
		get_data.ID = i;
		str = app_cas_api_get_data(&get_data);
		if (NULL !=str )
		{
			switch(chStbIdIndex)
			{
				case 0:
					GUI_SetProperty("win_cd_cas30_paried_stbid_list1", "string",str);
					break;
				case 1:
					GUI_SetProperty("win_cd_cas30_paried_stbid_list2", "string",str);
					break;
				case 2:
					GUI_SetProperty("win_cd_cas30_paried_stbid_list3", "string",str);								
					break;
				case 3:
					GUI_SetProperty("win_cd_cas30_paried_stbid_list4", "string",str);								
					break;
				case 4:
					GUI_SetProperty("win_cd_cas30_paried_stbid_list5", "string",str);								
					break;
				default:
					break;
			}			
			chStbIdIndex++;
		}			
	}

	if (chStbIdIndex > 0)
	{
		GUI_SetProperty("win_cd_cas30_paried_stbid_text", "string","STB ID List:");											
	}
	
	return 0;
}

SIGNAL_HANDLER  int win_cd_cas30_paried_destroy(const char* widgetname, void *usrdata)
{
	return 0;
}

SIGNAL_HANDLER  int win_cd_cas30_paried_keypress(const char* widgetname, void *usrdata)
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
				GUI_EndDialog("win_cd_cas30_paried");
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

