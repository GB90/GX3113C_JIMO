/*****************************************************************************
* 						   CONFIDENTIAL
*        Hangzhou GuoXin Science and Technology Co., Ltd.
*                      (C)2012, All right reserved
******************************************************************************

******************************************************************************
* File Name :	dvt_cas_porting.c
* Author    : 	zhouhm
* Project   :	goxceed dvbc
* Type      :
******************************************************************************
* Purpose   :	模块头文件
******************************************************************************
* Release History:
VERSION	Date			  AUTHOR         Description
1.0  	2013.11.29		  zhouhm 	 			creation
*****************************************************************************/
#include <stdlib.h>
#include <stdarg.h>
#include <gxcore.h>
#include "DVTCAS_STBInf.h"
#include "DVTSTB_CASInf.h"
#include "app_common_porting_ca_flash.h"
#include "app_common_porting_ca_demux.h"
#include "app_common_porting_ca_os.h"
#include "app_common_porting_ca_smart.h"
#include "app_common_porting_ca_descramble.h"
#include "app_common_play.h"
#include "app_common_prog.h"
#include "app_dvt_cas_api_demux.h"
#include "app_dvt_cas_api_pop.h"
#include "app_common_porting_stb_api.h"
#include "app_dvt_cas_api_smart.h"
#include "gxplayer_url.h"
#include "service/gxplayer.h"
#include "gxmsg.h"
#include "gui_key.h"
#include "gxprogram_manage_berkeley.h"
#include "app_common_play.h"
#include "app_common_lcn.h"
#include "app_dvt_cas_api_lock_service.h"
#include "gui_core.h"


#define FLASH_CA_DATA_A_START_ADDR  (0x10000)
#define FLASH_CA_DATA_SIZE (2*64*1024)
#define MAX_SEARIAL_LEN (17)

static dvt_cas_finger_data_st  fingerdata = {0};
static dvt_cas_rolling_message_st  rollingmsg = {0};
static dvt_cas_email_st emailflag= {0};

#define DVT_CA_CENTER_FRE           "DvtCACenterFre"
#define DVT_CA_CENTER_FRE_DV         235
//#define DVT_CA_CENTER_FRE_DV      387000///yiwu

HRESULT DVTSTBCA_ReleasePrivateDataFilter(uint8_t byReqID, uint16_t wPid)
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

	return DVTCA_OK;

}


/*++
功能：获得机顶盒分配给CAS的flash空间的起点地址和大小（以字节为单位）。
参数：
	ppStartAddr:			机顶盒分配给CAS的flash空间的开始地址。
	lSize:					输出机顶盒分配给CAS的flash空间的大小。
--*/
HRESULT DVTSTBCA_GetDataBufferAddr(long * lSize, char ** ppStartAddr)
{
	if ((NULL == lSize)||(NULL == ppStartAddr))
		{
			return DVTCAERR_STB_POINTER_INVALID;
		}
	*ppStartAddr = (char*)FLASH_CA_DATA_A_START_ADDR;
	*lSize		 = (long)FLASH_CA_DATA_SIZE;
	return DVTCA_OK;
}

/*++
功能：读取保存在机顶盒flash中的信息。
参数：
	pStartAddr:				要读取的存储空间的开始地址。
	plDataLen:				输入为要读取的最长数据值；输出为实际读取的大小。
	pData:					存放输出数据。
--*/
HRESULT DVTSTBCA_ReadDataBuffer(const char * pStartAddr, long * plDataLen, BYTE * pData)
{
	size_t readSize = (size_t)(*plDataLen);
	uint32_t  Offset = (uint32_t)(pStartAddr-FLASH_CA_DATA_A_START_ADDR);
	if ((NULL == pStartAddr)||(NULL == plDataLen)||(NULL == pData))
	{
		CA_FAIL(" DVTSTBCA_ReadDataBuffer   failed \n");
		return DVTCAERR_STB_POINTER_INVALID;
	}

	app_porting_ca_flash_read_data(Offset,(uint8_t *)pData,(uint32_t*)&readSize);
	
	return DVTCA_OK;
}

/*++
功能：向机顶盒的存储空间写信息。
参数：
	lStartAddr:				要写的存储空间的开始地址。
	plDataLen:				输入为要写的数据的长度；输出为写入的实际长度。
	pData:					要写的数据。
--*/
HRESULT DVTSTBCA_WriteDataBuffer(const char * pStartAddr, long * plDataLen, const BYTE * pData)
{
	size_t writeSize = (size_t)(*plDataLen);
	if ((NULL == pStartAddr)||(NULL == plDataLen)||(NULL == pData))
	{
		return DVTCAERR_STB_POINTER_INVALID;
	}
	uint32_t  Offset = (uint32_t)(pStartAddr-FLASH_CA_DATA_A_START_ADDR);

	app_porting_ca_flash_write_data(Offset,(uint8_t*)pData,writeSize);

	return DVTCA_OK;
}

/*++
功能：读取保存在机顶盒eeprom中的信息。
参数：
	plDataLen:				输入为要读取的最长数据值；输出为实际读取的大小。
	pData:					存放输出数据。
--*/
HRESULT DVTSTBCA_GetDataFromEeprom(long * plDataLen, BYTE * pData)
{
	return (HRESULT)-1;
}

/*++
功能：向机顶盒的eeprom存储空间写信息。
参数：
	plDataLen:				输入为要写的数据的长度；输出为写入的实际长度。
	pData:					要写的数据。
--*/
HRESULT DVTSTBCA_SaveDataToEeprom(long * plDataLen, const BYTE * pData)
{
	return (HRESULT)-1;
}

/*++
功能：挂起当前线程。
参数：
	dwMicroseconds:			毫秒。
--*/
HRESULT DVTSTBCA_Sleep( int dwMicroseconds )
{
	app_porting_ca_os_sleep(dwMicroseconds);
	return DVTCA_OK;
}

/*++
功能：初始化信号量,初始化后信号量没有信号。
参数：
	pSemaphore:				指向信号量的指针。
--*/
HRESULT DVTSTBCA_SemaphoreInit( DVTCA_Semaphore * pSemaphore )
{
	handle_t semaphore = 0;
	if (NULL == pSemaphore)
		{
		return DVTCAERR_STB_POINTER_INVALID;
		}
	
    app_porting_ca_os_sem_create(&semaphore,0);

	*pSemaphore = (handle_t)semaphore;
	return DVTCA_OK;
}

/*++
功能：给信号量加信号。
参数：
	pSemaphore: 			指向信号量的指针。
--*/
HRESULT DVTSTBCA_SemaphoreSignal( DVTCA_Semaphore * pSemaphore )
{
	if (NULL == pSemaphore)
		{
		return DVTCAERR_STB_POINTER_INVALID;
		}
	app_porting_ca_os_sem_signal((handle_t)*pSemaphore);

	return DVTCA_OK;
}

/*++
功能：等待信号量,等待成功之后,信号量为无信号。
参数：
	pSemaphore: 			指向信号量的指针。
--*/
HRESULT DVTSTBCA_SemaphoreWait( DVTCA_Semaphore * pSemaphore )
{
	if (NULL == pSemaphore)
		{
		return DVTCAERR_STB_POINTER_INVALID;
		}
	app_porting_ca_os_sem_wait((handle_t)*pSemaphore);
	return DVTCA_OK;
}

/*++
功能：CAS模块向机顶盒注册线程。
参数：
	szName:					注册的线程名称。
	pTaskFun:				要注册的线程函数。
--*/
HRESULT DVTSTBCA_RegisterTask(const char * szName, pThreadFunc pTaskFun)
{
    int         ret;
    handle_t    handle;
    uint32_t priority = GXOS_DEFAULT_PRIORITY;


	if ((NULL == szName)||(NULL == pTaskFun))
	{
		return DVTCAERR_STB_POINTER_INVALID;
	}

	if(0 == strcmp(szName,"DVTEcm_DisposeTask"))
	{
		//priority = GXOS_DEFAULT_PRIORITY-1;
		priority = GXOS_DEFAULT_PRIORITY-2;

	}
	else
	{
		//priority = GXOS_DEFAULT_PRIORITY+1;
		priority = GXOS_DEFAULT_PRIORITY-1;
 	}


    ret = app_porting_ca_os_create_task(szName, &handle, (void*)pTaskFun,
                              NULL, 20*1024, priority);

	if (ret != GXCORE_SUCCESS)
	{
		CA_FAIL("DVTSTBCA_RegisterTask return error = 0x%x\n", ret);
		return (HRESULT)-1;
	}
	
	return DVTCA_OK;

}


/*++
功能：设置过滤器接收CAS私有数据。
参数：
	byReqID:				请求收表的请求号。机顶盒应该保存该数据，接收到CAS私有数据后调用DVTCASTB_StreamGuardDataGot()时应该将该数据返回。
	wEcmPID:				需要过滤的流的PID。
	szFilter:				过滤器的值，为一个8个字节的数组。
	szMask:					过滤器的掩码，为一个8个字节的数组，与过滤器对应，当szMask的某一bit位为0时表示要过滤的表对应的bit位不需要与szFilter里对应的bit位相同，当szMask某一位为1时表示要过滤的表对应的bit位一定要与szFilter里对应的bit位相同。
	byLen:					过滤器的长度，为8。
	nWaitSeconds:			收此表最长的等待时间，如果为0则无限等待，单位秒。
--*/
HRESULT DVTSTBCA_SetStreamGuardFilter(BYTE byReqID, WORD wPID, const BYTE * szFilter, const BYTE * szMask, BYTE byLen, int nWaitSeconds)
{
	ca_filter_t filter = {0};
	bool repeatFlag = TRUE;
	uint32_t filterId = 0;
	ca_filter_t* sfilter = NULL;
//	int i = 0;
//	bool repeatFlag = FALSE;
//	uint8_t     match[18] = {0,};
//	uint8_t     mask[18] = {0,};
//	handle_t channelhandle=0;
	GxTime starttime={0};
	uint32_t starts;

/*	if (DVTCA_STB_ECM_SLOT != byReqID)
		{
			printf("byReqID=%d byLen = %d\n",byReqID,byLen);
			for (i = 0; i<byLen;i++)
				{
					printf("%02x ",szFilter[i]);
				}
			printf("\n");

			printf("\n");
			for (i = 0; i<byLen;i++)
				{
					printf("%02x ",szMask[i]);
				}
			printf("\n");		
		}*/



	if(szFilter == NULL || szMask == NULL )
	{
		return DVTCAERR_STB_POINTER_INVALID;
	}
	// 如果过滤条件完全相同，仅RESET

/*	if ((DVTCA_STB_ECM_SLOT == byReqID)||(DVTCA_STB_EMM_Private_SLOT == byReqID))
		{
			;
		}
	else
		{
			return DVTCA_OK;
		}*/

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
					GxCore_GetTickTime(&starttime);
					starts = starttime.seconds;
//					app_porting_ca_demux_lock();
					sfilter->nms = starts;
					sfilter->nWaitSeconds = nWaitSeconds;
//					app_porting_ca_demux_unlock();
					return DVTCA_OK;
				}
		}
	}
#endif

	DVTSTBCA_ReleasePrivateDataFilter(byReqID,wPID); 
	memset(&filter,0,sizeof(ca_filter_t));
	filter.byReqID = byReqID;
	filter.crcFlag = FALSE/*TRUE*/;
	filter.filterLen = byLen;
	memcpy(filter.match,szFilter,byLen);
	memcpy(filter.mask,szMask,byLen);		
	filter.nWaitSeconds = nWaitSeconds;
	filter.pid = wPID;
	filter.equalFlag = TRUE;
	filter.repeatFlag = repeatFlag;
	filter.Callback = app_dvt_cas_filter_notify;
	filter.timeOutCallback = app_dvt_cas_filter_timeout_notify;
	app_porting_ca_demux_start_filter(&filter);

	return DVTCA_OK;
}



/*++
功能：CA程序用此函数设置解扰器。将当前周期及下一周期的CW送给解扰器。
参数：
	wEcmPID:				CW所属的ECMPID。
	szOddKey:				奇CW的数据。
	szEvenKey:				偶CW的数据。
	byKeyLen:				CW的长度。
	bTapingControl:			true：允许录像,false：不允许录像。
--*/
HRESULT DVTSTBCA_SetDescrCW(WORD wEcmPID, BYTE byKeyLen, const BYTE * szOddKey, const BYTE * szEvenKey, bool bTapingControl)
{
	app_porting_ca_descramble_set_cw(wEcmPID,szOddKey,szEvenKey,byKeyLen,bTapingControl);
	return DVTCA_OK;
}



/*++
功能：打印调试信息。
参数：
	pszMsg:					调试信息内容。
--*/
void DVTSTBCA_AddDebugMsg(const char * pszMsg)
{
	printf(( char * ) pszMsg );
	printf("\n" );
}





/*++
功能:实现内存拷贝(针对ST71平台，如果源区域与目标区域重叠，
可能导致内存拷贝错误，需要重新定义)。
返回值:
pDest:目标地址
pSrc:  源地址
iSize:  大小
--*/
HRESULT DVTSTBCA_Memcpy(void  * pDest, const void * pSrc,  unsigned int iSize)
{
    memcpy(pDest, pSrc, iSize);
	return DVTCA_OK;
}




/*++
功能:获取芯片的CPUID。
参数:
	pbyCPUID:	芯片提供的CPUID，17个字节,用于内蒙机卡配对
返回值:
	0:		获取CPUID成功
	其他值:	获取CPUID失败
--*/
HRESULT DVTSTBCA_GetCPUID(BYTE * pbyCPUID)
{
	char caUser[MAX_SEARIAL_LEN+1]={0};
	app_flash_get_serial_number(0,(char *)caUser,MAX_SEARIAL_LEN);

    memcpy(pbyCPUID, caUser, 17);
	return DVTCA_OK;
}

#ifdef DVTCA_VERSION_NITAREALOCK
/*++
功能:获取NIT中的值，用于和区域码对比。
参数:
	pdwData:	机顶盒提供的从NIT中获取的值
返回值:
	0:		获取成功
	其它值:	获取失败
--*/
HRESULT	DVTSTBCA_GetNitValue(DWORD * pdwData)
{
	CAS_Dbg(" DVTSTBCA_GetNitValue \n"); 
	int32_t PDSDVaule = 0;
	int32_t DvtPDSDValue_flag = 0;
	GxBus_ConfigGetInt("DvtPDSDValue_flag",&DvtPDSDValue_flag, 0);
	if(DvtPDSDValue_flag==1)
	{
		GxBus_ConfigGetInt("DvtPDSDValue", &PDSDVaule, 0);
		if (0 != PDSDVaule)
		{
			*pdwData = (DWORD)PDSDVaule;
			return DVTCA_OK;	
		}
	    else
		{
			*pdwData = 0;	
			return (HRESULT)-1;		
		}
	}
	else
	{
			*pdwData = 0;	
			return (HRESULT)-1;		
	}
}
#endif
/*++
功能：获取机顶盒软件版本。
返回值：机顶盒软件版本号。
--*/
DWORD DVTSTBCA_GetSoftVer(void)
{
	uint32_t gsoftversion=0;
//	gsoftversion = get_softversion();
	CAS_Dbg(" DVTSTBCA_GetSoftVer gsoftversion=0x%x\n",gsoftversion); 
	return (DWORD )gsoftversion;
}

/*++
功能：显示新邮件的标志。
参数：
	byShow:					标志类型。含义如下：
							DVTCAS_NEW_EAMIL				新邮件通知。
							DVTCAS_NEW_EMAIL_NO_ROOM		有新邮件，但Email空间不够。
							DVTCAS_EMAIL_NONE				没有新邮件。
--*/
void DVTSTBCA_EmailNotify(BYTE byShow)
{
	CAS_Dbg(" DVTSTBCA_EmailNotify byShow=%d\n",byShow); 
	switch(byShow)
	{
		case DVTCAS_EMAIL_NEW:
			{
				emailflag.wStatus = DVB_CA_EMAIL_NEW;				
			}
			break;
		case DVTCAS_EMAIL_NEW_NO_ROOM:
			{
				emailflag.wStatus = DVB_CA_EMAIL_EXHAUST;				
			}
			break;		
		case DVTCAS_EMAIL_NONE:
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

	app_cas_api_on_event(DVB_CA_EMAIL_INFO,CAS_NAME, &emailflag, sizeof(dvt_cas_email_st));
	
	return;
}



/*++
功能：弹出框询问用户是否预订当前IPP节目。
参数：
	wEcmPid:				IPP对应的EcmPID
	pIpp:					当前IPP的信息。
--*/
void DVTSTBCA_InquireBookIpp(WORD wEcmPid, const SDVTCAIpp * pIpp )
{
	return;
}


/*分区控制*/
//int g_GdSwitchChannelFre = 0;//2510000;
uint16_t g_SwServiceID = 0;  
uint16_t g_TsID = 0;  
//int g_SwFrequency = 0; 
int g_SwitchChannelTicks = 0;
bool g_SwitchChannelAllFlg = FALSE;
bool g_bSwitchTask = FALSE;
bool g_bAreaLockOK = FALSE;
static int s_BakChannelTicks = 0;

int DVTSTBCA_GetProgPosByFrequency(uint16_t fre)
{
	GxBusPmDataTP tp = {0};
	int16_t prog_count = 0;
	int16_t prog_pos = 0;
	GxBusPmDataProg Prog;


	prog_count = GxBus_PmProgNumGet();
	for(prog_pos = 0;prog_pos<prog_count; prog_pos++)
	{
		GxBus_PmProgGetByPos(prog_pos, 1, &Prog);
		GxBus_PmTpGetById(Prog.tp_id,&tp);
		if (fre == tp.frequency)
		{
			return (int)prog_pos;
		}
	}

	return -1;

}

	
/*禁用/ 启用前面板和遥控器响应 */
void JudgeSwitchChannel( void* arg ) 
{ 
	int32_t serviceIndex= 0;
	GxBusPmDataProg prog_data = {0};
	GxMsgProperty_PlayerPlay *player_play;
	char url[GX_PM_MAX_PROG_URL_SIZE]={0};
	app_msg appMsg={0};
	int g_GdSwitchChannelFre = 0;//2510000;
	uint32_t gApp_cur_pos = 0;
	 if(!g_bSwitchTask)
	 {
	 	   goto continue1;  
	 }

	GxBus_ConfigGetInt(DVT_CA_CENTER_FRE, &g_GdSwitchChannelFre, DVT_CA_CENTER_FRE_DV);
	app_prog_get_playing_pos_in_group(&gApp_cur_pos);
	if (GxBus_PmProgNumGet() > 0)	
	{
		GxBus_PmProgGetByPos(gApp_cur_pos,1,&prog_data);	
		g_SwServiceID = prog_data.service_id;
		g_TsID = prog_data.ts_id;
		
		
	}

	app_play_set_msg_pop_type(MSG_POP_COMMON_END+DVTCA_SWTICH_CHANNEL+1);//正在获取信息，请稍候......

	serviceIndex = DVTSTBCA_GetProgPosByFrequency(g_GdSwitchChannelFre);
	if (-1 != serviceIndex)
	{
	     	GxBus_PmProgGetByPos(serviceIndex,1,&prog_data);	

		memset(url, 0, GX_PM_MAX_PROG_URL_SIZE);
		GxBus_PmProgUrlGet(&prog_data, (int8_t *)url, GX_PM_MAX_PROG_URL_SIZE);	
		appMsg.msg= GxBus_MessageNew(GXMSG_PLAYER_PLAY);
		player_play = GxBus_GetMsgPropertyPtr(appMsg.msg, GxMsgProperty_PlayerPlay);


		strcat(url, "&sync:3&tsid:0");
		
		memcpy(player_play->url,url,PLAYER_URL_LONG);
		player_play->player =  PLAYER0;
		printf("************************sent start*************************\n");	

		GxBus_MessageSend(appMsg.msg);
		appMsg.msg = NULL;
		appMsg.param = NULL;			
  	}
	else
	{

	 	DVTCASTB_SwitchChannelOver(2); 
	    app_play_clear_msg_pop_type(MSG_POP_COMMON_END+DVTCA_SWTICH_CHANNEL+1);
		serviceIndex = GxBus_PmProgPosGet(g_TsID,g_SwServiceID);	
		if (-1 != serviceIndex)
		{
		     	GxBus_PmProgGetByPos(serviceIndex,1,&prog_data);	

			memset(url, 0, GX_PM_MAX_PROG_URL_SIZE);
			GxBus_PmProgUrlGet(&prog_data, (int8_t *)url, GX_PM_MAX_PROG_URL_SIZE);	
			appMsg.msg= GxBus_MessageNew(GXMSG_PLAYER_PLAY);
			player_play = GxBus_GetMsgPropertyPtr(appMsg.msg, GxMsgProperty_PlayerPlay);


			strcat(url, "&sync:3&tsid:0");
			
			memcpy(player_play->url,url,PLAYER_URL_LONG);
			player_play->player =  PLAYER0;
			printf("************************sent start*************************\n");	

			GxBus_MessageSend(appMsg.msg);
			appMsg.msg = NULL;
			appMsg.param = NULL;			
	  	}
	  	g_bSwitchTask = false; 
		
		goto continue1;  
		 
	}
	
	g_SwitchChannelTicks = s_BakChannelTicks;

	DVTCASTB_SetEmmPid(app_dvt_cas_api_get_emm_pid());  // 播台的时候已经去搜索CAT
	while( g_SwitchChannelTicks)
	{ 
		  if(g_bAreaLockOK)
		  { 
			  g_bAreaLockOK = false; 
			  goto exit; 
		  }
		  
		  g_SwitchChannelTicks--;		  
    		  GxCore_ThreadDelay(100);
		  		  
	 } 
	 
	 DVTCASTB_SwitchChannelOver(1);  

exit: 
	 g_SwitchChannelTicks = 0; 
	 app_play_clear_msg_pop_type(MSG_POP_COMMON_END+DVTCA_SWTICH_CHANNEL+1);
		serviceIndex = GxBus_PmProgPosGet(g_TsID,g_SwServiceID);	
		if (-1 != serviceIndex)
		{
		     	GxBus_PmProgGetByPos(serviceIndex,1,&prog_data);	

			memset(url, 0, GX_PM_MAX_PROG_URL_SIZE);
			GxBus_PmProgUrlGet(&prog_data, (int8_t *)url, GX_PM_MAX_PROG_URL_SIZE);	
			appMsg.msg= GxBus_MessageNew(GXMSG_PLAYER_PLAY);
			player_play = GxBus_GetMsgPropertyPtr(appMsg.msg, GxMsgProperty_PlayerPlay);

			strcat(url, "&sync:3&ts:0");
			
			memcpy(player_play->url,url,PLAYER_URL_LONG);
			player_play->player =  PLAYER0;
			printf("************************sent start*************************\n");	

			GxBus_MessageSend(appMsg.msg);
			appMsg.msg = NULL;
			appMsg.param = NULL;			
	  	}
	 g_bSwitchTask = false;

continue1:	
    		 GxCore_ThreadDelay(1000);
	 	
		
} 




/*++
功能：通知机顶盒需要到特征码流频点?邮涨蛱卣髀肓鳌?(单频点区域锁定使用)
参数：
	wWaitSeconds:			超时时间，单位秒。
返回值：
	DVTCA_OK				成功
	其他					失败
--*/
HRESULT DVTSTBCA_SwitchChannel(WORD wWaitSeconds)
{
	int g_GdSwitchChannelFre = 0;//2510000;
	CAS_Dbg(" DVTSTBCA_SwitchChannel wWaitSeconds=%d\n",wWaitSeconds); 
   GxBus_ConfigGetInt(DVT_CA_CENTER_FRE, &g_GdSwitchChannelFre, DVT_CA_CENTER_FRE_DV);

    if(0 == app_prog_get_num_in_group()||  (g_GdSwitchChannelFre == 0))
    {
		DVTCASTB_SwitchChannelOver(2);
		return (!DVTCA_OK);
    }

	
	s_BakChannelTicks = wWaitSeconds;
	
	g_bSwitchTask = TRUE;  
	
	return DVTCA_OK;

}


/*
功能：显示用户不能观看节目等提示信息，下面两个函数对应使用。
参数：	
	byMesageNo:				要显示的消息编号。
--*/
void   DVTSTBCA_ShowPromptMessage(BYTE byMesageNo)
{
	printf(" DVTSTBCA_ShowPromptMessage byMesageNo=%d\n",byMesageNo);  
	if ((DVTCA_DATA_INVALID != byMesageNo)&&(DVTCA_DECRYPT_FAILURE != byMesageNo))
	{
		if (MSG_POP_COMMON_END+byMesageNo+1 >=64)
			return ;

		if ( TRUE != app_play_get_msg_pop_type_state(MSG_POP_COMMON_END+byMesageNo+1))
			{
				app_play_clear_ca_msg();
				app_play_set_msg_pop_type(MSG_POP_COMMON_END+byMesageNo+1);
			}		
	}

	return;
}

void   DVTSTBCA_HidePromptMessage(void)
{
	app_play_clear_ca_msg();

	return;
}

/*
功能：显示指纹信息。
参数：
	dwCardID:				卡号。
	wDuration:				持续秒数。
--*/
void	DVTSTBCA_ShowFingerPrinting(DWORD dwCardID, WORD wDuration)
{

	fingerdata.dwCardID = dwCardID;
	fingerdata.wDuration = wDuration;
	fingerdata.wStatus = TRUE;							

	app_cas_api_on_event(DVB_CA_FINGER_INFO,CAS_NAME, &fingerdata, sizeof(dvt_cas_finger_data_st));

	return;
}



#ifdef DVTCA_VERSION_NEW
/*
功能：CA模块用此函数通知机顶盒按CA前端的发送的OSD优先级（0~3）来有区别地显示普通、重要OSD。重要OSD用大号字体和特殊颜色显示。可以是同步显示，也可以是异步显示。
参数:	
	byPriority:	OSD优先级，0~3，优先级递增。
	szOSD:		用于显示的OSD信息。
说明：
	用户可以通过遥控器取消当前OSD显示。
--*/
void	DVTSTBCA_ShowOSDMsg(BYTE byPriority,const char * szOSD)
{
	int i = 0;
	int j=0;

	printf(" DVTSTBCA_ShowOSDMsg szOSD=%s\n",
		szOSD);  
	if (NULL == szOSD)
	{
	    return;
	}


	memset(&rollingmsg,0,sizeof(dvt_cas_rolling_message_st))	;
	rollingmsg.byStyle = 0;//byStyle;



	for (i = 0; i< strlen(szOSD);i++)
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
 
	rollingmsg.wStatus = TRUE;
	app_cas_api_on_event(DVB_CA_ROLLING_INFO,CAS_NAME, &rollingmsg, sizeof(dvt_cas_rolling_message_st));
	
	return;
}

#else 
/*
功能：以左滚字幕的方式显示一次OSD消息，可以是同步显示，也可以是异步显示。
参数:	
	szOSD:					用于显示的OSD信息。
说明：
	用户可以通过遥控器取消当前OSD显示。
--*/
void   DVTSTBCA_ShowOSDMsg(const char * szOSD)
{
	int i = 0;
	int j=0;

	printf(" DVTSTBCA_ShowOSDMsg szOSD=%s\n",
		szOSD);  
	if (NULL == szOSD)
	{
	return;
	}

	
		memset(&rollingmsg,0,sizeof(dvt_cas_rolling_message_st))	;
//		rollingmsg.byStyle = byStyle;
	
	
	
		for (i = 0; i< strlen(szOSD);i++)
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
	 
		rollingmsg.wStatus = TRUE;
		app_cas_api_on_event(DVB_CA_ROLLING_INFO,CAS_NAME, &rollingmsg, sizeof(dvt_cas_rolling_message_st));
	
	return;
}


#endif

/*++
功能：通知机顶盒区域锁定特征码流接收完毕。(单频点区域锁定使用)
--*/
void DVTSTBCA_AreaLockOk(void)
{
	CAS_Dbg(" DVTSTBCA_AreaLockOk szOSD\n");  
	// 参考移植文档
	DVTCASTB_SwitchChannelOver(0); 
	 g_bAreaLockOK = true; 
	 
	return;
}

/*++
功能：取消应急广播，切换回应急广播前用户观看的频道。
参数：无。
--*/
void DVTSTBCA_CancelUrgencyBroadcast( void )
{
#define KEY_RECALL          GUIK_BACKSPACE

	GUI_Event keyEvent = {0};
//	printf("------------CancelUrgencyBroadcast by application-------------\n");
	printf(" DVTSTBCA_CancelUrgencyBroadcast \n");  

	app_dvt_cas_api_set_lock_status(FALSE);
	GUI_SetProperty("full_screen_text_msg_5sec","state","hide");

	keyEvent.type = GUI_KEYDOWN;
	keyEvent.key.sym = KEY_RECALL;
	GUI_SendEvent(GUI_GetFocusWindow(),&keyEvent);	

	return;
}

/*++
功能：应急广播, 切换到指定频道。
参数：
	wOriNetID:				原始网络ID。
	wTSID:					传送流ID。
	wServiceID:				业务ID。
	wDuration:				持续秒数。
--*/
void DVTSTBCA_UrgencyBroadcast(WORD wOriNetID, WORD wTSID, WORD wServiceID, WORD wDuration)
{
	app_dvt_cas_api_lock_service(wOriNetID, wTSID, wServiceID, wDuration);
	return;
}

/*++
功能：重置智能卡。
--*/
HRESULT DVTSTBCA_SCReset(void) 
{
	uint8_t                     len;
    uint8_t               atr[256] = {0};
	int nRetValue = FALSE;

  	CAS_Dbg("[SC] DVTSTBCA_SCReset!\n");

    nRetValue = app_dvt_cas_api_smart_reset(atr,&len);

	if (TRUE == nRetValue)
	{
		return DVTCA_OK;
	}
    else
	{
		return DVTCAERR_STB_UNKNOWN;
	}

}

HRESULT DVTSTBCA_SCAPDU(BYTE byReaderNo, BYTE * pbyLen, const BYTE * byszCommand, BYTE * byszReply)
{
    uint8_t reply[1024] = {0};
    uint8_t sc_result;
	uint16_t CommandLen = *pbyLen;
	uint16_t ReplyLen = 0;

    //CAS_DUMP("[GOSSTBCA_SCAPDU] write->", byszCommand, *pbyLen);

	if (*pbyLen<5)
	{
		printf("DVTSTBCA_SCAPDU--->Send Len :%d, Command",*pbyLen);

		return DVTCAERR_STB_UNKNOWN;
	}

    sc_result =  app_porting_ca_smart_sc_apdu(byszCommand,CommandLen,reply,&ReplyLen);

    if (TRUE == sc_result)
	{
        
		memcpy(byszReply, reply, ReplyLen);
		*pbyLen = (BYTE)ReplyLen;
        
        //CAS_DUMP("[GOSSTBCA_SCAPDU] Read->", byszReply, ReplyLen);

		return DVTCA_OK;
	}
	else
	{
		return DVTCAERR_STB_UNKNOWN;			
	}
	
}









