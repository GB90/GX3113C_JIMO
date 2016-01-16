/*****************************************************************************
* 						   CONFIDENTIAL
*        Hangzhou GuoXin Science and Technology Co., Ltd.
*                      (C)2012, All right reserved
******************************************************************************

******************************************************************************
* File Name :	tr_cas_porting_demux.c
* Author    : 	bingyu
* Project   :	goxceed dvbc
* Type      :
******************************************************************************
* Purpose   :	模块头文件
******************************************************************************
* Release History:
VERSION	Date			  AUTHOR         Description
1.0  	2012.12.02		  xxxx 	 			creation
*****************************************************************************/
#include <gxtype.h>
#include "gxmsg.h"
#include "gxapp_sys_config.h"
#include "app_common_play.h"
#include "app_common_table_pmt.h"
#include "app_common_porting_stb_api.h"
#include "app_common_porting_ca_flash.h"
#include "app_common_porting_ca_demux.h"
#include "app_common_porting_ca_descramble.h"
#include "app_common_porting_ca_smart.h"

#include "Tr_Cas.h"


#define CHANNEL_NUM_MAX 		(10)
#define DESCRAMBLE_NUM_MAX  	(2)
#define CAS_SECLEN_MAX     (1024)
#define CAS_INVALID_PID    (0x1FFF)

#define TRCA_DMX_LOCK  			GxCore_SemWait(tr_demuxSema)
#define TRCA_DMX_UNLOCK 	GxCore_SemPost(tr_demuxSema)

typedef void(*chUpcallFct)(U32 ulChannelId);

typedef struct ca_channel_S
{	
	//handle_t handle;	
	uint8_t usedStatus;
	uint8_t bEnable;
	uint8_t FetchDataFlag[MAX_FILTER_COUNT];
	uint8_t *pSectionData[MAX_FILTER_COUNT];
	int pSectionLength[MAX_FILTER_COUNT];
	uint32_t dCrc32[MAX_FILTER_COUNT];
	ca_filter_t cafilter[MAX_FILTER_COUNT];	
	u_int16_t pid;	
	u_int16_t wMaxFilterSize;
	chUpcallFct clbkDemuxProcessor;
}ca_channel_t;

extern void app_tr_cas_api_get_app_scramble_para(play_scramble_para_t ** Param);

static ca_channel_t s_caChannel[CHANNEL_NUM_MAX] = {{0}};
static U16 w_EcmPid[DESCRAMBLE_NUM_MAX]={CAS_INVALID_PID, CAS_INVALID_PID};
static int dDescrambleCounter = 0;
static handle_t tr_demuxSema = 0;
static U8 s_evenKey[8] = {0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF};
static U8 s_oddKey[8] = {0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF};
static U8 s_evenKey1[8] = {0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF};
static U8 s_oddKey1[8] = {0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF};

static U16 wEmmPid = CAS_INVALID_PID;
static uint16_t Apid = CAS_INVALID_PID;
static uint16_t Vpid = CAS_INVALID_PID;

static void tr_cas_filter_notify(handle_t Filter, const uint8_t* Section, size_t Size);
static void tr_cas_filter_timeout_notify(handle_t Filter, const uint8_t* Section, size_t Size);


/*---------------------------------------------------------------------------------------------*/
void tr_cas_release_ecm_filter(void)
{
	uint8_t i = 0;
	uint8_t filterID = 0;
	ca_filter_t filter = {0};
	play_scramble_para_t *pPlayparam = NULL;

	//printf("[tr_cas_release_ecm_filter]Enter...\n");
	TRCA_DMX_LOCK;
	
	app_tr_cas_api_get_app_scramble_para(&pPlayparam);
	if (pPlayparam)
	{
		if (pPlayparam->p_ecm_pid != CAS_INVALID_PID)
		{
			for (i = 0; i < CHANNEL_NUM_MAX; i++)
			{
				if (s_caChannel[i].usedStatus 
					&& (s_caChannel[i].pid == pPlayparam->p_ecm_pid))
				{
					break;
				}
			}

			if (i < CHANNEL_NUM_MAX)
			{
				for (filterID = 0; filterID < MAX_FILTER_COUNT; filterID++)
				{
				    app_porting_ca_demux_get_by_filter_id(filterID, &filter);
					if ((0 != filter.usedStatus)
						&& (s_caChannel[i].pid == filter.pid)
						&& (0 != filter.handle))
					{
						printf("[tr_cas_release_ecm_filter]release ecm filter(%d:0x%x).\n",\
							 		filterID, filter.pid);
						app_porting_ca_demux_release_filter(filterID, TRUE);
					}
				}
				
				memset(s_caChannel[i].cafilter, 0x00, MAX_FILTER_COUNT*sizeof(ca_filter_t));
				memset(s_caChannel[i].dCrc32, 0x00, MAX_FILTER_COUNT*sizeof(uint32_t));
				s_caChannel[i].bEnable = 0;
				s_caChannel[i].usedStatus = 0;
				s_caChannel[i].wMaxFilterSize = 0;
				s_caChannel[i].pid = 0;
			}
			else
			{
				printf("[tr_cas_release_ecm_filter]not find valid channel:ECM PID(0x%x)!!!\n",\
					  		pPlayparam->p_ecm_pid);
			}
		}
				
		if (pPlayparam->p_ecm_pid_audio != CAS_INVALID_PID)
		{
			for (i = 0; i < CHANNEL_NUM_MAX; i++)
			{
				if (s_caChannel[i].usedStatus 
					&& (s_caChannel[i].pid == pPlayparam->p_ecm_pid_audio))
				{
					break;
				}
			}

			if (i < CHANNEL_NUM_MAX)
			{
				for (filterID = 0; filterID < MAX_FILTER_COUNT; filterID++)
				{
				    app_porting_ca_demux_get_by_filter_id(filterID, &filter);
					if ((0 != filter.usedStatus) 
						&& (s_caChannel[i].pid == filter.pid)
						&& (0 != filter.handle))
					{
						printf("[tr_cas_release_ecm_filter]release aud ecm filter(%d:0x%x).\n",\
							 		filterID, filter.pid);
						app_porting_ca_demux_release_filter(filterID, TRUE);
					}
				}
				
				memset(s_caChannel[i].cafilter, 0x00, MAX_FILTER_COUNT*sizeof(ca_filter_t));
				memset(s_caChannel[i].dCrc32, 0x00, MAX_FILTER_COUNT*sizeof(uint32_t));
				s_caChannel[i].bEnable = 0;
				s_caChannel[i].usedStatus = 0;
				s_caChannel[i].wMaxFilterSize = 0;
				s_caChannel[i].pid = 0;
			}
			else
			{
				printf("[tr_cas_release_ecm_filter]not find valid channel:AUD ECM PID(0x%x)!!!\n",\
					  		pPlayparam->p_ecm_pid_audio);
			}
		}

		if (pPlayparam->p_ecm_pid_video != CAS_INVALID_PID)
		{
			for (i = 0; i < CHANNEL_NUM_MAX; i++)
			{
				if (s_caChannel[i].usedStatus 
					&& (s_caChannel[i].pid == pPlayparam->p_ecm_pid_video))
				{
					break;
				}
			}

			if (i < CHANNEL_NUM_MAX)
			{
				for (filterID = 0; filterID < MAX_FILTER_COUNT; filterID++)
				{
				    app_porting_ca_demux_get_by_filter_id(filterID, &filter);
					if ((0 != filter.usedStatus) 
						&& (s_caChannel[i].pid == filter.pid)
						&& (0 != filter.handle))
					{
						printf("[tr_cas_release_ecm_filter]release vid ecm filter(%d:0x%x).\n",\
							  		filterID, filter.pid);
						app_porting_ca_demux_release_filter(filterID, TRUE);
					}
				}
				
				memset(s_caChannel[i].cafilter, 0x00, MAX_FILTER_COUNT*sizeof(ca_filter_t));
				memset(s_caChannel[i].dCrc32, 0x00, MAX_FILTER_COUNT*sizeof(uint32_t));
				s_caChannel[i].bEnable = 0;
				s_caChannel[i].usedStatus = 0;
				s_caChannel[i].wMaxFilterSize = 0;
				s_caChannel[i].pid = 0;
			}
			else
			{
				printf("[tr_cas_release_ecm_filter]not find valid channel:VID ECM PID(0x%x)!!!\n",\
					   		pPlayparam->p_ecm_pid_video);
			}
		}
	}

	TRCA_DMX_UNLOCK;
	GxCore_ThreadDelay(100);
	//printf("[tr_cas_release_ecm_filter]Exit.\n");
	return;
}

void tr_cas_release_emm_filter(void)
{
	uint8_t i = 0;
	uint8_t filterID = 0;
	ca_filter_t filter = {0};

	//printf("[tr_cas_release_emm_filter]Enter...\n");
	TRCA_DMX_LOCK;

	if (wEmmPid != CAS_INVALID_PID)
	{
		for (i = 0; i < CHANNEL_NUM_MAX; i++)
		{
			if (s_caChannel[i].usedStatus 
				&& (s_caChannel[i].pid == wEmmPid))
			{
				break;
			}
		}
		
		if (i < CHANNEL_NUM_MAX)
		{
			for (filterID = 0; filterID < MAX_FILTER_COUNT; filterID++)
			{
			    app_porting_ca_demux_get_by_filter_id(filterID, &filter);
				if ((0 != filter.usedStatus) 
					&& (s_caChannel[i].pid == filter.pid)
					&& (0 != filter.handle))
				{
					printf("[tr_cas_release_emm_filter]release emm filter(%d:0x%x).\n",\
						  		filterID, filter.pid);
					app_porting_ca_demux_release_filter(filterID, TRUE);
				}
			}
			
			memset(s_caChannel[i].cafilter, 0x00, MAX_FILTER_COUNT*sizeof(ca_filter_t));
			memset(s_caChannel[i].dCrc32, 0x00, MAX_FILTER_COUNT*sizeof(uint32_t));
			s_caChannel[i].bEnable = 0;
			s_caChannel[i].usedStatus = 0;
			s_caChannel[i].wMaxFilterSize = 0;
			s_caChannel[i].pid = 0;
		}
		else
		{
			printf("[tr_cas_release_emm_filter]not find valid channel:EMM PID(0x%x)!!!\n", wEmmPid);
		}
	}
	
	wEmmPid = CAS_INVALID_PID;
	TRCA_DMX_UNLOCK;
	//printf("[tr_cas_release_emm_filter]Exit.\n");
	return;
}

S16   TRDRV_DEMUX_Initialise(void)
{
	dDescrambleCounter = 0;
	memset(s_caChannel, 0x00, sizeof(ca_channel_t)*CHANNEL_NUM_MAX);
	memset(w_EcmPid, 0x00, sizeof(U16)*DESCRAMBLE_NUM_MAX);
	
	GxCore_SemCreate(&tr_demuxSema, 1);

	return 0;
}

U32   TRDRV_DEMUX_AllocateSectionChannel(U16 wDemuxId, U16 wMaxFilterNumber, 
	   												U16 wMaxFilterSize, U32 ulBufferSize)
{ 
	U8 i = 0;

	TRCA_DMX_LOCK;
	for (i = 0; i < CHANNEL_NUM_MAX; i++)
	{
		if ((0 == s_caChannel[i].usedStatus))
		{
			break;
		}
	}
	
	if (i >= CHANNEL_NUM_MAX)
	{
		CAS_Dbg("[TRDRV_DEMUX_AllocateSectionChannel] error CAS_DEMUX_INVALID_CHANNEL_ID!!!\n");
		TRCA_DMX_UNLOCK;
		return CAS_DEMUX_INVALID_CHANNEL_ID;
	}
	
	s_caChannel[i].usedStatus = 1;
	s_caChannel[i].wMaxFilterSize = wMaxFilterSize;
	
	if (wMaxFilterSize > 12)
	{
		CAS_Dbg("[TRDRV_DEMUX_AllocateSectionChannel]filter size > 12!!!\n");
	}
	
	TRCA_DMX_UNLOCK;
	return i;
}

void  TRDRV_DEMUX_SetChannelPid(U32 ulChannelId, U16 wChannelPid)
{
	char bTableId = 0;
	bool repeatFlag = TRUE;
	ca_filter_t filter = {0};
	int filterID = 0;
	int i = 0;
		
	//printf("[TRDRV_DEMUX_SetChannelPid]channelId(%d) Pid(0x%x).\n", (unsigned int)ulChannelId, wChannelPid);	
	TRCA_DMX_LOCK;
	
	if (ulChannelId >= CHANNEL_NUM_MAX)
	{
		CAS_Dbg("[TRDRV_DEMUX_SetChannelPid]error CHANNEL_NUM_MAX!!!");
		TRCA_DMX_UNLOCK;
		return;
	}
	
	if (s_caChannel[ulChannelId].usedStatus == 0)
	{
		CAS_Dbg("[TRDRV_DEMUX_SetChannelPid]error usedStatus=0!!!");
		TRCA_DMX_UNLOCK;
		return;
	}

	s_caChannel[ulChannelId].pid = wChannelPid;
	
	for (i = 0; i < MAX_FILTER_COUNT; i++)
	{
		bTableId = (s_caChannel[ulChannelId].cafilter[i].match[0]) & (s_caChannel[ulChannelId].cafilter[i].mask[0]);
		if (((bTableId != 0) && (bTableId != 0x81) && (bTableId != 0x80)) 
			&& (s_caChannel[ulChannelId].cafilter[i].usedStatus == 1)
			&& (s_caChannel[ulChannelId].cafilter[i].handle != 0))
		{
			for (filterID = 0; filterID < MAX_FILTER_COUNT; filterID++)
			{
			    app_porting_ca_demux_get_by_filter_id(filterID, &filter);
				if ((0 != filter.usedStatus) 
					&& (s_caChannel[ulChannelId].cafilter[i].handle == filter.handle))
				{
					printf("[TRDRV_DEMUX_SetChannelPid]release filter(%d).\n", filterID);
					app_porting_ca_demux_release_filter(filterID, TRUE);
					break;
				}
			}	
		}
	}

	for (i = 0; i < MAX_FILTER_COUNT; i++)
	{
		bTableId = (s_caChannel[ulChannelId].cafilter[i].match[0]) & (s_caChannel[ulChannelId].cafilter[i].mask[0]);
		if (((bTableId != 0) && (bTableId != 0x81) && (bTableId != 0x80)) 
			&& (s_caChannel[ulChannelId].cafilter[i].usedStatus == 1))
		{
			memset(&filter, 0, sizeof(ca_filter_t));
			filter.crcFlag = TRUE;
			filter.filterLen = s_caChannel[ulChannelId].wMaxFilterSize;
			memcpy(filter.match, s_caChannel[ulChannelId].cafilter[i].match, filter.filterLen);
			memcpy(filter.mask, s_caChannel[ulChannelId].cafilter[i].mask, filter.filterLen);
			//CAS_DUMP("EMMmatch", s_caChannel[ulChannelId].cafilter[i].match, s_caChannel[ulChannelId].wMaxFilterSize);
			//CAS_DUMP("EMMmask", s_caChannel[ulChannelId].cafilter[i].mask, s_caChannel[ulChannelId].wMaxFilterSize);
			filter.nWaitSeconds = -1;
			wEmmPid = filter.pid = s_caChannel[ulChannelId].pid;
			filter.equalFlag = TRUE;
			filter.repeatFlag = repeatFlag;
			filter.Callback = tr_cas_filter_notify;
			filter.timeOutCallback = tr_cas_filter_timeout_notify;
			app_porting_ca_demux_start_filter(&filter);
			//s_caChannel[ulChannelId].cafilter[ulFilterId].usedStatus=1;
			//memcpy(s_caChannel[ulChannelId].cafilter[ulFilterId].match,pbData,filter.filterLen);
			//memcpy(s_caChannel[ulChannelId].cafilter[ulFilterId].mask,pbMask,filter.filterLen);
			//printf("\n[EMM filter]filter  handle =0x%x.\n",(int)(filter.handle));
			memcpy(&(s_caChannel[ulChannelId].cafilter[i]), &filter, sizeof(ca_filter_t));
			s_caChannel[ulChannelId].bEnable = 1;
		}
	}

	TRCA_DMX_UNLOCK;	
	return;	
}

void  TRDRV_DEMUX_ControlChannel(U32 ulChannelId, CAS_DEMUX_CTRL eAction)
{
	TRCA_DMX_LOCK;
	
	if (ulChannelId >= CHANNEL_NUM_MAX)
	{
		CAS_Dbg("[TRDRV_DEMUX_ControlChannel] error CHANNEL_NUM_MAX!!!\n");
		TRCA_DMX_UNLOCK;
		return;
	}
	
	if (s_caChannel[ulChannelId].usedStatus == 0)
	{
		CAS_Dbg("[TRDRV_DEMUX_ControlChannel] error usedStatus=0!!!\n");
		TRCA_DMX_UNLOCK;
		return;
	}
	
	switch (eAction)
	{
			case CAS_DEMUX_DISABLE_CHANNEL:
			CAS_Dbg("\n CAS_DEMUX_DISABLE_CHANNEL+*+\n");
			s_caChannel[ulChannelId].bEnable=0;
			//app_porting_ca_demux_disable_channel(s_caChannel[ulChannelId].pid);
			break;

			case CAS_DEMUX_ENABLE_CHANNEL:
			CAS_Dbg("\n CAS_DEMUX_ENABLE_CHANNEL+-+\n");
			s_caChannel[ulChannelId].bEnable=1;
			//app_porting_ca_demux_enable_channel(s_caChannel[ulChannelId].pid);
			break;

			case CAS_DEMUX_RESET_CHANNEL:
			CAS_Dbg("\n CAS_DEMUX_RESET_CHANNEL+#+\n");
			//app_porting_ca_demux_reset_channel(s_caChannel[ulChannelId].pid);
			break;
			
			default:
			CAS_Dbg("\n[TRDRV_DEMUX_ControlChannel] error default!!!\n");
			break;

	}
	
	TRCA_DMX_UNLOCK;
	return;
}

void  TRDRV_DEMUX_RegisterChannelUpcallFct(U32 ulChannelId, IN void (*chUpcallFct)(U32 ulChannelId))
{
	TRCA_DMX_LOCK;
	
	if (ulChannelId >= CHANNEL_NUM_MAX)
	{
		printf("\n[TRDRV_DEMUX_SetChannelPid] error CHANNEL_NUM_MAX!!!\n");
		TRCA_DMX_UNLOCK;
		return;
	}
	
	if (s_caChannel[ulChannelId].usedStatus == 0)
	{
		printf("\n[TRDRV_DEMUX_SetChannelPid] error usedStatus=0\n");
		TRCA_DMX_UNLOCK;
		return;
	}

	if (NULL == chUpcallFct)
	{
		printf("\n[TRDRV_DEMUX_SetChannelPid]chUpcallFct is NULL!!!\n");
	}
	
	s_caChannel[ulChannelId].clbkDemuxProcessor = chUpcallFct;

	TRCA_DMX_UNLOCK;
	return;
}

void  TRDRV_DEMUX_FreeSectionChannel(U32 ulChannelId)
{
	//printf("\n[TRDRV_DEMUX_FreeSectionChannel]channelId(%d).\n",  (unsigned int)ulChannelId);
	TRCA_DMX_LOCK;
	
	if (ulChannelId >= CHANNEL_NUM_MAX)
	{
		printf("\n[TRDRV_DEMUX_FreeSectionChannel] error CHANNEL_NUM_MAX!!!\n");
		TRCA_DMX_UNLOCK;
		return;
	}
	
	if (s_caChannel[ulChannelId].usedStatus == 0)
	{
		printf("\n[TRDRV_DEMUX_FreeSectionChannel] error usedStatus=0!!!\n");
		TRCA_DMX_UNLOCK;
		return;
	}

	s_caChannel[ulChannelId].usedStatus = 0;
	s_caChannel[ulChannelId].pid = CAS_INVALID_PID;
	s_caChannel[ulChannelId].wMaxFilterSize = 0;
	s_caChannel[ulChannelId].clbkDemuxProcessor = NULL;
	TRCA_DMX_UNLOCK;
	return;
}

U32   TRDRV_DEMUX_AllocateFilter(U32 ulChannelId)
{
	uint8_t i = 0;
	
	TRCA_DMX_LOCK;
	if (ulChannelId >= CHANNEL_NUM_MAX)
	{
		printf("\n[TRDRV_DEMUX_AllocateFilter] error CHANNEL_NUM_MAX!!!\n");
		TRCA_DMX_UNLOCK;
		return CAS_DEMUX_INVALID_FILTER_ID;
	}
	
	if (s_caChannel[ulChannelId].usedStatus == 0)
	{
		printf("\n[TRDRV_DEMUX_AllocateFilter] error usedStatus=0!!!\n");
		TRCA_DMX_UNLOCK;
		return CAS_DEMUX_INVALID_FILTER_ID;
	}
	
	for (i = 0; i < MAX_FILTER_COUNT; i++)
	{
		if ((0 == s_caChannel[ulChannelId].cafilter[i].usedStatus) 
			&& (0 == s_caChannel[ulChannelId].cafilter[i].handle))
		{
			break;
		}
	}

	if (i >= MAX_FILTER_COUNT)
	{
		CAS_Dbg("\n[TRDRV_DEMUX_AllocateFilter]channelId(%d) have no free filter!!!\n", (unsigned int)ulChannelId);
		TRCA_DMX_UNLOCK;
		return CAS_DEMUX_INVALID_FILTER_ID;
	}
	
	s_caChannel[ulChannelId].cafilter[i].usedStatus = 1;
	//printf("\n[TRDRV_DEMUX_AllocateFilter]channelId(%d) get filterId(%d).\n", (unsigned int)ulChannelId, i);
	TRCA_DMX_UNLOCK;
	return i;
}

void tr_cas_filter_notify(handle_t Filter, const uint8_t* Section, size_t Size)
{
	int i = 0;
	int filterID = 0;
	int ret = 0;
	int16_t pid = 0;
	int len = Size;
	int section_length = 0;
	uint8_t *data = (uint8_t *)Section;
	int channelID = 0;
	uint8_t *matchPtr = NULL;
	uint8_t *maskPtr = NULL;
	uint8_t depth = 0;
	chUpcallFct clbkDemuxProcessor = NULL;
	uint32_t ecm_crc32 = 0;

	ret = GxDemux_FilterGetPID(Filter,&pid);

	TRCA_DMX_LOCK;
	while (len > 0) 
	{
		section_length = ((data[1] & 0x0F) << 8) + data[2] + 3;
		clbkDemuxProcessor = NULL;
		for (channelID = 0; channelID < CHANNEL_NUM_MAX; channelID++)
		{	
			if ((pid == s_caChannel[channelID].pid) 
				&& (1 == s_caChannel[channelID].usedStatus) 
				&& (s_caChannel[channelID].bEnable == 1))
			{
				for (filterID = 0; filterID < MAX_FILTER_COUNT; filterID++)
				{
					if ((Filter == s_caChannel[channelID].cafilter[filterID].handle)
						&& (1 == s_caChannel[channelID].cafilter[filterID].usedStatus))
					{
						matchPtr = s_caChannel[channelID].cafilter[filterID].match;
						maskPtr = s_caChannel[channelID].cafilter[filterID].mask;
						depth = s_caChannel[channelID].wMaxFilterSize;

						for (i = 0; i < depth; i++)
						{
							if ((data[i] & maskPtr[i]) != (matchPtr[i] & maskPtr[i]))
							{
								printf("<tr_cas_filter_notify>break errror111###\n");
								break;
							}
						}

						/*CRC check start.-----------------*/
						ecm_crc32 = (data[section_length-4] << 24) 
						  			| (data[section_length-3] << 16) 
						  			| (data[section_length-2] << 8) 
						  			| (data[section_length-1]);
						if (s_caChannel[channelID].dCrc32[filterID] == ecm_crc32)
						{
							goto TR_NOTIFY_FILTER_END;
						}
						
						s_caChannel[channelID].dCrc32[filterID] = ecm_crc32;
						/*CRC check end.-----------------*/

						if (i >= depth)
						{
							s_caChannel[channelID].pSectionLength[filterID] = section_length;
							if (NULL == s_caChannel[channelID].pSectionData[filterID])
							{
								s_caChannel[channelID].pSectionData[filterID] = GxCore_Malloc(CAS_SECLEN_MAX);
								if (NULL == s_caChannel[channelID].pSectionData[filterID])
								{
									printf("@@@[tr_cas_filter_notify]malloc pSectionData(%d) fail!!!\n", filterID);
								}
							}
							
							if (s_caChannel[channelID].pSectionData[filterID])
							{
								memset(s_caChannel[channelID].pSectionData[filterID], 0x00, CAS_SECLEN_MAX);
								if (Size <= CAS_SECLEN_MAX)
								{
									memcpy(s_caChannel[channelID].pSectionData[filterID], data, section_length);
								}
								else
								{
									printf("@@@[tr_cas_filter_notify]Size > CAS_SECLEN_MAX, Size(%d)!!!\n", Size);
								}
							}
							
							clbkDemuxProcessor = s_caChannel[channelID].clbkDemuxProcessor;
							break;	
						}
						else
						{
							printf("[tr_cas_filter_notify]pid(0x%x) tableid(0x%x) error!!!Reset filter.\n", pid, data[0]);
							goto TR_NOTIFY_FILTER_END;
						}
					}
				}
			}
			
			if (NULL != clbkDemuxProcessor)
			{
				//printf("\n [tr_cas_filter_notify]>>>>>>>clbkDemuxProcessor goto TR_NOTIFY_FILTER_END \n");
				goto TR_NOTIFY_FILTER_END;
			}
		}

		data += section_length;
		len -= section_length;		
	}
	 
TR_NOTIFY_FILTER_END:
	TRCA_DMX_UNLOCK;
	if (NULL != clbkDemuxProcessor)
	{
		//printf("\n [tr_cas_filter_notify]>>>>>>>channelID =%d \n",channelID);
		clbkDemuxProcessor(channelID);
	}

	return;
}

void tr_cas_filter_timeout_notify(handle_t Filter, const uint8_t* Section, size_t Size)
{
	//printf("\n tr_cas_filter_timeout_notify \n");
}

S16   TRDRV_DEMUX_SetFilter(U32 ulChannelId, U32 ulFilterId, IN U8 *pbMask, IN U8 *pbData)
{
	char bTableId = 0;
	bool repeatFlag = TRUE;
	ca_filter_t filter = {0};
	int filterID = 0;
	
	TRCA_DMX_LOCK;

	bTableId = pbMask[0] & pbData[0];
	//printf("\n[TRDRV_DEMUX_SetFilter]bTableId=0x%x.\n", bTableId);
	if ((bTableId != 0x80) && (bTableId != 0x81))
	{
		//printf("\n EMM filter usedStatus = %d.\n", s_caChannel[ulChannelId].cafilter[ulFilterId].usedStatus);
		memcpy(s_caChannel[ulChannelId].cafilter[ulFilterId].match,\
		  			pbData, s_caChannel[ulChannelId].wMaxFilterSize);
		memcpy(s_caChannel[ulChannelId].cafilter[ulFilterId].mask,\
			  		pbMask, s_caChannel[ulChannelId].wMaxFilterSize);
		TRCA_DMX_UNLOCK;
		return 0;
	}
	
	//printf("\n[TRDRV_DEMUX_SetFilter] ulChannelId=%ld,ulFilterId=%ld,wMaxFilterSize=%d\n", 
			//ulChannelId, ulFilterId, s_caChannel[ulChannelId].wMaxFilterSize);
	//CAS_DUMP("ECM match",pbData,s_caChannel[ulChannelId].wMaxFilterSize);
	//CAS_DUMP("ECM mask",pbMask,s_caChannel[ulChannelId].wMaxFilterSize);

	if (ulChannelId >= CHANNEL_NUM_MAX)
	{
		printf("\n[TRDRV_DEMUX_SetFilter] error CHANNEL_NUM_MAX!!!\n");
		TRCA_DMX_UNLOCK;
		return 1;
	}
	
	if (s_caChannel[ulChannelId].usedStatus == 0)
	{
		printf("\n[TRDRV_DEMUX_SetFilter] error usedStatus=0!!!\n");
		TRCA_DMX_UNLOCK;
		return 1;
	}
	
	if (ulFilterId > MAX_FILTER_COUNT)
	{
		printf("\nTRDRV_DEMUX_SetFilter] error MAX_FILTER_COUNT!!!\n");
		TRCA_DMX_UNLOCK;
		return 1;
	}
	
	for (filterID = 0; filterID < MAX_FILTER_COUNT; filterID++)
	{
	    app_porting_ca_demux_get_by_filter_id(filterID, &filter);
		if ((0 != filter.usedStatus) 
			&& (s_caChannel[ulChannelId].pid == filter.pid)
			&& (s_caChannel[ulChannelId].cafilter[ulFilterId].channelhandle == filter.channelhandle)
			&& (s_caChannel[ulChannelId].cafilter[ulFilterId].handle == filter.handle))
		{
			//printf("\n[TRDRV_DEMUX_SetFilter] release filter!!!\n");
			app_porting_ca_demux_release_filter(filterID, TRUE);
			break;
		}
	}

	memset(&filter, 0, sizeof(ca_filter_t));
	filter.crcFlag = TRUE;
	filter.filterLen = s_caChannel[ulChannelId].wMaxFilterSize;
	memcpy(filter.match, pbData, filter.filterLen);
	memcpy(filter.mask, pbMask, filter.filterLen);
	filter.nWaitSeconds = -1;
	filter.pid = s_caChannel[ulChannelId].pid;
	filter.equalFlag = TRUE;
	filter.repeatFlag = repeatFlag;
	filter.Callback = tr_cas_filter_notify;
	filter.timeOutCallback = tr_cas_filter_timeout_notify;
	app_porting_ca_demux_start_filter(&filter);

	//printf("\n[ECM filter]filter handle = 0x%x.\n", (int)(filter.handle));
	s_caChannel[ulChannelId].bEnable = 1;
	memcpy(&(s_caChannel[ulChannelId].cafilter[ulFilterId]), &filter, sizeof(ca_filter_t));

	TRCA_DMX_UNLOCK;
	return 0;
}

S16   TRDRV_DEMUX_FreeFilter(U32 ulChannelId, U32 ulFilterId)
{
	//printf("\nTRDRV_DEMUX_FreeFilter ulChannelId=%ld,ulFilterId=%ld.\n", ulChannelId, ulFilterId);
	TRCA_DMX_LOCK;
	
	if (ulChannelId >= CHANNEL_NUM_MAX)
	{
		printf("\n[TRDRV_DEMUX_FreeFilter] error CHANNEL_NUM_MAX!!!\n");
		TRCA_DMX_UNLOCK;
		return 1;
	}
	
	if (s_caChannel[ulChannelId].usedStatus == 0)
	{
		printf("\n[TRDRV_DEMUX_FreeFilter] error usedStatus=0!!!\n");
		TRCA_DMX_UNLOCK;
		return 1;
	}
	
	if (ulFilterId > MAX_FILTER_COUNT)
	{
		printf("\n[TRDRV_DEMUX_FreeFilter] error MAX_FILTER_COUNT!!!\n");
		TRCA_DMX_UNLOCK;
		return 1;
	}
	
	if (s_caChannel[ulChannelId].cafilter[ulFilterId].usedStatus == 1)
	{
		ca_filter_t filter = {0,};
		uint8_t filterID = 0;
		
		for (filterID = 0; filterID < MAX_FILTER_COUNT; filterID++)
		{
		    app_porting_ca_demux_get_by_filter_id(filterID, &filter);
			if ((0 != filter.usedStatus) 
				&& (s_caChannel[ulChannelId].pid == filter.pid)
				&& (0 != filter.channelhandle) 
				&& (filter.handle == s_caChannel[ulChannelId].cafilter[ulFilterId].handle))
			{
				//printf("\n[TRDRV_DEMUX_FreeFilter] <<<<<<<<<<<OK>>>>>>>>>>>>\n");
				app_porting_ca_demux_release_filter(filterID, TRUE);
				memset(&(s_caChannel[ulChannelId].cafilter[ulFilterId]), 0x00, sizeof(ca_filter_t));
				break;
			}
		}
	}

	TRCA_DMX_UNLOCK;
	return 0;
}

U8* TRDRV_DEMUX_GetSectionData(U32 ulChannelId, OUT U32 *pulSectionSize)
{
	int i = 0;
	U8 *pData = NULL;
	
	//printf("\nTRDRV_DEMUX_GetSectionData ulChannelId=%ld \n",ulChannelId);
	TRCA_DMX_LOCK;
	
	if (ulChannelId >= CHANNEL_NUM_MAX)
	{
		printf("\n[TRDRV_DEMUX_GetSectionData] error CHANNEL_NUM_MAX!!!\n");
		TRCA_DMX_UNLOCK;
		return 0;
	}
	
	if (s_caChannel[ulChannelId].usedStatus == 0)
	{
		printf("\n[TRDRV_DEMUX_GetSectionData] error usedStatus=0!!!\n");
		TRCA_DMX_UNLOCK;
		return 0;
	}	
	
	for (i = 0; i < MAX_FILTER_COUNT; i++)
	{
		if ((s_caChannel[ulChannelId].pSectionLength[i] != 0) 
			&& (s_caChannel[ulChannelId].FetchDataFlag[i] == 0))
		{
			if (pulSectionSize)
			{
				*pulSectionSize = s_caChannel[ulChannelId].pSectionLength[i];
			}
			
			s_caChannel[ulChannelId].FetchDataFlag[i] = 1;
			pData = s_caChannel[ulChannelId].pSectionData[i];
			break;
		}
	}
	
	TRCA_DMX_UNLOCK;
	return pData;
}

void  TRDRV_DEMUX_FreeSectionData(U32 ulChannelId, U32 ulSectionSize)
{
	int i = 0;
	
	//printf("\nTRDRV_DEMUX_FreeSectionData ulChannelId=%ld ,ulSectionSize=%ld\n",ulChannelId,ulSectionSize);
	TRCA_DMX_LOCK;

	if (ulChannelId >= CHANNEL_NUM_MAX)
	{
		printf("\n[TRDRV_DEMUX_FreeSectionData] error CHANNEL_NUM_MAX!!!\n");
		TRCA_DMX_UNLOCK;
		return;
	}
	
	if (s_caChannel[ulChannelId].usedStatus == 0)
	{
		printf("\n[TRDRV_DEMUX_FreeSectionData] error usedStatus=0!!!\n");
		TRCA_DMX_UNLOCK;
		return;
	}	
	
	for (i = 0; i < MAX_FILTER_COUNT; i++)
	{
		if ((s_caChannel[ulChannelId].pSectionLength[i] == ulSectionSize) 
			&& (s_caChannel[ulChannelId].FetchDataFlag[i] == 1))
		{
			s_caChannel[ulChannelId].FetchDataFlag[i] = 0;
			s_caChannel[ulChannelId].pSectionLength[i] = 0;
			break;
		}
	}
	
	TRCA_DMX_UNLOCK;
	return;
}

U32   TRDRV_DESC_OpenDescrambler(U16 wDemuxId)
{
	int ret = 0;
	
	ret = dDescrambleCounter;
	dDescrambleCounter = (dDescrambleCounter+1) % DESCRAMBLE_NUM_MAX;
	
	memset(s_evenKey, 0xFF, sizeof(s_evenKey));
	memset(s_oddKey, 0xFF, sizeof(s_oddKey));
	memset(s_evenKey1, 0xFF, sizeof(s_evenKey1));
	memset(s_oddKey1, 0xFF, sizeof(s_oddKey1));
	
	return ret;
}

void  TRDRV_DESC_CloseDescrambler(U32 ulDescId)
{
	//printf("\n TRDRV_DESC_CloseDescrambler+++\n");
	return;
}

void  TRDRV_DESC_SetDescramblerPid(U32 ulDescId, U16 wPid)
{
	play_scramble_para_t *pPlayparam = NULL;
	
	//printf("\n TRDRV_DESC_SetDescramblerPid ulDescId=%ld, ulDescId=0x%x\n",ulDescId,wPid);
	app_tr_cas_api_get_app_scramble_para(&pPlayparam);
	if (pPlayparam)
	{
		if (1 == ulDescId)
		{
			if (pPlayparam->p_ecm_pid_audio == CAS_INVALID_PID)
			{
				w_EcmPid[1] = pPlayparam->p_ecm_pid;
			}
			else
			{
				w_EcmPid[1] = pPlayparam->p_ecm_pid_audio;
			}
			
			if (Apid != wPid)
			{
				app_porting_ca_descramble_set_pid(1, pPlayparam->p_audio_pid);
				Apid = wPid;
			}
		}

		if (0 == ulDescId)
		{
			if (pPlayparam->p_ecm_pid_video == CAS_INVALID_PID)
			{
				w_EcmPid[0] = pPlayparam->p_ecm_pid;
			}
			else
			{
				w_EcmPid[0] = pPlayparam->p_ecm_pid_video;
			}
			
			if (Vpid != wPid)
			{
				app_porting_ca_descramble_set_pid(0, pPlayparam->p_video_pid);
				Vpid = wPid;
			}
		}
	}

	return;
}

void  TRDRV_DESC_SetDescramblerEvenKey(U32 ulDescId, IN U8 *pbEvenKey, U8 bEvenLen)
{
	U16 wPid = CAS_INVALID_PID;
	
	if (ulDescId == 0)
	{
		memcpy(s_evenKey, pbEvenKey, bEvenLen);
		wPid = w_EcmPid[ulDescId];
		app_porting_ca_descramble_set_cw(wPid, s_oddKey,s_evenKey, bEvenLen, 0);
		//CAS_DUMP("Es_oddKey", s_oddKey, 8);
		//CAS_DUMP("Es_evenKey", s_evenKey, 8);
	}
	else if ((ulDescId == 1) && (w_EcmPid[1] != w_EcmPid[0]))
	{
		memcpy(s_evenKey1, pbEvenKey, bEvenLen);
		wPid = w_EcmPid[ulDescId];
		app_porting_ca_descramble_set_cw(wPid, s_oddKey1, s_evenKey1, bEvenLen, 0);
		//CAS_DUMP("s_oddKey1", s_oddKey1, 8);
		//CAS_DUMP("s_evenKey1", s_evenKey1, 8); 
	}
	
	//printf("\n  TRDRV_DESC_SetDescramblerEvenKey ECM wPid (0x%x), ulDescId(%ld).\n", wPid, ulDescId);
	return;
}

void  TRDRV_DESC_SetDescramblerOddKey(U32 ulDescId, IN U8 *pbOddKey, U8 bOddLen)
{
	U16 wPid = CAS_INVALID_PID;
	
	if (ulDescId == 0)
	{
		memcpy(s_oddKey, pbOddKey, bOddLen);
		wPid = w_EcmPid[ulDescId];
		app_porting_ca_descramble_set_cw(wPid, s_oddKey,s_evenKey, bOddLen, 0);
		//CAS_DUMP("Os_oddKey", s_oddKey, 8);
		//CAS_DUMP("Os_evenKey", s_evenKey, 8);
	}
	else if ((ulDescId == 1) && (w_EcmPid[1] != w_EcmPid[0]))
	{	
		memcpy(s_oddKey1, pbOddKey, bOddLen);
		wPid = w_EcmPid[ulDescId];
		app_porting_ca_descramble_set_cw(wPid, s_oddKey1, s_evenKey1, bOddLen, 0);
		//CAS_DUMP("s_oddKey1", s_oddKey1, 8);
		//CAS_DUMP("s_evenKey1", s_evenKey1, 8);
	}
	
	//printf("\n TRDRV_DESC_SetDescramblerOddKey ECM wPid =0x%x,ulDescId=%ld.\n", wPid, ulDescId);
	return;
}

void TRDRV_DESC_Clear_Cw(void)
{
	memset(s_evenKey, 0xFF, sizeof(s_evenKey));
	memset(s_oddKey, 0xFF, sizeof(s_oddKey));
	memset(s_evenKey1, 0xFF, sizeof(s_evenKey1));
	memset(s_oddKey1, 0xFF, sizeof(s_oddKey1));
	
	app_porting_ca_descramble_set_cw(w_EcmPid[0], s_oddKey, s_evenKey, 8, 0);
	
	if (w_EcmPid[1] != w_EcmPid[0])
	{	
		app_porting_ca_descramble_set_cw(w_EcmPid[1], s_oddKey1, s_evenKey1, 8, 0);
	}

	return;
}


