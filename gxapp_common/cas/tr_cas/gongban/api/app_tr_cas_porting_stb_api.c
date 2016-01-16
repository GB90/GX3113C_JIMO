/*****************************************************************************
* 						   CONFIDENTIAL
*        Hangzhou GuoXin Science and Technology Co., Ltd.
*                      (C)2012, All right reserved
******************************************************************************

******************************************************************************
* File Name :	app_tr_cas_poring_stb_api.c
* Author    : 	xxxx
* Project   :	goxceed dvbc
* Type      :
******************************************************************************
* Purpose   :	模块头文件
******************************************************************************
* Release History:
VERSION	Date			  AUTHOR         Description
1.0  	2014.12.02		  xxxx	 			creation
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
#include "app_common_prog.h"
#include "app_common_porting_ca_os.h"

#include "Tr_Cas.h"
#include "app_tr_cas_api_smart.h"
#include "app_tr_cas_api_demux.h"
#include "tr_cas_porting.h"
#include "app_tr_cas_api_baseinfo.h"
#include "app_tr_cas_api_pin.h"
#include "app_tr_cas_api_rating.h"
#include "app_tr_cas_api_entitle.h"
#include "app_tr_cas_api_ippv_info.h"
#include "app_tr_cas_api_ippv_slot.h"
#include "app_tr_cas_api_ippv_bill.h"
#include "app_tr_cas_api_finger.h"
#include "app_tr_cas_api_email.h"
#include "app_tr_cas_api_rolling_message.h"
#include "app_tr_cas_api_announce.h"
#include "app_tr_cas_api_crc32.h"
#include "app_tr_cas_api_lock_service.h"

/*Marcos -----------------------------------------------------------------*/
#define CAS_FLASH_DATA_SIZE   (FLASH_TRCAS_COMMON_SIZE+EMAIL_DATA_SIZE+ANNOUNCE_DATA_SIZE)
#define CAS_MSG_DATA_CRC_LEN  (TRCAS_TITLE_LEN+TRCAS_CONTENT_LEN)

/*Extern data-----------------------------------------------------------------*/
extern void TRCA_toEmailNotify(uint8_t byShow);
extern void app_show_prompt(char* context);
extern void app_hide_prompt(void); 
extern void app_tr_cas_parent_lock(void);
extern void app_init_ipp_buy(void* pIppData);
extern void app_init_feed_card(void);
extern void app_trcas_porting_monitor_init(void);
extern void app_porting_ca_smart_check_status(void);
extern void app_trcas_porting_entitle_check(uint8_t entitle_days);
extern void app_trcas_ipp_indicate_show(uint8_t ipp_flag, uint8_t show_flag);
extern void app_trcas_porting_entitle_timer_check(void);

/*Local data-----------------------------------------------------------------*/
static void (*tr_show_prompt)(char* context) = NULL;
static void (*tr_hide_prompt)(void) = NULL; 
static void (*tr_parent_lock)(uint8_t showFlag) = NULL; 
static void (*tr_show_ipp_buy)(void* ippdata) = NULL; 
static void (*tr_show_feed_card)(void) = NULL;/*wangjian add on 20150106*/
static uint8_t (*tr_ipp_indicate)(uint8_t ipp_flag, uint8_t show_flag) = NULL;/*wangijan add on 20150330*/

static tr_cas_finger_data_st s_FingerData = {0,};
static CAS_IPPNOTIFY_STRUCT s_IppData = {0,};
static TR_CAS_ROLLING_MSG_t s_rollingInfo = {0};
static TR_CAS_ANNOUNCE_INFO_t s_announceInfo = {0};
static char s_IPPVFlag = 0;
static unsigned char chSpbyFeedData[256] = {0};
static unsigned char chLen = 0;
static uint8_t s_casMsgTitleData[CAS_MSG_DATA_CRC_LEN];
static uint8_t s_bCaModeInited = FALSE;
static handle_t s_semNotifyRoll = 0; /*wangjian add on 20150309*/
static event_list *pIpptMarkFlushTimerHdl = NULL;/*wangijan add on 20150330*/
static uint8_t s_ippMarkNeedShow = FALSE;/*wangijan add on 20150330*/
static uint8_t s_ippMarkNeedClear = FALSE;/*wangijan add on 20150331*/
static uint8_t s_bOsdRollEnable = FALSE;/*wangijan add on 20150331*/


/*API-----------------------------------------------------------------*/
static void app_tr_register_popMsg(void (*func)(char* context))
{
	tr_show_prompt = func;
}

static void app_tr_register_hideMsg(void (*func)(void))
{
	tr_hide_prompt = func;
}

static void app_tr_register_ipp_buy(void (*func)(void*))
{
	tr_show_ipp_buy = func;
}

#if 0 /*this compile rise to warning???*/
static void app_tr_register_parent_lock(void (*func)(uint8_t))
{
	tr_parent_lock = func;
}
#else
static void app_tr_register_parent_lock(void)
{
	tr_parent_lock = (void (*)(uint8_t))app_tr_cas_parent_lock;
}
#endif

static void app_tr_register_ipp_indicate(void)
{
	tr_ipp_indicate = (uint8_t (*)(uint8_t, uint8_t))app_trcas_ipp_indicate_show;
}

static void app_tr_register_feed_card(void (*func)(void))
{
	tr_show_feed_card = func;
}

static int8_t trcas_compare_timestamp(CAS_TIMESTAMP *pTimeStamp_1, CAS_TIMESTAMP *pTimeStamp_2)
{
	int result = 0;
	uint8_t bNewNotify = 0;
	
	if ((NULL == pTimeStamp_1) || (NULL == pTimeStamp_2))
	{
		printf("<trcas_compare_timestamp>param is NULL!!!\n");
		return 0;
	}
		
	result = memcmp(pTimeStamp_1, pTimeStamp_2, sizeof(CAS_TIMESTAMP));
	if (result > 0)
	{
		bNewNotify = 1;
	}
	else
	{
		bNewNotify = 0;
	}

	return bNewNotify;
}

/*CAS_MSG title and content CRC32 calute.*/
static uint32_t tr_cas_msg_crc32_calute(CAS_MSG_STRUCT *pCas_Msg)
{
	uint32_t crc32Val;
	uint8_t *pTmpData = NULL;
	uint32_t tmpSize = 0;
	uint16_t tmpLen = 0;
	uint16_t totalLen = 0;

	if (NULL == pCas_Msg)
	{
		return 0;
	}
	
	memset(s_casMsgTitleData, 0, sizeof(s_casMsgTitleData));
	pTmpData = s_casMsgTitleData;
	
	tmpLen = TRCAS_MIN(TRCAS_TITLE_LEN, pCas_Msg->wTitleLen);
	memcpy(s_casMsgTitleData, pCas_Msg->bMsgTitle, tmpLen);
	pTmpData += tmpLen;
	totalLen += tmpLen;

	tmpLen = TRCAS_MIN(TRCAS_CONTENT_LEN, pCas_Msg->wDataLen);
	memcpy(pTmpData, pCas_Msg->bMsgData, tmpLen);
	totalLen += tmpLen;

	if (0 == totalLen)
	{
		printf("[tr_cas_msg_crc32_calute]string is NULL!!!\n");
		return 0;
	}

	pTmpData = s_casMsgTitleData;
	tmpSize = totalLen * sizeof(uint8_t);
	app_tr_cas_crc32_init(32, 0x04C11DB7, (uint32_t)-1, 0, 0, 0x0);
	
    do
    {
        app_tr_cas_crc32_nextbyte(*pTmpData++);
    } while (--tmpSize);
	
    crc32Val = app_tr_cas_crc32_get_result();
	//printf("+++[tr_cas_msg_crc32_calute]crc32Val(0x%x)+++\n", crc32Val);
	
	return crc32Val;
}

static void app_tr_cas_notify_email(CAS_MSG_STRUCT *pCas_Msg)
{
	uint32_t tmpCrc32 = 0;
	int result = 0;

	if (NULL == pCas_Msg)
	{
		printf("[app_tr_cas_notify_email]pCas_Msg is NULL!!!\n");
		return;
	}

	tmpCrc32 = tr_cas_msg_crc32_calute(pCas_Msg);
	if (1 == app_tr_cas_api_email_isnew(tmpCrc32))
	{
		result = app_tr_cas_email_list_update((PVOID)pCas_Msg, tmpCrc32);
		if (0 == result)
		{
			TRCA_toEmailNotify(TRCAS_EMAIL_NEW);
		}
	}

	return;
}

static void app_tr_cas_notify_announce(CAS_MSG_STRUCT *pCas_Msg)
{
	uint16_t tmpLen = 0;
	uint32_t tmpCrc32 = 0;
	
	if (NULL == pCas_Msg)
	{
		printf("[app_tr_cas_notify_announce]pCas_Msg is NULL!!!\n");
		return;
	}

	/*compare CRCs of title and content whether is new; 
	*if full, then compare create time, replace the oldest.*/
	tmpCrc32 = tr_cas_msg_crc32_calute(pCas_Msg);
	if (0 == app_tr_cas_announce_is_new(tmpCrc32))
	{
		return;
	}
    
	memset(&s_announceInfo, 0, sizeof(s_announceInfo));/*wangjian add on 20150327*/
	s_announceInfo.status = ANNOUNCE_UNREAD;
	s_announceInfo.data_crc32 = tmpCrc32;
	memcpy(&s_announceInfo.create_time, &pCas_Msg->sCreateTime, sizeof(CAS_TIMESTAMP));
	
	s_announceInfo.level = pCas_Msg->bPriority;
	tmpLen = TRCAS_MIN(ANNOUNCE_TITLE_LEN, pCas_Msg->wTitleLen);
	s_announceInfo.title_len = tmpLen;
	memcpy(s_announceInfo.title, pCas_Msg->bMsgTitle, tmpLen);

	tmpLen = TRCAS_MIN(ANNOUNCE_CONTENT_LEN, pCas_Msg->wDataLen);
	s_announceInfo.content_len = tmpLen;
	memcpy(s_announceInfo.content, pCas_Msg->bMsgData, tmpLen);
	
	app_cas_api_on_event(DVB_CA_CURTAIN_INFO, CAS_NAME,\
		    			&s_announceInfo, sizeof(s_announceInfo));
	return;
}

/*wangjian add on 20150110*/
static uint16_t tr_cas_clear_symbol(uint8_t *strSource, uint8_t *strDest)
{
	uint16_t i = 0, j = 0;
	uint16_t tmpLen = 0;

	if ((NULL == strSource) || (NULL == strDest))
	{
		return 0;
	}

	tmpLen = strlen((char *)strSource);
	for (i = 0; i < tmpLen; i++)
	{
		if (j >= TR_CA_MAX_LEN_OSD)
		{
            strDest[j-1] = '\0';
			break;
		}
        
		if ((0x0d != strSource[i]) && (0x0a != strSource[i]))
		{
			strDest[j++] = strSource[i];
		}
	}

	return j;
}

static void tr_cas_init_osd_roll_data(void)
{
	GxCore_SemCreate(&s_semNotifyRoll, 1);
	
	memset(&s_rollingInfo, 0, sizeof(s_rollingInfo));
	s_rollingInfo.byStyle = TR_CA_OSD_TOP;
	s_rollingInfo.wTimes = TR_CA_SCROLL_TIMES;
	s_rollingInfo.valid_data = FALSE;
	return;
}

static int tr_cas_timer_roll(void *userdata)
{
	GxCore_SemWait(s_semNotifyRoll);/*wj add on 20150330*/
    printf("@@@@@@+[tr_cas_timer_roll]+######.\n");
    app_cas_api_on_event(DVB_CA_ROLLING_INFO, CAS_NAME,\
			    			&s_rollingInfo,\
			    		  sizeof(TR_CAS_ROLLING_MSG_t));
	s_rollingInfo.timer_handle = NULL;
	GxCore_SemPost(s_semNotifyRoll);
	
	return 0;
}

static int tr_cas_timer_ippt_mark_flush(void *userdata)
{
	if (tr_ipp_indicate != NULL)
	{/*wj add on 20150330*/
		printf("+------+[tr_cas_timer_ippt_mark_flush]RED->GREEN+------+.\n");
		tr_ipp_indicate(1, TRUE);
	}

	return 0;
}

static void tr_cas_ippt_mark_timer_start(void)
{
	if (pIpptMarkFlushTimerHdl != NULL)
	{
		remove_timer(pIpptMarkFlushTimerHdl);
		pIpptMarkFlushTimerHdl = NULL;	
	}
	
	pIpptMarkFlushTimerHdl = create_timer(tr_cas_timer_ippt_mark_flush,\
		   			   						4000, NULL, TIMER_ONCE);
	return;
}

void app_tr_cas_notify_osd_roll(CAS_MSG_STRUCT *pCas_Msg)
{
	uint8_t result = 1;
	uint32_t tmpCrc32 = 0;
	//TR_CAS_ROLLING_MSG_t rollingInfo;
	uint8_t entitleDays = 0;
	uint16_t osdContentLen = 0;

	result = 1;
	GxCore_SemWait(s_semNotifyRoll);
	
	do
	{
		if (NULL == pCas_Msg)
		{
			printf("[app_tr_cas_notify_osd_roll]pCas_Msg is NULL!!!\n");
			break;
		}

		#if 0 /*wangjian modify on 20150328*/
		if (1 == app_tr_cas_get_cur_rolling_info(TR_CA_OSD_TOP, &rollingInfo))
		{
			printf("[app_tr_cas_notify_osd_roll]app_tr_cas_get_cur_rolling_info fail!!!\n");
			break;
		}
		#endif

		tmpCrc32 = tr_cas_msg_crc32_calute(pCas_Msg);
		if (tmpCrc32 == s_rollingInfo.dataCrc32)
		{
			/*Old message.*/
			break;
		}
		else
		{
			//printf("[app_tr_cas_notify_osd_roll]CRC32 Differ(0x%x-0x%x)!!!\n", 
				  		//s_rollingInfo.dataCrc32, tmpCrc32);
		}

		if (0 == trcas_compare_timestamp(&pCas_Msg->sCreateTime, &s_rollingInfo.create_time))
		{
			printf("[app_tr_cas_notify_osd_roll]new osd info create time is older!!!\n");
			break;
		}

		if (s_rollingInfo.timer_handle)
		{
			remove_timer(s_rollingInfo.timer_handle);
			s_rollingInfo.timer_handle = NULL;
		}

		memset(&s_rollingInfo, 0, sizeof(s_rollingInfo));
		s_rollingInfo.dataCrc32 = tmpCrc32;
		s_rollingInfo.byStyle = TR_CA_OSD_TOP;

		entitleDays = pCas_Msg->bPriority;
		printf("[app_tr_cas_notify_osd_roll]wPeriod(%d); bPriority(%d)----\n",\
					pCas_Msg->wPeriod, pCas_Msg->bPriority);
		if (pCas_Msg->wPeriod > 0)
		{
			s_rollingInfo.period = pCas_Msg->wPeriod;
		}
		else
		{
			s_rollingInfo.period = 0;
		}

		memcpy(&s_rollingInfo.create_time, &pCas_Msg->sCreateTime, sizeof(CAS_TIMESTAMP));
		memset(s_rollingInfo.content, 0, sizeof(s_rollingInfo.content));
		osdContentLen = tr_cas_clear_symbol(pCas_Msg->bMsgData, s_rollingInfo.content);
		osdContentLen = TRCAS_MIN(TR_CA_MAX_LEN_OSD, osdContentLen);
		s_rollingInfo.len = osdContentLen;
		s_rollingInfo.wTimes = TR_CA_SCROLL_TIMES;

		if (osdContentLen > 0)
		{
			s_rollingInfo.valid_data = TRUE;
		}
		else
		{
			s_rollingInfo.valid_data = FALSE;
		}

		//printf("***[app_tr_cas_notify_osd_roll]valid(%d); osdstring(%s)***\n",
					//s_rollingInfo.valid_data, s_rollingInfo.content);
		result = 0;
	}while (0);
	
	GxCore_SemPost(s_semNotifyRoll);

	if (0 == result)
	{
		app_trcas_porting_entitle_check(entitleDays);
	}
	
	return;
}

void app_tr_cas_start_osd_rolling(CAS_MSG_STRUCT *pCas_Msg)
{
	uint16_t osdContentLen = 0;
	
	GxCore_SemWait(s_semNotifyRoll);

	do
	{
		if (NULL == pCas_Msg)
		{
			printf("[app_tr_cas_start_osd_rolling]pCas_Msg is NULL!!!\n");
			break;
		}

		if (s_rollingInfo.timer_handle)
		{
			remove_timer(s_rollingInfo.timer_handle);
			s_rollingInfo.timer_handle = NULL;
		}
		
		if (FALSE == s_rollingInfo.valid_data)
		{
			memset(s_rollingInfo.content, 0, sizeof(s_rollingInfo.content));
			osdContentLen = tr_cas_clear_symbol(pCas_Msg->bMsgData, s_rollingInfo.content);
			osdContentLen = TRCAS_MIN(TR_CA_MAX_LEN_OSD, osdContentLen);
			s_rollingInfo.len = osdContentLen;
			printf("===[app_tr_cas_start_osd_rolling]Default OsdTxt(%s).===\n", s_rollingInfo.content);
		}
		
		s_rollingInfo.wStatus = TRUE;
	    s_rollingInfo.roll_status = FALSE;

		#if 0
		if (s_rollingInfo.period > 0)
		{
			printf("[app_tr_cas_start_osd_rolling]Timer roll(%ds) create>>>+++.\n", s_rollingInfo.period);
			s_rollingInfo.timer_handle = create_timer(tr_cas_timer_roll,\
				   									  s_rollingInfo.period*1000,\
				   			   						  NULL, TIMER_REPEAT);
		}
		#endif
		
		app_cas_api_on_event(DVB_CA_ROLLING_INFO, CAS_NAME,\
			    		     &s_rollingInfo, sizeof(TR_CAS_ROLLING_MSG_t));
	}while (0);
	
	GxCore_SemPost(s_semNotifyRoll);
	return;
}

void app_tr_cas_osd_roll_timer_check(void)
{
	if (FALSE == s_bOsdRollEnable)
	{
		return;	
	}
	
	GxCore_SemWait(s_semNotifyRoll);
	
	if (s_rollingInfo.period > 0)
	{
		printf("[app_tr_cas_osd_roll_timer_check]Create Roll(%ds) Timer>>>+++.\n",\
			  		s_rollingInfo.period);

		if (s_rollingInfo.timer_handle)
		{
			remove_timer(s_rollingInfo.timer_handle);
			s_rollingInfo.timer_handle = NULL;
		}
		
		s_rollingInfo.timer_handle = create_timer(tr_cas_timer_roll,\
			   									  s_rollingInfo.period*1000,\
			   			   						  NULL, TIMER_ONCE);
	}
	
	GxCore_SemPost(s_semNotifyRoll);
	return;
}

void app_tr_cas_over_osd_roll(void)
{
	printf("======[app_tr_cas_over_osd_roll]-----\n");
	s_bOsdRollEnable = FALSE;

	if (0 == s_semNotifyRoll)
	{
		return;
	}

	GxCore_SemWait(s_semNotifyRoll);
	
	if (s_rollingInfo.timer_handle)
	{
		remove_timer(s_rollingInfo.timer_handle);
		s_rollingInfo.timer_handle = NULL;
	}
	
	GxCore_SemPost(s_semNotifyRoll);
	
	return;
}

void app_tr_cas_reset_entitle_check(void)
{
	printf("------[app_tr_cas_reset_entitle_check]======\n");
	s_bOsdRollEnable = TRUE;
	app_trcas_porting_entitle_timer_check();
	return;
}


/*wangjian add on 20150331*/
void app_tr_cas_ipp_mark_clear_check(uint16_t serv_id)
{
	uint8_t result = 0;
	static int32_t oldServId = -1;
	
	//printf("+++[app_tr_cas_ipp_mark_clear_check](%d-%d).++++++\n", oldServId, serv_id);
	if ((oldServId != -1) && (oldServId == serv_id))
	{
		return;
	}

	oldServId = serv_id;
	s_ippMarkNeedShow = FALSE;
	
	result = tr_ipp_indicate(0, FALSE);
	if (result != 0)
	{
		s_ippMarkNeedClear = TRUE;
	}
	else
	{
		s_ippMarkNeedClear = FALSE;
	}

	return;
}


static void TRCAS_Notify(CAS_NOTIFY_CONTROL eCaMsg, U16 wDemux, U32 ulParam)
{
	int ret = 0;
	uint32_t pos = 0;
	GxBusPmDataProg prog_data = {0};
	CAS_FINGERPRINT_STRUCT stTrFinger;
	CAS_ENHANCED_FP_STRUCT stTrEnhanceFinger;
	uint8_t result = 0;
	
	switch (eCaMsg)
	{
		case CAS_MC_DECODER_INFO:
			printf("+[TRCAS_Notify]CAS_MC_DECODER_INFO.+\n");
			break;
			
		case CAS_MC_SERVICE_INFO:
			printf("+[TRCAS_Notify]CAS_MC_SERVICE_INFO.+\n");
			break;
			
		case CAS_MC_SC_ACCESS:
			printf("+[TRCAS_Notify]CAS_MC_SC_ACCESS(%d).+\n", wDemux);
			if (((wDemux == CAS_STATE_E16) 
				|| (wDemux == CAS_STATE_E04) 
				|| (wDemux == CAS_STATE_E34)) 
				&& app_porting_ca_smart_status())
			{
				break;
			}
			
			if (wDemux == CAS_STATE_E23)
			{
				wDemux = 0;
			}
			
			if (0xff != wDemux)
			{
				if ((MSG_POP_COMMON_END+wDemux+1) >= MESSAGE_MAX_COUNT)
				{
					return;		
				}	
			}
			
			if (TRUE != app_play_get_msg_pop_type_state(MSG_POP_COMMON_END+wDemux+1))
			{
				app_play_clear_ca_msg();
				app_play_set_msg_pop_type(MSG_POP_COMMON_END+wDemux+1);
			}
			break;
			
		case CAS_MC_PIN_CODE_CHANGE:
			printf("+[TRCAS_Notify]CAS_MC_PIN_CODE_CHANGE.+\n");
			break;
			
		case CAS_MC_PIN_CODE_CHECK:
			printf("+[TRCAS_Notify]CAS_MC_PIN_CODE_CHECK.+\n");
			break;
			
		case CAS_MC_MONITOR_ECM:
			printf("+[TRCAS_Notify]CAS_MC_MONITOR_ECM.+\n");
			break;
			
		case CAS_MC_MONITOR_EMM:
			printf("+[TRCAS_Notify]CAS_MC_MONITOR_EMM.+\n");
			break;
			
		case CAS_MC_IPP_NOTIFY:
			printf("+[TRCAS_Notify]CAS_MC_IPP_NOTIFY.+\n");

			if (1 == wDemux)
			{
				printf("#-#-#-#-#-#[TROSD]IPPT节目的自动扣钱.#-#-#-#-#-#\n");
				if (tr_ipp_indicate != NULL)
				{
					tr_ipp_indicate(2, TRUE);
					tr_cas_ippt_mark_timer_start();
				}
				break;
			}

			if (0 != wDemux)//不是IPP类型的节目
			{
				printf("\n[TROSD]不是IPP类型的节目.\n");
				break;
			}

			app_prog_get_playing_pos_in_group(&pos);
			GxBus_PmProgGetByPos(pos, 1, &prog_data);
			if (s_IppData.wChannelId != prog_data.service_id)
			{
				//printf("[TROSD]This is not IPP!!!\n");
			}
			else
			{
				printf("[TROSD]This is a IPP prog(0x%x)*!*!*!*\n", s_IppData.wChannelId);
				s_IPPVFlag = 1;
				//app_cas_api_on_event(DVB_CA_IPPV_POP_INFO, CAS_NAME,
				   				 	// &s_IppData, sizeof(CAS_IPPNOTIFY_STRUCT));
			}
			break;
			
		case CAS_MC_IPP_INFO_UPDATE:
			printf("+[TRCAS_Notify]CAS_MC_IPP_INFO_UPDATE.+\n");
			
			if (0 == wDemux)//不是IPP类型的节目
			{
				if (tr_ipp_indicate != NULL)
				{
					tr_ipp_indicate(0, FALSE);
				}
				break;
			}
			
			ret = MC_STATE_QueryControl(eCaMsg, wDemux, (PVOID)&s_IppData);

			if (tr_ipp_indicate != NULL)
			{
				result = tr_ipp_indicate(s_IppData.bIppType, TRUE);
				if (result != 0)
				{
					s_ippMarkNeedShow = TRUE;
				}
			}
			break;
			
		case CAS_MC_ENHANCED_SHORT_MESSAGE:
		case CAS_MC_NOTIFY_SHORT_MESSAGE:
		{
			CAS_MSG_STRUCT caMsgInfo;
			CAS_ENHANCED_MSG_STRUCT caEnhancedMsgInfo;
			//uint32_t tmpCrc32 = 0;
			//int result = 0;
			uint16_t tmpLen = 0;

			memset(&caMsgInfo, 0, sizeof(caMsgInfo));
			if (CAS_MC_ENHANCED_SHORT_MESSAGE == eCaMsg)
			{
				printf("+[TRCAS_Notify]CAS_MC_ENHANCED_SHORT_MESSAGE.+\n");
				memset(&caEnhancedMsgInfo, 0, sizeof(caEnhancedMsgInfo));
				ret = MC_STATE_QueryControl(eCaMsg, wDemux, (PVOID)(&caEnhancedMsgInfo)); 
				if (0 == ret)
				{
					caMsgInfo.wIndex = caEnhancedMsgInfo.wIndex;
					caMsgInfo.bType = caEnhancedMsgInfo.bType;
					caMsgInfo.bClass = caEnhancedMsgInfo.bClass;
					caMsgInfo.bPriority = caEnhancedMsgInfo.bPriority;
					caMsgInfo.wPeriod = caEnhancedMsgInfo.dwPeriod;
					caMsgInfo.wTitleLen = caEnhancedMsgInfo.bTitleLen;
					caMsgInfo.wDataLen = caEnhancedMsgInfo.wDataLen;
					memcpy(&caMsgInfo.sCreateTime, &caEnhancedMsgInfo.sCreateTime, sizeof(CAS_TIMESTAMP));
			
					tmpLen = TRCAS_MIN(caMsgInfo.wTitleLen, MSG_TITLE_MAX_LENGTH);
					memcpy(caMsgInfo.bMsgTitle, caEnhancedMsgInfo.MsgTitle, tmpLen);
					tmpLen = TRCAS_MIN(caMsgInfo.wDataLen, MSG_DATA_MAX_LENGTH);
					memcpy(caMsgInfo.bMsgData, caEnhancedMsgInfo.MsgData, tmpLen);
				}
			}
			else
			{
				printf("+[TRCAS_Notify]CAS_MC_NOTIFY_SHORT_MESSAGE.+\n");
				ret = MC_STATE_QueryControl(eCaMsg, wDemux, (PVOID)(&caMsgInfo)); 
			}

			if (ret != 0)
			{
				printf("+[TRCAS_Notify]SHORT MESSAGE->get info fail!!!+\n");
				break;
			}
			
			if (CAS_MSG_MAIL == caMsgInfo.bType)
			{
				printf("+++[TRCAS_Notify]Got Email info+++\n");
				app_tr_cas_notify_email(&caMsgInfo);
			}
			else if (CAS_MSG_ANNOUNCE == caMsgInfo.bType)
			{
				printf("###[TRCAS_Notify]Got Announce info###\n");
				app_tr_cas_notify_announce(&caMsgInfo);	
			}
			else if (CAS_MSG_NOTIFICATION == caMsgInfo.bType)
			{
				printf("@@@[TRCAS_Notify]Got Notification info@@@\n");
				app_tr_cas_notify_osd_roll(&caMsgInfo);
			}
		}
			break;
			
		case CAS_MC_ENHANCED_FINGER_PRINT:
			printf("+[TRCAS_Notify]CAS_MC_ENHANCED_FINGER_PRINT.+\n");
			memset(&s_FingerData, 0x00, sizeof(tr_cas_finger_data_st));
			memset(&stTrEnhanceFinger, 0x00, sizeof(stTrEnhanceFinger));
			ret = MC_STATE_QueryControl(eCaMsg, wDemux, (PVOID)&stTrEnhanceFinger);
			if (0 == ret)
			{
				s_FingerData.wDuration = 4;
				s_FingerData.wStatus = TRUE;	
				s_FingerData.bHashedNumLen = stTrEnhanceFinger.bScNumLen;
				printf("[ENHANCED_FINGER]dwOnTime(%ld),dwOffTime(%ld),bRepeatNum(%d);\n",\
					  	stTrEnhanceFinger.dwOnTime,\
					  	stTrEnhanceFinger.dwOffTime,\
					  	stTrEnhanceFinger.bRepeatNum);
				printf("[ENHANCED_FINGER]bXPos(%d), bYPos(%d);\n",\
					   stTrEnhanceFinger.bXPos, stTrEnhanceFinger.bYPos);
				printf("[ENHANCED_FINGER]R(0x%x), G(0x%x), B(0x%x).\n",\
					   stTrEnhanceFinger.bBackgColorRed,\
					   stTrEnhanceFinger.bBackgColorGreen,\
					   stTrEnhanceFinger.bBackgColorBlue);

				memcpy(s_FingerData.pbHashedNum, stTrEnhanceFinger.ScNumString, stTrEnhanceFinger.bScNumLen);
				app_cas_api_on_event(DVB_CA_FINGER_INFO,CAS_NAME, &s_FingerData, sizeof(tr_cas_finger_data_st));
			}
			break;
			
		case CAS_MC_FORCE_CHANNEL:
		{
			printf("+[TRCAS_Notify]CAS_MC_FORCE_CHANNEL.+\n");
			CAS_FORCECHANNEL_STRUCT forceChannel;

			ret = MC_STATE_QueryControl(eCaMsg, wDemux, (PVOID)&forceChannel);
			if (0 == ret)
			{
				app_tr_cas_api_lock_service(&forceChannel);
			}
		}
			break;
			
		case CAS_MC_FINGER_PRINT:
			printf("+[TRCAS_Notify]CAS_MC_FINGER_PRINT.+\n");
			memset(&stTrFinger,0x00,sizeof(stTrFinger));
			memset(&s_FingerData,0x00,sizeof(tr_cas_finger_data_st));
			ret = MC_STATE_QueryControl(eCaMsg, wDemux, (PVOID)&stTrFinger);
			if (0 == ret)
			{
				s_FingerData.wDuration = 4;
				s_FingerData.wStatus = TRUE;	
				s_FingerData.bHashedNumLen = stTrFinger.bHashedNumLen;
				memcpy(s_FingerData.pbHashedNum, stTrFinger.pbHashedNum, stTrFinger.bHashedNumLen);
				app_cas_api_on_event(DVB_CA_FINGER_INFO,CAS_NAME, &s_FingerData, sizeof(tr_cas_finger_data_st));
			}
			break;
			
		case CAS_MC_EMM_DOWNLOAD:
			printf("+[TRCAS_Notify]CAS_MC_EMM_DOWNLOAD+\n");
			break;
			
		case CAS_MC_EMM_CHANGEPIN:
			printf("+[TRCAS_Notify]CAS_MC_EMM_CHANGEPIN+\n");
			break;
			
		case CAS_MC_NOTIFY_EXPIRY_STATE:
			printf("+[TRCAS_Notify]CAS_MC_NOTIFY_EXPIRY_STATE+\n");
			break;
			
		case CAS_MC_NOTIFY_CURRENT_STATE:
			printf("+[TRCAS_Notify]CAS_MC_NOTIFY_CURRENT_STATE->wDemux(%d).+\n", wDemux);
			if (((wDemux == CAS_STATE_E16) 
				|| (wDemux == CAS_STATE_E04) 
				|| (wDemux == CAS_STATE_E34))
				&& app_porting_ca_smart_status())
			{	
				break;
			}

			if ((CAS_STATE_E00 == wDemux) || (CAS_STATE_E23 == wDemux))
			{
				/*wangjian add on 20150328 for clear CA pop box.*/
				if (tr_parent_lock != NULL)
				{
					tr_parent_lock(1);
				}

				if (tr_show_ipp_buy != NULL)
				{
					tr_show_ipp_buy(NULL);
				}
			}
			
			if (wDemux == CAS_STATE_E23)
			{
				wDemux = 0;
			}
			
			if (0xff != wDemux)
			{
				if ((MSG_POP_COMMON_END+wDemux+1) >= MESSAGE_MAX_COUNT)
				{
					return;
				}			
			}
			
			if (TRUE != app_play_get_msg_pop_type_state(MSG_POP_COMMON_END+wDemux+1))
			{
				app_play_clear_ca_msg();
				app_play_set_msg_pop_type(MSG_POP_COMMON_END+wDemux+1);
			}
			
			if (wDemux == CAS_STATE_E31)
			{
				app_cas_api_on_event(DVB_CA_RATING_INFO, CAS_NAME, NULL, 0);
			}
			else if (CAS_STATE_E12 == wDemux)
			{
				app_cas_api_on_event(DVB_CA_MOTHER_CARD_INFO, CAS_NAME, NULL, 0);
			}
			break;
			
		default:
		{
			printf("+[TRCAS_Notify]invalid eCaMsg(%d).+\n", eCaMsg);
		}
			break;
	}

	return;
}

uint8_t app_tr_cas_api_setup_region_code(uint16_t region_code)
{
	if (!s_bCaModeInited)
	{
		//printf("[app_tr_cas_api_setup_region_code]TRCA module has been not inited!!!\n");
		return 1;
	}

	printf("[app_tr_cas_api_setup_region_code]MC_SetRegionCode(0x%x)@.\n", region_code);
	MC_PostRegionCode(region_code);
	
	return 0;
}

uint8_t app_tr_cas_api_init(void)
{
	int ret = 0;
	char *strVer = NULL;

	if (s_bCaModeInited)
	{
		printf("[app_tr_cas_api_init]s_bCaModeInited is true!!!\n");
		return FALSE;
	}

	memset(&s_FingerData, 0x00, sizeof(tr_cas_finger_data_st));
	memset(&s_IppData, 0x00, sizeof(CAS_IPPNOTIFY_STRUCT));
	tr_cas_init_osd_roll_data();
	
//	app_tr_api_smart_init();
	app_porting_ca_flash_init(CAS_FLASH_DATA_SIZE);
	app_porting_ca_demux_init();
	app_porting_ca_descramble_init();
	app_tr_cas_rolling_message_init();
	app_tr_cas_api_init_email_data();
	app_tr_cas_announce_init();
	app_porting_ca_create_filter_task();
	
	app_tr_register_popMsg(app_show_prompt);
	app_tr_register_hideMsg(app_hide_prompt);
	app_tr_register_parent_lock();
	app_tr_register_ipp_indicate();
	app_tr_register_ipp_buy(app_init_ipp_buy);
	app_tr_register_feed_card(app_init_feed_card);
	
	ret = MC_CoreInit(0);
	if (ret)
	{
		printf("[app_tr_cas_api_init]MC_CoreInit error!!!\n");
	}
	
	MC_STATE_RegisterCaNotify(TRCAS_Notify);
	app_porting_ca_smart_check_status();
	app_porting_ca_smart_create_task();
	
	strVer = (char *)MC_GetRevisionString();
	printf("[app_tr_cas_api_init]TRCas_version(%s).\n", strVer);
	s_bCaModeInited = TRUE;
	
	/*monitor entitle reminder.*/
	app_trcas_porting_monitor_init();
	app_tr_cas_api_search_regioncode();
		
	return TRUE;
}

/*
*  获取对应数据之前，初始化数据管理
(一般创建对应窗体之前或create中调用)。
* 如初始化邮件、授权等
*/
int32_t app_tr_cas_api_init_data(dvb_ca_data_type_t date_type)
{
	int32_t dvb_ca_flag =  app_flash_get_config_dvb_ca_flag();
	
	if (dvb_ca_flag != DVB_CA_TYPE_TR)
	{
		return FALSE;
	}

	switch (date_type)
	{
		case DVB_CA_EMAIL_INFO:
			return app_tr_cas_api_init_email_data();
			break;
			
		case DVB_CA_ENTITLE_INFO:
			return app_tr_cas_api_init_entitle_data();
			break;
			
		case DVB_CA_IPPV_PROG_INFO:
			return app_tr_cas_api_init_ippv_info_data();
			break;
			
		case DVB_CA_IPPV_SLOT_INFO:
			return app_tr_cas_api_init_ippv_slot_data();
			break;
			
		case DVB_CA_RATING_INFO:
			return app_tr_cas_api_init_rating_data();
			break;
			
		case DVB_CA_DETITLE_INFO:
			return app_tr_cas_api_init_ippv_bill_data();
			break;
			
		default:
			break;	
	}

	return FALSE;		
}

char* app_tr_cas_api_get_data(ca_get_date_t *data)
{
	int32_t dvb_ca_flag =  app_flash_get_config_dvb_ca_flag();
	
	if (dvb_ca_flag != DVB_CA_TYPE_TR)
	{
		return NULL;
	}

	if (NULL == data)
	{
		return NULL;	
	}

	switch (data->date_type)
	{
		case DVB_CA_BASE_INFO: 
			return (char *)app_tr_cas_api_get_baseinfo_data(data);
			break;
			
		case DVB_CA_EMAIL_INFO:
			return (char *)app_tr_cas_api_get_email_data(data);
			break;
			
		case DVB_CA_ENTITLE_INFO:
			return (char *)app_tr_cas_api_get_entitle_data(data);
			break;
			
		case DVB_CA_IPPV_PROG_INFO:
			return (char *)app_tr_cas_api_get_ippv_info_data(data);
			break;
			
		case DVB_CA_IPPV_SLOT_INFO:
			return (char *)app_tr_cas_api_get_ippv_slot_data(data);
			break;
			
		case DVB_CA_RATING_INFO:
			return (char *)app_tr_cas_api_get_rating_data(data);
			break;
			
		case DVB_CA_DETITLE_INFO:
			return (char *)app_tr_cas_api_get_ippv_bill_data(data);
			break;

		case DVB_CA_CURTAIN_INFO:/*wangjian add on 20150326*/
			return (char *)app_tr_cas_api_get_announce_data(data);
			break;	
			
		default:
			break;	
	}
	
	return NULL;	
}

uint8_t app_tr_cas_api_get_count(ca_get_count_t *data)
{
	int32_t dvb_ca_flag =  app_flash_get_config_dvb_ca_flag();
	
	if (dvb_ca_flag != DVB_CA_TYPE_TR)
	{
		return 0;
	}

	if (NULL == data)
	{
		return 0;
	}
	
	switch (data->date_type)
	{
		case DVB_CA_EMAIL_INFO:
			return app_tr_cas_api_get_email_count(data);
			break;
			
		case DVB_CA_ENTITLE_INFO:
			return app_tr_cas_api_get_entitle_count(data);
			break;
			
		case DVB_CA_IPPV_PROG_INFO:
			return app_tr_cas_api_get_ippv_info_count(data);				
			break;
			
		case DVB_CA_IPPV_SLOT_INFO:
			return app_tr_cas_api_get_ippv_slot_count(data);
			break;
			
		case DVB_CA_DETITLE_INFO:
			return app_tr_cas_api_get_ippv_bill_count(data);
			break;
			
		case DVB_CA_RATING_INFO:
			break;
			
		case DVB_CA_PAIRED_INFO:
			break;

		case DVB_CA_CURTAIN_INFO:/*wangjian add on 20150326*/
			return app_tr_cas_api_get_announce_count(data);
			break;	
			
		default:
			break;	
	}
	
	return 0;		
}

uint8_t app_tr_cas_api_is_ca_pop_msg_type(uint8_t type)
{
	if ((type >= (MSG_POP_COMMON_END+CAS_STATE_E00+1)) 
		&& (type <= (MSG_POP_COMMON_END+CAS_STATE_INFO_MAX+1)))
	{
		return TRUE;
	}
	
	return FALSE;
}

int32_t app_tr_cas_api_pop_msg_exec(uint8_t type)
{
	switch (type-MSG_POP_COMMON_END-1)
	{
		case CAS_STATE_E00:
			if (tr_hide_prompt != NULL)
			{
				tr_hide_prompt();
			}
			break;
			
		case CAS_STATE_E01:
			if (tr_show_prompt != NULL)
			{
				tr_show_prompt("E01:Please insert CA module");
			}
			break;
			
		case CAS_STATE_E02:
			if (tr_show_prompt != NULL)
			{
				tr_show_prompt("E02:Unknown CA module");
			}
			break;	
			
		case CAS_STATE_E03:
			if (tr_show_prompt != NULL)
			{
				tr_show_prompt("E03:CA module failure");
			}
			break;	
			
		case CAS_STATE_E04:
			if (tr_show_prompt != NULL)
			{
				tr_show_prompt("E04:Please insert smart card");
			}
			break;	
			
		case CAS_STATE_E05:
			if (tr_show_prompt != NULL)
			{
				tr_show_prompt("E05:Unknown smart card");
			}
			break;	
			
		case CAS_STATE_E06:
			if (tr_show_prompt != NULL)
			{
				tr_show_prompt("E06:Smart card failure");
			}
			break;
			
		case CAS_STATE_E07:
			if (tr_show_prompt != NULL)
			{
				tr_show_prompt("E07:Please insert card correctly");
			}
			break;	
			
		case CAS_STATE_E08:
			if (tr_show_prompt != NULL)
			{
				tr_show_prompt("E08:CA module EEPROM failure");
			}
			break;
			
		case CAS_STATE_E09:
			if (tr_show_prompt != NULL)
			{
				tr_show_prompt("E09:Smart card EEPROM failure");
			}
			break;	
			
		case CAS_STATE_E10:
			break;	
			
		case CAS_STATE_E11:
			if (tr_show_prompt != NULL)
			{
				tr_show_prompt("E11:Card marriage unmatched");
			}
			break;	
			
		case CAS_STATE_E12:
			if (tr_hide_prompt != NULL)
			{
				tr_show_prompt("E12:Please feed smart card");
			}
			break;	
			
		case CAS_STATE_E13:
			if (tr_show_prompt != NULL)
			{
				tr_show_prompt("E13:Dual communication error");
			}
			break;	
			
		case CAS_STATE_E14:
			if (tr_show_prompt != NULL)
			{
				tr_show_prompt("E14:No authorization");
			}
			break;	
			
		case CAS_STATE_E15:
			if (tr_show_prompt != NULL)
			{
				tr_show_prompt("E15:Valid authorization available");
			}
			break;	
			
		case CAS_STATE_E16:
		case CAS_STATE_E17:
			if (tr_show_prompt != NULL)
			{
				tr_show_prompt("E16:Service is currently scrambled");
			}
			break;	
			
		case CAS_STATE_E18:
			break;	
			
		case CAS_STATE_E19:
			break;	
			
		case CAS_STATE_E20:
			if (tr_show_prompt != NULL)
			{
				tr_show_prompt("E20:Not allowed in this broadcaster");
			}
			break;	
			
		case CAS_STATE_E21:
			if (tr_show_prompt != NULL)
			{
				tr_show_prompt("E21:PMT format error");
			}
			break;	
			
		case CAS_STATE_E22:

			break;	
			
		case CAS_STATE_E23:
			if (tr_show_prompt != NULL)
			{
				tr_show_prompt("E23:Service is currently descrambled");
			}
			break;	
			
		case CAS_STATE_E24:
			if (tr_show_prompt != NULL)
			{
				tr_show_prompt("E24:Not allowed in this region");
			}
			break;	
			
		case CAS_STATE_E25:
			break;	
			
		case CAS_STATE_E26:
			if (tr_show_prompt != NULL)
			{
				tr_show_prompt("E26:Unknown service");
			}
			break;	
			
		case CAS_STATE_E27:
			if (tr_show_prompt != NULL)
			{
				tr_show_prompt("E27:Service is not currently running");
			}
			break;	
			
		case CAS_STATE_E28:
			if (tr_show_prompt != NULL)
			{
				tr_show_prompt("E28:Smart card is locked");
			}
			break;	
			
		case CAS_STATE_E29:
			break;	
			
		case CAS_STATE_E30:
			if (tr_show_prompt != NULL)
			{
				tr_show_prompt("E30:Smart card not in working period ");
			}
			break;	
			
		case CAS_STATE_E31:
			if (tr_show_prompt != NULL)
			{
				tr_show_prompt("E31:Parental control locked ");
			}
			break;
			
		case CAS_STATE_E32:
			if (tr_show_prompt != NULL)
			{
				tr_show_prompt("E32:Not allowed in this country ");
			}
			break;	
			
		case CAS_STATE_E33:
			if (tr_show_prompt != NULL)
			{
				tr_show_prompt("E33:No authorization data ");
			}
			break;	
			
		case CAS_STATE_E34:
			if (tr_show_prompt != NULL)
			{
				tr_show_prompt("E34:Illegal box ");
			}
			break;	
			
		case CAS_STATE_E35:
			break;	
			
		case CAS_STATE_E36:
			break;	
			
		default:
			printf("\n app_tr_cas_api_pop_msg_exec default msg#\n");
			break;	
	}
	
	return 0;
}

static int32_t app_tr_cas_gxmsg_ca_on_event_IPP(GxMsgProperty0_OnEvent* event)
{
	CAS_IPPNOTIFY_STRUCT *ipp_data = NULL;
	
	if (NULL == event)
	{
		return 0;
	}

	ipp_data = (CAS_IPPNOTIFY_STRUCT *)(event->buf);
	if (NULL == ipp_data)
	{
		return 0;
	}

	if (s_IPPVFlag != 1)
	{
		return 0;
	}
	
	if (tr_show_ipp_buy != NULL)
	{
		tr_show_ipp_buy((void *)ipp_data);
	}
	
	s_IPPVFlag = 0;
	return 0;
}

int32_t app_tr_cas_gxmsg_ca_on_event_exec(GxMessage* msg)
{
	switch (msg->msg_id)
	{
		case GXMSG_CA_ON_EVENT:
		{
			GxMsgProperty0_OnEvent *event = NULL;
			event = GxBus_GetMsgPropertyPtr(msg, GxMsgProperty0_OnEvent);
			switch (event->key) 
			{
				case DVB_CA_EMAIL_INFO:
					{
						app_tr_cas_gxmsg_ca_on_event_email(event);
					}
					break;
				
				case DVB_CA_ROLLING_INFO:
					{
						app_tr_cas_gxmsg_ca_on_event_rolling(event);
					}
					break;
					
				case DVB_CA_FINGER_INFO:
					{
						app_tr_cas_gxmsg_ca_on_event_finger(event);
					}
					break;
					
				case DVB_CA_IPPV_POP_INFO:
					{
						app_tr_cas_gxmsg_ca_on_event_IPP(event);
					}
					break;

				case DVB_CA_RATING_INFO:
					if (tr_parent_lock != NULL)
					{
						tr_parent_lock(0);
					}
					break;

				case DVB_CA_MOTHER_CARD_INFO:
					if (tr_show_feed_card != NULL)
					{
						tr_show_feed_card();
					}
					break;	
				
				case DVB_CA_CURTAIN_INFO:/*wangjian add on 20150326*/
				{
					app_tr_cas_gxmsg_ca_on_event_announce(event);
				}
					break;
				
				default:
					break;
				}
			}
		
		default:
			break;
	}
	
	return 0;
}

void app_tr_cas_ipp_exec(void)
{	
	uint8_t result = 0;
	
	if (tr_ipp_indicate != NULL)
	{
		if (s_ippMarkNeedClear)
		{
			result = tr_ipp_indicate(0, FALSE);
			if (0 == result)
			{
				s_ippMarkNeedClear = FALSE;
			}
		}

		if (s_ippMarkNeedShow)
		{
			result = tr_ipp_indicate(s_IppData.bIppType, TRUE);
			if (0 == result)
			{
				s_ippMarkNeedShow = FALSE;
			}
		}
	}
	
	if (s_IPPVFlag != 1)
	{
		return;
	}
	
	if (tr_show_ipp_buy != NULL)
	{
		tr_show_ipp_buy((void *)&s_IppData);
	}
	
	s_IPPVFlag = 0;
}

int32_t app_tr_cas_api_osd_exec(void)
{
	app_tr_cas_announce_exec();
	app_tr_cas_rolling_message_exec();
	app_tr_cas_email_flag_exec();

	app_tr_cas_finger_exec();	
	app_tr_cas_ipp_exec();
	return 1;
}

bool app_tr_cas_api_delete(ca_delete_data_t *data)
{
	int32_t dvb_ca_flag = app_flash_get_config_dvb_ca_flag();
	
	if (dvb_ca_flag != DVB_CA_TYPE_TR)
	{
		return FALSE;
	}

	if (NULL == data)
	{
		return FALSE;
	}
	
	switch (data->date_type)
	{
		case DVB_CA_EMAIL_INFO:
			return app_tr_cas_api_delete_email(data);
			
		case DVB_CA_CURTAIN_INFO:/*wangjian add on 20150326*/
			return app_tr_cas_api_delete_announce(data);	
			
		default:
			break;	
	}
	
	return 0;	
}

bool app_tr_cas_api_delete_all(ca_delete_data_t *data)
{
	int32_t dvb_ca_flag =  app_flash_get_config_dvb_ca_flag();
	
	if (dvb_ca_flag != DVB_CA_TYPE_TR)
	{
		return FALSE;
	}

	if (NULL == data)
	{
		return FALSE;
	}
	
	switch (data->date_type)
	{
		case DVB_CA_EMAIL_INFO:
			return app_tr_cas_api_delete_all_email(data);

		case DVB_CA_CURTAIN_INFO:/*wangjian add on 20150326*/
			return app_tr_cas_api_delete_all_announce(data);	
			
		default:
			break;	
	}
	
	return 0;		
}

char* app_tr_cas_api_feed_mother_child_card(ca_mother_child_card_t *data)
{
	uint32_t pos ;
	uint32_t  state; /*0: init 1: read feed data 2: write feed data */
	static char* sstr = NULL;
	
	unsigned short wRet = 0;
	uint32_t Retvalue = 0;
	bool   bIsCardIn = TRUE;
	GxSmcTimeParams TimeParamsBefore = 
	{
		SMCT_ALL, RUNTIME_BAUD_RATE, RUNTIME_ETU, 
		100*372,0, 3600*372, 10*9600*372
	};
	
	GxSmcTimeParams TimeParamsEnd = 
	{
		SMCT_ALL, RUNTIME_BAUD_RATE, RUNTIME_ETU, 
		RUNTIME_EGT, RUNTIME_TGT, RUNTIME_WDT, RUNTIME_TWDT
	};

	if (NULL == data)
	{
		return NULL;
	}
	
	pos = data->pos;
	state = data->state;
	switch (state)
	{
		case 0: // init 
		bIsCardIn = app_porting_ca_smart_status();
		memset(chSpbyFeedData, 0x00, 256);
		chLen = 0;
		
		if (bIsCardIn == TRUE)
		{
			app_porting_ca_smart_config_time(TimeParamsBefore);
			wRet = MC_ReadFeedDataFromMaster(chSpbyFeedData, &chLen);
			//printf("\nread seed return value = %d\n",wRet);
			if (MC_OK == wRet)
			{
				sstr = "Please insert the child-card";
			}
			else
			{
				sstr = "Invalid parent-card";
			}
			
			app_porting_ca_smart_config_time(TimeParamsEnd);
			return sstr;
		}
		else
		{	
			sstr = "Please insert the parent-card";
			app_porting_ca_smart_config_time(TimeParamsEnd);
			return sstr;
		}
		break;
		
		case 1:
		app_porting_ca_smart_config_time(TimeParamsBefore);
		wRet = MC_ReadFeedDataFromMaster(chSpbyFeedData, &chLen);
		//printf("\n111read seed return value = %d\n",wRet);
		if (MC_OK == wRet)
		{
			app_porting_ca_smart_config_time(TimeParamsEnd);
			sstr = "Please insert the child-card";
			return sstr;
		}
		else
		{
			app_porting_ca_smart_config_time(TimeParamsEnd);
			sstr = "Invalid parent-card";
			return sstr;
		}
		break;
		
		case 2:
		app_porting_ca_smart_config_time(TimeParamsBefore);
		Retvalue = MC_WriteFeedDataToSlaver(chSpbyFeedData, chLen);
		if (MC_OK == Retvalue)
		{
			app_porting_ca_smart_config_time(TimeParamsEnd);
			sstr = "Feeding successfully";
			return sstr;					
		}
		else 
		{
			app_porting_ca_smart_config_time(TimeParamsEnd);
			sstr = "Feeding failed";
			return sstr;					
		}
		break;
		
		default:
		break;
	}

	return sstr;
}

uint8_t app_tr_cas_api_buy_ppv_data(ca_buy_ipp_t *data)
{
	S16 wRet = 0;
	CAS_IPPPURCHASE_STRUCT stippBuy = {0,};
	
	if (data == NULL)
	{
		return 0;
	}
	
	memcpy(&stippBuy, data->buydata, sizeof(CAS_IPPPURCHASE_STRUCT));
	wRet = MC_IppPurchase((U8*)(data->pin), 6, stippBuy);
	return wRet;
}

uint8_t app_tr_cas_api_buy_ipp(ca_buy_ipp_t *data)
{
    int32_t dvb_ca_flag =  app_flash_get_config_dvb_ca_flag();
	
	if (dvb_ca_flag != DVB_CA_TYPE_TR)
	{
		return 0;
	}

	if (NULL == data)
	{
		return 0;	
	}

	switch (data->date_type)
	{
	    case DVB_CA_IPPV_POP_INFO:
	         return app_tr_cas_api_buy_ppv_data(data);
			 
	    default:
			break;	
	}
    
    return 1;
}

APP_CAS_API_ControlBlock_t TRCasPortControlBlock =
{
	.m_cas_init_callback = app_tr_cas_api_init,
	.m_cas_close_callback = NULL,
	.m_cas_api_card_in_callback = app_tr_cas_api_card_in,
	.m_cas_api_card_out_callback = app_tr_cas_api_card_out,
	.m_cas_api_release_ecm_filter_callback = app_tr_cas_api_release_ecm_filter,
	.m_cas_api_start_ecm_filter_callback = app_tr_cas_api_start_ecm_filter,
	.m_cas_api_release_emm_filter_callback = app_tr_cas_api_release_emm_filter,
	.m_cas_api_start_emm_filter_callback = app_tr_cas_api_start_emm_filter,
	.m_cas_api_check_cat_ca_descriptor_valid_callback = app_tr_cas_api_check_cat_ca_descriptor_valid,
	.m_cas_api_check_pmt_ca_descriptor_valid_callback = app_tr_cas_api_check_pmt_ca_descriptor_valid,
	.m_cas_api_is_ca_pop_msg_type_callback = app_tr_cas_api_is_ca_pop_msg_type,
	.m_cas_api_pop_msg_exec_callback = app_tr_cas_api_pop_msg_exec,
	.m_cas_api_osd_exec_callback = app_tr_cas_api_osd_exec,
	.m_cas_api_gxmsg_ca_on_event_exec_callback = app_tr_cas_gxmsg_ca_on_event_exec, 								
	.m_cas_api_init_data_callback = app_tr_cas_api_init_data,
	.m_cas_api_get_data_callback = app_tr_cas_api_get_data,
	.m_cas_api_get_count_callback = app_tr_cas_api_get_count,
	.m_cas_api_delete_callback = app_tr_cas_api_delete,
	.m_cas_api_delete_all_callback = app_tr_cas_api_delete_all,
	.m_cas_api_buy_ipp_callback = app_tr_cas_api_buy_ipp,
	.m_cas_api_get_lock_status_callback = app_tr_cas_api_get_lock_status,
	.m_cas_api_feed_mother_child_card_callback = app_tr_cas_api_feed_mother_child_card,
	.m_cas_api_change_pin_callback = app_tr_cas_api_change_pin,
	.m_cas_api_verify_pin_callback = NULL,
	.m_cas_api_change_rating_callback = app_tr_cas_api_change_rating,
	.m_cas_api_change_worktime_callback = NULL,
};


