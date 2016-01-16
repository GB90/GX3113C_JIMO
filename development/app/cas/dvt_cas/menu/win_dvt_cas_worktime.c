/*****************************************************************************
* 						   CONFIDENTIAL
*        Hangzhou GuoXin Science and Technology Co., Ltd.
*                      (C)2012, All right reserved
******************************************************************************

******************************************************************************
* File Name :	win_dvt_cas_worktime.c
* Author    : 	zhouhm
* Project   :	goxceed dvbc
* Type      :
******************************************************************************
* Purpose   :	ģ??ͷ??\u0152?
******************************************************************************
* Release History:
VERSION	Date			  AUTHOR         Description
1.0  	2013.12.05		  zhouhm 	 			creation
*****************************************************************************/
#include "app_common_porting_stb_api.h"
#include "app_win_interface.h"
#include "app_dvt_cas_api_worktime.h"
#include "DVTCAS_STBInf.h"

SIGNAL_HANDLER  int win_dvt_cas_worktime_create(const char* widgetname, void *usrdata)
{
//	uint32_t rating = 0;
	ca_get_date_t get_data = {0};
	char* str = NULL;

	 char* osd_language=NULL;	
	
	 osd_language = app_flash_get_config_osd_language();
	 if (0 == strcmp(osd_language,LANGUAGE_CHINESE))	
	{
	//	 GUI_SetProperty("win_dvt_cas_worktime_title","img","title_ch_Period.bmp");  
	//	 GUI_SetProperty("win_dvt_cas_worktime_tip_image_exit", "img", "tips_exit.bmp");
	//	 GUI_SetProperty("win_dvt_cas_worktime_tip_image_ok", "img", "tips_confirm.bmp");

	 }
	 else if(0 == strcmp(osd_language,LANGUAGE_ENGLISH))
	 { 
	//	 GUI_SetProperty("win_dvt_cas_worktime_title","img","title_Period_e.bmp");
	//	 GUI_SetProperty("win_dvt_cas_worktime_tip_image_exit","img","tips_exit_e.bmp");
	//	 GUI_SetProperty("win_dvt_cas_worktime_tip_image_ok", "img", "tips_confirm_e2.bmp");
	 }

	
	get_data.date_type = DVB_CA_WORK_TIME_INFO;	
	get_data.ID = DVT_WORKTIME_START_ID;
	str = app_cas_api_get_data(&get_data);
	if (NULL !=str )
		{
                GUI_SetProperty("win_dvt_cas_worktime_boxitem2_edit", "string",str);			
		}

	get_data.date_type = DVB_CA_WORK_TIME_INFO;	
	get_data.ID = DVT_WORKTIME_END_ID;
	str = app_cas_api_get_data(&get_data);
	if (NULL !=str )
		{	printf("str=%s\n",str);			 
            GUI_SetProperty("win_dvt_cas_worktime_boxitem3_edit", "string",str);			
		}

	return 0;

}

SIGNAL_HANDLER  int win_dvt_cas_worktime_destroy(const char* widgetname, void *usrdata)
{
	return 0;
}

SIGNAL_HANDLER  int win_dvt_cas_worktime_keypress(const char* widgetname, void *usrdata)
{
	GUI_Event *event = NULL;
	event = (GUI_Event *)usrdata;

	if(event->type == GUI_KEYDOWN)
	{
		switch(event->key.sym)
		{
			case KEY_EXIT:
				break;
				return EVENT_TRANSFER_STOP;
			case KEY_RECALL:
				 GUI_EndDialog("win_dvt_cas_worktime");
				 return EVENT_TRANSFER_STOP;
			case KEY_POWER:
				return EVENT_TRANSFER_KEEPON;
			default:
				return EVENT_TRANSFER_STOP;
		}
	}

	return EVENT_TRANSFER_KEEPON;
}

SIGNAL_HANDLER  int win_dvt_cas_worktime_box_keypress(const char* widgetname, void *usrdata)
{
	GUI_Event *event = NULL;
    char *pin;
    char *starttime;
    char *endtime;
	char* str = NULL;
	ca_work_time_t worktime = {0};

	uint32_t nSelect;

	event = (GUI_Event *)usrdata;

	if(event->type == GUI_KEYDOWN)
	{
		switch(event->key.sym)
		{
		case KEY_MENU:
			event->key.sym = KEY_EXIT;
			return EVENT_TRANSFER_KEEPON;
			break;
		case KEY_DOWN:
			GUI_GetProperty("win_dvt_cas_worktime_box", "select", &nSelect);
			if(nSelect == 0)
			{
				GUI_GetProperty("win_dvt_cas_worktime_boxitem1_edit", "string", &pin);
				if(strlen((char*)pin)!=DVTCA_MAXLEN_PINCODE)
				{
                    #ifdef APP_SD
                    app_popmsg(210, 150,"Input 8-bit PIN",POPMSG_TYPE_OK);
    				#endif
    				#ifdef APP_HD
    				app_popmsg(340, 200,"Input 8-bit PIN",POPMSG_TYPE_OK);
    				#endif
					return EVENT_TRANSFER_STOP; 
				}
			}
			break;

		case KEY_OK:
			/*????ƥ??ԭʼ????*/

			GUI_GetProperty("win_dvt_cas_worktime_boxitem1_edit", "string", &pin);
			GUI_GetProperty("win_dvt_cas_worktime_boxitem2_edit", "string", &starttime);
			GUI_GetProperty("win_dvt_cas_worktime_boxitem3_edit", "string", &endtime);
			
			worktime.date_type = DVB_CA_WORK_TIME_INFO;
			worktime.pin = (char*)pin;
			worktime.starttime = starttime;
			worktime.endtime = endtime;
			
			str = app_cas_api_change_worktime(&worktime);
			if (str != NULL)
			{
				if (0 == strcmp(str,"Input new PIN"))
				{
					GUI_SetProperty("win_cd_cas30_worktime_boxitem1_edit","clear",NULL);
				}
                #ifdef APP_SD
                app_popmsg(210, 150,str,POPMSG_TYPE_OK);
				#endif
				#ifdef APP_HD
				app_popmsg(340, 200,str,POPMSG_TYPE_OK);
				#endif
			}
			
			return EVENT_TRANSFER_STOP;
		case KEY_POWER:
			return EVENT_TRANSFER_KEEPON;
		default:
			return EVENT_TRANSFER_KEEPON;
		}
	}

	return EVENT_TRANSFER_KEEPON;
}








