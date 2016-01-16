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
#include "app_by_cas_api_email.h"
#include "app_by_cas_api_demux.h"
#include "win_by_cas_pop.h"
#include "win_by_cas_rolling_message.h"
#include "win_by_cas_finger.h"
#include "win_by_cas_email.h"

/*
* app_by_cas_show_ca_prompt与app_show_prompt实现功能有可能存在差别
* 
*/
extern void app_show_prompt(char* context);
extern void app_hide_prompt(void); 
void app_by_cas_show_ca_prompt(char* context)
{
	app_show_prompt(context);
	return;		
}

/*
* app_by_cas_hide_ca_prompt与app_hide_prompt实现功能有可能存在差别
* 
*/
void app_by_cas_hide_ca_prompt(void)
{
	app_hide_prompt();
	return;			
}




