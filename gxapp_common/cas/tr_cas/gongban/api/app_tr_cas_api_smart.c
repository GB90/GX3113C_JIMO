/*****************************************************************************
* 						   CONFIDENTIAL
*        Hangzhou GuoXin Science and Technology Co., Ltd.
*                      (C)2012, All right reserved
******************************************************************************

******************************************************************************
* File Name :	app_tr_cas_api_smart.c
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
	
#include "Tr_Cas.h"
#include "app_tr_cas_api_smart.h"
#include "app_tr_cas_api_demux.h"


extern void TRDRV_DESC_Clear_Cw(void);
extern void app_tr_cas_over_osd_roll(void);/*wangjian add on 20150331*/
extern void app_tr_cas_reset_entitle_check(void);

// int8_t s_bCaSmartCardInit = FALSE;

void app_tr_api_smart_init(void)
{
	GxSmcParams param;
	GxSmcTimeParams time;

//	if (s_bCaSmartCardInit)
//	{
//		return;
//	}
	
	memset(&time, 0, sizeof(GxSmcTimeParams));
	memset(&param, 0, sizeof(GxSmcParams));

	time.baud_rate  = RUNTIME_BAUD_RATE;
	time.egt 	   = RUNTIME_EGT;
	time.etu 	   = RUNTIME_ETU;
	time.tgt 	   = RUNTIME_TGT;
	time.twdt	   = RUNTIME_TWDT;
	time.wdt 	   = RUNTIME_WDT;

	param.detect_pole = GXSMC_DETECT_HIGH_LEVEL;//GXSMC_DETECT_LOW_LEVEL;
	param.io_conv = GXSMC_DATA_CONV_DIRECT;
	param.parity = GXSMC_PARITY_ODD;// GXSMC_PARITY_EVEN;
	param.protocol = DISABLE_REPEAT_WHEN_ERR;
	param.sci_sel = __GXSMART_SCI2;
	param.stop_len = GXSMC_STOPLEN_0BIT;
	param.vcc_pole = GXSMC_VCC_HIGH_LEVEL;
	param.default_etu = RUNTIME_ETU;
	param.auto_etu = 1;
	param.auto_parity = 1;
	//param.debug_info = O_BDBG|O_CDBG;

    app_porting_ca_smart_card_init(param, time);
//	s_bCaSmartCardInit = TRUE;

	return;
}

/*ATR: 3B FE 11 00 00 50 80 FF 5C 54 52 43 07 00 00 0B 08 11 00 06 00 80 */
uint8_t app_tr_cas_api_smart_check_valid(void)
{
	uint8_t valid_art[22]={0x3B,0xFE,0x11,0x00,0x00,0x50,0x80,0xFF,0x5C,0x54,0x52,0x43,0x07,0x00,0x00,0x0B,0x08,0x11,0x00,0x06,0x00,0x80};
	unsigned char ucLen;
	GxSmcTimeParams TimeParam;
	unsigned char aATR[256] = {0,};
	uint32_t i;

	memset(&TimeParam,0,sizeof(GxSmcTimeParams));
	TimeParam.baud_rate  = RUNTIME_BAUD_RATE;
	TimeParam.egt        = RUNTIME_EGT;
	TimeParam.etu        = RUNTIME_ETU;
	TimeParam.tgt        = RUNTIME_TGT;
	TimeParam.twdt       = RUNTIME_TWDT;
	TimeParam.wdt        = RUNTIME_WDT;
	app_tr_api_smart_init()	;	
	if (FALSE == app_porting_ca_smart_reset(TimeParam, aATR, &ucLen))
	{
		printf("[app_tr_cas_api_smart_check_valid]Reset fail!!!\n");
		return FALSE;
	}
	if((aATR[0] != 0x3B) && (aATR[0] != 0x3F))
	{
		printf(" ATR[0] is not 0x3b or 0x3f \n");
		return FALSE;
	}
	for (i=1; i<12; i++)
	{
		if (valid_art[i]!=aATR[i])
		{
			printf(" smart card is not tr cas smart \n");
			return FALSE;
		}
	}
	printf(" smart card is tr cas smart\n");
	return TRUE;	
}

uint8_t app_tr_cas_api_card_in(char *atr, uint8_t len)
{
	int i=0;
	uint8_t valid_art1[16]={0x3B,0x6c,0x02,0x56,0x01,0x12,0x11,0x26,0x33,0xf7,0x02,0x13,0x04,0x11,0x28,0x1f};
	int flag = 1;
	TRDRV_SCARD_In(atr);
	app_tr_cas_reset_entitle_check();
#ifdef DVB_CA_TYPE_DS_CAS50_FLAG
	for (i=0; i<16; i++)
	{
		if (valid_art1[i]!=atr[i])
		{
			printf(" smart card is not desai cas smart \n");
			return;
		}
	}

	printf(" smart card is desai cas smart \n");
	app_cas_api_on_event(DVB_CA_CHANGE_INFO,CAS_NAME, &flag, 0);
#endif

	return 1;
}

uint8_t app_tr_cas_api_card_out(void)
{
	char tmpAtr[256] = {0,};

	//printf("-+>[app_tr_cas_api_card_out]-<+\n");
	memset(tmpAtr, 0, sizeof(tmpAtr));
	
	TRDRV_SCARD_Remove(tmpAtr);
	TRDRV_DESC_Clear_Cw();
	app_tr_cas_over_osd_roll();
	return 1;
}

