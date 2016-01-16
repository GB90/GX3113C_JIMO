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
//#include "BYCAstb_ca.h"
//#include "BYCAca_stb.h"
//#include "BYCApubtype.h"   
#include "stb_ca2app.h"
#include "stb_app2ca.h"
#include "app_common_porting_ca_flash.h"
#include "app_common_porting_ca_demux.h"
#include "app_common_porting_ca_os.h"
#include "app_common_porting_ca_smart.h"
#include "app_common_table_nit.h"
#include "app_common_search.h"
#include "app_common_play.h"
#include "app_common_porting_stb_api.h"
#include "app_qilian_cas_api_smart.h"
#include "app_common_porting_ca_descramble.h"
#include "app_common_prog.h"
#include "app_common_lcn.h"
#include "app_qilian_cas_api_pop.h"
#include "app_qilian_cas_api_demux.h"
#include "app_qilian_cas_api_email.h"
#include "app_qilian_cas_api_lock_service.h"
#include "app_qilian_cas_api_demux.h"

static qilian_ca_rolling_message_st  rollingmsg = {0};
static qilian_ca_finger_data_st  fingerdata = {0};
static qilian_cas_email_st emailflag = {0};
extern uint8_t bat_recieve_flag ;
#define ONE_SECTOR_SIZE (64*1024)



/*++
功能：返回智能卡当前状态。
参数：pbyStatus：	返回智能卡状态，为BYCAS_SC_OUT、BYCAS_SC_IN两种之一。
--*/
//BVOID BYSTBCA_GetSCStatus(BU8 * pbyStatus)
handle_t  queueId1= 0;
handle_t queueId2=0;
BU8 STBCA_MSGQUENE_Init(BU8 num)
{
	handle_t  queuedata = 0;
	if(num!=2)
	{
		return 0;
	}
 	if(GXCORE_SUCCESS==GxCore_QueueCreate(&queuedata,/*maxQueueLength*/20,sizeof(CA_MSG)))
	{
		queueId1=queuedata;
	}
	else
	{
		CAS_Dbg("STBCA_MSGQUENE_Init failed !\n");
	}
 	if(GXCORE_SUCCESS==GxCore_QueueCreate(&queuedata,/*maxQueueLength*/20,sizeof(CA_MSG)))
	{
		queueId2=queuedata;
	}
	else
	{
		CAS_Dbg("STBCA_MSGQUENE_Init failed !\n");
	}
	return 1;
}
/*++
功能：从第NO个消息队列中获取头条消息，非阻塞型。
参数：如果队列为空，则  参数msg 为空
	CA_MSG *msg1= NULL;
	STBCA_GETCAMSG(0,msg1);
	if(msg1 != NULL )
	{
		//从第一个消息队列中获取并  分析消息
	}
	CA_MSG *msg2= NULL;
	STBCA_GETCAMSG(1,msg2);
	if(msg2 != NULL )
	{
		//从第二个消息队列中获取并  分析消息
	}
--*/
//void STBCA_GETCAMSG(BU8 NO,CA_MSG* msg);
CA_MSG ca_msg1,ca_msg2;
CA_MSG* STBCA_GETCAMSG(BU8 NO)
{
	uint32_t    sz=0;
	int32_t     timeout;
	int         ret;

#if defined(ECOS_OS)
	timeout = -1;
#elif defined(LINUX_OS)
	timeout = 0;
#endif
	if(NO==0)
	{
		ret = GxCore_QueueGet(queueId1, &ca_msg1, sizeof(CA_MSG), &sz,timeout);
		if (ret == GXCORE_SUCCESS)
		{
			if (0 == sz )
			{
				CAS_Dbg("STBCA_GETCAMSG sz = 0...\n");
				return NULL;
			}
			
			return &ca_msg1;
		}
		else
		{
			CAS_Dbg("STBCA_GETCAMSG Return value is error...\n");
			return NULL;
		}
	}
	else
	{
		ret = GxCore_QueueGet(queueId2, &ca_msg2, sizeof(CA_MSG), &sz,timeout);
		if (ret == GXCORE_SUCCESS)
		{
			if (0 == sz )
			{
				CAS_Dbg("STBCA_GETCAMSG sz = 0...\n");
				return NULL;
			}
			
			return &ca_msg2;
		}
		else
		{
			CAS_Dbg("STBCA_GETCAMSG Return value is error...\n");
			return NULL;
		}
	}
}
/*++
功能：在第NO个消息队列中增加一条消息，如果队列满则删除所有的消息后增加消息。
参数：
	static CA_MSG msg1;
	msg1.msgtype = 1;
	msg1.msgparm1 = 0x33;
	msg1.msgparm2 = 0x44;
	
	STBCA_SENDCAMSG(0,&msg1);
	STBCA_SENDCAMSG(1,&msg1);
--*/
void STBCA_SENDCAMSG(BU8 NO,CA_MSG* msg)
{
	int   ret ;
	
	if(msg == NULL)
	{
		CAS_Dbg("XinShiMao_OSPQueueSendMessage Parameter is NULL\n");
		return;
	}
	handle_t queue_id;
	if(NO==0)
	{
		 queue_id=queueId1;
	}
	else
	{
		 queue_id=queueId2;
	}	
	ret = GxCore_QueuePut(queue_id, msg, sizeof(CA_MSG),0);

	if(ret == GXCORE_SUCCESS )
	{
		
	}
	else
	{
		CAS_Dbg("STBCA_SENDCAMSG pipeID=0x%x Error\n", (unsigned int)queue_id);

	}

}

void STBCA_ReleaseCAMSG(BU8 NO,CA_MSG* msg)
{
	CAS_Dbg("STBCA_ReleaseCAMSG NO=0x%x Error\n", NO);
}

BU8 STBCA_GetSCStatus(void )
{


	if (TRUE == app_porting_ca_smart_status())
		//*pbyStatus = BYCAS_SC_IN;
		return 1;
	else
		//*pbyStatus = BYCAS_SC_OUT;
		return  0;
}

/*++
功能：获得机顶盒分配给CAS的存储空间的起点地址和大小（以字节为单位）。
参数：
	ppStartAddr:			机顶盒分配给CAS的存储空间的开始地址。
	lSize:					输出机顶盒分配给CAS的储存空间的大小。
--*/
//BU8 BYSTBCA_GetDataBufferAddr(BU32* lSize,BU32 * ppStartAddr)
void STBCA_GetDataBufferAddr(BU32* lSize, BU32* ppStartAddr)
{
	if ((NULL == lSize)||(NULL ==ppStartAddr ))
		{
		  	CAS_Dbg("BYSTBCA_GetDataBufferAddr lSize=0x%x ppStartAddr=0x%x\n",(unsigned int)lSize,(unsigned int)ppStartAddr);		
			//return 0;
		}

	
    *ppStartAddr = ONE_SECTOR_SIZE;
	*lSize = ONE_SECTOR_SIZE;
	//return 1;
}

/*++
功能：读取保存在机顶盒中的信息。
参数：
	pStartAddr:				要读取的存储空间的开始地址。
	plDataLen:				输入为要读取的最长数据值；输出为实际读取的大小。
	pData:					存放输出数据。
--*/
//BU8 BYSTBCA_ReadDataBuffer(BU32 pStartAddr,BU32* plDataLen,BU8* pData)
BU8 STBCA_ReadDataBuffer(BU32* pStartAddr, BU32* plDataLen, BYTE* pData)
{
	uint32_t  Offset = *pStartAddr-ONE_SECTOR_SIZE;
	
       if ((NULL ==plDataLen )||(NULL == pData))
       	{
		  	CAS_Dbg("STBCA_ReadDataBuffer plDataLen=0x%x pData=0x%x\n",(unsigned int)plDataLen,(unsigned int)pData);		       	
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
//BU8 BYSTBCA_WriteDataBuffer(BU32 pStartAddr,BU32 DataLen,BU8* pData)
BU8 STBCA_WriteDataBuffer(BU32 pStartAddr, BU32 plDataLen, const BYTE* pData)
{
	uint32_t  Offset = pStartAddr-ONE_SECTOR_SIZE;

	if (NULL == pData)
		{
			CAS_Dbg("BYSTBCA_WriteDataBuffer pData=0x%x \n",(unsigned int)pData);		       	
			return 0;
		}
	
	if (Offset + plDataLen> ONE_SECTOR_SIZE)
		{
		  	CAS_Dbg("BYSTBCA_WriteDataBuffer Offset=0x%x DataLen=0x%x\n",(unsigned int)Offset,(unsigned int)plDataLen);		       	
			return 0;
		}
	
	return	app_porting_ca_flash_write_data(Offset,(uint8_t *)pData,plDataLen);
	
}


/*++
功能：挂起当前线程。
参数：
	dwmsec:			毫秒。
--*/
//BU8 BYSTBCA_Sleep(BU32 dwmsec)
void STBCA_Sleep( int dwMicroseconds )
{

	app_porting_ca_os_sleep(dwMicroseconds);
	//return 1;
}

/*++
功能：初始化信号量,初始化后信号量没有信号。
参数：pSemaphore:				指向信号量的指针。
	    flag:						  初始化信号量值
--*/
//BU8 BYSTBCA_SemaphoreInit(BY_Semaphore *pSemaphore,BU8 flag)
BU8 STBCA_SemaphoreInit( CA_Semaphore *pSemaphore ,BU8 flag)
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
#if 0
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
#endif
/*++
功能：给信号量加信号。
参数：pSemaphore: 			指向信号量的指针。
--*/
//BU8 BYSTBCA_SemaphoreSignal(BY_Semaphore *pSemaphore )
void STBCA_SemaphoreSignal( CA_Semaphore *pSemaphore )
{
	if (NULL == pSemaphore)
		{
			CAS_Dbg("BYSTBCA_SemaphoreSignal pSemaphore=0x%x \n",(unsigned int)pSemaphore);		       	
			return ;
		}
	
	app_porting_ca_os_sem_signal((handle_t)*pSemaphore);
	return ;
}

/*++
功能：等待信号量,等待成功之后,信号量为无信号。
参数：pSemaphore: 			指向信号量的指针。
--*/
void STBCA_SemaphoreWait( CA_Semaphore *pSemaphore )
//BU8 BYSTBCA_SemaphoreWait(BY_Semaphore *pSemaphore )
{
	if (NULL == pSemaphore)
		{
			CAS_Dbg("BYSTBCA_SemaphoreWait pSemaphore=0x%x \n",(unsigned int)pSemaphore);		       	
			return ;
		}
	
	app_porting_ca_os_sem_wait((handle_t)*pSemaphore);
	return ;
}

/*++
功能：CAS模块向机顶盒注册线程。
参数：
	szName:					注册的线程名称。
	pTaskFun:				要注册的线程函数。
--*/
//BU8 BYSTBCA_RegisterTask(const BS8* szName,pThreadFunc pTaskFun)
BU8	STBCA_RegisterTask(const char* szName, pThreadFunc pTaskFun)
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
//BU8 BYSTBCA_SetStreamGuardFilter(BU8 byReqID,BU16 wPID,BU8* szFilter,BU8* szMask,BU8 byLen,BU16 nWaitSecs)
BU8 STBCA_SetStreamGuardFilter(BYTE byReqID, WORD wPID, const BYTE* szFilter, const BYTE* szMask,BYTE byLen, int nWaitSecs)
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

 	CAS_Dbg("STBCA_SetStreamGuardFilter byReqID=0x%x,wPID=0x%x byLen=0x%x,nWaitSecs=%d\n",byReqID,wPID,byLen,nWaitSecs);

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
	      sfilter = app_porting_ca_demux_get_by_filter_id(filterID,&filter);
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
	filter.equalFlag = TRUE,
	filter.repeatFlag = repeatFlag;
	filter.Callback = app_qilian_cas_filter_notify;
	filter.timeOutCallback = app_qilian_cas_filter_timeout_notify;
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
//BU8 BYSTBCA_SetDescrCW(BU16 wEcmPid,BU8 byKeyLen,const BU8* szOddKey,const BU8* szEvenKey,BU8 bTaingControl)
void STBCA_ScrSetCW( BU16  wEcmPid, const BU8* szOddKey, const BU8* szEvenKey,BU8 byKeyLen)
{

	app_porting_ca_descramble_set_cw(wEcmPid,szOddKey,szEvenKey,byKeyLen,0);

	//return 1;
}

/*++
功能：打印调试信息。
参数：Param1,Param2,pszMsg:					调试信息内容。
--*/
//void BYSTBCA_AddDebugMsg(BU32 Param1,BU32 Param2,const BS8 *pszMsg)
void STBCA_AddDebugMsg(const char *pszMsg)
{
	if (NULL != pszMsg)
		//printf("[0x%x][0x%x] %s \n",(unsigned int)Param1,(unsigned int)Param2,pszMsg);
	//else
		//printf("[0x%x][0x%x] \n",(unsigned int)Param1,(unsigned int)Param2);	
		printf("%s \n",pszMsg);
}


/*++
功能：当用户插卡时,先调用该函数复位卡
返回:     true  成功，false失败
参数：pATR 最大64 字节
--*/
void STBCA_SMARTCARD_ReSet(void)
//BU8 BYCASTB_SCReset(BU8 *pATR)
{

  	    uint8_t                     len;
	    uint8_t               atr[256] = {0};
	int nRetValue = FALSE;

  	CAS_Dbg("[SC] BYCASTB_SCReset!\n");

        nRetValue = app_qilian_cas_api_smart_reset(atr,&len);

	if (TRUE == nRetValue)
    	{
    		//memcpy(pATR,atr,len);
    		return ;
    	}
    else
    	{
     		return ;
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
//BU8 BYSTBCA_SCAPDU(BU8 byReaderNo,const BU8* byszCommand,BU16* pbyLen,BU8* byszReply)
BU8 STBCA_SCAPDU(BU8 num, BYTE* byszCommand,BU32* bReplyLen, BYTE* byszReply)
{
    uint8_t reply[1024] = {0};
    uint8_t sc_result;
    uint16_t s_byLen=*bReplyLen&0XFFFF;
    uint16_t r_byLen=0;
    sc_result =  app_porting_ca_smart_sc_apdu(byszCommand,s_byLen,reply,&r_byLen);
    if (TRUE == sc_result)
    	{
    		*bReplyLen=r_byLen;
    		memcpy(byszReply, reply, r_byLen);	
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
//BVOID   BYSTBCA_ShowPromptMessage(BU8 byMesageNo)
void   STBCA_ShowOSDMessage(BYTE byMesageNo)
{
	#if 0
	if (MSG_POP_COMMON_END+byMesageNo+1 >=64)
		return ;

	if ( TRUE != app_play_get_msg_pop_type_state(MSG_POP_COMMON_END+byMesageNo+1))
		{
			app_play_clear_ca_msg();
			app_play_set_msg_pop_type(MSG_POP_COMMON_END+byMesageNo+1);
		}
	#endif
	switch( byMesageNo)
	{
		case STBCA_NO_ERROR:
		case STATUE_CA_OSD_CLS:
			app_play_clear_ca_msg();
			break;
		case STBCA_ERROR_InitCardCommand	: //							1   //智能卡初始化错误
		case STBCA_ERROR_ReadCardCommand	: //								2	// 智能卡无法识别
		case STBCA_ERROR_NO_CARD			: //										3	//请检查智能卡
		case STBCA_ERROR_EMM_RET				://								4  // 无效的EMM 数据
		case STBCA_ERROR_ECM_RET				: //									5	//无效的ECM数据
		case STBCA_ECM_RET_READCOMM_ERROR		: //							6
		case STBCA_EMMECM_RET_P1P2_ERROR		: //							7    //无效指令
		case STBCA_EMMECM_RET_P3_ERROR		: //								8	// 数据格式无效
		case STBCA_EMMECM_RET_DES_ERROR		: //								9
		case STBCA_ECM_RET_NOENTITLE_ERROR		: //						10			//当前服务没有授权
		case STBCA_EMMECM_RET_STORE_ERROR		: //							11
		case STBCA_EMMECM_RET_CHECK_ERROR		: //							12			// CRC   错误
		case STBCA_EMM_NOKEY_OR_NOFILE_ERROR	: //						13
		case STBCA_ECM_OUTDATE_ERROR			: //								14
		case STBCA_EMMECM_RET_UNKNOW_ERROR	: //				  		15
		case STBCA_NO_ECM_INFO				: //									16
		case STBCA_ZONE_FORBID_ERROR			: //								17  // 区域限播无效
		case STBCA_UserCdn_Limit_Ctrl_Enable		: //								18			// 条件禁止播放
		case STBCA_UserCdn_Limit_Ctrl_Disable		: //								19			// 条件禁播无效
		case STBCA_ZoneCdn_Limit_Ctrl_Enable		: //								20			// 条件禁止播放
		case STBCA_ZoneCdn_Limit_Ctrl_Disable		: //							21			// 条件禁播无效
		//机卡配对
		case STBCA_CARD_NEED_PAIRING			: //								22   // 没有机卡配对
		case STATUE_CA_SET_PAIR					: //								23	//  is pairing   正在设置配对信息，请等待
		case STBCA_CARD_PAIRING_OK				: //								24	// 机卡配对成功，清除31、16  24 25的消息
		case STBCA_PAIRED_OtherCARD_ERROR		: //								25 /*该机器与其它卡配对*/
		case STBCA_PAIRED_OtherMachine_ERROR		: //							26 /*该卡与此机器不配对*/

		//字母卡配对STATUE_CA_TIME_VAILD
		case	STATUE_CA_CHILD_SC				: //						27 //字母卡等待配对
		case STATUE_CA_TIME_VAILD                               : //	          			28// 字母卡配对时间已更新
		case STATUE_CA_NEED_PAIR				: //						29  //"已过有效期,请把卡插入主机配对"
		//漫游
		case STATUE_CA_CARD_ZONE_INVALID		: //						30		//您的智能卡不再服务区
		case  STATUE_CA_CARD_ZONE_CHECK_ENTER     : //	            			31    /* 正在寻找网络，请等待*/
		case  STATUE_CA_CARD_ZONE_CHEKC_OK           : //	            			32         // 当stb得到该消息后，区域检测完毕，
			if ( TRUE != app_play_get_msg_pop_type_state(MSG_POP_COMMON_END+byMesageNo+1))
			{
				app_play_clear_ca_msg();
				app_play_set_msg_pop_type(MSG_POP_COMMON_END+byMesageNo+1);
			}
			break;
		default:
			break;
	}
}
#if 0
BVOID   BYSTBCA_HidePromptMessage(BVOID)
{
	app_play_clear_ca_msg();
}
#endif
/*
功能：显示指纹信息。
参数：
	wEcmPid					ECM PID
	dwCardID:				卡号。
	wDuration:				持续秒数。
--*/
/*++
功能：显示指纹信息，以图标方式显示指纹,显示位置一般和邮件其它图标不重叠
参数：szOSD == NULL 时清除屏幕已显示的消息。
--*/
static char *finger=NULL;
void STBCA_FingerSHOW(const char* szOSD)
{
	//if ((dwCardID == fingerdata.dwCardID)&&(wEcmPid == fingerdata.wEcmPid)
	//	&&(0x1fff == wEcmPid)&&(0xffffffff == dwCardID))
	//	return ;

	//printf("BYSTBCA_ShowFingerPrinting wEcmPid=0x%x,dwCardID=0x%x,wDuration=0x%x\n",
	//	(unsigned int)wEcmPid,(unsigned int)dwCardID,(unsigned int)wDuration);


	//fingerdata.dwCardID = dwCardID;
	//fingerdata.wDuration = wDuration;
	//fingerdata.wEcmPid = wEcmPid;
		finger=szOSD;
		 if(szOSD==NULL)
		{
			fingerdata.wStatus = 0;
			fingerdata.data=NULL;
			
		}
		 else
		 {
			fingerdata.wStatus = 1;	
			fingerdata.data=szOSD;
		 }

	app_cas_api_on_event(DVB_CA_FINGER_INFO,CAS_NAME, &fingerdata, sizeof(qilian_ca_finger_data_st));
//	GxCore_ThreadDelay(20); 
}
#if 0
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

	app_cas_api_on_event(DVB_CA_FINGER_INFO,CAS_NAME, &fingerdata, sizeof(qilian_ca_finger_data_st));
//	GxCore_ThreadDelay(20); 
}
#endif
/*--------------------------- 类型2的界面：用户可以取消显示的界面 -----------------------*/
/*
功能：以左滚字幕的方式显示OSD消息，可以是同步显示，也可以是异步显示。
参数:	
	szOSD:					用于显示的OSD信息。
	wDuration:				0:显示一次后立即返回;其他值为显示的具体持续秒数。
说明：
	用户可以通过遥控器取消当前OSD显示。
--*/
char buf_osd_msg0[200]={0};
void   STBCA_ShowOSDMsg(const char* szOSD, WORD wDuration)

//BVOID	BYSTBCA_ShowOSDMsg(const BS8* szTitle,const BS8* szOSD,BU16 wDuration)
{
	if((wDuration==0 )&&( szOSD!=NULL))
	{
		memset(buf_osd_msg0,0,100);
		if(strlen(szOSD)>100)
		{
			memcpy(buf_osd_msg0,szOSD,99);
		}
		else
		{
			memcpy(buf_osd_msg0,szOSD,strlen(szOSD));
		}
		#if 1
		app_play_clear_ca_msg();
		app_cas_api_on_event(DVB_CA_QILIAN_POP_MSG,CAS_NAME, "qilian_cas", strlen("qilian_cas"));
		#else
		if ( TRUE != app_play_get_msg_pop_type_state(MSG_POP_COMMON_END+33+1))
		{
			app_play_clear_ca_msg();
			app_play_set_msg_pop_type(MSG_POP_COMMON_END+33+1);
		}
		#endif
		return;
	}
	int i = 0;
	int j=0;

	printf("BYSTBCA_ShowOSDMsg szOSD=%s\n",szOSD);

	memset(&rollingmsg,0,sizeof(qilian_ca_rolling_message_st))	;
	rollingmsg.wDuration = wDuration;


	//if ((NULL == szTitle)||(NULL == szOSD))
	if ((NULL == szOSD))
		{
			return;
		}
#if 0
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
#endif
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
	app_cas_api_on_event(DVB_CA_ROLLING_INFO,CAS_NAME, &rollingmsg, sizeof(qilian_ca_rolling_message_st));
//	GxCore_ThreadDelay(20); 
}

/*
功能: 提供随机数据
参数: blen 长度pbBuf 返回blen个随机数
*/

BU8 STBCA_RAND_GET( void )
{
	static BU32 seed=1;
	srand(seed);
	seed++;
	BU8 re_rand=rand();
	printf("rand=0x%x",re_rand);
	return re_rand;
}

/*
功能：通知APP锁定某个频道信号，在该特定的频点寻找漫游信息。
参数:	KHZ;
返回:  1，表示频点已锁到，否则未锁定该频点。
--*/
//BU8 BYCA_SetZoneFreq( BU32 Freq )
BU8 STBCA_SetZoneFreq( BU32 Freq )
{
	uint32_t symbol_rate = 0;
	uint32_t qam = 0;
	uint32_t freq = Freq;///1000;

	//if (TRUE == bat_recieve_flag)
	//	return 1;

	symbol_rate = app_flash_get_config_center_freq_symbol_rate();
	qam = app_flash_get_config_center_freq_qam();
	printf("%s Freq=%d",__FUNCTION__,Freq);
	if (0 == app_search_lock_tp(freq, symbol_rate,INVERSION_OFF, qam,1000))
	{
		printf("STBCA_SetZoneFreq lock\n");
		return 1;//app_qilian_cas_api_bat_filter_open();
	}
	else
	{
		printf("STBCA_SetZoneFreq unlock\n");
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
/*++
	功能: 通知菜单，显示邮件图标
	 参数
	 0,表示显示新邮件图标
	 1，表示显示邮箱已满
	 2, 表示没有新邮件图标
*/
#if 0
typedef enum
{
	DVB_CA_EMAIL_FLAG_HIDE=0,		       /*hide email flag */
	DVB_CA_EMAIL_NEW,		/*new email flag show*/		
	DVB_CA_EMAIL_EXHAUST			/*email space exhaust*/
}dvb_ca_email_flag_type_t;
#endif
#define	BYCAS_NEW_EAMIL						0/*新邮件通知。*/
#define     BYCAS_NEW_EMAIL_NO_ROOM		1/*有新邮件，但Email空间不够。*/
#define	BYCAS_EMAIL_NONE					2/*没有新邮件。*/
void STBCA_EmailNotify(BU8 byShow)
//BVOID BYSTBCA_EmailNotify(BU8 byShow)
{
#if 1
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

	app_cas_api_on_event(DVB_CA_EMAIL_INFO,CAS_NAME, &emailflag, sizeof(qilian_cas_email_st));
//	GxCore_ThreadDelay(20); 
	#endif
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
#if 0
BVOID BYCASTBCA_UrgencyBroad(BU16 wOriNetID,BU16 wTSID,BU16 wServiceID,BU8 *pUTCTime)
{
	printf(" BYCASTBCA_UrgencyBroad \n");
	return app_qilian_cas_api_lock_service(wOriNetID,wTSID,wServiceID,pUTCTime);
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
	app_qilian_cas_api_set_lock_status(FALSE);
	
	return;
}
#endif
