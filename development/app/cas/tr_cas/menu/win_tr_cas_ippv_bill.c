/*****************************************************************************
* 						   CONFIDENTIAL
*        Hangzhou GuoXin Science and Technology Co., Ltd.
*                      (C)2012, All right reserved
******************************************************************************

******************************************************************************
* File Name :	app_tr_cas_ippv_bill.c
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
#include "app_tr_cas_api_ippv_bill.h"

static ca_get_count_t get_count = {0};	


SIGNAL_HANDLER  int win_tr_cas_ippv_bill_create(const char* widgetname, void *usrdata)
{
	uint32_t pos = 0;
	char* osd_language = NULL;
	
    osd_language = app_flash_get_config_osd_language();
	if (0 == strcmp(osd_language,LANGUAGE_CHINESE))
    {
 //   	GUI_SetProperty("win_tr_cas_ippv_bill_tip_image_exit", "img", "tips_exit.bmp");
		//GUI_SetProperty("win_tr_cas_pin_tip_image_ok", "img", "tips_confirm.bmp");
    }
    else if(0 == strcmp(osd_language,LANGUAGE_ENGLISH))
     {
    //	GUI_SetProperty("win_tr_cas_ippv_bill_tip_image_exit", "img", "tips_exit_e.bmp");
		//GUI_SetProperty("win_tr_cas_pin_tip_image_ok", "img", "tips_confirm_e2.bmp");
    }
	
	GUI_SetProperty("win_tr_cas_ippv_bill_listview","select",(void*)&pos);

	return 0;
}

SIGNAL_HANDLER  int win_tr_cas_ippv_bill_destroy(const char* widgetname, void *usrdata)
{
	return 0;
}

SIGNAL_HANDLER  int win_tr_cas_ippv_bill_listview_key_press(const char* widgetname, void *usrdata)
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

SIGNAL_HANDLER  int win_tr_cas_ippv_bill_keypress(const char* widgetname, void *usrdata)
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
				GUI_EndDialog("win_tr_cas_ippv_bill");
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

SIGNAL_HANDLER  int win_tr_cas_ippv_bill_get_count(const char* widgetname, void *usrdata)
{
	get_count.date_type = DVB_CA_DETITLE_INFO;
    app_cas_api_get_count(&get_count);
	
    return get_count.totalnum;
}

SIGNAL_HANDLER  int win_tr_cas_ippv_bill_get_data(const char* widgetname, void *usrdata)
{
	ca_get_date_t get_data = {0};
	unsigned int Row = 0;
	unsigned int ID = 0;
	ListItemPara *item_para = NULL;
	
	if ((NULL == widgetname) || (NULL == usrdata))
	{
		return (1);
	}
	
	item_para = (ListItemPara *)(usrdata);
	Row = item_para->sel;

	get_data.date_type = DVB_CA_DETITLE_INFO;
	get_data.pos = Row; // 行

	for (ID = TR_IPP_BILL_ID; ID <= TR_IPP_PRICE_ID; ID++)
	{
		get_data.ID = ID;
		item_para->string = app_cas_api_get_data(&get_data);

		if (ID < TR_IPP_PRICE_ID)
		{
			item_para= item_para->next;
			if (NULL == item_para) 
			{
				return GXCORE_ERROR;
			}
		}
	}

	return 0;
}


