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
#include "app_kn_cas_api_demux.h"
#include "app_common_porting_ca_smart.h"
#include "app_common_porting_ca_demux.h"
#include "app_common_porting_ca_descramble.h"
#include "gx_demux.h"
#include "gx_descrambler.h"
#include "cas_hcn_Public.h"

void app_kn_cas_filter_notify(handle_t Filter, const uint8_t* Section, size_t Size)
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
            //数据处理
			hcncas_get_section( pid,(uint8_t*)Section);
            data += section_length;
            len -= section_length;
	    }
        return;

		
}
void app_kn_cas_filter_timeout_notify(handle_t Filter, const uint8_t* Section, size_t Size)
{
	uint8_t filterId;
	uint16_t	pid;
	ca_filter_t filter = {0};

    //play_scramble_para_t playpara = {0};
	//app_play_get_playing_para(&playpara);

	for (filterId= 0; filterId< MAX_FILTER_COUNT; filterId++)
	{
		app_porting_ca_demux_get_by_filter_id(filterId,&filter);
		if (( 0 != filter.usedStatus)&&(0 != filter.handle)
			&&(0 != filter.channelhandle))
		{
		
			if (Filter == filter.handle)
			{
				pid = filter.pid;

				//处理emm超时
				if (//(pid == playpara.p_emm_pid)&&
				    hcn_emm82_fileterid() == filter.byReqID
				    || hcn_emm83_fileterid() == filter.byReqID)	
				{ 
                        if (hcn_emm82_fileterid() == filter.byReqID)
            			{
            				CAS_Dbg("hcn_emm82 timeout\n");
                            hcn_emm82_Timeout();
			                
            			}
                        if (hcn_emm83_fileterid() == filter.byReqID)
            			{
            				CAS_Dbg("hcn_emm83 timeout\n");
                            hcn_emm83_Timeout();
            			}
                        //CAS_Dbg("app_kn_cas_filter_timeout_notify timeout\n");
				}	 
				
			}												
		}
	}


   return;	
}


uint8_t	app_kn_cas_api_release_ecm_filter(void)
{
	play_scramble_para_t playpara = {0};
	app_play_get_playing_para(&playpara);

	/*
	*如果当前频道为不加扰的频道，则需要将 byCount = 0或 pServiceInfos = NULL，可以停止ECM数据
	*/

	//BYCASTB_SetCurEcmInfos(0,BNULL);
	CAS_Dbg("app_kn_cas_api_release_ecm_filter \n");
	
	hcncas_ecm_stop();//让CA库释放。
	hcncas_clean_ecmpid();
	playpara.p_ecm_pid= PSI_INVALID_PID;
	playpara.p_ecm_pid_video= PSI_INVALID_PID;
	playpara.p_ecm_pid_audio= PSI_INVALID_PID;
	app_play_set_playing_para(&playpara);


	return 1;
	
}

uint8_t app_kn_cas_api_start_ecm_filter(play_scramble_para_t *pplayPara)
{
	
	play_scramble_para_t playPara = {0};
	CAS_Dbg("app_kn_cas_api_start_ecm_filter \n");
	memcpy(&playPara,pplayPara,sizeof(play_scramble_para_t));

	if (NULL == pplayPara)
	{
		return 0;
	}

//	app_by_cas_api_release_ecm_filter();

	if ((0x1FFF ==playPara.p_ecm_pid )&&(0x1FFF == playPara.p_ecm_pid_audio)&&(0x1FFF == playPara.p_ecm_pid_video))
	{
			/*清流节目*/
//			BYCASTB_SetCurEcmInfos(0,&pServiceInfos/*BNULL*/);

			return 0;
	}
	if (0x1FFF != playPara.p_ecm_pid)
	{
		CAS_Dbg("app_kn_cas_api_start_ecm_filter old_p_audio_pid=0x%x old_p_ecm_pid =0x%x\n",pplayPara->p_audio_pid,pplayPara->p_ecm_pid);
		CAS_Dbg("app_kn_cas_api_start_ecm_filter old_p_video_pid=0x%x old_p_ecm_pid =0x%x\n",pplayPara->p_video_pid,pplayPara->p_ecm_pid);

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
		
		hcncas_set_ecmpid(playPara.p_ecm_pid);
	}

	if (0x1FFF != playPara.p_ecm_pid_audio)
	{
		CAS_Dbg("app_kn_cas_api_start_ecm_filter old_p_video_pid=0x%x old_p_ecm_pid_video =0x%x\n",pplayPara->p_video_pid,pplayPara->p_ecm_pid_video);

		if (0 == app_porting_ca_descramble_set_pid(1,pplayPara->p_audio_pid ))
		{
			CA_FAIL(" Failed\n");
			return 0;
		}

		hcncas_set_ecmpid(playPara.p_ecm_pid_audio);

	}


	if (0x1FFF != playPara.p_ecm_pid_video)
	{
			CAS_Dbg("app_kn_cas_api_start_ecm_filter old_p_video_pid=0x%x old_p_ecm_pid_video =0x%x\n",pplayPara->p_video_pid,pplayPara->p_ecm_pid_video);

			if (0 == app_porting_ca_descramble_set_pid(0,pplayPara->p_video_pid ))
			{
				CA_FAIL(" Failed\n");
				return 0;
			}

			hcncas_set_ecmpid(playPara.p_ecm_pid_video);

	}
	return 1;
}

uint8_t app_kn_cas_api_release_emm_filter(void)
{
	//int32_t filterId =0;
	//ca_filter_t* filter = NULL;
    CAS_Dbg("app_kn_cas_api_release_emm_filter \n");
	hcncas_emm_stop();//让CA库释放。
	hcncas_clean_emmpid();
	return 0;
}

uint8_t app_kn_cas_api_start_emm_filter(uint16_t emm_pid)
{
    CAS_Dbg("app_kn_cas_api_start_emm_filter \n");
	hcncas_set_emmpid(emm_pid);
	return 0;
}

bool app_kn_cas_api_check_cat_ca_descriptor_valid(uint8_t *sectionData,uint32_t CA_system_id)
{
	UINT8 system_id[2];
	system_id[0]=(CA_system_id&0x0000ff00)>>8;
	system_id[1]=(CA_system_id&0x000000ff);
	return hcncas_ismatch(system_id);
}

bool app_kn_cas_api_check_pmt_ca_descriptor_valid(uint8_t *sectionData,uint32_t CA_system_id)
{
	UINT8 system_id[2];
	system_id[0]=(CA_system_id&0x0000ff00)>>8;
	system_id[1]=(CA_system_id&0x000000ff);
	return hcncas_ismatch(system_id);
}


void app_kn_cas_api_nit_zone_des(uint8_t* databuf)
{
	UINT8 netzoneid;
	netzoneid = *databuf;
	hcncas_set_netzoneid(netzoneid);
	return;
}
void app_kn_cas_api_get_remain_day(uint8_t* databuf)
{
	hcncas_get_remain_day(databuf);
	return;
}







