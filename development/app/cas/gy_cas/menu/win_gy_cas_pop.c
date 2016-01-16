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
#include "app_win_interface.h"
#include "win_gy_cas_pop.h"

/*
* app_cd_cas30_show_ca_prompt与app_show_prompt实现功能有可能存在差别
* 
*/
extern void app_show_prompt(char* context);
extern void app_hide_prompt(void); 

void app_gy_cas_show_ca_prompt(char* context)
{
	app_show_prompt(context);
	return;		
}

/*
* app_cd_cas30_hide_ca_prompt与app_hide_prompt实现功能有可能存在差别
* 
*/
void app_gy_cas_hide_ca_prompt(void)
{
	app_hide_prompt();
	return;			
}
void win_gy_cas_exigent_prompt(uint8_t opcode)
{
	char* focus_Window = (char*)GUI_GetFocusWindow();
	if(opcode == 0)
	{
		GUI_CreateDialog("win_search_result");
		GUI_SetInterface("flush", NULL);
	}
	else if(opcode == 1)
	{
		if ((NULL != focus_Window)&&(strcasecmp("win_full_screen", focus_Window) != 0))
		{
			GUI_EndDialog(focus_Window);
			GxCore_ThreadDelay(300);
			GUI_SetInterface("flush", NULL);
			GxCore_ThreadDelay(50);
		}
	}
}




