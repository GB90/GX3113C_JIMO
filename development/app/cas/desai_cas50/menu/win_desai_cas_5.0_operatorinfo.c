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
#include "desai_ca_api.h"
#include "app_win_interface.h"
SIGNAL_HANDLER  int win_desai_cas50_operatorinfo_create(const char* widgetname, void *usrdata)
{
	UINT8 buf0[DSCA_OPERATOR_NAME_LEN] ;
	
	UINT8 name[DSCA_OPERATOR_NAME_LEN] ;
	UINT8 phone[DSCA_OPERATOR_NAME_LEN] ;
	UINT8 address[DSCA_OPERATOR_ADDRESS_LEN] ;
	ST_OPERATOR_INFO  pstOperatorInfo;
	
	memset(buf0,0,DSCA_OPERATOR_NAME_LEN);
	
	memset(name,0,DSCA_OPERATOR_NAME_LEN);
	memset(phone,0,DSCA_OPERATOR_NAME_LEN);
	memset(address,0,DSCA_OPERATOR_ADDRESS_LEN);

	EN_ERR_CAS ret=0;

	ret = DSCA_GetOperatorID(buf0);
	if(ret == CA_ERROR_OK)
	{
		GUI_SetProperty("win_desai_cas50_baseinfo_operator_id_num", "string",buf0);
		CAS_Dbg("Line = %d	%s\n",__LINE__,buf0);
	}
	
	ret = DSCA_GetOperatorInfo(&pstOperatorInfo);
	CAS_Dbg("Line = %d	ret = %d\n",__LINE__,ret);
	if(ret == CA_ERROR_OK)
	{
		memcpy(name,pstOperatorInfo.aucOperatorName,DSCA_OPERATOR_NAME_LEN);
		memcpy(phone,pstOperatorInfo.aucOperatorPhone,DSCA_OPERATOR_NAME_LEN);
		if(pstOperatorInfo.aucOperatorAddress[0] > DSCA_OPERATOR_ADDRESS_LEN)
		{
			memcpy(address,pstOperatorInfo.aucOperatorAddress+1,DSCA_OPERATOR_ADDRESS_LEN);
		}
		else
		{
			memcpy(address,pstOperatorInfo.aucOperatorAddress+1,pstOperatorInfo.aucOperatorAddress[0]);
		}
		
		GUI_SetProperty("win_desai_cas50_operator1", "string",name);
		CAS_Dbg("Line = %d	%s\n",__LINE__,name);
		GUI_SetProperty("win_desai_cas50_operator2", "string",phone);
		CAS_Dbg("Line = %d	%s\n",__LINE__,phone);
		GUI_SetProperty("win_desai_cas50_operator3", "string",address);
		CAS_Dbg("Line = %d	%s\n",__LINE__,address);
	}
	return 0;
	
}

SIGNAL_HANDLER  int win_desai_cas50_operatorinfo_destroy(const char* widgetname, void *usrdata)
{
	return 0;
}

SIGNAL_HANDLER  int win_desai_cas50_operatorinfo_keypress(const char* widgetname, void *usrdata)
{
	GUI_Event *event = NULL;
	event = (GUI_Event *)usrdata;
	if(event->type == GUI_KEYDOWN)
	{
		switch(event->key.sym)
		{
			case KEY_EXIT:
			case KEY_RECALL:
			case KEY_MENU:
				GUI_EndDialog("win_desai_cas50_operatorinfo");
				return EVENT_TRANSFER_STOP;
			case KEY_OK:
				return EVENT_TRANSFER_STOP;
			case KEY_POWER:
				return EVENT_TRANSFER_KEEPON;
			default:
				return EVENT_TRANSFER_STOP;
		}
	}

	return EVENT_TRANSFER_KEEPON;
}


