/*****************************************************************************
* 						   CONFIDENTIAL
*        Hangzhou GuoXin Science and Technology Co., Ltd.
*                      (C)2014, All right reserved
******************************************************************************

******************************************************************************
* File Name :	app_gos_cas_api_demux.c
* Author    : 	zhouhm
* Project   :	goxceed dvbc
* Type      :
******************************************************************************
* Purpose   :	
******************************************************************************
* Release History:
VERSION	Date			  AUTHOR         Description
1.0  	2014.04.22		  zhouhm 	 			creation
*****************************************************************************/
#include "app_common_play.h"
#include "app_common_porting_stb_api.h"
#include "app_common_porting_ca_smart.h"
#include "app_common_porting_ca_demux.h"
#include "app_common_porting_ca_descramble.h"
#include "gx_demux.h"
#include "gx_descrambler.h"
#include "GOSCAS_STBDataPublic.h"
#include "GOSCAS_STBInf.h"
#include "GOSSTB_CASInf.h"


extern 	HRESULT GOSSTBCA_ReleasePrivateDataFilter(uint8_t byReqID, uint16_t wPid);
uint16_t app_gos_cas_api_get_emm_pid(void)
{
	play_scramble_para_t playpara = {0};
	app_play_get_playing_para(&playpara);
	return playpara.p_emm_pid;
}

void app_gos_cas_filter_timeout_notify(handle_t Filter, const uint8_t* Section, size_t Size)
{
	uint8_t filterId;
	uint16_t	pid;
	ca_filter_t filter = {0};
	uint8_t byReqID;


	for (filterId= 0; filterId< MAX_FILTER_COUNT; filterId++)
	{
		app_porting_ca_demux_get_by_filter_id(filterId,&filter);
		if (( 0 != filter.usedStatus)&&(0 != filter.handle)
			&&(0 != filter.channelhandle))
			{		
				if (Filter == filter.handle)
				{
					pid = filter.pid;
					byReqID = filter.byReqID;

					GOSCASTB_StreamGuardDataGet(byReqID, 
						false, 
						pid,
						0,
						NULL) ;				
				}												
			}
	}


   return;	
}

//ecm,emm数据处理
void app_gos_cas_filter_notify(handle_t Filter, const uint8_t* Section, size_t Size)
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
    			if (Filter == sfilter.handle)
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

    	if ((byReqID<GOSCA_STB_ECM_SLOT)||(byReqID>GOSCA_STB_EMM_Public_SLOT))
    	{ 
    		CA_FAIL(" byReqID=0x%x \n",byReqID);		
    //			GxCA_DemuxFilterReset(Filter);		
    		return;
    	}

    	GOSCASTB_StreamGuardDataGet(byReqID, TRUE, pid, section_length, data);

        data += section_length;
        len -= section_length;
    }
    return;	
}


uint8_t	app_gos_cas_api_release_ecm_filter(void)
{

	int32_t filterId =0;
	ca_filter_t filter = {0};
	SGOSCAServiceInfo 		GOSServiceInfo={0};
	play_scramble_para_t playpara = {0};
	app_play_get_playing_para(&playpara);



	for (filterId = 0; filterId<MAX_FILTER_COUNT;filterId++)
	{
		app_porting_ca_demux_get_by_filter_id(filterId,&filter);
		if (( 0 != filter.usedStatus)&&(0 != filter.handle)
			&&(0!=filter.channelhandle)
			&&(GOSCA_STB_ECM_SLOT == filter.byReqID))
			{
				GOSSTBCA_ReleasePrivateDataFilter(GOSCA_STB_ECM_SLOT,filter.pid);	
			}
	}

	GOSServiceInfo.m_wEcmPid = PSI_INVALID_PID;
	GOSServiceInfo.m_byServiceCount = 1;

	GOSCASTB_SetCurEcmInfos(1,&GOSServiceInfo);

	playpara.p_ecm_pid= PSI_INVALID_PID;
	playpara.p_ecm_pid_video= PSI_INVALID_PID;
	playpara.p_ecm_pid_audio= PSI_INVALID_PID;
	app_play_set_playing_para(&playpara);


	return 1;
}

uint8_t app_gos_cas_api_start_ecm_filter(play_scramble_para_t *pplayPara)
{
	SGOSCAServiceInfo 		GOSServiceInfo={0};
	play_scramble_para_t playPara = {0};
    CAS_Dbg("app_gos_cas_api_start_ecm_filter \n");
	memcpy(&playPara,pplayPara,sizeof(play_scramble_para_t));

	if (NULL == pplayPara)
	{
		return 0;
	}

	GOSServiceInfo.m_byServiceCount =1;
	if ((0x1FFF == playPara.p_ecm_pid)&&(0x1FFF == playPara.p_ecm_pid_video)
		&&(0x1FFF == playPara.p_ecm_pid_audio))
	{
			GOSServiceInfo.m_wEcmPid = 0x0;
           	GOSServiceInfo.m_wServiceID = playPara.program_num;
			GOSCASTB_SetCurEcmInfos(1,&GOSServiceInfo);			
	}

	if (0x1FFF != playPara.p_ecm_pid)
	{

		GOSServiceInfo.m_wEcmPid = playPara.p_ecm_pid;
        GOSServiceInfo.m_wServiceID = playPara.program_num;

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

		GOSCASTB_SetCurEcmInfos(1,&GOSServiceInfo);

	}

	if (0x1FFF != playPara.p_ecm_pid_audio)
	{

		GOSServiceInfo.m_wEcmPid = playPara.p_ecm_pid_audio;
        GOSServiceInfo.m_wServiceID = playPara.program_num;	
		
		if (0 == app_porting_ca_descramble_set_pid(1,pplayPara->p_audio_pid ))
		{
			CA_FAIL(" Failed\n");
			return 0;
		}

		GOSCASTB_SetCurEcmInfos(1,&GOSServiceInfo);
	}


	if (0x1FFF != playPara.p_ecm_pid_video)
	{
		GOSServiceInfo.m_wEcmPid = playPara.p_ecm_pid_video;
		GOSServiceInfo.m_wServiceID = playPara.program_num;

		if (0 == app_porting_ca_descramble_set_pid(0,pplayPara->p_video_pid ))
		{
			CA_FAIL(" Failed\n");
			return 0;
		}

		GOSCASTB_SetCurEcmInfos(1,&GOSServiceInfo);
	}

	return 1;
}

uint8_t app_gos_cas_api_release_emm_filter(void)
{
	int32_t filterId =0;
	ca_filter_t filter = {0};
	play_scramble_para_t playpara = {0};
	app_play_get_playing_para(&playpara);

    CAS_Dbg("app_gos_cas_api_release_emm_filter \n");
	for (filterId = 0; filterId<MAX_FILTER_COUNT;filterId++)
	{
		app_porting_ca_demux_get_by_filter_id(filterId,&filter);
		if (( 0 != filter.usedStatus)&&(0 != filter.handle)
			&&(0!=filter.channelhandle))
			{
				if (filter.pid == playpara.p_emm_pid)
					app_porting_ca_demux_release_filter( filterId, TRUE);
			}
	}
	
	playpara.p_emm_pid = 0x1fff;
	app_play_set_playing_para(&playpara);

	return 0;
	
}

uint8_t app_gos_cas_api_start_emm_filter(uint16_t emm_pid)
{
    CAS_Dbg("app_gos_cas_api_start_emm_filter \n");
	GOSCASTB_SetEmmPids(1,&emm_pid) ;

	return 0;
}

bool app_gos_cas_api_check_cat_ca_descriptor_valid(uint8_t *sectionData,uint32_t CA_system_id)
{
	return GOSCA_ISSystemID(CA_system_id) ;
}

bool app_gos_cas_api_check_pmt_ca_descriptor_valid(uint8_t *sectionData,uint32_t CA_system_id)
{
	return GOSCA_ISSystemID(CA_system_id) ;
}







