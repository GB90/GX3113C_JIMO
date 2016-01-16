/*****************************************************************************
* 						   CONFIDENTIAL
*        Hangzhou GuoXin Science and Technology Co., Ltd.
*                      (C)2012, All right reserved
******************************************************************************

******************************************************************************
* File Name :	win_dvt_cas_email.c
* Author    : 	zhouhm
* Project   :	goxceed dvbc
* Type      :
******************************************************************************
* Purpose   :	模块头文件
******************************************************************************
* Release History:
VERSION	Date			  AUTHOR         Description
1.0  	2013.12.02		  zhouhm 	 			creation
*****************************************************************************/
#include "app_common_play.h"
#include "app_common_porting_stb_api.h"
#include "app_win_interface.h"
#include "app_common_porting_ca_flash.h"
#include "app_dvt_cas_api_email.h"

uint32_t gMail_sel = 0;
static ca_get_count_t get_count = {0};


void app_dvt_cas_email_flag_img_show(uint8_t showFlag)
{
	switch(showFlag)
		{
			case DVB_CA_EMAIL_PROMTP_HIDE:
//				GUI_SetProperty("full_screen_img_mail","state","hide");
				GUI_SetProperty("full_screen_img_mail","state","osd_trans_hide");
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

SIGNAL_HANDLER  int app_dvt_cas_email_create(const char* widgetname, void *usrdata)
{
	char	 config[10];
	memset(config, 0, 10);
	char* osd_language=NULL;   

	 osd_language = app_flash_get_config_osd_language();
	 if (0 == strcmp(osd_language,LANGUAGE_CHINESE))	
	{
		 GUI_SetProperty("win_dvt_cas_email_title","img","title_ch_Mail_Manage.bmp");  
		 GUI_SetProperty("win_dvt_cas_email_hint_exit", "img", "tips_exit.bmp");
		 GUI_SetProperty("win_dvt_cas_email_confirm_flag", "img", "tips_confirm.bmp");

	 }
	 else if(0 == strcmp(osd_language,LANGUAGE_ENGLISH))
	 {
		 GUI_SetProperty("win_dvt_cas_email_title","img","title_Mail_Manage_e.bmp");
		 GUI_SetProperty("win_dvt_cas_email_hint_exit","img","tips_exit_e.bmp");
		 GUI_SetProperty("win_dvt_cas_email_confirm_flag", "img", "tips_confirm_e2.bmp");
	 }

	sprintf(config, "%d", get_count.totalnum);
	GUI_SetProperty("win_dvt_cas_email_text_count", "string", (void *)config);
	sprintf(config, "%d", get_count.emptynum);
	GUI_SetProperty("win_dvt_cas_email_text_space_count", "string", (void *)config);
	return 0;
}

SIGNAL_HANDLER  int  app_dvt_cas_email_destroy(const char* widgetname, void *usrdata)
{
	return 0;
}

SIGNAL_HANDLER  int app_dvt_cas_email_keypress(const char* widgetname, void *usrdata)
{
	GUI_Event *event = NULL;
	uint32_t nListViewSel = 0;
	uint32_t deleteSel;
	ca_delete_data_t delete_data = {0};
	ca_get_date_t get_data = {0};
	char	 config[10];
	popmsg_ret ret;
    
	event = (GUI_Event *)usrdata;

	switch(event->type)
	{
	case GUI_MOUSEBUTTONDOWN:
		break;

	case GUI_KEYDOWN:
		switch(event->key.sym)
		{
		case KEY_RED:			
			GUI_GetProperty("win_dvt_cas_email_listview", "select", &nListViewSel);
			if (get_count.totalnum>0)
			{
				deleteSel = nListViewSel;
				if(nListViewSel >=  get_count.totalnum)
				{
					nListViewSel = get_count.totalnum - 1;
				}
                #ifdef APP_SD
                ret = app_popmsg(210,150,"Sure to delete this mail?",POPMSG_TYPE_YESNO);
				#endif
				#ifdef APP_HD
				ret = app_popmsg(340,200,"Sure to delete this mail?",POPMSG_TYPE_YESNO);		
				#endif
				if(POPMSG_RET_NO == ret)			
				{				
					return EVENT_TRANSFER_STOP; 	
				}

				delete_data.date_type = DVB_CA_EMAIL_INFO;
				delete_data.pos = deleteSel;
				if (0 == app_cas_api_delete(&delete_data))
				{
                    #ifdef APP_SD
                    app_popmsg(210, 150,"Mail beyond, can not delete",POPMSG_TYPE_OK);	
                    #endif
                    #ifdef APP_HD
                    app_popmsg(340, 200,"Mail beyond, can not delete",POPMSG_TYPE_OK);	
                    #endif
					
					return EVENT_TRANSFER_STOP;						
				}
				get_count.date_type = DVB_CA_EMAIL_INFO;
				app_cas_api_get_count(&get_count);
				GUI_SetProperty("win_dvt_cas_email_listview", "update_all", NULL);
				GUI_SetProperty("win_dvt_cas_email_listview","select",(void*)&nListViewSel);
				sprintf(config, "%d", get_count.totalnum);
				GUI_SetProperty("win_dvt_cas_email_text_count", "string", (void *)config);
				sprintf(config, "%d", get_count.emptynum);
				GUI_SetProperty("win_dvt_cas_email_text_space_count", "string", (void *)config);
			}
			else
			{
                #ifdef APP_SD
                app_popmsg(210, 150,"No mail",POPMSG_TYPE_OK);
                #endif
                #ifdef APP_HD
                app_popmsg(340, 200,"No mail",POPMSG_TYPE_OK);
                #endif
				
			}
			break;
		case KEY_BLUE:			
			if (get_count.totalnum >0)
			{
                int x,y;
                #ifdef APP_SD
                x = 210,y = 150;
                #endif
                #ifdef APP_HD
                x = 340,y = 200;
                #endif
				ret = app_popmsg(x, y,"Sure to delete all mails?",POPMSG_TYPE_YESNO);
				if(POPMSG_RET_NO == ret)			
				{				
					return EVENT_TRANSFER_STOP; 	
				}

				delete_data.date_type = DVB_CA_EMAIL_INFO;
				if (0 == app_cas_api_delete_all(&delete_data))
				{
					app_popmsg(x, y,"Mail beyond, can not delete",POPMSG_TYPE_OK);
					return EVENT_TRANSFER_STOP;						
				}
				get_count.date_type = DVB_CA_EMAIL_INFO;
				app_cas_api_get_count(&get_count);

				GUI_SetProperty("win_dvt_cas_email_listview", "update_all", NULL);
				sprintf(config, "%d", get_count.totalnum);
				GUI_SetProperty("win_dvt_cas_email_text_count", "string", (void *)config);
				sprintf(config, "%d", get_count.emptynum);
				GUI_SetProperty("win_dvt_cas_email_text_space_count", "string", (void *)config);
			}
			else
			{
                int x,y;
                #ifdef APP_SD
                x = 210,y = 150;
                #endif
                #ifdef APP_HD
                x = 340,y = 200;
                #endif
				app_popmsg(x, y,"No mail",POPMSG_TYPE_OK);
			}
			break;
		case KEY_MENU:
		case KEY_EXIT:
		case KEY_RECALL:
			GUI_EndDialog("win_dvt_cas_email");
			if (TRUE != app_get_win_create_flag(MAIN_MENU_WIN))
				{
					app_win_set_focus_video_window(FULL_SCREEN_WIN);
					app_play_reset_play_timer(0);			
				}
			return EVENT_TRANSFER_STOP;
		case KEY_F1:
			return EVENT_TRANSFER_STOP;
		case KEY_OK:
			GUI_GetProperty("win_dvt_cas_email_listview","select",(void*)&gMail_sel);
			if ( 0 == get_count.totalnum)
			{
				
                #ifdef APP_SD
                app_popmsg(210, 150,"No mail",POPMSG_TYPE_OK);
                #endif
                #ifdef APP_HD
                app_popmsg(340, 200,"No mail",POPMSG_TYPE_OK);
                #endif
			}
			else
			{	
				get_data.date_type = DVB_CA_EMAIL_INFO;
				get_data.pos = gMail_sel; // 行
				get_data.ID = DVT_EMAIL_CONTENT_ID;
				if (NULL != app_cas_api_get_data(&get_data))
					{
						GUI_EndDialog("win_dvt_cas_email");
						GUI_CreateDialog("win_dvt_cas_emailcontent");
					}
				else
					{
                        #ifdef APP_SD
                        app_popmsg(210, 150,"Access to information error",POPMSG_TYPE_OK);
                        #endif
                        #ifdef APP_HD
                        app_popmsg(340, 200,"Access to information error",POPMSG_TYPE_OK);
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


SIGNAL_HANDLER  int app_dvt_cas_email_get_count(const char* widgetname, void *usrdata)
{
	int use = 0;
	char	 strmailNum[10]={0};
	char	 stremptyNum[10]={0};

	get_count.date_type = DVB_CA_EMAIL_INFO;
	app_cas_api_get_count(&get_count);


	sprintf(strmailNum, "%d", get_count.totalnum);
	GUI_SetProperty("win_dvt_cas_email_text_count", "string", (void *)strmailNum);
	sprintf(stremptyNum, "%d", get_count.emptynum);
	GUI_SetProperty("win_dvt_cas_email_text_space_count", "string", (void *)stremptyNum);
	use = get_count.totalnum;
	return use;
}

SIGNAL_HANDLER  int app_dvt_cas_email_get_data(const char* widgetname, void *usrdata)
{
	ca_get_date_t get_data = {0};
    unsigned int      Row    = 0;
    unsigned int      ID    = 0;
	static uint8_t pArry1[5] = {0};


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

	sprintf((char *)pArry1,"%3d",item_para->sel+1);
	item_para->string = (char *)pArry1;

	item_para= item_para->next;


	Row = item_para->sel;

	get_data.date_type = DVB_CA_EMAIL_INFO;
	get_data.pos = Row; // 行

	for (ID = DVT_EMAIL_STATE_ID;ID<=DVT_EMAIL_DESPATCHER_ID;ID++)
		{
			get_data.ID = ID;
			item_para->string = app_cas_api_get_data(&get_data);

			if (ID < DVT_EMAIL_DESPATCHER_ID)
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

SIGNAL_HANDLER  int app_dvt_cas_email_content_create(const char* widgetname, void *usrdata)
{
	ca_get_date_t get_data = {0};
	char* str = NULL;

	char* osd_language=NULL;   

	 osd_language = app_flash_get_config_osd_language();
	 if (0 == strcmp(osd_language,LANGUAGE_CHINESE))	
	{
	//	 GUI_SetProperty("win_dvt_cas_emailcontent_image_exit", "img", "tips_exit.bmp");

	 }
	 else if(0 == strcmp(osd_language,LANGUAGE_ENGLISH))
	 { 
	//	 GUI_SetProperty("win_dvt_cas_emailcontent_image_exit","img","tips_exit_e.bmp");
	 }


	get_data.date_type = DVB_CA_EMAIL_INFO;
	get_data.pos = gMail_sel;
	get_data.ID = DVT_EMAIL_TITLE_ID;
	str = app_cas_api_get_data(&get_data);
	if (NULL != str)
	{
		GUI_SetProperty("win_dvt_cas_emailcontent_emailtitle","string", str);
	}
	get_data.ID = DVT_EMAIL_CONTENT_ID;
	str = app_cas_api_get_data(&get_data);
	if (NULL != str)
	{
		GUI_SetProperty("win_dvt_cas_emailcontent_content","string", str);
	}

	return 0;
}

SIGNAL_HANDLER  int  app_dvt_cas_email_content_destroy(const char* widgetname, void *usrdata)
{
	return 0;
}


SIGNAL_HANDLER  int app_dvt_cas_email_content_keypress(const char* widgetname, void *usrdata)
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
				case KEY_MENU:
				case KEY_RECALL: // zhou add at 20110407
					app_cas_api_init_data(DVB_CA_EMAIL_INFO);
					GUI_EndDialog("win_dvt_cas_emailcontent");
					GUI_CreateDialog("win_dvt_cas_email");
				 return EVENT_TRANSFER_STOP;
			 case KEY_UP:
				 GUI_SetProperty("win_dvt_cas_emailcontent_content", "page_up", NULL);
				 return EVENT_TRANSFER_STOP;
			 case KEY_DOWN:
				 GUI_SetProperty("win_dvt_cas_emailcontent_content", "page_down", NULL);
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










