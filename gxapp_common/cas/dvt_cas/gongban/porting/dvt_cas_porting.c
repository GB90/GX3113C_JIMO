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
* Purpose   :	ģ��ͷ�ļ�
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
���ܣ���û����з����CAS��flash�ռ������ַ�ʹ�С�����ֽ�Ϊ��λ����
������
	ppStartAddr:			�����з����CAS��flash�ռ�Ŀ�ʼ��ַ��
	lSize:					��������з����CAS��flash�ռ�Ĵ�С��
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
���ܣ���ȡ�����ڻ�����flash�е���Ϣ��
������
	pStartAddr:				Ҫ��ȡ�Ĵ洢�ռ�Ŀ�ʼ��ַ��
	plDataLen:				����ΪҪ��ȡ�������ֵ�����Ϊʵ�ʶ�ȡ�Ĵ�С��
	pData:					���������ݡ�
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
���ܣ�������еĴ洢�ռ�д��Ϣ��
������
	lStartAddr:				Ҫд�Ĵ洢�ռ�Ŀ�ʼ��ַ��
	plDataLen:				����ΪҪд�����ݵĳ��ȣ����Ϊд���ʵ�ʳ��ȡ�
	pData:					Ҫд�����ݡ�
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
���ܣ���ȡ�����ڻ�����eeprom�е���Ϣ��
������
	plDataLen:				����ΪҪ��ȡ�������ֵ�����Ϊʵ�ʶ�ȡ�Ĵ�С��
	pData:					���������ݡ�
--*/
HRESULT DVTSTBCA_GetDataFromEeprom(long * plDataLen, BYTE * pData)
{
	return (HRESULT)-1;
}

/*++
���ܣ�������е�eeprom�洢�ռ�д��Ϣ��
������
	plDataLen:				����ΪҪд�����ݵĳ��ȣ����Ϊд���ʵ�ʳ��ȡ�
	pData:					Ҫд�����ݡ�
--*/
HRESULT DVTSTBCA_SaveDataToEeprom(long * plDataLen, const BYTE * pData)
{
	return (HRESULT)-1;
}

/*++
���ܣ�����ǰ�̡߳�
������
	dwMicroseconds:			���롣
--*/
HRESULT DVTSTBCA_Sleep( int dwMicroseconds )
{
	app_porting_ca_os_sleep(dwMicroseconds);
	return DVTCA_OK;
}

/*++
���ܣ���ʼ���ź���,��ʼ�����ź���û���źš�
������
	pSemaphore:				ָ���ź�����ָ�롣
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
���ܣ����ź������źš�
������
	pSemaphore: 			ָ���ź�����ָ�롣
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
���ܣ��ȴ��ź���,�ȴ��ɹ�֮��,�ź���Ϊ���źš�
������
	pSemaphore: 			ָ���ź�����ָ�롣
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
���ܣ�CASģ���������ע���̡߳�
������
	szName:					ע����߳����ơ�
	pTaskFun:				Ҫע����̺߳�����
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
���ܣ����ù���������CAS˽�����ݡ�
������
	byReqID:				�����ձ������š�������Ӧ�ñ�������ݣ����յ�CAS˽�����ݺ����DVTCASTB_StreamGuardDataGot()ʱӦ�ý������ݷ��ء�
	wEcmPID:				��Ҫ���˵�����PID��
	szFilter:				��������ֵ��Ϊһ��8���ֽڵ����顣
	szMask:					�����������룬Ϊһ��8���ֽڵ����飬���������Ӧ����szMask��ĳһbitλΪ0ʱ��ʾҪ���˵ı��Ӧ��bitλ����Ҫ��szFilter���Ӧ��bitλ��ͬ����szMaskĳһλΪ1ʱ��ʾҪ���˵ı��Ӧ��bitλһ��Ҫ��szFilter���Ӧ��bitλ��ͬ��
	byLen:					�������ĳ��ȣ�Ϊ8��
	nWaitSeconds:			�մ˱���ĵȴ�ʱ�䣬���Ϊ0�����޵ȴ�����λ�롣
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
	// �������������ȫ��ͬ����RESET

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
���ܣ�CA�����ô˺������ý�����������ǰ���ڼ���һ���ڵ�CW�͸���������
������
	wEcmPID:				CW������ECMPID��
	szOddKey:				��CW�����ݡ�
	szEvenKey:				żCW�����ݡ�
	byKeyLen:				CW�ĳ��ȡ�
	bTapingControl:			true������¼��,false��������¼��
--*/
HRESULT DVTSTBCA_SetDescrCW(WORD wEcmPID, BYTE byKeyLen, const BYTE * szOddKey, const BYTE * szEvenKey, bool bTapingControl)
{
	app_porting_ca_descramble_set_cw(wEcmPID,szOddKey,szEvenKey,byKeyLen,bTapingControl);
	return DVTCA_OK;
}



/*++
���ܣ���ӡ������Ϣ��
������
	pszMsg:					������Ϣ���ݡ�
--*/
void DVTSTBCA_AddDebugMsg(const char * pszMsg)
{
	printf(( char * ) pszMsg );
	printf("\n" );
}





/*++
����:ʵ���ڴ濽��(���ST71ƽ̨�����Դ������Ŀ�������ص���
���ܵ����ڴ濽��������Ҫ���¶���)��
����ֵ:
pDest:Ŀ���ַ
pSrc:  Դ��ַ
iSize:  ��С
--*/
HRESULT DVTSTBCA_Memcpy(void  * pDest, const void * pSrc,  unsigned int iSize)
{
    memcpy(pDest, pSrc, iSize);
	return DVTCA_OK;
}




/*++
����:��ȡоƬ��CPUID��
����:
	pbyCPUID:	оƬ�ṩ��CPUID��17���ֽ�,�������ɻ������
����ֵ:
	0:		��ȡCPUID�ɹ�
	����ֵ:	��ȡCPUIDʧ��
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
����:��ȡNIT�е�ֵ�����ں�������Աȡ�
����:
	pdwData:	�������ṩ�Ĵ�NIT�л�ȡ��ֵ
����ֵ:
	0:		��ȡ�ɹ�
	����ֵ:	��ȡʧ��
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
���ܣ���ȡ����������汾��
����ֵ������������汾�š�
--*/
DWORD DVTSTBCA_GetSoftVer(void)
{
	uint32_t gsoftversion=0;
//	gsoftversion = get_softversion();
	CAS_Dbg(" DVTSTBCA_GetSoftVer gsoftversion=0x%x\n",gsoftversion); 
	return (DWORD )gsoftversion;
}

/*++
���ܣ���ʾ���ʼ��ı�־��
������
	byShow:					��־���͡��������£�
							DVTCAS_NEW_EAMIL				���ʼ�֪ͨ��
							DVTCAS_NEW_EMAIL_NO_ROOM		�����ʼ�����Email�ռ䲻����
							DVTCAS_EMAIL_NONE				û�����ʼ���
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
���ܣ�������ѯ���û��Ƿ�Ԥ����ǰIPP��Ŀ��
������
	wEcmPid:				IPP��Ӧ��EcmPID
	pIpp:					��ǰIPP����Ϣ��
--*/
void DVTSTBCA_InquireBookIpp(WORD wEcmPid, const SDVTCAIpp * pIpp )
{
	return;
}


/*��������*/
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

	
/*����/ ����ǰ����ң������Ӧ */
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

	app_play_set_msg_pop_type(MSG_POP_COMMON_END+DVTCA_SWTICH_CHANNEL+1);//���ڻ�ȡ��Ϣ�����Ժ�......

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

	DVTCASTB_SetEmmPid(app_dvt_cas_api_get_emm_pid());  // ��̨��ʱ���Ѿ�ȥ����CAT
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
���ܣ�֪ͨ��������Ҫ����������Ƶ��?����������������?(��Ƶ����������ʹ��)
������
	wWaitSeconds:			��ʱʱ�䣬��λ�롣
����ֵ��
	DVTCA_OK				�ɹ�
	����					ʧ��
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
���ܣ���ʾ�û����ܹۿ���Ŀ����ʾ��Ϣ����������������Ӧʹ�á�
������	
	byMesageNo:				Ҫ��ʾ����Ϣ��š�
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
���ܣ���ʾָ����Ϣ��
������
	dwCardID:				���š�
	wDuration:				����������
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
���ܣ�CAģ���ô˺���֪ͨ�����а�CAǰ�˵ķ��͵�OSD���ȼ���0~3�������������ʾ��ͨ����ҪOSD����ҪOSD�ô�������������ɫ��ʾ��������ͬ����ʾ��Ҳ�������첽��ʾ��
����:	
	byPriority:	OSD���ȼ���0~3�����ȼ�������
	szOSD:		������ʾ��OSD��Ϣ��
˵����
	�û�����ͨ��ң����ȡ����ǰOSD��ʾ��
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
				/*�س����лᱻ����������*/
				printf("szMessage[%d]=0x%x\n",i,szOSD[i]);
			}

	}
 
	rollingmsg.wStatus = TRUE;
	app_cas_api_on_event(DVB_CA_ROLLING_INFO,CAS_NAME, &rollingmsg, sizeof(dvt_cas_rolling_message_st));
	
	return;
}

#else 
/*
���ܣ��������Ļ�ķ�ʽ��ʾһ��OSD��Ϣ��������ͬ����ʾ��Ҳ�������첽��ʾ��
����:	
	szOSD:					������ʾ��OSD��Ϣ��
˵����
	�û�����ͨ��ң����ȡ����ǰOSD��ʾ��
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
						/*�س����лᱻ����������*/
						printf("szMessage[%d]=0x%x\n",i,szOSD[i]);
					}
	
			}
	 
		rollingmsg.wStatus = TRUE;
		app_cas_api_on_event(DVB_CA_ROLLING_INFO,CAS_NAME, &rollingmsg, sizeof(dvt_cas_rolling_message_st));
	
	return;
}


#endif

/*++
���ܣ�֪ͨ����������������������������ϡ�(��Ƶ����������ʹ��)
--*/
void DVTSTBCA_AreaLockOk(void)
{
	CAS_Dbg(" DVTSTBCA_AreaLockOk szOSD\n");  
	// �ο���ֲ�ĵ�
	DVTCASTB_SwitchChannelOver(0); 
	 g_bAreaLockOK = true; 
	 
	return;
}

/*++
���ܣ�ȡ��Ӧ���㲥���л���Ӧ���㲥ǰ�û��ۿ���Ƶ����
�������ޡ�
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
���ܣ�Ӧ���㲥, �л���ָ��Ƶ����
������
	wOriNetID:				ԭʼ����ID��
	wTSID:					������ID��
	wServiceID:				ҵ��ID��
	wDuration:				����������
--*/
void DVTSTBCA_UrgencyBroadcast(WORD wOriNetID, WORD wTSID, WORD wServiceID, WORD wDuration)
{
	app_dvt_cas_api_lock_service(wOriNetID, wTSID, wServiceID, wDuration);
	return;
}

/*++
���ܣ��������ܿ���
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









