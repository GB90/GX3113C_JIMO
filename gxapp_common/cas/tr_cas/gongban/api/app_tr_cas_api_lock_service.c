/*****************************************************************************
* 						   CONFIDENTIAL
*        Hangzhou GuoXin Science and Technology Co., Ltd.
*                      (C)2014, All right reserved
******************************************************************************

******************************************************************************
* File Name :	app_tr_cas_api_lock_service.c
* Author    : 	wangjian
* Project   :	goxceed dvbc
* Type      :
******************************************************************************
* Purpose   :	模块头文件
******************************************************************************
* Release History:
VERSION	Date			  AUTHOR         Description
1.0  	2014.12.30		  wangjian 	 			creation
*****************************************************************************/
#include "app_common_play.h"
#include "app_common_prog.h"
#include "app_common_search.h"
#include "app_common_porting_stb_api.h"
#include "app_common_porting_ca_flash.h"
#include "app_common_lcn.h"

#include "gxmsg.h"
#include "gxprogram_manage_berkeley.h"
#include "service/gxplayer.h"
#include "gxplayer_url.h"
#include "gui_core.h"
#include "app_tr_cas_api_lock_service.h"


typedef struct
{
	uint16_t net_id;
	uint16_t ts_id;
	uint16_t serv_id;
}FORCECHANNEL_INFO_t;

extern void app_win_exist_to_full_screen(void);

static FORCECHANNEL_INFO_t s_forceChannelInfo = {0};
static uint8_t s_lockServStatus = FALSE;
static event_list *pLockServTimerHdl = NULL;/*wangjian add on 20150330*/


static int32_t trcas_get_progpos_by_serviceid(uint16_t net_id, uint16_t ts_id, uint16_t serv_id)
{
	int32_t i = 0;
	int32_t totalNum = 0;
	int32_t ret = 0;
	GxBusPmDataProg prog_data = {0};
	
	totalNum = GxBus_PmProgNumGet();
	if (0 == totalNum)
	{
		return (-1);
	}
	
	for (i = 0; i < totalNum; i++)
	{
		ret = GxBus_PmProgGetByPos(i, 1, (void *)&prog_data);
		if (ret != -1)
		{
			if ((net_id == prog_data.original_id) 
				&& (ts_id == prog_data.ts_id) 
				&& (serv_id == prog_data.service_id))
			{
				break;
			}		
		}
	}

	if (i >= totalNum)
	{
		return (-2);
	}

	return i;
}

static uint8_t trcas_play_lock_service(uint16_t netid, uint16_t tsid, uint16_t servid)
{
	uint8_t result = 0;
	int32_t serviceIndex = 0; // 逻辑频道号
	GxBusPmDataProg prog_data = {0};
	GxBusPmViewInfo sysinfo = {0};
	int32_t config = 0;
	uint16_t pos = 0;

	printf("=++=[trcas_play_lock_service]netid(%d); tsid(%d); servid(%d)=++=\n", netid, tsid, servid);
	
	do
	{	
		if (0 == app_prog_get_num_in_group())
		{
			printf("[trcas_play_lock_service]Cur prog num is 0!!!\n");
			result = 1;
			break;
		}
	
		/*通过逻辑频道 号，获取节目位置*/
		config = app_flash_get_config_lcn_flag();
		if (LCN_STATE_ON == config)
		{
			serviceIndex = GxBus_PmProgPosGetByServiceId(servid); // 返回逻辑频道号
			if (-1 == serviceIndex)
			{
				printf("[trcas_play_lock_service]GxBus_PmProgPosGet fail!!!\n");
				result = 1;
				break;
			}
		
			if (FALSE == app_lcn_list_get_num_prog_pos(serviceIndex, &pos))
			{
				printf("[trcas_play_lock_service]app_get_prog_pos_by_LCN fail!!!\n");
				result = 2;
				break; 					
			}
		}
		else
		{
			serviceIndex = trcas_get_progpos_by_serviceid(netid, tsid, servid);
			if (serviceIndex < 0)
			{
				printf("[trcas_play_lock_service]invalid serviceIndex!!!\n");
				result = 3;
				break;
			}

			pos = serviceIndex;
		}
	
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
		GUI_CreateDialog("win_prog_bar");
		app_play_reset_play_timer(0);
		result = 0;
	}while (0);

	return result;
}

static int trcas_player_lock_timer_check(void *userdata)
{
	uint32_t pos = 0;
	GxBusPmDataProg Prog;
	FORCECHANNEL_INFO_t channelInfo = {0};

	/*Prog search return;*/
	if (FALSE == app_play_get_play_status())
	{
		return 0;
	}
	
	printf("======[trcas_player_lock_timer_check]======.\n");
	channelInfo = s_forceChannelInfo;
	app_prog_get_playing_pos_in_group(&pos);
	if (app_prog_get_num_in_group() > 0)
	{
		GxBus_PmProgGetByPos(pos, 1, &Prog);
		if ((Prog.ts_id == channelInfo.ts_id) 
			&& (Prog.service_id == channelInfo.serv_id))
		{
			return 0;
		}

		trcas_play_lock_service(channelInfo.net_id, channelInfo.ts_id, channelInfo.serv_id);
	}
	
	return 0;
}

void app_tr_cas_api_lock_service(CAS_FORCECHANNEL_STRUCT *pCasForce_channel)
{
	uint8_t result = 0;

	if (NULL == pCasForce_channel)
	{
		printf("[app_tr_cas_api_lock_service]pCasForce_channel is NULL!!!\n");	
		return;
	}

	printf("===[app_tr_cas_api_lock_service]bLockFlag(%d); netId(%d); tsId(%d); servId(%d)===\n",\
		 	pCasForce_channel->bLockFlag, pCasForce_channel->wNetwrokId,\
		 	pCasForce_channel->wTsId, pCasForce_channel->wServId);
	printf("===[app_tr_cas_api_lock_service]len(%d); pbContent(%s)===\n",\
		 	pCasForce_channel->wContentLen, pCasForce_channel->pbContent);
	result = trcas_play_lock_service(pCasForce_channel->wNetwrokId, \
					   				 	pCasForce_channel->wTsId,\
					   				 	pCasForce_channel->wServId);
	if (result != 0)
	{
		return;
	}

	/*Lock the IR.*/
	if (pCasForce_channel->bLockFlag)
	{
		printf("[app_tr_cas_api_lock_service]LOCK IR++++++++++\n");	
		app_tr_cas_api_set_lock_status(TRUE);
	}
	else
	{
		printf("[app_tr_cas_api_lock_service]UNLOCK IR----------\n");
		app_tr_cas_api_set_lock_status(FALSE);
	}

	if (NULL == pLockServTimerHdl)
	{
		pLockServTimerHdl = create_timer(trcas_player_lock_timer_check, 5*1000,\
				   			   			  NULL, TIMER_REPEAT);
	}

	s_forceChannelInfo.net_id = pCasForce_channel->wNetwrokId;
	s_forceChannelInfo.ts_id = pCasForce_channel->wTsId;
	s_forceChannelInfo.serv_id = pCasForce_channel->wServId;
	
	return;	
}

uint8_t app_tr_cas_api_set_lock_status(uint8_t lock_status)
{
	if (s_lockServStatus != lock_status)
	{
		s_lockServStatus = lock_status;
	}
	
	return s_lockServStatus;
}

//应急广播锁定状态
uint8_t app_tr_cas_api_get_lock_status(void)
{
	return s_lockServStatus;
}

