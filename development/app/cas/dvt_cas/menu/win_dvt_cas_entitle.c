/*****************************************************************************
* 						   CONFIDENTIAL
*        Hangzhou GuoXin Science and Technology Co., Ltd.
*                      (C)2012, All right reserved
******************************************************************************

******************************************************************************
* File Name :	win_dvt_cas_entitle.c
* Author    : 	zhouhm
* Project   :	goxceed dvbc
* Type      :
******************************************************************************
* Purpose   :	模块头文件
******************************************************************************
* Release History:
VERSION	Date			  AUTHOR         Description
1.0  	2013.12.04		  zhouhm 	 			creation
*****************************************************************************/
#include "app_common_porting_stb_api.h"
#include "app_win_interface.h"
#include "app_dvt_cas_api_entitle.h"
#include "app_dvt_cas_api_operator.h"

static ca_get_count_t get_count = {0};	
static uint32_t s_EntitleSel=0;
extern uint32_t operator_sel;


static void update_entitle(uint32_t sel)
{
	ca_get_date_t get_data = {0};
	char* str = NULL;

	get_data.date_type = DVB_CA_ENTITLE_INFO;		
	get_data.ID = DVT_ENTITLE_NAME_ID;
	get_data.pos = sel;
	str = app_cas_api_get_data(&get_data);
	if (NULL !=str )
		{
			printf("str = %s\n",str);
            GUI_SetProperty("win_dvt_cas_entitle_name", "string",str);			
		}

	get_data.ID = DVT_ENTITLE_ID_ID;
	str = app_cas_api_get_data(&get_data);
	if (NULL !=str )
		{
                GUI_SetProperty("win_dvt_cas_entitle_id", "string",str);			
		}

	get_data.ID = DVT_ENTITLE_AUTHORIZE_TIME_ID;
	str = app_cas_api_get_data(&get_data);
	if (NULL !=str )
		{
                GUI_SetProperty("win_dvt_cas_entitle_time", "string",str);			
		}

	get_data.ID = DVT_ENTITLE_START_TIME_ID;
	str = app_cas_api_get_data(&get_data);
	if (NULL !=str )
		{
                GUI_SetProperty("win_dvt_cas_entitle_start", "string",str);			
		}

	get_data.ID = DVT_ENTITLE_END_TIME_ID;
	str = app_cas_api_get_data(&get_data);
	if (NULL !=str )
		{
                GUI_SetProperty("win_dvt_cas_entitle_end", "string",str);			
		}

	
}

SIGNAL_HANDLER  int win_dvt_cas_entitle_create(const char* widgetname, void *usrdata)
{
	static uint8_t pArry3[60] = {0};
	ca_get_date_t get_data = {0};
	char* str = NULL;

	 char* osd_language=NULL;	
	
	 osd_language = app_flash_get_config_osd_language();
	 if (0 == strcmp(osd_language,LANGUAGE_CHINESE))	
	{
	//	 GUI_SetProperty("win_dvt_cas_entitle_title","img","title_ch_Authorization.bmp");  
	//	 GUI_SetProperty("win_stb_info_tip_image_exit", "img", "tips_exit.bmp");
	 }
	 else if(0 == strcmp(osd_language,LANGUAGE_ENGLISH))
	 { 
	//	 GUI_SetProperty("win_dvt_cas_entitle_title","img","title_Authorization_e.bmp");
	//	 GUI_SetProperty("win_stb_info_tip_image_exit","img","tips_exit_e.bmp");
	 }


    get_count.date_type = DVB_CA_ENTITLE_INFO;
    app_cas_api_get_count(&get_count);

	get_data.date_type = DVB_CA_OPERATOR_INFO;
	get_data.ID = DVT_OPERATOR_NAME_ID;
	get_data.pos = operator_sel; // 行

	str = app_cas_api_get_data(&get_data);

	// 运营商名称
	if (NULL !=str )
		{
                GUI_SetProperty("win_dvt_cas_entitle_operator", "string",str);			
		}

	if ( 0 != get_count.totalnum)
		{
			sprintf((char *)pArry3,"总的授权页数: %d 页，当前第%d 页",get_count.totalnum,1);
			GUI_SetProperty("win_dvt_cas_entitle_num_text","string", pArry3);
			s_EntitleSel = 0;
			update_entitle(s_EntitleSel);				
		}
	else
		{
			sprintf((char *)pArry3,"总的授权页数: %d 页，当前第%d 页",get_count.totalnum,0);
			GUI_SetProperty("win_dvt_cas_entitle_num_text","string", pArry3);			
		}
	
	return 0;
}

SIGNAL_HANDLER  int win_dvt_cas_entitle_destroy(const char* widgetname, void *usrdata)
{
	return 0;
}

SIGNAL_HANDLER  int win_dvt_cas_entitle_keypress(const char* widgetname, void *usrdata)
{
	GUI_Event *event = NULL;
	uint32_t pageIndex;
	static uint8_t pArry3[60] = {0};
	event = (GUI_Event *)usrdata;
	if(event->type == GUI_KEYDOWN)
	{
		switch(event->key.sym)
		{
			case KEY_EXIT:
				break;
				return EVENT_TRANSFER_STOP;
			case KEY_RECALL:
				GUI_EndDialog("win_dvt_cas_entitle");
				return EVENT_TRANSFER_STOP;
			case KEY_UP:
			case KEY_PAGE_UP:
				if (0 == get_count.totalnum)
					break;
				if (s_EntitleSel == 0)
				{
					pageIndex = s_EntitleSel+1;
					// 总的授权个数
					sprintf((char *)pArry3,"总的授权页数: %d 页，当前第%d 页",get_count.totalnum,pageIndex);
					GUI_SetProperty("win_dvt_cas_entitle_num_text","string", pArry3);
					
				}
				else
				{
					s_EntitleSel--;
					pageIndex =s_EntitleSel+1;
					// 总的授权个数
					sprintf((char *)pArry3,"总的授权页数: %d 页，当前第%d 页",get_count.totalnum,pageIndex);
					GUI_SetProperty("win_dvt_cas_entitle_num_text","string", pArry3);						
				}
				update_entitle(s_EntitleSel);
				break;
			case KEY_DOWN:
			case KEY_PAGE_DOWN:
				if (0 == get_count.totalnum)
					break;
				if (s_EntitleSel == get_count.totalnum-1)
				{
					pageIndex = s_EntitleSel+1;
					// 总的授权个数
					sprintf((char *)pArry3,"总的授权页数: %d 页，当前第%d 页",get_count.totalnum,pageIndex);
					GUI_SetProperty("win_dvt_cas_entitle_num_text","string", pArry3);
					
				}
				else
				{
					s_EntitleSel++;
					pageIndex =s_EntitleSel+1;
					// 总的授权个数
					sprintf((char *)pArry3,"总的授权页数: %d 页，当前第%d 页",get_count.totalnum,pageIndex);
					GUI_SetProperty("win_dvt_cas_entitle_num_text","string", pArry3);						
				}
				update_entitle(s_EntitleSel);
				break;
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

