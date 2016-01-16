/*****************************************************************************
* 						   CONFIDENTIAL
*        Hangzhou GuoXin Science and Technology Co., Ltd.
*                      (C)2012, All right reserved
******************************************************************************

******************************************************************************
* File Name :	tr_cas_porting_smartcard.c
* Author    : 	xxxx
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
#include "app_tr_cas_api_smart.h"
#include "Tr_Cas.h"


//#define USE_SCATR_NOTIFY_BAKEUP

#ifdef USE_SCATR_NOTIFY_BAKEUP
extern TRCAS_SC_ATR_NOTIFY g_trcasScNotifyBakeup;
#endif

static TRCAS_SC_ATR_NOTIFY s_trcasAtrNotify = NULL;


S16 TRDRV_SCARD_Initialise(IN S16 (*SC_ATR_Notify)(U8 bCardNumber, U8 bStatus, U8 *pbATR, U8 bProtocol))
{
	if (SC_ATR_Notify != NULL)
	{
		s_trcasAtrNotify = SC_ATR_Notify;

		#ifdef USE_SCATR_NOTIFY_BAKEUP
		g_trcasScNotifyBakeup = SC_ATR_Notify;
		#endif
		return 0;
	}
	else
	{
		printf("[TRDRV_SCARD_Initialise]SC_ATR_Notify is NULL!!!\n");
		return 1;
	}
}

void  TRDRV_SCARD_ResetCard(U8 bCardNumber)
{
	U8 pbyLen = 33;
	U8 pbyATR[256] = {0,};
	GxSmcTimeParams smcTime;

	memset(&smcTime, 0, sizeof(smcTime));
	
	smcTime.baud_rate  = RUNTIME_BAUD_RATE;
	smcTime.egt        = RUNTIME_EGT;
	smcTime.etu        = RUNTIME_ETU;
	smcTime.tgt        = RUNTIME_TGT;
	smcTime.twdt       = RUNTIME_TWDT;
	smcTime.wdt        = RUNTIME_WDT;

	//printf("-+>[TRDRV_SCARD_ResetCard]-<+\n");
	app_porting_ca_smart_reset(smcTime, pbyATR, &pbyLen);
	return;
}

S16 TRDRV_SCARD_AdpuFunction(U8 bCardNumber, IN U8 *pbTxData, 
	   									U32 ulTxLength, OUT U8 *pbRxData, 
	   									OUT U32 *pulRxLength)
{
	int ret = 0;
	uint8_t replyData[1024] = {0};
	uint16_t wCommandLen = ulTxLength;
	uint8_t *pcmd = pbTxData;
	uint16_t replyLen = 0;

	//printf("-+>[TRDRV_SCARD_AdpuFunction]-<+\n");
	//CAS_DUMP("[+SC] Send Command ", pcmd, wCommandLen);
	ret = app_porting_ca_smart_status();
	if (!ret)
	{ 
		printf("[TRDRV_SCARD_AdpuFunction]smart out!!!\n");
		return 1;
	}

	memset(replyData, 0, sizeof(replyData));
	ret = app_porting_ca_smart_sc_apdu61(pcmd, wCommandLen, replyData, &replyLen);
	if (TRUE == ret)
	{ 	
		if (pbRxData)
		{
			memcpy(pbRxData, replyData, replyLen);
	 	}
		
		if (pulRxLength)
	 	{
			*pulRxLength = replyLen;
		}
		
	//	CAS_DUMP("\nREPLY", replyData, replyLen);
		return 0;
	}
	
	return 1;
}

S16 TRDRV_SCARD_GetSlotState(U8 bCardNumber, OUT U8 *pbSlotState)
{
	bool bIsCardIn = TRUE;

	bIsCardIn = app_porting_ca_smart_status();
	if (TRUE == bIsCardIn)
	{
		*pbSlotState = CAS_SCARD_INSERT;
	}
	else if (FALSE == bIsCardIn)
	{
       	*pbSlotState = CAS_SCARD_REMOVE;
	}

	printf("-+>[TRDRV_SCARD_GetSlotState]state(%d)-<+\n", *pbSlotState);
	return 0;
}

void TRDRV_SCARD_In(char *pbyATR)
{
	#ifdef USE_SCATR_NOTIFY_BAKEUP
	if (NULL == s_trcasAtrNotify)
	{
		printf("-+>[TRDRV_SCARD_In]s_trcasAtrNotify is NULL, used bakeup#+#-<+\n");
		s_trcasAtrNotify = g_trcasScNotifyBakeup;
	}
	#endif
	
	if (s_trcasAtrNotify != NULL)
	{
		s_trcasAtrNotify(0, CAS_SCARD_INSERT, (U8 *)pbyATR, 0);     	
	}
	else
	{
		printf("-+>[TRDRV_SCARD_In]s_trcasAtrNotify is NULL!!!-<+\n");
	}

	return;
}

void TRDRV_SCARD_Remove(char *pbyATR)
{	
	#ifdef USE_SCATR_NOTIFY_BAKEUP
	if (NULL == s_trcasAtrNotify)
	{
		printf("-+>[TRDRV_SCARD_Remove]s_trcasAtrNotify is NULL, used bakeup#@#-<+\n");
		s_trcasAtrNotify = g_trcasScNotifyBakeup;
	}
	#endif
	
	if (s_trcasAtrNotify != NULL)
	{
		s_trcasAtrNotify(0, CAS_SCARD_REMOVE, (U8 *)pbyATR, 0);     	
	}
	else
	{
		printf("-+>[TRDRV_SCARD_Remove]s_trcasAtrNotify is NULL!!!-<+\n");
	}

	return;
}


