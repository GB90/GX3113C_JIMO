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
#include "app_wf_cas_api_demux.h"
#include "app_common_porting_ca_smart.h"
#include "app_common_porting_ca_demux.h"
#include "app_common_porting_ca_descramble.h"
#include "gx_demux.h"
#include "gx_descrambler.h"

#include "wanfacas.h"

WFCAS_CaTable_t CaTable;

extern uint8_t app_wf_cas_api_set_ecm_filter(uint16_t ecm_pid);
extern uint8_t app_wf_cas_api_reset_ecm_filter(uint16_t table_id,uint16_t pid);
extern void WanFa_ReleasePrivateDataFilter(uint8_t  byReqID,uint16_t wPid);

static uint16_t g_emm_pid = 0x1fff;

uint16_t prog_service_id = 0;

void app_wf_cas_emm_filter_notify(handle_t Filter, const uint8_t* Section, size_t Size)
{
	uint16_t            filterID;
    	uint16_t            i;
    	int16_t            pid;
    	uint16_t            section_length;
	bool bFlag = FALSE;
	uint8_t byReqID = 0;
    	uint8_t*            data = (uint8_t*)Section;
    	int                 len = Size;
    ca_filter_t filter = {0};
    	int32_t ret;

	U32	iRet = WFERR_COMMON_NO;

//    ASSERT(Section != NULL);
   	ret = GxDemux_FilterGetPID(Filter,&pid);

       while(len > 0) 
	{
            	bFlag = FALSE;
            	section_length = ((data[1] & 0x0F) << 8) + data[2] + 3;
		for (filterID = 0; filterID< MAX_FILTER_COUNT; filterID++)
		{
		      app_porting_ca_demux_get_by_filter_id(filterID,&filter);
			if (( 0 !=filter.usedStatus)&&(pid == filter.pid)
				&&(0 !=filter.channelhandle))
			{
				if (Filter == filter.handle)
					{
						for (i = 0; i<filter.filterLen; i++)
						{
							if ((data[i]&filter.mask[i]) != (filter.match[i]&filter.mask[i]))
								{
									CA_FAIL(" data[%d]=0x%x cafilter[filterID].match[%d]=0x%x \n",
										i,data[i],i,filter.match[i]);
									return;
								}
						}

//						sfilter->nms = 0;
						byReqID = filter.byReqID;
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

		CaTable.Array = data + 3;
		CaTable.Bytes = (section_length -3);
		CaTable.TableId = data[0];

    		if(section_length < 1120)//256
    		{
        		iRet = WFCAS_ParseCaTable(&CaTable);
			if (iRet != WFERR_COMMON_NO)
			{
				CAS_Dbg("EMM_section WFCAS_ParseCaTable wRet=%x\n", (unsigned int)iRet);
			}
			else 
				CAS_Dbg("EMM_section WFCAS_ParseCaTable OK\n");
		}

            	data += section_length;
            	len -= section_length;
        }
        return;

		
}

void app_wf_cas_ecm_filter_notify(handle_t Filter, const uint8_t* Section, size_t Size)
{
	uint16_t            filterID;
   	uint16_t            i;
    	int16_t            pid;
    	uint16_t            section_length;
	bool bFlag = FALSE;
	uint8_t byReqID = 0;
    	uint8_t*            data = (uint8_t*)Section;
    	int                 len = Size;
	ca_filter_t filter = {0};
    	int32_t ret;

	U32	iRet = WFERR_COMMON_NO;
	U32 table_id = 0;
    	ASSERT(Section != NULL);
   	ret = GxDemux_FilterGetPID(Filter,&pid);

       while(len > 0) 
	{
          	bFlag = FALSE;
            	section_length = ((data[1] & 0x0F) << 8) + data[2] + 3;
		for (filterID = 0; filterID< MAX_FILTER_COUNT; filterID++)
		{
		      app_porting_ca_demux_get_by_filter_id(filterID,&filter);
			if (( 0 !=filter.usedStatus)&&(pid == filter.pid)
				&&(0 !=filter.channelhandle))
			{
				if (Filter == filter.handle)
					{
						for (i = 0; i<filter.filterLen; i++)
						{
							if ((data[i]&filter.mask[i]) != (filter.match[i]&filter.mask[i]))
								{
									CA_FAIL(" data[%d]=0x%x cafilter[filterID].match[%d]=0x%x \n",
										i,data[i],i,filter.match[i]);
									return;
								}
						}

//						sfilter->nms = 0;
						byReqID = filter.byReqID;
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
	//	GxDemux_FilterDisable(Filter);

		if((data[0] == WFCAS_ECMTABLE_ID1)||(data[0] == WFCAS_ECMTABLE_ID2))
		{
		
			WanFa_ReleasePrivateDataFilter(byReqID,pid);
		
			CaTable.Array = data + 3;
			CaTable.Bytes = (section_length -3);
			CaTable.TableId = data[0];
		//	CAS_DUMP("CaTable",data,section_length);
	    		if(section_length < 256)
	    		{
	        		iRet = WFCAS_ParseCaTable(&CaTable);
				if (iRet != WFERR_COMMON_NO)
				{
					CAS_Dbg("ECM_section WFCAS_ParseCaTable wRet=%x\n", (unsigned int)iRet);
				}
				else 
					CAS_Dbg("ECM_section WFCAS_ParseCaTable OK	TableId=0x%x\n",data[0]);
			}

			if (data[0] == WFCAS_ECMTABLE_ID1)
			{
				table_id = WFCAS_ECMTABLE_ID2;
			}
			else if (data[0] == WFCAS_ECMTABLE_ID2)
			{
				table_id = WFCAS_ECMTABLE_ID1;
			}
			
			app_wf_cas_api_reset_ecm_filter(table_id,pid);
			
		}
		
            	data += section_length;
            	len -= section_length;
        }
        return;

		
}

void app_wf_cas_filter_timeout_notify(handle_t Filter, const uint8_t* Section, size_t Size)
{
	uint8_t filterId;
//	uint16_t	pid;
	ca_filter_t filter = {0};

	for (filterId= 0; filterId< MAX_FILTER_COUNT; filterId++)
	{
		app_porting_ca_demux_get_by_filter_id(filterId,&filter);
		if (( 0 != filter.usedStatus)&&(0 != filter.handle)
			&&(0 != filter.channelhandle))
		{
		
			if (Filter == filter.handle)
			{


			}												
		}
	}


   	return;	
}


uint8_t	app_wf_cas_api_release_ecm_filter(void)
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
	CAS_Dbg("%s\n",__FUNCTION__);
			
	playpara.p_ecm_pid= PSI_INVALID_PID;
	playpara.p_ecm_pid_video= PSI_INVALID_PID;
	playpara.p_ecm_pid_audio= PSI_INVALID_PID;

	app_play_set_playing_para(&playpara);
    
	return 1;
	
}

void ca_notify_change(U16 Network_Id, U16 Stream_Id, U16 Program_No)
{
	//CA_PRINTF("[CAS] %s\n",__FUNCTION__);
	WFCAS_Program_t wfca_program;
	U32 wRet = 0;
	CAS_Dbg("%s\n",__FUNCTION__);
	wfca_program.Network_Id = Network_Id;
	wfca_program.Program_No = Program_No;
	wfca_program.Stream_Id = Stream_Id;
	
	wRet = WFCAS_ProgramEntitle(&wfca_program);
	if (wRet != WFERR_COMMON_NO)
	{
		CAS_Dbg("ProgramEntitle wRet=%ld\n", wRet);
	}
	else
	{
		CAS_Dbg("WFCAS_ProgramEntitle OK\n");
	}
	return;
}
uint8_t app_wf_cas_api_start_ecm_filter(play_scramble_para_t *pplayPara)
{
	CAS_Dbg("%s\n",__FUNCTION__);
	play_scramble_para_t playPara = {0};
	memcpy(&playPara,pplayPara,sizeof(play_scramble_para_t));

	if (NULL == pplayPara)
	{
		return 0;
	}

	if ((0x1FFF ==playPara.p_ecm_pid )&&(0x1FFF == playPara.p_ecm_pid_audio)&&(0x1FFF == playPara.p_ecm_pid_video))
	{
		/*清流节目*/
		return 0;
	}
	
	ca_notify_change(0xfe, 0xfe,prog_service_id);

	if (0x1FFF != playPara.p_ecm_pid)
	{
			CAS_Dbg("app_wf_cas_api_start_ecm_filter old_p_audio_pid=0x%x old_p_ecm_pid =0x%x\n",pplayPara->p_audio_pid,pplayPara->p_ecm_pid);
			CAS_Dbg("app_wf_cas_api_start_ecm_filter old_p_video_pid=0x%x old_p_ecm_pid =0x%x\n",pplayPara->p_video_pid,pplayPara->p_ecm_pid);


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

			app_wf_cas_api_set_ecm_filter(playPara.p_ecm_pid);


	}

	if (0x1FFF != playPara.p_ecm_pid_audio)
	{
		CAS_Dbg("app_wf_cas_api_start_ecm_filter old_p_video_pid=0x%x old_p_ecm_pid_video =0x%x\n",pplayPara->p_video_pid,pplayPara->p_ecm_pid_video);

        if (0 == app_porting_ca_descramble_set_pid(1,pplayPara->p_audio_pid ))
        {
            CA_FAIL(" Failed\n");
            return 0;
        }

		app_wf_cas_api_set_ecm_filter(playPara.p_ecm_pid_audio);

	}


	if (0x1FFF != playPara.p_ecm_pid_video)
	{
		CAS_Dbg("app_wf_cas_api_start_ecm_filter old_p_video_pid=0x%x old_p_ecm_pid_video =0x%x\n",pplayPara->p_video_pid,pplayPara->p_ecm_pid_video);


        if (0 == app_porting_ca_descramble_set_pid(0,pplayPara->p_video_pid ))
        {
            CA_FAIL(" Failed\n");
            return 0;
        }

		app_wf_cas_api_set_ecm_filter(playPara.p_ecm_pid_video);
	}
	
	return 1;
}

uint8_t app_wf_cas_api_release_emm_filter(void)
{
	CAS_Dbg("%s\n",__FUNCTION__);
	int32_t filterId =0;
	ca_filter_t filter = {0};

	for (filterId = 0; filterId<MAX_FILTER_COUNT;filterId++)
	{
		app_porting_ca_demux_get_by_filter_id(filterId,&filter);
		if (( 0 != filter.usedStatus)&&(0 != filter.handle)&&
			(0!=filter.channelhandle))
			{
				if (filter.pid == g_emm_pid)
					app_porting_ca_demux_release_filter( filterId, TRUE);
			}

	}

	g_emm_pid = 0x1fff;
	return 1;
}

extern uint8_t app_wf_cas_api_set_emm_filter(uint16_t emm_pid);
uint8_t app_wf_cas_api_start_emm_filter(uint16_t emm_pid)
{
	CAS_Dbg("%s	emm_pid=%d\n",__FUNCTION__,emm_pid);
	g_emm_pid = emm_pid;
	app_wf_cas_api_set_emm_filter(g_emm_pid);
	return 1;
}

bool app_wf_cas_api_check_cat_ca_descriptor_valid(uint8_t *sectionData,uint32_t CA_system_id)
{
	WFCAS_Version_t wfca_ver;
	U32	iRet = WFERR_COMMON_NO;
   	iRet = WFCAS_GetVerInfo(&wfca_ver);
	if (iRet != WFERR_COMMON_NO)
	{
		CAS_Dbg("[cat]WFCAS_GetVerInfo Ret=%x\n", (unsigned int)iRet);
	}
	
	CAS_Dbg("[cat]CA_PID %x, Cas_Id %x\n", CA_system_id, wfca_ver.Cas_Id);
	if (0xff00 == CA_system_id)
	{
		return TRUE;
	}
	else
		return FALSE;
}

bool app_wf_cas_api_check_pmt_ca_descriptor_valid(uint8_t *sectionData,uint32_t CA_system_id)
{
	WFCAS_Version_t wfca_ver;
	U32	iRet = WFERR_COMMON_NO;
   	iRet = WFCAS_GetVerInfo(&wfca_ver);
	if (iRet != WFERR_COMMON_NO)
	{
		CAS_Dbg("[pmt]WFCAS_GetVerInfo Ret=%x\n", (unsigned int)iRet);
	}
	
	CAS_Dbg("[pmt]CA_PID %x, Cas_Id %x\n", CA_system_id, wfca_ver.Cas_Id);
	if (0xff00 == CA_system_id)
	{
		return TRUE;
	}
	else
		return FALSE;
}

void app_wf_cas_api_nit_zone_des(uint8_t* databuf)
{
	CAS_Dbg("%s\n",__FUNCTION__);
	int32_t ret = 0;	
	U08 * data = databuf;
	U08 descriptor_length = 0; 
	U08 aucAddrCode[8];
	descriptor_length = data[1];
	data = data + 2;
	if (descriptor_length == 8)
	{
		memcpy(aucAddrCode, &data[0], sizeof(aucAddrCode));
		ret = WFCAS_SetZone(aucAddrCode);
		if(ret == 0)
			CAS_Dbg("app_wf_cas_api_nit_zone_des	success\n");
		else
			CAS_Dbg("app_wf_cas_api_nit_zone_des	failure\n");
		//DUMP("GET AddCode",aucAddrCode,8);
	}
}


