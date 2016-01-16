/*****************************************************************************
* 						   CONFIDENTIAL
*        Hangzhou GuoXin Science and Technology Co., Ltd.
*                      (C)2012, All right reserved
******************************************************************************

******************************************************************************
* File Name :	win_tr_cas_entitle.c
* Author    : 	zhouhm
* Project   :	goxceed dvbc
* Type      :
******************************************************************************
* Purpose   :	模块头文件
******************************************************************************
* Release History:
VERSION	Date			  AUTHOR         Description
1.0  	2013.12.04		  zhouhm 	 			creation
*****************************************************************************/
#include "app_common_porting_stb_api.h"
#include "app_win_interface.h"
#include "app_tr_cas_api_entitle.h"

static ca_get_count_t s_getCasCount = {0};	


SIGNAL_HANDLER  int win_tr_cas_entitle_create(const char* widgetname, void *usrdata)
{
	char *osd_language = NULL;	
	
	osd_language = app_flash_get_config_osd_language();
	if (0 == strcmp(osd_language, LANGUAGE_CHINESE))	
	{
	//	GUI_SetProperty("win_tr_cas_entitle_title","img","title_ch_Authorization.bmp");  
	//	GUI_SetProperty("win_tr_cas_entitle_hint_exit", "img", "tips_exit.bmp");
	}
	else if(0 == strcmp(osd_language, LANGUAGE_ENGLISH))
	{ 
	//	GUI_SetProperty("win_tr_cas_entitle_title","img","title_Authorization_e.bmp");
	//	GUI_SetProperty("win_tr_cas_entitle_hint_exit","img","tips_exit_e.bmp");
	}
	
	return 0;
}

SIGNAL_HANDLER  int win_tr_cas_entitle_destroy(const char* widgetname, void *usrdata)
{
	return 0;
}

SIGNAL_HANDLER  int win_tr_cas_entitle_keypress(const char* widgetname, void *usrdata)
{
	GUI_Event *event = NULL;
	
	event = (GUI_Event *)usrdata;
	if (event->type == GUI_KEYDOWN)
	{
		switch (event->key.sym)
		{
			case KEY_EXIT:
				break;
				return EVENT_TRANSFER_STOP;
				
			case KEY_RECALL:
				GUI_EndDialog("win_tr_cas_entitle");
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

SIGNAL_HANDLER  int win_tr_cas_entitle_listview_key_press(const char* widgetname, void *usrdata)
{
	GUI_Event *event = NULL;
	
	event = (GUI_Event *)usrdata;
	if (event->type == GUI_KEYDOWN)
	{
		switch (event->key.sym)
		{
		case KEY_MENU:
			event->key.sym = KEY_EXIT;
			return EVENT_TRANSFER_KEEPON;
			break;
			
		case KEY_POWER:
			return EVENT_TRANSFER_KEEPON;
			
		default:
			return EVENT_TRANSFER_KEEPON;
		}
	}

	return EVENT_TRANSFER_KEEPON;
}

SIGNAL_HANDLER  int win_tr_cas_entitle_get_count(const char* widgetname, void *usrdata)
{
	int count = 0;
	char strCount[5] = {0};

	s_getCasCount.date_type = DVB_CA_ENTITLE_INFO;
    app_cas_api_get_count(&s_getCasCount);

	sprintf(strCount, "%d", s_getCasCount.totalnum);
	GUI_SetProperty("win_tr_cas_entitle_text_count", "string", (void *)strCount);
	count = s_getCasCount.totalnum;
	
	return count;
}

SIGNAL_HANDLER  int win_tr_cas_entitle_get_data(const char* widgetname, void *usrdata)
{
    ca_get_date_t get_data = {0};
    unsigned int Row = 0;
    unsigned int i = 0;
	ListItemPara *item_para = NULL;

	if (NULL == widgetname || NULL == usrdata)
	{
		return (1);
	}
	
	item_para = (ListItemPara *)(usrdata);
	if (NULL == item_para) 
	{
		return GXCORE_ERROR;
	}

	Row = item_para->sel;
	get_data.date_type = DVB_CA_ENTITLE_INFO;
	get_data.pos = Row;

	for (i = 0; i < 4; i++)
	{
		if (i == 0)
		{
			get_data.ID = TR_ENTITLE_INDEX_ID;
		}
		else if (i == 1)
		{
			get_data.ID = TR_ENTITLE_STATE_ID;
		}
		else if (i == 2)
		{
			get_data.ID = TR_ENTITLE_DATES_ID;
		}
		else if (i == 3)
		{
			get_data.ID = TR_ENTITLE_LEFTDAY_ID;
		}

		item_para->string = app_cas_api_get_data(&get_data);
		//printf("[win_tr_cas_entitle_get_data]string<%s>---###\n", item_para->string);

		if (i < 4)
		{
			item_para = item_para->next;
			if (NULL == item_para) 
			{
				return GXCORE_ERROR;
			}
		}
	}
	
	return 0;
}



