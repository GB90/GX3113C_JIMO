/*****************************************************************************
* 						   CONFIDENTIAL
*        Hangzhou GuoXin Science and Technology Co., Ltd.
*                      (C)2014, All right reserved
******************************************************************************

******************************************************************************
* File Name :	app_mg_cas_api_lock_service.c
* Author    : 	zhouhm
* Project   :	goxceed dvbc
* Type      :
******************************************************************************
* Purpose   :	模块头文件
******************************************************************************
* Release History:
VERSION	Date			  AUTHOR         Description
1.0  	2014.04.16		  zhouhm 	 			creation
*****************************************************************************/
#include "app_common_play.h"
#include "app_common_prog.h"
#include "app_common_search.h"
#include "app_common_porting_stb_api.h"
#include "app_common_porting_ca_flash.h"
#include "app_common_porting_ca_smart.h"
#include "app_common_porting_ca_demux.h"
#include "app_common_porting_ca_descramble.h"
#include "app_common_lcn.h"
#include "app_mg_cas312_api_demux.h"
#include "app_mg_cas312_api_lock_service.h"
#include "gxmsg.h"
#include "gxprogram_manage_berkeley.h"
#include "service/gxplayer.h"
#include "gxplayer_url.h"

extern 	void MGCA312_ShowBugMessage(MG_S16 messageType);
extern void app_win_exist_to_full_screen(void);

static uint8_t s_lockServStatus = FALSE;


void app_mg_cas312_api_lock_service(uint16_t wServiceID)
{
	int32_t serviceIndex= 0;	// 逻辑频道号
	GxBusPmDataProg prog_data = {0};
	GxBusPmViewInfo sysinfo;
	GxBusPmViewInfo old_sysinfo;
	int32_t config;
	uint16_t pos;

	printf("[app_mg_cas312_api_lock_service]wServiceID(%d)===\n", wServiceID);	
	GxBus_PmViewInfoGet(&sysinfo);
	memcpy(&old_sysinfo, &sysinfo, sizeof(GxBusPmViewInfo));
	serviceIndex = GxBus_PmProgPosGetByServiceId(wServiceID); // 返回逻辑频道号
	if (-1 == serviceIndex)
	{
		printf("[app_mg_cas312_api_lock_service]GxBus_PmProgPosGet fail!!!\n");
		return;
	}

	/*通过逻辑频道 号，获取节目位置*/
	config = app_flash_get_config_lcn_flag();
	if (LCN_STATE_ON == config)
	{
		if (FALSE == app_lcn_list_get_num_prog_pos(serviceIndex,&pos))
		{
			printf("[app_mg_cas312_api_lock_service]app_get_prog_pos_by_LCN fail!!!\n");
			return; 					
		}
	}
	else
	{
		app_prog_get_pos_in_group_by_service_id(wServiceID,&pos);
	}	

	MGCA312_ShowBugMessage(CA_MG_EMM_URGENT_SERVICE);
	app_win_exist_to_full_screen();

	GxBus_PmProgGetByPos(pos, 1, &prog_data); 
	if (GXCORE_SUCCESS == GxBus_PmViewInfoGet(&sysinfo))
	{
		if (sysinfo.stream_type == GXBUS_PM_PROG_TV)
		{
			sysinfo.tv_prog_cur = prog_data.id;
		}
#if (1 == DVB_HD_LIST)
		else if(sysinfo.stream_type == GXBUS_PM_PROG_HD_SERVICE)
		{
			sysinfo.hd_prog_cur = prog_data.id;
		}
#endif
		else
		{
			sysinfo.radio_prog_cur = prog_data.id;
		}
		
		GxBus_PmViewInfoModify(&sysinfo);
		app_prog_save_playing_pos_in_group(pos);//设置节目号
	}

	app_play_switch_prog_clear_msg();	
	app_play_reset_play_timer(0);//定时器切台,准备播放节目		
	//app_mg_cas312_api_set_lock_status(TRUE);
	return;	
}

uint8_t app_mg_cas312_api_set_lock_status(uint8_t lock_status)
{
	if (s_lockServStatus != lock_status)
	{
		s_lockServStatus = lock_status;
	}
	
	return s_lockServStatus;
}

//应急广播锁定状态
uint8_t app_mg_cas312_api_get_lock_status(void)
{
	return s_lockServStatus;
}

