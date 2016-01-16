/*****************************************************************************
* 						   CONFIDENTIAL
*        Hangzhou GuoXin Science and Technology Co., Ltd.
*                      (C)2012, All right reserved
******************************************************************************

******************************************************************************
* File Name :	win_kp_cas_email.c
* Author    : 	chenth
* Project   :	goxceed dvbc
* Type      :
******************************************************************************
* Purpose   :	模块头文件
******************************************************************************
* Release History:
VERSION	Date			  AUTHOR         Description
1.0  	2014.4.21		  chenth 	 			creation
*****************************************************************************/

#include "app_common_porting_stb_api.h"
#include "app_win_interface.h"
#include "app_kp_cas_api_email.h"

static ca_get_count_t get_count = {0};	

void app_kp_cas_email_flag_img_show(uint8_t showFlag)
{
	switch(showFlag)
	{
	case DVB_CA_EMAIL_PROMTP_HIDE:
		GUI_SetProperty("full_screen_img_mail","state","hide");
		break;
	case DVB_CA_EMAIL_PROMTP_SHOW:
		GUI_SetProperty("full_screen_img_mail","state","show");
		break;
	case DVB_CA_EMAIL_PROMTP_TRANS_HIDE:
		GUI_SetProperty("full_screen_img_mail","state","osd_trans_hide");
		break;
	default:
		break;
	}
}

void app_kp_cas_email_list_update()
{
    char* focus_Window = (char*)GUI_GetFocusWindow();
    if(NULL != focus_Window && !strcasecmp("win_kp_cas_email", focus_Window))
    {
        char	 config[10];
        get_count.date_type = DVB_CA_EMAIL_INFO;
		app_cas_api_get_count(&get_count);
		GUI_SetProperty("win_kp_cas_email_listview", "update_all", NULL);
		sprintf(config, "%d", get_count.totalnum);
		GUI_SetProperty("win_kp_cas_email_text_count", "string", (void *)config);
		sprintf(config, "%d", get_count.newnum);
		GUI_SetProperty("win_kp_cas_email_text_new_count", "string", (void *)config);
    }
}

SIGNAL_HANDLER  int app_kp_cas_email_create(const char* widgetname, void *usrdata)
{

	char	 config[10];
	memset(config, 0, 10);

	printf("app_kp_cas_email_create get_count.totalnum=%d, get_count.newnum=%d\n",get_count.totalnum,get_count.newnum);
	sprintf(config, "%d", get_count.totalnum);
	GUI_SetProperty("win_kp_cas_email_text_count", "string", (void *)config);
	sprintf(config, "%d", get_count.newnum);
	GUI_SetProperty("win_kp_cas_email_text_new_count", "string", (void *)config);

    return 0;
}
SIGNAL_HANDLER  int app_kp_cas_email_destroy(const char* widgetname, void *usrdata)
{
	return 0;
}


SIGNAL_HANDLER  int app_kp_cas_email_listview_key_press(const char* widgetname, void *usrdata)
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

SIGNAL_HANDLER  int app_kp_cas_email_keypress(const char* widgetname, void *usrdata)
{

	GUI_Event *event = NULL;
	uint32_t nListViewSel = 0;
	uint32_t deleteSel;
	ca_delete_data_t delete_data = {0};
	ca_get_date_t get_data = {0};
	char	 config[10];
	uint32_t gMail_sel = 0;

	event = (GUI_Event *)usrdata;

	switch(event->type)
	{
	case GUI_MOUSEBUTTONDOWN:
		break;

	case GUI_KEYDOWN:
		switch(event->key.sym)
		{
		case KEY_RED://删除单封邮件
			GUI_GetProperty("win_kp_cas_email_listview", "select", &nListViewSel);
			if (get_count.totalnum>0)
			{
				deleteSel = nListViewSel;
				if(nListViewSel >=  get_count.totalnum - 1)
				{
					nListViewSel --;
				}
				delete_data.date_type = DVB_CA_EMAIL_INFO;
				delete_data.pos = deleteSel;
				app_cas_api_delete(&delete_data);
				get_count.date_type = DVB_CA_EMAIL_INFO;
				app_cas_api_get_count(&get_count);
				GUI_SetProperty("win_kp_cas_email_listview", "update_all", NULL);
				GUI_SetProperty("win_kp_cas_email_listview","select",(void*)&nListViewSel);
				sprintf(config, "%d", get_count.totalnum);
				GUI_SetProperty("win_kp_cas_email_text_count", "string", (void *)config);
				sprintf(config, "%d", get_count.newnum);
				GUI_SetProperty("win_kp_cas_email_text_new_count", "string", (void *)config);
			}
			else
			{
				app_popmsg(POP_DIALOG_X_START, POP_DIALOG_Y_START,"No mail",POPMSG_TYPE_OK);
			}
			break;
		case KEY_BLUE://删除全部邮件
			if (get_count.totalnum >0)
			{
				delete_data.date_type = DVB_CA_EMAIL_INFO;
				app_cas_api_delete_all(&delete_data);
                
				get_count.date_type = DVB_CA_EMAIL_INFO;
				app_cas_api_get_count(&get_count);

				GUI_SetProperty("win_kp_cas_email_listview", "update_all", NULL);
				sprintf(config, "%d", get_count.totalnum);
				GUI_SetProperty("win_kp_cas_email_text_count", "string", (void *)config);
				sprintf(config, "%d", get_count.newnum);
				GUI_SetProperty("win_kp_cas_email_text_new_count", "string", (void *)config);
			}
			else
			{
				app_popmsg(POP_DIALOG_X_START, POP_DIALOG_Y_START,"No mail",POPMSG_TYPE_OK);
			}
			break;
		case KEY_MENU:
		case KEY_EXIT:
		case KEY_RECALL:
			GUI_EndDialog("win_kp_cas_email");
			return EVENT_TRANSFER_STOP;
		case KEY_F1:
			return EVENT_TRANSFER_STOP;
		case KEY_OK:
            //被选中的那一行
			GUI_GetProperty("win_kp_cas_email_listview","select",(void*)&gMail_sel);
			if ( 0 == get_count.totalnum)
			{
				app_popmsg(POP_DIALOG_X_START, POP_DIALOG_Y_START,"No mail",POPMSG_TYPE_OK);
			}
			else
			{	
				get_data.date_type = DVB_CA_EMAIL_INFO;
				get_data.pos = gMail_sel; // 行
				get_data.ID = EMAIL_CONTENT_ID;
				if (NULL != app_cas_api_get_data(&get_data))
				{
					GUI_CreateDialog("win_kp_cas_email_content");
				}
				else
				{
					app_popmsg(POP_DIALOG_X_START, POP_DIALOG_Y_START,"No mail",POPMSG_TYPE_OK);
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


SIGNAL_HANDLER  int app_kp_cas_email_get_count(const char* widgetname, void *usrdata)
{

	int use = 0;
	char	 strmailNum[10]={0};
	char	 strnewNum[10]={0};

	get_count.date_type = DVB_CA_EMAIL_INFO;
	app_cas_api_get_count(&get_count);

	sprintf(strmailNum, "%d", get_count.totalnum);
	GUI_SetProperty("win_kp_cas_email_text_count", "string", (void *)strmailNum);
	sprintf(strnewNum, "%d", get_count.newnum);
	GUI_SetProperty("win_kp_cas_email_text_new_count", "string", (void *)strnewNum);
	use = get_count.totalnum;
	return use;
}

SIGNAL_HANDLER  int app_kp_cas_email_get_data(const char* widgetname, void *usrdata)
{
    ca_get_date_t get_data = {0};
    unsigned int      Row    = 0;
    unsigned int      i    = 0;
	ListItemPara *item_para =NULL;
	if(NULL == widgetname || NULL == usrdata )
	{
		return (1);
	}
	item_para = (ListItemPara *)(usrdata);
    
	if(NULL == item_para) 
		return GXCORE_ERROR;


	Row = item_para->sel;
	get_data.date_type = DVB_CA_EMAIL_INFO;
	get_data.pos = Row; // 行;

    for(i = 0;i<4;i++)
	{
      if(i==0)
		get_data.ID = EMAIL_FLAG_ID;
      else if(i==1)
        get_data.ID = EMAIL_SENDER_ID;
      else if(i==2)
        get_data.ID = EMAIL_TITLE_ID;
      else if(i==3)
        get_data.ID = EMAIL_SENDTIME_ID;
      
		item_para->string = app_cas_api_get_data(&get_data);

		if (i < 4)
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

SIGNAL_HANDLER  int win_kp_cas_email_content_create(const char* widgetname, void *usrdata)
{
	ca_get_date_t get_data = {0};
	char* str = NULL;
	uint32_t nListViewSel = 0;
	GUI_GetProperty("win_kp_cas_email_listview", "select", &nListViewSel);		

	get_data.date_type = DVB_CA_EMAIL_INFO;
	get_data.pos = nListViewSel; // 行
	get_data.ID = EMAIL_TITLE_ID;
	str = app_cas_api_get_data(&get_data);
	if (NULL != str)
	{
		GUI_SetProperty("win_kp_cas_email_content_head","string", str);
	}
	get_data.ID = EMAIL_CONTENT_ID;
	str = app_cas_api_get_data(&get_data);
	if (NULL != str)
	{
		GUI_SetProperty("win_kp_cas_email_content_content","string", str);
	}

	return 0;
}

SIGNAL_HANDLER  int  win_kp_cas_email_content_destroy(const char* widgetname, void *usrdata)
{
	return 0;
}


SIGNAL_HANDLER  int win_kp_cas_email_content_keypress(const char* widgetname, void *usrdata)
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
				case KEY_EXIT:
					break;
					return EVENT_TRANSFER_STOP;
				case KEY_MENU:
					break;
				case KEY_RECALL:
					get_count.date_type = DVB_CA_EMAIL_INFO;
	    			app_cas_api_get_count(&get_count);
					GUI_EndDialog("win_kp_cas_email_content");
				    return EVENT_TRANSFER_STOP;
                case KEY_UP:
					GUI_SetProperty("win_kp_cas_email_content_content", "page_up", NULL);
					return EVENT_TRANSFER_STOP;
				case KEY_DOWN:
					GUI_SetProperty("win_kp_cas_email_content_content", "page_down", NULL);
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


