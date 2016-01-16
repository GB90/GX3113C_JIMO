/*****************************************************************************
* 						   CONFIDENTIAL
*        Hangzhou GuoXin Science and Technology Co., Ltd.
*                      (C)2012, All right reserved
******************************************************************************

******************************************************************************
* File Name :	app_ca_api.c
* Author    : 	bingyu
* Project   :	goxceed dvbc
* Type      :
******************************************************************************
* Purpose   :	模块头文件
******************************************************************************
* Release History:
VERSION	Date			  AUTHOR         Description
1.0  	2012.12.02		  bingyu 	 			creation
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
	
#include "gyca_def.h"
#include "gyca_pub.h"
#include "gyca_osp.h"
#include "gyca_interface.h"
#include "gyca_porting.h"
	
	
#include "app_gy_cas_api_smart.h"
#include "app_gy_cas_api_demux.h"

extern SC_Status_Notify_Fct_t GYCA_GetNotify(void);


uint8_t app_gy_cas_api_smart_check_valid(void)
{
	unsigned char ucLen,i;
	GxSmcTimeParams TimeParam;
	unsigned char aGyCasATR[YGCAS_ART_LEN]={0x3b,0x6d,0x00,0x00,0x57,0x44,0x28,0x57,0x46,0x03,0x50,0x62,0x13,0x65,0x48,0x42,0x1f};
	unsigned char aATR[YGCAS_ART_LEN]={0};

	GYCA_SC_DRV_Initialize();

	TimeParam.baud_rate  = RUNTIME_BAUD_RATE;
	TimeParam.egt        = RUNTIME_EGT;
	TimeParam.etu        = RUNTIME_ETU;
	TimeParam.tgt        = RUNTIME_TGT;
	TimeParam.twdt       = RUNTIME_TWDT;
	TimeParam.wdt        = RUNTIME_WDT;
			
	if(TRUE==app_porting_ca_smart_reset(TimeParam,aATR, &ucLen))
	{
		for (i=0; i<YGCAS_ART_LEN; i++)
		{
			if (aGyCasATR[i]!=aATR[i])
			{
				CA_FAIL(" smart card is not gy cas smart \n");				
			}
		}
	}

	return TRUE;	
}

uint8_t app_gy_cas_api_card_in(char* atr,uint8_t len)
{
	if(GYTRUE==GYCA_SC_ValidateATR((GYU8 *)atr, (GYU16)len))
	{
		if(GYCA_GetNotify() != NULL)
		{
			GYCA_GetNotify()(0, GYCA_CARD_IN, (GYU8 *)atr, (GYU16)len);
		}		
	}
	else
	{
		if(GYCA_GetNotify() != NULL)
		{
			GYCA_GetNotify()(0, GYCA_CARD_NOTVALID, 0, 0);
		}
	}
	
	return 1;
}

uint8_t app_gy_cas_api_card_out(void)
{
	if(GYCA_GetNotify() != NULL)
	{
		GYCA_GetNotify()(0, GYCA_CARD_OUT, 0, 0);
	}
	
	return 1;
}



