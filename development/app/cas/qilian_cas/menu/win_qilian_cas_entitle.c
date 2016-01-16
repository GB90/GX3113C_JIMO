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
#include "app_qilian_cas_api_entitle.h"

static ca_get_count_t get_count = {0};	


SIGNAL_HANDLER  int app_qilian_cas_entitle_create(const char* widgetname, void *usrdata)
{

	uint32_t pos = 0;
    char                  sTextBuffer[10];
	
    get_count.date_type = DVB_CA_ENTITLE_INFO;
    app_cas_api_get_count(&get_count);
	
    sprintf(sTextBuffer,"%d",get_count.totalnum);
    GUI_SetProperty("win_by_cas_entitle_text_entitle_count", "string", sTextBuffer);

	GUI_SetProperty("win_by_cas_entitle_listview","select",(void*)&pos);

	return 0;
}

SIGNAL_HANDLER  int app_qilian_cas_entitle_destroy(const char* widgetname, void *usrdata)
{
	return 0;
}


SIGNAL_HANDLER  int app_qilian_cas_entitle_listview_key_press(const char* widgetname, void *usrdata)
{
	GUI_Event *event = NULL;
	event = (GUI_Event *)usrdata;

	if(event->type == GUI_KEYDOWN)
	{
		switch(event->key.sym)
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

SIGNAL_HANDLER  int app_qilian_cas_entitle_keypress(const char* widgetname, void *usrdata)
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
				GUI_EndDialog("win_by_cas_entitle");
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

SIGNAL_HANDLER  int app_qilian_cas_entitle_get_count(const char* widgetname, void *usrdata)
{
    get_count.date_type = DVB_CA_ENTITLE_INFO;
    app_cas_api_get_count(&get_count);
	
    return get_count.totalnum;
}

SIGNAL_HANDLER  int app_qilian_cas_entitle_get_data(const char* widgetname, void *usrdata)
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
	if(NULL == item_para) 
		return GXCORE_ERROR;

	Row = item_para->sel;

	get_data.date_type = DVB_CA_ENTITLE_INFO;
	get_data.pos = Row; // 行


	for (ID = BY_ENTITLE_PRODUCT_ID;ID<=BY_ENTITLE_PRODUCT_ENDTIME_ID;ID++)
		{
			get_data.ID = ID;
			item_para->string = app_cas_api_get_data(&get_data);

			if (ID < BY_ENTITLE_PRODUCT_ENDTIME_ID)
				{
					/*
					* 不是最后一列
					*/
					item_para= item_para->next;
					if(NULL == item_para) 
						return GXCORE_ERROR;
				}
		}

	return 0;
}




