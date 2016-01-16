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
#include "app_common_play.h"
#include "app_common_porting_stb_api.h"
#include "win_kn_cas_pop.h"


/*
* app_kn_cas_show_ca_prompt与app_show_prompt实现功能有可能存在差别
* 
*/
extern void app_show_prompt(char* context);
extern void app_hide_prompt(void); 
void app_kn_cas_show_ca_prompt(char* context)
{
	app_show_prompt(context);
	return;		
}

/*
* app_by_cas_hide_ca_prompt与app_hide_prompt实现功能有可能存在差别
* 
*/
void app_kn_cas_hide_ca_prompt(void)
{
	app_hide_prompt();
	return;			
}

void app_kn_cas_show_tip_prompt(char* context)
{
	//GUI_CreateDialog("win_pop_tip_long");
	//GUI_SetProperty("win_pop_tip_tip_long", "string", context);
	//GUI_SetFocusWidget("win_full_screen");
	GUI_SetProperty("win_pop_tip_tip_long", "string", context);
	GUI_SetProperty("win_pop_tip_long_rect_image","state","show"); 
	GUI_SetProperty("win_pop_tip_tip_long","state","show"); 
	GUI_SetProperty("win_pop_tip_title_long","state","show"); 
	
	return;		
}
void app_kn_cas_hide_tip_prompt(void)
{
	GUI_SetProperty("win_pop_tip_long_rect_image","state","hide"); 
	GUI_SetProperty("win_pop_tip_tip_long","state","hide"); 
	GUI_SetProperty("win_pop_tip_title_long","state","hide"); 
	return;		
}




