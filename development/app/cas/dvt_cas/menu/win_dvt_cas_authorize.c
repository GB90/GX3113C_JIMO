/*****************************************************************************
* 						   CONFIDENTIAL								
*        Hangzhou GuoXin Science and Technology Co., Ltd.             
*                      (C)2010, All right reserved
******************************************************************************

******************************************************************************
* File Name :	win_dvt_cas_authorize.c
* Author    : 	zhouhm
* Project   :	goxceed dvbc
* Type      :	
******************************************************************************
* Purpose   :	模块头文件
******************************************************************************
* Release History:
  VERSION	Date			  AUTHOR         Description
   1.0  	2013.12.03		  zhouhm 	 			creation
*****************************************************************************/
#include "app_common_porting_stb_api.h"
#include "app_win_interface.h"
#include "app_dvt_cas_api_operator.h"


static ca_get_count_t get_count = {0};	
uint32_t operator_sel = 0;


SIGNAL_HANDLER  int app_dvt_cas_authorize_create(const char* widgetname, void *usrdata)
{
	 char* osd_language=NULL;	
	
	 osd_language = app_flash_get_config_osd_language();
	 
	 if (0 == strcmp(osd_language,LANGUAGE_CHINESE))	
	{
	//	 GUI_SetProperty("win_dvt_cas_authorize_title","img","title_ch_Authorization.bmp");  
	//	 GUI_SetProperty("win_dvt_cas_authorize_tip_image_exit", "img", "tips_exit.bmp");
	//	 GUI_SetProperty("win_dvt_cas_authorize_confirm_flag", "img", "tips_confirm.bmp");
	 }
	 else if(0 == strcmp(osd_language,LANGUAGE_ENGLISH))
	 { 
	//	 GUI_SetProperty("win_dvt_cas_authorize_title","img","title_Authorization_e.bmp");
	//	 GUI_SetProperty("win_dvt_cas_authorize_tip_image_exit","img","tips_exit_e.bmp");
	//	 GUI_SetProperty("win_dvt_cas_authorize_confirm_flag", "img", "tips_confirm_e2.bmp");
	 }

	return 0;
}

SIGNAL_HANDLER  int  app_dvt_cas_authorize_destroy(const char* widgetname, void *usrdata)
{
	return 0;
}


SIGNAL_HANDLER  int app_dvt_cas_authorize_keypress(const char* widgetname, void *usrdata)
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
				case KEY_RECALL:
					GUI_EndDialog("win_dvt_cas_authorize");
					return EVENT_TRANSFER_STOP;
				case KEY_OK:
					GUI_GetProperty("win_dvt_cas_authorize_listview","select",(void*)&operator_sel);
					if ( 0 == get_count.totalnum)
					{
                        #ifdef APP_SD
                        app_popmsg(210,150,"No corporation information",POPMSG_TYPE_OK);
        				#endif
        				#ifdef APP_HD
        				app_popmsg(340,200,"No corporation information",POPMSG_TYPE_OK);		
        				#endif
						return EVENT_TRANSFER_STOP;
					}

					if (1 == app_cas_api_init_data(DVB_CA_ENTITLE_INFO))
					{
						GUI_CreateDialog("win_dvt_cas_entitle");
					}
				    else
					{
                        #ifdef APP_SD
                        app_popmsg(210,150,"Access to information error",POPMSG_TYPE_OK);
        				#endif
        				#ifdef APP_HD
        				app_popmsg(340,200,"Access to information error",POPMSG_TYPE_OK);		
        				#endif
						return EVENT_TRANSFER_STOP;
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

SIGNAL_HANDLER  int app_dvt_cas_authorize_get_count(const char* widgetname, void *usrdata)
{
    get_count.date_type = DVB_CA_OPERATOR_INFO;
    app_cas_api_get_count(&get_count);
	
    return get_count.totalnum;
}

SIGNAL_HANDLER  int app_dvt_cas_authorize_get_data(const char* widgetname, void *usrdata)
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
	if(NULL == item_para) 
		return GXCORE_ERROR;

	Row = item_para->sel;

	get_data.date_type = DVB_CA_OPERATOR_INFO;
	get_data.pos = Row; // 行


	for (ID = DVT_OPERATOR_NO_ID;ID<=DVT_OPERATOR_NAME_ID;ID++)
		{
			get_data.ID = ID;
			item_para->string = app_cas_api_get_data(&get_data);

			if (ID < DVT_OPERATOR_NAME_ID)
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

SIGNAL_HANDLER  int app_dvt_cas_authorize_listview_key_press(const char* widgetname, void *usrdata)
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



