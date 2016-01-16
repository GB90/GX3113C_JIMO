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
#include "app_common_porting_ca_smart.h"
#include "app_win_interface.h"
#include "desai_ca_api.h"

UINT32 uiOperatorID;

SIGNAL_HANDLER  int app_desai_cas50_entitle_create(const char* widgetname, void *usrdata)
{
	return 0;
}

SIGNAL_HANDLER  int  app_desai_cas50_entitle_destroy(const char* widgetname, void *usrdata)
{
	return 0;
}

SIGNAL_HANDLER  int app_desai_cas50_entitle_keypress(const char* widgetname, void *usrdata)
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
					GUI_EndDialog("win_desai_cas50_entitle_info");
					return EVENT_TRANSFER_STOP;
				case KEY_MENU:
				case KEY_RECALL:
					GUI_EndDialog("win_desai_cas50_entitle_info");
					return EVENT_TRANSFER_STOP;
				case KEY_F1:
					return EVENT_TRANSFER_STOP;
				case KEY_OK:
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

SIGNAL_HANDLER  int app_desai_cas50_entitle_get_count(const char* widgetname, void *usrdata)
{
	EN_ERR_CAS ret;
	UINT32 puiNumber=0;
	UINT8 OperID[4]    ={0};

	if(app_porting_ca_smart_status() == FALSE)
		return 0;
	
	ret=DSCA_GetOperatorID(OperID);
	if(ret !=CA_ERROR_OK)
	{
		CAS_Dbg("GetOperatorID ERROR\n");
	}
	uiOperatorID = atoi((char *)OperID);
	CAS_Dbg("%s-------------ret = %d	uiOperatorID = %d\n",__FUNCTION__,ret,uiOperatorID);
	ret=DSCA_GetProductNumber(uiOperatorID,&puiNumber);
	if(ret !=CA_ERROR_OK)
		CAS_Dbg("GetProductNumber ERROR\n");
	CAS_Dbg("%s-------------ret = %d	puiNumber = %d\n",__FUNCTION__,ret,puiNumber);
	return puiNumber;
}

SIGNAL_HANDLER  int app_desai_cas50_entitle_get_data(const char* widgetname, void *usrdata)
{
	EN_ERR_CAS ret;
	ListItemPara *item = NULL;
	item = (ListItemPara*)usrdata;
	static char type[50] = {0};
	static char id[50] = {0};
	static char starttime[50] = {0};
	static char endtime[50] = {0};
	ST_PRODUCT_ENTITLE pstProductEntitles;

	if(NULL == item)
	{
		return EVENT_TRANSFER_KEEPON;
	}
		
	ret=DSCA_GetProductInfo (uiOperatorID,  item->sel, item->sel+1, &pstProductEntitles);
	if(ret !=CA_ERROR_OK)
		CAS_Dbg("GetProductInfo ERROR\n");
	memset(type,0,50);
	sprintf(type,"%d",item->sel+1);
	item->string = type;
           
	item = item->next;
	memset(id,0,50);
	sprintf(id,"%d",pstProductEntitles.usProductID);
	item->string = id;
	if(pstProductEntitles.ucEnableType==2)
	{
		item = item->next;
		memset(starttime,0,50);
		sprintf(starttime,"%04d/%02d/%02d %02d:%02d:%02d",
			pstProductEntitles.stBeginTime.usYear,
			pstProductEntitles.stBeginTime.ucMonth,
			pstProductEntitles.stBeginTime.ucDay,
			pstProductEntitles.stBeginTime.ucHour,
			pstProductEntitles.stBeginTime.ucMinute,
			pstProductEntitles.stBeginTime.ucSecond);
		//sprintf(starttime,"%02d/%02d %02d:%02d",pstProductEntitles.stBeginTime.ucMonth,
		//	pstProductEntitles.stBeginTime.ucDay,pstProductEntitles.stBeginTime.ucHour,pstProductEntitles.stBeginTime.ucMinute
		//	);
		item->string = starttime;

		item = item->next;
		memset(endtime,0,50);
		sprintf(endtime,"%04d/%02d/%02d %02d:%02d:%02d",
			pstProductEntitles.stEndTime.usYear,
			pstProductEntitles.stEndTime.ucMonth,
			pstProductEntitles.stEndTime.ucDay,
			pstProductEntitles.stEndTime.ucHour,
			pstProductEntitles.stEndTime.ucMinute,
			pstProductEntitles.stEndTime.ucSecond);
		//sprintf(endtime,"%02d/%02d %02d:%02d",pstProductEntitles.stEndTime.ucMonth,
		//	pstProductEntitles.stEndTime.ucDay,pstProductEntitles.stEndTime.ucHour,pstProductEntitles.stEndTime.ucMinute
		//	);
		item->string = endtime;
	}
	else
	{
		item = item->next;
		item->string ="Invalid";
		item = item->next;
		item->string ="Invalid";
	}
	return 0;
}


