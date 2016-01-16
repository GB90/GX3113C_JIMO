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
#include "app_cd_cas_3.0_api_features.h"

static ca_get_count_t get_count = {0};	
static event_list* spApp_Featuretime = NULL;

static  int featrue_timer(void *userdata)
{
	uint32_t value;


	/*如果焦点不在当前界面，不响应TIMER，避免界面错乱*/
	if( strcasecmp("win_cd_cas30_features", GUI_GetFocusWindow()) != 0)
		{
			return 0;
		}
	
	GUI_GetProperty("win_cd_cas30_features_listview","select",(void*)&value);
	app_cd_cas30_api_init_features_data();
    get_count.date_type = DVB_CA_FETURE_INFO;
    app_cas_api_get_count(&get_count);

	if (0 == get_count.totalnum)
		{
	        GUI_SetProperty("win_cd_cas30_features_text_id1", "string", " ");
	        GUI_SetProperty("win_cd_cas30_features_text_id2", "string", " ");			
	        GUI_SetProperty("win_cd_cas30_features_text_id3", "string", " ");			
		}
	
	GUI_SetProperty("win_cd_cas30_features_listview","update_all",NULL);							
	GUI_SetProperty("win_cd_cas30_features_listview","select",(void*)&value);

	return 0;
}

SIGNAL_HANDLER  int win_cd_cas30_features_create(const char* widgetname, void *usrdata)
{
	ca_get_date_t get_data = {0};
	char* str = NULL;
	char* osd_language=NULL;
    osd_language = app_flash_get_config_osd_language();
	if (0 == strcmp(osd_language,LANGUAGE_CHINESE))
    {
 //   	GUI_SetProperty("win_cd_cas30_features_tip_image_exit", "img", "tips_exit.bmp");
		//GUI_SetProperty("win_cd_cas30_feed_tip_image_ok", "img", "tips_confirm.bmp");
    }
    else if(0 == strcmp(osd_language,LANGUAGE_ENGLISH))
     {
    //	GUI_SetProperty("win_cd_cas30_features_tip_image_exit", "img", "tips_exit_e.bmp");
		//GUI_SetProperty("win_cd_cas30_feed_tip_image_ok", "img", "tips_confirm_e2.bmp");
    }

	
	app_cd_cas30_api_init_features_data();

    get_count.date_type = DVB_CA_FETURE_INFO;
    app_cas_api_get_count(&get_count);

	if (get_count.totalnum)
		{
			get_data.date_type = DVB_CA_FETURE_INFO;
			get_data.pos = 0; // 行	
			get_data.ID = CD_FEATURES_ID1_ID;
			str = app_cas_api_get_data(&get_data);
			if (NULL !=str )
				{
					GUI_SetProperty("win_cd_cas30_features_text_id1", "string", str);							
				}

			get_data.ID = CD_FEATURES_ID2_ID;
			str = app_cas_api_get_data(&get_data);
			if (NULL !=str )
				{
					GUI_SetProperty("win_cd_cas30_features_text_id2", "string", str);							
				}
			
			get_data.ID = CD_FEATURES_ID3_ID;
			str = app_cas_api_get_data(&get_data);
			if (NULL !=str )
				{
					GUI_SetProperty("win_cd_cas30_features_text_id3", "string", str);							
				}
		}

	spApp_Featuretime = create_timer(featrue_timer, 2000, NULL,  TIMER_REPEAT);

	return 0;
}

SIGNAL_HANDLER  int win_cd_cas30_features_destroy(const char* widgetname, void *usrdata)
{
	remove_timer(spApp_Featuretime);
	spApp_Featuretime = NULL;

	return 0;
}
SIGNAL_HANDLER  int win_cd_cas30_features_change(const char* widgetname, void *usrdata)
{
	ca_get_date_t get_data = {0};
	uint32_t listview_sel;
	char* str = NULL;

	GUI_GetProperty("win_cd_cas30_features_listview","select",(void*)&listview_sel);
	get_data.date_type = DVB_CA_FETURE_INFO;
	get_data.pos = listview_sel; // 行 
	get_data.ID = CD_FEATURES_ID1_ID;
	str = app_cas_api_get_data(&get_data);
	if (NULL !=str )
		{
			GUI_SetProperty("win_cd_cas30_features_text_id1", "string", str);							
		}
	
	get_data.ID = CD_FEATURES_ID2_ID;
	str = app_cas_api_get_data(&get_data);
	if (NULL !=str )
		{
			GUI_SetProperty("win_cd_cas30_features_text_id2", "string", str);							
		}
	get_data.ID = CD_FEATURES_ID3_ID;
	str = app_cas_api_get_data(&get_data);
	if (NULL !=str )
		{
			GUI_SetProperty("win_cd_cas30_features_text_id3", "string", str);							
		}

	return 0;
}

SIGNAL_HANDLER  int win_cd_cas30_features_keypress(const char* widgetname, void *usrdata)
{
	GUI_Event *event = NULL;
	event = (GUI_Event *)usrdata;

	if(event->type == GUI_KEYDOWN)
	{
		switch(event->key.sym)
		{
		case KEY_EXIT:
			GUI_EndDialog("win_cd_cas30_features");
			return EVENT_TRANSFER_KEEPON;
			break;
	    case KEY_RECALL:
				 GUI_EndDialog("win_cd_cas30_features");
				 return EVENT_TRANSFER_STOP;
		default:
			break;
		}
	}
	return EVENT_TRANSFER_STOP;
}

SIGNAL_HANDLER  int win_cd_cas30_features_get_count(const char* widgetname, void *usrdata)
{
    get_count.date_type = DVB_CA_FETURE_INFO;
    app_cas_api_get_count(&get_count);

    return get_count.totalnum;
}

SIGNAL_HANDLER  int win_cd_cas30_features_get_data(const char* widgetname, void *usrdata)
{
	ca_get_date_t get_data = {0};
   unsigned int      Row    = 0;
   unsigned int      ID    = 0;

	ListItemPara *item_para =NULL;
	if(NULL == widgetname || NULL == usrdata )
	{
		return (1);
	}
	item_para = (ListItemPara *)(usrdata);

	Row = item_para->sel;

	get_data.date_type = DVB_CA_FETURE_INFO;
	get_data.pos = Row; // 行

	for (ID = CD_FEATURES_TVS_ID;ID<=CD_FEATURES_BOUQUETID_ID;ID++)
		{
			get_data.ID = ID;
			item_para->string = app_cas_api_get_data(&get_data);

			if (ID < CD_FEATURES_BOUQUETID_ID)
				{
					/*
					* 不是最后一列
					*/
					item_para= item_para->next;
				}
		}

	return 0;
}


