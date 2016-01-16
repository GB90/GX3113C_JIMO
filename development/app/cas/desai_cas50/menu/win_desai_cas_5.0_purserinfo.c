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
#include "app_common_porting_ca_smart.h"
#include "app_win_interface.h"
#include "desai_ca_api.h"


SIGNAL_HANDLER  int app_desai_cas50_purser_info_create(const char* widgetname, void *usrdata)
{
	return 0;
}

SIGNAL_HANDLER  int app_desai_cas50_purser_info_destroy(const char* widgetname, void *usrdata)
{
	return 0;
}

SIGNAL_HANDLER  int app_desai_cas50_purser_info_keypress(const char* widgetname, void *usrdata)
{
	GUI_Event *event = NULL;
	event = (GUI_Event *)usrdata;

	switch(event->type)
	{
		case GUI_MOUSEBUTTONDOWN:
			break;

		case GUI_KEYDOWN:
			switch(event->key.sym)
			{
				case KEY_MENU:
				case KEY_EXIT:
				case KEY_RECALL:	
					GUI_EndDialog("win_desai_cas50_purser_info");
					return EVENT_TRANSFER_STOP;
				case KEY_F1:
					return EVENT_TRANSFER_STOP;
				case KEY_OK:
					return EVENT_TRANSFER_STOP;
				default:
					break;
			}
			break;
		default:
			break;
	}
	return EVENT_TRANSFER_KEEPON;
}

SIGNAL_HANDLER  int app_desai_cas50_purser_info_get_count(const char* widgetname, void *usrdata)
{
	if(app_porting_ca_smart_status() == TRUE)
		return 4;
	else
		return 0;		
}

SIGNAL_HANDLER  int app_desai_cas50_purser_info_get_data(const char* widgetname, void *usrdata)
{
	ListItemPara *item = NULL;
	item = (ListItemPara*)usrdata;
	static char id[10] = {0};
	static UINT8  pucPurser[32];

	if(NULL == item)
	{
		return EVENT_TRANSFER_KEEPON;
	}
	if(DSCA_GetPurserInfo(item->sel+1, pucPurser) ==CA_ERROR_OK)
	{
		memset(id,0,sizeof(char)*10);
		sprintf(id,"%d",item->sel+1);
		item->string = id;
		CAS_Dbg("%s\n",pucPurser);
		item = item->next;
		item->string = (char *)pucPurser;
	
	}
	else
		CAS_Dbg("==========DSCA_GetPurserInfo Error==========\n");
	return EVENT_TRANSFER_STOP;		
}

SIGNAL_HANDLER  int app_desai_cas50_purser_info_change(const char* widgetname, void *usrdata)
{
	return 0;
}