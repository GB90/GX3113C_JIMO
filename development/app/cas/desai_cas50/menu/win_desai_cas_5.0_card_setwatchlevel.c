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
#include "sys_common_config.h"
#include "app_win_interface.h"
#include "desai_ca_api.h"

SIGNAL_HANDLER  int win_card_setwatchlevel_create(const char* widgetname, void *usrdata)
{
	UINT32 value;
	UINT8 pucWorkLevel = 0;
	EN_ERR_CAS ret;
	ret = DSCA_GetWorkLevel(&pucWorkLevel);
	CAS_Dbg("pucWorkLevel=%d ret=%d \n",pucWorkLevel,ret);
	if(pucWorkLevel == 0)
		value = 1;
	else
		value=(INT32)pucWorkLevel-1;
	GUI_SetProperty("win_desai_cas50_card_setwatchlevel_boxitem1_combobox", "select", &value);	
	return  0;
}

SIGNAL_HANDLER  int win_card_setwatchlevel_destroy(const char* widgetname, void *usrdata)
{
	return 0;
}

SIGNAL_HANDLER  int win_card_setwatchlevel_keypress(const char* widgetname, void *usrdata)
{
	GUI_Event *event = NULL;
	event = (GUI_Event *)usrdata;
	
	if(event->type == GUI_KEYDOWN)
	{
		switch(event->key.sym)
		{
			case KEY_EXIT:
			case KEY_MENU:
				GUI_EndDialog("win_desai_cas50_card_setwatchlevel");
				return EVENT_TRANSFER_STOP;
			case KEY_OK:
				GUI_EndDialog("win_desai_cas50_card_setwatchlevel");
				return EVENT_TRANSFER_STOP;
			case KEY_POWER:
				return EVENT_TRANSFER_KEEPON;
			default:
				return EVENT_TRANSFER_STOP;
		}
	}

	return EVENT_TRANSFER_KEEPON;
}
SIGNAL_HANDLER  int win_card_setwatchlevel_box_keypress(const char* widgetname, void *usrdata)
{
	GUI_Event *event = NULL;
	event = (GUI_Event *)usrdata;
	INT32 value;
	INT8 pucPin[7]={0,};
	UINT8  pucOddTimes;

	UINT8 level=0;
	EN_ERR_CAS ret;
	char buf[40];

	UINT8 newPin[3]={0,};
	int temp=0;
	char* osd_language=NULL;
	
	if(event->type == GUI_KEYDOWN)
	{	
		switch(event->key.sym)
		{
			case KEY_OK:
				{
					GUI_GetProperty("win_desai_cas50_card_setwatchlevel_boxitem2_edit", "string", &value);
					memcpy(pucPin,(char*)value,6);
					if(strlen(pucPin) !=6)
					{	
						osd_language = app_flash_get_config_osd_language();
						if (0 == strcmp(osd_language,LANGUAGE_CHINESE))
							app_popmsg(POP_DIALOG_X_START, POP_DIALOG_Y_START,"PIN码必须为6位",POPMSG_TYPE_OK);
						else
							app_popmsg(POP_DIALOG_X_START, POP_DIALOG_Y_START,"PIN length must be six!",POPMSG_TYPE_OK);
						GUI_SetProperty("win_desai_cas50_card_setwatchlevel_boxitem2_edit","clear",NULL);
						return EVENT_TRANSFER_STOP;	
					}
					temp=atoi(pucPin);
					newPin[0]=(char)(temp>>16);
					newPin[1]=(char)(temp>>8);
					newPin[2]=(char)(temp&0xff);

					GUI_GetProperty("win_desai_cas50_card_setwatchlevel_boxitem1_combobox", "select", &value);

					level=(UINT8)(value&0xff)+1;

					ret=DSCA_SetWatchLevel(newPin, &pucOddTimes, &level);
					CAS_Dbg("level=%d ret=%d \n",level,ret);
					
					if(ret == CA_CARD_CHECK_PIN_ERROR)
					{
						osd_language = app_flash_get_config_osd_language();
						if (0 == strcmp(osd_language,LANGUAGE_CHINESE))
							snprintf(buf,40,"PIN码错误，你还可以试 %d 次",pucOddTimes);
						else
							snprintf(buf,40,"Pin Err，you can try %d times",pucOddTimes);
						app_popmsg(POP_DIALOG_X_START, POP_DIALOG_Y_START,buf, POPMSG_TYPE_OK);
						GUI_SetProperty("win_desai_cas50_card_setwatchlevel_boxitem2_edit","clear",NULL);
						return EVENT_TRANSFER_STOP;	
					}
					if(ret == CA_CARD_CHECK_PIN_LOCK)
					{
						osd_language = app_flash_get_config_osd_language();
						if (0 == strcmp(osd_language,LANGUAGE_CHINESE))
							app_popmsg(POP_DIALOG_X_START, POP_DIALOG_Y_START,"卡被锁定", POPMSG_TYPE_OK);
						else
							app_popmsg(POP_DIALOG_X_START, POP_DIALOG_Y_START,"CARD_CHECK_PIN_LOCK", POPMSG_TYPE_OK);
						GUI_SetProperty("win_desai_cas50_card_setwatchlevel_boxitem2_edit","clear",NULL);
						return EVENT_TRANSFER_STOP;	
					}
					if(ret == CA_CARD_SUCCESS)
					{
						osd_language = app_flash_get_config_osd_language();
						if (0 == strcmp(osd_language,LANGUAGE_CHINESE))
							app_popmsg(POP_DIALOG_X_START, POP_DIALOG_Y_START,"设置成功!", POPMSG_TYPE_OK);
						else
							app_popmsg(POP_DIALOG_X_START, POP_DIALOG_Y_START,"Set successfully!", POPMSG_TYPE_OK);
						break;
					}
					if(ret == CA_CARD_FAILURE)
					{
						osd_language = app_flash_get_config_osd_language();
						if (0 == strcmp(osd_language,LANGUAGE_CHINESE))
							app_popmsg(POP_DIALOG_X_START, POP_DIALOG_Y_START,"卡通讯失败!", POPMSG_TYPE_OK);
						else
							app_popmsg(POP_DIALOG_X_START, POP_DIALOG_Y_START,"CARD_FAILURE", POPMSG_TYPE_OK);
						GUI_SetProperty("win_desai_cas50_card_setwatchlevel_boxitem2_edit","clear",NULL);
						return EVENT_TRANSFER_STOP;	
					}
					if(ret == CA_ERROR_PARAMETER)
					{
						osd_language = app_flash_get_config_osd_language();
						if (0 == strcmp(osd_language,LANGUAGE_CHINESE))
							app_popmsg(POP_DIALOG_X_START, POP_DIALOG_Y_START,"卡参数错误", POPMSG_TYPE_OK);
						else
							app_popmsg(POP_DIALOG_X_START, POP_DIALOG_Y_START,"ERROR_PARAMETER ", POPMSG_TYPE_OK);
						GUI_SetProperty("win_desai_cas50_card_setwatchlevel_boxitem2_edit","clear",NULL);
						return EVENT_TRANSFER_STOP;	
					}					
				}
				return EVENT_TRANSFER_KEEPON;
			default:
				return EVENT_TRANSFER_KEEPON;	
		}
	}
	return EVENT_TRANSFER_KEEPON;
}


