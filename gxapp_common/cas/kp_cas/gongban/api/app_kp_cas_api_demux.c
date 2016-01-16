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
1.0  	2014.12.17		  chenth 	 			creation
*****************************************************************************/
#include "app_common_play.h"
#include "app_common_porting_stb_api.h"
#include "app_common_porting_ca_smart.h"
#include "app_common_porting_ca_demux.h"
#include "app_common_porting_ca_descramble.h"
#include "app_kp_cas_api_demux.h"
#include "KPAPP.h"
#include "kp_cas.h"
#include "gx_demux.h"
#include "gx_descrambler.h"


static uint8_t emm_filter_flag = FALSE;

//过滤器超时回调
//重新设置过滤器
void app_kp_cas_filter_timeout_notify(handle_t Filter, const uint8_t* Section, size_t Size)
{
	uint8_t filterId;
	uint16_t	pid;
	ca_filter_t filter = {0};
	play_scramble_para_t playpara = {0};
	app_play_get_playing_para(&playpara);

	for (filterId= 0; filterId< MAX_FILTER_COUNT; filterId++)
	{
		app_porting_ca_demux_get_by_filter_id(filterId,&filter);
		if (( 0 != filter.usedStatus)&&(0 != filter.handle)&&(0 != filter.channelhandle))
		{		
			if (Filter == filter.handle)
			{
				pid = filter.pid;

				if ((pid == playpara.p_emm_pid)&&(0x1fff != playpara.p_emm_pid))	
				{
                    CAS_Dbg("####app_kp_cas_filter_timeout_notify,emm:%d,TableId:%#x\n",pid,filter.byReqID);
                    KPCAAPP_StopPid(playpara.p_emm_pid);
                    KPCAAPP_StartEMMPid(playpara.p_emm_pid);
				} 				
			}		
		}
	}


   return;	
}
//ecm,emm数据处理
//过滤器数据回调处理接口
void app_kp_cas_filter_notify(handle_t Filter, const uint8_t* Section, size_t Size)
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
    U32 queueid;
	play_scramble_para_t playpara = {0};
	app_play_get_playing_para(&playpara);

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
                            //CAS_Dbg("########################\n");
                            //CAS_DUMP("[mask] ->", sfilter.mask, sfilter.filterLen);
                            //CAS_DUMP("[match] ->", sfilter.match, sfilter.filterLen);
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

			if (filter_list_search(sfilter.pid,sfilter.byReqID,&queueid)>0)
			{
                //KP_Dbg("####pid:%d,tableid:%#x,queueid:%d\n",sfilter.pid,sfilter.byReqID,queueid);
                //KP_DUMP("[data] ->", data, section_length);
                /*if(data[0]== 0x50 || data[0] == 0x53 )
                {
                    KP_DUMP("[data] ->", data, section_length);
                }*/
				KPAPPCA_QueueSend(queueid,(U8 *)data);
                if(KP_ECM_TABLEID == sfilter.byReqID)
                {
				    return;
                }
			}
            else
            {
                KP_Dbg("not find,####pid:%d,tableid:%#x,queueid:%d\n",sfilter.pid,sfilter.byReqID,queueid);
            }

            
            data += section_length;
            len -= section_length;
		}
		
        return;

		
}

//切台前调用
uint8_t	app_kp_cas_api_release_ecm_filter(void)
{
	int32_t filterId =0;
	ca_filter_t filter = {0};
	play_scramble_para_t playpara = {0};
	app_play_get_playing_para(&playpara);

    CAS_Dbg("app_kp_cas_api_release_ecm_filter \n");

	for (filterId = 0; filterId<MAX_FILTER_COUNT;filterId++)
	{
		app_porting_ca_demux_get_by_filter_id(filterId,&filter);
		if (( 0 != filter.usedStatus)&&(0 != filter.handle)
			&&(0x10 != filter.pid)&&(0!=filter.channelhandle)
			&&(KP_ECM_TABLEID == filter.byReqID))
			{
				//KPCA_ReleasePrivateDataFilter(KP_ECM_TABLEID,filter.pid);
                KPCAAPP_StopPid(filter.pid);
			}

	}

	playpara.p_ecm_pid= PSI_INVALID_PID;
	playpara.p_ecm_pid_video= PSI_INVALID_PID;
	playpara.p_ecm_pid_audio= PSI_INVALID_PID;
	app_play_set_playing_para(&playpara);
    CAS_Dbg("app_kp_cas_api_release_ecm_filter ok.\n");
	return 1;
	
}

//----ecm过滤器参数设置
/*0设置为视频，1设置为音频*/
uint8_t app_kp_cas_api_start_ecm_filter(play_scramble_para_t *pplayPara)
{
	play_scramble_para_t playPara = {0};

	CAS_Dbg("app_kp_cas_api_start_ecm_filter \n");

	if (NULL == pplayPara)
	{
		return 0;
	}
    memcpy(&playPara,pplayPara,sizeof(play_scramble_para_t));

	if (0x1FFF != playPara.p_ecm_pid)
	{
			CAS_Dbg("KPCAAPP_StartECMPid old_p_audio_pid=%d old_p_ecm_pid =%d\n",pplayPara->p_audio_pid,pplayPara->p_ecm_pid);
			CAS_Dbg("KPCAAPP_StartECMPid old_p_video_pid=%d old_p_ecm_pid =%d\n",pplayPara->p_video_pid,pplayPara->p_ecm_pid);

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

			KPCAAPP_StartECMPid(pplayPara->p_ecm_pid);
	}

	if (0x1FFF != playPara.p_ecm_pid_audio)
	{
			CAS_Dbg("KPCAAPP_StartECMPid old_p_audio_pid=%d p_ecm_pid_audio =%d\n",pplayPara->p_audio_pid,pplayPara->p_ecm_pid_audio);

			if (0 == app_porting_ca_descramble_set_pid(1,pplayPara->p_audio_pid ))
			{
				CA_FAIL(" Failed\n");
				return 0;
			}

			KPCAAPP_StartECMPid(pplayPara->p_ecm_pid_audio);
	}


	if (0x1FFF != playPara.p_ecm_pid_video)
	{
			CAS_Dbg("KPCAAPP_StartECMPid old_p_video_pid=%d old_p_ecm_pid_video =%d\n",pplayPara->p_video_pid,pplayPara->p_ecm_pid_video);

			if (0 == app_porting_ca_descramble_set_pid(0,pplayPara->p_video_pid ))
			{
				CA_FAIL(" Failed\n");
				return 0;
			}
			
			KPCAAPP_StartECMPid(pplayPara->p_ecm_pid_video);

	}
    CAS_Dbg("app_kp_cas_api_start_ecm_filter ok.\n");
	return 1;
}

uint8_t app_kp_cas_api_release_emm_filter(void)
{
	play_scramble_para_t playpara = {0};
	app_play_get_playing_para(&playpara);
	CAS_Dbg("app_kp_cas_api_release_emm_filter, emm_pid:%d\n",playpara.p_emm_pid);
    CAS_Dbg("[----\n");
    KPCAAPP_StopPid(playpara.p_emm_pid);
    CAS_Dbg("----]\n");
	playpara.p_emm_pid = 0x1fff;
	app_play_set_playing_para(&playpara);
	emm_filter_flag = FALSE;
	return 0;
}

//智能卡出入时需要复位过滤器
uint8_t app_kp_cas_api_reset_emm_filter(void)
{
	play_scramble_para_t playpara = {0};
	app_play_get_playing_para(&playpara);

    CAS_Dbg("app_kp_cas_api_reset_emm_filter \n");
	if ((0x1fff != playpara.p_emm_pid)&&(FALSE == emm_filter_flag))
		app_kp_cas_api_start_emm_filter(playpara.p_emm_pid);
	return 0;	
}

//启动设置EMM过滤器
uint8_t app_kp_cas_api_start_emm_filter(uint16_t emm_pid)
{
    CAS_Dbg("app_kp_cas_api_start_emm_filter,emm_pid:%d\n",emm_pid);
    CAS_Dbg("[----\n");
    KPCAAPP_StartEMMPid(emm_pid);
    CAS_Dbg("----]\n");
    emm_filter_flag = TRUE;
	return 0;
}

//CA cat表校验
bool app_kp_cas_api_check_cat_ca_descriptor_valid(uint8_t *sectionData,uint32_t CA_system_id)
{
    KP_Dbg("KP CAT check:%x\n",CA_system_id);
	if (CA_system_id == KPCAAPP_GetCASID())
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}
//CA pmt表校验
bool app_kp_cas_api_check_pmt_ca_descriptor_valid(uint8_t *sectionData,uint32_t CA_system_id)
{
    KP_Dbg("KP PMT check:%x\n",CA_system_id);
	if (CA_system_id == KPCAAPP_GetCASID())
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}







