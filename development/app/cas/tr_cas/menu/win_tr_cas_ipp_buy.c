#include "app_common_porting_stb_api.h"
#include "app_win_interface.h"
#include "Tr_Cas.h"


#define IPP_PROG_MARK 	  	("full_screen_ipp_mark_image")
#define IMG_IPPV_GREEN  	("ippv_green.bmp")
#define IMG_IPPV_RED  	   	("ippv_red.bmp")
#define IMG_IPPT_GREEN    	("ippt_green.bmp")
#define IMG_IPPT_RED        ("ippt_red.bmp")

extern uint8_t win_trcas_feed_card_menu_show_flag(void);

static CAS_IPPNOTIFY_STRUCT stIpp_data  = {0,};
static CAS_IPPPURCHASE_STRUCT s_IppProduct = {0,};
static uint8_t bCreateIpp = FALSE;
static ca_buy_ipp_t stBuyIpp = {0,};


/*wangjian add on 20150306*/
uint8_t win_trcas_ipp_buy_menu_show_flag(void)
{
	return bCreateIpp;
}

/*wangjian add on 20150330*/
uint8_t app_trcas_ipp_indicate_show(uint8_t ipp_flag, uint8_t show_flag)
{
	//printf("[app_trcas_ipp_indicate_show]ipp_flag(%d); show_flag(%d).\n", ipp_flag, show_flag);
	
	if (0 == app_win_is_full_window())
	{
		return 1;	
	}
	
	if (0 == ipp_flag)
	{
		GUI_SetProperty(IPP_PROG_MARK, "img", IMG_IPPV_GREEN);
	}
	else if (1 == ipp_flag)
	{
		GUI_SetProperty(IPP_PROG_MARK, "img", IMG_IPPT_GREEN);
	}
	else
	{
		GUI_SetProperty(IPP_PROG_MARK, "img", IMG_IPPT_RED);
	}

	if (show_flag)
	{	
		GUI_SetProperty(IPP_PROG_MARK, "state", "show");
	}
	else
	{
		GUI_SetProperty(IPP_PROG_MARK, "state", "hide");
	}
	
	return 0;
}

void win_trcas_ipp_indicate_clear(void)
{
	static uint8_t bEnabledClear = FALSE;
	uint8_t result = 0;

	if (0 == app_prog_get_num_in_group())
	{
		if (FALSE == bEnabledClear)
		{
			result = app_trcas_ipp_indicate_show(0, FALSE);
			if (0 == result)
			{
				bEnabledClear = TRUE;
			}
		}
	}
	else
	{
		if (bEnabledClear)
		{
			bEnabledClear = FALSE;
		}
	}

	return;
}

void app_init_ipp_buy(void* pIppData)
{
	if (bCreateIpp)
	{
		if (NULL == pIppData)
		{
			printf("#[app_init_ipp_buy]CLEAR IPP Menu===#\n");
			GUI_EndDialog("win_tr_cas_ipp_buy");
		}
	
		return;
	}

	if (0 == app_win_is_full_window())
	{
		return;	
	}

	if (TRUE == win_trcas_feed_card_menu_show_flag())
	{
		return;
	}

	if (pIppData != NULL)
	{
		memset(&s_IppProduct, 0x00, sizeof(CAS_IPPPURCHASE_STRUCT));
		memset(&stIpp_data, 0x00, sizeof(CAS_IPPNOTIFY_STRUCT));
		memcpy(&stIpp_data, pIppData, sizeof(CAS_IPPNOTIFY_STRUCT));
		
		GUI_EndDialog("win_tr_cas_parent_lock");
		GUI_EndDialog("win_prog_bar");
		GUI_CreateDialog("win_tr_cas_ipp_buy");
	}
	
	return;
}

SIGNAL_HANDLER  int win_tr_ppv_create(const char* widgetname, void *usrdata)
{
	char str[30] = {0};
	char* osd_language = NULL;
	
	bCreateIpp = TRUE;
	
	osd_language = app_flash_get_config_osd_language();
	memset(str,0,30);
	sprintf(str, "%u", stIpp_data.wChannelId);
	GUI_SetProperty("win_tr_ppv_p1","string",str);
	
	memset(str,0,30);
	if (stIpp_data.bIppType == 0)
	{
		GUI_SetProperty("win_tr_ppv_p2", "string", "IPPV");
		if (0 == strcmp(osd_language,LANGUAGE_CHINESE))	
		{
			sprintf( str, "%d点", (unsigned int)stIpp_data.ulIppCharge);
		}
		else
		{
			sprintf( str, "%d point", (unsigned int)stIpp_data.ulIppCharge);
		}
	}
	else if (stIpp_data.bIppType == 1)
	{
		GUI_SetProperty("win_tr_ppv_p2", "string", "IPPT");
		if (0 == strcmp(osd_language,LANGUAGE_CHINESE))	
		{
			sprintf(str, "%d秒,%d点",(unsigned int)stIpp_data.ulIppUnitTime, (unsigned int)stIpp_data.ulIppCharge);
		}
		else
		{
			sprintf(str, "%dsecond,%dpoint",(unsigned int)stIpp_data.ulIppUnitTime, (unsigned int)stIpp_data.ulIppCharge);
		}
	}
	
	GUI_SetProperty("win_tr_ppv_p3", "string", str);

	memset(str,0,30);
	sprintf(str, "%u", (unsigned int)stIpp_data.ulIppRunningNum);
	GUI_SetProperty("win_tr_ppv_p4", "string", str);
	
	memset(str,0,30);
	sprintf( str, "%02d%02d.%d.%d-%02d:%02d:%02d",
			stIpp_data.ulIppStart.bYear[0],
			stIpp_data.ulIppStart.bYear[1],
			stIpp_data.ulIppStart.bMonth, 
			stIpp_data.ulIppStart.bDay,  
			stIpp_data.ulIppStart.bHour,  
			stIpp_data.ulIppStart.bMinute, 
			stIpp_data.ulIppStart.bSecond
	       );
	GUI_SetProperty("win_tr_ppv_p5", "string", str);

	memset(str,0,30);
	sprintf( str, "%02d%02d.%d.%d-%02d:%02d:%02d",
			stIpp_data.ulIppEnd.bYear[0],
			stIpp_data.ulIppEnd.bYear[1],
			stIpp_data.ulIppEnd.bMonth, 
			stIpp_data.ulIppEnd.bDay,  
			stIpp_data.ulIppEnd.bHour,  
			stIpp_data.ulIppEnd.bMinute, 
			stIpp_data.ulIppEnd.bSecond
	       );
	GUI_SetProperty("win_tr_ppv_p6", "string", str);					

	return 0;
}


SIGNAL_HANDLER  int win_tr_ppv_destroy(const char* widgetname, void *usrdata)
{	
	bCreateIpp = FALSE;
	return 0;
}

SIGNAL_HANDLER  int win_tr_ppv_keypress(const char* widgetname, void *usrdata)
{
	GUI_Event *event = NULL;
	
	event = (GUI_Event *)usrdata;
	if (event->type == GUI_KEYDOWN)
	{
		switch (event->key.sym)
		{
			case KEY_EXIT:
			case KEY_RECALL:
				GUI_EndDialog("win_tr_cas_ipp_buy");
				return EVENT_TRANSFER_STOP;
			case KEY_OK:
				return EVENT_TRANSFER_STOP;
			case KEY_POWER:
				return EVENT_TRANSFER_KEEPON;
			default:
				return EVENT_TRANSFER_KEEPON;
		}
	}

	return EVENT_TRANSFER_KEEPON;
}

SIGNAL_HANDLER  int app_tr_cas_pin_box_key_press(const char* widgetname, void *usrdata)
{	
	GUI_Event *event = NULL;
	uint8_t newPassword1[7] = {0};
	static uint8_t newPasswordValue1[7] = {0};
	uint8_t *value = NULL;
	popmsg_ret ret = 0;
	uint16_t wRet = 0;
	
	event = (GUI_Event *)usrdata;

	if (event->type == GUI_KEYDOWN)
	{
		switch (event->key.sym)
		{
			case KEY_EXIT:
			case KEY_RECALL:
			case KEY_MENU:
				event->key.sym = KEY_EXIT;
				return EVENT_TRANSFER_KEEPON;
				
			case KEY_OK:
				s_IppProduct.wChannelId      = stIpp_data.wChannelId;
				s_IppProduct.bIppType        = stIpp_data.bIppType;
				s_IppProduct.ulPurchaseNum   = 1;
				s_IppProduct.ulIppCharge     = stIpp_data.ulIppCharge;
				s_IppProduct.ulIppUnitTime   = stIpp_data.ulIppUnitTime;
				s_IppProduct.ulIppRunningNum = stIpp_data.ulIppRunningNum;
				
				GUI_GetProperty("win_tr_ppv_pin_edit", "string", &value);
				memset(newPassword1, 0, 7);
				memcpy(newPassword1,(char*)value, 6);

				newPasswordValue1[0] = newPassword1[0]-0x30;
				newPasswordValue1[1] = newPassword1[1]-0x30;
				newPasswordValue1[2] = newPassword1[2]-0x30;
				newPasswordValue1[3] = newPassword1[3]-0x30;
				newPasswordValue1[4] = newPassword1[4]-0x30;
				newPasswordValue1[5] = newPassword1[5]-0x30;

				if (strlen((char*)newPassword1)!=6)
				{
					GUI_SetProperty("win_tr_ppv_pin_edit","clear",NULL);
					GUI_SetProperty("win_tr_ppv_pin_hint", "string", "Error,reput again!");		
					return EVENT_TRANSFER_STOP;	
				}
				
				//wRet = MC_IppPurchase(newPasswordValue1, 6, s_IppProduct);/*购买IPP产品*/
				memset(&stBuyIpp,0x00,sizeof(ca_buy_ipp_t));
				stBuyIpp.date_type = DVB_CA_IPPV_POP_INFO;
				stBuyIpp.pin = (char*)newPasswordValue1;
				stBuyIpp.buystate = 1;
				stBuyIpp.buydata =(void*)&s_IppProduct;
				wRet = app_cas_api_buy_ipp(&stBuyIpp);
				//printf("\n app_cas_api_buy_ipp wRet=%d\n",wRet);
				switch (wRet)
				{
					case MC_OK:
						app_set_popmsg_yes_timeout(3);/*wangjian add on 20150330*/
						ret = app_popmsg(POP_DIALOG_X_START, POP_DIALOG_Y_START,"BuySuccessed",POPMSG_TYPE_OK);
						break;
					case MC_NOT_OK:
						ret = app_popmsg(POP_DIALOG_X_START, POP_DIALOG_Y_START,"Operation failed",POPMSG_TYPE_OK);							
						break;
					case MC_PARAMETER_ERROR:
						ret = app_popmsg(POP_DIALOG_X_START, POP_DIALOG_Y_START,"Para Error",POPMSG_TYPE_OK);								
						break;
					case MC_DATA_ERROR:
						ret = app_popmsg(POP_DIALOG_X_START, POP_DIALOG_Y_START,"Data Error",POPMSG_TYPE_OK);										
						break;						
					case MC_MEMORY_RW_ERROR:
						ret = app_popmsg(POP_DIALOG_X_START, POP_DIALOG_Y_START,"Memory rw Error",POPMSG_TYPE_OK);								;
						break;	
					case MC_NOT_SUPPORTED:
						ret = app_popmsg(POP_DIALOG_X_START, POP_DIALOG_Y_START,"Not Supported",POPMSG_TYPE_OK);								
						break;	
					case MC_STATE_ERROR:
						ret = app_popmsg(POP_DIALOG_X_START, POP_DIALOG_Y_START,"State Error",POPMSG_TYPE_OK);	
						break;
					case MC_SCSN_UNMATCHED:
						ret = app_popmsg(POP_DIALOG_X_START, POP_DIALOG_Y_START,"Not Matched",POPMSG_TYPE_OK);	
						break;
					default:
						ret = app_popmsg(POP_DIALOG_X_START, POP_DIALOG_Y_START,"Unknow Error",POPMSG_TYPE_OK);								
						break;		
				}		
				
				GUI_EndDialog("win_tr_cas_ipp_buy");
				return EVENT_TRANSFER_STOP;
			case KEY_UP:
			case KEY_DOWN:
				if (FULL_SCREEN_WIN == app_win_get_focus_video_window())
				{
					bCreateIpp = FALSE;
					GUI_EndDialog("win_tr_cas_ipp_buy");
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
				return EVENT_TRANSFER_KEEPON;
		}
	}

	return EVENT_TRANSFER_KEEPON;
}


