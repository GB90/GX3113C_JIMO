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
#include "app_cd_cas_3.0_api_mother_child.h"
#include "app_cd_cas_3.0_api_pop.h"

static ca_get_count_t get_count = {0};

void app_cd_cas30_feed_show(uint8_t showFlag,char* message)
{	
	if (DVB_CA_FEED_FLAG_HIDE == showFlag)
	{	
		GUI_SetProperty("win_full_screen_msg_5sec","state","hide");
	}
	else if (DVB_CA_FEED_FLAG_SHOW == showFlag)
	{
		GUI_SetProperty("win_full_screen_msg_5sec", "string", message);
		GUI_SetProperty("win_full_screen_msg_5sec","state","show");
	}
	return;
}

SIGNAL_HANDLER  int win_cd_cas30_feed_create(const char* widgetname, void *usrdata)
{
	int32_t  nListSel = 0;
	char* osd_language=NULL;
    osd_language = app_flash_get_config_osd_language();
	if (0 == strcmp(osd_language,LANGUAGE_CHINESE))
    {
 //   	GUI_SetProperty("win_cd_cas30_feed_tip_image_exit", "img", "tips_exit.bmp");
//		GUI_SetProperty("win_cd_cas30_feed_tip_image_ok", "img", "tips_confirm.bmp");
    }
    else if(0 == strcmp(osd_language,LANGUAGE_ENGLISH))
     {
   // 	GUI_SetProperty("win_cd_cas30_feed_tip_image_exit", "img", "tips_exit_e.bmp");
//		GUI_SetProperty("win_cd_cas30_feed_tip_image_ok", "img", "tips_confirm_e2.bmp");
    }

	GUI_SetProperty("win_cd_cas30_feed_listview", "select", &nListSel);
	return 0;

}

SIGNAL_HANDLER  int win_cd_cas30_feed_destroy(const char* widgetname, void *usrdata)
{
	return 0;
}

SIGNAL_HANDLER  int win_cd_cas30_feed_keypress(const char* widgetname, void *usrdata)
{
	GUI_Event *event = NULL;
	event = (GUI_Event *)usrdata;
	if(event->type == GUI_KEYDOWN)
	{
		switch(event->key.sym)
		{
			case KEY_EXIT:
			case KEY_RECALL:
				GUI_EndDialog("win_cd_cas30_feed");
				return EVENT_TRANSFER_STOP;
			case KEY_OK:
				return EVENT_TRANSFER_STOP;
			case KEY_POWER:
				return EVENT_TRANSFER_KEEPON;
			default:
				return EVENT_TRANSFER_STOP;
		}
	}
	return EVENT_TRANSFER_STOP;	
}

SIGNAL_HANDLER  int win_cd_cas30_feed_listview_key_press(const char* widgetname, void *usrdata)
{
	GUI_Event *event = NULL;
    unsigned int listview_sel =0;
	char* str = NULL;
	event = (GUI_Event *)usrdata;
	ca_mother_child_card_t card = {0};

	if(event->type == GUI_KEYDOWN)
	{
		switch(event->key.sym)
		{
		case KEY_MENU:
			event->key.sym = KEY_EXIT;
			return EVENT_TRANSFER_KEEPON;
			break;
		case KEY_OK:
			GUI_GetProperty("win_cd_cas30_feed_listview", "select", &listview_sel);
			card.date_type = DVB_CA_MOTHER_CARD_INFO;
			card.pos = listview_sel;
			card.state = 0; // init
			str = app_cas_api_feed_mother_child_card(&card);
			if (0 == strcmp(str,"Please insert the parent-card"))
			{
					#ifdef APP_SD
					app_popmsg(210,150,str,POPMSG_TYPE_OK);
					#endif
					#ifdef APP_HD
				app_popmsg(340, 200,str,POPMSG_TYPE_OK);				
					#endif
				card.state = 1; // read feed data
				str = app_cas_api_feed_mother_child_card(&card);
				if (0 == strcmp(str,"Please insert the child-card"))
				{
												#ifdef APP_SD
					app_popmsg(210,150,str,POPMSG_TYPE_OK);
					#endif
					#ifdef APP_HD
					app_popmsg(340, 200,str,POPMSG_TYPE_OK);
					#endif
					card.state = 2; // write feed data
					str = app_cas_api_feed_mother_child_card(&card);
												#ifdef APP_SD
					app_popmsg(210,150,str,POPMSG_TYPE_OK);
					#endif
					#ifdef APP_HD
					app_popmsg(340, 200,str,POPMSG_TYPE_OK);
					#endif
					return EVENT_TRANSFER_STOP;
				}
				else
				{
												#ifdef APP_SD
					app_popmsg(210,150,str,POPMSG_TYPE_OK);
					#endif
					#ifdef APP_HD
					app_popmsg(340, 200,str,POPMSG_TYPE_OK);
					#endif
					return EVENT_TRANSFER_STOP;
				}
			}
			else if (0 == strcmp(str,"Please insert the child-card"))
			{
						#ifdef APP_SD
						app_popmsg(210,150,str,POPMSG_TYPE_OK);
						#endif
						#ifdef APP_HD
				app_popmsg(340, 200,str,POPMSG_TYPE_OK);
						#endif
				card.state = 2; // write feed data
				str = app_cas_api_feed_mother_child_card(&card);
						#ifdef APP_SD
						app_popmsg(210,150,str,POPMSG_TYPE_OK);
						#endif
						#ifdef APP_HD
				app_popmsg(340, 200,str,POPMSG_TYPE_OK);						
						#endif						
			}
			else
			{
					#ifdef APP_SD
					app_popmsg(210,150,str,POPMSG_TYPE_OK);
					#endif
					#ifdef APP_HD
				app_popmsg(340, 200,str,POPMSG_TYPE_OK);
					#endif
				return EVENT_TRANSFER_STOP;
			}
			return EVENT_TRANSFER_STOP;
		default:
			return EVENT_TRANSFER_KEEPON;
		}
	}

	return EVENT_TRANSFER_KEEPON;
}


SIGNAL_HANDLER  int win_cd_cas30_feed_get_count(const char* widgetname, void *usrdata)
{
    get_count.date_type = DVB_CA_MOTHER_CARD_INFO;
    app_cas_api_get_count(&get_count);

    return get_count.totalnum;
}

SIGNAL_HANDLER  int win_cd_cas30_feed_get_data(const char* widgetname, void *usrdata)
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

	Row = item_para->sel;

	get_data.date_type = DVB_CA_MOTHER_CARD_INFO;
	get_data.pos = Row; // 行

	for (ID = CD_MOTHER_CHILD_TVS_ID;ID<=CD_MOTHER_CHILD_FEED_MOTHER_CARD_ID;ID++)
	{
		get_data.ID = ID;
		item_para->string = app_cas_api_get_data(&get_data);

		if (ID < CD_MOTHER_CHILD_FEED_MOTHER_CARD_ID)
		{
			/*
			* 不是最后一列
			*/
			item_para= item_para->next;
		}
	}

	return 0;
}


