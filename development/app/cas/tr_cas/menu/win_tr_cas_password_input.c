/*****************************************************************************
* 						   CONFIDENTIAL
*        Hangzhou GuoXin Science and Technology Co., Ltd.
*                      (C)2010, All right reserved
******************************************************************************

******************************************************************************
* File Name :	win_tr_cas_password_input.c
* Author    : 	zhouhm
* Project   :	goxceed dvbc
* Type      :
******************************************************************************
* Purpose   :	ģ��ͷ�ļ�
******************************************************************************
* Release History:
VERSION	Date			  AUTHOR         Description
1.0  	2012.11.16		  zhouhm 	 			creation
*****************************************************************************/
#include "app.h"
#include "Tr_Cas.h"


static event_list* swin_passowrd_time = NULL;
static int rech_end_ok_flag1 = 0;


void	app_tr_cas_passwd_check(void)
{
	if (FULL_SCREEN_WIN == app_win_get_focus_video_window())
	{	
		GUI_EndDialog("win_tr_cas_ipp_buy");
		GUI_CreateDialog("win_tr_cas_password_input");
	}
}

static  int timer(void *userdata)
{
	if (1 == rech_end_ok_flag1)
	{
		GUI_EndDialog("win_tr_cas_password_input");
		if (NULL != swin_passowrd_time)
		{
			remove_timer(swin_passowrd_time);
			swin_passowrd_time = NULL;			
		}
	}
	
	return 0;
}

SIGNAL_HANDLER  int win_tr_cas_password_input_create(const char* widgetname, void *usrdata)
{
	int sApp_list_prog_Sel = 0;
	rech_end_ok_flag1 = 0;

	if (PROGRAM_TV_LIST_WIN == app_win_get_focus_video_window() ) 
	{
		GUI_GetProperty("win_tv_prog_listview","select",(void*)&sApp_list_prog_Sel);
		GUI_SetProperty("win_tv_prog_listview","active",&sApp_list_prog_Sel);
	}
	else if(WEEKLY_EPG_WIN == app_win_get_focus_video_window()) 
	{
		GUI_GetProperty("win_epg_prog_list_listview","select",(void*)&sApp_list_prog_Sel);
		GUI_SetProperty("win_epg_prog_list_listview","active",&sApp_list_prog_Sel);
	}
	else if(FAVORITE_LIST_WIN == app_win_get_focus_video_window()) 
	{
		GUI_GetProperty("win_favorite_prog_listview","select",(void*)&sApp_list_prog_Sel);
		GUI_SetProperty("win_favorite_prog_listview","active",&sApp_list_prog_Sel);	
	}					

	swin_passowrd_time = create_timer(timer, 200, NULL, TIMER_REPEAT);

	return 0;
}

SIGNAL_HANDLER  int win_tr_cas_password_input_destroy(const char* widgetname, void *usrdata)
{
	int sApp_list_prog_Sel = -1;

	if (NULL != swin_passowrd_time)
	{
		remove_timer(swin_passowrd_time);
		swin_passowrd_time = NULL;			
	}
	
	rech_end_ok_flag1 = 0;

	if (PROGRAM_TV_LIST_WIN == app_win_get_focus_video_window() ) 
	{
		GUI_SetProperty("win_tv_prog_listview","active",&sApp_list_prog_Sel);
	}
 	else if (WEEKLY_EPG_WIN == app_win_get_focus_video_window()) 
	{
		GUI_SetProperty("win_epg_prog_list_listview","active",&sApp_list_prog_Sel);
	}
	else if (FAVORITE_LIST_WIN == app_win_get_focus_video_window()) 
	{
		GUI_SetProperty("win_favorite_prog_listview","active",&sApp_list_prog_Sel);	
	}
	
	return 0;
}


SIGNAL_HANDLER  int win_tr_cas_password_input_edit_keypress(const char* widgetname, void *usrdata)
{
	GUI_Event *event = NULL;
	
	event = (GUI_Event *)usrdata;
	if (event->type == GUI_KEYDOWN)
	{	
		switch (event->key.sym)
		{
			case KEY_LEFT:
			case KEY_RIGHT:
				return EVENT_TRANSFER_STOP;					
			default:
				break;	
		}
	}
	
	return EVENT_TRANSFER_KEEPON;
}

SIGNAL_HANDLER  int win_tr_cas_password_input_reach_end(const char* widgetname, void *usrdata)
{
	int32_t rtn = 0;
	char *value = NULL;
	uint8_t Password1[6+1] = {0};

	GUI_GetProperty("win_tr_cas_password_input_edit", "string", &value);
	if (value)
	{
		Password1[0] = value[0]-0x30;
		Password1[1] = value[1]-0x30;
		Password1[2] = value[2]-0x30;
		Password1[3] = value[3]-0x30;
		Password1[4] = value[4]-0x30;
		Password1[5] = value[5]-0x30;
	}
	//printf("\n win_tr_cas_password_input_edit value =%s \n",value);
	
	rtn = MC_SCARD_ParentalCtrlUnlock(Password1, 6);
	if (0 == rtn)
	{
		app_play_clear_ca_msg();
		rech_end_ok_flag1 = 1;
	}
	else
	{
		rech_end_ok_flag1 = 0;
		GUI_SetProperty("win_tr_cas_password_input_text895","string","Error,reput again!");
		GUI_SetProperty("win_tr_cas_password_input_edit","clear",NULL);
		return EVENT_TRANSFER_STOP;	
	}
	
	return 0;
}


SIGNAL_HANDLER  int win_tr_cas_password_input_keypress(const char* widgetname, void *usrdata)
{
	GUI_Event *event = NULL;
	int32_t rtn = 0;
	char *value = NULL;
	uint8_t Password1[6+1] = {0};

	event = (GUI_Event *)usrdata;
	if (event->type == GUI_KEYDOWN)
	{	
		switch (event->key.sym)
		{
			case KEY_EXIT:
				GUI_EndDialog("win_tr_cas_password_input");
				return EVENT_TRANSFER_STOP;	
			case KEY_LEFT:
			case KEY_RIGHT:
				return EVENT_TRANSFER_STOP;					
			case KEY_UP:
			case KEY_DOWN:
				if (PROGRAM_TV_LIST_WIN == app_win_get_focus_video_window()) 
				{
					GUI_EndDialog("win_tr_cas_password_input");
					GUI_SetFocusWidget("win_tv_prog_listview");
					GUI_SendEvent("win_tv_prog_listview", event);	
				}
				else if(WEEKLY_EPG_WIN == app_win_get_focus_video_window()) 
				{
					GUI_EndDialog("win_tr_cas_password_input");
					GUI_SetFocusWidget("win_epg_prog_list_listview");
					GUI_SendEvent("win_epg_prog_list_listview", event);	
				}
				else if(FAVORITE_LIST_WIN == app_win_get_focus_video_window()) 
				{
					GUI_EndDialog("win_tr_cas_password_input");
					GUI_SetFocusWidget("win_favorite_prog_listview");
					GUI_SendEvent("win_favorite_prog_listview", event);	
				}					
				else if (FULL_SCREEN_WIN == app_win_get_focus_video_window())
				{
					if (TRUE == app_get_win_create_flag(PROGRAM_BAR_WIN))
					{
						/*
						* ��Ϣ����ʾ״̬�£��������͵���Ϣ��
						*/
						GUI_EndDialog("win_tr_cas_password_input");
						GUI_SetInterface("flush", NULL);
						GUI_SendEvent("win_prog_bar", event);
					}
					else
					{
						GUI_EndDialog("win_tr_cas_password_input");
						GUI_SetInterface("flush", NULL);
						GUI_SendEvent("win_full_screen", event);
					}
				}					
				return EVENT_TRANSFER_STOP;					
			case KEY_MENU:
			case KEY_RECALL:
			case KEY_EPG:
			case KEY_TV_RADIO:
			case KEY_TV:
			case KEY_RADIO:
				if (FULL_SCREEN_WIN == app_win_get_focus_video_window())
				{
					GUI_EndDialog("win_tr_cas_password_input");
					GUI_EndDialog("win_prog_bar");
					GUI_SendEvent("win_full_screen", event);
				}	
				return EVENT_TRANSFER_STOP;		
			case KEY_OK:
				GUI_GetProperty("win_tr_cas_password_input_edit", "string", &value);
				if (value)
				{
					Password1[0] = value[0]-0x30;
					Password1[1] = value[1]-0x30;
					Password1[2] = value[2]-0x30;
					Password1[3] = value[3]-0x30;
					Password1[4] = value[4]-0x30;
					Password1[5] = value[5]-0x30;
				}
				//printf("\n win_tr_cas_password_input_edit value =%s \n",value);
				rtn = MC_SCARD_ParentalCtrlUnlock(Password1, 6);
				//rtn = app_win_check_password_valid("win_tr_cas_password_input_edit",MAX_PASSWD_LEN);
				if (0 == rtn)
				{
					GUI_EndDialog("win_tr_cas_password_input");
				}
				else
				{	
					GUI_SetProperty("win_tr_cas_password_input_text895","string","Error,reput again!");
					GUI_SetProperty("win_tr_cas_password_input_edit","clear",NULL);				
					return EVENT_TRANSFER_STOP;	
				}
				return EVENT_TRANSFER_STOP;	
			default:
				break;	
		}
	}
	
	return EVENT_TRANSFER_KEEPON;
}


