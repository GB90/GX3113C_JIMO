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
#include "app_common_prog.h"
#include "app_common_porting_stb_api.h"
#include "app_common_lcn.h"
#include "app_common_porting_ca_flash.h"
#include "app_common_porting_ca_smart.h"
#include "app_common_porting_ca_demux.h"
#include "app_common_porting_ca_descramble.h"
#include "gxplayer_url.h"
#include "app_by_cas_api_lock_service.h"

 static uint8_t by_lock_service_status = FALSE;
extern void app_win_exist_to_full_screen(void);
void app_by_cas_api_lock_service(BU16 wOriNetID,BU16 wTSID,BU16 wServiceID,BU8 *pUTCTime)
{
	/*
	* 根据tsid\service id查找节目播放
	*/

	int32_t config;
	int32_t serviceIndex= 0;	// 逻辑频道号
	GxBusPmDataProg prog_data = {0};
	uint16_t pos=0;
	printf(" app_by_cas_api_lock_service  wOriNetID=%d wTSID=%d wServiceID=%d pUTCTime=0x%x\n",
		wOriNetID,wTSID,wServiceID,(unsigned int)pUTCTime);  
	
	serviceIndex = GxBus_PmProgPosGet(wTSID,wServiceID);		// 返回逻辑频道号
	if (-1 == serviceIndex)
	{
		printf("app_by_cas_api_lock_service GxBus_PmProgPosGet fail\n");
		return;
	}

	/*
	* 根据逻辑频道号获取节目索引
	*/

	config = app_flash_get_config_lcn_flag();
	if(LCN_STATE_ON == config)
	{
		if (FALSE == app_lcn_list_get_num_prog_pos(serviceIndex,&pos))
		{
			printf("app_mg_cas_api_lock_service app_get_prog_pos_by_LCN fail\n");
			return; 					
		}
	}
	else
	{
		app_prog_get_pos_in_group_by_service_id(wServiceID,&pos);
	}	
	/************************************************************************************************
	*    应急广播需退出到全屏下，需退出到全屏,并且不响应任何按键
	*/

	app_win_exist_to_full_screen();

	GxBus_PmProgGetByPos(pos,1,&prog_data);	
	printf("app_by_cas_api_lock_service PROG NAME = %s\n", prog_data.prog_name);

	app_prog_save_playing_pos_in_group(pos);
	app_play_switch_prog_clear_msg();	
	app_play_reset_play_timer(0);			
	app_by_cas_api_set_lock_status(TRUE);
	return;	
}

uint8_t app_by_cas_api_set_lock_status(uint8_t lock_status)
{
	if (by_lock_service_status != lock_status)
		by_lock_service_status = lock_status;
	return by_lock_service_status;
	
}

uint8_t app_by_cas_api_get_lock_status(void)
{
	return by_lock_service_status;
}


