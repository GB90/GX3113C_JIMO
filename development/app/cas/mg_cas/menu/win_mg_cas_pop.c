/*****************************************************************************
* 						   CONFIDENTIAL
*        Hangzhou GuoXin Science and Technology Co., Ltd.
*                      (C)2012, All right reserved
******************************************************************************

******************************************************************************
* File Name :	win_mg_cas_pop.c
* Author    : 	zhouhm
* Project   :	goxceed dvbc
* Type      :
******************************************************************************
* Purpose   :	ģ��ͷ�ļ�
******************************************************************************
* Release History:
VERSION	Date			  AUTHOR         Description
1.0  	2014.04.15		  zhouhm 	 			creation
*****************************************************************************/
#include "app_common_play.h"
#include "app_common_porting_stb_api.h"
#include "app_win_interface.h"
#include "app_mg_cas_api_demux.h"

/*
* app_cd_cas30_show_ca_prompt��app_show_promptʵ�ֹ����п��ܴ��ڲ��
* 
*/
extern void app_show_prompt(char* context);
extern void app_hide_prompt(void); 

void app_mg_cas_show_ca_prompt(char* context)
{
	app_show_prompt(context);
	return;		
}

/*
* app_cd_cas30_hide_ca_prompt��app_hide_promptʵ�ֹ����п��ܴ��ڲ��
* 
*/
void app_mg_cas_hide_ca_prompt(void)
{
	app_hide_prompt();
	return;			
}




