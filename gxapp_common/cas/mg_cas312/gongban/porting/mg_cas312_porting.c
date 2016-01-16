/*****************************************************************************
* 						   CONFIDENTIAL
*        Hangzhou GuoXin Science and Technology Co., Ltd.
*                      (C)2014, All right reserved
******************************************************************************

******************************************************************************
* File Name :	mg_cas312_porting.c
* Author    : 	wangjian
* Project   :	runde 3113c
* Type      :
******************************************************************************
* Purpose   :	mg cas porting
******************************************************************************
* Release History:
VERSION	Date			  AUTHOR         Description
1.0  	2014.04.11		  zhouhm 	    creation
2.0  	2014.12.16		  wangjian 	    modify for v3.1.2
*****************************************************************************/
#ifndef __MG_CAS312_PORTING_C__
#define __MG_CAS312_PORTING_C__

#include "mg312def.h"
#include "app_common_porting_ca_flash.h"
#include "app_common_porting_ca_demux.h"
#include "app_common_porting_ca_os.h"
#include "app_common_porting_ca_smart.h"
#include "app_common_porting_ca_descramble.h"
#include "app_common_play.h"
#include "app_mg_cas312_api_demux.h"
#include "app_mg_cas312_api_pop.h"
#include "app_mg_cas312_api_event.h"
#include "app_common_porting_stb_api.h"
#include "app_mg_cas312_api_smart.h"
#include "app_mg_cas312_api_email.h"
#include "gxplayer_url.h"
#include "app_mg_cas312_api_lock_service.h"


/*Marcos define -------------------------------------------------------*/
#define RUNTIME_ETU                         (372)
#define RUNTIME_BAUD_RATE                   (9600*RUNTIME_ETU)

#if (RC_VERSION>=GOXCEED_v1_9_4)
#define RUNTIME_EGT                         (RUNTIME_ETU * 2)
#define RUNTIME_WDT                         (15 * RUNTIME_ETU)
#else
#define RUNTIME_EGT                         (RUNTIME_ETU * 20)
#define RUNTIME_WDT                         (45 * RUNTIME_ETU)
#endif

#define RUNTIME_TGT                         (0)
#define RUNTIME_TWDT                        (3*9600 * RUNTIME_ETU)


#define MGCAS312_STB_SEARIL_DV 		("0000000000")
#define MGCAS312_STB_SEARIL   		("STBSerail")
#define MGCAS312_SERIAL_LEN  		(10)

#define CW_LEN_MAX (8)

/*Extern data -------------------------------------------------------*/
extern uint8_t zonecode;
extern mg_cas312_rolling_message_st g_rollingMsg[];

/*Local data--------------------------------------------*/
static uint8_t s_caReply[1024] = {0};
static mg312_cas_email_st s_emailFlag = {0};

static handle_t s_semaMgcas312 = 0;

static MG_U16 s_counterU = 0xffff;
static MG_U16 s_counterG = 0xffff;
static MG_U16 s_counterA = 0xffff;


/*API -----------------------------------------------------------------*/
MG_STATUS MG_STBPort_Send_Command(MG_U8 *MG_SendComm, MG_U8 MG_CommLen, 
       							MG_U8 *MG_RecvResp, MG_U16 *SW, MG_U8 *NumberRead)
{
	uint8_t sc_result;
	uint16_t reclen = 0;
	
#ifdef _DEBUG
	CAS_DUMP("[MG CAS] Write:", MG_SendComm, MG_CommLen);
#endif

	sc_result = app_porting_ca_smart_sc_apdu61(MG_SendComm, MG_CommLen, s_caReply, &reclen);

	if (TRUE == sc_result)
	{
		if (reclen >= 2)
		{
#ifdef _DEBUG
			CAS_DUMP("[MG CAS] Read:", s_caReply, reclen);
#endif
			 *NumberRead = reclen-2;
			 memcpy(MG_RecvResp, s_caReply, *NumberRead);
			 *SW = ((s_caReply[reclen-2] << 8) + s_caReply[reclen-1]) & 0xffff;
			 return MG_SUCCESS; 		
		}
		else
		{
			*NumberRead = 0;
			*SW = 0;
			return (MG_FAILING);
		}			
	}
	else
	{
		*NumberRead = 0;
		*SW = 0;
		printf("##MG_STBPort_Send_Command failed!!!\n");
		return (MG_FAILING);			
	}
}

MG_STATUS MG_STBPort_Create_Semaphore(void)
{
	app_porting_ca_os_sem_create(&s_semaMgcas312, 1);
	return MG_SUCCESS;
}

MG_STATUS MG_STBPort_Obtain_Semaphore(void)
{
	app_porting_ca_os_sem_wait(s_semaMgcas312);
	return MG_SUCCESS;
}

MG_STATUS MG_STBPort_Release_Semaphore(void)
{
	app_porting_ca_os_sem_signal(s_semaMgcas312);
	return MG_SUCCESS;
}

MG_STATUS MG_STBPort_Delete_Semaphore(void)
{
	app_porting_ca_os_sem_delete(s_semaMgcas312);
	s_semaMgcas312 = 0;
	
	return MG_SUCCESS;
}

MG_STATUS MG_STBPort_Reset_Card(MG_U8 *MG_RespData)
{
	MG_STATUS retVal = MG_SUCCESS;
	uint8_t errCode = 0;
	GxSmcTimeParams smcTime;
	uint8_t atrLen = 0;

	if (NULL == MG_RespData)
	{
		printf("[MG_STBPort_Reset_Card]MG_RespData is NULL!!!\n");
		return MG_FAILING;
	}

	// TODO: if get ATR fail, then retry again!!!
	smcTime.baud_rate  = RUNTIME_BAUD_RATE;
	smcTime.egt        = RUNTIME_EGT;
	smcTime.etu        = RUNTIME_ETU;
	smcTime.tgt        = RUNTIME_TGT;
	smcTime.twdt       = RUNTIME_TWDT;
	smcTime.wdt        = RUNTIME_WDT;

	errCode = app_porting_ca_smart_reset(smcTime, MG_RespData, &atrLen);
	if (!errCode)
	{
		printf("[MG_STBPort_Reset_Card]app_porting_ca_smart_reset fail!!!\n");
		retVal = MG_FAILING;
	}
	else
	{
//#ifdef _DEBUG
		CAS_DUMP("[MGCAS312]ATR:", MG_RespData, atrLen);
//#endif
	}

	return retVal;
}

MG_U32 MG_STBPort_Get_OSTime(MG_VOID)
{
	GxTime time;
	
	GxCore_GetLocalTime(&time);
	return (time.seconds*100);
}

MG_U32 MG_STBPort_Add_OSTime(MG_U32 time1,MG_U32 time2)
{
	return (MG_U32)(time1+time2);
}

MG_U32 MG_STBPort_Minus_OSTime(MG_U32 time1,MG_U32 time2)
{
	return (MG_U32)(time1-time2);
}

MG_U32 MG_STBPort_Get_OSTime_Ticks(MG_VOID)
{
	return (MG_U32)(100);
}

/*-----------------------------------------------------------------*/
char*  MGCA312_Get_Config_Serial(void)
{
	 static char serial[MGCAS312_SERIAL_LEN+1]={0};

	 memset(serial, 0, MGCAS312_SERIAL_LEN+1);
	 GxBus_ConfigGet(MGCAS312_STB_SEARIL, serial,\
	 	  			 MGCAS312_SERIAL_LEN,\
	 	  			 MGCAS312_STB_SEARIL_DV);
	 return serial;
}

int32_t MGCA312_Save_Config_Serial(const char* mgca_serial)
{
	if (NULL == mgca_serial)
	{
		return 0;
	}

	GxBus_ConfigSet(MGCAS312_STB_SEARIL, mgca_serial);
	return 0;
}

void MGCA312_ShowBugMessage(MG_S16 messageType)
{
	MG_U8  byMessageType = 0xff;

	switch (messageType)
	{
		case MG_SUCCESS:
			byMessageType = CA_MG_SUCCESS;
			break;
			
		case MG_ERR_SCARD_NOT_ACT:
			byMessageType = CA_MG_ERR_SCARD_NOT_ACT;
			break;
			
		case MG_ERR_ZONE_CODE:
			byMessageType = CA_MG_ERR_ZONE_CODE;
			break;
			
		case MG_ERR_NO_MONEY:
			byMessageType = CA_MG_ERR_NO_MONEY;
			break;
			
		case MG_ERR_NOT_AUTHORED:
			byMessageType = CA_MG_ERR_NOT_AUTHORED;
			break;
			
		case MG_ERR_CARD_LOCKED:
			byMessageType = CA_MG_ERR_CARD_LOCKED;
			break;
			
		case MG_ERR_APP_LOCKED:
			byMessageType = CA_MG_ERR_APP_LOCKED;
			break;
			
		case MG_ERR_PIN_AUTHEN:
			byMessageType = CA_MG_ERR_PIN_AUTHEN;
			break;
			
		case MG_ERR_EXTER_AUTHEN:
			byMessageType = CA_MG_ERR_EXTER_AUTHEN;
			break;
			
		case MG_ERR_DONOT_TELEVIEW_PPV:	
			byMessageType = CA_MG_ERR_DONOT_TELEVIEW_PPV;
			break;
			
		case MG_ERR_CARD_NOTFOUND: 
			byMessageType = CA_MG_ERR_CARD_NOTFOUND;
			break;
			
		case MG_ERR_CARD_NOTMATCH: 
			byMessageType = CA_MG_ERR_CARD_NOTMATCH;
			break;
			
		case MG_ERR_UPDATE_GK: 
			byMessageType = CA_MG_ERR_UPDATE_GK;
			break;

		case MG_EMM_APP_STOP:
			byMessageType = CA_MG_EMM_APP_STOP;
			break;
			
		case MG_EMM_APP_RESUME:
			byMessageType = CA_MG_EMM_APP_RESUME;
			break;
			
		case MG_EMM_PROG_UNAUTH:
			byMessageType = CA_MG_EMM_PROG_UNAUTH;
			break;
			
		case MG_EMM_PROG_AUTH:
			byMessageType = CA_MG_EMM_PROG_AUTH;
			break;
			
		case MG_EMM_EN_PARTNER:
		case MG_EMM_DIS_PARTNER:
		case MG_EMM_SONCARD:
			if(messageType == MG_EMM_EN_PARTNER)
			{
				byMessageType = CA_MG_EMM_EN_PARTNER;
			}
			else if (messageType == MG_EMM_DIS_PARTNER)
			{
				byMessageType = CA_MG_EMM_DIS_PARTNER;
			}
			else /* MG_EMM_SONCARD */
			{
				byMessageType = CA_MG_EMM_SONCARD;
			}
			break;
			
		case MG_EMM_UPDATE_GNO:
			byMessageType = CA_MG_EMM_UPDATE_GNO;
			break;	
			
		case MG_EMM_UPDATE_GK:
			byMessageType = CA_MG_EMM_UPDATE_GK;
			break;

		case MG_EMM_CHARGE:
			byMessageType = CA_MG_EMM_CHARGE;
			break;
			
		case MG_EMM_URGENT_SERVICE:
			byMessageType = CA_MG_EMM_URGENT_SERVICE;
			break;
			
		case MG_EMM_MODIFY_ZONECODE:
			byMessageType = CA_MG_EMM_MODIFY_ZONECODE;
			break;

		case MG_EMM_UPDATE_COUT:
			byMessageType = CA_MG_EMM_UPDATE_COUT;
			break;	
			
		case MG_EMM_UPDATE_CRT:
			byMessageType = CA_MG_EMM_UPDATE_CRT;
			break;
			
		default:
			break;
	}

	if (0xff != byMessageType)
	{
		if ((MSG_POP_COMMON_END+byMessageType+1) >= MESSAGE_MAX_COUNT)
		{
			return;
		}			
	}
	
	if (TRUE != app_play_get_msg_pop_type_state(MSG_POP_COMMON_END+byMessageType+1))
	{
		app_play_clear_ca_msg();
		app_play_set_msg_pop_type(MSG_POP_COMMON_END+byMessageType+1);

#ifdef _DEBUG
     	printf("[MGCA312_ShowBugMessage]pop msgid:%d.\n", MSG_POP_COMMON_END+byMessageType+1);
#endif
	}

	return;
}

/*++
功能：显示新邮件的标志。
参数：
	byShow:					标志类型。含义如下：
	MGCAS312_NEW_EAMIL						新邮件通知。
	MGCAS312_NEW_EMAIL_NO_ROOM		有新邮件，但Email空间不够。
	MGCAS312_EMAIL_NONE					没有新邮件。
*/
void MGCA312_toEmailNotify(uint8_t byShow)
{
	if ((MGCAS312_EMAIL_NONE == byShow) 
		&& (DVB_CA_EMAIL_FLAG_HIDE == s_emailFlag.wStatus))
	{
		return;
	}
	
	switch (byShow)
	{
		case MGCAS312_EMAIL_NONE:
			s_emailFlag.wStatus = DVB_CA_EMAIL_FLAG_HIDE;
			break;
			
		case MGCAS312_NEW_EAMIL:
			s_emailFlag.wStatus = DVB_CA_EMAIL_NEW;
			break;
			
		case MGCAS312_NEW_EMAIL_NO_ROOM:
			s_emailFlag.wStatus = DVB_CA_EMAIL_EXHAUST;				
			break;
			
		default:
			break;
	}

    app_cas_api_on_event(DVB_CA_EMAIL_INFO, CAS_NAME,\
		    		&s_emailFlag, sizeof(s_emailFlag));
	return;
}

static void MGCA312_toShowFinger(int OnOff, char *msg)
{
    mg_ca312_finger_data_st fingerdata;
	
    memset(&fingerdata, 0, sizeof(mg_ca312_finger_data_st));
    
    if (TRUE == OnOff)
    {
        fingerdata.wStatus = TRUE;
        if (msg)
        {
            snprintf(fingerdata.msg, sizeof(fingerdata.msg)-1, "%s", msg);
        }
        
    }
    else
    {
        fingerdata.wStatus = FALSE;
    }
	
    app_cas_api_on_event(DVB_CA_FINGER_INFO, CAS_NAME, &fingerdata,\
		   					sizeof(mg_ca312_finger_data_st));
	return;
}

static void MGCA312_toShowPPVLogo(int time, char *msg)
{
	mg_ca312_ppv_data_st ppvdata;

	memset(&ppvdata, 0, sizeof(mg_ca312_ppv_data_st));
	ppvdata.time = time;
	strncpy(ppvdata.msg, msg, sizeof(ppvdata.msg)-1);
	app_cas_api_on_event(DVB_CA_IPPV_POP_INFO,CAS_NAME,\
		    		&ppvdata, sizeof(mg_ca312_ppv_data_st)); 
	return;
}


void MGCA312_ReleasePrivateDataFilter(MG_U8 byReqID, MG_U16 wPid)
{
	uint32_t filterId = 0;
	bool releaseChannel = TRUE;
	ca_filter_t sfilter = {0};

	for (filterId = 0; filterId < MAX_FILTER_COUNT; filterId++)
	{
		app_porting_ca_demux_get_by_filter_id(filterId, &sfilter);
		if ((0 != sfilter.usedStatus) 
			&& (0 != sfilter.handle) 
			&& (0 != sfilter.channelhandle)
			&& (wPid == sfilter.pid) 
			&& (byReqID == sfilter.byReqID))
		{
			if (0x10 == wPid)
			{
				releaseChannel = FALSE;
			}
			else
			{
				releaseChannel = TRUE;
			}
			
            if (EMM_TABLEID_U == byReqID)
            {
                s_counterU = 0;
            }

            if (EMM_TABLEID_G == byReqID)
            {
            	s_counterG = 0;
            }

            if (EMM_TABLEID_A == byReqID)
            {
                s_counterA = 0;
            }

            app_porting_ca_demux_release_filter(filterId, releaseChannel);
            return;
		}
	}

	return;
}

uint8_t MGCA312_SetPrivateDataFilter(MG_U8 byReqID, const MG_U8 *pbyFilter, 
	  										const MG_U8 *pbyMask, MG_U8 byLen, 
	  										MG_U16 wPid, MG_U8 byWaitSeconds)
{
	ca_filter_t filter = {0};
	bool repeatFlag = TRUE;
	uint8_t match[18] = {0,};
	uint8_t mask[18] = {0,};

	if ((pbyFilter == NULL) || (pbyMask == NULL))
	{
		return FALSE;
	}

	memcpy(match, pbyFilter, byLen);
	memcpy(mask, pbyMask, byLen);

	// DUMP(DEMUX,INFO,byLen,pbyFilter,"%x");
	// DUMP(DEMUX,INFO,byLen,pbyMask,"%x");
	MGCA312_ReleasePrivateDataFilter(byReqID, wPid);

	filter.byReqID = byReqID;
	filter.crcFlag = FALSE;
	filter.filterLen = byLen;
	memcpy(filter.match, match, byLen);
	memcpy(filter.mask, mask, byLen);
	filter.nWaitSeconds = byWaitSeconds;
	filter.pid = wPid;
	filter.equalFlag = TRUE;
	filter.repeatFlag = repeatFlag;
	filter.Callback = app_mg_cas312_filter_notify;
	filter.timeOutCallback = app_mg_cas312_filter_timeout_notify;
	return app_porting_ca_demux_start_filter(&filter);
}

uint8_t MGCA312_SetEcmFilter(MG_U16 wPid)
{
    uint8_t match[8] = {0};
    uint8_t mask[8] = {0};
	
	match[0] = ECM_TABLEID_80;
	mask[0] = 0xFE;
 	MGCA312_SetPrivateDataFilter(ECM_TABLEID_80, match,\
		  						 mask, 1, wPid, 0);	
	return 0;
}

void MGCA312_EcmDataGot(MG_U16 wEcmPID, MG_U8 *MG_ECM)
{
	MG_STATUS EcmResult;
	MG_U8 pCWUpd = 0;
	MG_U8 pBalUpd = 0;
	MG_U8 pOddCW[8], pEvenCW[8];
    static MG_U8 OldCw[8];
    static MG_U8 EvenCw[8];
    
	EcmResult = MG312_ECM_Process(MG_ECM, &pCWUpd, &pBalUpd, zonecode);
   
	if (pCWUpd)
	{
		if (pCWUpd & MG_CWUPD_BIT_ODD) /* Odd CW changed */
		{
			MG_Get_Odd_CW(pOddCW);
			memcpy(OldCw, pOddCW, CW_LEN_MAX);
			app_porting_ca_descramble_set_cw(wEcmPID, OldCw, EvenCw, CW_LEN_MAX, 0);
		}
		
		if (pCWUpd & MG_CWUPD_BIT_EVEN) /* Even CW changed */
		{
			MG_Get_Even_CW(pEvenCW);
			memcpy(EvenCw, pEvenCW, CW_LEN_MAX);
			app_porting_ca_descramble_set_cw(wEcmPID, OldCw, EvenCw, CW_LEN_MAX, 0);
		}

#ifdef _DEBUG
        CAS_DUMP("[OldCw] ->", OldCw, CW_LEN_MAX);
        CAS_DUMP("[EvenCw] ->", EvenCw, CW_LEN_MAX);
#endif
	}
    
    if (pBalUpd)
	{
		/* disp ppv logo at screen corner, delay 5 seconds then disappear */
		 printf("[MGCA312_EcmDataGot]ECM show 5s PPV LOGO#.\n");
	     MGCA312_toShowPPVLogo(5, "PPV");
	}
    
#ifdef _DEBUG
     printf("#[MGCA312_EcmDataGot]EcmResult(%d).#\n", EcmResult);
#endif
    
	/* checking ecm func result */
	if (EcmResult == MG_SUCCESS)
	{
		/* nothing should do */
		MGCA312_ShowBugMessage(MG_SUCCESS);
        /* only for dbg */
	}
	else if (EcmResult == MG_FAILING)
	{
		/* nothing should do */
         /* only for dbg */
	}
	else if ((EcmResult == MG_ERR_SCARD_NOT_ACT) 
		|| (EcmResult == MG_ERR_SCARD_DISABLED))
	{
		MGCA312_ShowBugMessage(CA_MG_ERR_SCARD_NOT_ACT);
	}
	else if (EcmResult == MG_ERR_ZONE_CODE)
	{
		MGCA312_ShowBugMessage(MG_ERR_ZONE_CODE);
	}
	else if (EcmResult == MG_ERR_NO_MONEY)
	{
		MGCA312_ShowBugMessage(MG_ERR_NO_MONEY);
	}
	else if (EcmResult == MG_ERR_NOT_AUTHORED)
	{
		MGCA312_ShowBugMessage(MG_ERR_NOT_AUTHORED);
	}
	else if (EcmResult == MG_ERR_CARD_LOCKED)
	{
		MGCA312_ShowBugMessage(MG_ERR_CARD_LOCKED);
	}
	else if (EcmResult == MG_ERR_APP_LOCKED)
	{
		MGCA312_ShowBugMessage(MG_ERR_APP_LOCKED);
	}
	else if (EcmResult == MG_ERR_PIN_AUTHEN)
	{
		MGCA312_ShowBugMessage(MG_ERR_PIN_AUTHEN);		
	}
	else if (EcmResult == MG_ERR_EXTER_AUTHEN)
	{
		MGCA312_ShowBugMessage(MG_ERR_EXTER_AUTHEN);
	}
	else if ((EcmResult == MG_ERR_QUESTION_TELEVIEW_PPV) 
		|| (EcmResult == MG_ERR_DONOT_TELEVIEW_PPV))
	{
		MGCA312_ShowBugMessage(MG_ERR_DONOT_TELEVIEW_PPV);
	}
	else if (EcmResult == MG_ERR_CARD_NOTFOUND)
	{
		MGCA312_ShowBugMessage(MG_ERR_CARD_NOTFOUND);
	}
	else if (EcmResult == MG_ERR_CARD_NOTMATCH)
	{
		MGCA312_ShowBugMessage(MG_ERR_CARD_NOTMATCH);
	}
	else if (EcmResult == MG_ERR_UPDATE_GK)
	{
		MGCA312_ShowBugMessage(MG_ERR_UPDATE_GK);
	}
	else
	{
		/* nothing should do */
 		//STB_DbgMsg("wrong msg %d",EcmResult); /* only for dbg */
	}	

	return;
}

//Emm过滤器设置
 void MGCA312_SetEmmFilterU(uint16_t emm_pid)
{
    uint8_t match[16];
    uint8_t cardSN[6];
    uint8_t mask[16];
    uint16_t uCounter;
    uint16_t i;

	//printf("[MGCA312_SetEmmFilterU]emm_pid(0x%x)+\n", emm_pid);
    memset(match, 0, sizeof(match));
    memset(mask, 0, sizeof(mask));

    /* set condition*/
    if (MG_Get_Card_Ready() == MG_TRUE)
    {
        uCounter = MG_Get_UCounter() + 1;
        if (s_counterU == uCounter)
        {
        	return;
        }
		
        match[0] = EMM_TABLEID_U;
        mask[0] = 0xff;
        match[3] = (uCounter >> 8) & 0xff;
        match[4] = uCounter & 0xff;
        mask[3] = 0xff;
        mask[4] = 0xff;

        MG_Get_Card_ID(cardSN);
        for (i = 5; i < 9; i++)
        {
            match[i] = cardSN[i-5];
            mask[i] = 0xff;
        }

 	    MGCA312_SetPrivateDataFilter(EMM_TABLEID_U, match, mask, 10, emm_pid, 20);
		s_counterU = uCounter;
		//printf("[MGCA312_SetEmmFilterU]uCounter(%d)+++\n", uCounter);
    }
	else
	{
		printf("[MGCA312_SetEmmFilterU]card is invalid!!!\n");
	}

	return;
}

void MGCA312_SetEmmFilterG(uint16_t emm_pid)
{
    uint8_t match[16] = {0};
    uint8_t mask[16] = {0};
    uint16_t uCounter;

	//printf("[MGCA312_SetEmmFilterG]emm_pid(0x%x)@\n", emm_pid);
    memset(match, 0, sizeof(match));
    memset(mask, 0, sizeof(mask));

    /* set condition*/
    if (MG_Get_Card_Ready() == MG_TRUE)
    {
        uCounter = MG_Get_GCounter() + 1;
        if (s_counterG == uCounter)
        {
        	return;
        }
		
        match[0] = EMM_TABLEID_G;
        mask[0] = 0xff;
        match[3] = (uCounter >> 8) & 0xff;
        match[4] = uCounter & 0xff;
        mask[3] = 0xff;
        mask[4] = 0xff;
		MGCA312_SetPrivateDataFilter(EMM_TABLEID_G, match, mask, 5, emm_pid, 20);
		s_counterG = uCounter;
		//printf("[MGCA312_SetEmmFilterG]uCounter(%d)+@+\n", uCounter);
    }
	else
	{
		printf("[MGCA312_SetEmmFilterG]card is invalid!!!\n");
	}

	return;
}

void MGCA312_SetEmmFilterA(uint16_t emm_pid)
{
    uint8_t match[16];
    uint8_t mask[16];
    uint16_t uCounter;

	//printf("[MGCA312_SetEmmFilterA]emm_pid(0x%x)#\n", emm_pid);
    memset(match, 0, sizeof(match));
    memset(mask, 0, sizeof(mask));

    /* set condition*/
    if (MG_Get_Card_Ready() == MG_TRUE)
    {
        uCounter =  MG_Get_ACounter() + 1;
        if (s_counterA == uCounter)
        {
        	return;
        }	

        match[0] = EMM_TABLEID_A;
        mask[0] = 0xff;

        match[3] = (uCounter >> 8) & 0xff;
        match[4] = uCounter & 0xff;
        mask[3] = 0xff;
        mask[4] = 0xff;
		MGCA312_SetPrivateDataFilter(EMM_TABLEID_A, match, mask, 5, emm_pid, 20);
		s_counterA = uCounter;  
		//printf("[MGCA312_SetEmmFilterA]uCounter(%d)+#+\n", uCounter);
    }
	else
	{
		printf("[MGCA312_SetEmmFilterA]card is invalid!!!\n");
	}

	return;
}

//EMM处理
void MGCA312_EmmDataGot(MG_U16 wEcmPID, MG_U8 *MG_EMM)
{
	MG_STATUS EmmResult;
	MG_U8 EmmFunc;
    MG_U8 STBSerial[11] = {0,};

	if ((EmmResult = MG312_EMM_C_Process(MG_EMM, &EmmFunc)) == MG_SUCCESS)
	{
        //printf("@[MGCA312_EmmDataGot]EmmResult(%d)@.\n", EmmResult);
		switch (EmmFunc)
		{
			case MG_EMM_APP_STOP:
				MGCA312_ShowBugMessage(MG_EMM_APP_STOP);
				break;
				
			case MG_EMM_APP_RESUME:
				MGCA312_ShowBugMessage(MG_EMM_APP_RESUME);
				break;
				
			case MG_EMM_PROG_UNAUTH:
				MGCA312_ShowBugMessage(MG_EMM_PROG_UNAUTH);
				break;
				
			case MG_EMM_PROG_AUTH:
				MGCA312_ShowBugMessage(MG_EMM_PROG_AUTH);
				break;

			case MG_EMM_UPDATE_GNO:
				MGCA312_ShowBugMessage(MG_EMM_UPDATE_GNO);
				break;	

			case MG_EMM_UPDATE_GK:
				MGCA312_ShowBugMessage(MG_EMM_UPDATE_GK);
				break;	

			case MG_EMM_MODIFY_ZONECODE:
				MGCA312_ShowBugMessage(MG_EMM_MODIFY_ZONECODE);
				break;	

			case MG_EMM_UPDATE_COUT:
				MGCA312_ShowBugMessage(MG_EMM_UPDATE_COUT);
				break;

			case MG_EMM_UPDATE_CRT:
				MGCA312_ShowBugMessage(MG_EMM_UPDATE_CRT);
				break;	
			
			case MG_EMM_EN_PARTNER:
			case MG_EMM_DIS_PARTNER:
			case MG_EMM_SONCARD:
				{
    				MG_U8 MG_PartnerSerials[11] = {0};
					
    				MG_Get_STB_Serial(MG_PartnerSerials);
                    memset(STBSerial, 0, 11);
                    memcpy(STBSerial, (char *)MG_PartnerSerials, 10);
					MGCA312_Save_Config_Serial((const char *)STBSerial);
    				
    				if (EmmFunc == MG_EMM_EN_PARTNER)//启用机卡配对	
    				{
    					MGCA312_ShowBugMessage(MG_EMM_EN_PARTNER);
					}
    				else if (EmmFunc == MG_EMM_DIS_PARTNER)//取消机卡配对
    				{
    					MGCA312_ShowBugMessage(MG_EMM_DIS_PARTNER);
					}
    				else /* MG_EMM_SONCARD */
    				{
    					MGCA312_ShowBugMessage(MG_EMM_SONCARD);
					}
				}
				break;

			case MG_EMM_CHARGE:
				{
					printf("+++[MGCA312_EmmDataGot]MG_EMM_CHARGE+++\n");
					MGCA312_ShowBugMessage(MG_EMM_CHARGE);
				}
				break;	

			case MG_EMM_URGENT_SERVICE:
				{
					MG_U8 urgentServID[2] = {0};
					uint16_t wServID = 0;

					MGCA312_ShowBugMessage(CA_MG_EMM_URGENT_SERVICE);
					MG_Get_ServiceID(urgentServID);
					wServID = (urgentServID[0] << 8) + urgentServID[1];
					printf("###[MGCA312_EmmDataGot]URGENT_SERVICE(0x%x)###\n", wServID);
					app_mg_cas312_api_lock_service(wServID);
				}
				break;		

			case MG_EMM_FIGEIDENT://指纹显示
				{
    				MG_U8 Card_SN[8];
                    char s_text_buffer[70] = {0};
					
    				MG_Get_Card_SN(Card_SN);
                    sprintf(s_text_buffer, "%02X%02X%02X%02X%02X%02X%02X%02X",\
						   	Card_SN[0], Card_SN[1], Card_SN[2], Card_SN[3],\
						   	Card_SN[4], Card_SN[5], Card_SN[6], Card_SN[7]);
                    printf("[MGCA312_EmmDataGot]EMM show 3Min Finger, CA CardSN(%s).\n",\
						 	s_text_buffer);
                    MGCA312_toShowFinger(TRUE, s_text_buffer);
				}
				break;		
					
			case MG_EMM_MSG:
				{
					printf("[MGCA312_EmmDataGot]recv msg<MG_EMM_MSG>$\n");
					MG_U8 *pCmdData = NULL;
					
					pCmdData = MG_Get_Command_Data();
					memset(&g_rollingMsg[MG312_MSG_MSG], 0, sizeof(mg_cas312_rolling_message_st));
					g_rollingMsg[MG312_MSG_MSG].message_type = MG312_MSG_MSG;
					memcpy(g_rollingMsg[MG312_MSG_MSG].content, pCmdData, 128);
					g_rollingMsg[MG312_MSG_MSG].content[128] = 0;
					//printf("[MGCA312_EmmDataGot]<EMM_MSG>(%s)$\n", g_rollingMsg[MG312_MSG_MSG].content);
					app_cas_api_on_event(DVB_CA_ROLLING_INFO, CAS_NAME,\
						   				&g_rollingMsg[MG312_MSG_MSG],\
						   				sizeof(mg_cas312_rolling_message_st));	
				}
				break;
				
			case MG_EMM_OSD:
				{
					printf("[MGCA312_EmmDataGot]recv msg<MG_EMM_OSD>#\n");
					MG_U8 *pCmdData = NULL;
                    int i, j;
                    char *pstr = NULL;
					
					pCmdData = MG_Get_Command_Data();
					if ((0x21 == pCmdData[0]) || (0x22 == pCmdData[0]))
					{
                        GxCore_MutexLock(g_rollingMsg[MG312_MSG_OSD].mutex);
						memset(g_rollingMsg[MG312_MSG_OSD].content, 0, MG312_CA_MAX_LEN_OSD);
						g_rollingMsg[MG312_MSG_OSD].message_type = MG312_MSG_OSD;
						if (0x21 == pCmdData[0])
						{
							g_rollingMsg[MG312_MSG_OSD].byStyle = MG312_CA_OSD_TOP;
						}

						if (0x22 == pCmdData[0])
						{
							g_rollingMsg[MG312_MSG_OSD].byStyle = MG312_CA_OSD_BOTTOM;
						}

                        i = 0;
	                    j = 0;
                        pstr = (char *)(pCmdData+3);
		                for (i = 0; i < strlen(pstr); i++)
                		{
                			if (j >= MG312_CA_MAX_LEN_OSD)
                			{
                                g_rollingMsg[MG312_MSG_OSD].content[j-1] = '\0';
                				break;
                			}
                            
                			if ((0x0d != pstr[i]) && (0x0a != pstr[i]))
            				{
            					g_rollingMsg[MG312_MSG_OSD].content[j++] = pstr[i];
            				}
                		}

						if (j < MG312_CA_MAX_LEN_OSD)
						{
							g_rollingMsg[MG312_MSG_OSD].content[j] = 0;
						}
						
						//printf("[MGCA312_EmmDataGot]<EMM_OSD(%d)>(%s)#\n",
							  	//g_rollingMsg[MG312_MSG_OSD].byStyle,
							  	//g_rollingMsg[MG312_MSG_OSD].content);
						g_rollingMsg[MG312_MSG_OSD].wTimes = pCmdData[1];	
						g_rollingMsg[MG312_MSG_OSD].wStatus = TRUE;
                        g_rollingMsg[MG312_MSG_OSD].roll_status = FALSE;
                        GxCore_MutexUnlock(g_rollingMsg[MG312_MSG_OSD].mutex);
						app_cas_api_on_event(DVB_CA_ROLLING_INFO, CAS_NAME,\
							    			&g_rollingMsg[MG312_MSG_OSD],\
							    			sizeof(mg_cas312_rolling_message_st));						
					}
				}
				break;	
				
			case MG_EMM_EMAIL://接收邮件
				{
					printf("[MGCA312_EmmDataGot]recv msg<MG_EMM_EMAIL>@\n");
    				MG_U8 *pEmailHead, *pEmailBody;
    				MG_U8 EmailHead[50];
    				MG_U8 EmailBody[512];
                    char Title[21];
                    char Sender[21];
                    char cTime[20];
                    int ContentLen = 0;
    				int ret = 0;
					
    				pEmailHead = MG_Get_Mail_Head();
    				pEmailBody = MG_Get_Mail_Body();
                    memset(EmailHead, 0, sizeof(EmailHead));
                    memset(EmailBody, 0, sizeof(EmailBody));
    				memcpy(EmailHead, pEmailHead, 50);
    				memcpy(EmailBody, pEmailBody, 512);
					
                    //保存邮件数据
                    MG_CAS312_EmailHead_Analyze(EmailHead, Title, Sender, cTime, &ContentLen);
                    if ((ContentLen >= 0) && (ContentLen < 512))
                    {
                        EmailBody[ContentLen] = '\0';
                    }
					
                    ret = MG_CAS312_EmailList_Update(Title,(char *)EmailBody, Sender, cTime, 0);
                    if (ret > 0)
                    {
                        MGCA312_toEmailNotify(MGCAS312_NEW_EAMIL);
                    }
                    else if (ret == 0)//邮件保存失败
                    {
                        MGCA312_toEmailNotify(MGCAS312_NEW_EMAIL_NO_ROOM);
                    }
				}
				break;

			default:
				break;
		}
	}
	else
	{
		printf("!!![MGCA312_EmmDataGot]Error; EmmResult(%d)!!!.\n", EmmResult);
	}	

	return;
}

#endif

