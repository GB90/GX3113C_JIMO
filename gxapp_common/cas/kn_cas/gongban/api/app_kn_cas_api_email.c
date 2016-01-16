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
#include "app_common_epg.h"
#include "cas_hcn_Public.h"


int32_t app_kn_cas_gxmsg_ca_on_event_email(GxMsgProperty0_OnEvent* event)
{
	return 0;
}

/*
* 反授权图标显示、隐藏
*/
int32_t app_kn_cas_email_flag_exec()
{
	return 0;
}



int32_t app_kn_cas_api_init_email_data(void)
{
	return 0;

}


char * app_kn_cas_api_get_email_data(ca_get_date_t *data)
{
	return 0;

}

uint8_t app_kn_cas_api_get_email_count(ca_get_count_t *data)
{
	return 0;
}

bool app_kn_cas_api_delete_email(ca_delete_data_t *data)
{

	return 0;
}

bool app_kn_cas_api_delete_all_email(ca_delete_data_t *data)
{
	return 0;
}


