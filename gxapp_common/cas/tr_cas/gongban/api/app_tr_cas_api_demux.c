/*****************************************************************************
* 						   CONFIDENTIAL
*        Hangzhou GuoXin Science and Technology Co., Ltd.
*                      (C)2012, All right reserved
******************************************************************************

******************************************************************************
* File Name :	app_gy_cas_api_demux.c
* Author    : 	xxxx
* Project   :	goxceed dvbc
* Type      :
******************************************************************************
* Purpose   :	模块头文件
******************************************************************************
* Release History:
VERSION	Date			  AUTHOR         Description
1.0  	2012.12.02		  xxxx		 			creation
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
#include "app_common_search.h"
#include "app_common_table_nit.h"

#include "Tr_Cas.h"


#define USE_REGIONCODE_SEARCH 

#ifdef USE_REGIONCODE_SEARCH
#define REGION_FREQ_NUM (4)

typedef struct
{
	uint8_t freq_num;
	uint32_t freq_list[REGION_FREQ_NUM];
}CAS_REGION_FREQ_INFO_t;
#endif

extern void tr_cas_release_ecm_filter(void);
extern void tr_cas_release_emm_filter(void);
extern uint8_t app_tr_cas_api_setup_region_code(uint16_t region_code);

extern play_scramble_para_t play_Para;
extern ca_descramble_t g_descramlbeList[MAX_SCRAMBLE_CHANNEL];

static play_scramble_para_t TrplayPara = {0, 0, 0, 0x1FFF, 0x1FFF, 0x1FFF, 0x1FFF, 0x1FFF, 0x1FFF};
static unsigned short int s_emmPid = PSI_INVALID_PID;

#ifdef USE_REGIONCODE_SEARCH
static handle_t s_semRegionCode = 0;
static uint8_t s_regionCodeFlag = 0;
static CAS_REGION_FREQ_INFO_t s_regionFreqInfo = {0};
static uint16_t s_curRegionCode = 0; /*wangjian add on 20150305*/
#endif


uint8_t	app_tr_cas_api_release_ecm_filter(void)
{
	play_scramble_para_t playpara = {0};

	CAS_Dbg("app_tr_cas_api_release_ecm_filter.\n");
	app_play_get_playing_para(&playpara);

	tr_cas_release_ecm_filter();

	playpara.p_ecm_pid = PSI_INVALID_PID;
	playpara.p_ecm_pid_video = PSI_INVALID_PID;
	playpara.p_ecm_pid_audio = PSI_INVALID_PID;
	app_play_set_playing_para(&playpara);
	return 1;
}

void app_tr_cas_api_get_app_scramble_para(play_scramble_para_t **Param)
{	
	if (Param)
	{
		*Param = &TrplayPara;
	}
}

uint8_t app_tr_cas_api_start_ecm_filter(play_scramble_para_t *pplayPara)
{
	CAS_Dbg("<app_tr_cas_api_start_ecm_filter>.\n");
	
	if (NULL == pplayPara)
	{
		return 0;
	}
	
	memcpy(&TrplayPara, pplayPara, sizeof(play_scramble_para_t));
	return 1;
}

uint8_t app_tr_cas_api_release_emm_filter(void)
{	
	CAS_Dbg("app_tr_cas_api_release_emm_filter.\n");
	tr_cas_release_emm_filter();
	s_emmPid = PSI_INVALID_PID;
	
	return TRUE;
}

uint8_t app_tr_cas_api_start_emm_filter(uint16_t emm_pid)
{
	CAS_Dbg("app_tr_cas_api_start_emm_filter(0x%x).\n", emm_pid);
	if (emm_pid != PSI_INVALID_PID)
	{
		s_emmPid = emm_pid;
	}
	
	return TRUE;
}

bool app_tr_cas_api_check_cat_ca_descriptor_valid(uint8_t *sectionData, uint32_t CA_system_id)
{
	if (CA_system_id == 0x4af9)
	{
		return TRUE;
	}

	printf("[CAT]Not TRCA->CA_system_id(0x%x)!!!\n", CA_system_id);
	return FALSE;
}

bool app_tr_cas_api_check_pmt_ca_descriptor_valid(uint8_t *sectionData, uint32_t CA_system_id)
{
	if (CA_system_id == 0x4af9)
	{
		return TRUE;
	}

	printf("[PMT]NOT TRCA->CA_system_id(0x%x)!!!\n", CA_system_id);
	return FALSE;
}

void app_tr_cas_api_nit_zone_des(uint8_t* databuf)
{	
	uint8_t *pSecData = databuf;
	uint8_t descLen = 0; 
	uint8_t *pRegionIdDesc = NULL;
	int8_t i = 0, count = 0;
	uint16_t regionFreq = 0;
	uint16_t regionCode = 0;
	#ifdef USE_REGIONCODE_SEARCH
	static uint8_t bInitFreqList = FALSE;
	#endif

	/*wangjian modify on 20141231*/
	printf("[app_tr_cas_api_nit_zone_des]Find TRCAS RegionCode Desc@.\n");
	descLen = pSecData[1];
	if ((descLen < 4) || (descLen % 4))
	{
		printf("[app_tr_cas_api_nit_zone_des]invaid TRCAS RegionCode Desc!!!\n");
		return;
	}

	i = 0;
	pRegionIdDesc = pSecData + 2;
	count = descLen / 4;
	if (0 == count)
	{
		return;
	}

	#ifdef USE_REGIONCODE_SEARCH
	if (FALSE == bInitFreqList)
	{
		bInitFreqList = TRUE;
		memset(&s_regionFreqInfo, 0, sizeof(s_regionFreqInfo));
	}
	#endif

	/*Find region code.*/
	while (count > 0)
	{
		regionFreq = ((pRegionIdDesc[0] << 8) | pRegionIdDesc[1]);
		regionCode = ((pRegionIdDesc[2] << 8) | pRegionIdDesc[3]);
		printf("[app_tr_cas_api_nit_zone_des]TRCAS(%d): Freq(%d); RegionCode(0x%x)#.\n",\
				i, regionFreq, regionCode);
		if (regionCode != 0)
		{
			break;
		}

		pRegionIdDesc += 4;
		count--;
		
		/*Invalid region code, then tune to the freq one by one.*/
		#ifdef USE_REGIONCODE_SEARCH
		if (0 == s_regionCodeFlag)
		{
			s_regionFreqInfo.freq_list[i] = regionFreq;
			s_regionFreqInfo.freq_num = i+1;

			i++;
			if (i > REGION_FREQ_NUM)
			{
				printf("[app_tr_cas_api_nit_zone_des]freq num exceed to %d!!!\n", REGION_FREQ_NUM);
				break;
			}
		}
		#endif
	}

	s_curRegionCode = regionCode;
	if (regionCode != 0)
	{
		#ifdef USE_REGIONCODE_SEARCH
		if (s_regionCodeFlag != 0)
		{
			s_regionCodeFlag = 2;
			GxCore_SemPost(s_semRegionCode);
		}
		#endif
	
		app_tr_cas_api_setup_region_code(regionCode);
	}
	else
	{
		#ifdef USE_REGIONCODE_SEARCH
		if (s_regionCodeFlag != 0)
		{
			s_regionCodeFlag = 3;
			GxCore_SemPost(s_semRegionCode);
		}
		#endif
	}
	
	return;
}

#ifdef USE_REGIONCODE_SEARCH
static uint8_t trcas_search_region_freq(uint32_t freq)
{
	uint8_t result = 0;
	int32_t symb = 6875;
	int32_t qam = 2;
	status_t lockState = 0;

	symb = app_flash_get_config_center_freq_symbol_rate();
	qam = app_flash_get_config_center_freq_qam();
	printf("+[trcas_search_region_freq]lock freq(%d %d %d).+\n",\
			freq, symb, qam);

	lockState = app_search_lock_tp(freq, symb, INVERSION_OFF, qam, 3000);
	if (1 == lockState)
	{	
		printf("[trcas_search_region_freq]lock freq(%d) fail!!!\n", freq);
		result = 1;
	}
	else
	{
		printf("[trcas_search_region_freq]wait get region code...\n");
		GxCore_SemTimedWait(s_semRegionCode, 3000);
		
		if (2 == s_regionCodeFlag)
		{
			result = 0;
		}
		else
		{
			result = 2;
		}
	}
	
	return result;
}
#endif

uint8_t app_tr_cas_api_search_regioncode(void)
{
	uint8_t result = 0;

	printf("###[app_tr_cas_api_search_regioncode]Enter###\n");
	if (s_curRegionCode != 0)
	{
		app_tr_cas_api_setup_region_code(s_curRegionCode);
		printf("###[app_tr_cas_api_search_regioncode]Over+##\n");
		return 0;
	}
	
#ifdef USE_REGIONCODE_SEARCH	
	uint32_t curFreq = 0;
	uint8_t i = 0;

	do
	{
		result = 1;
		GxCore_SemCreate(&s_semRegionCode, 0);
		if (0 == s_semRegionCode)
		{
			printf("[app_tr_cas_api_search_regioncode]s_semRegionCode is 0!!!\n");
			break;
		}

		s_regionCodeFlag = 1;

		for (i = 0; i < s_regionFreqInfo.freq_num; i++)
		{
			curFreq = s_regionFreqInfo.freq_list[i];
			printf("[app_tr_cas_api_search_regioncode]<%d>curFreq(%d).\n", i, curFreq);
			if (0 == curFreq)
			{
				continue;
			}
			
			app_table_nit_monitor_filter_restart();
			result = trcas_search_region_freq(curFreq);
			if (0 == result)
			{
				break;
			}
		}

		if (result != 0)
		{
			printf("###[app_tr_cas_api_search_regioncode]get region code fail!!!###\n");
		}
	}while (0);
	
	printf("###[app_tr_cas_api_search_regioncode]Over###\n");
#endif

	return result;
}

uint16_t app_tr_cas_api_get_regioncode(void)
{
	uint16_t regionCode = 0;
	
	if (0 != MC_SCARD_GetRegionCode(&regionCode))
	{
		printf("[app_tr_cas_api_get_regioncode]MC_SCARD_GetRegionCode fail!!!\n");
		regionCode = 0x0000;
	}

	return regionCode;
}


