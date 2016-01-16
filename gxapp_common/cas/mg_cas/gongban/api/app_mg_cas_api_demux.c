/*****************************************************************************
* 						   CONFIDENTIAL
*        Hangzhou GuoXin Science and Technology Co., Ltd.
*                      (C)2014, All right reserved
******************************************************************************

******************************************************************************
* File Name :	app_mg_cas_api_demux.c
* Author    : 	zhouhm
* Project   :	goxceed dvbc
* Type      :
******************************************************************************
* Purpose   :	
******************************************************************************
* Release History:
VERSION	Date			  AUTHOR         Description
1.0  	2014.04.11		  zhouhm 	 			creation
*****************************************************************************/
#include "app_common_play.h"
#include "app_common_porting_stb_api.h"
#include "app_common_porting_ca_smart.h"
#include "app_common_porting_ca_demux.h"
#include "app_common_porting_ca_descramble.h"
#include "app_mg_cas_api_demux.h"
#include "mgdef.h"
#include "gx_demux.h"
#include "gx_descrambler.h"


/*解扰器初始化打开一次，0设置为视频，1设置为音频*/
static uint8_t emm_filter_flag = FALSE;


extern void MGCA_SetEmmFilterG(uint16_t emm_pid);
extern 	void MGCA_SetEmmFilterU(uint16_t emm_pid);
extern void MGCA_SetEmmFilterA(uint16_t emm_pid);
extern 	void MGCA_EcmDataGot(MG_U16 wEcmPID,MG_U8 *MG_ECM);
extern 	void MGCA_EmmDataGot(MG_U16 wEcmPID,MG_U8 *MG_EMM);
extern void MGCA_ReleasePrivateDataFilter( MG_U8  byReqID,
                                              MG_U16 wPid );
extern 	uint8_t MGCA_SetEcmFilter(MG_U16	   wPid);

//过滤器超时回调
//重新设置过滤器
void app_mg_cas_filter_timeout_notify(handle_t Filter, const uint8_t* Section, size_t Size)
{
	uint8_t filterId;
	uint16_t	pid;
	ca_filter_t filter = {0};
	play_scramble_para_t playpara = {0};
	app_play_get_playing_para(&playpara);

	for (filterId= 0; filterId< MAX_FILTER_COUNT; filterId++)
	{
		app_porting_ca_demux_get_by_filter_id(filterId,&filter);
		if (( 0 != filter.usedStatus)&&(0 != filter.handle)
			&&(0 != filter.channelhandle)&&(0x80 == (0x80&filter.byReqID)))
			{		
				if (Filter == filter.handle)
				{
						pid = filter.pid;

						if ((pid == playpara.p_emm_pid)&&(0x1fff != playpara.p_emm_pid)&&
                             ((EMM_TABLEID_U ==filter.byReqID)||
						     (EMM_TABLEID_G == filter.byReqID)||
						     (EMM_TABLEID_A == filter.byReqID)))	
						{
								if(MG_Get_Card_Ready()==MG_TRUE)
								{
									if (EMM_TABLEID_U == filter.byReqID)
									{
										MGCA_SetEmmFilterU(pid);
									}
									if (EMM_TABLEID_G == filter.byReqID)
									{
										MGCA_SetEmmFilterG(pid);
									}
									if (EMM_TABLEID_A == filter.byReqID)
									{
										MGCA_SetEmmFilterA(pid);
									}
								}
						} 				
				}												
			}
	}


   return;	
}
//ecm,emm数据处理
//过滤器数据回调处理接口
void app_mg_cas_filter_notify(handle_t Filter, const uint8_t* Section, size_t Size)
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
	play_scramble_para_t playpara = {0};
	app_play_get_playing_para(&playpara);

//    ASSERT(Section != NULL);
    ret = GxDemux_FilterGetPID(Filter,&pid);

	 while(len > 0) 
     {
	       bFlag = FALSE;
	       section_length = ((data[1] & 0x0F) << 8) + data[2] + 3;
			for (filterID = 0; filterID< MAX_FILTER_COUNT; filterID++)
			{//遍历所有过滤器
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
								{//条件匹配
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

			if (FALSE == bFlag)//数据匹配成功
			{
				CA_FAIL(" FALSE == bFlag\n");
				return;
			}

//			printf("byReqID = 0x%x\n",byReqID);

			if (ECM_TABLEID_80 == byReqID)
			{
				/*
				* ecm数据处理
				*/
				MGCA_EcmDataGot(pid,data);
				return;
			}
			else //emm 数据
			{
			    if ((EMM_TABLEID_U ==byReqID)||
    				(EMM_TABLEID_G == byReqID)||
    				(EMM_TABLEID_A == byReqID))
				{
					if(MG_Get_Card_Ready() != MG_TRUE)
					{
						return ;
					}
					else
					{
						MGCA_EmmDataGot(pid,data);
//									break;
					}
					
				}

			}

            
            data += section_length;
            len -= section_length;
		}

		
		if (TRUE == bFlag)
		{//重设过滤器
			if ((pid == playpara.p_emm_pid)&&(0x1fff != playpara.p_emm_pid)&&
               ((EMM_TABLEID_U ==byReqID)||
			    (EMM_TABLEID_G == byReqID)||
			    (EMM_TABLEID_A == byReqID)))	
			{
					if(MG_Get_Card_Ready() == MG_TRUE)
					{
						if (EMM_TABLEID_U == byReqID)
						{
							MGCA_SetEmmFilterU(pid);
						}
						if (EMM_TABLEID_G == byReqID)
						{
							MGCA_SetEmmFilterG(pid);
						}
						if (EMM_TABLEID_A == byReqID)
						{
							MGCA_SetEmmFilterA(pid);
						}

					}
			}
		}
        return;

		
}

//切台前调用
uint8_t	app_mg_cas_api_release_ecm_filter(void)
{
	int32_t filterId =0;
	ca_filter_t filter = {0};
	play_scramble_para_t playpara = {0};
	app_play_get_playing_para(&playpara);

#ifdef _DEBUG
    CAS_Dbg("app_mg_cas_api_release_ecm_filter \n");
#endif
	for (filterId = 0; filterId<MAX_FILTER_COUNT;filterId++)
	{
		app_porting_ca_demux_get_by_filter_id(filterId,&filter);
		if (( 0 != filter.usedStatus)&&(0 != filter.handle)
			&&(0x10 != filter.pid)&&(0!=filter.channelhandle)
			&&(ECM_TABLEID_80 == filter.byReqID))
			{
				MGCA_ReleasePrivateDataFilter(ECM_TABLEID_80,filter.pid);
			}

	}

    //节目切换,清除上一个节目的信息
	MG_Clean_LastPNo_Info();

	playpara.p_ecm_pid= PSI_INVALID_PID;
	playpara.p_ecm_pid_video= PSI_INVALID_PID;
	playpara.p_ecm_pid_audio= PSI_INVALID_PID;
	app_play_set_playing_para(&playpara);

	return 1;
	
}

//----ecm过滤器参数设置
/*0设置为视频，1设置为音频*/
uint8_t app_mg_cas_api_start_ecm_filter(play_scramble_para_t *pplayPara)
{
	play_scramble_para_t playPara = {0};
#ifdef _DEBUG
	CAS_Dbg("app_mg_cas_api_start_ecm_filter \n");
#endif
	if (NULL == pplayPara)
	{
		return 0;
	}
    memcpy(&playPara,pplayPara,sizeof(play_scramble_para_t));

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

			MGCA_SetEcmFilter(pplayPara->p_ecm_pid);
	}

	if (0x1FFF != playPara.p_ecm_pid_audio)
	{
			CAS_Dbg("DVTCASTB_SetCurEcmInfo old_p_audio_pid=0x%x old_p_ecm_pid_video =0x%x\n",pplayPara->p_audio_pid,pplayPara->p_ecm_pid_audio);

			if (0 == app_porting_ca_descramble_set_pid(1,pplayPara->p_audio_pid ))
			{
				CA_FAIL(" Failed\n");
				return 0;
			}

			MGCA_SetEcmFilter(pplayPara->p_ecm_pid_audio);
	}


	if (0x1FFF != playPara.p_ecm_pid_video)
	{
			CAS_Dbg("DVTCASTB_SetCurEcmInfo old_p_video_pid=0x%x old_p_ecm_pid_video =0x%x\n",pplayPara->p_video_pid,pplayPara->p_ecm_pid_video);

			if (0 == app_porting_ca_descramble_set_pid(0,pplayPara->p_video_pid ))
			{
				CA_FAIL(" Failed\n");
				return 0;
			}
			
			MGCA_SetEcmFilter(pplayPara->p_ecm_pid_video);

	}

	return 1;
}

uint8_t app_mg_cas_api_release_emm_filter(void)
{
	play_scramble_para_t playpara = {0};
	app_play_get_playing_para(&playpara);

#ifdef _DEBUG
	CAS_Dbg("app_mg_cas_api_release_emm_filter \n");
#endif
	MGCA_ReleasePrivateDataFilter(EMM_TABLEID_U,playpara.p_emm_pid);
	MGCA_ReleasePrivateDataFilter(EMM_TABLEID_G,playpara.p_emm_pid);
	MGCA_ReleasePrivateDataFilter(EMM_TABLEID_A,playpara.p_emm_pid);
	playpara.p_emm_pid = 0x1fff;
	app_play_set_playing_para(&playpara);
	
	emm_filter_flag = FALSE;
	return 0;
	
}
//智能卡出入时需要复位过滤器
uint8_t app_mg_cas_api_reset_emm_filter(void)
{
	play_scramble_para_t playpara = {0};
	app_play_get_playing_para(&playpara);

#ifdef _DEBUG
    CAS_Dbg("app_mg_cas_api_reset_emm_filter \n");
#endif
	if ((0x1fff != playpara.p_emm_pid)&&(FALSE == emm_filter_flag))
		app_mg_cas_api_start_emm_filter(playpara.p_emm_pid);
	return 0;	
}

//启动设置EMM过滤器
uint8_t app_mg_cas_api_start_emm_filter(uint16_t emm_pid)
{
	printf("emm_pid = 0x%x\n",emm_pid);
#ifdef _DEBUG
	CAS_Dbg("app_mg_cas_api_start_emm_filter.\n");
#endif
	if (MG_Get_Card_Ready() == 1)
 	{
		 MGCA_SetEmmFilterU(emm_pid);
		 MGCA_SetEmmFilterG(emm_pid);
		 MGCA_SetEmmFilterA(emm_pid);
		 emm_filter_flag = TRUE;
 	}
	 else
 	{
	 	emm_filter_flag = FALSE;
 	}

	return 0;
}

//CA cat表校验
bool app_mg_cas_api_check_cat_ca_descriptor_valid(uint8_t *sectionData,uint32_t CA_system_id)
{
    //printf("MG CAT check:%x\n",CA_system_id);
	if (CA_system_id == 0x4a40)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}
//CA pmt表校验
bool app_mg_cas_api_check_pmt_ca_descriptor_valid(uint8_t *sectionData,uint32_t CA_system_id)
{
    //printf("MG PMT check:%x\n",CA_system_id);
	if (CA_system_id == 0x4a40)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}







