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
#include "desai_ca_api.h"
#include "gx_demux.h"
#include "gx_descrambler.h"

void app_desai_cas50_filter_timeout_notify(handle_t Filter, const uint8_t* Section, size_t Size)
{
		
	uint8_t	filterId;
	int16_t	pid;
	ca_filter_t filter = {0};

	int32_t ret;

    	//ASSERT(Section != NULL);
   	ret = GxDemux_FilterGetPID(Filter,&pid);

	for (filterId= 0; filterId< MAX_FILTER_COUNT; filterId++)
	{
		app_porting_ca_demux_get_by_filter_id(filterId,&filter);
		if (( 0 != filter.usedStatus)&&(0 != filter.handle)
			&&(0 != filter.channelhandle)&&(Filter == filter.handle)&&(pid == filter.pid))
			{	
			//	pid = filter->pid;
				if((filter.byReqID == 1)||(filter.byReqID == 2))
				{
					//CAS_Dbg("app_desai_cas50_filter_timeout_notify filterId=%d	pid = %d filter.byReqID = %d\n",
					//	filterId,pid,filter->byReqID);
				}
				DSCA_GetPrivateData(pid,filter.byReqID, NULL, 0);			
			}
	}
	return;

}

void app_desai_cas50_filter_notify(handle_t Filter, const uint8_t* Section, size_t Size)
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
	int j=0;

    	//ASSERT(Section != NULL);
   	ret = GxDemux_FilterGetPID(Filter,&pid);

    	

       while(len > 0) 
	{
		if(app_porting_ca_section_filter_crc32_check(data))
		{
			break;
		}
            	bFlag = FALSE;
            	section_length = ((data[1] & 0x0F) << 8) + data[2] + 3;
		if(j == 0)
		{
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
										//CAS_Dbg("data[%d]=0x%x cafilter[filterID].match[%d]=0x%x \n",
										//	i,data[i],i,sfilter->match[i]);
										return;
									}
							}

							//sfilter->nms = 0;
							byReqID = sfilter.byReqID&0x7f;
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

			//GxDemux_FilterDisable(Filter);
			//if((byReqID == 1))
			//{
				//CAS_Dbg("*********DSCA_GetPrivateData pid=0x%x byReqID=%d,len=%d**********\n",pid,byReqID,section_length);
				//CAS_DUMP("ecm===",data,10);
			//}
			//if((byReqID == 2))
			//{
				//CAS_Dbg("*********DSCA_GetPrivateData pid=0x%x byReqID=%d,len=%d**********\n",pid,byReqID,section_length);
				//CAS_DUMP("emm===",data,10);
			//}
			DSCA_GetPrivateData(pid,byReqID,data,section_length);
		}
		j ++;
             data += section_length;
	      	len -= section_length;
	 }
        return;

		
}


uint8_t	app_desai_cas50_api_release_ecm_filter(void)
{
	ca_filter_t filter = {0};
	ca_filter_t* sfilter = NULL;

	int32_t filterId =0;
	play_scramble_para_t playpara = {0};
	app_play_get_playing_para(&playpara);
	
	CAS_Dbg("app_desai_cas50_api_release_ecm_filter\n");
	for (filterId = 0; filterId<MAX_FILTER_COUNT;filterId++)
	{
		sfilter = app_porting_ca_demux_get_by_filter_id(filterId,&filter);
		if(!sfilter)
		{
			continue;
		}
		if (( 0 != filter.usedStatus)&&(0 != filter.handle)&&
			(0!=filter.channelhandle))
		{
			if ((filter.pid == playpara.p_ecm_pid)||(filter.pid == playpara.p_ecm_pid_video)
				||(filter.pid == playpara.p_ecm_pid_audio))
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

	return 1;
	
}


uint8_t app_desai_cas50_api_start_ecm_filter(play_scramble_para_t *pplayPara)
{
	play_scramble_para_t playPara = {0};
	
	CAS_Dbg("app_desai_cas50_api_start_ecm_filter \n");
	memcpy(&playPara,pplayPara,sizeof(play_scramble_para_t));
	if (NULL == pplayPara)
	{
		return 0;
	}

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

			DSCA_SetEcmPid(pplayPara->p_ecm_pid);
	}

	if (0x1FFF != playPara.p_ecm_pid_audio)
	{
			CAS_Dbg("DVTCASTB_SetCurEcmInfo old_p_audio_pid=0x%x old_p_ecm_pid_video =0x%x\n",pplayPara->p_audio_pid,pplayPara->p_ecm_pid_audio);

			if (0 == app_porting_ca_descramble_set_pid(1,pplayPara->p_audio_pid ))
			{
				CA_FAIL(" Failed\n");
				return 0;
			}

			DSCA_SetEcmPid(pplayPara->p_ecm_pid_audio);
	}


	if (0x1FFF != playPara.p_ecm_pid_video)
	{
			CAS_Dbg("DVTCASTB_SetCurEcmInfo old_p_video_pid=0x%x old_p_ecm_pid_video =0x%x\n",pplayPara->p_video_pid,pplayPara->p_ecm_pid_video);


			if (0 == app_porting_ca_descramble_set_pid(0,pplayPara->p_video_pid ))
			{
				CA_FAIL(" Failed\n");
				return 0;
			}

			DSCA_SetEcmPid(pplayPara->p_ecm_pid_video);
	}

	if ((0x1FFF == playPara.p_ecm_pid_video)&&(0x1FFF == playPara.p_ecm_pid_audio)&&(0x1FFF == playPara.p_ecm_pid))
	{
		CAS_Dbg("========================DSCA_SetEcmPid    0x1FFFF=============================\n");
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
		DSCA_SetEcmPid(0x1FFF);
	}

	CAS_Dbg("app_desai_cas50_api_start_ecm_filter End\n");

	return 1;
}

uint8_t app_desai_cas50_api_release_emm_filter(void)
{
	
#if 0
    ca_filter_t* filter = NULL;
	int32_t filterId =0;
	CAS_Dbg("app_desai_cas50_api_release_emm_filter\n");
	for (filterId = 0; filterId<MAX_FILTER_COUNT;filterId++)
	{
		filter = app_porting_ca_demux_get_by_filter_id(filterId);
		if(!filter)
		{
			continue;
		}
		if (( 0 != filter->usedStatus)&&(0 != filter->handle)&&
			(0!=filter->channelhandle) && (g_emm_pid == filter->pid))
		{
				app_porting_ca_demux_release_filter( filterId, TRUE);
		}

	}
	g_emm_pid = 0x1fff;
#endif
	play_scramble_para_t playpara = {0};
	app_play_get_playing_para(&playpara);
	playpara.p_emm_pid = 0x1fff;
	app_play_set_playing_para(&playpara);

	return 1;
}


uint8_t app_desai_cas50_api_start_emm_filter(uint16_t emm_pid)
{
	CAS_Dbg("%s	emm_pid=%d\n",__FUNCTION__,emm_pid);
	DSCA_SetEmmPid(emm_pid);
	return 1;
}

bool app_desai_cas50_api_check_cat_ca_descriptor_valid(uint8_t *sectionData,uint32_t CA_system_id)
{
	return DSCA_IsDSCa(CA_system_id);
}

bool app_desai_cas50_api_check_pmt_ca_descriptor_valid(uint8_t *sectionData,uint32_t CA_system_id)
{
	return DSCA_IsDSCa(CA_system_id);
}

void app_desai_cas50_api_nit_zone_des(uint8_t* databuf)
{
	CAS_Dbg("%s\n",__FUNCTION__);
	UINT8 * data = databuf;
	UINT8 descriptor_length = 0; 
	UINT8 aucAddrCode[7];
	descriptor_length = data[1];
	data = data + 2;
	if (descriptor_length == 7)
	{
		memcpy(aucAddrCode, &data[0], sizeof(aucAddrCode));
		DSCA_SetCardAddressCode(aucAddrCode);
		//DUMP("GET AddCode",aucAddrCode,7);
	}

}





