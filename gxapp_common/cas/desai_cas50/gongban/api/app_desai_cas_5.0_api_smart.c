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
* Purpose   :	ģ��ͷ�ļ�
******************************************************************************
* Release History:
VERSION	Date			  AUTHOR         Description
1.0  	2013.05.14		  huangyp 	 			creation
*****************************************************************************/
#include "app_common_play.h"
#include "app_common_porting_stb_api.h"
//#include "app_win_interface.h"
#include "app_desai_cas_5.0_api_smart.h"
#include "app_common_porting_ca_flash.h"
#include "app_common_porting_ca_smart.h"
#include "app_common_porting_ca_demux.h"
#include "app_common_porting_ca_descramble.h"
#include "app_desai_cas_5.0_api_demux.h"
//#include "win_desai_cas_5.0_rolling_message.h"
#include "desai_ca_api.h"
#include "app_common_prog.h"


extern int8_t app_desai_cas50_rolling_message_init(void);
/*
#define RUNTIME_BAUD_RATE                   (3570000)
#define RUNTIME_ETU                         (372)
#define RUNTIME_EGT                         (RUNTIME_ETU * 20)

#define RUNTIME_WDT                         (45 * RUNTIME_ETU)
#define RESET_WDT                         	(9600 * RUNTIME_ETU)
#define RUNTIME_TGT                         (0)
#define RUNTIME_TWDT                        (2*9600 * RUNTIME_ETU)
*/
#if 1
#define RUNTIME_ETU                         (372)
#define RUNTIME_BAUD_RATE             (3570000)//(9600*RUNTIME_ETU)
#define RUNTIME_EGT                   (RUNTIME_ETU * 30)
#define RUNTIME_WDT                   (400 * RUNTIME_ETU)
#define RUNTIME_TGT                         (0)
#define RUNTIME_TWDT                   (0x1ffffff)
//#define RUNTIME_TWDT                   (9600 * RUNTIME_ETU)
#else
#define RUNTIME_ETU                         (372)
#define RUNTIME_BAUD_RATE                   (9600*RUNTIME_ETU)
#define RUNTIME_EGT                         (RUNTIME_ETU * 20)
#define RUNTIME_WDT                         (150 * RUNTIME_ETU)
#define RUNTIME_TGT                         (0)
#define RUNTIME_TWDT                        (3*9600 * RUNTIME_ETU)

#endif
/*
#define RUNTIME_BAUD_RATE                   (3570000)
#define RUNTIME_ETU                         (372)
#define RUNTIME_EGT                         (RUNTIME_ETU * 30)
#define RUNTIME_WDT                         (400 * RUNTIME_ETU)
#define RUNTIME_TGT                         (0)
#define RUNTIME_TWDT                        (0x1ffffff)
#define RESET_WDT                        (400 * RUNTIME_ETU)
*/


uint8_t app_desai_cas50_api_smart_check_valid(void)
{ 
	uint8_t len;
	uint8_t atr[ATR_MAX_LEN] = {0,};
	uint8_t ret = 0;
	uint32_t i;
	uint8_t flag = 0;
	uint8_t valid_art[16]={0x3B,0x6c,0x02,0x56,0x02,0x15,0x01,0x16,0x33,0xf7,0x07,0x15,0x03,0x05,0x28,0x1f};
	uint8_t valid_art1[16]={0x3B,0x6c,0x02,0x56,0x01,0x12,0x11,0x26,0x33,0xf7,0x02,0x13,0x04,0x11,0x28,0x1f};
	app_desai_cas50_api_smart_init();
	ret = app_desai_cas50_api_smart_reset(atr, &len);

	if (FALSE == ret)
	{	
		printf(" smart card is not desai cas smart \n");
		return FALSE;
	}
	if((atr[0] != 0x3B) && (atr[0] != 0x3F))
	{
		printf(" ATR[0] is not 0x3b or 0x3f \n");
		return FALSE;
	}
	for (i=1; i<16; i++)
	{
		if (valid_art[i]!=atr[i])
		{
			printf(" smart card is not ds cas smart i = %d valid_art=0x%x, atr=0x%x\n",i,valid_art[i],atr[i]);
			flag = 1;
			break;//			return FALSE;
		}
	}
	for (i=1; i<16; i++)
	{
		if (valid_art1[i]!=atr[i])
		{
			printf(" smart card is not ds cas smart i = %d valid_art=0x%x, atr=0x%x\n",i,valid_art1[i],atr[i]);
			if (flag ==1)
			{	
				flag = 2;
			}
			else
			{
				flag = 1;
			}	
			break;//			return FALSE;
		}
	}
	if( flag == 2 )
	{
		return FALSE;
	}
	printf(" smart card is desai cas smart\n");
	return TRUE;	
}

void app_desai_cas50_api_smart_init(void)
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
#if (PANEL_TYPE == PANEL_TYPE_2KEY)
	param.detect_pole = GXSMC_DETECT_LOW_LEVEL;
#else
	param.detect_pole = GXSMC_DETECT_HIGH_LEVEL;
#endif
       param.io_conv = GXSMC_DATA_CONV_DIRECT;
    	param.parity = GXSMC_PARITY_ODD;//GXSMC_PARITY_EVEN;
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

uint8_t app_desai_cas50_api_smart_reset(uint8_t* pbyATR, uint8_t *pbyLen )
{
	GxSmcTimeParams    time;
	memset(&time,0,sizeof(GxSmcTimeParams));
	time.baud_rate  = RUNTIME_BAUD_RATE;
	 time.egt        = RUNTIME_EGT;
	 time.etu        = RUNTIME_ETU;
	 time.tgt        = RUNTIME_TGT;
	 time.twdt       = RUNTIME_TWDT;
	 time.wdt        = RUNTIME_WDT;

	 return app_porting_ca_smart_reset(time,pbyATR,pbyLen);
}

extern status_t app_search_lock_tp(uint32_t fre, uint32_t symb, fe_spectral_inversion_t inversion, fe_modulation_t modulation,uint32_t delayms);
extern void app_desai_cas50_finger_init(void);
uint8_t app_desai_cas50_api_init(void)
{
	EN_ERR_CAS    ret;
	char * menu_language = NULL;
	uint32_t freq = 0;
	uint32_t symbol_rate = 0;
	uint32_t qam = 0;

	app_porting_ca_flash_init(64*1024);
	app_porting_ca_demux_init();
	app_porting_ca_descramble_init();
	app_desai_cas50_rolling_message_init();
	app_desai_cas50_finger_init();
	app_porting_ca_create_filter_task();

	//2014-11-27,qm,��ǰ��ʼ�����ܿ������⿪���г�ʼ����ʾ
	//app_desai_cas50_api_smart_init();
	app_porting_ca_smart_create_task();

	ret=DSCA_Init(GXOS_DEFAULT_PRIORITY, CA_LANGUAGE_CHINESE);//16:47
    	if(ret==CA_ERROR_OK)
		CAS_Dbg("....DSCA Lib Init Ok......\n");
	else
	{
		CAS_Dbg("....DSCA Lib Init Failure...... ret=%d\n",ret);
		return ret;
	}
	
	/*Lock the center frequence to get NIT data.*/
	symbol_rate = app_flash_get_config_center_freq_symbol_rate();
	qam = app_flash_get_config_center_freq_qam();
	freq = app_flash_get_config_center_freq();
	if(0 == app_search_lock_tp(freq, symbol_rate,INVERSION_OFF, qam,2000))
	{
		/*��Ƶ�ɹ����ȴ�NIT�����˵�*/
		GxCore_ThreadDelay(2000);
	}
	
	menu_language = app_flash_get_config_osd_language();
	if(menu_language)
	{
		if(strcmp(menu_language,"English") == 0)
		{
			DSCA_SetSysLanguage(CA_LANGUAGE_ENGLISH);
		}
		else if(strcmp(menu_language,"Chinese") == 0)
		{
			DSCA_SetSysLanguage(CA_LANGUAGE_CHINESE);
		}
	}

	return TRUE;
}

uint8_t app_desai_cas50_api_card_in(char* atr,uint8_t len)
{
	uint8_t valid_art[22]={0x3B,0xFE,0x11,0x00,0x00,0x50,0x80,0xFF,0x5C,0x54,0x52,0x43,0x07,0x00,0x00,0x0B,0x08,0x11,0x00,0x06,0x00,0x80};
	int i = 0;
	int flag = 0;
	DSCA_SetCardState(1);
#ifdef DVB_CA_TYPE_TR_FLAG
	for (i=0; i<12; i++)
	{
		if (valid_art[i]!=atr[i])
		{
			printf(" smart card is not tr cas smart \n");
			return 1;
		}
	}
	app_cas_api_on_event(DVB_CA_CHANGE_INFO,CAS_NAME, &flag, 0);
#endif	

	return 1;
}

uint8_t app_desai_cas50_api_card_out(void)
{

	DSCA_SetCardState(0);
	
	return 1;
}



