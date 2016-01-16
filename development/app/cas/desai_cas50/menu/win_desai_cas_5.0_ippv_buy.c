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
#include "win_desai_cas_5.0_ippv_buy.h"
#include "desai_ca_api.h"
#include "app_common_epg.h"
#include "app_desai_cas_5.0_api_pop.h"
#if defined (DVB_AD_TYPE_DS_FLAG)||defined (DVB_AD_TYPE_DS_FLAG41)
extern void app_ds_ad_set_unauthorized_status(bool status);
#endif

static desai_cas50_ippv_buy_st ca_ippv_buy = {0};
static ST_IPPV_INFO* buyinfo = NULL;
extern void app_epg_convert_to_utc( unsigned short wDate, com_time_t* pTimeOut );
/*
* 刷新IPPV购买显示框
*/
void app_desai_cas50_ippv_buy_update(void)
{
	char buf[40]={0,};
	CAS_Dbg("tarce %s %d\n",__FUNCTION__,__LINE__);
	buyinfo = &ca_ippv_buy.ippvBuyInfo;
	sprintf(buf,"%d",buyinfo->ucProviderID);
	GUI_SetProperty("win_desai_cas50_ippv_buy_ucProviderID_value", "string", buf);
	CAS_Dbg("buyinfo->ucProviderID=%d\n",buyinfo->ucProviderID);
	memset(buf,0,40);
	memcpy(buf,buyinfo->aucIppvName,strlen((char*)(buyinfo->aucIppvName)));
	GUI_SetProperty("win_desai_cas50_ippv_buy_aucIppvName_value", "string", buf);
	CAS_Dbg("buyinfo->aucIppvName=%s\n",buyinfo->aucIppvName);
	memset(buf,0,40);
	sprintf(buf,"%d",buyinfo->usPrice);
	GUI_SetProperty("win_desai_cas50_ippv_buy_price_value", "string", buf);
	CAS_Dbg("buyinfo->usPrice=%d\n",buyinfo->usPrice);
	memset(buf,0,40);
	snprintf(buf,40,"%d.%02d.%02d %02d:%02d:%02d",buyinfo->stBeginTime.usYear,
	buyinfo->stBeginTime.ucMonth,buyinfo->stBeginTime.ucDay,buyinfo->stBeginTime.ucHour,
	buyinfo->stBeginTime.ucMinute,buyinfo->stBeginTime.ucSecond);
	GUI_SetProperty("win_desai_cas50_ippv_buy_stBeginTime_value", "string", buf);

	memset(buf,0,40);
	snprintf(buf,40,"%d.%02d.%02d %02d:%02d:%02d",buyinfo->stEndTime.usYear,
	buyinfo->stEndTime.ucMonth,buyinfo->stEndTime.ucDay,buyinfo->stEndTime.ucHour,
	buyinfo->stEndTime.ucMinute,buyinfo->stEndTime.ucSecond);
	GUI_SetProperty("win_desai_cas50_ippv_buy_stEndTime_value", "string", buf);

	UINT8  pucPurser[32]={0,};
	
	DSCA_GetPurserInfo(1, pucPurser);
	GUI_SetProperty("win_desai_cas50_ippv_buy_purse1_value", "string", pucPurser);
	memset(pucPurser,0,32);
	DSCA_GetPurserInfo(2, pucPurser);
	GUI_SetProperty("win_desai_cas50_ippv_buy_purse2_value", "string", pucPurser);
	memset(pucPurser,0,32);
	DSCA_GetPurserInfo(3, pucPurser);
	GUI_SetProperty("win_desai_cas50_ippv_buy_purse3_value", "string", pucPurser);
	memset(pucPurser,0,32);
	DSCA_GetPurserInfo(4, pucPurser);
	GUI_SetProperty("win_desai_cas50_ippv_buy_purse4_value", "string", pucPurser);
	
	ca_ippv_buy.wShowStatus= 1;
	return ;
}


int32_t app_desai_cas50_gxmsg_ca_on_event_ippv_buy(GxMsgProperty0_OnEvent* event)
{
	desai_cas50_ippv_buy_st* ippv_buy = NULL;
	char* focus_Window = (char*)GUI_GetFocusWindow();
	if (NULL == event)
		return 0;

	ippv_buy = (desai_cas50_ippv_buy_st*)(event->buf);

	if (NULL == ippv_buy)
		return 0;

	if (TRUE == ippv_buy->wStatus)
	{
		/*需要显示IPPV购买框*/
		memcpy(&ca_ippv_buy,ippv_buy,sizeof(desai_cas50_ippv_buy_st));
		if (TRUE == ca_ippv_buy.wShowStatus)
		{
			
			app_desai_cas50_ippv_buy_update();
		}
		else
		{
			if ((NULL != focus_Window)&&(strcasecmp("win_prog_bar", focus_Window) == 0))
			{
				GUI_EndDialog("win_prog_bar");
				
				GxCore_ThreadDelay(300);
				GUI_SetInterface("flush", NULL);
				GxCore_ThreadDelay(50);
			}
			GUI_CreateDialog("win_desai_cas50_ippv_buy");	
		}
	
	}
	else	
	{
		/*需要取消IPPV购买框*/
		if (TRUE == ca_ippv_buy.wShowStatus)
		{
			ca_ippv_buy.ippvBuyInfo.ucProviderID = ippv_buy->ippvBuyInfo.ucProviderID;
			DSCA_IppvBuy(0,ca_ippv_buy.ippvBuyInfo.ucProviderID);
			ca_ippv_buy.wShowStatus = FALSE;
			CAS_Dbg("tarce %s %d\n",__FUNCTION__,__LINE__);
			GUI_EndDialog("win_desai_cas50_ippv_buy");													
		}
	}
	
	return 1;
}


int32_t app_desai_cas50_gxmsg_ca_on_event_ippv_slot(GxMsgProperty0_OnEvent* event)
{
	char * language =NULL;
    char* cur_window = NULL;

    cur_window = (char*)GUI_GetFocusWindow();
    if (strcmp(cur_window,"win_prog_bar") == 0)
        GUI_EndDialog("win_prog_bar");

    #if 0

    win_dialog_show(WIN_DIALOG_ARREARS);
    #else
	language = app_flash_get_config_osd_language();
	if(language &&strcmp(language,"English") == 0)
		app_popmsg(POP_DIALOG_X_START,POP_DIALOG_Y_START,"Card is in debt",POPMSG_TYPE_OK);
	else
		app_popmsg(POP_DIALOG_X_START,POP_DIALOG_Y_START,"智能卡欠费",POPMSG_TYPE_OK);
    #endif

    return 0;
}

SIGNAL_HANDLER  int win_desai_cas50_ippv_buy_create(const char* widgetname, void *usrdata)
{
	CAS_Dbg("tarce %s %d\n",__FUNCTION__,__LINE__);
	GUI_SetFocusWidget("win_desai_cas50_ippv_buy_box");
	app_desai_cas50_ippv_buy_update();
	return 0;
}

SIGNAL_HANDLER  int win_desai_cas50_ippv_buy_destroy(const char* widgetname, void *usrdata)
{
	ca_ippv_buy.wShowStatus= 0;
	return 0;	
}

SIGNAL_HANDLER int win_desai_cas50_ippv_buy_cancle_keypress(const char* widgetname, void *usrdata)
{
	GUI_Event *event = NULL;
//	int i = 0;
//	UINT8 ProviderID = 0;

	event = (GUI_Event *)usrdata;

	switch(event->type)
	{
	case GUI_MOUSEBUTTONDOWN:
		break;

	case GUI_KEYDOWN:
		switch(event->key.sym)
		{
		case KEY_OK:
			DSCA_IppvBuy(0,buyinfo->ucProviderID);
			GUI_EndDialog("win_desai_cas50_ippv_buy");	
			return EVENT_TRANSFER_STOP; 
		case KEY_EXIT:
			return EVENT_TRANSFER_KEEPON;	
			break;
		case KEY_LEFT:
		case KEY_RIGHT:
			GUI_SetFocusWidget("win_desai_cas50_ippv_buy_ok");
			return EVENT_TRANSFER_STOP; 
		case KEY_UP:
		case KEY_DOWN:
			DSCA_IppvBuy(0,buyinfo->ucProviderID);
			ca_ippv_buy.wShowStatus = FALSE;
			GUI_EndDialog("win_desai_cas50_ippv_buy");		

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
			else if (FULL_SCREEN_WIN == app_win_get_focus_video_window())
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
		default:
			return EVENT_TRANSFER_KEEPON;	
			break;
		}

	default:
		return EVENT_TRANSFER_KEEPON;	
	}
	return EVENT_TRANSFER_KEEPON;	
}

SIGNAL_HANDLER int win_desai_cas50_ippv_buy_ok_keypress(const char* widgetname, void *usrdata)
{
	GUI_Event *event = NULL;
//	int i = 0;
//	UINT8 ProviderID = 0;

	event = (GUI_Event *)usrdata;

	switch(event->type)
	{
	case GUI_MOUSEBUTTONDOWN:
		break;

	case GUI_KEYDOWN:
		switch(event->key.sym)
		{
		case KEY_OK:
			GUI_SetFocusWidget("win_desai_cas50_ippv_buy_box");
			return EVENT_TRANSFER_STOP; 
		case KEY_LEFT:
		case KEY_RIGHT:
			GUI_SetFocusWidget("win_desai_cas50_ippv_buy_cancle");
			return EVENT_TRANSFER_STOP; 
		case KEY_EXIT:
			break;
		case KEY_UP:
		case KEY_DOWN:
			DSCA_IppvBuy(0,buyinfo->ucProviderID);
			ca_ippv_buy.wShowStatus = FALSE;
			GUI_EndDialog("win_desai_cas50_ippv_buy");		

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
		default:
			break;
		}

	default:
		return EVENT_TRANSFER_KEEPON;	
	}
	return EVENT_TRANSFER_KEEPON;	
}

SIGNAL_HANDLER int win_desai_cas50_ippv_buy_box_keypress(const char* widgetname, void *usrdata)
{
	GUI_Event *event = NULL;
	int i = 0;
//	UINT8 ProviderID = 0;

	event = (GUI_Event *)usrdata;

	switch(event->type)
	{
	case GUI_MOUSEBUTTONDOWN:
		break;

	case GUI_KEYDOWN:
		switch(event->key.sym)
		{
		case KEY_EXIT:
			return EVENT_TRANSFER_KEEPON;	
			break;
		case KEY_UP:
			GUI_GetProperty("win_desai_cas50_ippv_buy_box", "select", &i);
			if (0 == i)
			{
				DSCA_IppvBuy(0,buyinfo->ucProviderID);
				ca_ippv_buy.wShowStatus = FALSE;
				GUI_EndDialog("win_desai_cas50_ippv_buy");		

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
				else if (FULL_SCREEN_WIN == app_win_get_focus_video_window())
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
			}
			else
			{
				return EVENT_TRANSFER_KEEPON;				
			}
			break;
		case KEY_DOWN:
			GUI_GetProperty("win_desai_cas50_ippv_buy_box", "select", &i);
			if (0 == i)
			{
				DSCA_IppvBuy(0,buyinfo->ucProviderID);
				ca_ippv_buy.wShowStatus = FALSE;
				GUI_EndDialog("win_desai_cas50_ippv_buy");	
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
				else if (FULL_SCREEN_WIN == app_win_get_focus_video_window())
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
			}
			else
			{
				return EVENT_TRANSFER_KEEPON;				
			}
			return EVENT_TRANSFER_KEEPON;	
			break;	
		default:
			return EVENT_TRANSFER_KEEPON;	
			break;
		}

	default:
		return EVENT_TRANSFER_KEEPON;	
	}
	return EVENT_TRANSFER_KEEPON;	
}

SIGNAL_HANDLER  int win_desai_cas50_ippv_buy_keypress(const char* widgetname, void *usrdata)
{
	GUI_Event *event = NULL;
	event = (GUI_Event *)usrdata;
	char* osd_language=NULL;
	EN_ERR_CAS ret;
	UINT8 ucPin[3] , ucOddTimes;
	int temp;
	int value;
	char des[7];
	char buf[40];
	
	if(event->type == GUI_KEYDOWN)
	{	
		switch(event->key.sym)
		{
			case KEY_EXIT:
				/* IPPV节目不购买 */
				DSCA_IppvBuy(0,buyinfo->ucProviderID);
				GUI_EndDialog("win_desai_cas50_ippv_buy");	
				return EVENT_TRANSFER_STOP;	
			case KEY_MENU:
				/* IPPV节目不购买 */
				DSCA_IppvBuy(0,buyinfo->ucProviderID);
				GUI_EndDialog("win_desai_cas50_ippv_buy");	
				 return EVENT_TRANSFER_STOP;	
			case KEY_OK:
				GUI_GetProperty("win_desai_cas50_ippv_buy_purse_boxitem1_edit", "string", &value);
				memcpy(des,(char *)value,6);
				des[6]='\0';
				if(strlen(des)!=6)
				{
					osd_language = app_flash_get_config_osd_language();
					if (0 == strcmp(osd_language,LANGUAGE_CHINESE))
						app_popmsg(POP_DIALOG_X_START, POP_DIALOG_Y_START,"PIN码必须为6位",POPMSG_TYPE_OK);
					else
						app_popmsg(POP_DIALOG_X_START, POP_DIALOG_Y_START,"PIN length must be six!",POPMSG_TYPE_OK);
					GUI_SetProperty("win_desai_cas50_ippv_buy_purse_boxitem1_edit","clear",NULL);
					return EVENT_TRANSFER_STOP;	
				}
				printf("\ninput %s\n",des);
				temp=atoi(des);
				ucPin[0]=(UINT8)(temp>>16);
				ucPin[1]=(UINT8)(temp>>8);
				ucPin[2]=(UINT8)(temp&0xff);
				ret=DSCA_PurseCheckPin(ucPin,&ucOddTimes);
				printf("\nippv ret=%d\n",ret);
				if(ret==CA_CARD_CHECK_PIN_ERROR)
				{   
					osd_language = app_flash_get_config_osd_language();
					if (0 == strcmp(osd_language,LANGUAGE_CHINESE))
						snprintf(buf,40,"PIN码错误，你还可以试 %d 次",ucOddTimes);
					else
						snprintf(buf,40,"Pin Err，you can try %d times",ucOddTimes);
					app_popmsg(POP_DIALOG_X_START, POP_DIALOG_Y_START,buf, POPMSG_TYPE_OK);

					GUI_SetProperty("win_desai_cas50_ippv_buy_purse_boxitem1_edit", "clear", NULL);
				    	return EVENT_TRANSFER_STOP;
				}
				else if(ret==CA_CARD_CHECK_PIN_LOCK)//密码被锁。
				{   
					osd_language = app_flash_get_config_osd_language();
					if (0 == strcmp(osd_language,LANGUAGE_CHINESE))
						app_popmsg(POP_DIALOG_X_START, POP_DIALOG_Y_START,"卡被锁定", POPMSG_TYPE_OK);
					else
						app_popmsg(POP_DIALOG_X_START, POP_DIALOG_Y_START,"Card is locked", POPMSG_TYPE_OK);

			  		//GUI_EndDialog("win_desai_cas50_ippv_buy");
					return EVENT_TRANSFER_STOP;	
				}
				else
				{
					ret=DSCA_IppvBuy(1,buyinfo->ucProviderID);
				}
				
				if(ret == CA_ERROR_IC_INVALID)
				{
					osd_language = app_flash_get_config_osd_language();
					if (0 == strcmp(osd_language,LANGUAGE_CHINESE))
						app_popmsg(POP_DIALOG_X_START, POP_DIALOG_Y_START,"无效卡", POPMSG_TYPE_OK);
					else
						app_popmsg(POP_DIALOG_X_START, POP_DIALOG_Y_START,"Invalid card", POPMSG_TYPE_OK);

					//GUI_EndDialog("win_desai_cas50_ippv_buy");
					return EVENT_TRANSFER_STOP;	
					
				}
				if(ret == CA_CARD_SUCCESS)
				{
					osd_language = app_flash_get_config_osd_language();
					if (0 == strcmp(osd_language,LANGUAGE_CHINESE))
						app_popmsg(POP_DIALOG_X_START, POP_DIALOG_Y_START,"购买成功", POPMSG_TYPE_OK);
					else
						app_popmsg(POP_DIALOG_X_START, POP_DIALOG_Y_START,"Buy successfully!", POPMSG_TYPE_OK);

#if defined (DVB_AD_TYPE_DS_FLAG)||defined (DVB_AD_TYPE_DS_FLAG41)
					app_ds_ad_set_unauthorized_status(FALSE);
#endif
				}
				if(ret == CA_CARD_WRITEPURSER_ERROR)
				{
					osd_language = app_flash_get_config_osd_language();
					if (0 == strcmp(osd_language,LANGUAGE_CHINESE))
						app_popmsg(POP_DIALOG_X_START, POP_DIALOG_Y_START,"写钱包失败", POPMSG_TYPE_OK);
					else
						app_popmsg(POP_DIALOG_X_START, POP_DIALOG_Y_START,"Write purser error", POPMSG_TYPE_OK);

					//GUI_EndDialog("win_desai_cas50_ippv_buy");
					return EVENT_TRANSFER_STOP;	
					
				}
				if(ret == CA_ERROR_PARAMETER)
				{
					osd_language = app_flash_get_config_osd_language();
					if (0 == strcmp(osd_language,LANGUAGE_CHINESE))
						app_popmsg(POP_DIALOG_X_START, POP_DIALOG_Y_START,"卡参数错误", POPMSG_TYPE_OK);
					else
						app_popmsg(POP_DIALOG_X_START, POP_DIALOG_Y_START,"Card parameter error", POPMSG_TYPE_OK);

					//GUI_EndDialog("win_desai_cas50_ippv_buy");
					return EVENT_TRANSFER_STOP;	
					
				}

				if(ret == CA_CARD_NOMONEY)
				{
					osd_language = app_flash_get_config_osd_language();
					if (0 == strcmp(osd_language,LANGUAGE_CHINESE))
						app_popmsg(POP_DIALOG_X_START, POP_DIALOG_Y_START,"卡内金额不足", POPMSG_TYPE_OK);
					else
						app_popmsg(POP_DIALOG_X_START, POP_DIALOG_Y_START,"Not enough money in card", POPMSG_TYPE_OK);

					//GUI_EndDialog("win_desai_cas50_ippv_buy");

					 return EVENT_TRANSFER_STOP;	
				}
				GUI_EndDialog("win_desai_cas50_ippv_buy");

				return EVENT_TRANSFER_STOP;	
			case KEY_UP:
			case KEY_DOWN:
				return EVENT_TRANSFER_STOP;					
			case KEY_POWER:
				return EVENT_TRANSFER_KEEPON;	
			default:
				return EVENT_TRANSFER_STOP;	
		}
	}
	
	return EVENT_TRANSFER_KEEPON;
}



