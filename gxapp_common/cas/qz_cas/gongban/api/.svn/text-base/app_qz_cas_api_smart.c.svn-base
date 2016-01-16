/*****************************************************************************
* 						   CONFIDENTIAL
*        Hangzhou GuoXin Science and Technology Co., Ltd.
*                      (C)2012, All right reserved
******************************************************************************

******************************************************************************
* File Name :	app_mg_cas_api_smart.c
* Author    : 	zhouhm
* Project   :	goxceed dvbc
* Type      :
******************************************************************************
* Purpose   :	
******************************************************************************
* Release History:
VERSION	Date			  AUTHOR         Description
1.0  	2014.04.11		  zhouhm 	 			creation
*****************************************************************************/
#include "app_common_play.h"
#include "app_common_porting_stb_api.h"
#include "app_common_porting_ca_flash.h"
#include "app_common_porting_ca_smart.h"
#include "app_common_porting_ca_demux.h"
#include "app_common_porting_ca_descramble.h"
#include "app_qz_cas_api_demux.h"
#include "app_qz_cas_api_smart.h"
#include "ca_process.h"

#define RUNTIME_ETU                         (372)
#define RUNTIME_BAUD_RATE                   (9600*RUNTIME_ETU)
#define RUNTIME_EGT                         (RUNTIME_ETU * 20)
#define RUNTIME_WDT                         (45 * RUNTIME_ETU)
#define RUNTIME_TGT                         (0)
#define RUNTIME_TWDT                        (3*9600 * RUNTIME_ETU)

SMARTCARD_INFO card;
extern uint8_t prog_scramble_flag;

extern int DVTSTBCA_GetValidSNID(uint8_t *pSN,int *len);
extern void check_sw1sw2(uint16_t sw1sw2);
uint8_t app_qz_cas_api_smart_check_valid(void)
{
	uint8_t valid_art[]={0x3b,0x93,0x11,0x00};
	uint8_t len;
	uint8_t i;
	uint8_t atr[ATR_MAX_LEN] = {0,};
	uint8_t ret = 0;

	app_qz_cas_api_smart_init();
	ret = app_qz_cas_api_smart_reset(atr, &len);
	if (FALSE == ret)
	{
		printf(" smart card is not qz cas smart \n");
		return FALSE;
	}
    
	for (i=0; i<sizeof(valid_art); i++)
	{
		if (valid_art[i]!=atr[i])
		{
			printf(" smart card is not qz cas smart \n");
			return FALSE;
		}
	}
	printf(" smart card is qz cas smart\n");
	return TRUE;	
}

//智能卡初始化
void app_qz_cas_api_smart_init(void)
{
	GxSmcParams    param;
	GxSmcTimeParams    time;
	
	memset(&time,0,sizeof(GxSmcTimeParams));
	memset(&param,0,sizeof(GxSmcParams));

	time.baud_rate  = RUNTIME_BAUD_RATE;
	time.egt        = RUNTIME_EGT;
	time.etu        = RUNTIME_ETU;
	time.tgt        = RUNTIME_TGT;
	time.twdt       = RUNTIME_TWDT;
	time.wdt        = RUNTIME_WDT;

	param.detect_pole = GXSMC_DETECT_HIGH_LEVEL;
    param.io_conv = GXSMC_DATA_CONV_DIRECT;
	param.parity = GXSMC_PARITY_ODD;// GXSMC_PARITY_EVEN;
    param.protocol = DISABLE_REPEAT_WHEN_ERR;
    param.sci_sel = __GXSMART_SCI2;
    param.stop_len = GXSMC_STOPLEN_0BIT;
    param.vcc_pole = GXSMC_VCC_HIGH_LEVEL;
    param.default_etu = 372;
	param.auto_etu = 1;
	param.auto_parity = 1;
//	param.debug_info = O_BDBG|O_CDBG;

    return app_porting_ca_smart_card_init(param,time);
	
}
//智能卡复位
uint8_t app_qz_cas_api_smart_reset(uint8_t* pbyATR, uint8_t *pbyLen )
{
	 GxSmcTimeParams    time;
	 time.baud_rate  = RUNTIME_BAUD_RATE;
	 time.egt        = RUNTIME_EGT;
	 time.etu        = RUNTIME_ETU;
	 time.tgt        = RUNTIME_TGT;
	 time.twdt       = RUNTIME_TWDT;
	 time.wdt        = RUNTIME_WDT;

	 return app_porting_ca_smart_reset(time,pbyATR,pbyLen);
}

//智能卡插入回调
uint8_t app_qz_cas_api_card_in(char* atr,uint8_t len)
{
    uint8_t stb_SN[16] = {0x0};
    int32_t snLen = sizeof(stb_SN);

    snLen = DVTSTBCA_GetValidSNID(stb_SN,&snLen);
    if (ERR_OK != check_card_apdu((char *)stb_SN, (uint8_t)snLen,
        card.super_cas_id,card.smartcard_id,card.cw_mask))
    {
        printf("[CA]check_card_apdu failure\n");
        return 0;
    }
    else
    {
        CAS_DUMP("STB SN,->", stb_SN, snLen);
        CAS_DUMP("super_cas_id,->", card.super_cas_id, 4);
        CAS_DUMP("smartcard_id,->", card.smartcard_id, 3);
        CAS_DUMP("cw_mask,->", card.cw_mask, 8);
    }

    memset(&card.area_code[0],0,sizeof(card.area_code));
    if(ERR_OK == get_area_code(&card.area_code[2]))
    {
        CAS_DUMP("area_code,->", card.area_code, 4);
    }


    Ca_CardIn();
    
	app_play_clear_msg_pop_type(MSG_POP_COMMON_END+CA_QZ_ERR_CARD_NOTFOUND+1); 
    app_qz_cas_api_reset_ecm_filter();
    app_qz_cas_api_reset_emm_filter();
	return 1;
}
//智能卡拔出
uint8_t app_qz_cas_api_card_out(void)
{
    printf("card out\n");
	Ca_CardOut();
    if(prog_scramble_flag)
    {
        check_sw1sw2(QZ_CARD_NOTFOUND);
    }
	return 1;
}



