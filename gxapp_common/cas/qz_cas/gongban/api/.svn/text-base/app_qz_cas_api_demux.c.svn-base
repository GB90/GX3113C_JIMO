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
1.0  	2014.5.6		  chenth 	 			creation
*****************************************************************************/
#include "app_common_play.h"
#include "app_common_porting_stb_api.h"
#include "app_common_porting_ca_smart.h"
#include "app_common_porting_ca_demux.h"
#include "app_common_porting_ca_descramble.h"
#include "app_qz_cas_api_demux.h"
#include "gx_demux.h"
#include "gx_descrambler.h"
#include "ca_process.h"

/*解扰器初始化打开一次，0设置为视频，1设置为音频*/
static uint8_t emm_filter_flag = FALSE;
static uint8_t ecm_filter_flag = FALSE;

extern uint8_t prog_scramble_flag;

extern void check_sw1sw2(uint16_t sw1sw2);
extern 	void QZCA_EcmDataGot(uint16_t wEcmPID,uint8_t *ECM_Data,uint8_t ECM_Len);
extern 	void QZCA_EmmDataGot(uint16_t wEcmPID,uint8_t *EMM_Data,uint8_t EMM_Len);
extern void QZCA_ReleasePrivateDataFilter( uint8_t  byReqID,
                                       uint16_t wPid );
extern 	uint8_t QZCA_SetEcmFilter(uint16_t wPid);
extern uint8_t QZCA_SetEmmFilter(uint16_t  wPid);
//过滤器超时回调
//重新设置过滤器
void app_qz_cas_filter_timeout_notify(handle_t Filter, const uint8_t* Section, size_t Size)
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
			&&(0 != filter.channelhandle)&&(EMM_TABLEID == (EMM_TABLEID&filter.byReqID)))
			{		
				if (Filter == filter.handle)
				{
					pid = filter.pid;

					if ((pid == playpara.p_emm_pid)&&(0x1fff != playpara.p_emm_pid)&&
                         (EMM_TABLEID == filter.byReqID))	
					{
                         if (Ca_Card_Ready() == ERR_OK)
                         {
                             printf("app_qz_cas_filter_timeout_notify,set Emm.\n");
                             QZCA_SetEmmFilter(pid);
                         }
					} 				
				}												
			}
	}


   return;	
}

//过滤器数据回调处理接口
void app_qz_cas_filter_notify(handle_t Filter, const uint8_t* Section, size_t Size)
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
			{//遍历所有过滤器
			    app_porting_ca_demux_get_by_filter_id(filterID,&sfilter);
				if (( 0 !=sfilter.usedStatus)&&(pid == sfilter.pid)
					&&(0 !=sfilter.channelhandle))
				{
					    if ((Filter == sfilter.handle))
						{
							/*NIT表或相应FILTER数据*/
							for (i = 0; i<sfilter.filterLen; i++)
							{
								if ((data[i]&sfilter.mask[i]) != (sfilter.match[i]&sfilter.mask[i]))
								{//条件匹配
									CA_FAIL("Pid:%d,byReqID = %#x,data[%d]=0x%x,mask[%d]=0x%x,match[%d]=0x%x \n",
										sfilter.pid,sfilter.byReqID,i,data[i],i,sfilter.mask[i],i,sfilter.match[i]);

                                    //##################
                             #if 0
                                    if(EMM_TABLEID == sfilter->byReqID)
                                    {
                                        printf("################ERROR: release and reset!!!!!\n");
                                        app_qz_cas_api_release_emm_filter();
                                        app_qz_cas_api_reset_emm_filter();

                                    }
                             #endif
                                    
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

			Qz_Dbg("byReqID = %#x\n",byReqID);
			if (ECM_TABLEID == byReqID)
			{
				// ecm数据处理
				if (Ca_Card_Ready() != ERR_OK)
				{
                    Qz_Dbg("CA Card Not Ready\n");
					return ;
				}
                
                Qz_Dbg("[ECM Pack,id:%#x]\n",ECM_TABLEID);
				QZCA_EcmDataGot(pid,data,section_length-3);
				return;
			}
			else if (EMM_TABLEID == byReqID)//emm 数据
			{
                Qz_Dbg("[EMM Pack,id:%#x]\n",EMM_TABLEID);
                if (Ca_Card_Ready() != ERR_OK)
				{
                    Qz_Dbg("CA Card Not Ready\n");
					return ;
				}
				else
				{
					QZCA_EmmDataGot(pid,data,section_length-3);
				}
			}

            
            data += section_length;
            len -= section_length;
		}

		
        return;
}

//切台前调用
uint8_t	app_qz_cas_api_release_ecm_filter(void)
{
	int32_t filterId =0;
	ca_filter_t filter = {0};
	play_scramble_para_t playpara = {0};
	app_play_get_playing_para(&playpara);

	for (filterId = 0; filterId<MAX_FILTER_COUNT;filterId++)
	{
		app_porting_ca_demux_get_by_filter_id(filterId,&filter);
		if (( 0 != filter.usedStatus)&&(0 != filter.handle)
			&&(0!=filter.channelhandle)
			&&(ECM_TABLEID == filter.byReqID))
			{
				QZCA_ReleasePrivateDataFilter(ECM_TABLEID,filter.pid);
			}

	}

    //节目切换,清除上一个节目的信息
	playpara.p_ecm_pid= PSI_INVALID_PID;
	playpara.p_ecm_pid_video= PSI_INVALID_PID;
	playpara.p_ecm_pid_audio= PSI_INVALID_PID;
	app_play_set_playing_para(&playpara);


	return 1;
	
}
play_scramble_para_t qz_play_Para={0};
//----ecm过滤器参数设置
/*0设置为视频，1设置为音频*/
uint8_t app_qz_cas_api_start_ecm_filter(play_scramble_para_t *pplayPara)
{
	play_scramble_para_t playPara = {0};

	CAS_Dbg("app_qz_cas_api_start_ecm_filter \n");
    
	if (NULL == pplayPara)
	{
		return 0;
	}
    
    memcpy(&playPara,pplayPara,sizeof(play_scramble_para_t));

    if(Ca_Card_Ready() != ERR_OK)
    {
        ecm_filter_flag = FALSE;
        memcpy(&qz_play_Para,pplayPara,sizeof(play_scramble_para_t));
        //消息提示
        if(prog_scramble_flag)
        {
            printf("CA Card Not Ready.\n");
            check_sw1sw2(QZ_CARD_NOTFOUND);
        }
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

		QZCA_SetEcmFilter(pplayPara->p_ecm_pid);
	}

	if (0x1FFF != playPara.p_ecm_pid_audio)
	{
		CAS_Dbg("DVTCASTB_SetCurEcmInfo old_p_audio_pid=0x%x old_p_ecm_pid_video =0x%x\n",pplayPara->p_audio_pid,pplayPara->p_ecm_pid_audio);

		if (0 == app_porting_ca_descramble_set_pid(1,pplayPara->p_audio_pid ))
		{
			CA_FAIL(" Failed\n");
			return 0;
		}

		QZCA_SetEcmFilter(pplayPara->p_ecm_pid_audio);
	}


	if (0x1FFF != playPara.p_ecm_pid_video)
	{
		CAS_Dbg("DVTCASTB_SetCurEcmInfo old_p_video_pid=0x%x old_p_ecm_pid_video =0x%x\n",pplayPara->p_video_pid,pplayPara->p_ecm_pid_video);

		if (0 == app_porting_ca_descramble_set_pid(0,pplayPara->p_video_pid ))
		{
			CA_FAIL(" Failed\n");
			return 0;
		}

		QZCA_SetEcmFilter(pplayPara->p_ecm_pid_video);

	}
    ecm_filter_flag = TRUE;
	return 1;
}

uint8_t app_qz_cas_api_reset_ecm_filter(void)
{
    CAS_Dbg("app_qz_cas_api_reset_ecm_filter.\n");
	if (FALSE == ecm_filter_flag)
	{
        app_cas_api_release_ecm_filter();
		app_cas_api_start_ecm_filter(&qz_play_Para);//开启ecm过滤器
        CAS_Dbg("app_qz_cas_api_reset_ecm_filter ok.\n");
	}
	return 0;	
}


uint8_t app_qz_cas_api_release_emm_filter(void)
{
	play_scramble_para_t playpara = {0};
	app_play_get_playing_para(&playpara);
	CAS_Dbg("app_qz_cas_api_release_emm_filter,pid:%d \n",playpara.p_emm_pid);
	QZCA_ReleasePrivateDataFilter(EMM_TABLEID,playpara.p_emm_pid);
	playpara.p_emm_pid = 0x1fff;
	app_play_set_playing_para(&playpara);

	return 0;
}

uint8_t app_qz_cas_api_reset_emm_filter(void)
{
	play_scramble_para_t playpara = {0};
	app_play_get_playing_para(&playpara);


    CAS_Dbg("app_qz_cas_api_reset_emm_filter \n");

	if ((0x1fff != playpara.p_emm_pid)&&(FALSE == emm_filter_flag))
	{
		app_qz_cas_api_start_emm_filter(playpara.p_emm_pid);
        CAS_Dbg("app_qz_cas_api_reset_emm_filter ok.\n");
	}
	return 0;	
}


//启动设置EMM过滤器
uint8_t app_qz_cas_api_start_emm_filter(uint16_t emm_pid)
{
    CAS_Dbg("app_qz_cas_api_start_emm_filter \n");
	printf("emm_pid = 0x%x\n",emm_pid);

    if(Ca_Card_Ready() == ERR_OK)
    {
        emm_filter_flag = TRUE;
        QZCA_SetEmmFilter(emm_pid);
    }
    else
    {
        emm_filter_flag = FALSE;
        if(prog_scramble_flag)
        {
            check_sw1sw2(QZ_CARD_NOTFOUND);
        }
    }
	return 0;
}

//CA cat表校验
bool app_qz_cas_api_check_cat_ca_descriptor_valid(uint8_t *sectionData,uint32_t CA_system_id)
{
    //printf("CAT check:0x%x\n",CA_system_id);
	if (CA_system_id == 0x1234)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

//CA pmt表校验
bool app_qz_cas_api_check_pmt_ca_descriptor_valid(uint8_t *sectionData,uint32_t CA_system_id)
{
    //printf("PMT check:0x%x\n",CA_system_id);
	if (CA_system_id == 0x1234)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

