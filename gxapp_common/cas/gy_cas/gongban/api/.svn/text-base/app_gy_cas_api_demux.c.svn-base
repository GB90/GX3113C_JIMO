/*****************************************************************************
* 						   CONFIDENTIAL
*        Hangzhou GuoXin Science and Technology Co., Ltd.
*                      (C)2012, All right reserved
******************************************************************************

******************************************************************************
* File Name :	app_ca_api.c
* Author    : 	bingyu
* Project   :	goxceed dvbc
* Type      :
******************************************************************************
* Purpose   :	模块头文件
******************************************************************************
* Release History:
VERSION	Date			  AUTHOR         Description
1.0  	2012.12.02		  bingyu		 			creation
*****************************************************************************/
#include <gxtype.h>
#include "gxmsg.h"
#include "gxapp_sys_config.h"
#include "app_common_play.h"
#include "app_common_table_pmt.h"
#include "app_common_porting_stb_api.h"
#include "app_common_porting_ca_flash.h"
#include "app_common_porting_ca_demux.h"
#include "app_common_porting_ca_descramble.h"
#include "app_common_porting_ca_smart.h"
	
#include "gyca_def.h"
#include "gyca_pub.h"
#include "gyca_osp.h"
#include "gyca_interface.h"
#include "gyca_porting.h"
	
	
#include "app_gy_cas_api_smart.h"
#include "app_gy_cas_api_demux.h"
extern play_scramble_para_t play_Para;

extern ca_descramble_t g_descramlbeList[MAX_SCRAMBLE_CHANNEL];


uint8_t	app_gy_cas_api_release_ecm_filter(void)
{
	ca_filter_t* filter = NULL;
	ca_filter_t temp_filter = {0};
	int32_t filterId =0;
		play_scramble_para_t playpara = {0};
	app_play_get_playing_para(&playpara);

	CAS_Dbg("app_gy_cas_api_release_ecm_filter\n");
	for (filterId = 0; filterId<MAX_FILTER_COUNT;filterId++)
	{
		filter = app_porting_ca_demux_get_by_filter_id(filterId,&temp_filter);
		if(!filter)
		{
			continue;
		}
		if (( 0 != temp_filter.usedStatus)&&(0 != temp_filter.handle)&&
			(0!=temp_filter.channelhandle))
		{
			if ((temp_filter.pid == playpara.p_ecm_pid)||(temp_filter.pid == playpara.p_ecm_pid_video)
				||(temp_filter.pid == playpara.p_ecm_pid_audio))
				app_porting_ca_demux_release_filter( filterId, TRUE);
		}

	}
	/*
	
	filter = app_porting_ca_demux_get_by_filter_id(1);
	if (( 0 != filter->usedStatus)&&(0 != filter->handle)&&(0 != filter->channelhandle))
	{
		ret = GxDemux_FilterDisable(filter->handle);
		CHECK_RET(DEMUX,ret);
		
		ret = GxDemux_FilterFree(filter->handle);
		CHECK_RET(DEMUX,ret);

		ret = GxDemux_ChannelFree(filter->channelhandle);
		CHECK_RET(DEMUX,ret);
		
		memset(filter,0,sizeof(ca_filter_t));
	}*/

	playpara.p_ecm_pid= PSI_INVALID_PID;
	playpara.p_ecm_pid_video= PSI_INVALID_PID;
	playpara.p_ecm_pid_audio= PSI_INVALID_PID;
	app_play_set_playing_para(&playpara);
	GYCA_SetEcmPID(CA_INFOR_CLEAR,NULL);

	return 1;
	
}


uint8_t app_gy_cas_api_start_ecm_filter(play_scramble_para_t *pplayPara)
{
	play_scramble_para_t playPara = {0};
	T_CAServiceInfo ServiceInfo;
	memset(&ServiceInfo,0x00, sizeof(T_CAServiceInfo));
	
	CAS_Dbg("app_gy_cas_api_start_ecm_filter \n");
	if (NULL == pplayPara)
	{
		return 0;
	}
	memcpy(&playPara,pplayPara,sizeof(play_scramble_para_t));

	ServiceInfo.m_byServiceCount = 1;
	ServiceInfo.m_wServiceID[0]= pplayPara->program_num;
	if (0x1FFF != playPara.p_ecm_pid)
	{
		CAS_Dbg("app_gy_cas_api_start_ecm_filter old_p_audio_pid=0x%x old_p_ecm_pid =0x%x\n",pplayPara->p_audio_pid,pplayPara->p_ecm_pid);
		CAS_Dbg("app_gy_cas_api_start_ecm_filter old_p_video_pid=0x%x old_p_ecm_pid =0x%x\n",pplayPara->p_video_pid,pplayPara->p_ecm_pid);

		if (0 == app_porting_ca_descramble_set_pid(1,pplayPara->p_audio_pid ))
		{
			CA_FAIL(" Failed\n");
			return 0;
		}
		
		if (0 == app_porting_ca_descramble_set_pid(0,pplayPara->p_video_pid ))
		{
			CA_FAIL(" Failed\n");
			return 0;
		}
		
		ServiceInfo.m_wEcmPid = pplayPara->p_ecm_pid;
		GYCA_SetEcmPID(CA_INFOR_ADD,&ServiceInfo);
	}

	if (0x1FFF != playPara.p_ecm_pid_audio)
	{
		CAS_Dbg("app_gy_cas_api_start_ecm_filter old_p_audio_pid=0x%x old_p_ecm_pid_video =0x%x\n",pplayPara->p_audio_pid,pplayPara->p_ecm_pid_audio);

		if (0 == app_porting_ca_descramble_set_pid(1,pplayPara->p_audio_pid ))
		{
			CA_FAIL(" Failed\n");
			return 0;
		}
		ServiceInfo.m_wEcmPid = pplayPara->p_ecm_pid_audio;

		GYCA_SetEcmPID(CA_INFOR_ADD,&ServiceInfo);
	}


	if (0x1FFF != playPara.p_ecm_pid_video)
	{
		CAS_Dbg("app_gy_cas_api_start_ecm_filter old_p_video_pid=0x%x old_p_ecm_pid_video =0x%x\n",pplayPara->p_video_pid,pplayPara->p_ecm_pid_video);

		if (0 == app_porting_ca_descramble_set_pid(0,pplayPara->p_video_pid ))
		{
			CA_FAIL(" Failed\n");
			return 0;
		}
		
		ServiceInfo.m_wEcmPid = pplayPara->p_ecm_pid_video;
		GYCA_SetEcmPID(CA_INFOR_ADD,&ServiceInfo);
	}

	if ((0x1FFF == playPara.p_ecm_pid_video)&&(0x1FFF == playPara.p_ecm_pid_audio)&&(0x1FFF == playPara.p_ecm_pid))
	{
		CAS_Dbg("========================GYCA_SetEcmPID    0x1FFFF=============================\n");
/*

		filter = app_porting_ca_demux_get_by_filter_id(1);
		if (( 0 != filter->usedStatus)&&(0 != filter->handle)&&(0 != filter->channelhandle))
		{
			ret = GxDemux_FilterDisable(filter->handle);
			CHECK_RET(DEMUX,ret);
			
			ret = GxDemux_FilterFree(filter->handle);
			CHECK_RET(DEMUX,ret);

			ret = GxDemux_ChannelFree(filter->channelhandle);
			CHECK_RET(DEMUX,ret);
			
			memset(filter,0,sizeof(ca_filter_t));
		}
*/
		GYCA_SetEcmPID(CA_INFOR_CLEAR,NULL);
	}

	CAS_Dbg("app_gy_cas_api_start_ecm_filter End\n");

	return 1;
}



static unsigned short int g_emm_pid=0;

uint8_t app_gy_cas_api_release_emm_filter(void)
{
	ca_filter_t* filter = NULL;
	ca_filter_t temp_filter = {0};
	int32_t filterId =0;
	
	CAS_Dbg("app_gy_cas_api_release_emm_filter \n");
	if(g_emm_pid != 0 && g_emm_pid != 0x1fff)
	{
		for (filterId = 0; filterId<MAX_FILTER_COUNT;filterId++)
		{
			filter = app_porting_ca_demux_get_by_filter_id(filterId,&temp_filter);
			if(!filter)
			{
				continue;
			}
			if (( 0 != temp_filter.usedStatus)&&(0 != temp_filter.handle)&&
				(0!=temp_filter.channelhandle))
			{
				if (temp_filter.pid == g_emm_pid)
				{
					app_porting_ca_demux_release_filter( filterId, TRUE);

				}
			}
		}
		
		GYCA_SetEmmPID(CA_INFOR_CLEAR, 0x1FFF);
	}
	
	return TRUE;
}

uint8_t app_gy_cas_api_start_emm_filter(uint16_t emm_pid)
{
	CAS_Dbg("app_gy_cas_api_start_emm_filter \n");
	if(emm_pid != 0x1FFF)
	{
		g_emm_pid = emm_pid;
		GYCA_SetEmmPID(CA_INFOR_ADD, emm_pid);
	}
	
	return TRUE;
}


bool app_gy_cas_api_check_cat_ca_descriptor_valid(uint8_t *sectionData,uint32_t CA_system_id)
{
	int err;

	err = GYCA_IsMatchCAID(CA_system_id);

	if(GYTRUE != err)
		return FALSE;
	
	return TRUE;
}

bool app_gy_cas_api_check_pmt_ca_descriptor_valid(uint8_t *sectionData,uint32_t CA_system_id)
{
	int err;
	
	
	err = GYCA_IsMatchCAID(CA_system_id);

	if(GYTRUE != err)
		return FALSE;
	
	return TRUE;
}





