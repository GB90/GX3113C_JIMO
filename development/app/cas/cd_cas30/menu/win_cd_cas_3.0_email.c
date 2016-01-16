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
* Purpose   :	ģ��ͷ�ļ�
******************************************************************************
* Release History:
VERSION	Date			  AUTHOR         Description
1.0  	2012.12.02		  zhouhm 	 			creation
*****************************************************************************/
#include "app_common_play.h"
#include "app_common_porting_stb_api.h"
#include "app_win_interface.h"
#include "win_cd_cas_3.0_email.h"
#include "app_common_porting_ca_flash.h"
#include "app_cd_cas_3.0_api_pop.h"
#include "app_cd_cas_3.0_api_email.h"

static uint32_t gMail_sel = 0;
static ca_get_count_t get_count = {0};

void app_cd_cas30_email_flag_img_show(uint8_t showFlag)
{
	switch(showFlag)
		{
			case DVB_CA_EMAIL_PROMTP_HIDE:
				GUI_SetProperty("full_screen_img_mail","state","hide");
				break;
			case DVB_CA_EMAIL_PROMTP_SHOW:
//				GUI_SetProperty("full_screen_img_mail","img","Static.bmp");
				GUI_SetProperty("full_screen_img_mail","state","show");
//				GUI_SetProperty("full_screen_img_mail", "draw_now", NULL);				
				break;
			case DVB_CA_EMAIL_PROMTP_TRANS_HIDE:
				GUI_SetProperty("full_screen_img_mail","state","osd_trans_hide");
//				GUI_SetProperty("full_screen_img_mail", "draw_now", NULL);				
				break;
			default:
				break;
		}
}

SIGNAL_HANDLER  int app_cd_cas30_email_create(const char* widgetname, void *usrdata)
{
	char	 config[10];

	char* osd_language=NULL;
    osd_language = app_flash_get_config_osd_language();
	if (0 == strcmp(osd_language,LANGUAGE_CHINESE))
    {
 //   	GUI_SetProperty("win_cd_cas30_email_hint_exit", "img", "tips_exit.bmp");
//		GUI_SetProperty("win_cd_cas30_email_confirm_flag", "img", "tips_confirm.bmp");
    }
    else if(0 == strcmp(osd_language,LANGUAGE_ENGLISH))
     {
   // 	GUI_SetProperty("win_cd_cas30_email_hint_exit", "img", "tips_exit_e.bmp");
//		GUI_SetProperty("win_cd_cas30_email_confirm_flag", "img", "tips_confirm_e2.bmp");
    }

	
	memset(config, 0, 10);
	sprintf(config, "%d", get_count.totalnum);
	GUI_SetProperty("win_cd_cas30_email_text_count", "string", (void *)config);
	sprintf(config, "%d", get_count.emptynum);
	GUI_SetProperty("win_cd_cas30_email_text_space_count", "string", (void *)config);
	return 0;
}

SIGNAL_HANDLER  int  app_cd_cas30_email_destroy(const char* widgetname, void *usrdata)
{
	return 0;
}


SIGNAL_HANDLER  int app_cd_cas30_email_keypress(const char* widgetname, void *usrdata)
{
	GUI_Event *event = NULL;
	uint32_t nListViewSel = 0;
	uint32_t deleteSel;
	ca_delete_data_t delete_data = {0};
	ca_get_date_t get_data = {0};
	char	 config[10];
	event = (GUI_Event *)usrdata;

	switch(event->type)
	{
	case GUI_MOUSEBUTTONDOWN:
		break;

	case GUI_KEYDOWN:
		switch(event->key.sym)
		{
		case KEY_RED:
			GUI_GetProperty("win_cd_cas30_email_listview", "select", &nListViewSel);
			if (get_count.totalnum>0)
			{
				deleteSel = nListViewSel;
				if(nListViewSel >=  get_count.totalnum)
				{
					nListViewSel = get_count.totalnum - 1;
				}
				delete_data.date_type = DVB_CA_EMAIL_INFO;
				delete_data.pos = deleteSel;
				app_cas_api_delete(&delete_data);
				
				//app_popmsg(340, 200,"No mail",POPMSG_TYPE_OK);
				
				get_count.date_type = DVB_CA_EMAIL_INFO;
				app_cas_api_get_count(&get_count);
				GUI_SetProperty("win_cd_cas30_email_listview", "update_all", NULL);
				GUI_SetProperty("win_cd_cas30_email_listview","select",(void*)&nListViewSel);
				sprintf(config, "%d", get_count.totalnum);
				GUI_SetProperty("win_cd_cas30_email_text_count", "string", (void *)config);
				sprintf(config, "%d", get_count.emptynum);
				GUI_SetProperty("win_cd_cas30_email_text_space_count", "string", (void *)config);
			}
			else
			{
				#ifdef APP_SD
				app_popmsg(210,150,"No mail",POPMSG_TYPE_OK);
				#endif
				#ifdef APP_HD
				app_popmsg(340, 200,"No mail",POPMSG_TYPE_OK);
				#endif
			}
			break;
		case KEY_BLUE:
			if (get_count.totalnum >0)
			{
				delete_data.date_type = DVB_CA_EMAIL_INFO;
				app_cas_api_delete_all(&delete_data);
				app_popmsg(210, 200,"No mail",POPMSG_TYPE_OK);
				get_count.date_type = DVB_CA_EMAIL_INFO;
				app_cas_api_get_count(&get_count);

				GUI_SetProperty("win_cd_cas30_email_listview", "update_all", NULL);
				sprintf(config, "%d", get_count.totalnum);
				GUI_SetProperty("win_cd_cas30_email_text_count", "string", (void *)config);
				sprintf(config, "%d", get_count.emptynum);
				GUI_SetProperty("win_cd_cas30_email_text_space_count", "string", (void *)config);
			}
			else
			{
				#ifdef APP_SD
				app_popmsg(210,150,"No mail",POPMSG_TYPE_OK);
				#endif
				#ifdef APP_HD
				app_popmsg(340, 200,"No mail",POPMSG_TYPE_OK);
				#endif
			}
			break;
		case KEY_MENU:
		case KEY_EXIT:
		case KEY_RECALL:
			GUI_EndDialog("win_cd_cas30_email");
			return EVENT_TRANSFER_STOP;
		case KEY_F1:
			return EVENT_TRANSFER_STOP;
		case KEY_OK:
			GUI_GetProperty("win_cd_cas30_email_listview","select",(void*)&gMail_sel);
			if ( 0 == get_count.totalnum)
			{
				#ifdef APP_SD
				app_popmsg(210,150,"No mail",POPMSG_TYPE_OK);
				#endif
				#ifdef APP_HD
				app_popmsg(340, 200,"No mail",POPMSG_TYPE_OK);
				#endif
			}
			else
			{	
				get_data.date_type = DVB_CA_EMAIL_INFO;
				get_data.pos = gMail_sel; // ��
				get_data.ID = CD_EMAIL_CONTENT;
				if (NULL != app_cas_api_get_data(&get_data))
				{
					GUI_CreateDialog("win_cd_cas30_email_content");
				}
				else
				{
#ifdef APP_SD
						app_popmsg(210,150,"No mail",POPMSG_TYPE_OK);
#endif
						#ifdef APP_HD
					app_popmsg(340, 200,"No mail",POPMSG_TYPE_OK);
						#endif
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


SIGNAL_HANDLER  int app_cd_cas30_email_get_count(const char* widgetname, void *usrdata)
{
	int use = 0;
	char	 strmailNum[10]={0};
	char	 stremptyNum[10]={0};

	get_count.date_type = DVB_CA_EMAIL_INFO;
	app_cas_api_get_count(&get_count);


	sprintf(strmailNum, "%d", get_count.totalnum);
	GUI_SetProperty("win_cd_cas30_email_text_count", "string", (void *)strmailNum);
	sprintf(stremptyNum, "%d", get_count.emptynum);
	GUI_SetProperty("win_cd_cas30_email_text_space_count", "string", (void *)stremptyNum);
	use = get_count.totalnum;
	return use;
}

SIGNAL_HANDLER  int app_cd_cas30_email_get_data(const char* widgetname, void *usrdata)
{
	ca_get_date_t get_data = {0};
    unsigned int      Row    = 0;
    unsigned int      ID    = 0;
	static uint8_t indexStr[5];

	ListItemPara *item_para =NULL;	
	if(NULL == widgetname || NULL == usrdata)	
	{		
		return (1);	
	}	
	item_para = (ListItemPara *)(usrdata);

	if(item_para->sel>=get_count.totalnum)
	{
		return (1);
	}

	Row = item_para->sel;
	sprintf((char *)indexStr,"%d",Row+1);
	item_para->string = (char *)indexStr;
	item_para= item_para->next;

	get_data.date_type = DVB_CA_EMAIL_INFO;
	get_data.pos = Row; // ��

	for (ID = CD_EMAIL_IS_NEW_ID;ID<=CD_EMAIL_IMPORTANCE;ID++)
		{
			get_data.ID = ID;
			item_para->string = app_cas_api_get_data(&get_data);

			if (ID < CD_EMAIL_IMPORTANCE)
				{
					/*
					* �������һ��
					*/
					item_para= item_para->next;
					if(NULL == item_para) 
						return GXCORE_ERROR;
				}
		}

	return 0;
}



SIGNAL_HANDLER  int win_cd_cas30_email_content_create(const char* widgetname, void *usrdata)
{
	ca_get_date_t get_data = {0};
	char* str = NULL;
	uint32_t nListViewSel = 0;

	char* osd_language=NULL;
    osd_language = app_flash_get_config_osd_language();
	if (0 == strcmp(osd_language,LANGUAGE_CHINESE))
    {
 //   	GUI_SetProperty("win_cd_cas30_email_content_tip_image_exit", "img", "tips_exit.bmp");
		//GUI_SetProperty("win_cd_cas30_feed_tip_image_ok", "img", "tips_confirm.bmp");
    }
    else if(0 == strcmp(osd_language,LANGUAGE_ENGLISH))
     {
    //	GUI_SetProperty("win_cd_cas30_email_content_tip_image_exit", "img", "tips_exit_e.bmp");
		//GUI_SetProperty("win_cd_cas30_feed_tip_image_ok", "img", "tips_confirm_e2.bmp");
    }

	
	GUI_GetProperty("win_cd_cas30_email_listview", "select", &nListViewSel);		

	get_data.date_type = DVB_CA_EMAIL_INFO;
	get_data.pos = nListViewSel; // ��
	get_data.ID = CD_EMAIL_HEAD_ID;
	str = app_cas_api_get_data(&get_data);
	if (NULL != str)
	{
		GUI_SetProperty("win_cd_cas30_email_content_head","string", str);
	}
	get_data.ID = CD_EMAIL_CONTENT;
	str = app_cas_api_get_data(&get_data);
	if (NULL != str)
	{
		GUI_SetProperty("win_cd_cas30_email_content_content","string", str);
	}

	return 0;
}

SIGNAL_HANDLER  int  win_cd_cas30_email_content_destroy(const char* widgetname, void *usrdata)
{
	return 0;
}


SIGNAL_HANDLER  int win_cd_cas30_email_content_keypress(const char* widgetname, void *usrdata)
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
				case KEY_RECALL: // zhou add at 20110407
					get_count.date_type = DVB_CA_EMAIL_INFO;
	    			app_cas_api_get_count(&get_count);
					GUI_EndDialog("win_cd_cas30_email_content");
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




