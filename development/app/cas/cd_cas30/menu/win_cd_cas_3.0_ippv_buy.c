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
#include "win_cd_cas_3.0_ippv_buy.h"
#include "CDCASS.h"
#include "app_common_epg.h"
#include "app_cd_cas_3.0_api_pop.h"


static cd_cas30_ippv_buy_st ca_ippv_buy = {0};
static 	SCDCAIppvBuyInfo* buyinfo = NULL;
extern void app_epg_convert_to_utc( unsigned short wDate, com_time_t* pTimeOut );
extern void cd_cas30_finger_show(uint8_t number,uint8_t showFlag,char* message);
extern void cdcas_redisplay_finger(void);
/*
* 刷新IPPV购买显示框
*/
void app_cd_cas30_ippv_buy_update(void)
{
	char str[30]={0};
	uint8_t i = 0;
    com_time_t  Time_t          =
    {
        0
    };
	 buyinfo = &ca_ippv_buy.ippvBuyInfo;
	switch(ca_ippv_buy.byMessageType)
	{
		case  CDCA_IPPV_FREEVIEWED_SEGMENT:    /* IPPV免费预览阶段，是否购买 */
			GUI_SetProperty("win_cd_cas30_ippv_buy_title", "string", "IPPV Buy");
			GUI_SetProperty("win_cd_cas30_ippt_buy_tip","state","hide");				
			sprintf(str,"%u",(unsigned int)buyinfo->m_dwProductID);
			GUI_SetProperty("win_cd_cas30_ippv_buy_id_value", "string", str);				
			GUI_SetProperty("win_cd_cas30_ippv_buy_overdue", "string", "Expired Date");	

			app_epg_convert_to_utc( buyinfo->m_wIPPVTime.m_wExpiredDate, &Time_t );
				sprintf( str, "%04d-%02d-%02d", Time_t.year, Time_t.month, Time_t.day );
			GUI_SetProperty("win_cd_cas30_ippv_buy_overdue_value", "string", str);					

			GUI_SetProperty("win_cd_cas30_ippv_buy_no_price_value", "string", "0");
			GUI_SetProperty("win_cd_cas30_ippv_buy_price_value", "string", "0");	
			for (i=0; i<buyinfo->m_byPriceNum;i++)
			{
				if (CDCA_IPPVPRICETYPE_TPPVVIEW ==buyinfo->m_Price[i].m_byPriceCode)
				{
					memset(str,0,30);
					sprintf(str,"%d",buyinfo->m_Price[i].m_wPrice);				
					GUI_SetProperty("win_cd_cas30_ippv_buy_no_price_value", "string", str);									
				}
				else if (CDCA_IPPVPRICETYPE_TPPVVIEWTAPING ==buyinfo->m_Price[i].m_byPriceCode)
				{
					memset(str,0,30);
					sprintf(str,"%d",buyinfo->m_Price[i].m_wPrice);			
					GUI_SetProperty("win_cd_cas30_ippv_buy_price_value", "string", str);										
				}
			}
			
			memset(str,0,30);
			sprintf(str,"%d",buyinfo->m_bySlotID);				
			GUI_SetProperty("win_cd_cas30_ippv_buy_purse_value", "string", str);

			memset(str,0,30);
			sprintf(str,"%d",buyinfo->m_wTvsID);				
			GUI_SetProperty("win_cd_cas30_ippv_buy_purse_tvs_value", "string", str);						
			
			break;
		case CDCA_IPPV_PAYVIEWED_SEGMENT:	/* IPPV收费阶段，是否购买 */
			//GUI_SetProperty("win_cd_cas30_ippv_buy_title", "string", "IPPV CHARGE BUY");	
			GUI_SetProperty("win_cd_cas30_ippt_buy_tip","state","hide");
			sprintf(str,"%u",(unsigned int)buyinfo->m_dwProductID);
			GUI_SetProperty("win_cd_cas30_ippv_buy_id_value", "string", str);					

			GUI_SetProperty("win_cd_cas30_ippv_buy_overdue", "string", "Expired Date");					
			app_epg_convert_to_utc( buyinfo->m_wIPPVTime.m_wExpiredDate, &Time_t );
				sprintf( str, "%04d-%02d-%02d", Time_t.year, Time_t.month, Time_t.day );				
			GUI_SetProperty("win_cd_cas30_ippv_buy_overdue_value", "string", str);					
			
			GUI_SetProperty("win_cd_cas30_ippv_buy_no_price_value", "string", "0");
			GUI_SetProperty("win_cd_cas30_ippv_buy_price_value", "string", "0");	
			for (i=0; i<buyinfo->m_byPriceNum;i++)
			{
				if (CDCA_IPPVPRICETYPE_TPPVVIEW ==buyinfo->m_Price[i].m_byPriceCode)
				{
					memset(str,0,30);
					sprintf(str,"%d",buyinfo->m_Price[i].m_wPrice);				
					GUI_SetProperty("win_cd_cas30_ippv_buy_no_price_value", "string", str);									
				}
				else if (CDCA_IPPVPRICETYPE_TPPVVIEWTAPING ==buyinfo->m_Price[i].m_byPriceCode)
				{
					memset(str,0,30);
					sprintf(str,"%d",buyinfo->m_Price[i].m_wPrice);			
					GUI_SetProperty("win_cd_cas30_ippv_buy_price_value", "string", str);										
				}
			}
			
			memset(str,0,30);
			sprintf(str,"%d",buyinfo->m_bySlotID);				
			GUI_SetProperty("win_cd_cas30_ippv_buy_purse_value", "string", str);	

			memset(str,0,30);
			sprintf(str,"%d",buyinfo->m_wTvsID);				
			GUI_SetProperty("win_cd_cas30_ippv_buy_purse_tvs_value", "string", str);						
			break;
		case CDCA_IPPT_PAYVIEWED_SEGMENT:	/* IPPT收费段，是否购买 */
			GUI_SetProperty("win_cd_cas30_ippv_buy_title", "string", "IPPT Buy");	
			GUI_SetProperty("win_cd_cas30_ippt_buy_tip","state","show");				
			GUI_SetProperty("win_cd_cas30_ippt_buy_tip", "string", "System will automatic deduction when watching program");	
			
			sprintf(str,"%u",(unsigned int)buyinfo->m_dwProductID);
			GUI_SetProperty("win_cd_cas30_ippv_buy_id_value", "string", str);				
			
			GUI_SetProperty("win_cd_cas30_ippv_buy_overdue", "string", "IPPT Interval");		
			sprintf(str,"%d",buyinfo->m_wIPPVTime.m_wIntervalMin);				
			GUI_SetProperty("win_cd_cas30_ippv_buy_overdue_value", "string", str);					
			

			GUI_SetProperty("win_cd_cas30_ippv_buy_no_price_value", "string", "0");
			GUI_SetProperty("win_cd_cas30_ippv_buy_price_value", "string", "0");	
			for (i=0; i<buyinfo->m_byPriceNum;i++)
			{
				if (CDCA_IPPVPRICETYPE_TPPVVIEW ==buyinfo->m_Price[i].m_byPriceCode)
				{
					memset(str,0,30);
					sprintf(str,"%d",buyinfo->m_Price[i].m_wPrice);				
					GUI_SetProperty("win_cd_cas30_ippv_buy_no_price_value", "string", str);									
				}
				else if (CDCA_IPPVPRICETYPE_TPPVVIEWTAPING ==buyinfo->m_Price[i].m_byPriceCode)
				{
					memset(str,0,30);
					sprintf(str,"%d",buyinfo->m_Price[i].m_wPrice);			
					GUI_SetProperty("win_cd_cas30_ippv_buy_price_value", "string", str);										
				}
			}
			
			memset(str,0,30);
			sprintf(str,"%d",buyinfo->m_bySlotID);				
			GUI_SetProperty("win_cd_cas30_ippv_buy_purse_value", "string", str);

			memset(str,0,30);
			sprintf(str,"%d",buyinfo->m_wTvsID);				
			GUI_SetProperty("win_cd_cas30_ippv_buy_purse_tvs_value", "string", str);						
			break;
		default:
			break;
	}
	ca_ippv_buy.wShowStatus= 1;
	return ;
}


int32_t app_cd_cas30_gxmsg_ca_on_event_ippv_buy(GxMsgProperty0_OnEvent* event)
{
	cd_cas30_ippv_buy_st* ippv_buy = NULL;
	char* focus_Window = (char*)GUI_GetFocusWindow();
	if (NULL == event)
		return 0;

	ippv_buy = (cd_cas30_ippv_buy_st*)(event->buf);

	if (NULL == ippv_buy)
		return 0;

	if (TRUE == ippv_buy->wStatus)
	{
		/*需要显示IPPV购买框*/
		if (TRUE == ca_ippv_buy.wShowStatus)
		{
			/*已经显示，只需要更新显示数据*/
			memcpy(&ca_ippv_buy,ippv_buy,sizeof(cd_cas30_ippv_buy_st));
			if(app_get_pop_msg_flag_status() == FALSE)
				app_cd_cas30_ippv_buy_update();
			ca_ippv_buy.wShowStatus = TRUE;
			GUI_SetInterface("flush", NULL);
		}
		else
		{
			/*从未显示，需要创建窗体*/
			memcpy(&ca_ippv_buy,ippv_buy,sizeof(cd_cas30_ippv_buy_st));
			if ((NULL != focus_Window)&&(strcasecmp("win_prog_bar", focus_Window) == 0))
				GUI_EndDialog("win_prog_bar");
			/*关闭全屏下的指纹，显示IPPV菜单中的指纹*/
			cd_cas30_finger_show(0,DVB_CA_FINGER_FLAG_HIDE,(char*)NULL);
			GUI_CreateDialog("win_cd_cas30_ippv_buy");	
			ca_ippv_buy.wShowStatus = TRUE;
			GUI_SetInterface("flush", NULL);
			cdcas_redisplay_finger();
		}
	
	}
	else	
	{
		/*需要取消IPPV购买框*/
		if (TRUE == ca_ippv_buy.wShowStatus)
		{
			ca_ippv_buy.wEcmPid = ippv_buy->wEcmPid;
			CDCASTB_StopIPPVBuyDlg( CDCA_FALSE,
                                ca_ippv_buy.wEcmPid,
                                NULL,
                                NULL );
			GUI_EndDialog("win_cd_cas30_ippv_buy");													
			ca_ippv_buy.wShowStatus = FALSE;
		}
	}
	
	return 1;
}

SIGNAL_HANDLER  int win_cd_cas30_ippv_buy_create(const char* widgetname, void *usrdata)
{
	char* osd_language=NULL;
    osd_language = app_flash_get_config_osd_language();
	if (0 == strcmp(osd_language,LANGUAGE_CHINESE))
    {
 //   	GUI_SetProperty("win_cd_cas30_ippv_buy_image_cancel", "img", "tips_exit.bmp");
//		GUI_SetProperty("win_cd_cas30_ippv_buy_image_confirm", "img", "tips_confirm.bmp");
    }
    else if(0 == strcmp(osd_language,LANGUAGE_ENGLISH))
     {
   // 	GUI_SetProperty("win_cd_cas30_ippv_buy_image_cancel", "img", "tips_exit_e.bmp");
//		GUI_SetProperty("win_cd_cas30_ippv_buy_image_confirm", "img", "tips_confirm_e2.bmp");
    }
	app_cd_cas30_ippv_buy_update();
	return 0;
}

SIGNAL_HANDLER  int win_cd_cas30_ippv_buy_destroy(const char* widgetname, void *usrdata)
{
	ca_ippv_buy.wShowStatus= 0;
	cdcas_redisplay_finger();
	return 0;	
}

SIGNAL_HANDLER int win_cd_cas30_ippv_buy_box_keypress(const char* widgetname, void *usrdata)
{
	GUI_Event *event = NULL;
	int i = 0;
	u_int16_t ecmpid = 0x1fff;

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
			GUI_GetProperty("win_cd_cas30_ippv_buy_box", "select", &i);
			if (0 == i)
			{
				ecmpid = ca_ippv_buy.wEcmPid;
				CDCASTB_StopIPPVBuyDlg( CDCA_FALSE,
                                    ecmpid,
                                    NULL,
                                    NULL );
			 	GUI_EndDialog("win_cd_cas30_ippv_buy");
				ca_ippv_buy.wShowStatus = FALSE;
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
			}
			else
			{
				return EVENT_TRANSFER_KEEPON;				
			}
			break;
		case KEY_DOWN:
			GUI_GetProperty("win_cd_cas30_ippv_buy_box", "select", &i);
			if (1 == i)
			{
				ecmpid = ca_ippv_buy.wEcmPid;
				CDCASTB_StopIPPVBuyDlg( CDCA_FALSE,
                                    ecmpid,
                                    NULL,
                                    NULL );
			 	GUI_EndDialog("win_cd_cas30_ippv_buy");
				ca_ippv_buy.wShowStatus = FALSE;
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

SIGNAL_HANDLER  int win_cd_cas30_ippv_buy_keypress(const char* widgetname, void *usrdata)
{
	GUI_Event *event = NULL;
	event = (GUI_Event *)usrdata;
	uint8_t newPassword1[CDCA_MAXLEN_PINCODE+1]={0};
	uint8_t newPasswordValue1[CDCA_MAXLEN_PINCODE+1]={0};
	uint8_t *value;
	popmsg_ret ret;
	u_int16_t ecmpid = 0x1fff;
	CDCA_U16   rtn;
	uint32_t ratevalue;
	uint8_t i = 0;
	CDCA_U16  m_wPrice_tppvview=0;
	CDCA_U16  m_wPrice_tppvviewtapping=0;
	SCDCAIPPVPrice buyIPPVPrice = {0};

	 ecmpid = ca_ippv_buy.wEcmPid;
	 
	if(event->type == GUI_KEYDOWN)
	{	
		switch(event->key.sym)
		{
			case KEY_EXIT:
				/* IPPV节目不购买 */
				  CDCASTB_StopIPPVBuyDlg( CDCA_FALSE,
                                        ecmpid,
                                        NULL,
                                        NULL );
				 GUI_EndDialog("win_cd_cas30_ippv_buy");
				 ca_ippv_buy.wShowStatus = FALSE;
				return EVENT_TRANSFER_STOP;	
			case KEY_RECALL:
				/* IPPV节目不购买 */
				  CDCASTB_StopIPPVBuyDlg( CDCA_FALSE,
                                        ecmpid,
                                        NULL,
                                        NULL );	
				 GUI_EndDialog("win_cd_cas30_ippv_buy");
				 ca_ippv_buy.wShowStatus = FALSE;
				 return EVENT_TRANSFER_STOP;	
			case KEY_OK:
				/*首先匹配原始密码*/
				
				GUI_GetProperty("win_cd_cas30_ippv_buy_purse_boxitem2_edit", "string", &value);
				memset(newPassword1, 0, CDCA_MAXLEN_PINCODE+1);
				memcpy(newPassword1,(char*)value, CDCA_MAXLEN_PINCODE);


				newPasswordValue1[0] = newPassword1[0]-0x30;
				newPasswordValue1[1] = newPassword1[1]-0x30;
				newPasswordValue1[2] = newPassword1[2]-0x30;
				newPasswordValue1[3] = newPassword1[3]-0x30;
				newPasswordValue1[4] = newPassword1[4]-0x30;
				newPasswordValue1[5] = newPassword1[5]-0x30;

				if(strlen((char*)newPassword1)!=6)
				{
					GUI_SetProperty("win_cd_cas30_ippv_buy_purse_boxitem2_edit","clear",NULL);
					ret = app_popmsg(210,200,"Invalid PIN",POPMSG_TYPE_OK);
					return EVENT_TRANSFER_STOP;	
				}


				for (i=0; i<buyinfo->m_byPriceNum;i++)
				{
					if (CDCA_IPPVPRICETYPE_TPPVVIEW ==buyinfo->m_Price[i].m_byPriceCode)
					{
						m_wPrice_tppvview = buyinfo->m_Price[i].m_wPrice;								
					}
 					else if (CDCA_IPPVPRICETYPE_TPPVVIEWTAPING ==buyinfo->m_Price[i].m_byPriceCode)
					{
						m_wPrice_tppvviewtapping = buyinfo->m_Price[i].m_wPrice;									
					}
				}
				
				/*是否可录像*/
				GUI_GetProperty("win_cd_cas30_rating_boxitem1_value", "select", &ratevalue);
				if (0 == ratevalue)
				{
					buyIPPVPrice.m_byPriceCode = CDCA_IPPVPRICETYPE_TPPVVIEW;
					buyIPPVPrice.m_wPrice = m_wPrice_tppvview;
					// 不可录像						
				}
				else
				{
					buyIPPVPrice.m_byPriceCode = CDCA_IPPVPRICETYPE_TPPVVIEWTAPING;
					buyIPPVPrice.m_wPrice = m_wPrice_tppvviewtapping;
					// 可录像
				}


				/* IPPV节目购买 */
				rtn =  CDCASTB_StopIPPVBuyDlg( CDCA_TRUE,
                                        ecmpid,
                                        newPasswordValue1,
                                       /*buyinfo*/&buyIPPVPrice );
				switch(rtn)
				{
					case CDCA_RC_OK:
						if(ca_ippv_buy.byMessageType == CDCA_IPPV_FREEVIEWED_SEGMENT
							|| ca_ippv_buy.byMessageType == CDCA_IPPV_PAYVIEWED_SEGMENT)
						{
						ret = app_popmsg(210,200,"IPPV product purchased successfully",POPMSG_TYPE_OK);
						}
						else if(ca_ippv_buy.byMessageType == CDCA_IPPT_PAYVIEWED_SEGMENT)
						{
							ret = app_popmsg(210,200,"IPPT product purchased successfully",POPMSG_TYPE_OK);
						}
						GUI_EndDialog("win_cd_cas30_ippv_buy");
						ca_ippv_buy.wShowStatus = FALSE;
						printf("购买成功\n");
						break;
					case CDCA_RC_CARD_INVALID:
						ret = app_popmsg(210,200,"Invalid Smartcard",POPMSG_TYPE_OK);							
						printf("智能卡不在机顶盒或无效卡无法购买\n");
						break;
					case CDCA_RC_CARD_NO_ROOM:
						ret = app_popmsg(210,200,"No room for new IPPV",POPMSG_TYPE_OK);										
						printf("智能卡没有空间\n");
						break;						
						break;	
					case CDCA_RC_PIN_INVALID:
						ret = app_popmsg(210,200,"Invalid PIN",POPMSG_TYPE_OK);								
						printf("PIN码无效\n");
						break;	
					case CDCA_RC_POINTER_INVALID:
						ret = app_popmsg(210,200,"Pointer Invaild",POPMSG_TYPE_OK); 							
						printf("价格指针为空\n");
						break;
					case CDCA_RC_PROG_STATUS_INVALID:
						ret = app_popmsg(210,200,"Program Status Invalid",POPMSG_TYPE_OK);								
						printf("价格状态无效\n");
					case CDCA_RC_DATA_NOT_FIND:
						ret = app_popmsg(210,200,"Data Not Find",POPMSG_TYPE_OK);	
						printf("没有找到所要的数据\n");
						break;
					default:
						ret = app_popmsg(210,200,"Other Error",POPMSG_TYPE_OK);								
						printf("其他错误\n");
						break;										
				}
				
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



