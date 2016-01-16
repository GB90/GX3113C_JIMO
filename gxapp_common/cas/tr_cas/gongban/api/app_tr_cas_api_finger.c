/*****************************************************************************
* 						   CONFIDENTIAL
*        Hangzhou GuoXin Science and Technology Co., Ltd.
*                      (C)2012, All right reserved
******************************************************************************

******************************************************************************
* File Name :	app_tr_cas_api_finger.c
* Author    : 	zhouhm
* Project   :	goxceed dvbc
* Type      :
******************************************************************************
* Purpose   :	模块头文件
******************************************************************************
* Release History:
VERSION	Date			  AUTHOR         Description
1.0  	2013.11.29		  zhouhm 	 			creation
*****************************************************************************/
#include "app_common_porting_stb_api.h"
#include "app_tr_cas_api_finger.h"
#include "gui_core.h"


extern void tr_cas_finger_show(uint8_t number, uint8_t showFlag, char* message);
extern uint8_t win_trcas_ipp_buy_menu_show_flag(void);
extern uint8_t win_trcas_feed_card_menu_show_flag(void);

static uint8_t fullscreenFlag = FALSE;
static tr_cas_finger_data_st ca_finger_data = {0};


static uint8_t trcas_finger_show_check_flag(void)
{
	char* curFocusWin = (char *)GUI_GetFocusWindow(); /*wangjian add on 20141231*/
	
	if ((NULL != curFocusWin) && (strcasecmp(curFocusWin, "win_full_screen") != 0))
	{
		return TRUE;
	}

	/*wangjian add on 20150306*/
	if ((TRUE == win_trcas_ipp_buy_menu_show_flag()) 
		|| (TRUE == win_trcas_feed_card_menu_show_flag()))
	{
		return TRUE;
	}

	return FALSE;
}

int32_t app_tr_cas_gxmsg_ca_on_event_finger(GxMsgProperty0_OnEvent* event)
{
	tr_cas_finger_data_st* finger_data = NULL;

	if (TRUE == trcas_finger_show_check_flag())
	{
		return FALSE;
	}
	
	if (NULL == event)
	{
		return 0;
	}

	finger_data = (tr_cas_finger_data_st *)(event->buf);
	if (NULL == finger_data)
	{
		return 0;
	}

	if ((0 != ca_finger_data.wStatus) && (TRUE == ca_finger_data.wShowStatus))
	{
		if (ca_finger_data.position >= 3)
		{
			tr_cas_finger_show(ca_finger_data.position, DVB_CA_FINGER_FLAG_HIDE, NULL);
		}
		else
		{
			tr_cas_finger_show(ca_finger_data.position, DVB_CA_FINGER_FLAG_TRANS_HIDE, NULL);
		}
		
		ca_finger_data.wStatus = 0;
		ca_finger_data.wShowStatus = 0;
		ca_finger_data.wDuration = 0;
		ca_finger_data.wusedTime = 0;
	}

	memcpy(&ca_finger_data, finger_data, sizeof(tr_cas_finger_data_st));
	return 1;
}

int32_t app_tr_cas_finger_exec(void)
{
	uint8_t chFingerBuf[20] = {0};	

	if (TRUE == trcas_finger_show_check_flag())
	{
		return FALSE;
	}

	if (TRUE == ca_finger_data.wStatus)	
	{
		if (FALSE == ca_finger_data.wShowStatus)
		{
			ca_finger_data.position = rand()%3;
			fullscreenFlag = FALSE;
			
			memcpy(chFingerBuf, ca_finger_data.pbHashedNum, ca_finger_data.bHashedNumLen);
			tr_cas_finger_show(ca_finger_data.position, DVB_CA_FINGER_FLAG_SHOW, (char *)chFingerBuf);

			ca_finger_data.wShowStatus = TRUE;				
		}
		else
		{
			if (ca_finger_data.wusedTime < ca_finger_data.wDuration*1000)
			{
				ca_finger_data.wusedTime += 200;	
			}
			else
			{
				tr_cas_finger_show(ca_finger_data.position,DVB_CA_FINGER_FLAG_HIDE,NULL);
				ca_finger_data.wStatus = 0;
				ca_finger_data.wShowStatus = 0;
				ca_finger_data.wDuration = 0;
				ca_finger_data.wusedTime = 0;
			}
		}	
	}	
	
	return TRUE;
}


