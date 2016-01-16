/*****************************************************************************
* 						   CONFIDENTIAL
*        Hangzhou GuoXin Science and Technology Co., Ltd.
*                      (C)2012, All right reserved
******************************************************************************

******************************************************************************
* File Name :	win_dvt_cas_pop.c
* Author    : 	zhouhm
* Project   :	goxceed dvbc
* Type      :
******************************************************************************
* Purpose   :	模块头文件
******************************************************************************
* Release History:
VERSION	Date			  AUTHOR         Description
1.0  	2013.12.02		  zhouhm 	 			creation
*****************************************************************************/
#include "app_common_play.h"
#include "app_common_porting_stb_api.h"
#include "app_win_interface.h"
#include "app_dvt_cas_api_demux.h"
#include "win_dvt_cas_finger.h"
#include "app_dvt_cas_api_email.h"

/*
* app_cd_cas30_show_ca_prompt与app_show_prompt实现功能有可能存在差别
* 
*/
extern void app_show_prompt(char* context);
extern void app_hide_prompt(void); 

void app_dvt_cas_show_ca_prompt(char* context)
{
	app_show_prompt(context);
	return;		
}

/*
* app_cd_cas30_hide_ca_prompt与app_hide_prompt实现功能有可能存在差别
* 
*/
void app_dvt_cas_hide_ca_prompt(void)
{
	app_hide_prompt();
	return;			
}



