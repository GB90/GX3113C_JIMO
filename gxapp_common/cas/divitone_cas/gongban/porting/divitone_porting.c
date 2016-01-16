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
#include "DiviGuardCa.h"
#include "app_common_porting_ca_flash.h"
#include "app_common_porting_ca_demux.h"
#include "app_common_porting_ca_os.h"
#include "app_common_porting_ca_smart.h"
#include "app_common_porting_ca_descramble.h"
#include "app_common_porting_stb_api.h"
#include "app_common_play.h"
#include "app_divitone_cas_api_demux.h"
#include "app_divitone_cas_api_pop.h"
#include "app_divitone_cas_api_smart.h"
//#include "app_divitone_api_lock_service.h"
#include "gxprogram_manage_berkeley.h"
#include "app_common_prog.h"
#include "gxplayer_url.h"
#include "kernelcalls.h"

static divitone_cas_rolling_message_st rollingmsg = {0};
static divitone_cas_email_st emailflag= {0};
static divitone_cas_active_child_st active_child= {0};

static INT32 sem_flags=0;

typedef struct _ca_semphore
{
	UINT32 semId;
	UINT32 Count;
	UINT32 WaitMode;

}CA_SEMPHORE;

CA_SEMPHORE ca_semphore[32];
gx_mutex_t	ca_semphore_mutex[32];
gx_mutex_t	get_ca_sem_mutex;

UINT8 DIVI_SC_Get_CardState(void)
{
//	CAS_Dbg("%s\n",__FUNCTION__);
	uint8_t ret;
	ret = app_porting_ca_smart_status();
	if(ret == TRUE)
		return 0x01;
	else
		return 0x00;
}

INT32 DIVI_SC_DRV_Initialize(void)
{
	CAS_Dbg("%s\n",__FUNCTION__);
	return SUCCESS;
}
/*
INT32  DIVI_SC_ValidateATR(UINT8 * pbNewATR,UINT16 wLen)
{
	CAS_Dbg("%s\n",__FUNCTION__);
	return SUCCESS;
}
*/


/* 释放私有数据过滤器 */
void Divitone_ReleasePrivateDataFilter( uint8_t  byReqID,uint16_t wPid )
{
	uint32_t filterId = 0;
	bool releaseChannel = TRUE;
	ca_filter_t sfilter = {0};

	/*
	* 查找对应channel id , filter id
	*/
	
//	CAS_Dbg("Divitone_ReleasePrivateDataFilter  byReqID=0x%x,  wPid=0x%x \n",
//		byReqID,wPid);

	for (filterId = 0; filterId< MAX_FILTER_COUNT;filterId++)
	{
		app_porting_ca_demux_get_by_filter_id(filterId,&sfilter);
		if (( 0 != sfilter.usedStatus)&&(0 != sfilter.handle)&&(0 != sfilter.channelhandle)
			&&(byReqID == sfilter.byReqID))
		{
			if (0x10 == wPid)
			{
				releaseChannel = FALSE;
			}
			else
			{
				releaseChannel = TRUE;
			}

			app_porting_ca_demux_release_filter( filterId, releaseChannel);
			return ;
		}
	}
	return ;
}

void  DIVI_EventHandle(UINT32 event,UINT32 param1,UINT32 param2,UINT32 param3)
{
	CAS_Dbg("%s	event=%ld	param1=%ld		param2=%ld	param3=%ld\n",__FUNCTION__,event,param1,param2,param3);
	UINT8 ucMessageType_new = 0;
	switch(event)
	{
		case 0x01:
			rollingmsg.from = 0;
			rollingmsg.wStatus = 1;
			rollingmsg.param1 = param1;
			rollingmsg.param2 = param2;
			app_cas_api_on_event(DVB_CA_ROLLING_INFO,CAS_NAME, &rollingmsg, sizeof(divitone_cas_rolling_message_st));
			break;
		case 0x02:
			ucMessageType_new = 0;
			break;
		case 0x03:
			ucMessageType_new = 1;
			break;
		case 0x04:
			emailflag.wStatus = 1;
			app_cas_api_on_event(DVB_CA_EMAIL_INFO,CAS_NAME, &emailflag, sizeof(divitone_cas_email_st));
			break;
		case 0x05:
			ucMessageType_new = 2+param1;
			break;
		case 0x06:
			if(param1 == 0x09)
				ucMessageType_new = 7+6;
			else					
				ucMessageType_new = 7+param1;
			break;
		case 0x07:
			ucMessageType_new = 14+param1;
			break;
		case 0x08:
			ucMessageType_new = 16+param1;
			break;	
		case 0x09:
			break;	
		case 0x0a:
			ucMessageType_new = 18+param1;
			break;	
		case 0x0b:
			ucMessageType_new = 21;
			break;	
		case 0x0c:
			active_child.param1 = param1;
			app_cas_api_on_event(DVB_CA_MOTHER_CARD_INFO,CAS_NAME, &active_child, sizeof(divitone_cas_active_child_st));
			break;
		case 0x0d:
			ucMessageType_new = 22;
			break;	
		case 0x0e:
			ucMessageType_new = 23;
			break;				
		default:
			break;
	}

	if (MSG_POP_COMMON_END+ucMessageType_new+1 >=64)
			return ;

	if ( TRUE != app_play_get_msg_pop_type_state(MSG_POP_COMMON_END+ucMessageType_new+1))
		{
			app_play_clear_ca_msg();
			app_play_set_msg_pop_type(MSG_POP_COMMON_END+ucMessageType_new+1);
		}
	return;
}

INT32  DIVI_SC_DRV_ResetCard(UINT8 bCardNumber,UINT8 * ATRBuf,UINT8 BufferLen,UINT8 * RLength)
{
	CAS_Dbg("%s	bCardNumber=%d	BufferLen=%d\n",__FUNCTION__,bCardNumber,BufferLen);
	uint8_t ret;
	ret = app_divitone_cas_api_smart_reset(ATRBuf,RLength);
	CAS_DUMP("ATR",ATRBuf,*RLength);
	if(ret == 1)
		return SUCCESS;
	else
		return FAILTURE;
}


INT32 DIVI_SC_DRV_SendDataEx(UINT8 bCardNumber,UINT8 bLength,
	                                                      UINT8 * pabMessage,
	                                                      UINT8 * pabResponse,
	                                                      UINT8 RBufferLen,
	                                                      UINT8 * bRLength,
	                                                      UINT8 * pbSW1,
	                                                      UINT8 * pbSW2)
{
//	CAS_Dbg("%s	bCardNumber=%d	bLength=%d	RBufferLen=%d\n",__FUNCTION__,bCardNumber,bLength,RBufferLen);
	uint8_t reply[1024] = {0};
	uint8_t sc_result;
	uint16_t ReceiveLen;

	sc_result =  app_porting_ca_smart_sc_apdu(pabMessage,bLength,reply,&ReceiveLen);
	if (TRUE == sc_result)
	{
	 	if (ReceiveLen >= 2)
 		{
 			*bRLength = ReceiveLen-2;
	 		memcpy(pabResponse, reply, *bRLength);
			*pbSW1 = reply[ ReceiveLen-2];
			*pbSW2 = reply[ ReceiveLen-1];
 		}
		else
		{
			*bRLength = 0;
			CA_FAIL("DIVI_SC_DRV_SendDataEx 1 *bRLength = 0\n");
			return FALSE;
		}	
	}
	else
 	{
 		*bRLength = 0;
	 	CA_FAIL("DIVI_SC_DRV_SendDataEx 2 *bRLength = 0\n");
 		return FALSE;
 	}
#if 0
	CAS_DUMP("[SC] pabMessage", pabMessage, bLength);
	CAS_DUMP("[SC] pabResponse", pabResponse, *bRLength);
	CAS_Dbg("[SC] SW	0x%02x	0x%02x\n",*pbSW1,*pbSW2);
#endif
	return TRUE;
}

UINT16 DIVI_GetCurr_ServiceID(void)
{
	GxBusPmDataProg Prog={0};
    	uint32_t pos=0;
	UINT16 service_id = 0;

	if (app_prog_get_num_in_group()>0)
	{	
		app_prog_get_playing_pos_in_group(&pos);
		GxBus_PmProgGetByPos(pos,1,&Prog);
		service_id = Prog.service_id;
		return service_id;
	}
	return 0;	
}

UINT16 DIVI_GetCurr_EmmID(void)
{
	return app_divitone_cas_api_get_emm_pid();	
}

UINT16 DIVI_GetCurr_EcmID(void)
{
	return app_divitone_cas_api_get_ecm_pid();	
}
INT32 DIVI_TableStart(UINT8 bRequestID,
                                                   const UINT8 * pbFilterMatch,
                                                   const UINT8 * pbFilterMask,
	                                            UINT8 bLen,
	                                            UINT16 wPid,
	                                            UINT8 bWaitTime)
{
	CAS_Dbg("%s,%d,%d 0x%x\n",__FUNCTION__,bRequestID,bLen,wPid);
	/*if ((CA_STB_FILTER_2 == bRequestID)||(CA_STB_FILTER_3 == bRequestID))
	{
		return SUCCESS;
	}*/
	CAS_DUMP("match:",pbFilterMatch,bLen);
	CAS_DUMP("mask:",pbFilterMask,bLen);
	if ( wPid > 0 && wPid < 0x1fff )
	{
		ca_filter_t filter = {0};
		bool repeatFlag = TRUE;
		uint8_t 	match[18] = {0,};
		uint8_t 	mask[18] = {0,};
		uint16_t	 i=0;
	
		if(pbFilterMatch == NULL || pbFilterMask == NULL)
		{
			return FAILTURE;
		}

		if (CA_STB_FILTER_4 == bRequestID)
		{
			repeatFlag = FALSE;
		}

		//		printf("iRequestID=0x%x wPid=0x%x bWaitTime=%d\n",iRequestID,wPid,bWaitTime);
		for (i = 0; i< bLen; i++)
		{
			if (i == 0)
			{
				match[i] = pbFilterMatch[i];
				mask[i] = pbFilterMask[i];
			}
			else
			{
				match[i+2] = pbFilterMatch[i];
				mask[i+2] = pbFilterMask[i];
			}
		}

		Divitone_ReleasePrivateDataFilter(bRequestID,wPid);

		/*		if (FALSE == repeatFlag)
				{
				for (i = MSG_POP_COMMON_END+SC_NO_CARD+1;i<=MSG_POP_COMMON_END+SC_CARDENTITLEREMAINTDAY;i++)
				{
				if (TRUE == app_play_get_msg_pop_type_state(i))
				{
				GxCore_ThreadDelay(20); 
				break;
				}
				}
				}
				*/	
		filter.byReqID = bRequestID;
		filter.crcFlag = TRUE;
		//       filter.crcFlag = FALSE;
		filter.filterLen = bLen+2;
		memcpy(filter.match,match,bLen+2);
		memcpy(filter.mask,mask,bLen+2);
		if (0 != bWaitTime)
		{
			if (bWaitTime < 1000)
				filter.nWaitSeconds = 1;
			else
				filter.nWaitSeconds = bWaitTime/1000;
		}
		else
		{
			filter.nWaitSeconds = bWaitTime;	
		}
		filter.pid = wPid;
		filter.equalFlag = TRUE;
		filter.repeatFlag = repeatFlag;
		filter.Callback = app_divitone_cas_filter_notify;
		filter.timeOutCallback = app_divitone_cas_filter_timeout_notify;
		app_porting_ca_demux_start_filter(&filter);		
	}
	else
	{
		Divitone_ReleasePrivateDataFilter(bRequestID, wPid);
	}

	return SUCCESS;
}

void DIVI_SetCW(UINT16 wEcmPid,const UINT8 * szOddKey,const UINT8 * szEvenKey,
		UINT8 bKeyLen,UINT8 bReservedFlag)
{
	CAS_Dbg("%s\n",__FUNCTION__);
	CAS_DUMP("[odd CW]->", szOddKey, bKeyLen);
	CAS_DUMP("[even CW]->", szEvenKey, bKeyLen);
	app_porting_ca_descramble_set_cw(wEcmPid,szOddKey,szEvenKey,bKeyLen,bReservedFlag);
	return ;
}

BOOL DIVI_GetBuffer(UINT32 * lStartAddr,UINT32 * lSize)
{
	CAS_Dbg("%s\n",__FUNCTION__);
	*lStartAddr = 0x100;
	*lSize = 0x10000;
	return TRUE;
}

BOOL DIVI_Get_STBToCardSuit_Buffer(UINT32 *lStartAddr,UINT32 * lSize)
{
	CAS_Dbg("%s\n",__FUNCTION__);
	*lStartAddr = 0x10000 + 0x100;
	*lSize = 0x100;
	return TRUE;
}

BOOL  DIVI_ReadBuffer(UINT32 lStartAddr,UINT8 *pData,INT32 nLen)
{
	CAS_Dbg("%s\n",__FUNCTION__);
	uint8_t ret;
	uint32_t read_nLen = nLen;
	if(pData == NULL)
		return FALSE;
	ret = app_porting_ca_flash_read_data(lStartAddr,(uint8_t*)pData,&read_nLen);
	if(ret)
		return TRUE;
	else
		return FALSE;
}

BOOL DIVI_WriteBuffer(const UINT32 lStartAddr,const UINT8 *pData,INT32 nLen)
{
	CAS_Dbg("%s\n",__FUNCTION__);
	uint8_t ret;
	if(pData == NULL)
		return FALSE;
	ret = app_porting_ca_flash_write_data(lStartAddr,(uint8_t*)pData,nLen);
	if(ret)
		return TRUE;
	else
		return FALSE;
}


INT32 DIVI_OSPSCTaskCreate(char name[],UINT32 stackSize,
		void (*entryPoint)(void *),
		INT32 priority,
		UINT32 arg1,
		UINT32 arg2,
		UINT32 * taskId)
{
	int         ret;
	handle_t    handle;

	if ((NULL == name)||(NULL == entryPoint))
	{
		return FAILTURE;
	}

	CAS_Dbg("DIVITONE_STBCA_RegisterTask szName=%s	stackSize=%ld	priority=%ld	arg1=%ld\n",name,stackSize,priority,arg1);

	ret = app_porting_ca_os_create_task(name, &handle, entryPoint,
			NULL, 1024*20, GXOS_DEFAULT_PRIORITY);

	*taskId = handle;

	if (ret != GXCORE_SUCCESS)
	{
		CA_FAIL("DIVITONE_STBCA_RegisterTask return error = 0x%x\n", ret);
		return FAILTURE;
	}

	return SUCCESS;
}


INT32 DIVI_OSPCaCoreTaskCreate(char name[],UINT32 stackSize,
		void (*entryPoint)(void *),
		INT32 priority,
		UINT32 arg1,
		UINT32 arg2,
		UINT32 * taskId)
{
	int         ret;
	handle_t    handle;

	if ((NULL == name)||(NULL == entryPoint))
	{
		return FAILTURE;
	}

	CAS_Dbg("DIVITONE_STBCA_RegisterTask szName=%s	stackSize=%ld	priority=%ld	arg1=%ld\n",name,stackSize,priority,arg1);

	ret = app_porting_ca_os_create_task(name, &handle, entryPoint,
			NULL, 1024*20, GXOS_DEFAULT_PRIORITY);

	*taskId = handle;

	if (ret != GXCORE_SUCCESS)
	{
		CA_FAIL("DIVITONE_STBCA_RegisterTask return error = 0x%x\n", ret);
		return FAILTURE;
	}

	return SUCCESS;
}

INT32 DIVI_OSPOSDTaskCreate(char name[],UINT32 stackSize,
		void (*entryPoint)(void *),
		INT32 priority,
		UINT32 arg1,
		UINT32 arg2,
		UINT32 * taskId)
{
	int         ret;
	handle_t    handle;

	if ((NULL == name)||(NULL == entryPoint))
	{
		return FAILTURE;
	}

	CAS_Dbg("DIVITONE_STBCA_RegisterTask szName=%s	stackSize=%ld	priority=%ld	arg1=%ld\n",name,stackSize,priority,arg1);

	ret = app_porting_ca_os_create_task(name, &handle, entryPoint,
			NULL, 1024*20, GXOS_DEFAULT_PRIORITY);

	*taskId = handle;

	if (ret != GXCORE_SUCCESS)
	{
		CA_FAIL("DIVITONE_STBCA_RegisterTask return error = 0x%x\n", ret);
		return FAILTURE;
	}

	return SUCCESS;
}


INT32 DIVI_OSPUserTaskCreate(char name[],UINT32 stackSize,
		void (*entryPoint)(void *),
		INT32 priority,
		UINT32 arg1,
		UINT32 arg2,
		UINT32 * taskId)
{
	int         ret;
	handle_t    handle;

	if ((NULL == name)||(NULL == entryPoint))
	{
		return FAILTURE;
	}

	CAS_Dbg("DIVITONE_STBCA_RegisterTask szName=%s	stackSize=%ld	priority=%ld	arg1=%ld\n",name,stackSize,priority,arg1);

	ret = app_porting_ca_os_create_task(name, &handle, entryPoint,
			NULL, 1024*20, GXOS_DEFAULT_PRIORITY);

	*taskId = handle;

	if (ret != GXCORE_SUCCESS)
	{
		CA_FAIL("DIVITONE_STBCA_RegisterTask return error = 0x%x\n", ret);
		return FAILTURE;
	}

	return SUCCESS;
}

INT32 DIVI_OSPTaskTemporarySleep(UINT32 milliSecsToWait)
{
	app_porting_ca_os_sleep(milliSecsToWait);
	return SUCCESS;
}

void * DIVI_OSPMalloc(UINT32 numberOfBytes)
{
	return app_porting_ca_os_malloc(numberOfBytes);
}

INT32 DIVI_OSPFree(void * ptrToSegment)
{
	CAS_Dbg("%s\n",__FUNCTION__);
	if(ptrToSegment == NULL)
		return FAILTURE;
	app_porting_ca_os_free(ptrToSegment);
	return SUCCESS;
}

void ca_init_mutex(void)
{
	int i;
	CAS_Dbg("[CA] ca_init_mutex******\n");
	gx_mutex_init(&get_ca_sem_mutex);
	for(i=0;i<32;i++)
	{
		gx_mutex_init(&ca_semphore_mutex[i]);
	}
	CAS_Dbg("[CA]ca_init_mutex  END******\n");
	return;
}

INT32 get_ca_semphore(void)
{
	INT32 i;
	INT32 SHIFT=0x01;	

	CAS_Dbg("[CA] IN get_ca_semphore ID: before gxos_mutex_lock******\n");
	gx_mutex_lock(&get_ca_sem_mutex);
	CAS_Dbg("[CA] IN get_ca_semphore ID: after gxos_mutex_lock******\n");

	for(i=0;i<32;i++)
	{
		if(0==(sem_flags&SHIFT))
		{
			sem_flags|=SHIFT;
			gx_mutex_unlock(&get_ca_sem_mutex);
			CAS_Dbg("[CA] get_ca_semphore ID=%ld******\n",i);
			return i;
		}
		else
			SHIFT<<=1;
	}

	gx_mutex_unlock(&get_ca_sem_mutex);

	CAS_Dbg("[CA] get_ca_semphore  Failure******\n");

	return -1;

}

INT32 DIVI_OSPSemaphoreCreate(char name[4],UINT32 initialTokenCount,UINT32 taskWaitMode,UINT32 * semaphoreId)
{
	CAS_Dbg("%s\n",__FUNCTION__);
	UINT32 gotten_id;
	gotten_id=get_ca_semphore();

	if(-1==gotten_id)
	{
		CAS_Dbg("[CA] DIVI_OSPSemaphoreCreate FAILURE*****\n");
		return FAILTURE;
	}

	ca_semphore[gotten_id].semId=gotten_id;
	ca_semphore[gotten_id].Count=initialTokenCount;
	ca_semphore[gotten_id].WaitMode=taskWaitMode;

	*semaphoreId=gotten_id;

	CAS_Dbg("[CA] DIVI_OSPSemaphoreCreate SUCESS ID=%ld******\n",gotten_id);

	return SUCCESS;	

}

INT32 DIVI_OSPSemaphoreGetToken(UINT32 semaphoreId,UINT32 waitMode,UINT32 milliSecsToWait)
{
	//	CAS_Dbg("%s\n",__FUNCTION__);
	UINT32 i;

	i=semaphoreId;

	if(semaphoreId>32)
	{
		CAS_Dbg("[CA] DIVI_OSPSemaphoreGetToken FAILURE(semaphoreId>32)*****\n");
		return FAILTURE;
	}

	//	CAS_Dbg("[CA]   IN DIVI_OSPSemaphoreGetToken ID: before gxos_mutex_lock******\n");
	gx_mutex_lock(&ca_semphore_mutex[i]);
	//	CAS_Dbg("[CA]   IN DIVI_OSPSemaphoreGetToken ID: after gxos_mutex_lock******\n");

	if(ca_semphore[i].Count>0)
	{
		ca_semphore[i].Count=0;
		gx_mutex_unlock(&ca_semphore_mutex[i]);
		//	CAS_Dbg("[CA] DIVI_OSPSemaphoreGetToken SUCCESS*****\n");
		return SUCCESS;
	}	 
	else if(DIVI_TASK_NOWAIT==ca_semphore[i].WaitMode)	 	
	{
		gx_mutex_unlock(&ca_semphore_mutex[i]);
		CAS_Dbg("[CA] DIVI_OSPSemaphoreGetToken DIVI_TASK_NOWAIT  FAILURE*****\n");
		return FAILTURE;
	}
	else if(DIVI_TASK_WAIT==ca_semphore[i].WaitMode&&milliSecsToWait!=0)
	{
		//
		int loop;

		if(milliSecsToWait<10)
		{
			milliSecsToWait=1;
		}
		else
		{
			milliSecsToWait/=10;
		}

		if(milliSecsToWait>=5)
			loop=milliSecsToWait/5;
		else
			loop=1;
		while(1)
		{
			CAS_Dbg("[CA] DIVI_OSPSemaphoreGetToken WAITING....*****\n");
			//gxos_mutex_lock(&ca_semphore_mutex[i]);
			if(ca_semphore[i].Count>0)
			{
				ca_semphore[i].Count=0;
				gx_mutex_unlock(&ca_semphore_mutex[i]);
				CAS_Dbg("[CA] DIVI_OSPSemaphoreGetToken SUCCESS*****\n");
				return SUCCESS;
			}	
			if(loop--==0)
				break;
			gx_mutex_unlock(&ca_semphore_mutex[i]);
			gx_thread_delay(5);	
			gx_mutex_lock(&ca_semphore_mutex[i]);


		}
	}
	else//wait==0 表示永远等着?
	{
		CAS_Dbg("[CA] msg get wait time is %ld!\n",milliSecsToWait);
		while(1)
		{

			//gxos_mutex_lock(&ca_semphore_mutex[i]);
			if(ca_semphore[i].Count>0)
			{
				ca_semphore[i].Count=0;
				gx_mutex_unlock(&ca_semphore_mutex[i]);
				CAS_Dbg("[CA] DIVI_OSPSemaphoreGetToken SUCCESS*****\n");
				return SUCCESS;
			}	
			gx_mutex_unlock(&ca_semphore_mutex[i]);
			gx_thread_delay(5);	
			gx_mutex_lock(&ca_semphore_mutex[i]);


		}
	}

	gx_mutex_unlock(&ca_semphore_mutex[i]);

	return FAILTURE;

}

INT32 DIVI_OSPSemaphoreReturnToken(UINT32 semaphoreId)
{
	//	CAS_Dbg("%s\n",__FUNCTION__);
	UINT32 i;
	i=semaphoreId;

	gx_mutex_lock(&ca_semphore_mutex[i]);

	ca_semphore[i].Count=1;

	gx_mutex_unlock(&ca_semphore_mutex[i]);

	return SUCCESS;	
}

INT32 DIVI_OSPQueueCreate(char name[],UINT32 maxQueueLength,UINT32 taskWaitMode,UINT32 * queueId)
{
	CAS_Dbg("%s	%s\n",__FUNCTION__,name);
	handle_t  queuedata = 0;
	if(GXCORE_SUCCESS==GxCore_QueueCreate(&queuedata,20/*maxQueueLength*/,sizeof(DIVI_QUEUE_MESSAGE)))
	{
		*queueId=(UINT32)queuedata;
		CAS_Dbg("success!size=%d depth=%ld queue=%ld	taskWaitMode=%ld\n",sizeof(DIVI_QUEUE_MESSAGE),maxQueueLength,*queueId,taskWaitMode);
		return SUCCESS;
	}
	else
	{
		CAS_Dbg(" failed !\n");
		return FAILTURE;
	}	
}

INT32 DIVI_OSPQueueGetMessage(UINT32 queueId,DIVI_QUEUE_MESSAGE * message,UINT32 waitMode,UINT32 milliSecsToWait)
{
	//	CAS_Dbg("%s\n",__FUNCTION__);
	uint32_t    sz=0;
	int32_t     timeout;
	int         ret;

#if defined(ECOS_OS)
	timeout = -1;
#elif defined(LINUX_OS)
	timeout = 0;
#endif

	ret = GxCore_QueueGet(queueId, message, sizeof(DIVI_QUEUE_MESSAGE), &sz,timeout);

	if (ret == GXCORE_SUCCESS)
	{
		if (0 == sz )
		{
			CAS_Dbg("DIVI_OSPQueueGetMessage sz = 0...\n");
			return FAILTURE;
		}

		return SUCCESS;
	}
	else
	{
		CAS_Dbg("DIVI_OSPQueueGetMessage Return value is error...\n");
		return FAILTURE;
	}
	return SUCCESS;

}

INT32 DIVI_OSPQueueSendMessage(UINT32 queueId,DIVI_QUEUE_MESSAGE * message)
{
	//	CAS_Dbg("%s\n",__FUNCTION__);
	int   ret ;

	if(message == NULL)
	{
		CAS_Dbg("DIVI_OSPQueueSendMessage Parameter is NULL\n");
		return FAILTURE;
	}

	ret = GxCore_QueuePut(queueId, message, sizeof(DIVI_QUEUE_MESSAGE),0);

	if(ret == GXCORE_SUCCESS )
	{
		return SUCCESS;
	}
	else
	{
		CAS_Dbg("DIVI_OSPQueueSendMessage pipeID=0x%x Error\n", (unsigned int)queueId);
		return FAILTURE;
	}
	return SUCCESS;	
}

UINT32 DBGPrint(const char * fmt,...)
{
	static char buf[250];
	/*	int ret;
		va_list args;
		va_start(args, fmt);
		ret = vsprintf(buf,fmt,args);

		CAS_Dbg(buf);	

		return 1;
		*/
	//return TRUE;

	va_list args;
	int i;

	va_start(args,fmt);
	i = Vsnprintf(buf,sizeof(buf), fmt, args);
	va_end(args);

	printf("%s", buf);
	return TRUE;
}

UINT32 DIVI_GetRandomNumber()
{
	UINT32 randnum;	
	randnum = (UINT32)(rand() * 0xffff);
	return randnum;
}

UINT8 GxShowPPDatas(UINT8 Value)
{
	CAS_Dbg("%s %d\n",__FUNCTION__,Value);
	return 0;	
}



UINT16 DIVI_GetCurr_NetRegionID(void)
{
	CAS_Dbg("%s\n",__FUNCTION__);
	UINT16 netRegion=0xff;
	return netRegion;	
}



BOOL DIVI_Show_Finger(UINT8 Position,UINT8 Duriaon,UINT8 * Finger_Content,UINT8 Content_Length)
{
	CAS_Dbg("%s\n",__FUNCTION__);
	return 0;
}

INT8 DIVI_GetFingerNum()
{
	CAS_Dbg("%s\n",__FUNCTION__);
	return 0;
}


