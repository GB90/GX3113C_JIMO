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

void app_ca_get_work_time(void)
{
	ST_WORKTIME_INFO pstWorkTime;
	char buf[40];

	DSCA_GetWorkTime(&pstWorkTime);
	printf("\n  %02d:%02d:%02d",pstWorkTime.stBeginTime.ucHour,pstWorkTime.stBeginTime.ucMinute,
			pstWorkTime.stBeginTime.ucSecond);
	printf("\n  %02d:%02d:%02d",pstWorkTime.stEndTime.ucHour,pstWorkTime.stEndTime.ucMinute,
			pstWorkTime.stEndTime.ucSecond);

	snprintf(buf,40,"%02d:%02d",pstWorkTime.stBeginTime.ucHour,pstWorkTime.stBeginTime.ucMinute);
	GUI_SetProperty("win_desai_cas50_card_setworktime_boxitem1_edit", "string",buf);
	
	memset(buf,0,40);
	snprintf(buf,40,"%02d:%02d",pstWorkTime.stEndTime.ucHour,pstWorkTime.stEndTime.ucMinute);
	GUI_SetProperty("win_desai_cas50_card_setworktime_boxitem2_edit", "string",buf);

}


SIGNAL_HANDLER  int win_card_setworktime_create(const char* widgetname, void *usrdata)
{
	app_ca_get_work_time();
	return 0;
}

SIGNAL_HANDLER  int win_card_setworktime_destroy(const char* widgetname, void *usrdata)
{
	return 0;
}

SIGNAL_HANDLER  int win_card_setworktime_keypress(const char* widgetname, void *usrdata)
{
	GUI_Event *event = NULL;
	event = (GUI_Event *)usrdata;
	
	if(event->type == GUI_KEYDOWN)
	{
		switch(event->key.sym)
		{
			case KEY_EXIT:
			case KEY_MENU:
				GUI_EndDialog("win_desai_cas50_card_setworktime");
				return EVENT_TRANSFER_STOP;
			case KEY_OK:
				GUI_EndDialog("win_desai_cas50_card_setworktime");
				return EVENT_TRANSFER_STOP;
			case KEY_POWER:
				return EVENT_TRANSFER_KEEPON;
			default:
				return EVENT_TRANSFER_STOP;
		}
	}

	return EVENT_TRANSFER_KEEPON;
}

SIGNAL_HANDLER  int win_card_setworktime_box_keypress(const char* widgetname, void *usrdata)
{
	GUI_Event *event = NULL;
	event = (GUI_Event *)usrdata;
	INT8 pucPin[7]={0,};
	UINT8 Pin[4]={0,};
	int temp1=0;
	INT32 value;
	UINT8 pucOddTimes;
	char* osd_language=NULL;
	ST_WORKTIME_INFO  pstWorkTime;
	char* time_get;
	char* temp;
	EN_ERR_CAS ret;
	char buf[40];
	
	if(event->type == GUI_KEYDOWN)
	{	
		switch(event->key.sym)
		{
			case KEY_OK:
				{	
					GUI_GetProperty("win_desai_cas50_card_setworktime_boxitem4_edit", "string", &value);
					memcpy(pucPin,(char*)value,6);
					pucPin[6]='\0';
					if(strlen(pucPin)!=6)
					{
						osd_language = app_flash_get_config_osd_language();
						if (0 == strcmp(osd_language,LANGUAGE_CHINESE))
							app_popmsg(POP_DIALOG_X_START, POP_DIALOG_Y_START,"PIN码必须为6位",POPMSG_TYPE_OK);
						else
							app_popmsg(POP_DIALOG_X_START, POP_DIALOG_Y_START,"PIN length must be six!",POPMSG_TYPE_OK);
						GUI_SetProperty("win_desai_cas50_card_setworktime_boxitem4_edit","clear",NULL);
						app_ca_get_work_time();
						return EVENT_TRANSFER_STOP;	
					}
					
					GUI_GetProperty("win_desai_cas50_card_setworktime_boxitem1_edit", "string",&time_get);
					temp = strtok(time_get, ":"); 
					pstWorkTime.stBeginTime.ucHour= atoi(temp);
					temp = strtok(NULL, ":"); 
					pstWorkTime.stBeginTime.ucMinute= atoi(temp);
				//	temp = strtok(NULL, ":"); 
				//	pstWorkTime.stBeginTime.ucSecond= atoi(temp);

					pstWorkTime.stBeginTime.ucSecond= 0;
					
					GUI_GetProperty("win_desai_cas50_card_setworktime_boxitem2_edit", "string",&time_get);
					temp = strtok(time_get, ":"); 
					pstWorkTime.stEndTime.ucHour= atoi(temp);
					temp = strtok(NULL, ":"); 
					pstWorkTime.stEndTime.ucMinute= atoi(temp);
				//	temp = strtok(NULL, ":"); 
				//	pstWorkTime.stEndTime.ucSecond= atoi(temp);

					pstWorkTime.stEndTime.ucSecond= 0;
					#if 1
					
					snprintf(buf,40,"%02d:%02d:%02d-%02d:%02d:%02d",pstWorkTime.stBeginTime.ucHour,pstWorkTime.stBeginTime.ucMinute,
							pstWorkTime.stBeginTime.ucSecond,pstWorkTime.stEndTime.ucHour,pstWorkTime.stEndTime.ucMinute,pstWorkTime.stEndTime.ucSecond);
					CAS_Dbg("pucpin %s\n",pucPin);
					CAS_Dbg("time  %s",buf);
					
					#endif

					temp1=atoi(pucPin);
					Pin[0]=(char)(temp1>>16);
					Pin[1]=(char)(temp1>>8);
					Pin[2]=(char)(temp1&0xff);
					CAS_Dbg("%0d  %0d  %0d\n",Pin[0],Pin[1],Pin[2]);
					ret=DSCA_SetWorkTime(Pin,&pucOddTimes,&pstWorkTime);
					CAS_Dbg("%d\n",ret);
					
					if(ret == CA_CARD_CHECK_PIN_ERROR)
					{
						osd_language = app_flash_get_config_osd_language();
						if (0 == strcmp(osd_language,LANGUAGE_CHINESE))
							snprintf(buf,40,"PIN码错误，你还可以试 %d 次",pucOddTimes);
						else
							snprintf(buf,40,"Pin Err，you can try %d times",pucOddTimes);
						app_popmsg(POP_DIALOG_X_START, POP_DIALOG_Y_START,buf, POPMSG_TYPE_OK);
						GUI_SetProperty("win_desai_cas50_card_setworktime_boxitem4_edit","clear",NULL);
						app_ca_get_work_time();
						return EVENT_TRANSFER_STOP;	
					}
					if(ret == CA_CARD_CHECK_PIN_LOCK)
					{
						osd_language = app_flash_get_config_osd_language();
						if (0 == strcmp(osd_language,LANGUAGE_CHINESE))
							app_popmsg(POP_DIALOG_X_START, POP_DIALOG_Y_START,"卡被锁定", POPMSG_TYPE_OK);
						else
							app_popmsg(POP_DIALOG_X_START, POP_DIALOG_Y_START,"CARD_CHECK_PIN_LOCK", POPMSG_TYPE_OK);
						GUI_SetProperty("win_desai_cas50_card_setworktime_boxitem4_edit","clear",NULL);
						app_ca_get_work_time();
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
						GUI_SetProperty("win_desai_cas50_card_setworktime_boxitem4_edit","clear",NULL);
						app_ca_get_work_time();
						return EVENT_TRANSFER_STOP;	
					}
					if(ret == CA_ERROR_PARAMETER)
					{
						osd_language = app_flash_get_config_osd_language();
						if (0 == strcmp(osd_language,LANGUAGE_CHINESE))
							app_popmsg(POP_DIALOG_X_START, POP_DIALOG_Y_START,"卡参数错误", POPMSG_TYPE_OK);
						else
							app_popmsg(POP_DIALOG_X_START, POP_DIALOG_Y_START,"ERROR_PARAMETER ", POPMSG_TYPE_OK);
						GUI_SetProperty("win_desai_cas50_card_setworktime_boxitem4_edit","clear",NULL);
						app_ca_get_work_time();
						return EVENT_TRANSFER_STOP;	
					}
					if(ret == CA_ERROR_WORKTIME_FORMAT)
					{
						osd_language = app_flash_get_config_osd_language();
						if (0 == strcmp(osd_language,LANGUAGE_CHINESE))
							app_popmsg(POP_DIALOG_X_START, POP_DIALOG_Y_START,"工作时段格式错误", POPMSG_TYPE_OK);
						else
							app_popmsg(POP_DIALOG_X_START, POP_DIALOG_Y_START,"ERROR_PARAMETER_WORKTIME_FORMAT ", POPMSG_TYPE_OK);
						GUI_SetProperty("win_desai_cas50_card_setworktime_boxitem4_edit","clear",NULL);
						app_ca_get_work_time();
						return EVENT_TRANSFER_STOP;	
					}					
					return EVENT_TRANSFER_KEEPON;
				}
				
				default:
					return EVENT_TRANSFER_KEEPON;
			}
	}
	return EVENT_TRANSFER_KEEPON;
}


