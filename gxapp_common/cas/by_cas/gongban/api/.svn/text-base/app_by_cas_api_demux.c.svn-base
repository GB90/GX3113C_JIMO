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
#include "app_by_cas_api_demux.h"
#include "app_common_porting_ca_smart.h"
#include "app_common_porting_ca_demux.h"
#include "app_common_porting_ca_descramble.h"
#include "gx_demux.h"
#include "gx_descrambler.h"

#include "BYCAca_stb.h"



uint8_t bat_recieve_flag = FALSE;
static handle_t bat_filter =0;
static handle_t bat_channel=0;


handle_t app_by_cas_api_bat_get_filter_handle(void)
{
	return bat_filter;
}

handle_t app_by_cas_api_bat_get_channel_handle(void)
{
	return bat_channel;
}


void app_by_cas_api_bat_filter_notify(handle_t Filter, const uint8_t* Section, size_t Size)
{
	
    int16_t            pid;
    uint8_t*            data = (uint8_t*)Section;
    int32_t ret;


//    ASSERT(Section != NULL);
    ret = GxDemux_FilterGetPID(Filter,&pid);
    if (BAT_PID != pid)
    	{
             CAS_Dbg("app_by_cas_api_bat_filter_notify pid error  pid:0x%x\n", pid);
    		return ;
    	}

	BYCA_ParseBAT(data);
	printf("app_by_cas_api_bat_filter_notify BYCA_ParseBAT\n");
	bat_recieve_flag = TRUE;
	{
		app_by_cas_api_release_bat_filter();
	}
			
        return;
		
}

int app_by_cas_api_bat_filter_open(void)
{
    int32_t ret;
	handle_t filter;
	uint8_t  match = BAT_TID;
	uint8_t  mask  = 0xFF;
	CAS_Dbg("app_table_bat_filter_open\n");
	app_porting_psi_demux_lock();
	if(bat_channel != 0)
	{
		printf("bat_channel = 0x%x\n",bat_channel);
		app_porting_psi_demux_unlock();
		return 0;
	}
	
	bat_channel = GxDemux_ChannelAllocate(0, BAT_PID);
	if ((0 == bat_channel)||(-1 == bat_channel))
		{
			CA_ERROR("	bat_channel=0x%x\n",bat_channel);
			bat_channel = 0;
			app_porting_psi_demux_unlock();
			return 0;
		}
	
	filter = GxDemux_FilterAllocate(bat_channel);
	if (( 0 == filter)||(-1 == filter))
	{
		ret = GxDemux_ChannelFree(bat_channel);
		CHECK_RET(DEMUX,ret);
		bat_channel = 0;
	
		CA_ERROR(" filter=0x%x\n",filter);
		filter = 0;
		app_porting_psi_demux_unlock();
		return 0;
	}
	
	bat_filter = filter;
	ret = GxDemux_FilterSetup(filter, &match, &mask, TRUE, TRUE,0, 1);
	CHECK_RET(DEMUX,ret);
	ret = GxDemux_ChannelEnable(bat_channel);
	CHECK_RET(DEMUX,ret);
	ret = GxDemux_FilterEnable(filter);
	CHECK_RET(DEMUX,ret);
	app_porting_psi_demux_unlock();

	return 0;
}


uint8_t	app_by_cas_api_release_bat_filter(void)
{
    int32_t ret;
	app_porting_psi_demux_lock();

	if (0 != bat_channel)
		{
			if(bat_filter != 0)
			{
//				ret = GxDemux_FilterDisable(bat_filter);
//				CHECK_RET(DEMUX,ret);
				ret = GxDemux_FilterFree(bat_filter);
				CHECK_RET(DEMUX,ret);
				bat_filter = 0;
			}
	
			ret = GxDemux_ChannelFree(bat_channel);
			CHECK_RET(DEMUX,ret);
			bat_channel = 0;
		}
	app_porting_psi_demux_unlock();

	return 0;	
}


void app_by_cas_filter_notify(handle_t Filter, const uint8_t* Section, size_t Size)
{
	uint16_t            filterID;
    uint16_t            i;
    int16_t            pid;
    uint16_t            section_length;
	bool bFlag = FALSE;
	uint8_t byReqID = 0;
    uint8_t*            data = (uint8_t*)Section;
    int                 len = Size;
	ca_filter_t sfilter = {0};
    int32_t ret;



//    ASSERT(Section != NULL);
   ret = GxDemux_FilterGetPID(Filter,&pid);
 
	        while(len > 0) {
	            bFlag = FALSE;
	            section_length = ((data[1] & 0x0F) << 8) + data[2] + 3;
			for (filterID = 0; filterID< MAX_FILTER_COUNT; filterID++)
			{
			      app_porting_ca_demux_get_by_filter_id(filterID,&sfilter);
				if (( 0 !=sfilter.usedStatus)&&(pid == sfilter.pid)
					&&(0 !=sfilter.channelhandle))
				{
					if (Filter == sfilter.handle)
						{
							for (i = 0; i<sfilter.filterLen; i++)
							{
								if ((data[i]&sfilter.mask[i]) != (sfilter.match[i]&sfilter.mask[i]))
									{
										CA_FAIL(" data[%d]=0x%x cafilter[filterID].match[%d]=0x%x \n",
											i,data[i],i,sfilter.match[i]);
										return;
									}
							}

//							sfilter->nms = 0;
							byReqID = sfilter.byReqID;
							bFlag = TRUE;
							break;
						}
				}
			}

			if (FALSE == bFlag)
			{
				CA_FAIL(" FALSE == bFlag\n");
				return;
			}

			BYCASTB_StreamGuardDataGot(byReqID,TRUE,pid,data,section_length);

//			  	CAS_Dbg("app_by_cas_filter_notify byReqID=0x%x,wPID=0x%x data[0]=0x%x,section_length=%d\n",
//		byReqID,pid,data[0],section_length);


			/*过滤一次模式，分析完数据后释放filter*/
/*			if (0x80 == (0x80&byReqID))
			{
				return;
			}*/

	            data += section_length;
	            len -= section_length;
	        }
        return;

		
}


void app_by_cas_filter_timeout_notify(handle_t Filter, const uint8_t* Section, size_t Size)
{
	uint8_t filterId;
	uint16_t	pid;
	ca_filter_t filter = {0};

	for (filterId= 0; filterId< MAX_FILTER_COUNT; filterId++)
	{
		app_porting_ca_demux_get_by_filter_id(filterId,&filter);
		if (( 0 != filter.usedStatus)&&(0 != filter.handle)
			&&(0 != filter.channelhandle))
			{
			
				if (Filter == filter.handle)
					{
						pid = filter.pid;

						/* 私有数据接收回调 */
						BYCASTB_StreamGuardDataGot( filter.byReqID,
					  	FALSE,
						pid,
						NULL,
						0); 					
					}												
			}
	}


   return;	
}


uint8_t	app_by_cas_api_release_ecm_filter(void)
{
	int32_t filterId =0;
	ca_filter_t filter = {0};
	play_scramble_para_t playpara = {0};
	app_play_get_playing_para(&playpara);

	for (filterId = 0; filterId<MAX_FILTER_COUNT;filterId++)
	{
		app_porting_ca_demux_get_by_filter_id(filterId,&filter);
		if (( 0 != filter.usedStatus)&&(0 != filter.handle)&&
			(0!=filter.channelhandle))
			{
				if ((filter.pid == playpara.p_ecm_pid)||(filter.pid == playpara.p_ecm_pid_video)
					||(filter.pid == playpara.p_ecm_pid_audio))
					app_porting_ca_demux_release_filter( filterId, TRUE);
			}

	}

	/*
	*如果当前频道为不加扰的频道，则需要将 byCount = 0或 pServiceInfos = NULL，可以停止ECM数据
	*/

	BYCASTB_SetCurEcmInfos(0,BNULL);
			
	playpara.p_ecm_pid= PSI_INVALID_PID;
	playpara.p_ecm_pid_video= PSI_INVALID_PID;
	playpara.p_ecm_pid_audio= PSI_INVALID_PID;
	app_play_set_playing_para(&playpara);

	return 1;
	
}

uint8_t app_by_cas_api_start_ecm_filter(play_scramble_para_t *pplayPara)
{
	SBYCAServiceInfo 		pServiceInfos={0};
	play_scramble_para_t playPara = {0};
	CAS_Dbg("app_by_cas_api_start_ecm_filter \n");
	if (NULL == pplayPara)
	{
		return 0;
	}

	memcpy(&playPara,pplayPara,sizeof(play_scramble_para_t));

//	app_by_cas_api_release_ecm_filter();

	if ((0x1FFF ==playPara.p_ecm_pid )&&(0x1FFF == playPara.p_ecm_pid_audio)&&(0x1FFF == playPara.p_ecm_pid_video))
		{
			/*清流节目*/
//			BYCASTB_SetCurEcmInfos(0,&pServiceInfos/*BNULL*/);

			return 0;
		}

	BYCASTB_SetCurEcmInfos (BYCAS_LIST_START,BNULL);

	pServiceInfos.m_byServiceCount = 1;
	pServiceInfos.m_wServiceID[0] = pplayPara->program_num;
	if (0x1FFF != playPara.p_ecm_pid)
		{
				CAS_Dbg("app_by_cas_api_start_ecm_filter old_p_audio_pid=0x%x old_p_ecm_pid =0x%x\n",pplayPara->p_audio_pid,pplayPara->p_ecm_pid);
				CAS_Dbg("app_by_cas_api_start_ecm_filter old_p_video_pid=0x%x old_p_ecm_pid =0x%x\n",pplayPara->p_video_pid,pplayPara->p_ecm_pid);


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

				pServiceInfos.m_wEcmPid				= playPara.p_ecm_pid;
				BYCASTB_SetCurEcmInfos(BYCAS_LIST_ADD,&pServiceInfos);

		}

		if (0x1FFF != playPara.p_ecm_pid_audio)
		{
				CAS_Dbg("app_by_cas_api_start_ecm_filter old_p_video_pid=0x%x old_p_ecm_pid_video =0x%x\n",pplayPara->p_video_pid,pplayPara->p_ecm_pid_video);


				if (0 == app_porting_ca_descramble_set_pid(1,pplayPara->p_audio_pid ))
				{
					CA_FAIL(" Failed\n");
					return 0;
				}

				pServiceInfos.m_wEcmPid				= playPara.p_ecm_pid_audio;
				BYCASTB_SetCurEcmInfos(BYCAS_LIST_ADD,&pServiceInfos);
		}


	if (0x1FFF != playPara.p_ecm_pid_video)
		{
				CAS_Dbg("app_by_cas_api_start_ecm_filter old_p_video_pid=0x%x old_p_ecm_pid_video =0x%x\n",pplayPara->p_video_pid,pplayPara->p_ecm_pid_video);

				if (0 == app_porting_ca_descramble_set_pid(0,pplayPara->p_video_pid ))
				{
					CA_FAIL(" Failed\n");
					return 0;
				}


				pServiceInfos.m_wEcmPid				= playPara.p_ecm_pid_video;
				BYCASTB_SetCurEcmInfos(BYCAS_LIST_ADD,&pServiceInfos);
		}


			BYCASTB_SetCurEcmInfos(BYCAS_LIST_END,/*&pServiceInfos*/BNULL);
			CAS_Dbg("app_by_cas_api_start_ecm_filter End\n");

	return 1;
}

uint8_t app_by_cas_api_release_emm_filter(void)
{
	int32_t filterId =0;
	ca_filter_t filter = {0};
	play_scramble_para_t playpara = {0};
	app_play_get_playing_para(&playpara);

	for (filterId = 0; filterId<MAX_FILTER_COUNT;filterId++)
	{
		app_porting_ca_demux_get_by_filter_id(filterId,&filter);
		if (( 0 != filter.usedStatus)&&(0 != filter.handle)&&
			(0!=filter.channelhandle))
			{
				if (filter.pid == playpara.p_emm_pid)
					app_porting_ca_demux_release_filter( filterId, TRUE);
			}

	}

	BYCASTB_SetEmmPids(0,NULL);
	playpara.p_emm_pid = 0x1fff;
	app_play_set_playing_para(&playpara);

	return 0;
}

uint8_t app_by_cas_api_start_emm_filter(uint16_t emm_pid)
{
	BYCASTB_SetEmmPids(0,NULL/*&emm_pid*/);
	BYCASTB_SetEmmPids(1,&emm_pid);
	return 0;
}

bool app_by_cas_api_check_cat_ca_descriptor_valid(uint8_t *sectionData,uint32_t CA_system_id)
{
	return BYCASTB_CASIDVerify(CA_system_id);
}

bool app_by_cas_api_check_pmt_ca_descriptor_valid(uint8_t *sectionData,uint32_t CA_system_id)
{
	return BYCASTB_CASIDVerify(CA_system_id);
}

void app_by_cas_api_nit_zone_des(uint8_t* databuf)
{
	printf("app_by_cas_api_nit_zone_des BYCAS_NitZONEDes\n");
	if(TRUE == bat_recieve_flag)
		return ;
	return BYCAS_NitZONEDes(databuf);
}







