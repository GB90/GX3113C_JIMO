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
//#include "app_by_cas_api_baseinfo.h"
#include "cas_hcn.h"
hcncas_smcinfo base_info;

SIGNAL_HANDLER  int win_kn_cas_baseinfo_create(const char* widgetname, void *usrdata)
{
/*
	ca_get_date_t get_data = {0};
	char* str = NULL;
	
	get_data.date_type = DVB_CA_BASE_INFO;		
	get_data.ID = BY_BASEINFO_VERSION_ID;
	str = app_cas_api_get_data(&get_data);
	if (NULL !=str )
		{
                GUI_SetProperty("win_by_cas_baseinfo_version", "string",str);			
		}

	get_data.ID = BY_BASEINFO_SYS_SP_ID;
	str = app_cas_api_get_data(&get_data);
	if (NULL !=str )
		{
                GUI_SetProperty("win_by_cas_baseinfo_sysspid", "string",str);			
		}

	get_data.ID = BY_BASEINFO_SYS_CT_ID;
	str = app_cas_api_get_data(&get_data);
	if (NULL !=str )
		{
                GUI_SetProperty("win_by_cas_baseinfo_sysctid", "string",str);			
		}

	get_data.ID = BY_BASEINFO_CARDID_ID;
	str = app_cas_api_get_data(&get_data);
	if (NULL !=str )
		{
                GUI_SetProperty("win_by_cas_baseinfo_cardid", "string",str);			
		}

	
	return 0;
	*/
	UINT8 ret;
	UINT8 buffer[40]= {0,};

	memset(&base_info,0,sizeof(hcncas_smcinfo));
		
	ret =  hcncas_get_cardinfo(&base_info);
	if(ret !=0)
	{
		memset(buffer,0,sizeof(buffer));
		snprintf(buffer,40,"%d",base_info.IC_NO);
		
		GUI_SetProperty("win_kn_cas_baseinfo_IC_NO", "string",buffer);	

		if(base_info.CARD_STATUS == 1)
			GUI_SetProperty("win_kn_cas_baseinfo_PAIR_STATUS", "string","PAIRED");	
		else
			GUI_SetProperty("win_kn_cas_baseinfo_PAIR_STATUS", "string","NOTPAIRED");	


		memset(buffer,0,sizeof(buffer));
		snprintf(buffer,40,"%d",base_info.PARENTAL_RATING);
		
		GUI_SetProperty("win_kn_cas_baseinfo_PARENTAL_RATING", "string",buffer);	
		
		GUI_SetProperty("win_kn_cas_baseinfo_CARD_LABEL", "string",base_info.CARD_LABEL);	

		printf("\n*****%02d%02d%02d%02d**********\n",base_info.CAM_VER[0],base_info.CAM_VER[1],base_info.CAM_VER[2],base_info.CAM_VER[3]);
		printf("*****%02d%02d%02d**********\n",base_info.OPERATION_ID[0],base_info.OPERATION_ID[1],base_info.OPERATION_ID[2]);
		
	}
	return 0;
	
}

SIGNAL_HANDLER  int win_kn_cas_baseinfo_destroy(const char* widgetname, void *usrdata)
{
	return 0;
}

SIGNAL_HANDLER  int win_kn_cas_baseinfo_keypress(const char* widgetname, void *usrdata)
{
	GUI_Event *event = NULL;
	event = (GUI_Event *)usrdata;
	if(event->type == GUI_KEYDOWN)
	{
		switch(event->key.sym)
		{
			case KEY_EXIT:
				break;
				return EVENT_TRANSFER_STOP;
			case KEY_RECALL:
				GUI_EndDialog("win_kn_cas_baseinfo");
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

