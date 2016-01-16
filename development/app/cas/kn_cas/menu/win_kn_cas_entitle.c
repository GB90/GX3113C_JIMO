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
//#include "app_by_cas_api_entitle.h"
#include "cas_hcn.h"

HCNCAS_PPCENTITLE entitle_info[50];
UINT8 entitle_num;

SIGNAL_HANDLER  int app_kn_cas_entitle_create(const char* widgetname, void *usrdata)
{
/*
	uint32_t pos = 0;
    char                  sTextBuffer[10];
	
    get_count.date_type = DVB_CA_ENTITLE_INFO;
    app_cas_api_get_count(&get_count);
	
    sprintf(sTextBuffer,"%d",get_count.totalnum);
    GUI_SetProperty("win_by_cas_entitle_text_entitle_count", "string", sTextBuffer);

	GUI_SetProperty("win_by_cas_entitle_listview","select",(void*)&pos);

	return 0;
*/
	INT32 ret;
    	char sTextBuffer[40];

	memset(&entitle_info,0,50*sizeof(HCNCAS_PPCENTITLE));
	entitle_num = 0;
	memset(sTextBuffer,0,sizeof(sTextBuffer));
		
 	ret = hcncas_get_entitle(entitle_info,&entitle_num);
	printf("\n***hcncas_get_entitle ret=%d	entitle_num=%d********\n",ret,entitle_num);

	printf("\n*** %d********\n",entitle_info[0].index[0]);
	printf("\n*** %02x	%02x	%02x	%02x********\n",entitle_info[0].Start_Time[0],entitle_info[0].Start_Time[1],entitle_info[0].Start_Time[2],entitle_info[0].Start_Time[3]);
	printf("\n*** %02x	%02x	%02x	%02x********\n",entitle_info[0].End_Time[0],entitle_info[0].End_Time[1],entitle_info[0].End_Time[2],entitle_info[0].End_Time[3]);

	snprintf(sTextBuffer,40,"%d",entitle_num);
    	GUI_SetProperty("win_kn_cas_entitle_text_entitle_count", "string", sTextBuffer);

	return 0;
}

SIGNAL_HANDLER  int app_kn_cas_entitle_destroy(const char* widgetname, void *usrdata)
{
	return 0;
}


SIGNAL_HANDLER  int app_kn_cas_entitle_keypress(const char* widgetname, void *usrdata)
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
				GUI_EndDialog("win_kn_cas_entitle");
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

SIGNAL_HANDLER  int app_kn_cas_entitle_get_count(const char* widgetname, void *usrdata)
{
	if(entitle_num > 50)
		entitle_num = 50;
	
    	return entitle_num;
}

SIGNAL_HANDLER  int app_kn_cas_entitle_get_data(const char* widgetname, void *usrdata)
{
       unsigned int      Row    = 0;
	ListItemPara *item_para =NULL;
	
	static char sTextBuffer[20] = {0};
	static char starttime[20] = {0};
	static char endtime[20] = {0};
	
	if(NULL == widgetname || NULL == usrdata )
	{
		return (1);
	}
	item_para = (ListItemPara *)(usrdata);
	if(NULL == item_para) 
		return GXCORE_ERROR;

	Row = item_para->sel;

	memset(sTextBuffer,0,sizeof(sTextBuffer));	
	sprintf(sTextBuffer,"%02d",entitle_info[Row].index[0]);
	item_para->string = sTextBuffer;

	item_para= item_para->next;
	
	memset(starttime,0,sizeof(starttime));
	sprintf(starttime,"%02x%02x/%02x/%02x",entitle_info[Row].Start_Time[0],entitle_info[Row].Start_Time[1],entitle_info[Row].Start_Time[2],entitle_info[Row].Start_Time[3]);
	item_para->string = starttime;


	item_para= item_para->next;
	
	memset(endtime,0,sizeof(endtime));
	sprintf(endtime,"%02x%02x/%02x/%02x",entitle_info[Row].End_Time[0],entitle_info[Row].End_Time[1],entitle_info[Row].End_Time[2],entitle_info[Row].End_Time[3]);
	item_para->string = endtime;

	return 0;
}




