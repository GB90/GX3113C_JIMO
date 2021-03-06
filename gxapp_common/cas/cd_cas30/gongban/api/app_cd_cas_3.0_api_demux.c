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
//#include "app_win_interface.h"
#include "app_common_porting_ca_smart.h"
#include "app_common_porting_ca_demux.h"
#include "app_common_porting_ca_descramble.h"
#include "CDCASS.h"
#include "gx_demux.h"
#include "gx_descrambler.h"

extern void cd_cas_30_private_data_got( CDCA_U8        byReqID,
								  	CDCA_BOOL      bTimeout,
									CDCA_U16       wPid,
									const CDCA_U8* pbyReceiveData,
									CDCA_U16       wLen            );

static void app_cd_cas_30_set_ecm_pid( CDCA_U8 byType,
                               const SCDCASServiceInfo* pServiceInfo )
{
	CAS_Dbg("CDCASTB_SetEcmPid byType=0x%02x,",byType);
	if(pServiceInfo)
	{
		printf("pServiceInfo->m_wEcmPid=0x%x,\
pServiceInfo->m_byServiceNum=%d,	\
pServiceInfo->m_wServiceID[]=%x,%x,%x,%x\n",
			pServiceInfo->m_wEcmPid,
			pServiceInfo->m_byServiceNum,
			pServiceInfo->m_wServiceID[0],
			pServiceInfo->m_wServiceID[1],
			pServiceInfo->m_wServiceID[2],
			pServiceInfo->m_wServiceID[3]);
	}
	else
	{
		printf("pServiceInfo=NULL\n");
	}
	CDCASTB_SetEcmPid(byType, pServiceInfo);
}
void app_cd_cas_30_filter_timeout_notify(handle_t Filter, const uint8_t* Section, size_t Size)
{
	uint8_t filterId;
	uint16_t	pid;
	ca_filter_t filter = {0};

	for (filterId= 0; filterId< MAX_FILTER_COUNT; filterId++)
	{
		app_porting_ca_demux_get_by_filter_id(filterId,&filter);
		if (( 0 != filter.usedStatus)&&(0 != filter.handle)
			&&(0 != filter.channelhandle)&&(0x80 == (0x80&filter.byReqID)))
		{		
			if (Filter == filter.handle)
			{
				pid = filter.pid;
				CAS_Dbg("wait time [%d]s pid[0x%x]\n",filter.nWaitSeconds,pid);
				CAS_DUMP("filter timeout", filter.match, 6);
				/* 私有数据接收回调 */
				cd_cas_30_private_data_got( 
				filter.byReqID,
				CDCA_TRUE,
				pid,
				(const CDCA_U8 *)NULL,
				0); 				
			}												
		}
	}


   return;	
}


void app_cd_cas_30_filter_notify(handle_t Filter, const uint8_t* Section, size_t Size)
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

		/*永新视博CA要求，不大于16的数据包丢弃*/
		if ((section_length<=16)&&(0x80!=(0x80&byReqID)))
		{
			CA_FAIL(" section_length =%d\n",section_length);
			return;
		}


		/*过滤一次模式，分析完数据后释放filter*/
		if (0x80 == (0x80&byReqID))
		{
			CDSTBCA_ReleasePrivateDataFilter(byReqID,pid);
		}
		
		//CAS_Dbg("tableid = 0x%x byReqID=0x%x\n",data[0],byReqID);
		//if(data[0] == 0x80 || data[0] == 0x81)
		//	CAS_DUMP("DATA", data, 8);
        
		cd_cas_30_private_data_got(         byReqID,
							  	CDCA_FALSE,
								pid,
								data,
								section_length);

		/*过滤一次模式，分析完数据后释放filter*/
		if (0x80 == (0x80&byReqID))
		{
			return;
		}
            data += section_length;
            len -= section_length;
        }
        return;

		
}


uint8_t	app_cd_cas30_api_release_ecm_filter(void)
{
	/*调用CDCASTB_SetEcmPid(CDCA_LIST_FIRST, NULL)，并未清除ECM的FILTER，导致频繁切台出现
	* 加扰节目黑屏，无法播放现象
	*  由应用强制释放之前频道的ECM-FILTER
	*/
	int32_t filterId =0;
	ca_filter_t filter = {0};
	play_scramble_para_t playpara = {0};
	app_play_get_playing_para(&playpara);


	for (filterId = 0; filterId<MAX_FILTER_COUNT;filterId++)
	{
		app_porting_ca_demux_get_by_filter_id(filterId,&filter);
		if (( 0 != filter.usedStatus)&&(0 != filter.handle)
			&&(0x10 != filter.pid)&&(0!=filter.channelhandle)
			&&(0x80 == (filter.byReqID&0x80)))
		{
			CDSTBCA_ReleasePrivateDataFilter(filter.byReqID,filter.pid);
		}

	}

	app_cd_cas_30_set_ecm_pid(CDCA_LIST_FIRST, NULL);
	app_cd_cas_30_set_ecm_pid(CDCA_LIST_OK, NULL);

	playpara.p_ecm_pid= PSI_INVALID_PID;
	playpara.p_ecm_pid_video= PSI_INVALID_PID;
	playpara.p_ecm_pid_audio= PSI_INVALID_PID;
	app_play_set_playing_para(&playpara);


	return 1;
	
}

uint8_t app_cd_cas30_api_start_ecm_filter(play_scramble_para_t *pplayPara)
{
	SCDCASServiceInfo 		CDCAServiceInfo={0};
	play_scramble_para_t playPara = {0};
	CAS_Dbg("app_cdcas30_api_start_ecm_filter \n");
	memcpy(&playPara,pplayPara,sizeof(play_scramble_para_t));

	if (NULL == pplayPara)
	{
		return 0;
	}

	app_cd_cas_30_set_ecm_pid(CDCA_LIST_FIRST, NULL);

	CDCAServiceInfo.m_byServiceNum = 1;
	CDCAServiceInfo.m_wServiceID[0] = pplayPara->program_num;
	if (0x1FFF != playPara.p_ecm_pid)
	{
		CAS_Dbg("DVTCASTB_SetCurEcmInfo old_p_audio_pid=0x%x old_p_ecm_pid =0x%x\n",pplayPara->p_audio_pid,pplayPara->p_ecm_pid);
		CAS_Dbg("DVTCASTB_SetCurEcmInfo old_p_video_pid=0x%x old_p_ecm_pid =0x%x\n",pplayPara->p_video_pid,pplayPara->p_ecm_pid);

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

		CDCAServiceInfo.m_wEcmPid = playPara.p_ecm_pid;
		app_cd_cas_30_set_ecm_pid(CDCA_LIST_ADD, &CDCAServiceInfo);
	}

	if (0x1FFF != playPara.p_ecm_pid_audio)
	{
		CAS_Dbg("DVTCASTB_SetCurEcmInfo old_p_audio_pid=0x%x old_p_ecm_pid_video =0x%x\n",pplayPara->p_audio_pid,pplayPara->p_ecm_pid_audio);


		if (0 == app_porting_ca_descramble_set_pid(1,pplayPara->p_audio_pid ))
		{
			CA_FAIL(" Failed\n");
			return 0;
		}

		CDCAServiceInfo.m_wEcmPid = playPara.p_ecm_pid_audio;
		app_cd_cas_30_set_ecm_pid(CDCA_LIST_ADD, &CDCAServiceInfo);
	}


	if (0x1FFF != playPara.p_ecm_pid_video)
	{
		CAS_Dbg("DVTCASTB_SetCurEcmInfo old_p_video_pid=0x%x old_p_ecm_pid_video =0x%x\n",pplayPara->p_video_pid,pplayPara->p_ecm_pid_video);


		if (0 == app_porting_ca_descramble_set_pid(0,pplayPara->p_video_pid ))
		{
			CA_FAIL(" Failed\n");
			return 0;
		}

		CDCAServiceInfo.m_wEcmPid = playPara.p_ecm_pid_video;
		app_cd_cas_30_set_ecm_pid(CDCA_LIST_ADD, &CDCAServiceInfo);
	}


	app_cd_cas_30_set_ecm_pid(CDCA_LIST_OK, NULL);
	CAS_Dbg("GxCaSetEcmPidByRecordPid End\n");

	return 1;
}

uint8_t app_cd_cas30_api_release_emm_filter(void)
{
	CAS_Dbg("CDCASTB_SetEmmPid wEmmPid=0\n");
	CDCASTB_SetEmmPid(0);
	return 0;
	
}

uint8_t app_cd_cas30_api_start_emm_filter(uint16_t emm_pid)
{
	CAS_Dbg("CDCASTB_SetEmmPid wEmmPid=0\n");
	CDCASTB_SetEmmPid(0);
	CAS_Dbg("CDCASTB_SetEmmPid wEmmPid=0x%x\n",emm_pid);
	CDCASTB_SetEmmPid(emm_pid);
	return 0;
}

bool app_cd_cas30_api_check_cat_ca_descriptor_valid(uint8_t *sectionData,uint32_t CA_system_id)
{
	return CDCASTB_IsCDCa(CA_system_id);
}

bool app_cd_cas30_api_check_pmt_ca_descriptor_valid(uint8_t *sectionData,uint32_t CA_system_id)
{
	return CDCASTB_IsCDCa(CA_system_id);
}







