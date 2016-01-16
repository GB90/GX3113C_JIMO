/*****************************************************************************
* 						   CONFIDENTIAL
*        Hangzhou GuoXin Science and Technology Co., Ltd.
*                      (C)2012, All right reserved
******************************************************************************

******************************************************************************
* File Name :	tr_cas_porting_system.c
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
#include "app_common_porting_stb_api.h"
#include "app_tr_cas_api_email.h"


#define TRCAS_WAIT_CHECK_SECS  ((60 * 5) * 1000)
#define TRCAS_NOTIFY_DAYS   (5) /*Default value is 5.*/

extern const char* i18n_get_string(const char *text);
extern void app_tr_cas_start_osd_rolling(CAS_MSG_STRUCT *pCas_Msg);

static TRCAS_EMAIL_t s_caEmailFlag = {0};
static handle_t s_semEntitleRemainder = 0;
static uint8_t s_entitleDays = TRCAS_NOTIFY_DAYS;/*wj add on 20150328*/
static uint8_t s_bMonitorInited = FALSE;/*wangjian add on 20150331*/
static event_list *pEntitleCheckTimerHdl = NULL;/*wangijan add on 20150331*/


void TRCA_toEmailNotify(uint8_t byShow)
{
	if ((TRCAS_EMAIL_NONE == byShow) 
		&& (DVB_CA_EMAIL_FLAG_HIDE == s_caEmailFlag.status))
	{
		return;
	}
	
	switch (byShow)
	{
		case TRCAS_EMAIL_NONE:
			s_caEmailFlag.status = DVB_CA_EMAIL_FLAG_HIDE;
			break;
			
		case TRCAS_EMAIL_NEW:
			s_caEmailFlag.status = DVB_CA_EMAIL_NEW;
			break;

		default:
			break;
	}

	//printf("[TRCA_toEmailNotify]email.state(%d).\n", s_caEmailFlag.status);
    app_cas_api_on_event(DVB_CA_EMAIL_INFO, CAS_NAME,\
       					 &s_caEmailFlag, sizeof(s_caEmailFlag));
	return;
}

static uint8_t cas_entitle_time_check(uint32_t *pRemain_day, uint16_t *pProd_id)
{
	uint16_t i = 0;
	CAS_TIMESTAMP startTime, endTime;
	uint32_t remainDay = 0;
	int16_t result = 0;
	
	for (i = 0; i < TRCAS_ENTITLE_NUM_MAX; i++)
	{
		result = MC_TIME_GetEntitleInfo((U8 *)(&startTime), (U8 *)(&endTime),\
										(U32 *)(&remainDay), i);
		if (((0 == result)) && (remainDay > 0) && (remainDay < s_entitleDays))
		{
			if (pRemain_day)
			{
				*pRemain_day = remainDay;
			}

			if (pProd_id)
			{
				*pProd_id = i;
			}

			printf("@<cas_entitle_time_check>find (%d)entitle days(%d) need to remainder.@\n", i, remainDay);
			break;	
		}
	}

	if (i >= TRCAS_ENTITLE_NUM_MAX)
	{
		printf("@<cas_entitle_time_check>NO find entitle remainder item!!!\n");
		return 1;
	}

	return 0;
}

static void monitor_entitle_task(void *arg)
{
	uint8_t result = 0;
	uint32_t remainDay = 0xffffff;
	uint16_t prodIdx = 0xffff;
	CAS_MSG_STRUCT scrollInfo = {0};
	uint8_t osdTxt[256] = {0};
	uint8_t tmpTxt[100] = {0};
	static uint8_t bWaitCheck = FALSE;

	while (1)
	{
		if (0 == s_semEntitleRemainder)
		{
			GxCore_ThreadDelay(TRCAS_WAIT_CHECK_SECS);
		}
		else
		{
			if (FALSE == bWaitCheck)
			{
				GxCore_SemTimedWait(s_semEntitleRemainder, TRCAS_WAIT_CHECK_SECS);
				bWaitCheck = TRUE;
			}
			else
			{
				GxCore_SemWait(s_semEntitleRemainder);
			}
		}

		//printf("[monitor_entitle_task]cas_entitle_time_check.@@@+++\n");
		result = cas_entitle_time_check(&remainDay, &prodIdx);
		if (0 == result)
		{
			memset(&scrollInfo, 0, sizeof(scrollInfo));
			strcpy((char *)osdTxt, (char *)i18n_get_string((const char *)"Entitle Remainder"));
			sprintf((char *)tmpTxt, "%d", remainDay);
			strcat((char *)osdTxt, (char *)tmpTxt);
			strcpy((char *)tmpTxt, (char *)i18n_get_string((const char *)"Entitle Remainder_1"));
			strcat((char *)osdTxt, (char *)tmpTxt);
			strcpy((char *)scrollInfo.bMsgData, (char *)osdTxt);
			//printf("---[monitor_entitle_task]reminder osdTxt(%s).---\n", osdTxt);
			scrollInfo.wDataLen = strlen((char *)scrollInfo.bMsgData);
			app_tr_cas_start_osd_rolling(&scrollInfo);
		}
	}
}

void app_trcas_porting_monitor_init(void)
{
	handle_t monitor_task = 0;

	if (s_bMonitorInited)
	{
		return;
	}

	printf("<app_trcas_porting_monitor_init>monitor entitle remainder init@.\n");
	GxCore_SemCreate(&s_semEntitleRemainder, 0);
	GxCore_ThreadCreate("TrCas_monitor", &monitor_task,\
						monitor_entitle_task, NULL, 8 * 1024,\
						GXOS_DEFAULT_PRIORITY+1);
	s_bMonitorInited = TRUE;
	
	return;
}

void app_trcas_porting_entitle_check(uint8_t entitle_days)
{
	if (FALSE == s_bMonitorInited)
	{
		return;
	}

	printf("+++[app_trcas_porting_entitle_check]EntitleDays<%d>+++.\n", entitle_days);
	if (entitle_days > 0)
	{
		s_entitleDays = entitle_days;
	}
	
	GxCore_SemPost(s_semEntitleRemainder);
	return;
}

static int trcas_porting_timer_entitle_check(void *userdata)
{
	app_trcas_porting_entitle_check(0);
	return 0;
}

void app_trcas_porting_entitle_timer_check(void)
{
	if (pEntitleCheckTimerHdl != NULL)
	{
		remove_timer(pEntitleCheckTimerHdl);
		pEntitleCheckTimerHdl = NULL;	
	}
	
	pEntitleCheckTimerHdl = create_timer(trcas_porting_timer_entitle_check,\
		   			   						10000, NULL, TIMER_ONCE);
	return;
}


