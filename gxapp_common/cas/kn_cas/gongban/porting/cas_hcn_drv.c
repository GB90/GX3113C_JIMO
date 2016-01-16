#ifndef CAS_HCN_DRV_C
#define CAS_HCN_DRV_C


#include "app_common_porting_ca_flash.h"
#include "app_common_porting_ca_demux.h"
#include "app_common_porting_ca_os.h"
#include "app_common_porting_ca_smart.h"
#include "app_common_porting_ca_descramble.h"
#include "app_common_play.h"
#include "app_common_porting_stb_api.h"
#include "gxplayer_url.h"
#include "cas_hcn_Public.h"
#include "app_kn_cas_api_demux.h"
#include "app_kn_cas_api_smart.h"

//INT8 hcncas_debug(char *Format_p, ...)CASDRVFUNCPOSTFIX;


#define MSG_AD_MEQUEUE_MAX_NUM      (10)//最多支持的消息队列个数
#define MEQUEUE_DEPTH               (501) //队列长度
typedef struct _ad_task_mequeue_t
{
	int 	 size;//数据单元大小
	handle_t    m_hMqueueHandle;
}AD_TASK_MQUEUE_T;

AD_TASK_MQUEUE_T        RSADVQueueList[MSG_AD_MEQUEUE_MAX_NUM];
void hcncas_create_signal(UINT32* pSemaphore ,unsigned char  bInitVal)CASDRVFUNCPOSTFIX
{
	handle_t semaphore = 0; 
	if (NULL == pSemaphore)
	{
		CAS_Dbg("BYSTBCA_SemaphoreInit pSemaphore=0x%x \n",(unsigned int)pSemaphore);				
		return ;
	}
    app_porting_ca_os_sem_create(&semaphore,bInitVal);
    *pSemaphore = (handle_t)semaphore;
	return ;
}

void hcncas_send_signal(UINT32* pSemaphore)CASDRVFUNCPOSTFIX
{
	if (NULL == pSemaphore)
		{
			CAS_Dbg("BYSTBCA_SemaphoreSignal pSemaphore=0x%x \n",(unsigned int)pSemaphore);		       	
			return ;
		}
	
	app_porting_ca_os_sem_signal((handle_t)*pSemaphore);
	return ;
}

void hcncas_get_signal(UINT32* pSemaphore)CASDRVFUNCPOSTFIX
{
	if (NULL == pSemaphore)
	{
		CAS_Dbg("BYSTBCA_SemaphoreWait pSemaphore=0x%x \n",(unsigned int)pSemaphore);				
		return ;
	}
		
    app_porting_ca_os_sem_wait((handle_t)*pSemaphore);
	return ;
}

void hcncas_del_signal(UINT32* pSemaphore)CASDRVFUNCPOSTFIX
{
	if (NULL == pSemaphore)
		{
			CAS_Dbg("BYSTBCA_SemaphoreDestory pSemaphore=0x%x \n",(unsigned int)pSemaphore);		       	
			return ;
		}

	app_porting_ca_os_sem_delete((handle_t)*pSemaphore);
	return ;
}

void hcncas_task_sleep(UINT16 time)CASDRVFUNCPOSTFIX
{
	
	app_porting_ca_os_sleep(time);
	return ;
}

/*
	创建一个消息队列
*/
void hcncas_create_msgqe(UINT32 *message_queue_id ,UINT8 *queue_name)CASDRVFUNCPOSTFIX
{
	
    int i = 0;
    int size = sizeof(hcn_msg_data);
    for(i = 0; i< MSG_AD_MEQUEUE_MAX_NUM;i++)
    {
        if(RSADVQueueList[i].m_hMqueueHandle == 0)
        {
            break;
        }
    }

    if( i== MSG_AD_MEQUEUE_MAX_NUM)
    {
        return ;
    }
    if(GXCORE_SUCCESS != GxCore_QueueCreate(&RSADVQueueList[i].m_hMqueueHandle,MEQUEUE_DEPTH,size))
    {
        return ;
    }
    RSADVQueueList[i].size = size;
	
	hcncas_debug("QueueCreate OK, i = %d `````````````QUEUE_size = %d````%d`````` \n",i, RSADVQueueList[i].size,RSADVQueueList[i].m_hMqueueHandle);
	
	*message_queue_id = i;
		
}

/*
	创建一个线程
*/
void hcncas_register_task(char *Name, UINT8 byPriority,void* pTaskFun,void* pParam, UINT32 nStackSize)CASDRVFUNCPOSTFIX
{
		int 		ret;
		handle_t	handle;
		uint32_t priority = GXOS_DEFAULT_PRIORITY;
	
		if ((NULL == Name)||(NULL == pTaskFun))
		{
			CAS_Dbg("BYSTBCA_RegisterTask szName=%s  pTaskFun=0x%x\n",Name,(unsigned int)pTaskFun); 			
			return ;
		}
	
		printf("BYSTBCA_RegisterTask szName=%s\n",Name);
	
		ret = app_porting_ca_os_create_task(Name, &handle, (void*)pTaskFun,
								  pParam,nStackSize, priority);
	
		return ;
}

/*
	往消息队列发送一个消息
*/
void hcncas_send_queue(UINT32 QUEUE_ID,UINT8 *DATA,UINT32 LEN,UINT32 TIME_DELAY)CASDRVFUNCPOSTFIX
{
	int ret;
    //hcncas_debug("QUEUE_ID = %d``````````sd msg  %d %d %d \n",QUEUE_ID,DATA[0],RSADVQueueList[QUEUE_ID].m_hMqueueHandle);
	if(QUEUE_ID>=MSG_AD_MEQUEUE_MAX_NUM|| RSADVQueueList[QUEUE_ID].m_hMqueueHandle==0)
	{
		hcncas_debug("[ERROE!!!]QUEUE_ID`````````````QUEUE_ID = %d````%d``````sd msg \n",QUEUE_ID,RSADVQueueList[QUEUE_ID].m_hMqueueHandle);
		return ;
	}
    ret = GxCore_QueuePut( RSADVQueueList[QUEUE_ID].m_hMqueueHandle, (char*)DATA, LEN,0);
	if( 0 !=  ret)
	{
		hcncas_debug("RSADV_QueueSend error \n");

		return ;
	}
	else
		return ;
}

/*
	从消息队列获得一个消息
*/
INT32 hcncas_receive_queue(UINT32 QUEUE_ID,UINT8 *DATA,UINT32 *LEN,UINT32 TIME_DELAY)CASDRVFUNCPOSTFIX
{
    int ret;
    int32_t     timeout;
	int ID=QUEUE_ID;
	if(ID>=MSG_AD_MEQUEUE_MAX_NUM || RSADVQueueList[ID].m_hMqueueHandle==0)
	{
		return 1;
	}
		 
	//hcncas_debug("RSADV_QueueReceiveTimeout RSADVQueueList[QueueID].m_hMqueueHandle=%d   QueueID=%d ulTimeInMS=%d\n", RSADVQueueList[QueueID].m_hMqueueHandle,QueueID,ulTimeInMS);

#if defined(ECOS_OS)
	timeout = -1;
#elif defined(LINUX_OS)
	timeout = 0;
#endif
    /*
	if(0xFFFFFFFF == TIME_DELAY)
		ret = GxCore_QueueGet(RSADVQueueList[ID].m_hMqueueHandle, (char*)DATA,RSADVQueueList[ID].size,LEN,TIME_DELAY);
	else
		ret = GxCore_QueueGet(RSADVQueueList[ID].m_hMqueueHandle, (char*)DATA,RSADVQueueList[ID].size,LEN,TIME_DELAY);
    */

    ret = GxCore_QueueGet(RSADVQueueList[ID].m_hMqueueHandle, (char*)DATA,RSADVQueueList[ID].size,LEN,timeout);
    if (0 != ret)
	{
		//hcncas_debug("hcncas_receive_queue failed\n");
		return 1;
	}
	else
	{
		//hcncas_debug("RSADV_QueueReceiveTimeout ADV_SUCCESS\n");
		   
		return 0;
	}
}


///////////////////////////////////////////////////////////
void hcncas_memcpy(void *des,void *src,UINT32 LEN)CASDRVFUNCPOSTFIX
{
	memcpy(des,src,LEN);
}
void hcncas_memset(void *des,UINT8 value,UINT32 LEN)CASDRVFUNCPOSTFIX
{
	memset(des, value, LEN);
}
INT32 hcncas_memcmp(void *src1,void *src2,UINT32 LEN)CASDRVFUNCPOSTFIX
{
	return memcmp(src1,src2,LEN);
}

void* hcncas_malloc(UINT32 byBufSize)CASDRVFUNCPOSTFIX
{
    // CA_PRINTF("[CDCAS] CDSTBCA_Malloc\n");
    void* pBuf = NULL;
    pBuf = malloc( byBufSize );
    //CA_ASSERT( pBuf != NULL );

    return pBuf;
}
void  hcncas_free(void* pBuf)CASDRVFUNCPOSTFIX
{
    //CA_PRINTF("[CDCAS] CDSTBCA_Free\n");

    //CA_ASSERT( pBuf != NULL );

    free( pBuf );
}
////////////////////////////////////////////////////////////

UINT8 hcncas_is_email_full(void)CASDRVFUNCPOSTFIX//是否需要邮件接收  //1需要邮件,0,不需要
{

	#if 0
		extern UINT8  Hr_get_isneed_new_mail(void);
		UINT8 ret = 0;
		ret = Hr_get_isneed_new_mail();
		return ret; 	
	#else
	return 0;
	#endif
}

typedef struct channel_info_s{

int pid;
int timeout;
int use;
} Channel_Info_T;

typedef struct filter_info_s{
int use;
} Filter_Info_T;


Channel_Info_T   channel_info_[32];
Filter_Info_T   filter_info[32];

void channel_filter_init(void)
{
	memset(&channel_info_[0],0,32*sizeof(Channel_Info_T));
	memset(&filter_info[0],0,32*sizeof(Filter_Info_T));
}

//超时没有作用。当前。
cas_channel_id hcncas_allocatechannel(UINT16 PID,UINT8 wait_seconds)CASDRVFUNCPOSTFIX
{
	int i;
	for(i=0;i<32;i++)
	{
		if(channel_info_[i].use==1&&channel_info_[i].pid==PID)
		{
			channel_info_[i].timeout=wait_seconds;
			return i;
		}

	}

	
	for(i=0;i<32;i++)
	{
		if(channel_info_[i].use==0)
		{
			channel_info_[i].pid=PID;
			channel_info_[i].timeout=wait_seconds;
			channel_info_[i].use=1;
			return i;
		}

	}
	printf("********%s FAILURE!!!**************\n",__FUNCTION__);
	return INVALID_FILTER_ID;
}

INT8 hcncas_freechannel(cas_channel_id channel_id)CASDRVFUNCPOSTFIX
{
	channel_info_[channel_id].use=0;
	return  0;
}
INT8 hcncas_startchannel(cas_channel_id channel_id)CASDRVFUNCPOSTFIX
{
	return 0;
}

cas_filter_id hcncas_allocatefilterid(void)
{
	int i;
	for(i=0;i<32;i++)
	{
		if(filter_info[i].use==0)
		{
			filter_info[i].use=1;
			return i;
		}

	}
	printf("********%s FAILURE!!!**************\n",__FUNCTION__);
	return INVALID_FILTER_ID;
}

INT8 hcncas_freefilterid(cas_filter_id filter_id)
{
	filter_info[filter_id].use=0;
	return  0;
}

cas_filter_id hcncas_allocatefilter(unsigned char byReqID,cas_channel_id channel_id, hcn_restrict* MASK)CASDRVFUNCPOSTFIX//mask??
{
		ca_filter_t filter = {0};
		unsigned char  repeatFlag = TRUE;
		uint8_t 	match[18] = {0,};
		uint8_t 	mask[18] = {0,};
		uint16_t	 i=0;
		uint8_t * pbyFilter=MASK->value;
		uint8_t * pbyMask=MASK->mask;
		int byLen;
		int ret;

		byLen=	MASK->mask_len;
		
	
		if(pbyFilter == NULL || pbyMask == NULL )
		{
			return INVALID_FILTER_ID;
		}
	
		repeatFlag = FALSE;
	
		for (i = 0; i< byLen; i++)
		{
			if (1!= i||2!= i)
			{
				match[i]	 = pbyFilter[i];
				mask[i] = pbyMask[i];
			}
			
		}
		DUMP(DEMUX,INFO,byLen,pbyFilter,"%x");
		DUMP(DEMUX,INFO,byLen,pbyMask,"%x");
    
		filter.byReqID = hcncas_allocatefilterid();//byReqID;//
		filter.crcFlag = FALSE;
		filter.filterLen = byLen;
		memcpy(filter.match,match,byLen);
		memcpy(filter.mask,mask,byLen);
		filter.nWaitSeconds = 10;//channel_info_[channel_id].timeout;
		filter.pid = channel_info_[channel_id].pid;
		filter.equalFlag = TRUE,
		filter.repeatFlag = repeatFlag;
		filter.Callback = app_kn_cas_filter_notify;
		filter.timeOutCallback = app_kn_cas_filter_timeout_notify;
		if(1==app_porting_ca_demux_start_filter(&filter))
		{
			ret=filter.byReqID;
		}
		else
		{
			ret = INVALID_FILTER_ID;
		}
		return ret;
	
}


extern int n_switch_channel_fg;


UINT8  hcncas_freefilter(cas_filter_id filter_id)CASDRVFUNCPOSTFIX
{
		uint32_t i = 0;
		unsigned char  releaseChannel = TRUE;
		ca_filter_t sfilter = {0};
	
		/*
		* 查找对应channel id , filter id
		*/
		
	//	CAS_Dbg("CDSTBCA_ReleasePrivateDataFilter  byReqID=0x%x,  wPid=0x%x \n",
	//		byReqID,wPid);
	
		for (i = 0; i< MAX_FILTER_COUNT;i++)
		{
			app_porting_ca_demux_get_by_filter_id(i,&sfilter);
			if (( 0 != sfilter.usedStatus)&&(0 != sfilter.handle)&&(0 != sfilter.channelhandle)&&(filter_id == sfilter.byReqID))
			{
				  releaseChannel = TRUE;
				  app_porting_ca_demux_release_filter( i, releaseChannel);
				  hcncas_freefilterid(filter_id);
				  return 0;
	
			}
			
		}
		for (i = 0; i< MAX_FILTER_COUNT;i++)
		{
			app_porting_ca_demux_get_by_filter_id(i,&sfilter);
			if(sfilter.byReqID==filter_id)
			{
				printf("free failure %d,%x,%x,%d,%d\n",sfilter.usedStatus,sfilter.handle,sfilter.channelhandle,sfilter.byReqID,filter_id);
				return 1;

			}
			
		}
	
		{

			printf("\n\n\n********************hcncas_freefilter filter_id %d no find !!!!!!!!!!!!************\n\n\n",filter_id);

		}
	
		return 1;
}
extern int Ott_Entitle;
void hcncas_setcw(UINT8 *pbyOddKey,UINT8 *pbyEvenKey,UINT16 ECM_PID)CASDRVFUNCPOSTFIX
{
    /*
    CAS_Dbg("[ECM_PID]:%d\n",ECM_PID);
    CAS_DUMP("[OddKey] ->", pbyOddKey, 8);
    CAS_DUMP("[EvenKey] ->", pbyEvenKey, 8);
    */
    if(Ott_Entitle==0)
    {
        Ott_Entitle = 1;
    }
	app_porting_ca_descramble_set_cw(ECM_PID,pbyOddKey,pbyEvenKey,8,0);
	
	return;
}
/////////////////////////////////////////
INT32 hcncas_smc_reset(void)CASDRVFUNCPOSTFIX
{
	char pbyATR[256];
	char byLen;
	int ret;
	ret = app_kn_cas_api_smart_reset((uint8_t *) pbyATR,(uint8_t *) &byLen);
	return !ret;
}


UINT8 hcncas_smc_status(void)CASDRVFUNCPOSTFIX//:insert status;0:remove status
{
	return app_porting_ca_smart_status();
}

/*
	写卡函数
*/
uint8_t reply[1024] = {0};

uint8_t sc_cmd[512] = {0};

INT32 hcncas_smc_write(UINT8 *apdu_header, const UINT8 *apdu_body, UINT8 *sw1sw2)CASDRVFUNCPOSTFIX
{	
	uint8_t sc_result;
	uint16_t pwReplyLen;
	if(apdu_header==NULL)return 1;
	if(apdu_body==NULL)return 1;
	memcpy(sc_cmd,apdu_header,5);
	memcpy(sc_cmd+5,apdu_body,apdu_header[4]);
	
	sc_result =  app_porting_ca_smart_sc_apdu(sc_cmd,5+apdu_header[4],reply,&pwReplyLen);
	if (TRUE == sc_result)
	{
		//memcpy((void *)apdu_body, (void *)reply, pwReplyLen-2);
		sw1sw2[0]=reply[pwReplyLen-2];
		sw1sw2[1]=reply[pwReplyLen-1];
		return 0;
	}
	return 1;
}

/*	
	读卡函数
*/
INT32 hcncas_smc_read(UINT8 *apdu_header, UINT8 *apdu_body, UINT8 *sw1sw2 )CASDRVFUNCPOSTFIX
{	
 
    uint8_t sc_result;
	uint16_t pwReplyLen;

	if(apdu_header==NULL)return 1;
	if(apdu_body==NULL)  return 1;
    sc_result =  app_porting_ca_smart_sc_apdu(apdu_header,5,reply,&pwReplyLen);
    if (TRUE == sc_result)
    	{
    		memcpy(apdu_body, reply, pwReplyLen-2);
			sw1sw2[0]=reply[pwReplyLen-2];
			sw1sw2[1]=reply[pwReplyLen-1];
			return 0;
    	}
    return 1;
}
////////////////////////////////////////
void hcncas_get_netid(UINT8 *net_id)CASDRVFUNCPOSTFIX
{
#if 0
	extern char net_work_id[2];
	
	net_id[0] = net_work_id[0];
	net_id[1] = net_work_id[1];
#endif
}


INT8 hcncas_save_icno(UINT8 *src1,UINT8 LEN)CASDRVFUNCPOSTFIX
{
	int offset=0;
	int datalen=LEN;
	app_porting_ca_flash_write_data(offset,(uint8_t *)src1,datalen);
	return 0;
}

INT8 hcncas_read_icno(UINT8 *src1,UINT8 LEN)CASDRVFUNCPOSTFIX
{
    int offset=0;
	unsigned int datalen=LEN;
	app_porting_ca_flash_read_data(offset,(uint8_t *)src1,&datalen);
	return 0;
}



INT32 hcncas_savepin_buffer(UINT8 *src1)
{
	int offset=256;
	int datalen=8;
	app_porting_ca_flash_write_data(offset,(uint8_t *)src1,datalen);
	return 0;
}

INT32 hcncas_getpin_buffer(UINT8* src1)
{
    int offset=256;
	unsigned int datalen=8;
	app_porting_ca_flash_read_data(offset,(uint8_t *)src1,&datalen);
	return 0;
}


INT8 hcncas_save_pack(UINT8 *src1,UINT16 LEN)CASDRVFUNCPOSTFIX
{
	int offset=512;
	int datalen=LEN;
	app_porting_ca_flash_write_data(offset,(uint8_t *)src1,datalen);
	return 0;
}



INT8 hcncas_get_pack(UINT8 *src1,UINT16 LEN)CASDRVFUNCPOSTFIX
{
	int offset=512;
	unsigned int datalen=LEN;
	app_porting_ca_flash_read_data(offset,(uint8_t *)src1,&datalen);
	return 0;
}




char hcncas_debug(char *Format_p, ...)CASDRVFUNCPOSTFIX
{
	static char buf[1024];
	va_list args;
	
	va_start(args,Format_p);
    Vsnprintf(buf,sizeof(buf), Format_p, args);
	va_end(args);
	printf("%s",buf);
    return 0;
}

long hcnca_show_osd(void)CASDRVFUNCPOSTFIX
{
	static hcncas_osd temosd;

	if(hcncas_get_move_osd(&temosd) == HCN_RET_FAILURE)
	{
		printf("osd data err\n");
		return -1;
	}
	app_cas_api_on_event(DVB_CA_ROLLING_INFO,CAS_NAME, &temosd, sizeof(hcncas_osd));
	return 0;
}
extern int KN_BJTS;
UINT8 hcncas_report_status(UINT8* mesg,UINT32 para1,UINT32 para2)CASDRVFUNCPOSTFIX
{
	//由用户填写代码，CA内核调用
	//*DATA只有一个标志，比如说0x30	
	//char buf[50]={0,};
	//uint8_t remain_day=0;
    
	int byMessageType = mesg[0];
	hcncas_debug("hcncas_report_status DATA[0]=%d byMessageType=%d\n",(int)mesg[0],byMessageType);
	if (MSG_POP_COMMON_END+byMessageType+1 >=64)
		return 0;

  	switch(byMessageType)
  	{
		case HCNCAS_SUBTITLE_NOTIFY://osd
			
			hcncas_debug("OSD!!!!osd!!!osd~osd!!osd,,osd,,osd,,OSD! !\n");
			hcnca_show_osd();
			break;
		/*case HCNCAS_DISPLAY_URGE_FEE_NOTIFY:
			//app_kn_cas_show_ca_prompt("URGE_FEE_NOTIFY");
			app_kn_cas_api_get_remain_day(&remain_day);
			//sprintf(buf,"%s%d","授权快到期，还剩余天数",remain_day);
			//app_kn_cas_show_ca_prompt(buf);
			hcncas_debug("还剩余%d天,%d\n",KN_BJTS,remain_day);
			hcncas_debug("授权快到期---\n");
			break;
		case HCNCAS_STOP_URGE_FEE_NOTIFY:
			app_kn_cas_hide_ca_prompt();
			hcncas_debug("clear notiy fee---\n");
			break;	
		case DISP_REMAIN_DAY:
			hcncas_debug("\n1 kenengcas  notify to fullscreen  DATA[0]=%d\n",(int)mesg[0]);
			
			hcncas_debug("还剩余二天\n");
			//sprintf(buf,"%s%d","kn ca report code",type-MSG_POP_COMMON_END-1);
			//app_kn_cas_show_ca_prompt(buf);
			break;*/
		case HCNCAS_DISPALY_FINGER_NOTIFY:
			break;
		case HCNCAS_STOP_FINGER_NOTIFY:
			break;

	
  	}
	if(byMessageType == 6)
		return  0;

    
	if ( TRUE != app_play_get_msg_pop_type_state(MSG_POP_COMMON_END+byMessageType+1))
	{
		app_play_clear_ca_msg();
		app_play_set_msg_pop_type(MSG_POP_COMMON_END+byMessageType+1);
	}
	
	
	return  0;
}


#ifdef HAS_JIAMI_CHIP


extern u8 EPORT_PIO_JMXP_DATA;
extern u8 EPORT_PIO_JMXP_CLC;
extern u8 EPORT_PIO_JMXP_RESET;


#define MAX(x,y) ((x) > (y)) ? (x) : (y)
#define MIN(x,y) ((x) < (y)) ? (x) : (y)
#define ONE_MINUTE    1000
#define DELAY_COM0    5   /*5*/
#define CHIP_STX 	0x02
#define CHIP_ETX 	0x03

 UINT32 PioPowerHandle;
 UINT32 PioSwitchHandle;
 UINT32 PioResetHandle;
 UINT32 PioRxHandle;

  UINT32 PioChipRxHandle = 1;  //数据
 UINT32 PioChipTxHandle =2 ;    //时钟
 UINT32 PioChipPowerHandle = 3;  //电源

	
 void HRPio_Set( UINT32 hdeler)
 {
 	if(hdeler == 1)
 	{
 		PANEL_SET(handle_front, (1<<EPORT_PIO_JMXP_DATA), GXPIO_BIT_HIGHT_LEVEL);
 	}
	else  if(hdeler == 2)
	{
	 	PANEL_SET(handle_front, (1<<EPORT_PIO_JMXP_CLC), GXPIO_BIT_HIGHT_LEVEL);
	}
	else  if(hdeler == 3)
	{
		 PANEL_SET(handle_front, (1<<EPORT_PIO_JMXP_RESET), GXPIO_BIT_HIGHT_LEVEL);
	}
 }

u8 HRPio_Get( UINT32 hdeler)
{
	GXPIO_Data_t whether_key; 

	PANEL_GET(handle_front, &whether_key);
		
	 if(hdeler == 1)
 	{
		return !(whether_key &(1 <<EPORT_PIO_JMXP_DATA ));
 	}
	else  if(hdeler == 2)
	{
		return !(whether_key &(1 << EPORT_PIO_JMXP_CLC));

	}
	else  if(hdeler == 3)
	{
		return !(whether_key &(1 << EPORT_PIO_JMXP_RESET));
	}
}



void HRPio_Clear( UINT32 hdeler)
 {
     // hcncas_debug("  HRPio_Clear  \n");

 	if(hdeler == 1)
 	{
 		PANEL_SET(handle_front, (1<<EPORT_PIO_JMXP_DATA), GXPIO_BIT_LOW_LEVEL);
 	}
	else  if(hdeler == 2)
	{
	 	PANEL_SET(handle_front, (1<<EPORT_PIO_JMXP_CLC), GXPIO_BIT_LOW_LEVEL);
	}
	else  if(hdeler == 3)
	{
		 PANEL_SET(handle_front, (1<<EPORT_PIO_JMXP_RESET), GXPIO_BIT_LOW_LEVEL);
	}
 }

void TimeDelay(int iCount)
{

	u32 delay = iCount*20000;
	while( delay )
	{
		delay--;
		//hcncas_debug("d");
		if(delay == 0)
			break;
	}
	
	return;
}



UINT8 HRChip_Send_Byte(UINT8 Send_data) 
{
	int i;
		 
	for(i=7;i>=0;i--)
	{
		HRPio_Clear(PioChipTxHandle);		//时钟线

		if( (Send_data>>i)&0x01 )
		{
			HRPio_Set(PioChipRxHandle);
		}
		else
		{
			HRPio_Clear(PioChipRxHandle);		//数据线			
		}
		TimeDelay(DELAY_COM0);
		
		GXWDT_FeedDog(WDTHandle);

		//OSTimeDly(800);
		
		HRPio_Set(PioChipTxHandle);	//时钟线
		TimeDelay(DELAY_COM0);


	}
		
	HRPio_Set(PioChipRxHandle);

	HRPio_Clear(PioChipTxHandle);		//时钟线

	



	return TRUE;
}


void HRChip_Reset() 
{
	TimeDelay(200);
	TimeDelay(200);
	TimeDelay(200);
	TimeDelay(200);
	     hcncas_debug("  HRChip 设置低电平\n" );

	HRPio_Clear(PioChipPowerHandle);
	TimeDelay(200);
	TimeDelay(200);
	TimeDelay(200);
	TimeDelay(200);
	     hcncas_debug("  HRChip 设置高电平\n" );

	HRPio_Set(PioChipPowerHandle);
	TimeDelay(200);
	TimeDelay(200);
	TimeDelay(200);
	TimeDelay(200);
}

UINT8 HRChip_Recv_Byte(void) 
{

	int i;
	UINT8 datatemp=0,pioget;
	
	for(i=7;i>=0;i--)
	{
	
		
		
		HRPio_Set(PioChipTxHandle);	//时钟线

		TimeDelay(DELAY_COM0);

	
										//数据线			
		pioget = HRPio_Get(PioChipRxHandle);
		//hcncas_debug(" HRChip_Recv_Byte pioget:%x\n",(int)pioget);
		datatemp |= (pioget <<i);//读取数据

		GXWDT_FeedDog(WDTHandle);

		
		HRPio_Clear(PioChipTxHandle);		//时钟线
	
		TimeDelay(DELAY_COM0);

	}
//	HRPio_Set(PioChipRxHandle);

      // hcncas_debug("  HRChip_Recv_Byte datatemp = %x  \n" , datatemp);

	return ~datatemp;
}


UINT8 HRChip_Send_Recv (unsigned char *Send_Data,unsigned char *Recv_Data) 
{
//hcncas_debug(" HRChip_Send_Recv \n ");


	 UINT8 Error;
	 int i;
	HRPio_Set(PioChipTxHandle);				//时钟线

	HRPio_Set(PioChipRxHandle);				//数据线
    
	HRPio_Clear(PioChipRxHandle);				//数据线

	TimeDelay(DELAY_COM0);
	HRChip_Send_Byte(CHIP_STX);
	TimeDelay(DELAY_COM0);
	HRChip_Send_Byte(Send_Data[0]);        //命令字
	TimeDelay(DELAY_COM0);
	HRChip_Send_Byte(Send_Data[1]);        //数据长度
	TimeDelay(DELAY_COM0);

	//hcncas_debug(" HRChip_Send_Recv send data 写数据 命令= %x 数据长度=%x \n",Send_Data[0],Send_Data[1] );
	for(i=0;i<(int)Send_Data[1];i++)
	{
		HRChip_Send_Byte(Send_Data[2+i]);	
		TimeDelay(DELAY_COM0);
		//hcncas_debug("%x ", Send_Data[2+i]);
	}
	//hcncas_debug(" \n");

	HRChip_Send_Byte(CHIP_ETX);
	TimeDelay(15);
	keneng_data_io_in();
	TimeDelay(200);
	TimeDelay(50);
	//TimeDelay(200);

	if( HRChip_Recv_Byte()!=CHIP_STX )
	{
			hcncas_debug("  HRChip_Send_Recv send RECV CHIP_STX!=0X02\n");
			return FALSE;
	}
	Recv_Data[0]=HRChip_Recv_Byte();          //命令字
	Recv_Data[1]=HRChip_Recv_Byte();          //数据长度
	//hcncas_debug(" HRChip_Send_Recv send data  读数据 命令= %x 数据长度=%x \n",Recv_Data[0],Recv_Data[1] );

	for(i=0;i<(int)Recv_Data[1];i++)
	{
		Recv_Data[2+i]=HRChip_Recv_Byte();          //数据长度	
		hcncas_debug("%x ", Recv_Data[2+i]);
	}
	if( HRChip_Recv_Byte()!=CHIP_ETX )
	{
			hcncas_debug("RECV CHIP_ETX!=0X03");
			return FALSE;
	}
  	keneng_data_io_out();
	HRPio_Set(PioChipTxHandle);				//时钟线
	HRPio_Set(PioChipRxHandle);				//数据线
	
    return TRUE;
}



//芯片读写函数
INT32 hcncas_chip_read(UINT8 *send_data, UINT8 *recv_data )CASDRVFUNCPOSTFIX
{	
		INT32 ret;
	UINT8 card_state = 0;

	
    gxos_scheduler_lock();
    gxos_interrupt_disable();
	ret = HRChip_Send_Recv(send_data,recv_data);
gxos_interrupt_enable();
    gxos_scheduler_unlock();
	
	if(ret != TRUE)
	{
		HRChip_Reset();
		hcncas_debug("KN-- reset chip----------\n");
			
    gxos_scheduler_lock();
    gxos_interrupt_disable();
		ret = HRChip_Send_Recv(send_data,recv_data);
		 gxos_interrupt_enable();
    gxos_scheduler_unlock();
	
		if(ret != TRUE)
		{
			hcncas_debug("KN-- RCVE_SEND_DATA   ERR\n");
			return -1 ;
		}
	}
	return 0 ;
	
}

#endif

#endif
