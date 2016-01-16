/*****************************************************************************
* 						   CONFIDENTIAL
*        Hangzhou GuoXin Science and Technology Co., Ltd.
*                      (C)2012, All right reserved
******************************************************************************

******************************************************************************
* File Name :	win_tr_cas_announce_list.c
* Author    : 	wangjian
* Project   :	goxceed dvbc
* Type      :
******************************************************************************
* Purpose   :	模块头文件
******************************************************************************
* Release History:
VERSION	Date			  AUTHOR         Description
1.0  	2015.3.26		  wangjian 	   creation
*****************************************************************************/

#include "app_common_porting_stb_api.h"
#include "app_win_interface.h"
#include "app_tr_cas_api_announce.h"

static ca_get_count_t s_announceGetCount = {0};	

#if 0
void win_tr_cas_announce_list_update(void)
{
    char *focus_Window = (char *)GUI_GetFocusWindow();
	
    if ((NULL != focus_Window) && (0 == strcasecmp("win_tr_cas_announce_list", focus_Window)))
    {
        char config[10];
		
        s_announceGetCount.date_type = DVB_CA_CURTAIN_INFO;
		app_cas_api_get_count(&s_announceGetCount);
		GUI_SetProperty("win_tr_cas_announce_list_listview", "update_all", NULL);
		sprintf(config, "%d", s_announceGetCount.totalnum);
		GUI_SetProperty("win_tr_cas_announce_list_text_count", "string", (void *)config);
    }
}
#endif

SIGNAL_HANDLER  int win_tr_cas_announce_list_create(const char* widgetname, void *usrdata)
{
	char config[10];
	char* osd_language = NULL;

	memset(config, 0, 10);
    osd_language = app_flash_get_config_osd_language();
	if (0 == strcmp(osd_language, LANGUAGE_CHINESE))
    {
   // 	GUI_SetProperty("win_tr_cas_announce_list_hint_exit", "img", "tips_exit.bmp");
//		GUI_SetProperty("win_tr_cas_announce_list_confirm_flag", "img", "tips_confirm.bmp");
    }
    else if (0 == strcmp(osd_language, LANGUAGE_ENGLISH))
     {
   // 	GUI_SetProperty("win_tr_cas_announce_list_hint_exit", "img", "tips_exit_e.bmp");
//		GUI_SetProperty("win_tr_cas_announce_list_confirm_flag", "img", "tips_confirm_e2.bmp");
    }

	memset(&s_announceGetCount, 0, sizeof(s_announceGetCount));
	s_announceGetCount.date_type = DVB_CA_CURTAIN_INFO;
	app_cas_api_get_count(&s_announceGetCount);
		
	sprintf(config, "%d", s_announceGetCount.totalnum);
	GUI_SetProperty("win_tr_cas_announce_list_text_count", "string", (void *)config);
	sprintf(config, "%d", s_announceGetCount.newnum);
	GUI_SetProperty("win_tr_cas_announce_list_text_new_count", "string", (void *)config);
	sprintf(config, "%d", ANNOUNCE_NUM_MAX);
	GUI_SetProperty("win_tr_cas_announce_list_text_max_count", "string", (void *)config);
	
    return 0;
}

SIGNAL_HANDLER  int win_tr_cas_announce_list_destroy(const char* widgetname, void *usrdata)
{
	return 0;
}

SIGNAL_HANDLER  int win_tr_cas_announce_list_listview_key_press(const char* widgetname, void *usrdata)
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

SIGNAL_HANDLER  int win_tr_cas_announce_list_keypress(const char* widgetname, void *usrdata)
{
	GUI_Event *event = NULL;
	uint32_t nListViewSel = 0;
	uint32_t deleteSel;
	ca_delete_data_t delete_data = {0};
	ca_get_date_t get_data = {0};
	char config[10] = {0};
	uint32_t gMail_sel = 0;
	char strNewNum[10] = {0};

	event = (GUI_Event *)usrdata;
	switch (event->type)
	{
	case GUI_MOUSEBUTTONDOWN:
		break;

	case GUI_KEYDOWN:
		switch (event->key.sym)
		{
		case KEY_RED://删除单封公告
			GUI_GetProperty("win_tr_cas_announce_list_listview", "select", &nListViewSel);
			if (s_announceGetCount.totalnum > 0)
			{
				deleteSel = nListViewSel;
				if (nListViewSel >= (s_announceGetCount.totalnum - 1))
				{
					nListViewSel--;
				}
				
				delete_data.date_type = DVB_CA_CURTAIN_INFO;
				delete_data.pos = deleteSel;
				app_cas_api_delete(&delete_data);
				s_announceGetCount.date_type = DVB_CA_CURTAIN_INFO;
				app_cas_api_get_count(&s_announceGetCount);
				GUI_SetProperty("win_tr_cas_announce_list_listview", "update_all", NULL);
				GUI_SetProperty("win_tr_cas_announce_list_listview","select",(void*)&nListViewSel);
				sprintf(config, "%d", s_announceGetCount.totalnum);
				GUI_SetProperty("win_tr_cas_announce_list_text_count", "string", (void *)config);
				sprintf(config, "%d", s_announceGetCount.newnum);
				GUI_SetProperty("win_tr_cas_announce_list_text_new_count", "string", (void *)config);
			}
			else
			{
				app_popmsg(POP_DIALOG_X_START, POP_DIALOG_Y_START,"No Announce",POPMSG_TYPE_OK);
			}
			break;
			
		case KEY_BLUE://删除全部邮件
			if (s_announceGetCount.totalnum > 0)
			{
				delete_data.date_type = DVB_CA_CURTAIN_INFO;
				app_cas_api_delete_all(&delete_data);
                
				s_announceGetCount.date_type = DVB_CA_CURTAIN_INFO;
				app_cas_api_get_count(&s_announceGetCount);

				GUI_SetProperty("win_tr_cas_announce_list_listview", "update_all", NULL);
				sprintf(config, "%d", s_announceGetCount.totalnum);
				GUI_SetProperty("win_tr_cas_announce_list_text_count", "string", (void *)config);
				sprintf(config, "%d", s_announceGetCount.newnum);
				GUI_SetProperty("win_tr_cas_announce_list_text_new_count", "string", (void *)config);
			}
			else
			{
				app_popmsg(POP_DIALOG_X_START, POP_DIALOG_Y_START,"No Announce",POPMSG_TYPE_OK);
			}
			break;
			
		case KEY_MENU:
		case KEY_EXIT:
		case KEY_RECALL:
			GUI_EndDialog("win_tr_cas_announce_list");
			return EVENT_TRANSFER_STOP;
		
		case KEY_OK:
			GUI_GetProperty("win_tr_cas_announce_list_listview","select",(void*)&gMail_sel);
			if ( 0 == s_announceGetCount.totalnum)
			{
				app_popmsg(POP_DIALOG_X_START, POP_DIALOG_Y_START, "No Announce", POPMSG_TYPE_OK);
			}
			else
			{	
				get_data.date_type = DVB_CA_CURTAIN_INFO;
				get_data.pos = gMail_sel; // 行
				get_data.ID = TR_ANNOUNCE_CONTENT_ID;
				if (NULL != app_cas_api_get_data(&get_data))
				{
					s_announceGetCount.date_type = DVB_CA_CURTAIN_INFO;
					app_cas_api_get_count(&s_announceGetCount);
					sprintf(strNewNum, "%d", s_announceGetCount.newnum);
					GUI_SetProperty("win_tr_cas_announce_list_text_new_count", "string", (void *)strNewNum);
					
					GUI_CreateDialog("win_tr_cas_announce_content");
				}
				else
				{
					app_popmsg(POP_DIALOG_X_START, POP_DIALOG_Y_START, "No Announce", POPMSG_TYPE_OK);
				}
			}
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

SIGNAL_HANDLER  int win_tr_cas_announce_list_get_count(const char* widgetname, void *usrdata)
{
	int use = 0;
	char strTotalNum[10] = {0};
	char strNewNum[10] = {0};

	s_announceGetCount.date_type = DVB_CA_CURTAIN_INFO;
	app_cas_api_get_count(&s_announceGetCount);

	sprintf(strTotalNum, "%d", s_announceGetCount.totalnum);
	GUI_SetProperty("win_tr_cas_announce_list_text_count", "string", (void *)strTotalNum);
	sprintf(strNewNum, "%d", s_announceGetCount.newnum);
	GUI_SetProperty("win_tr_cas_announce_list_text_new_count", "string", (void *)strNewNum);
	use = s_announceGetCount.totalnum;
	
	return use;
}

SIGNAL_HANDLER  int win_tr_cas_announce_list_get_data(const char* widgetname, void *usrdata)
{
    ca_get_date_t get_data = {0};
    unsigned int Row = 0;
    unsigned int i = 0;
	ListItemPara *item_para = NULL;

	if ((NULL == widgetname) || (NULL == usrdata))
	{
		return (1);
	}
	
	item_para = (ListItemPara *)(usrdata);
	if (NULL == item_para) 
	{
		return GXCORE_ERROR;
	}

	Row = item_para->sel;
	get_data.date_type = DVB_CA_CURTAIN_INFO;
	get_data.pos = Row;

	for (i = 0; i < 4; i++)
	{
		if (i == 0)
		{
			get_data.ID = TR_ANNOUNCE_FLAG_ID;
		}
		else if (i == 1)
		{
			get_data.ID = TR_ANNOUNCE_TITLE_ID;
		}
		else if (i == 2)
		{
			get_data.ID = TR_ANNOUNCE_SENDTIME_ID;
		}
		else if (i == 3)
		{
			get_data.ID = TR_ANNOUNCE_LEVEL_ID;
		}

		item_para->string = app_cas_api_get_data(&get_data);
		//printf("[win_tr_cas_announce_list_get_data]string<%s>---###\n", item_para->string);

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

SIGNAL_HANDLER  int win_tr_cas_announce_content_create(const char* widgetname, void *usrdata)
{
	ca_get_date_t get_data = {0};
	char* str = NULL;
	uint32_t nListViewSel = 0;
	
	GUI_GetProperty("win_tr_cas_announce_list_listview", "select", &nListViewSel);		

	get_data.date_type = DVB_CA_CURTAIN_INFO;
	get_data.pos = nListViewSel;
	get_data.ID = TR_ANNOUNCE_TITLE_ID;
	str = app_cas_api_get_data(&get_data);
	if (NULL != str)
	{
		GUI_SetProperty("win_tr_cas_announce_content_title","string", str);
	}
	
	get_data.ID = TR_ANNOUNCE_CONTENT_ID;
	str = app_cas_api_get_data(&get_data);
	if (NULL != str)
	{
		GUI_SetProperty("win_tr_cas_announce_content_content","string", str);
	}

	return 0;
}

SIGNAL_HANDLER  int  win_tr_cas_announce_content_destroy(const char* widgetname, void *usrdata)
{
	return 0;
}

SIGNAL_HANDLER  int win_tr_cas_announce_content_keypress(const char* widgetname, void *usrdata)
{
	GUI_Event *event = NULL;
	
	event = (GUI_Event *)usrdata;
	
	switch (event->type)
	{
		case GUI_MOUSEBUTTONDOWN:
			break;
			
		case GUI_KEYDOWN:
			switch (event->key.sym)
			{
				case KEY_EXIT:
				case KEY_MENU:
					GUI_EndDialog("win_tr_cas_announce_content");
				    return EVENT_TRANSFER_STOP;
					
                case KEY_UP:
					GUI_SetProperty("win_tr_cas_announce_content_content", "page_up", NULL);
					return EVENT_TRANSFER_STOP;
					
				case KEY_DOWN:
					GUI_SetProperty("win_tr_cas_announce_content_content", "page_down", NULL);
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


