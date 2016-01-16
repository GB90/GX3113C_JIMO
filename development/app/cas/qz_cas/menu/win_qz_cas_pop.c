/*****************************************************************************
* 						   CONFIDENTIAL
*        Hangzhou GuoXin Science and Technology Co., Ltd.
*                      (C)2012, All right reserved
******************************************************************************

******************************************************************************
* File Name :	win_qz_cas_pop.c
* Author    : 	chenth
* Project   :	goxceed dvbc
* Type      :
******************************************************************************
* Purpose   :	模块头文件
******************************************************************************
* Release History:
VERSION	Date			  AUTHOR         Description
1.0  	2014.5.13		  chenth 	 			creation
*****************************************************************************/
#include "app_common_play.h"
#include "app_common_porting_stb_api.h"
#include "app_win_interface.h"


extern void app_show_prompt(char* context);
extern void app_hide_prompt(void); 

void app_qz_cas_show_ca_prompt(char* context)
{
	app_show_prompt(context);
	return;		
}

void app_qz_cas_hide_ca_prompt(void)
{
	app_hide_prompt();
	return;			
}




