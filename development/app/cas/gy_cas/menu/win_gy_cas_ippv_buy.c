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
#include "app_common_epg.h"
#include "app_win_interface.h"
#include "win_gy_cas_ippv_buy.h"
#include "app_common_epg.h"
#include "app_gy_cas_api_pop.h"
#include "gyca_def.h"


static gy_cas_ippv_buy_st caippvbuy = {0};
static T_CA_IPPVInfo* ippvbuyinfo = NULL;
extern void app_epg_convert_to_utc( unsigned short wDate, com_time_t* pTimeOut );

int32_t app_gy_cas_gxmsg_ca_on_event_ippv_buy(GxMsgProperty0_OnEvent* event)
{
	gy_cas_ippv_buy_st* ippv_buy = NULL;
	char* focus_Window = (char*)GUI_GetFocusWindow();

	if (NULL == event)
		return 0;

	ippv_buy = (gy_cas_ippv_buy_st*)(event->buf);

	if (NULL == ippv_buy)
		return 0;
	if(ippv_buy->wStatus == TRUE && caippvbuy.wShowStatus == FALSE)
	{
		memcpy(&caippvbuy,ippv_buy,sizeof(gy_cas_ippv_buy_st));
		
		if ((NULL != focus_Window)&&(strcasecmp("win_full_screen", focus_Window) != 0))
		{
			GUI_EndDialog(focus_Window);
			
			GxCore_ThreadDelay(300);
			GUI_SetInterface("flush", NULL);
			GxCore_ThreadDelay(50);
		}

		GUI_CreateDialog("win_gy_cas_ippv_buy");	
	}
	else if (ippv_buy->wStatus == FALSE && caippvbuy.wShowStatus == TRUE)
	{
		GUI_EndDialog("win_gy_cas_ippv_buy");
	}
	return 1;
}
extern void _Mjd2Date( uint8_t u8D1, uint8_t u8D2, com_time_t * pstTM );
extern void MjdToUtc(uint32_t mjd,com_time_t * m_utc);


SIGNAL_HANDLER  int win_gy_cas_ippv_buy_create(const char* widgetname, void *usrdata)
{
	char str[30]={0};
    com_time_t  Time_t;
		
	ippvbuyinfo = &caippvbuy.ippvBuyInfo;
	if((unsigned int)ippvbuyinfo->price.m_byEventPriceType == 1)
	{
		sprintf(str,"%d",(unsigned int)ippvbuyinfo->m_wEventId);
	}
	else
	{
		sprintf(str,"%d",(unsigned int)ippvbuyinfo->m_dwEvent_Ex_Sn);
	}
	GUI_SetProperty("win_gy_cas_ippv_buy_eventid_value", "string", str);

	sprintf(str,"%d",(unsigned int)ippvbuyinfo->m_wServiceId);
	GUI_SetProperty("win_gy_cas_ippv_buy_serviceid_value", "string", str);

	sprintf(str,"%d.%d",(unsigned int)GYCA_GetSMCTokenInfo()/100,
		(unsigned int)GYCA_GetSMCTokenInfo()%100);
	GUI_SetProperty("win_gy_cas_ippv_buy_money_value", "string", str);	

	sprintf(str,"%d.%d",(unsigned int)ippvbuyinfo->price.m_wEventPrice/100,
		(unsigned int)ippvbuyinfo->price.m_wEventPrice%100);
	GUI_SetProperty("win_gy_cas_ippv_buy_price_value", "string", str);

	if((unsigned int)ippvbuyinfo->price.m_byEventPriceType == 1)
	{
		/*IPPPT*/
		GUI_SetProperty("win_gy_cas_ippv_buy_title", "string", "IPPT");	
	}
	/*_Mjd2Date(ippvbuyinfo->validDate.date.m_byDate>>8, 
		ippvbuyinfo->validDate.date.m_byDate&0xff, 
		&Time_t );*/
	
	MjdToUtc(ippvbuyinfo->validDate.date.m_byDate,&Time_t);
	//app_epg_convert_to_utc(ippvbuyinfo->validDate.date.m_byDate, &Time_t );
	printf("m_byDate=0x%x\n",ippvbuyinfo->validDate.date.m_byDate);
	sprintf( str, "%04d-%02d-%02d %02d:%02d:%02d", Time_t.year, Time_t.month, Time_t.day,
		ippvbuyinfo->validDate.time.m_byHour,
		ippvbuyinfo->validDate.time.m_byMinute,
		ippvbuyinfo->validDate.time.m_bySecond);
	GUI_SetProperty("win_gy_cas_ippv_buy_starttime_value", "string", str);	

	/*_Mjd2Date(ippvbuyinfo->expiredDate.date.m_byDate>>8, 
		ippvbuyinfo->expiredDate.date.m_byDate&0xff, 
		&Time_t );*/
	
	MjdToUtc(ippvbuyinfo->expiredDate.date.m_byDate,&Time_t);
	//app_epg_convert_to_utc(ippvbuyinfo->expiredDate.date.m_byDate, &Time_t );
	sprintf( str, "%04d-%02d-%02d %02d:%02d:%02d", Time_t.year, Time_t.month, Time_t.day,
		ippvbuyinfo->expiredDate.time.m_byHour,
		ippvbuyinfo->expiredDate.time.m_byMinute,
		ippvbuyinfo->expiredDate.time.m_bySecond);
	GUI_SetProperty("win_gy_cas_ippv_buy_endtime_value", "string", str);	
		

	caippvbuy.wShowStatus= 1;
	return 0;
}

SIGNAL_HANDLER  int win_gy_cas_ippv_buy_destroy(const char* widgetname, void *usrdata)
{
	caippvbuy.wShowStatus= 0;
	return 0;	
}


SIGNAL_HANDLER int win_gy_cas_ippv_buy_box_keypress(const char* widgetname, void *usrdata)
{
	GUI_Event *event = NULL;
	event = (GUI_Event *)usrdata;
	if(event == NULL)
	{
		return EVENT_TRANSFER_KEEPON;
	}
	if(event->type == GUI_KEYDOWN)
	{	
		switch(event->key.sym)
		{
			case KEY_UP:
			case KEY_DOWN:
			GUI_EndDialog("win_gy_cas_ippv_buy");		
			
			if(PROGRAM_TV_LIST_WIN == app_win_get_focus_video_window() ) 
			{
				GUI_SetFocusWidget("win_tv_prog_listview");
				GUI_SendEvent("win_tv_prog_listview", event);	
			}
			else if(WEEKLY_EPG_WIN == app_win_get_focus_video_window()) 
			{
				GUI_SetFocusWidget("win_epg_prog_list_listview");
				GUI_SendEvent("win_epg_prog_list_listview", event); 
			}
			else if(FAVORITE_LIST_WIN == app_win_get_focus_video_window()) 
			{
				GUI_SetFocusWidget("win_favorite_prog_listview");
				GUI_SendEvent("win_favorite_prog_listview", event); 
			}					
			else	if (FULL_SCREEN_WIN == app_win_get_focus_video_window())
			{
				if (TRUE == app_get_win_create_flag(PROGRAM_BAR_WIN))
				{
					/*
					* 信息条显示状态下，按键发送到信息条
					*/
					GUI_SendEvent("win_prog_bar", event);
				}
				else
				{
					GUI_SendEvent("win_full_screen", event);
				}
			}		
		}
	}
	return EVENT_TRANSFER_KEEPON;	
}

SIGNAL_HANDLER  int win_gy_cas_ippv_buy_keypress(const char* widgetname, void *usrdata)
{
	GUI_Event *event = NULL;
	event = (GUI_Event *)usrdata;
	uint8_t newPassword1[6+1]={0};
	uint8_t newPasswordValue1[6+1]={0};
	uint8_t *value;
	popmsg_ret ret;
	u_int16_t ecmpid = 0x1fff;
	GYS32   rtn;
	uint32_t ratevalue;
	uint8_t i = 0;
	uint16_t  m_wPrice_tppvview=0;
	uint16_t  m_wPrice_tppvviewtapping=0;
	T_CA_IPPVPrice buyIPPVPrice = {0};

	 ecmpid = caippvbuy.wEcmPid;
	 
	if(event->type == GUI_KEYDOWN)
	{	
		switch(event->key.sym)
		{
			case KEY_EXIT:
				  GYCA_IPPVStopBuyDlg( FALSE,
                                        ecmpid,
                                        NULL,
                                        NULL );
				 GUI_EndDialog("win_gy_cas_ippv_buy");
				return EVENT_TRANSFER_STOP;	
			case KEY_RECALL:
				  GYCA_IPPVStopBuyDlg( FALSE,
                                        ecmpid,
                                        NULL,
                                        NULL );	
				 GUI_EndDialog("win_gy_cas_ippv_buy");
				 return EVENT_TRANSFER_STOP;	
			case KEY_OK:
				
				GUI_GetProperty("win_gy_cas_ippv_buy_purse_boxitem2_edit", "string", &value);
				memset(newPassword1, 0, 6+1);
				memcpy(newPassword1,(char*)value, 6);

				if(strlen((char*)newPassword1)!=6)
				{
					GUI_SetProperty("win_gy_cas_ippv_buy_purse_boxitem2_edit","clear",NULL);
					ret = app_popmsg(210,200,"Input new PIN",POPMSG_TYPE_OK);
					return EVENT_TRANSFER_STOP;	
				}

				rtn = GYCA_IPPVStopBuyDlg(TRUE,ecmpid,newPassword1, &buyIPPVPrice);
				
				switch(rtn)
				{
					case GYCA_OK:
						ret = app_popmsg(210,200,"购买成功",POPMSG_TYPE_OK);
						break;
					case GYCA_PIN_ERR:
						ret = app_popmsg(210,200,"密码错误",POPMSG_TYPE_OK);							
						break;
					case GYCA_ERR:
						ret = app_popmsg(210,200,"金额不足或卡出错",POPMSG_TYPE_OK);								
						break;
				}
				
				 GUI_EndDialog("win_gy_cas_ippv_buy");

				return EVENT_TRANSFER_STOP;	
			case KEY_UP:
			case KEY_DOWN:
				GUI_EndDialog("win_gy_cas_ippv_buy");		
				
				if(PROGRAM_TV_LIST_WIN == app_win_get_focus_video_window() ) 
				{
					GUI_SetFocusWidget("win_tv_prog_listview");
					GUI_SendEvent("win_tv_prog_listview", event);	
				}
				else if(WEEKLY_EPG_WIN == app_win_get_focus_video_window()) 
				{
					GUI_SetFocusWidget("win_epg_prog_list_listview");
					GUI_SendEvent("win_epg_prog_list_listview", event); 
				}
				else if(FAVORITE_LIST_WIN == app_win_get_focus_video_window()) 
				{
					GUI_SetFocusWidget("win_favorite_prog_listview");
					GUI_SendEvent("win_favorite_prog_listview", event); 
				}					
				else	if (FULL_SCREEN_WIN == app_win_get_focus_video_window())
				{
					if (TRUE == app_get_win_create_flag(PROGRAM_BAR_WIN))
					{
						/*
						* 信息条显示状态下，按键发送到信息条
						*/
						GUI_SendEvent("win_prog_bar", event);
					}
					else
					{
						GUI_SendEvent("win_full_screen", event);
					}
				}						
				return EVENT_TRANSFER_STOP; 	
			case KEY_POWER:
				return EVENT_TRANSFER_KEEPON;	
			default:
				return EVENT_TRANSFER_STOP;	
		}
	}
	
	return EVENT_TRANSFER_KEEPON;
}



