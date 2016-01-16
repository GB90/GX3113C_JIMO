/*****************************************************************************
* 						   CONFIDENTIAL
*        Hangzhou GuoXin Science and Technology Co., Ltd.
*                      (C)2014, All right reserved
******************************************************************************

******************************************************************************
* File Name :	gos_cas_porting.c
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
#include <stdlib.h>
#include <stdarg.h>
#include <gxcore.h>
#include "GOSCAS_STBInf.h"
#include "GOSSTB_CASInf.h"
#include "app_common_porting_ca_flash.h"
#include "app_common_porting_ca_demux.h"
#include "app_common_porting_ca_os.h"
#include "app_common_porting_ca_smart.h"
#include "app_common_porting_ca_descramble.h"
#include "app_common_play.h"
#include "app_common_prog.h"
#include "app_gos_cas_api_demux.h"
#include "app_gos_cas_api_pop.h"
#include "app_common_porting_stb_api.h"
#include "app_gos_cas_api_smart.h"
#include "gxplayer_url.h"
#include "service/gxplayer.h"
#include "gxmsg.h"
#include "gui_key.h"
#include "gxprogram_manage_berkeley.h"
#include "app_common_play.h"
#include "app_common_lcn.h"
//#include "app_gos_cas_api_lock_service.h"
#include "gui_core.h"
#include "app_common_init.h"


#define FLASH_CA_DATA_A_START_ADDR  (0x10000)
#define FLASH_CA_DATA_SIZE (2*64*1024)
#define MAX_SEARIAL_LEN (17)


static gos_cas_finger_data_st  fingerdata = {0};
static gos_cas_rolling_message_st  rollingmsg = {0};
static gos_cas_email_st emailflag= {0};

uint32_t s_duration = 0;
uint32_t s_ippt = 0;
uint32_t s_ippt_duration = 0;



HRESULT GOSSTBCA_ReleasePrivateDataFilter(uint8_t byReqID, uint16_t wPid)
{
	uint32_t filterId = 0;
	bool releaseChannel = TRUE;
	ca_filter_t sfilter = {0};

	for (filterId = 0; filterId< MAX_FILTER_COUNT;filterId++)
	{
		app_porting_ca_demux_get_by_filter_id(filterId,&sfilter);
		if (( 0 != sfilter.usedStatus)&&(0 != sfilter.handle)&&(0 != sfilter.channelhandle)
			&&(wPid == sfilter.pid)&&(byReqID == sfilter.byReqID))
		{			
			app_porting_ca_demux_release_filter( filterId, releaseChannel);
			break ;
		}
	}

	return GOSCA_OK;

}


/*++

���ܣ�
	CAS���ںʹ���ͨ��ʧ��һ�κ����������Զ���1����ֵ����GOSCA_UART_ERROR_MAX
	����øýӿڡ��ӿں���ʵ�ֿ���λ��ȷ��ATR ��ȷ��

������
-*/
void GOSSTBCA_SetSCReset() 
{
	uint8_t                     len;
    uint8_t               atr[256] = {0};
	int nRetValue = FALSE;

  	CAS_Dbg("[SC] GOSSTBCA_SetSCReset!\n");

    nRetValue = app_gos_cas_api_smart_reset(atr,&len);
	return ;
    	
}

/*++


���ܣ�
	�������ܿ���ǰ״̬

������
	pbySCStatus���������ܿ�״̬��ΪGOSCAS_SC_OUT��GOSCAS_SC_IN����֮һ��
--*/
void GOSSTBCA_GetSCStatus(unsigned char * pbySCStatus) 
{
	if (NULL == pbySCStatus)
	{
	  	CAS_Dbg("GOSSTBCA_GetSCStatus pbyStatus=NULL\n");
		return;
	
	}
    //printf("###status:%d\n",app_porting_ca_smart_status());
    
	if (TRUE == app_porting_ca_smart_status())
		*pbySCStatus = GOSCAS_SC_IN;
	else
		*pbySCStatus = GOSCAS_SC_OUT;
	return ;	
}

/*++

���ܣ�
	�������������С�

������
-*/
void GOSSTBCA_ReStartSTB()  
{
	restart();
	return;
}


/*++


���ܣ�
	��û����з����CAS�Ĵ洢�ռ������ַ�ʹ�С�����ֽ�Ϊ��λ��

������
	ppStartAddr:	     �����з����CAS�Ĵ洢�ռ�Ŀ�ʼ��ַ
	lSize:               ��������з����CAS�Ĵ���ռ�Ĵ�С��
	
����ֵ��
	GOSCA_OK	�ɹ�
	  
˵����
       
--*/
HRESULT GOSSTBCA_GetDataBufferAddr(unsigned int* lSize, unsigned long* pStartAddr) 
{
	if ((NULL == lSize)||(NULL == pStartAddr))
	{
		return GOSCAERR_STB_POINTER_INVALID;
	}
	*pStartAddr = (unsigned long)FLASH_CA_DATA_A_START_ADDR;
	*lSize		 = (unsigned int)FLASH_CA_DATA_SIZE;
	return GOSCA_OK;
}


/*++

���ܣ�
	��ȡ�����ڻ������е���Ϣ

������
	pStartAddr:	    Ҫ��ȡ�Ĵ洢�ռ�Ŀ�ʼ��ַ
	plDataLen:       ����ΪҪ��ȡ�������ֵ
	                ����ʵ�ʶ�ȡ�Ĵ�С
	pData��			����������

����ֵ��
	GOSCA_OK	�ɹ�
	       
--*/
HRESULT GOSSTBCA_ReadDataBuffer(const unsigned long pStartAddr, unsigned int DataLen, unsigned char* pData) 
{
	size_t readSize = (size_t)DataLen;
	uint32_t  Offset = (uint32_t)(pStartAddr-FLASH_CA_DATA_A_START_ADDR);
	if (NULL == pData)
	{
		CA_FAIL(" GOSSTBCA_ReadDataBuffer   failed \n");
		return GOSCAERR_STB_POINTER_INVALID;
	}

	app_porting_ca_flash_read_data(Offset,(uint8_t *)pData,(uint32_t*)&readSize);
	
	return GOSCA_OK;
}

/*++

���ܣ�
	������еĴ洢�ռ�д��Ϣ

������
	lStartAddr:	     Ҫд�Ĵ洢�ռ�Ŀ�ʼ��ַ
	plDataLen:        Ҫд�����ݵĳ���
					 ����Ϊд���ʵ�ʳ���
	pData��			 Ҫд������
	
	                     
����ֵ��
	
˵����
	GOSCA_OK	�ɹ�
       
--*/
HRESULT GOSSTBCA_WriteDataBuffer(const unsigned long pStartAddr, const unsigned int DataLen, const unsigned char* pData)  
{
	size_t writeSize = (size_t)DataLen;
	if (NULL == pData)
	{
		return GOSCAERR_STB_POINTER_INVALID;
	}
	uint32_t  Offset = (uint32_t)(pStartAddr-FLASH_CA_DATA_A_START_ADDR);

	app_porting_ca_flash_write_data(Offset,(uint8_t*)pData,writeSize);

	return GOSCA_OK;
}

HRESULT GOSSTBCA_FlashErase( )
{
	app_porting_ca_flash_erase(0,FLASH_CA_DATA_SIZE);
	return GOSCA_OK;
}


/*++

���ܣ�
	������еı����ڴ���Ϣ

������
	
	                     
����ֵ��
	
˵����
	GOSCA_OK	�ɹ�
       
--*/
HRESULT GOSSTBCA_Data_Back(const unsigned long pStartAddr, const unsigned int DataLen)  
{
	return GOSCA_OK;
}

/*++

���ܣ�
	�����ڴ�

������
	dwSize:		�����ڴ�Ŀռ�
						 
����ֵ��
	�����ڴ�ռ�ĵ�ַ
			
˵����
	   
--*/
unsigned char *GOSSTBCA_malloc( unsigned int dwSize ) 
{
	return app_porting_ca_os_malloc(dwSize);	
}

/*++

���ܣ�
	�ͷ��������ڴ�

������
	*pAddr: 	�����ڴ�ĵ�ַ
						 
����ֵ��
			
˵����
	   
--*/
void GOSSTBCA_free( unsigned char *pAddr ) 
{
	app_porting_ca_os_free(pAddr);
	return ;
}

/*++

���ܣ�
	����ǰ�߳�

������
	dwSleepMicroseconds: 		����
	                     
����ֵ��
		    
˵����
       
--*/
HRESULT GOSSTBCA_Sleep( long dwSleepMicroseconds ) 
{
	app_porting_ca_os_sleep(dwSleepMicroseconds);
	return GOSCA_OK;	
}

/*++

���ܣ�
	��ʼ���ź���,��ʼ�����ź���û���źš�

������
	pSemaphore: 		ָ���ź�����ָ��
	                     
����ֵ��
		    
˵����
       
--*/
HRESULT GOSSTBCA_SemaphoreInit( GOSCA_Semaphore *pSemaphore ) 
{
	handle_t semaphore = 0;
	if (NULL == pSemaphore)
    {
        return GOSCAERR_STB_POINTER_INVALID;
    }
	
    app_porting_ca_os_sem_create(&semaphore,0);

	*pSemaphore = (handle_t)semaphore;
	return GOSCA_OK;
}

HRESULT GOSSTBCA_SemaphoreTimeOutInit( GOSCA_Semaphore *pSemaphore ) 
{
	handle_t semaphore = 0;
	if (NULL == pSemaphore)
    {
        return GOSCAERR_STB_POINTER_INVALID;
    }
	
    app_porting_ca_os_sem_create(&semaphore,0);

	*pSemaphore = (handle_t)semaphore;
	return GOSCA_OK;	
}

/*++

���ܣ�
	�ͷ�һ���ź���

������
	pSemaphore: 		ָ���ź�����ָ��
	                     
����ֵ��
		    
˵����
       
--*/
HRESULT GOSSTBCA_SemaphoreDestory( GOSCA_Semaphore *pSemaphore ) 
{
	if (NULL == pSemaphore)
	{
	    return GOSCAERR_STB_POINTER_INVALID;
	}
	app_porting_ca_os_sem_delete((handle_t)*pSemaphore);

	return GOSCA_OK;	
}

/*++
���ܣ�
	���ź������ź�

������
	pSemaphore: 		ָ���ź�����ָ��
	                     
����ֵ��
		    
˵����
       
--*/
HRESULT GOSSTBCA_SemaphoreSignal( GOSCA_Semaphore *pSemaphore ) 
{
	if (NULL == pSemaphore)
	{
	    return GOSCAERR_STB_POINTER_INVALID;
	}
	app_porting_ca_os_sem_signal((handle_t)*pSemaphore);

	return GOSCA_OK;

}

/*++
���ܣ�
	�ȴ��ź���,�ȴ��ɹ�֮��,�ź���Ϊ���ź�

������
	pSemaphore: 		ָ���ź�����ָ��
	                     
����ֵ��
		    
˵����
       
--*/
HRESULT GOSSTBCA_SemaphoreWait( GOSCA_Semaphore *pSemaphore ) 
{
	if (NULL == pSemaphore)
	{
	    return GOSCAERR_STB_POINTER_INVALID;
	}
	app_porting_ca_os_sem_wait((handle_t)*pSemaphore);
	return GOSCA_OK;

}

HRESULT GOSSTBCA_SemaphoreWait_TimeOut( GOSCA_Semaphore *pSemaphore, unsigned int timeout ) 
{
	if (NULL == pSemaphore)
	{
	    return GOSCAERR_STB_POINTER_INVALID;
	}

    app_porting_ca_os_sem_timeout_wait((handle_t)*pSemaphore,timeout);
	return GOSCA_OK;

}

/*++
���ܣ�
	CASģ���������ע���߳�

������
	szName:				ע����߳�����
	ProcessID:			ע����̵߳�ID
	pTaskFun:			Ҫע����̺߳���	                     

����ֵ��
	GOSCA_OK			�߳�ע��ɹ�
	
˵����
       
--*/
HRESULT GOSSTBCA_RegisterTask(const char* szName,const unsigned long ProcessID, pThreadFunc pTaskFun) 
{
    int         ret;
    handle_t    handle;
    uint32_t priority = GXOS_DEFAULT_PRIORITY;


	if ((NULL == szName)||(NULL == pTaskFun))
	{
		return GOSCAERR_STB_POINTER_INVALID;
	}

/*	if(0 == strcmp(szName,"DVTEcm_DisposeTask"))
	{
		priority = GXOS_DEFAULT_PRIORITY;

	}
	else
	{
		priority = GXOS_DEFAULT_PRIORITY+1;
 	}*/
 	
	priority = GXOS_DEFAULT_PRIORITY;


    ret = app_porting_ca_os_create_task(szName, &handle, (void*)pTaskFun,
                              NULL, 20*1024, priority);

	if (ret != GXCORE_SUCCESS)
	{
		CA_FAIL("GOSSTBCA_RegisterTask return error = 0x%x\n", ret);
		return GOSCA_FAILED;
	}
	
	return GOSCA_OK;

}



/*++

���ܣ�
	���ù���������CAS˽�����ݡ�

������
	byReqID:	�����ձ������š�������Ӧ�ñ�������ݣ����յ�CAS˽�����ݺ����GOSCASTB_StreamGuardDataGot()ʱӦ�ý������ݷ��ء�
	wEcmPID:	��Ҫ���˵�����PID��
	szFilter:	��������ֵ��Ϊһ��8���ֽڵ����飬�൱��8*8�Ĺ�����
	szMask:		�����������룬Ϊһ��8���ֽڵ����飬��8*8�Ĺ�������Ӧ����szMask��ĳһbitλΪ0ʱ��ʾҪ���˵ı��Ӧ��bitλ����Ҫ��szFilter���Ӧ��bitλ��ͬ����szMaskĳһλΪ1ʱ��ʾҪ���˵ı��Ӧ��bitλһ��Ҫ��szFilter���Ӧ��bitλ��ͬ
	byLen:		�������ĳ��ȣ�Ϊ8
	bContiunue:	�Ƿ���������

--*/
HRESULT GOSSTBCA_SetStreamGuardFilter(unsigned char byReqID, unsigned short wPID,
                                                const unsigned char* szFilter,const unsigned char* szMask,
                                                unsigned char byLen, unsigned int bContiunue) 
{
	ca_filter_t filter = {0};
	bool repeatFlag = TRUE;
	uint32_t filterId = 0;
	ca_filter_t* sfilter = NULL;
//	GxTime starttime={0};
//	uint32_t starts;

	if(szFilter == NULL || szMask == NULL )
	{
		return GOSCAERR_STB_POINTER_INVALID;
	}
	// �������������ȫ��ͬ����RESET

	if (TRUE == bContiunue)
	{
		repeatFlag = TRUE;
	}
	else
	{
		repeatFlag = FALSE;
	}


    //printf("regID:%d,pid:%d,masklen:%d\n",byReqID,wPID,byLen);

#if 1
	for (filterId = 0; filterId< MAX_FILTER_COUNT;filterId++)
	{
		sfilter = app_porting_ca_demux_get_by_filter_id(filterId,&filter);
		if (( 0 != filter.usedStatus)&&(0 != filter.handle)&&(0 != filter.channelhandle)
			&&(wPID == filter.pid)&&(byReqID == filter.byReqID)
			&&(byLen == filter.filterLen))
		{
			if ((0 == memcmp(filter.match,szFilter,byLen))&&
				(0 == memcmp(filter.mask,szMask,byLen)))
				{
/*					GxCore_GetTickTime(&starttime);
					starts = starttime.seconds;
					sfilter->nms = starts;
					sfilter->nWaitSeconds = 0;*///nWaitSeconds;
					return GOSCA_OK;
				}
		}
	}
#endif

	GOSSTBCA_ReleasePrivateDataFilter(byReqID,wPID); 

	memset(&filter,0,sizeof(ca_filter_t));
	filter.byReqID = byReqID;
	filter.crcFlag = TRUE;//FALSE/*TRUE*/;
	filter.filterLen = byLen;
	memcpy(filter.match,szFilter,byLen);
	memcpy(filter.mask,szMask,byLen);		
	filter.nWaitSeconds = 0;//nWaitSeconds;
	filter.pid = wPID;
	filter.equalFlag = TRUE;
	filter.repeatFlag = repeatFlag;
	filter.Callback = app_gos_cas_filter_notify;
	filter.timeOutCallback = app_gos_cas_filter_timeout_notify;
	app_porting_ca_demux_start_filter(&filter);

	return GOSCA_OK;
}



/*++

���ܣ�
	CA�����ô˺������ý�����������ǰ���ڼ���һ���ڵ�CW�͸���������

������
	wEcmPID:				CW������ECMPID��
	szOddKey:				��CW������
	szEvenKey:				żCW������
	byKeyLen:				CW�ĳ���
	bTaingControl           true������¼��,false��������¼��

	                     
����ֵ��
       
--*/
HRESULT GOSSTBCA_SetDescrCW(unsigned short wEcmPID,unsigned char byKeyLen,const unsigned char* szOddKey,const unsigned char* szEvenKey,
                             unsigned char bTaingControl) 
{
    //CAS_Dbg("SetDescrCW,pid:%d\n",wEcmPID);
    //CAS_DUMP("[odd CW]->", szOddKey, byKeyLen);
    //CAS_DUMP("[even CW]->", szEvenKey, byKeyLen);
	app_porting_ca_descramble_set_cw(wEcmPID,szOddKey,szEvenKey,byKeyLen,bTaingControl);
	return GOSCA_OK;
}



/*++
���ܣ�
	   ��ӡ������Ϣ

������
	pszMsg:     ������Ϣ����
	                     
����ֵ��
	void

˵����
      
--*/
extern void GOSSTBCA_AddDebugMsg(char *pszMsg) 
{
	printf("[GOS_CA]%s\n",( char * ) pszMsg );
}


/*++
���ܣ�
	ʵ�������ܿ�֮���ͨѶ������7816��׼��
������
	byReaderNo��	��Ӧ�Ķ��������
	byLen:			����Ϊ�����������ֽڵĳ��ȣ����Ϊ�����ֽڳ���
	byszCommand:	�����͵�һ�������ֽ�
	szReply:		���ص����ݣ��ռ䳤�Ⱥ�Ϊ256�ֽ�
	
����ֵ��
      
--*/
HRESULT GOSSTBCA_SCAPDU(unsigned char byReaderNo, unsigned char* pbyLen, unsigned char* byszCommand, unsigned char* byszReply) 
{
    uint8_t reply[1024] = {0};
    uint8_t sc_result;
	uint16_t CommandLen = *pbyLen;
	uint16_t ReplyLen = 0;

    //CAS_DUMP("[GOSSTBCA_SCAPDU] write->", byszCommand, *pbyLen);

	if (*pbyLen<5)
	{
		printf("GOSSTBCA_SCAPDU--->Send Len :%d, Command",*pbyLen);
		return GOSCAERR_STB_UNKNOWN;
	}
    
    //int recvLen = byszCommand[CommandLen-1];
    sc_result =  app_porting_ca_smart_sc_apdu(byszCommand,CommandLen,reply,&ReplyLen);
    if (TRUE == sc_result)
	{
		memcpy(byszReply, reply, ReplyLen);
		*pbyLen = (unsigned char)ReplyLen;
        //CAS_DUMP("[GOSSTBCA_SCAPDU] Read->", byszReply, ReplyLen);
        /*if(ReplyLen != recvLen+2)
        {
            printf("ERROR!!!################recvLen:%d(%x),ReplyLen:%d\n",recvLen,recvLen,ReplyLen);
        }*/
		return GOSCA_OK;
	}
	else
	{
		return GOSCAERR_STB_UNKNOWN;			
	}
	
}


/*------------------------------------ ����1�Ľ��棺�������û�ͨ��ң����ȡ����ʾ�Ľ���--------------------------------*/
/*
���ܣ�
	��ʾ�û����ܹۿ���Ŀ����ʾ��Ϣ����������������Ӧʹ�á�
����:	
	byMesageNo:  Ҫ��ʾ����Ϣ���
	bShowInfo==1:��ʾ����ʾ��Ϣ��Ҫ��ʾ������ʾ��Ϣ��������ʾ��ʾ���ݣ�ǰ�˷������ʼ�����$#OperSvr*�����ʼ�����������ʾ��
	bShowInfo==0:����pString�Ͳ���strlen���ܣ�

--*/
void   GOSSTBCA_ShowPromptMessage(const char byMesageNo, unsigned int duration, unsigned long ippt, bool bShowInfo, unsigned char *pString, unsigned int strlen)
{
	uint8_t messageType = CA_GOS_No_OSDPromt;
    /*
	printf(" GOSSTBCA_ShowPromptMessage byMesageNo=%d duration=%d ippt=0x%lx bShowInfo = %d\n",
		byMesageNo,duration,ippt,bShowInfo);  
    */
	switch(byMesageNo)
	{
		case GOSCA_No_OSDPromt:        			/*��OSD��ʾ*/	
			 messageType = CA_GOS_No_OSDPromt;
			 break;
		case GOSCA_SMART_STOP:					/*���ѱ�����*/
			messageType = CA_GOS_SMART_STOP;
			break;
		case GOSCA_SMART_AREA_NOPAIR:			/*��������ƥ��*/
			messageType = CA_GOS_SMART_AREA_NOPAIR;
			break;
		case GOSCA_SMART_IPPT_SWITCH:			/*IPPT�û���ʾ*/
			messageType = CA_GOS_SMART_IPPT_SWITCH;
			break;				
		case GOSCA_SMART_STB_NOPAIR:			/*���������*/
			messageType = CA_GOS_SMART_STB_NOPAIR;
			break;	
		case GOSCA_SMART_PROG_NOENTITLE:		/*��Ŀ��δ��Ȩ*/
			messageType = CA_GOS_SMART_PROG_NOENTITLE;
			break;					
		case GOSCA_SMART_PROG_NOENTITLE_IPPT:   /*��Ŀ��δ��Ȩ*/
			messageType = CA_GOS_SMART_PROG_NOENTITLE_IPPT;
			break;					
		case GOSCA_SMART_MOTHER_SON_PAIR:		/*��ĸ����������*/
			messageType = CA_GOS_SMART_MOTHER_SON_PAIR;
			break;					
		case GOSCA_SMART_MOTHER_SON_TIMEOUT:	/*��ĸ����Գ�ʱ*/
			messageType = CA_GOS_SMART_MOTHER_SON_TIMEOUT;
			break;				
		case GOSCA_SMART_PAIR_OK:				/*������ӦOK*/
			messageType = CA_GOS_SMART_PAIR_OK;
			break;					
		case GOSCA_SMART_LAST_DAY:				/*�����ʾ����*/
			messageType = CA_GOS_SMART_LAST_DAY;
			s_duration = duration;
			break;				
		case GOSCA_SMART_AREA_LOCK:	 			/*���������� */
			messageType = CA_GOS_SMART_AREA_LOCK;
			break;					
		case GOSCA_SMART_CARD_LOCK: 			/*���ѱ����� */
			messageType = CA_GOS_SMART_CARD_LOCK;
			break;					
		case GOSCA_SMART_IPPT:					/*IPPT��ʾ*/
			messageType = CA_GOS_SMART_IPPT;
			s_ippt_duration = duration;
			s_ippt = ippt; 				
			break;					
		case GOSCA_SMART_LIMIT_WATCH:			/*�������տ�*/
			messageType = CA_GOS_SMART_LIMIT_WATCH;
			break;					
		case GOSCA_SMART_NO_WORK_TIME:			/*�����ڹ���ʱ���*/
			messageType = CA_GOS_SMART_NO_WORK_TIME;
			break;					
		case GOSCA_SMART_OUT:					/*��Ŀ���ţ���忨*/
			messageType = CA_GOS_SMART_OUT;
			break;
		default:
			break;
			//	GOSCA_SMART_UNKNOWN_ERROR,		/*δ֪����*/
		}

	
		if (MSG_POP_COMMON_END+messageType+1 >=64)
			return ;

		if ( TRUE != app_play_get_msg_pop_type_state(MSG_POP_COMMON_END+messageType+1))
		{
			app_play_clear_ca_msg();
			app_play_set_msg_pop_type(MSG_POP_COMMON_END+messageType+1);
		}		

	return;	
}

void   GOSSTBCA_HidePromptMessage(void) 
{
	app_play_clear_ca_msg();

	return;	
}

/*
���ܣ�
	��ʾָ����Ϣ��
����:
	wDuration �����뼶��Ĭ����3���Զ���ʧ
--*/
void	GOSSTBCA_ShowFingerPrinting(unsigned long version, unsigned long dwCardID,unsigned short wDuration) 
{
	fingerdata.version = version;
	fingerdata.dwCardID = dwCardID;
	fingerdata.wDuration = wDuration;
	fingerdata.wStatus = TRUE;							

	app_cas_api_on_event(DVB_CA_FINGER_INFO,CAS_NAME, &fingerdata, sizeof(gos_cas_finger_data_st));

	return;	
}

/*
���ܣ�
	��ʾOSD��Ϣ��������ͬ����ʾ��Ҳ�������첽��ʾ��
����:	
	szOSD��		������ʾ��OSD��Ϣ
	wTimes��	��ʾ�Ĵ���
				
˵����
	�û�����ͨ��ң����ȡ����ǰOSD��ʾ��
--*/
void   GOSSTBCA_ShowOSDMsg(const char* szOSD, unsigned char OSD_len, unsigned long OSD_version, unsigned short wTimes) 
{
		int i = 0;
		int j=0;
	
		printf(" GOSSTBCA_ShowOSDMsg szOSD=%s\n",
			szOSD);  
		if (NULL == szOSD)
		{
		return;
		}
	
		
		memset(&rollingmsg,0,sizeof(gos_cas_rolling_message_st))	;
	
		for (i = 0; i< OSD_len;i++)
		{
			if ((0x0d !=szOSD[i] )&&(0x0a != szOSD[i]))
			{
				rollingmsg.content[j++]=	szOSD[i];
			}
			else
			{
				/*�س����лᱻ����������*/
				printf("szMessage[%d]=0x%x\n",i,szOSD[i]);
			}
		}
	 	rollingmsg.wTimes = wTimes;
		rollingmsg.OSD_version = OSD_version;
		rollingmsg.wStatus = TRUE;
		app_cas_api_on_event(DVB_CA_ROLLING_INFO,CAS_NAME, &rollingmsg, sizeof(gos_cas_rolling_message_st));
	
		return;
}

/*++
���ܣ�
	��ʾ���ʼ��ı�־��
������
	byShow:		GOSCAS_NEW_EAMIL				���ʼ�֪ͨ
				GOSCAS_NEW_EMAIL_NO_ROOM		�����ʼ�����Email�ռ䲻��
				GOSCAS_EMAIL_NONE				û�����ʼ�
--*/
void GOSSTBCA_EmailNotify(unsigned char byShow, unsigned char index) 
{
	CAS_Dbg(" GOSSTBCA_EmailNotify byShow=%d\n",byShow); 
	switch(byShow)
	{
	case GOSCAS_EMAIL_NEW:
		{
			emailflag.wStatus = DVB_CA_EMAIL_NEW;				
		}
		break;
	case GOSCAS_EMAIL_NEW_NO_ROOM:
		{
			emailflag.wStatus = DVB_CA_EMAIL_EXHAUST;				
		}
		break;		
	case GOSCAS_EMAIL_NONE:
		{
			emailflag.wStatus = DVB_CA_EMAIL_FLAG_HIDE;				
		}
		break;
	default:
		{
			emailflag.wStatus = DVB_CA_EMAIL_FLAG_HIDE;				
		}			
		break;
	}

	app_cas_api_on_event(DVB_CA_EMAIL_INFO,CAS_NAME, &emailflag, sizeof(gos_cas_email_st));
	

}

/*++
���ܣ�
	��ȡ���������кš�
������

����ֵ��
	���������кš������Ա���ڿ���ʹ��
--*/
unsigned long GOSSTBCA_GetSerialNo( void ) 
{
    
	char caUser[MAX_SEARIAL_LEN+1]={0};
	app_flash_get_serial_number(0,(char *)caUser,MAX_SEARIAL_LEN);
    printf("GOSSTBCA_GetSerialNo\n");
	return atoi(caUser+12);
}

/*++
���ܣ�
	��ȡ������Ӳ���汾�š�
������

����ֵ��
	������Ӳ���汾�š���������ʱδ��ʹ��
--*/
extern unsigned int convert_version_str(const char *str);
unsigned long GOSSTBCA_GetHardVer(void) 
{
	char *hardwareVer = NULL;
	uint32_t		uHWVer=0;
	
	hardwareVer = app_flash_get_oem_hardware_version_str();
	uHWVer  = convert_version_str(hardwareVer);
	return uHWVer;
}

/*++
���ܣ�
	��ȡ����������汾��
������
����ֵ��
	����������汾�š���������ʱδ��ʹ��
--*/
unsigned long GOSSTBCA_GetSoftVer( ) 
{
	char *softwareVer = NULL;
	uint32_t		uSOFTVer=0;
	
	softwareVer = app_flash_get_oem_hardware_version_str();
	uSOFTVer  = convert_version_str(softwareVer);
	return uSOFTVer;	
}

/*++
���ܣ�
	��ʾǿ��ָ�ơ�������������ʾ����ָ����Ҫһֱ��ʾ����������ʾ������Intervalʱ���������仯��
������
	version:			ָ�ư汾��Ψһ
	FontColor:		ָ������ɫ
	FontSize:			ָ���ֺ�
	BackGroundColor:	ָ�Ʊ�����ɫ����ʽΪARGB8888
	ChnlCnt:			ָ����ʾƵ���������Ϊ0 ����ʾ����Ƶ������ʾָ��
						��0����ʾServiceId����service id��Ŀ��Ŀǰ�汾���֧��ָ��10��Ƶ��
						����ʽΪARGB8888
	ServiceId:		��ʾָ��Ƶ����service id
	IDType:			������Ŀǰ�汾Ϊ0x00
	Interval:			ʱ����,������ʾ֮��Ĳ���ʾָ�Ƶ�ʱ�䳤��
	Lock:			�Ƿ������û����� 1: �����û�������0: ������
	PosType:			ָ��λ�����ͣ�0 : �����ʾ��1: �̶�λ��
	Pos_X:			�̶�λ��ָ�Ƶ�X ����
	Pos_Y:			�̶�λ��ָ�Ƶ�Y ����
����ֵ��
	������Ӳ���汾�š�
--*/
void	GOSSTBCA_ShowForceFingerPrinting(unsigned long version, unsigned long FontColor, unsigned char FontSize, unsigned long BackGroundColor,
			unsigned char ChnlCnt, unsigned short *ServiceId, unsigned char IDType, unsigned short Interval, unsigned char Lock,
				unsigned char PosType, unsigned short Pos_X, unsigned short Pos_Y)
{
	return;
}

/*++
���ܣ�
	��ʾǿ��ָ�ơ�
������
	Version:				��ǰǿ��ָ�ư汾��
����ֵ��
--*/

void GOSSTBCA_CancleForceFingerPrinting(unsigned long Version)
{
	return;
}

/*++
���ܣ�
	ǿ�ƻ�̨
������
	pForceLockService:		ǿ�ƻ�̨�������μ�GOSCAS_STBDataPublic.h
	Version:				��ǰǿ�ƻ�̨�汾��
����ֵ��
--*/
void GOSSTBCA_ForceLockService(SGOSCALockService *pForceLockService, unsigned long Version)
{
	return;
}


/*++
���ܣ�
	ȡ��ǿ�ƻ�̨
������
	
����ֵ��
--*/
void GOSSTBCA_CancleForceLockService(unsigned long Version)
{
	return;
}


/*++
���ܣ�
	ȡ��ǿ��OSD
������
	osdID:			ID��Ψһ
	Style:			0: ������ʾ��1: �ı���ʾ
	Stylevalue:		styleΪ0 ��������Ϊ1: �Ϸ������� 0: �·�����
					styleΪ1��������Ϊ�趨�ı���ռ��Ļ�İٷֱȣ�ֵΪ20%-80%
	lock:				ָ�Ʊ�����ɫ
	FontColor:		OSD ��ʾ���ֵ���ɫ����ʽΪARGB8888
	FontSize:			OSD ��ʾ���ֺ�
	BackGroundColor:	�ı�OSD ��ʾ�ı�����ɫ����ʽΪARGB8888
	Showtimes:		���Ϊ������ʾOSD ���������趨����������������Ϊ�˼����ϵ�OSD ��ʽ
	Data_Body:		OSD ��ʾ����
	Data_Len:		OSD ��ʾ���ݳ���
����ֵ��
--*/
void	GOSSTBCA_ShowForceOSD(unsigned long osdID, unsigned char Style, unsigned char Stylevalue, unsigned char lock,unsigned long FontColor,
		unsigned char FontSize, unsigned long BackGroundColor, unsigned char Showtimes, unsigned char *Data_Body, unsigned short Data_Len)
{
	return;
}

/*++
���ܣ�
	ȡ��ǿ��OSD
������
	
����ֵ��
--*/
void GOSSTBCA_HideForceOSD()
{
	return;
}

/*++
���ܣ�
	�ر��������
������
	
����ֵ��
--*/
void GOSSTBCA_TaskDispatchoff()
{
	return;
}

/*++
���ܣ�
	���������
������
	
����ֵ��
--*/
void GOSSTBCA_TaskDispatchon()
{
	return;
}

