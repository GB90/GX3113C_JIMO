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
#include "BYCAstb_ca.h"
#include "BYCAca_stb.h"
#include "BYCApubtype.h"               
#include "app_common_porting_ca_flash.h"
#include "app_common_porting_ca_demux.h"
#include "app_common_porting_ca_os.h"
#include "app_common_porting_ca_smart.h"
#include "app_common_table_nit.h"
#include "app_common_search.h"
#include "app_common_play.h"
#include "app_common_porting_stb_api.h"
#include "app_by_cas_api_smart.h"
#include "app_common_porting_ca_descramble.h"
#include "app_common_prog.h"
#include "app_common_lcn.h"
#include "app_by_cas_api_pop.h"
#include "app_by_cas_api_demux.h"
#include "app_by_cas_api_email.h"
#include "app_by_cas_api_lock_service.h"
#include "app_by_cas_api_demux.h"

static by_ca_rolling_message_st  rollingmsg = {0};
static by_ca_finger_data_st  fingerdata = {0};
static by_cas_email_st emailflag = {0};
extern uint8_t bat_recieve_flag ;
#define ONE_SECTOR_SIZE (64*1024)



/*++
功能：返回智能卡当前状态。
参数：pbyStatus：	返回智能卡状态，为BYCAS_SC_OUT、BYCAS_SC_IN两种之一。
--*/
BVOID BYSTBCA_GetSCStatus(BU8 * pbyStatus)
{
	if (NULL == pbyStatus)
		{
		  	CAS_Dbg("BYSTBCA_GetSCStatus pbyStatus=NULL\n");
			return;
		
		}

	if (TRUE == app_porting_ca_smart_status())
		*pbyStatus = BYCAS_SC_IN;
	else
		*pbyStatus = BYCAS_SC_OUT;
	return ;
}

/*++
功能：获得机顶盒分配给CAS的存储空间的起点地址和大小（以字节为单位）。
参数：
	ppStartAddr:			机顶盒分配给CAS的存储空间的开始地址。
	lSize:					输出机顶盒分配给CAS的储存空间的大小。
--*/
BU8 BYSTBCA_GetDataBufferAddr(BU32* lSize,BU32 * ppStartAddr)
{
	if ((NULL == lSize)||(NULL ==ppStartAddr ))
		{
		  	CAS_Dbg("BYSTBCA_GetDataBufferAddr lSize=0x%x ppStartAddr=0x%x\n",(unsigned int)lSize,(unsigned int)ppStartAddr);		
			return 0;
		}

	
    *ppStartAddr = ONE_SECTOR_SIZE;
	*lSize = ONE_SECTOR_SIZE;
	return 1;
}

/*++
功能：读取保存在机顶盒中的信息。
参数：
	pStartAddr:				要读取的存储空间的开始地址。
	plDataLen:				输入为要读取的最长数据值；输出为实际读取的大小。
	pData:					存放输出数据。
--*/
BU8 BYSTBCA_ReadDataBuffer(BU32 pStartAddr,BU32* plDataLen,BU8* pData)
{
	uint32_t  Offset = pStartAddr-ONE_SECTOR_SIZE;
	
       if ((NULL ==plDataLen )||(NULL == pData))
       	{
		  	CAS_Dbg("BYSTBCA_ReadDataBuffer plDataLen=0x%x pData=0x%x\n",(unsigned int)plDataLen,(unsigned int)pData);		       	
	   		return 0;
       	}
	   
	if (Offset + *plDataLen> ONE_SECTOR_SIZE)
		{
		  	CAS_Dbg("BYSTBCA_ReadDataBuffer Offset=0x%x *plDataLen=0x%x\n",(unsigned int)Offset,(unsigned int)(*plDataLen));		       	
			return 0;
		}
	
	return app_porting_ca_flash_read_data(Offset,(uint8_t *)pData,(uint32_t *)plDataLen);
}

/*++
功能：向机顶盒的存储空间写信息。
参数：
	lStartAddr:				要写的存储空间的开始地址。
	plDataLen:				输入为要写的数据的长度；输出为写入的实际长度。
	pData:					要写的数据。
--*/
BU8 BYSTBCA_WriteDataBuffer(BU32 pStartAddr,BU32 DataLen,BU8* pData)
{
	uint32_t  Offset = pStartAddr-ONE_SECTOR_SIZE;

	if (NULL == pData)
		{
			CAS_Dbg("BYSTBCA_WriteDataBuffer pData=0x%x \n",(unsigned int)pData);		       	
			return 0;
		}
	
	if (Offset + DataLen> ONE_SECTOR_SIZE)
		{
		  	CAS_Dbg("BYSTBCA_WriteDataBuffer Offset=0x%x DataLen=0x%x\n",(unsigned int)Offset,(unsigned int)DataLen);		       	
			return 0;
		}
	
	return	app_porting_ca_flash_write_data(Offset,(uint8_t *)pData,DataLen);
	
}


/*++
功能：挂起当前线程。
参数：
	dwmsec:			毫秒。
--*/
BU8 BYSTBCA_Sleep(BU32 dwmsec)
{

	app_porting_ca_os_sleep(dwmsec);
	return 1;
}

/*++
功能：初始化信号量,初始化后信号量没有信号。
参数：pSemaphore:				指向信号量的指针。
	    flag:						  初始化信号量值
--*/
BU8 BYSTBCA_SemaphoreInit(BY_Semaphore *pSemaphore,BU8 flag)
{
	handle_t semaphore = 0;	
	if (NULL == pSemaphore)
		{
			CAS_Dbg("BYSTBCA_SemaphoreInit pSemaphore=0x%x \n",(unsigned int)pSemaphore);		       	
			return 0;
		}
	
    	  app_porting_ca_os_sem_create(&semaphore,flag);
	*pSemaphore = (handle_t)semaphore;
	return 1;
}

/*++
功能：释放一个信号量。
参数：pSemaphore: 			指向信号量的指针。
--*/
BU8 BYSTBCA_SemaphoreDestory(BY_Semaphore *pSemaphore )
{
	if (NULL == pSemaphore)
		{
			CAS_Dbg("BYSTBCA_SemaphoreDestory pSemaphore=0x%x \n",(unsigned int)pSemaphore);		       	
			return 0;
		}

	app_porting_ca_os_sem_delete((handle_t)*pSemaphore);
	return 1;
}

/*++
功能：给信号量加信号。
参数：pSemaphore: 			指向信号量的指针。
--*/
BU8 BYSTBCA_SemaphoreSignal(BY_Semaphore *pSemaphore )
{
	if (NULL == pSemaphore)
		{
			CAS_Dbg("BYSTBCA_SemaphoreSignal pSemaphore=0x%x \n",(unsigned int)pSemaphore);		       	
			return 0;
		}
	
	app_porting_ca_os_sem_signal((handle_t)*pSemaphore);
	return 1;
}

/*++
功能：等待信号量,等待成功之后,信号量为无信号。
参数：pSemaphore: 			指向信号量的指针。
--*/
BU8 BYSTBCA_SemaphoreWait(BY_Semaphore *pSemaphore )
{
	if (NULL == pSemaphore)
		{
			CAS_Dbg("BYSTBCA_SemaphoreWait pSemaphore=0x%x \n",(unsigned int)pSemaphore);		       	
			return 0;
		}
	
	app_porting_ca_os_sem_wait((handle_t)*pSemaphore);
	return 1;
}

/*++
功能：CAS模块向机顶盒注册线程。
参数：
	szName:					注册的线程名称。
	pTaskFun:				要注册的线程函数。
--*/
BU8 BYSTBCA_RegisterTask(const BS8* szName,pThreadFunc pTaskFun)
{
    int         ret;
    handle_t    handle;
    uint32_t priority = GXOS_DEFAULT_PRIORITY;

	if ((NULL == szName)||(NULL == pTaskFun))
	{
		CAS_Dbg("BYSTBCA_RegisterTask szName=%s  pTaskFun=0x%x\n",szName,(unsigned int)pTaskFun);		       	
		return 0;
	}

	printf("BYSTBCA_RegisterTask szName=%s\n",szName);

           	ret = app_porting_ca_os_create_task(szName, &handle, (void*)pTaskFun,
                              NULL, 1024*10, priority);

	return 1;
}

/*++
功能：设置过滤器接收CAS私有数据。
参数：
	byReqID:				请求收表的请求号。机顶盒应该保存该数据，接收到CAS私有数据后调用BYCASTB_StreamGuardDataGot()时应该将该数据返回。
	wEcmPID:				需要过滤的流的PID。
	szFilter:				过滤器的值，为一个8个字节的数组，相当于8*8的过滤器。
	szMask:					过滤器的掩码，为一个8个字节的数组，与8*8的过滤器对应，当szMask的某一bit位为0时表示要过滤的表对应的bit位不需要与szFilter里对应的bit位相同，当szMask某一位为1时表示要过滤的表对应的bit位一定要与szFilter里对应的bit位相同。
	byLen:					过滤器的长度，为8。
	nWaitSecs:			收此表最长的等待时间，如果为0则无限等待，单位秒。
--*/
BU8 BYSTBCA_SetStreamGuardFilter(BU8 byReqID,BU16 wPID,BU8* szFilter,BU8* szMask,BU8 byLen,BU16 nWaitSecs)
{

	uint16_t            filterID;
	ca_filter_t* sfilter = NULL;
	ca_filter_t filter = {0};
	bool repeatFlag = TRUE;
	uint8_t     match[18] = {0,};
	uint8_t     mask[18] = {0,};
	BU8* pbyFilter = szFilter;
	BU8* pbyMask = szMask;
	uint16_t	 i=0;


	if(pbyFilter == NULL || pbyMask == NULL )
	{
		CAS_Dbg("BYSTBCA_SetStreamGuardFilter pbyFilter=0x%x  pbyMask=0x%x\n",(unsigned int)pbyFilter,(unsigned int)pbyMask);		       	
		return 0;
	}

/*	if (0x80 ==( 0x80&byReqID))
	{
		repeatFlag = FALSE;
	}*/

//  	CAS_Dbg("BYSTBCA_SetStreamGuardFilter byReqID=0x%x,wPID=0x%x byLen=0x%x,nWaitSecs=%d\n",
//		byReqID,wPID,byLen,nWaitSecs);

//	CAS_DUMP("BYSTBCA_SetStreamGuardFilter pbyFilter", pbyFilter , byLen);
//	CAS_DUMP("BYSTBCA_SetStreamGuardFilter pbyMask", pbyMask , byLen);


	for (i = 0; i< byLen; i++)
	{
		if (0 == i)
		{
			match[i]	 = pbyFilter[i];
			mask[i] = pbyMask[i];
		}
		else
		{
			match[i+2]	 = pbyFilter[i];
			mask[i+2] = pbyMask[i];
		}
	}



	for (filterID = 0; filterID< MAX_FILTER_COUNT; filterID++)
	{
	    app_porting_ca_demux_get_by_filter_id(filterID,&filter);
		if (( 0 !=filter.usedStatus)&&(wPID == filter.pid)
			&&(0 !=filter.channelhandle)&&(byReqID == filter.byReqID))
		{
			app_porting_ca_demux_release_filter(filterID, TRUE);
			break;
		}
	}
	

	memset(&filter,0,sizeof(ca_filter_t));
	filter.byReqID = byReqID;
//	filter.crcFlag = FALSE;
	filter.crcFlag = TRUE;
	filter.filterLen = byLen;
	memcpy(filter.match,match,byLen+2);
	memcpy(filter.mask,mask,byLen+2);
	filter.nWaitSeconds = nWaitSecs;
	filter.pid = wPID;
	filter.equalFlag = TRUE;
	filter.repeatFlag = repeatFlag;
	filter.Callback = app_by_cas_filter_notify;
	filter.timeOutCallback = app_by_cas_filter_timeout_notify;
	return app_porting_ca_demux_start_filter(&filter);

}


/*++
功能：CA程序用此函数设置解扰器。将当前周期及下一周期的CW送给解扰器。
参数：
	wEcmPID:				CW所属的ECMPID。
	szOddKey:				奇CW的数据。
	szEvenKey:				偶CW的数据。
	byKeyLen:				CW的长度。
	bTaingControl:			true：允许录像,false：不允许录像。
--*/
BU8 BYSTBCA_SetDescrCW(BU16 wEcmPid,BU8 byKeyLen,const BU8* szOddKey,const BU8* szEvenKey,BU8 bTaingControl)
{

	app_porting_ca_descramble_set_cw(wEcmPid,szOddKey,szEvenKey,byKeyLen,bTaingControl);

	return 1;
}

/*++
功能：打印调试信息。
参数：Param1,Param2,pszMsg:					调试信息内容。
--*/
void BYSTBCA_AddDebugMsg(BU32 Param1,BU32 Param2,const BS8 *pszMsg)
{
#if 0
	if (NULL != pszMsg)
		printf("[0x%x][0x%x] %s \n",(unsigned int)Param1,(unsigned int)Param2,pszMsg);
	else
		printf("[0x%x][0x%x] \n",(unsigned int)Param1,(unsigned int)Param2);		
#endif
}


/*++
功能：当用户插卡时,先调用该函数复位卡
返回:     true  成功，false失败
参数：pATR 最大64 字节
--*/
BU8 BYCASTB_SCReset(BU8 *pATR)
{

  	    uint8_t                     len;
	    uint8_t               atr[256] = {0};
	int nRetValue = FALSE;

  	CAS_Dbg("[SC] BYCASTB_SCReset!\n");

        nRetValue = app_by_cas_api_smart_reset(atr,&len);

	if (TRUE == nRetValue)
    	{
    		memcpy(pATR,atr,len);
    		return TRUE;
    	}
    else
    	{
     		return FALSE;
    	}
}

/*++
功能：实现与智能卡之间的通讯。采用7816标准。
参数：
	byReaderNo:				对应的读卡器编号。
	pbyLen:					输入为待发送命令字节的长度;输出为返回字节长度。
	byszCommand:			待发送的一串命令字节。
	szReply:				返回的数据，空间长度恒为256字节。
--*/
BU8 BYSTBCA_SCAPDU(BU8 byReaderNo,const BU8* byszCommand,BU16* pbyLen,BU8* byszReply)
{
    uint8_t reply[1024] = {0};
    uint8_t sc_result;
    sc_result =  app_porting_ca_smart_sc_apdu(byszCommand,*pbyLen,reply,pbyLen);
    if (TRUE == sc_result)
    	{
    		memcpy(byszReply, reply, *pbyLen);	
//	       CAS_DUMP("[SC] BYSTBCA_SCAPDU success ", byszReply, *pbyLen);
    	}
   return sc_result;

}

/*------------ 类型1的界面：不允许用户通过遥控器取消显示的界面 -----------------------*/
/*
功能：显示用户不能观看节目等提示信息，下面两个函数对应使用。
参数：	
	byMesageNo:				要显示的消息编号。
--*/
BVOID   BYSTBCA_ShowPromptMessage(BU8 byMesageNo)
{
	if (MSG_POP_COMMON_END+byMesageNo+1 >=64)
		return ;

	if ( TRUE != app_play_get_msg_pop_type_state(MSG_POP_COMMON_END+byMesageNo+1))
		{
			app_play_clear_ca_msg();
			app_play_set_msg_pop_type(MSG_POP_COMMON_END+byMesageNo+1);
		}
}
BVOID   BYSTBCA_HidePromptMessage(BVOID)
{
	app_play_clear_ca_msg();
}

/*
功能：显示指纹信息。
参数：
	wEcmPid					ECM PID
	dwCardID:				卡号。
	wDuration:				持续秒数。
--*/
BVOID	BYSTBCA_ShowFingerPrinting(BU16 wEcmPid,BU32 dwCardID,BU32 wDuration)
{
	if ((dwCardID == fingerdata.dwCardID)&&(wEcmPid == fingerdata.wEcmPid)
		&&(0x1fff == wEcmPid)&&(0xffffffff == dwCardID))
		return ;

	printf("BYSTBCA_ShowFingerPrinting wEcmPid=0x%x,dwCardID=0x%x,wDuration=0x%x\n",
		(unsigned int)wEcmPid,(unsigned int)dwCardID,(unsigned int)wDuration);


	fingerdata.dwCardID = dwCardID;
	fingerdata.wDuration = wDuration;
	fingerdata.wEcmPid = wEcmPid;

	if((wEcmPid != 0x1fff) && (dwCardID != 0xffffffff))
		{
			fingerdata.wStatus = 2;
		}
	else if((wEcmPid == 0x1fff) && (dwCardID != 0xffffffff))
		{
			fingerdata.wStatus = 1;		
		}
	else if((wEcmPid == 0x1fff) && (dwCardID == 0xffffffff))
		{
			fingerdata.wStatus = 0;				
		}

	app_cas_api_on_event(DVB_CA_FINGER_INFO,CAS_NAME, &fingerdata, sizeof(by_ca_finger_data_st));
//	GxCore_ThreadDelay(20); 
}

/*--------------------------- 类型2的界面：用户可以取消显示的界面 -----------------------*/
/*
功能：以左滚字幕的方式显示OSD消息，可以是同步显示，也可以是异步显示。
参数:	
	szOSD:					用于显示的OSD信息。
	wDuration:				0:显示一次后立即返回;其他值为显示的具体持续秒数。
说明：
	用户可以通过遥控器取消当前OSD显示。
--*/
BVOID	BYSTBCA_ShowOSDMsg(const BS8* szTitle,const BS8* szOSD,BU16 wDuration)
{
	int i = 0;
	int j=0;

	printf("BYSTBCA_ShowOSDMsg szOSD=%s\n",szOSD);

	memset(&rollingmsg,0,sizeof(by_ca_rolling_message_st))	;
	rollingmsg.wDuration = wDuration;


	if ((NULL == szTitle)||(NULL == szOSD))
		{
			return;
		}

	for (i = 0; i< strlen(szTitle);i++)
		{
			if (i >= ROLL_TITLE_MAX)
				break;
			if ((0x0d !=szTitle[i] )&&(0x0a != szTitle[i]))
				{
					rollingmsg.title[j++]=	szTitle[i];
				}
			else
				{
					/*回车或换行会被当作结束符*/
					printf("szTitle[%d]=0x%x\n",i,szTitle[i]);
				}

		}

	i = 0;
	j=0;

		for (i = 0; i< strlen(szOSD);i++)
		{
			if (i >= ROLL_CONTENT_MAX)
				break;
			if ((0x0d !=szOSD[i] )&&(0x0a != szOSD[i]))
				{
					rollingmsg.content[j++]=	szOSD[i];
				}
			else
				{
					/*回车或换行会被当作结束符*/
					printf("szOSD[%d]=0x%x\n",i,szOSD[i]);
				}

		}
	rollingmsg.wStatus = TRUE;
	app_cas_api_on_event(DVB_CA_ROLLING_INFO,CAS_NAME, &rollingmsg, sizeof(by_ca_rolling_message_st));
//	GxCore_ThreadDelay(20); 
}

/*
功能: 提供随机数据
参数: blen 长度pbBuf 返回blen个随机数
*/
BU8	BYSTBCA_Random(BU8 blen,BU8 *pbBuf)
{
	uint8_t i;
	for (i = 0; i<blen; i++)
		{
			pbBuf[i] = rand()%256;
		}
	return 1;
}


/*
功能：通知APP锁定某个频道信号，在该特定的频点寻找漫游信息。
参数:	KHZ;
返回:  1，表示频点已锁到，否则未锁定该频点。
--*/
BU8 BYCA_SetZoneFreq( BU32 Freq )
{
	uint32_t symbol_rate = 0;
	uint32_t qam = 0;
	uint32_t freq = Freq/1000;

	if (TRUE == bat_recieve_flag)
		return 1;

	symbol_rate = app_flash_get_config_center_freq_symbol_rate();
	qam = app_flash_get_config_center_freq_qam();

	if (0 == app_search_lock_tp(freq, symbol_rate,
		INVERSION_OFF, qam,1000))
	{
		return app_by_cas_api_bat_filter_open();
	}
	else
		{
			return 0;
		}
	return 1;
}

/*++
功能：显示新邮件的标志。
参数：
	byShow:					标志类型。含义如下：
							BYCAS_NEW_EAMIL						新邮件通知。
							BYCAS_NEW_EMAIL_NO_ROOM		有新邮件，但Email空间不够。
							BYCAS_EMAIL_NONE					没有新邮件。
--*/
BVOID BYSTBCA_EmailNotify(BU8 byShow)
{

	if ((BYCAS_EMAIL_NONE == byShow)&&(0 == emailflag.wStatus))
		return ;
	
	switch(byShow)
		{
			case BYCAS_EMAIL_NONE:
				emailflag.wStatus = DVB_CA_EMAIL_FLAG_HIDE;
//				printf("BYSTBCA_EmailNotify byShow=%d 要求隐藏邮件图标\n ",byShow);
				break;
			case BYCAS_NEW_EAMIL:
				emailflag.wStatus = DVB_CA_EMAIL_NEW;
//				printf("BYSTBCA_EmailNotify byShow=%d 要求显示新邮件图标\n ",byShow);				
				break;
			case BYCAS_NEW_EMAIL_NO_ROOM:
				emailflag.wStatus = DVB_CA_EMAIL_EXHAUST;
//				printf("BYSTBCA_EmailNotify byShow=%d 要求显示邮件图标闪烁\n ",byShow);					
				break;
			default:
				break;
		}

	app_cas_api_on_event(DVB_CA_EMAIL_INFO,CAS_NAME, &emailflag, sizeof(by_cas_email_st));
//	GxCore_ThreadDelay(20); 
	
	return;
}

/*--------------------------- 应急广播 -----------------------*/
/*++
功能：应急广播, 切换到指定频道。
参数：
	wOriNetID:				原始网?鏘D。
	wTSID:					传送流ID。
	wServiceID:				业务ID。
	pUTCTime--5 bytes:				UTC时间。
--*/
BVOID BYCASTBCA_UrgencyBroad(BU16 wOriNetID,BU16 wTSID,BU16 wServiceID,BU8 *pUTCTime)
{
	printf(" BYCASTBCA_UrgencyBroad \n");
	return app_by_cas_api_lock_service(wOriNetID,wTSID,wServiceID,pUTCTime);
}

/*++
功能：取消应急广播，切换回应急广播前用户观看的频道。
参数：无。
--*/
BVOID BYCASTBCA_CancelUrgencyBroad(BVOID)
{
	printf(" BYCASTBCA_CancelUrgencyBroad \n");  
	app_play_recall_prog();
	app_play_switch_prog_clear_msg();	
	app_play_reset_play_timer(0);
	app_by_cas_api_set_lock_status(FALSE);
	
	return;
}

