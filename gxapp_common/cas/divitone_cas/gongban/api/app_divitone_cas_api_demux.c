/*****************************************************************************
* 						   CONFIDENTIAL
*        Hangzhou GuoXin Science and Technology Co., Ltd.
*                      (C)2012, All right reserved
******************************************************************************

******************************************************************************
* File Name :	app_xinshimao_ca_api_demux.c
* Author    : 	zhouhm
* Project   :	goxceed dvbc
* Type      :
******************************************************************************
* Purpose   :	模块头文件
******************************************************************************
* Release History:
VERSION	Date			  AUTHOR         Description
1.0  	2013.09.18		  zhouhm 	 			creation
*****************************************************************************/
#include "app_common_play.h"
#include "app_common_porting_stb_api.h"
#include "app_common_porting_ca_smart.h"
#include "app_common_porting_ca_demux.h"
#include "app_common_porting_ca_descramble.h"
#include "gx_demux.h"
#include "gx_descrambler.h"
#include "DiviGuardCa.h"
#ifdef DVB_AD_TYPE_DIVITONE_FLAG
#include "app_divitone_ads_porting_stb_api.h"
#endif 
extern void Divitone_ReleasePrivateDataFilter( uint8_t  byReqID,uint16_t wPid );

void app_divitone_cas_filter_timeout_notify(handle_t Filter, const uint8_t* Section, size_t Size)
{
	uint8_t filterId;
	uint16_t	pid;
	uint8_t 	 byReqID;

	ca_filter_t filter = {0};

	for (filterId= 0; filterId< MAX_FILTER_COUNT; filterId++)
	{
		app_porting_ca_demux_get_by_filter_id(filterId,&filter);
		if (( 0 != filter.usedStatus)&&(0 != filter.handle)
			&&(0 != filter.channelhandle))	
			{		
				if (Filter == filter.handle)
				{
					byReqID = filter.byReqID;
					pid = filter.pid;
					if(CA_STB_FILTER_4 == byReqID)
					{
						Divitone_ReleasePrivateDataFilter(byReqID,pid);
					}

					DIVI_TableReceived( byReqID, 
									FALSE, 
									pid ,
									NULL, 
									0 ); 				
				}												
			}
	}
   	return;	
}


void app_divitone_cas_filter_notify(handle_t Filter, const uint8_t* Section, size_t Size)
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

	while(len > 0) 
	{
		bFlag = FALSE;
		section_length = ((data[1] & 0x0F) << 8) + data[2] + 3;
		for (filterID = 0; filterID< MAX_FILTER_COUNT; filterID++)
		{
			app_porting_ca_demux_get_by_filter_id(filterID,&sfilter);
			if (( 0 !=sfilter.usedStatus)&&(pid == sfilter.pid)
					&&(0 !=sfilter.channelhandle))
			{
				if ((0x10 == pid)||(Filter == sfilter.handle))
				{
					/*NIT表或相应FILTER数据*/
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

		/*过滤一次模式，分析完数据后释放filter*/

		if(CA_STB_FILTER_4 == byReqID)
		{
			Divitone_ReleasePrivateDataFilter(byReqID,pid);
		}
		if( data[0] ==0x84 )
		{
			//		CAS_Dbg("tableid = 0x%x byReqID=%d   Filter=%d Filter_data=%d\n",data[0],byReqID,data[5],data[6]);
			if( data[5]==0x8 || data[5]==0x9 )
			{
				//		CAS_DUMP("ad_data:",data,section_length);
#ifdef DVB_AD_TYPE_DIVITONE_FLAG
				ad_data_got(data,section_length);
#endif
				data += section_length;
				len -= section_length;
				continue;
			}
		}

		DIVI_TableReceived( byReqID, 
				TRUE, 
				pid, 
				data, 
				section_length );

		/*过滤一次模式，分析完数据后释放filter*/
		if(CA_STB_FILTER_4 == byReqID)
		{
			return;
		}
		data += section_length;
		len -= section_length;
	}
	return;		
}


uint8_t	app_divitone_cas_api_release_ecm_filter(void)
{
	int32_t filterId =0;
	ca_filter_t filter = {0};
	int16_t pid;
	uint8_t byReqID = 0;
	DIVI_CAServiceInfo CAServiceInfo = {0};

	play_scramble_para_t playpara = {0};
	app_play_get_playing_para(&playpara);


	for (filterId = 0; filterId<MAX_FILTER_COUNT;filterId++)
	{
		app_porting_ca_demux_get_by_filter_id(filterId,&filter);
		if (( 0 != filter.usedStatus)&&(0 != filter.handle)
				&&(0x10 != filter.pid)&&(0!=filter.channelhandle)
				&&(CA_STB_FILTER_4 == filter.byReqID))
		{
			pid = filter.pid;
			byReqID = filter.byReqID;

			Divitone_ReleasePrivateDataFilter(byReqID,pid);
			DIVI_TableReceived(byReqID, 
					FALSE, 
					pid ,
					NULL, 
					0 );				
			break;
		}
	}

	playpara.p_ecm_pid= PSI_INVALID_PID;
	playpara.p_ecm_pid_video= PSI_INVALID_PID;
	playpara.p_ecm_pid_audio= PSI_INVALID_PID;
	app_play_set_playing_para(&playpara);


	return 1;

}

uint8_t app_divitone_cas_api_start_ecm_filter(play_scramble_para_t *pplayPara)
{
	DIVI_CAServiceInfo CAServiceInfo = {0};
	play_scramble_para_t playPara = {0};
	CAS_Dbg("app_divitone_cas_api_start_ecm_filter\n");

	if (NULL == pplayPara)
	{
		return 0;
	}

	memcpy(&playPara,pplayPara,sizeof(play_scramble_para_t));

	CAServiceInfo.m_bServiceCount = 1;
	CAServiceInfo.m_wServiceId[0] = pplayPara->program_num;

	if (0x1FFF != playPara.p_ecm_pid)
	{
		CAS_Dbg("DIVICASTB_SetCurEcmInfo old_p_audio_pid=0x%x old_p_ecm_pid =0x%x\n",pplayPara->p_audio_pid,pplayPara->p_ecm_pid);
		CAS_Dbg("DIVICASTB_SetCurEcmInfo old_p_video_pid=0x%x old_p_ecm_pid =0x%x\n",pplayPara->p_video_pid,pplayPara->p_ecm_pid);

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

		CAServiceInfo.m_wEcmPid = playPara.p_ecm_pid;
		DIVI_SetEcmPID(0,&CAServiceInfo);

	}

	if (0x1FFF != playPara.p_ecm_pid_audio)
	{
		CAS_Dbg("DIVICASTB_SetCurEcmInfo old_p_audio_pid=0x%x old_p_ecm_pid_video =0x%x\n",pplayPara->p_audio_pid,pplayPara->p_ecm_pid_audio);

		if (0 == app_porting_ca_descramble_set_pid(1,pplayPara->p_audio_pid ))
		{
			CA_FAIL(" Failed\n");
			return 0;
		}

		CAServiceInfo.m_wEcmPid = playPara.p_ecm_pid_audio;

		DIVI_SetEcmPID(0,&CAServiceInfo);
	}


	if (0x1FFF != playPara.p_ecm_pid_video)
	{
		CAS_Dbg("DIVICASTB_SetCurEcmInfo old_p_video_pid=0x%x old_p_ecm_pid_video =0x%x\n",pplayPara->p_video_pid,pplayPara->p_ecm_pid_video);

		if (0 == app_porting_ca_descramble_set_pid(0,pplayPara->p_video_pid ))
		{
			CA_FAIL(" Failed\n");
			return 0;
		}

		CAServiceInfo.m_wEcmPid = playPara.p_ecm_pid_video;

		DIVI_SetEcmPID(0,&CAServiceInfo);
	}

	CAS_Dbg("GxCaSetEcmPidByRecordPid End\n");

	return 1;
}

uint8_t app_divitone_cas_api_release_emm_filter(void)
{
	play_scramble_para_t playpara = {0};

	CAS_Dbg("app_divitone_cas_api_release_emm_filter\n");
	app_play_get_playing_para(&playpara);
	playpara.p_emm_pid = 0x1fff;
	app_play_set_playing_para(&playpara);

	return 0;
}

uint8_t app_divitone_cas_api_start_emm_filter(uint16_t emm_pid)
{
	CAS_Dbg("app_divitone_cas_api_start_emm_filter\n");
	DIVI_SetEmmPID(0,emm_pid);
	return 0;
}

bool app_divitone_cas_api_check_cat_ca_descriptor_valid(uint8_t *sectionData,uint32_t CA_system_id)
{
	if( DIVI_IsMatchCAID(CA_system_id) )
		return TRUE;
	else
	{
		printf("cat DIVI_IsMatchCAID false CA_system_id=0x%x\n",CA_system_id);		
		return FALSE;
	}
}

bool app_divitone_cas_api_check_pmt_ca_descriptor_valid(uint8_t *sectionData,uint32_t CA_system_id)
{
	if( DIVI_IsMatchCAID(CA_system_id) )
		return TRUE;
	else
	{
		printf("pmt DIVI_IsMatchCAID false CA_system_id=0x%x\n",CA_system_id);			
		return FALSE;
	}
}

uint16_t app_divitone_cas_api_get_emm_pid(void)
{
	play_scramble_para_t playpara = {0};
	app_play_get_playing_para(&playpara);
	return playpara.p_emm_pid ;
}

uint16_t app_divitone_cas_api_get_ecm_pid(void)
{
	play_scramble_para_t playpara = {0};
	app_play_get_playing_para(&playpara);
	if (0x1fff != playpara.p_ecm_pid)
		return playpara.p_ecm_pid;

	if (0x1fff != playpara.p_ecm_pid_audio)
		return playpara.p_ecm_pid_audio;

	if (0x1fff != playpara.p_ecm_pid_video)
		return playpara.p_ecm_pid_video;


	return 0x1fff;
}


