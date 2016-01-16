/*****************************************************************************
* 						   CONFIDENTIAL
*        Hangzhou GuoXin Science and Technology Co., Ltd.
*                      (C)2010, All right reserved
******************************************************************************

******************************************************************************
* File Name :	win_all_searach.c
* Author    : 	zhouhm
* Project   :	goxceed dvbc
* Type      :
******************************************************************************
* Purpose   :	模块头文件
******************************************************************************
* Release History:
VERSION	Date			  AUTHOR         Description
1.0  	2012.11.12		  zhouhm 	 			creation
*****************************************************************************/
#include "app.h"

SIGNAL_HANDLER int win_all_search_create(const char* widgetname, void *usrdata)
{
	uint32_t lowfre = FRE_BEGIN_LOW_FULL;
	uint32_t highfre = FRE_BEGIN_HIGH_FULL;
	uint32_t symbol_rate = 0;
	uint32_t qam = 0;
	char App_Fre[7];//频点
	char sApp_Sym[5];//符号率
	char* osd_language=NULL;


    osd_language = app_flash_get_config_osd_language();
    if (0 == strcmp(osd_language,LANGUAGE_CHINESE))
    {
#ifdef SD

 //   	    GUI_SetProperty("win_all_search_title", "img", "DS_TITLE_FULLSEARCH_CH.bmp");
#endif
#ifdef HD
    	GUI_SetProperty("win_all_search_title", "img", "title_search3.bmp");
        
    	GUI_SetProperty("win_all_search_tip_image_exit", "img", "tips_exit.bmp");
    	GUI_SetProperty("win_all_search_tip_image_ok", "img", "tips_confirm.bmp");
#endif   
    }
    else 
     {
#ifdef SD

    //	    GUI_SetProperty("win_all_search_title", "img", "DS_TITLE_FULLSEARCH_ENG.bmp");
#endif
#ifdef HD
 
    	    GUI_SetProperty("win_all_search_title", "img", "title_search3_e.bmp");
        
    	GUI_SetProperty("win_all_search_tip_image_exit", "img", "tips_exit_e.bmp");
    	GUI_SetProperty("win_all_search_tip_image_ok", "img", "tips_confirm_e2.bmp");
#endif
    }
	


	memset(App_Fre,0,7);
	sprintf(App_Fre,"%03d", lowfre);
	GUI_SetProperty("win_all_search_begin_frequency_value", "string", App_Fre);
	memset(App_Fre,0,7);
	sprintf(App_Fre,"%03d", highfre);
	GUI_SetProperty("win_all_search_end_frequency_value", "string", App_Fre);


	symbol_rate = app_flash_get_config_manual_search_symbol_rate();
	qam = app_flash_get_config_manual_search_qam();

	memset(sApp_Sym,0,5);//符号率

	sprintf(sApp_Sym, "%04d", symbol_rate);

	GUI_SetProperty("win_all_search_symbol_value", "string", sApp_Sym);
	GUI_SetProperty("win_all_search_qam_value", "select", &qam);	
	GUI_SetProperty("win_all_search_time_text", "string", app_win_get_local_date_string());
#ifdef APP_SD
	int32_t widget_value = 0;
    
    widget_value = app_flash_get_config_gx150x_mode_flag();
	if(GXBUS_PM_SAT_1501_DTMB == widget_value)
	{
		GUI_SetProperty("win_all_search_symbol_boxitem", "state","disable");
		GUI_SetProperty("win_all_search_symbol_boxitem", "state","hide");
		GUI_SetProperty("win_all_search_qam_boxitem", "state","disable");
		GUI_SetProperty("win_all_search_qam_boxitem", "state","hide");
	}
	else
	{
		GUI_SetProperty("win_all_search_symbol_boxitem", "state","enable");
		GUI_SetProperty("win_all_search_symbol_boxitem", "state","show");
		GUI_SetProperty("win_all_search_qam_boxitem", "state","enable");
		GUI_SetProperty("win_all_search_qam_boxitem", "state","show");
	}
	
#endif

	
	return 0;
}

SIGNAL_HANDLER  int win_all_search_destroy(const char* widgetname, void *usrdata)
{
	return 0;
}

SIGNAL_HANDLER int win_all_search_keypress(const char* widgetname, void *usrdata)
{
	GUI_Event *event = NULL;
	event = (GUI_Event *)usrdata;
	uint32_t symbol_rate = 0;
	uint32_t qam = 0;
	uint32_t lowfre = 0;
	uint32_t highfre = 0;
	char *value;
	uint32_t value2;

	if(event->type == GUI_KEYDOWN)
	{
		switch(event->key.sym)
		{
		case KEY_0:
	        case KEY_1:
	        case KEY_2:
	        case KEY_3:
	        case KEY_4:
	        case KEY_5:
	        case KEY_6:
	        case KEY_7:
	        case KEY_8:
	        case KEY_9:
			GUI_GetProperty("win_all_search_begin_frequency_value", "string", &value);
			lowfre = atoi(value);
			GUI_GetProperty("win_all_search_end_frequency_value", "string", &value);
			highfre = atoi(value);

	//		GUI_GetProperty("win_all_search_symbol_value", "string", &value);
	//		symbol_rate = atoi(value);
			GUI_GetProperty("win_all_search_box", "select", &value2);
			if(0==value2){
				app_win_check_fre_vaild(FRE_BEGIN,lowfre);
			}
			else if(1==value2){
				app_win_check_fre_vaild(FRE_END,highfre);
			}
	//		app_win_check_sym_vaild(symbol_rate);
		        return EVENT_TRANSFER_STOP;
		case KEY_RECALL:
		case KEY_EXIT:
		case KEY_MENU:
			GUI_EndDialog("win_all_search");
			return EVENT_TRANSFER_STOP;	
		case KEY_OK:
			GUI_GetProperty("win_all_search_begin_frequency_value", "string", &value);
			lowfre = atoi(value);
			GUI_GetProperty("win_all_search_end_frequency_value", "string", &value);
			highfre = atoi(value);

//			GUI_GetProperty("win_all_search_symbol_value", "string", &value);
//			symbol_rate = atoi(value);
//			GUI_GetProperty("win_all_search_qam_value", "select", &value2);
//			qam = value2;

			if (FALSE ==  app_win_check_fre_range_vaild(lowfre,highfre))
			{
				return EVENT_TRANSFER_STOP;						
			}
#if 0
			if (FALSE ==  app_win_check_sym_vaild(symbol_rate))
			{
				return EVENT_TRANSFER_STOP;						
			}
#endif
		symbol_rate = app_flash_get_config_manual_search_symbol_rate();
		qam = app_flash_get_config_manual_search_qam();

		
			GUI_CreateDialog("win_search_result");
			GUI_SetInterface("flush", NULL);
			app_search_scan_all_mode(lowfre,highfre,symbol_rate,qam);
			return EVENT_TRANSFER_STOP;				
		default:
			return EVENT_TRANSFER_KEEPON;
		}
	}

	return EVENT_TRANSFER_KEEPON;
}

