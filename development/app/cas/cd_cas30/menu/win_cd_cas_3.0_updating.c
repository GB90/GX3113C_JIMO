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
#include "app_cd_cas_3.0_api_pop.h"


u_int8_t create_flag = FALSE;
void win_cd_cas30_cardupdating_message_show(u_int8_t byProgress,u_int8_t byMark)
{

	int32_t BarTmp=0;
	int8_t percentStr[10] = {0};
	BarTmp = byProgress;

	if (byProgress > 100)
	{
		/*进度值大于100，关闭或隐藏进度条显示*/
		GUI_EndDialog("win_cd_cas30_updating");
	}
	else
	{
		if (FALSE == create_flag)
		{
			GUI_CreateDialog("win_cd_cas30_updating");
		}			

		if (2 == byMark)
		{
			GUI_SetProperty("win_cd_cas30_updating_mark", "string", "Writing to smart card");
		}
		else if (1 == byMark)
		{
			GUI_SetProperty("win_cd_cas30_updating_mark", "string", "Receiving data");	
		}

		GUI_SetProperty("win_cd_cas30_updating_progbar", "value", (void*)&BarTmp);	
		sprintf((char *)percentStr,"%d%%",BarTmp);
		GUI_SetProperty("win_cd_cas30_updating_text_percent", "string", (void*)percentStr);	
		if ((100 == byProgress)&&(2 == byMark))
		{	
			GUI_SetProperty("win_cd_cas30_updating_mark", "string", "Card Update Success"); 
		}
	}
}

SIGNAL_HANDLER	int win_cd_cas30_updating_create(const char* widgetname, void *usrdata)
{
	int32_t BarTmp=0;
	int8_t percentStr[10] = {0};
	create_flag = TRUE;
	GUI_SetProperty("win_cd_cas30_updating_progbar", "value", (void*)&BarTmp);					
	sprintf((char *)percentStr,"%d%%",BarTmp);
	GUI_SetProperty("win_cd_cas30_updating_text_percent", "string", (void*)percentStr); 

	return 0;
}

SIGNAL_HANDLER	int win_cd_cas30_updating_destroy(const char* widgetname, void *usrdata)
{
	create_flag = FALSE;
	return 0;	
}

SIGNAL_HANDLER	int win_cd_cas30_updating_keypress(const char* widgetname, void *usrdata)
{
	GUI_Event *event = NULL;
	event = (GUI_Event *)usrdata;
	 
	if(event->type == GUI_KEYDOWN)
	{	
		switch(event->key.sym)
		{
			case KEY_EXIT:
				return EVENT_TRANSFER_KEEPON;	
			case KEY_RECALL:
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



