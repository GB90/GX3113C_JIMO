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

功能：
	CAS库在和串口通信失败一次后错误计数会自动加1，当值大于GOSCA_UART_ERROR_MAX
	会调用该接口。接口函数实现卡复位并确认ATR 正确。

参数：
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


功能：
	返回智能卡当前状态

参数：
	pbySCStatus：返回智能卡状态，为GOSCAS_SC_OUT、GOSCAS_SC_IN两种之一。
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

功能：
	重新启动机定盒。

参数：
-*/
void GOSSTBCA_ReStartSTB()  
{
	restart();
	return;
}


/*++


功能：
	获得机顶盒分配给CAS的存储空间的起点地址和大小（以字节为单位）

参数：
	ppStartAddr:	     机顶盒分配给CAS的存储空间的开始地址
	lSize:               输出机顶盒分配给CAS的储存空间的大小；
	
返回值：
	GOSCA_OK	成功
	  
说明：
       
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

功能：
	读取保存在机顶盒中的信息

参数：
	pStartAddr:	    要读取的存储空间的开始地址
	plDataLen:       输入为要读取的最长数据值
	                返回实际读取的大小
	pData：			存放输出数据

返回值：
	GOSCA_OK	成功
	       
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

功能：
	向机顶盒的存储空间写信息

参数：
	lStartAddr:	     要写的存储空间的开始地址
	plDataLen:        要写的数据的长度
					 返回为写入的实际长度
	pData：			 要写的数据
	
	                     
返回值：
	
说明：
	GOSCA_OK	成功
       
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

功能：
	向机顶盒的备份内存信息

参数：
	
	                     
返回值：
	
说明：
	GOSCA_OK	成功
       
--*/
HRESULT GOSSTBCA_Data_Back(const unsigned long pStartAddr, const unsigned int DataLen)  
{
	return GOSCA_OK;
}

/*++

功能：
	申请内存

参数：
	dwSize:		申请内存的空间
						 
返回值：
	分配内存空间的地址
			
说明：
	   
--*/
unsigned char *GOSSTBCA_malloc( unsigned int dwSize ) 
{
	return app_porting_ca_os_malloc(dwSize);	
}

/*++

功能：
	释放所申请内存

参数：
	*pAddr: 	申请内存的地址
						 
返回值：
			
说明：
	   
--*/
void GOSSTBCA_free( unsigned char *pAddr ) 
{
	app_porting_ca_os_free(pAddr);
	return ;
}

/*++

功能：
	挂起当前线程

参数：
	dwSleepMicroseconds: 		毫秒
	                     
返回值：
		    
说明：
       
--*/
HRESULT GOSSTBCA_Sleep( long dwSleepMicroseconds ) 
{
	app_porting_ca_os_sleep(dwSleepMicroseconds);
	return GOSCA_OK;	
}

/*++

功能：
	初始化信号量,初始化后信号量没有信号。

参数：
	pSemaphore: 		指向信号量的指针
	                     
返回值：
		    
说明：
       
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

功能：
	释放一个信号量

参数：
	pSemaphore: 		指向信号量的指针
	                     
返回值：
		    
说明：
       
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
功能：
	给信号量加信号

参数：
	pSemaphore: 		指向信号量的指针
	                     
返回值：
		    
说明：
       
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
功能：
	等待信号量,等待成功之后,信号量为无信号

参数：
	pSemaphore: 		指向信号量的指针
	                     
返回值：
		    
说明：
       
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
功能：
	CAS模块向机顶盒注册线程

参数：
	szName:				注册的线程名称
	ProcessID:			注册的线程的ID
	pTaskFun:			要注册的线程函数	                     

返回值：
	GOSCA_OK			线程注册成功
	
说明：
       
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

功能：
	设置过滤器接收CAS私有数据。

参数：
	byReqID:	请求收表的请求号。机顶盒应该保存该数据，接收到CAS私有数据后调用GOSCASTB_StreamGuardDataGot()时应该将该数据返回。
	wEcmPID:	需要过滤的流的PID。
	szFilter:	过滤器的值，为一个8个字节的数组，相当于8*8的过滤器
	szMask:		过滤器的掩码，为一个8个字节的数组，与8*8的过滤器对应，当szMask的某一bit位为0时表示要过滤的表对应的bit位不需要与szFilter里对应的bit位相同，当szMask某一位为1时表示要过滤的表对应的bit位一定要与szFilter里对应的bit位相同
	byLen:		过滤器的长度，为8
	bContiunue:	是否连续过滤

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
	// 如果过滤条件完全相同，仅RESET

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

功能：
	CA程序用此函数设置解扰器。将当前周期及下一周期的CW送给解扰器。

参数：
	wEcmPID:				CW所属的ECMPID。
	szOddKey:				奇CW的数据
	szEvenKey:				偶CW的数据
	byKeyLen:				CW的长度
	bTaingControl           true：允许录像,false：不允许录像

	                     
返回值：
       
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
功能：
	   打印调试信息

参数：
	pszMsg:     调试信息内容
	                     
返回值：
	void

说明：
      
--*/
extern void GOSSTBCA_AddDebugMsg(char *pszMsg) 
{
	printf("[GOS_CA]%s\n",( char * ) pszMsg );
}


/*++
功能：
	实现与智能卡之间的通讯。采用7816标准。
参数：
	byReaderNo：	对应的读卡器编号
	byLen:			输入为待发送命令字节的长度，输出为返回字节长度
	byszCommand:	待发送的一串命令字节
	szReply:		返回的数据，空间长度恒为256字节
	
返回值：
      
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


/*------------------------------------ 类型1的界面：不允许用户通过遥控器取消显示的界面--------------------------------*/
/*
功能：
	显示用户不能观看节目等提示信息，下面两个函数对应使用。
参数:	
	byMesageNo:  要显示的消息编号
	bShowInfo==1:表示有提示信息需要显示；在提示信息的下面显示提示内容，前端发送了邮件名“$#OperSvr*”的邮件内容这里显示；
	bShowInfo==0:参数pString和参数strlen不管；

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
		case GOSCA_No_OSDPromt:        			/*无OSD提示*/	
			 messageType = CA_GOS_No_OSDPromt;
			 break;
		case GOSCA_SMART_STOP:					/*卡已被禁用*/
			messageType = CA_GOS_SMART_STOP;
			break;
		case GOSCA_SMART_AREA_NOPAIR:			/*流卡区域不匹配*/
			messageType = CA_GOS_SMART_AREA_NOPAIR;
			break;
		case GOSCA_SMART_IPPT_SWITCH:			/*IPPT用户提示*/
			messageType = CA_GOS_SMART_IPPT_SWITCH;
			break;				
		case GOSCA_SMART_STB_NOPAIR:			/*机卡不配对*/
			messageType = CA_GOS_SMART_STB_NOPAIR;
			break;	
		case GOSCA_SMART_PROG_NOENTITLE:		/*节目类未授权*/
			messageType = CA_GOS_SMART_PROG_NOENTITLE;
			break;					
		case GOSCA_SMART_PROG_NOENTITLE_IPPT:   /*节目类未授权*/
			messageType = CA_GOS_SMART_PROG_NOENTITLE_IPPT;
			break;					
		case GOSCA_SMART_MOTHER_SON_PAIR:		/*子母卡需从新配对*/
			messageType = CA_GOS_SMART_MOTHER_SON_PAIR;
			break;					
		case GOSCA_SMART_MOTHER_SON_TIMEOUT:	/*子母卡配对超时*/
			messageType = CA_GOS_SMART_MOTHER_SON_TIMEOUT;
			break;				
		case GOSCA_SMART_PAIR_OK:				/*机卡对应OK*/
			messageType = CA_GOS_SMART_PAIR_OK;
			break;					
		case GOSCA_SMART_LAST_DAY:				/*最后提示天数*/
			messageType = CA_GOS_SMART_LAST_DAY;
			s_duration = duration;
			break;				
		case GOSCA_SMART_AREA_LOCK:	 			/*卡区域锁定 */
			messageType = CA_GOS_SMART_AREA_LOCK;
			break;					
		case GOSCA_SMART_CARD_LOCK: 			/*卡已被锁定 */
			messageType = CA_GOS_SMART_CARD_LOCK;
			break;					
		case GOSCA_SMART_IPPT:					/*IPPT提示*/
			messageType = CA_GOS_SMART_IPPT;
			s_ippt_duration = duration;
			s_ippt = ippt; 				
			break;					
		case GOSCA_SMART_LIMIT_WATCH:			/*卡限制收看*/
			messageType = CA_GOS_SMART_LIMIT_WATCH;
			break;					
		case GOSCA_SMART_NO_WORK_TIME:			/*卡不在工作时间段*/
			messageType = CA_GOS_SMART_NO_WORK_TIME;
			break;					
		case GOSCA_SMART_OUT:					/*节目加扰，请插卡*/
			messageType = CA_GOS_SMART_OUT;
			break;
		default:
			break;
			//	GOSCA_SMART_UNKNOWN_ERROR,		/*未知错误*/
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
功能：
	显示指纹信息。
参数:
	wDuration ：毫秒级，默认在3秒自动消失
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
功能：
	显示OSD消息，可以是同步显示，也可以是异步显示。
参数:	
	szOSD：		用于显示的OSD信息
	wTimes：	显示的次数
				
说明：
	用户可以通过遥控器取消当前OSD显示。
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
				/*回车或换行会被当作结束符*/
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
功能：
	显示新邮件的标志。
参数：
	byShow:		GOSCAS_NEW_EAMIL				新邮件通知
				GOSCAS_NEW_EMAIL_NO_ROOM		有新邮件，但Email空间不够
				GOSCAS_EMAIL_NONE				没有新邮件
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
功能：
	获取机顶盒序列号。
参数：

返回值：
	机顶盒序列号。保留以便后期库能使用
--*/
unsigned long GOSSTBCA_GetSerialNo( void ) 
{
    
	char caUser[MAX_SEARIAL_LEN+1]={0};
	app_flash_get_serial_number(0,(char *)caUser,MAX_SEARIAL_LEN);
    printf("GOSSTBCA_GetSerialNo\n");
	return atoi(caUser+12);
}

/*++
功能：
	获取机顶盒硬件版本号。
参数：

返回值：
	机顶盒硬件版本号。保留，暂时未被使用
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
功能：
	获取机顶盒软件版本。
参数：
返回值：
	机顶盒软件版本号。保留，暂时未被使用
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
功能：
	显示强制指纹。如果不是随机显示。该指纹需要一直显示。如果随机显示，需再Interval时间进行随机变化；
参数：
	version:			指纹版本，唯一
	FontColor:		指纹字颜色
	FontSize:			指纹字号
	BackGroundColor:	指纹背景颜色，格式为ARGB8888
	ChnlCnt:			指纹显示频道数，如果为0 ，表示所有频道都显示指纹
						非0，表示ServiceId参数service id数目，目前版本最大支持指定10个频道
						，格式为ARGB8888
	ServiceId:		显示指纹频道的service id
	IDType:			保留，目前版本为0x00
	Interval:			时间间隔,两次显示之间的不显示指纹的时间长度
	Lock:			是否锁定用户操作 1: 锁定用户操作，0: 不锁定
	PosType:			指纹位置类型，0 : 随机显示，1: 固定位置
	Pos_X:			固定位置指纹的X 坐标
	Pos_Y:			固定位置指纹的Y 坐标
返回值：
	机顶盒硬件版本号。
--*/
void	GOSSTBCA_ShowForceFingerPrinting(unsigned long version, unsigned long FontColor, unsigned char FontSize, unsigned long BackGroundColor,
			unsigned char ChnlCnt, unsigned short *ServiceId, unsigned char IDType, unsigned short Interval, unsigned char Lock,
				unsigned char PosType, unsigned short Pos_X, unsigned short Pos_Y)
{
	return;
}

/*++
功能：
	显示强制指纹。
参数：
	Version:				当前强制指纹版本号
返回值：
--*/

void GOSSTBCA_CancleForceFingerPrinting(unsigned long Version)
{
	return;
}

/*++
功能：
	强制换台
参数：
	pForceLockService:		强制换台参数，参见GOSCAS_STBDataPublic.h
	Version:				当前强制换台版本号
返回值：
--*/
void GOSSTBCA_ForceLockService(SGOSCALockService *pForceLockService, unsigned long Version)
{
	return;
}


/*++
功能：
	取消强制换台
参数：
	
返回值：
--*/
void GOSSTBCA_CancleForceLockService(unsigned long Version)
{
	return;
}


/*++
功能：
	取消强制OSD
参数：
	osdID:			ID，唯一
	Style:			0: 滚动显示，1: 文本显示
	Stylevalue:		style为0 ，本参数为1: 上方滚动， 0: 下方滚动
					style为1，本参数为设定文本框占屏幕的百分比，值为20%-80%
	lock:				指纹背景颜色
	FontColor:		OSD 显示文字的颜色，格式为ARGB8888
	FontSize:			OSD 显示的字号
	BackGroundColor:	文本OSD 显示的背景颜色，格式为ARGB8888
	Showtimes:		如果为滚动显示OSD ，本参数设定滚动次数，本参数为了兼容老的OSD 方式
	Data_Body:		OSD 显示内容
	Data_Len:		OSD 显示内容长度
返回值：
--*/
void	GOSSTBCA_ShowForceOSD(unsigned long osdID, unsigned char Style, unsigned char Stylevalue, unsigned char lock,unsigned long FontColor,
		unsigned char FontSize, unsigned long BackGroundColor, unsigned char Showtimes, unsigned char *Data_Body, unsigned short Data_Len)
{
	return;
}

/*++
功能：
	取消强制OSD
参数：
	
返回值：
--*/
void GOSSTBCA_HideForceOSD()
{
	return;
}

/*++
功能：
	关闭任务调度
参数：
	
返回值：
--*/
void GOSSTBCA_TaskDispatchoff()
{
	return;
}

/*++
功能：
	打开任务调度
参数：
	
返回值：
--*/
void GOSSTBCA_TaskDispatchon()
{
	return;
}

