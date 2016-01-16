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
#include "app_common_play.h"
#include "app_common_porting_stb_api.h"
#include "app_win_interface.h"
#include "app_dvb_cas_api_email.h"
#include "app_dvb_cas_api_demux.h"
#include "win_dvb_cas_pop.h"
#include "win_dvb_cas_rolling_message.h"
#include "win_dvb_cas_finger.h"
#include "win_dvb_cas_email.h"
#include "app_dvb_cas_api_pop.h"

/* app_dvb_cas_show_ca_prompt与app_show_prompt实现功能有可能存在差别*/
extern void app_show_prompt(char* context);
extern void app_hide_prompt(void);
void app_dvb_cas_show_ca_prompt(char* context)
{
	app_show_prompt(context);
	return;
}

/*app_dvb_cas_hide_ca_prompt与app_hide_prompt实现功能有可能存在差别*/
void app_dvb_cas_hide_ca_prompt(void)
{
	app_hide_prompt();
	return;
}

/*紧急广播*/
bool g_win_popmsg_createflag = false;
static dvb_ca_urgent_message_st dvb_urgent_msg = {0};

//#define POP_DIALOG_X_START          210
//#define POP_DIALOG_Y_START          200
void win_dvb_cas_showDialog(unsigned char *title,unsigned char *content)
{
#ifdef APP_SD
	uint32_t x = 190;
	uint32_t y = 160;
#endif
#ifdef APP_HD

	uint32_t x = 340;
	uint32_t y = 200;
#endif
	if(content == NULL)
		return ;

	dvb_urgent_msg.title = title;
	dvb_urgent_msg.content = content;
	if(g_win_popmsg_createflag)
	{
		GUI_EndDialog("win_popmsg");
	}

	GUI_CreateDialog("win_popmsg");
	GUI_SetProperty("win_popmsg", "move_window_x", &x);
	GUI_SetProperty("win_popmsg", "move_window_y", &y);
	g_win_popmsg_createflag = true;
}

void win_dvb_cas_close_dialog()
{
	GUI_EndDialog("win_popmsg");
}

SIGNAL_HANDLER int win_popmsg_create(const char* widgetname, void *usrdata)
{
	if (dvb_urgent_msg.title != NULL)
	{
		GUI_SetProperty("win_popmsg_head", "string",(char *)dvb_urgent_msg.title);
	}
	if (dvb_urgent_msg.content != NULL)
	{
		GUI_SetProperty("win_popmsg_text", "string",(char *)dvb_urgent_msg.content);
	}
	return 0;
}

SIGNAL_HANDLER int win_popmsg_keypress(const char* widgetname, void *usrdata)
{
	GUI_Event *event = NULL;

	if(NULL == usrdata) return EVENT_TRANSFER_STOP;

	event = (GUI_Event *)usrdata;
	if(event->type == GUI_KEYDOWN)
	{
		switch(event->key.sym)
		{
			case KEY_EXIT:
			case KEY_MENU:
			case KEY_RECALL:
			case KEY_OK:
				GUI_EndDialog("win_popmsg");
				g_win_popmsg_createflag = false;
				return EVENT_TRANSFER_STOP;
			default:
				return EVENT_TRANSFER_KEEPON;
		}
	}
	return EVENT_TRANSFER_KEEPON;
}

