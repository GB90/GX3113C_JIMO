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
#include "desai_ca_api.h"

int gMail_sel = 0;
ST_MAILLIST_INFO pstEmailInfoList;

SIGNAL_HANDLER  int app_email_content_create(const char* widgetname, void *usrdata)
{	
	int pos;
	int email_num;
	EN_ERR_CAS ret;
	
	UINT16 pusContentLen=0;
	static UINT8 buf[1024]={0,};
	GUI_GetProperty("win_desai_cas50_email_listview", "select", &pos);

	memcpy(buf,pstEmailInfoList.stMailInfo[pos].aucMailTitle,30);
	GUI_SetProperty("win_desai_cas50_emailcontent_mailtitle", "string", buf);

	memset(buf,0,1024);
	email_num=pstEmailInfoList.stMailInfo[pos].ucMailID;
	ret=DSCA_GetEmailContent(email_num,buf,&pusContentLen);
	if(ret ==CA_ERROR_OK)
	{
		GUI_SetProperty("win_desai_cas50_emailcontent_content", "string", buf);
	}
	
	return 0;
}

SIGNAL_HANDLER  int  app_email_content_destroy(const char* widgetname, void *usrdata)
{
	return 0;
}


SIGNAL_HANDLER  int app_email_content_keypress(const char* widgetname, void *usrdata)
{
	GUI_Event *event = NULL;
	event = (GUI_Event *)usrdata;
	switch(event->type)
	{
		case GUI_MOUSEBUTTONDOWN:
			break;
		case GUI_KEYDOWN:
			switch(event->key.sym)
			{
				case KEY_EXIT:
                		case KEY_MENU:
					GUI_EndDialog("win_desai_cas50_emailcontent");
					
					GUI_SetProperty("win_desai_cas50_email_listview","update_all",NULL);
				//	GUI_SetProperty("win_desai_cas50_email_listview","select",&gMail_sel);
					break;
				case KEY_UP:
				{
					int value = 3;
					GUI_SetProperty("win_desai_cas50_emailcontent_content", "line_up", &value);
					return EVENT_TRANSFER_STOP;
				}
				case KEY_DOWN:
				{
					int value = 3;
					GUI_SetProperty("win_desai_cas50_emailcontent_content", "line_down", &value);
					return EVENT_TRANSFER_STOP;
				}
				case KEY_PAGE_UP:
				{
					int value = 1;
					GUI_SetProperty("win_desai_cas50_emailcontent_content", "page_up", &value);
					return EVENT_TRANSFER_STOP;
				}
				case KEY_PAGE_DOWN:
				{
					int value = 1;
					GUI_SetProperty("win_desai_cas50_emailcontent_content", "page_down", &value);
					return EVENT_TRANSFER_STOP;
				}
				default:
					break;
			}
			break;
		default:
			break;
	}
	return EVENT_TRANSFER_KEEPON;
}

SIGNAL_HANDLER  int app_desai_cas50_email_create(const char* widgetname, void *usrdata)
{
	return 0;
}

SIGNAL_HANDLER  int  app_desai_cas50_email_destroy(const char* widgetname, void *usrdata)
{
	return 0;
}


SIGNAL_HANDLER  int app_desai_cas50_email_keypress(const char* widgetname, void *usrdata)
{
	GUI_Event *event = NULL;
	event = (GUI_Event *)usrdata;

	switch(event->type)
	{
		case GUI_MOUSEBUTTONDOWN:
			break;

		case GUI_KEYDOWN:
			switch(event->key.sym)
			{
				case KEY_EXIT:
                		case KEY_MENU:
					GUI_EndDialog("win_desai_cas50_email");
					return EVENT_TRANSFER_STOP;
				case KEY_OK:
					GUI_SetProperty("win_desai_cas50_email_listview","update_all",NULL);
				//	GUI_SetProperty("win_desai_cas50_email_listview","select",&gMail_sel);
					return EVENT_TRANSFER_STOP;
				default:
					break;
			}
			break;
		default:
			break;
	}
	return EVENT_TRANSFER_KEEPON;
}

SIGNAL_HANDLER  int app_desai_cas50_email_list_create(const char* widgetname, void *usrdata)
{	
	return 0;
}
SIGNAL_HANDLER  int app_desai_cas50_email_get_count(const char* widgetname, void *usrdata)
{
	EN_ERR_CAS ret;
	UINT8 pstEmailNo;
	ret=DSCA_GetEmailInfoList(&pstEmailInfoList);
	ret=DSCA_GetEmailNumber(&pstEmailNo);
	CAS_Dbg("pstEmailNo = %d\n",pstEmailNo);
	if(ret == CA_ERROR_OK)
		return pstEmailNo;
	else
		return 0;
}

SIGNAL_HANDLER  int app_desai_cas50_email_get_data(const char* widgetname, void *usrdata)
{
	ListItemPara *item = NULL;
	item = (ListItemPara*)usrdata;
	char* osd_language=NULL;
	static char id[10] = {0};
	static char tmp[40]={0,};
	static char tmp1[40]={0,};
	static char tmp2[40]={0,};
//	int32_t Config;

	ST_DATE_TIME_INFO	stSendTime;	
	if(NULL == item)
	{
		return EVENT_TRANSFER_KEEPON;
	}
	
	memset(id,0,10);
	sprintf(id,"%d",pstEmailInfoList.stMailInfo[item->sel].ucMailID);
	item->string = id;
       
	item = item->next;

	osd_language = app_flash_get_config_osd_language();
	if (0 == strcmp(osd_language,LANGUAGE_CHINESE))
		item->string=pstEmailInfoList.stMailInfo[item->sel].ucNewFlag?"未读":"已读";
	else		
		item->string=pstEmailInfoList.stMailInfo[item->sel].ucNewFlag?"Unread":"Read";
	
	memset(tmp,0,40);
	item = item->next;
	CAS_Dbg("\n%s\n",pstEmailInfoList.stMailInfo[item->sel].aucMailTitle);

	memcpy(tmp,pstEmailInfoList.stMailInfo[item->sel].aucMailTitle,strlen((char*)(pstEmailInfoList.stMailInfo[item->sel].aucMailTitle)));
	CAS_Dbg("\n%s\n",tmp);
	item->string = tmp;
	
	memset(tmp1,0,40);
	item = item->next;
	CAS_Dbg("\n%s\n",pstEmailInfoList.stMailInfo[item->sel].aucMailSender);
	memcpy(tmp1, pstEmailInfoList.stMailInfo[item->sel].aucMailSender,strlen((char*)(pstEmailInfoList.stMailInfo[item->sel].aucMailSender)));
	CAS_Dbg("\n%s\n",tmp1);
	item->string =tmp1;
	
	memset(tmp2,0,40);
	item = item->next;
	
	stSendTime.usYear=pstEmailInfoList.stMailInfo[item->sel].stSendTime.usYear;
	stSendTime.ucMonth=pstEmailInfoList.stMailInfo[item->sel].stSendTime.ucMonth;
	stSendTime.ucDay=pstEmailInfoList.stMailInfo[item->sel].stSendTime.ucDay;
	stSendTime.ucHour=pstEmailInfoList.stMailInfo[item->sel].stSendTime.ucHour;
	stSendTime.ucMinute=pstEmailInfoList.stMailInfo[item->sel].stSendTime.ucMinute;
	stSendTime.ucSecond=pstEmailInfoList.stMailInfo[item->sel].stSendTime.ucSecond;
	
	snprintf(tmp2,40,"%d/%02d/%02d %02d:%02d:%02d",stSendTime.usYear,stSendTime.ucMonth,
			stSendTime.ucDay,stSendTime.ucHour,stSendTime.ucMinute,stSendTime.ucSecond);
	item->string =tmp2;// pstEmailInfoList->stMailInfo[item->sel].aucMailSender;
	return EVENT_TRANSFER_STOP;		
	
	
}

SIGNAL_HANDLER  int app_desai_cas50_email_list_change(const char* widgetname, void *usrdata)
{
	return 0;
}
SIGNAL_HANDLER  int app_desai_cas50_email_list_keypress(const char* widgetname, void *usrdata)
{
	GUI_Event *event = NULL;
	int pos;
	char* osd_language=NULL;
	popmsg_ret ret;
	event = (GUI_Event *)usrdata;

	UINT8 EmailNo = 0;
	
	
	if(GUI_KEYDOWN ==  event->type)
	{
		switch(event->key.sym)
		{
			
			case STBK_MENU:
			case STBK_EXIT:
				break;
			case STBK_OK:
			{		
				GUI_GetProperty("win_desai_cas50_email_listview", "select", &pos);
			//	gMail_sel=pos;
				DSCA_GetEmailNumber(&EmailNo);	
				if(EmailNo > 0)
					GUI_CreateDialog("win_desai_cas50_emailcontent");
				
				return EVENT_TRANSFER_STOP;
			}
			case KEY_RED:
			{
				
				EN_ERR_CAS ret1;
				UINT8 email_num=0;
				GUI_GetProperty("win_desai_cas50_email_listview", "select", &pos);
			//	gMail_sel=pos;
				DSCA_GetEmailNumber(&EmailNo);	
				if(EmailNo > 0)
				{
					osd_language = app_flash_get_config_osd_language();
					if (0 == strcmp(osd_language,LANGUAGE_CHINESE))
						ret=app_popmsg(POP_DIALOG_X_START, POP_DIALOG_Y_START, "确定要删除此邮件?", POPMSG_TYPE_YESNO);
					else
						ret=app_popmsg(POP_DIALOG_Y_START, POP_DIALOG_Y_START, "Sure to delete this mail?", POPMSG_TYPE_YESNO);
					if(ret ==POPMSG_RET_YES)
					{
						email_num=pstEmailInfoList.stMailInfo[pos].ucMailID;
						ret1=DSCA_DelEmail(email_num);
					}
					GUI_SetProperty("win_desai_cas50_email_listview","update_all",NULL);
				}
				else
				{
					osd_language = app_flash_get_config_osd_language();
					if (0 == strcmp(osd_language,LANGUAGE_CHINESE))
						ret=app_popmsg(POP_DIALOG_X_START, POP_DIALOG_Y_START, "没有邮件!", POPMSG_TYPE_OK);
					else
						ret=app_popmsg(POP_DIALOG_X_START, POP_DIALOG_Y_START, "No email!", POPMSG_TYPE_OK);
				}

			//	GUI_SetProperty("win_desai_cas50_email_listview","select",&gMail_sel);
				return EVENT_TRANSFER_STOP;
			}
			case KEY_YELLOW:
			{
				EN_ERR_CAS ret1;
				DSCA_GetEmailNumber(&EmailNo);	
				if(EmailNo > 0)
				{
					osd_language = app_flash_get_config_osd_language();
					if (0 == strcmp(osd_language,LANGUAGE_CHINESE))
						ret=app_popmsg(POP_DIALOG_X_START, POP_DIALOG_Y_START, "删除所有邮件?", POPMSG_TYPE_YESNO);
					else
						ret=app_popmsg(POP_DIALOG_X_START, POP_DIALOG_Y_START, "DELETE ALL?", POPMSG_TYPE_YESNO);
					if(ret ==POPMSG_RET_YES)
					{
						ret1=DSCA_DelAllEmail();
					}
					GUI_SetProperty("win_desai_cas50_email_listview","update_all",NULL);
				}
				else
				{
					osd_language = app_flash_get_config_osd_language();
					if (0 == strcmp(osd_language,LANGUAGE_CHINESE))
						ret=app_popmsg(POP_DIALOG_X_START, POP_DIALOG_Y_START, "没有邮件!", POPMSG_TYPE_OK);
					else
						ret=app_popmsg(POP_DIALOG_X_START, POP_DIALOG_Y_START, "No email!", POPMSG_TYPE_OK);
				}
				return EVENT_TRANSFER_STOP;
			}
			default :
				break;
		}
	}
	return EVENT_TRANSFER_KEEPON;
}







