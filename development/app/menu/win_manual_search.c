/*****************************************************************************
* 						   CONFIDENTIAL
*        Hangzhou GuoXin Science and Technology Co., Ltd.
*                      (C)2010, All right reserved
******************************************************************************

******************************************************************************
* File Name :	win_manual_search.c
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
static event_list* spmanualsearchtime = NULL;
static search_dvbc_param previous_param={0};


static  int timer(void *userdata)
{
	strength_xml strengthxml = {0};
	signal_xml singalxml = {0};
	char* focus_Window = (char*)GUI_GetFocusWindow();


	if((NULL !=focus_Window )&&( strcasecmp("win_manual_search", focus_Window) != 0))
	{
		return 0;
	}
	
	if(previous_param.flag==0)
	{
		return 0;
	}

	if (previous_param.fre < FRE_BEGIN_LOW || previous_param.fre > FRE_BEGIN_HIGH)
	{
		return 0;
	}

	if (previous_param.symbol_rate < 2000 || previous_param.symbol_rate > 7000)
	{
		return 0;
	}


//	if (1 == GxFrontend_QueryFrontendStatus(0))
	if(1 ==  GxFrontend_QueryStatus(0))
	{
		GUI_SetProperty("win_manual_search_lock_text", "string", "Locked");
		strengthxml.lock_status = 1;	
		singalxml.lock_status =1;
	}
	else
	{
		GUI_SetProperty("win_manual_search_lock_text", "string", "UnLock");	
		strengthxml.lock_status = 0;	
		singalxml.lock_status =0;
	}
	strengthxml.widget_name_strength_bar = "win_manual_search_signal_strength_progbar";
	strengthxml.widget_name_strength_bar_value = "win_manual_search_signal_strength_value";
	app_search_set_strength_progbar(strengthxml);
	singalxml.widget_name_signal_bar = "win_manual_search_signal_quality_progbar";
	singalxml.widget_name_signal_bar_value = "win_manual_search_signal_quality_value";
	singalxml.widget_name_error_rate_bar = NULL;
	singalxml.widget_name_error_rate = "win_manual_search_err_value";
	app_search_set_signal_progbar(singalxml);

	return 0;
}
SIGNAL_HANDLER int win_manual_search_create(const char* widgetname, void *usrdata)
{
	search_dvbc_param tmp_param;
	char App_Fre[7];//频点
	char sApp_Sym[5];//符号率
    uint32_t sApp_Mod;//调制方式
	uint32_t value = 0;
	char* osd_language=NULL;

//	GxFrontend_StopMonitor(0);//监控里去设置频率，UI里不用设置这样不会卡顿。

    osd_language = app_flash_get_config_osd_language();
    if (0 == strcmp(osd_language,LANGUAGE_CHINESE))
    {
#ifdef SD
 //   	GUI_SetProperty("win_manual_search_title", "img", "DS_TITLE_MANUAL_SEARCH_CH.bmp");
#endif
#ifdef HD
    	GUI_SetProperty("win_manual_search_title", "img", "title_search2.bmp");
    	GUI_SetProperty("win_manual_search_tip_image_exit", "img", "tips_exit.bmp");
    	GUI_SetProperty("win_manual_search_tip_image_ok", "img", "tips_confirm.bmp");
#endif
    }
    else if(0 == strcmp(osd_language,LANGUAGE_ENGLISH))
    {
#ifdef SD
    //	GUI_SetProperty("win_manual_search_title", "img", "DS_TITLE_MANUAL_SEARCH_ENG.bmp");
#endif

#ifdef HD	    
    	GUI_SetProperty("win_manual_search_title", "img", "title_search2_e.bmp");
    	GUI_SetProperty("win_manual_search_tip_image_exit", "img", "tips_exit_e.bmp");
    	GUI_SetProperty("win_manual_search_tip_image_ok", "img", "tips_confirm_e2.bmp");
#endif    
    }

	memset(&previous_param,0,sizeof(search_dvbc_param));
	memset(&tmp_param,0,sizeof(search_dvbc_param));

	tmp_param.fre= app_flash_get_config_center_freq();
	memset(App_Fre,0,7);
	sprintf(App_Fre,"%03d", tmp_param.fre);
	GUI_SetProperty("win_manual_search_frequency_value", "string", App_Fre);

	tmp_param.symbol_rate= app_flash_get_config_manual_search_symbol_rate();
	tmp_param.qam= app_flash_get_config_manual_search_qam();

	memset(sApp_Sym,0,5);//符号率

	sprintf(sApp_Sym, "%04d", tmp_param.symbol_rate);
   	 sApp_Mod = tmp_param.qam;

	GUI_SetProperty("win_manual_search_symbol_value", "string", sApp_Sym);
	GUI_SetProperty("win_manual_search_qam_value", "select", &sApp_Mod);
	GUI_SetProperty("win_manual_search_time_text", "string", app_win_get_local_date_string());

	/*
	* reset progress zero
	*/
	value = 0;
	GUI_SetProperty("win_manual_search_signal_strength_progbar", "value", (void*)&value);
	GUI_SetProperty("win_manual_search_signal_quality_progbar", "value", (void*)&value);
	printf("[%s]:%d\n",__FUNCTION__,__LINE__);

	app_search_lock_tp(tmp_param.fre, tmp_param.symbol_rate,INVERSION_OFF, tmp_param.qam,0);
	printf("%s[%s]:%d\n",__FILE__,__FUNCTION__,__LINE__);
	spmanualsearchtime = create_timer(timer, 500, NULL,  TIMER_REPEAT);

	previous_param.fre= tmp_param.fre;
	previous_param.symbol_rate= tmp_param.symbol_rate;
	previous_param.qam= tmp_param.qam;
	previous_param.flag = 1;

	#ifdef APP_SD
	int32_t widget_value = 0;
    
    widget_value = app_flash_get_config_gx150x_mode_flag();
	if(GXBUS_PM_SAT_1501_DTMB == widget_value)
	{
		GUI_SetProperty("win_manual_search_symbol_boxitem", "state","disable");
		GUI_SetProperty("win_manual_search_symbol_boxitem", "state","hide");
		GUI_SetProperty("win_manual_search_qam_boxitem", "state","disable");
		GUI_SetProperty("win_manual_search_qam_boxitem", "state","hide");
	}
	else
	{
		GUI_SetProperty("win_manual_search_symbol_boxitem", "state","enable");
		GUI_SetProperty("win_manual_search_symbol_boxitem", "state","show");
		GUI_SetProperty("win_manual_search_qam_boxitem", "state","enable");
		GUI_SetProperty("win_manual_search_qam_boxitem", "state","show");
	}
	
	#endif
	
		
	return 0;
}

SIGNAL_HANDLER  int win_manual_search_destroy(const char* widgetname, void *usrdata)
{
	remove_timer(spmanualsearchtime);
	spmanualsearchtime = NULL;

	return 0;
}

SIGNAL_HANDLER int win_manual_search_keypress(const char* widgetname, void *usrdata)
{
	GUI_Event *event = NULL;
	event = (GUI_Event *)usrdata;
	uint32_t symbol_rate = 0;
	uint32_t qam = 0;
	uint32_t fre = 0;
	char *value;
	uint32_t value2;
	strength_xml strengthxml = {0};
	signal_xml singalxml = {0};

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
			GUI_GetProperty("win_manual_search_box", "select", &value2);
			if (0 == value2)
			{
				GUI_GetProperty("win_manual_search_frequency_value", "string", &value);
				fre = atoi(value);
				if (FALSE ==  app_win_check_fre_vaild(FRE_MID,fre))
				{
					previous_param.fre = fre;
					previous_param.flag=0;
					strengthxml.lock_status =0;
					strengthxml.widget_name_strength_bar = "win_manual_search_signal_strength_progbar";
					strengthxml.widget_name_strength_bar_value = "win_manual_search_signal_strength_value";
					app_search_set_strength_progbar(strengthxml);
					singalxml.lock_status=0;
					singalxml.widget_name_signal_bar = "win_manual_search_signal_quality_progbar";
					singalxml.widget_name_signal_bar_value = "win_manual_search_signal_quality_value";
					singalxml.widget_name_error_rate_bar = NULL;
					singalxml.widget_name_error_rate = "win_manual_search_err_value";
					app_search_set_signal_progbar(singalxml);
					GUI_SetProperty("win_manual_search_lock_text", "string", "UnLock");	

					return EVENT_TRANSFER_STOP;						
				}
				
				previous_param.flag = 1;
				if (fre != previous_param.fre)
				{
						previous_param.fre = fre;
						app_search_lock_tp(previous_param.fre, previous_param.symbol_rate,
									INVERSION_OFF, previous_param.qam,0);
                        printf("%s[%s]:%d\n",__FILE__,__FUNCTION__,__LINE__);
						
				}
				
			}
#if 0
		else if (1 == value2)
			{
				GUI_GetProperty("win_manual_search_symbol_value", "string", &value);
				symbol_rate = atoi(value);
				if (FALSE ==  app_win_check_sym_vaild(symbol_rate))
				{
					previous_param.symbol_rate = symbol_rate;
					previous_param.flag=0;
					strengthxml.lock_status =0;
					strengthxml.widget_name_strength_bar = "win_manual_search_signal_strength_progbar";
					strengthxml.widget_name_strength_bar_value = "win_manual_search_signal_strength_value";
					app_search_set_strength_progbar(strengthxml);
					singalxml.lock_status=0;
					singalxml.widget_name_signal_bar = "win_manual_search_signal_quality_progbar";
					singalxml.widget_name_signal_bar_value = "win_manual_search_signal_quality_value";
					singalxml.widget_name_error_rate_bar = NULL;
					singalxml.widget_name_error_rate = "win_manual_search_err_value";
					app_search_set_signal_progbar(singalxml);

					GUI_SetProperty("win_manual_search_lock_text", "string", "UnLock");	

					return EVENT_TRANSFER_STOP;						
				}
				
				previous_param.flag=1;
				if (symbol_rate != previous_param.symbol_rate)
				{

					previous_param.symbol_rate = symbol_rate;
					app_search_lock_tp(previous_param.fre, previous_param.symbol_rate,
								INVERSION_OFF, previous_param.qam,0);
                    printf("%s[%s]:%d\n",__FILE__,__FUNCTION__,__LINE__);
					
				}
			}
#endif
		return EVENT_TRANSFER_STOP;	
#if 0
		case KEY_LEFT:
		case KEY_RIGHT:
			GUI_GetProperty("win_manual_search_qam_value", "select", &value2);
			qam = value2;
			previous_param.flag=1;
			if (qam != previous_param.qam)
			{
					previous_param.qam = qam;
					app_search_lock_tp(previous_param.fre, previous_param.symbol_rate,
								INVERSION_OFF, previous_param.qam,0);
                    printf("%s[%s]:%d\n",__FILE__,__FUNCTION__,__LINE__);
					
			}
			
			return EVENT_TRANSFER_STOP;				
#endif
		case KEY_RECALL:
		case KEY_EXIT:
		case KEY_MENU:
			GxFrontend_StartMonitor(0);
			GUI_EndDialog("win_manual_search");
			return EVENT_TRANSFER_STOP;	
		case KEY_OK:
			GUI_GetProperty("win_manual_search_frequency_value", "string", &value);
			fre = atoi(value);
		
			if (FALSE ==  app_win_check_fre_vaild(FRE_MID,fre))
			{
				return EVENT_TRANSFER_STOP;						
			}
#if 0
			GUI_GetProperty("win_manual_search_symbol_value", "string", &value);
			symbol_rate = atoi(value);
			
			if (FALSE ==  app_win_check_sym_vaild(symbol_rate))
			{
				return EVENT_TRANSFER_STOP;						
			}
#endif
			GxCore_ThreadDelay(100);	
			
//			if (1 != GxFrontend_QueryFrontendStatus(0))
			if(1 != GxFrontend_QueryStatus(0))
			{
				app_popmsg(POP_DIALOG_X_START, POP_DIALOG_Y_START,"Lock Failed",POPMSG_TYPE_OK);

				return EVENT_TRANSFER_STOP;
			}
#if 0				
			GUI_GetProperty("win_manual_search_qam_value", "select", &value2);
			qam = value2;
#endif			
			symbol_rate= app_flash_get_config_manual_search_symbol_rate();
			qam= app_flash_get_config_manual_search_qam();


			GUI_CreateDialog("win_search_result");
			GUI_SetInterface("flush", NULL);
			app_search_scan_manual_mode(fre,symbol_rate,qam);
			return EVENT_TRANSFER_STOP;				
		default:
			return EVENT_TRANSFER_KEEPON;
		}
	}

	return EVENT_TRANSFER_KEEPON;
}

