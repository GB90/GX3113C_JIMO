/*****************************************************************************
* 						   CONFIDENTIAL
*        Hangzhou GuoXin Science and Technology Co., Ltd.
*                      (C)2014, All right reserved
******************************************************************************

******************************************************************************
* File Name :	mg_cas_porting.c
* Author    : 	zhouhm
* Project   :	goxceed dvbc
* Type      :
******************************************************************************
* Purpose   :	mg cas porting
******************************************************************************
* Release History:
VERSION	Date			  AUTHOR         Description
1.0  	2014.04.11		  zhouhm 	 			creation
*****************************************************************************/
#include "mgdef.h"
#include "app_common_porting_ca_flash.h"
#include "app_common_porting_ca_demux.h"
#include "app_common_porting_ca_os.h"
#include "app_common_porting_ca_smart.h"
#include "app_common_porting_ca_descramble.h"
#include "app_common_play.h"
#include "app_mg_cas_api_demux.h"
#include "app_mg_cas_api_pop.h"
#include "app_mg_cas_api_event.h"
#include "app_common_porting_stb_api.h"
#include "app_mg_cas_api_smart.h"
#include "app_mg_cas_api_email.h"
#include "gxplayer_url.h"
#include "app_mg_cas_api_lock_service.h"


extern mg_cas_rolling_message_st  rollingmsg[MG_MSG_MAX];
static mg_cas_auth_st auth = {0};
static mg_cas_email_st emailflag= {0};

handle_t semaphore = 0;	
extern uint8_t zonecode; 



static MG_U16 counterU = 0;
static MG_U16 counterG = 0;
static MG_U16 counterA = 0;

#define MGCAS42X_STB_SEARIL_DV			"0000000000"
#define MGCAS42X_STB_SEARIL			    "STBSerail"
#define MGCAS42X_SERIAL_LEN         (10)


uint8_t reply[1024] = {0};
char*  MGCA_Get_Config_Serial(void)
{
	static char serial[MGCAS42X_SERIAL_LEN+1]={0};

	memset(serial,0,MGCAS42X_SERIAL_LEN+1);
	GxBus_ConfigGet(MGCAS42X_STB_SEARIL, serial,MGCAS42X_SERIAL_LEN,MGCAS42X_STB_SEARIL_DV);
	return serial;
}

int32_t MGCA_Save_Config_Serial(const char* mgca_serial)
{
	if (NULL == mgca_serial)
		return 0;

	GxBus_ConfigSet(MGCAS42X_STB_SEARIL,mgca_serial);
	return 0;
}

static void MGCA_toShowPPVLogo(int time,char *msg)
{
	mg_ca_ppv_data_st ppvdata;

	memset(&ppvdata,0,sizeof(mg_ca_ppv_data_st));
	ppvdata.time = time;
	strncpy(ppvdata.msg,msg,sizeof(ppvdata.msg)-1);
	app_cas_api_on_event(DVB_CA_IPPV_POP_INFO,CAS_NAME, &ppvdata, sizeof(mg_ca_ppv_data_st)); 
}

static void MGCA_toShowFinger(int OnOff,char *msg)
{
	mg_ca_finger_data_st fingerdata;
	memset(&fingerdata,0,sizeof(mg_ca_finger_data_st));

	if(TRUE == OnOff)
	{
		fingerdata.wStatus = TRUE;
		if(msg)
		{
			snprintf(fingerdata.msg,sizeof(fingerdata.msg)-1,"%s",msg);
		}

	}
	else
	{
		fingerdata.wStatus = FALSE;
	}
	app_cas_api_on_event(DVB_CA_FINGER_INFO,CAS_NAME, &fingerdata, sizeof(mg_ca_finger_data_st));
}

uint8_t MGCA_SetPrivateDataFilter( MG_U8        byReqID,
		const MG_U8* pbyFilter,
		const MG_U8* pbyMask,
		MG_U8        byLen,
		MG_U16       wPid,
		MG_U8        byWaitSeconds );
/* 释放私有数据过滤器 */
void MGCA_ReleasePrivateDataFilter( MG_U8  byReqID,
		MG_U16 wPid )
{
	uint32_t filterId = 0;
	bool releaseChannel = TRUE;
	ca_filter_t sfilter = {0};

	/*
	 * 查找对应channel id , filter id
	 */

	for (filterId = 0; filterId< MAX_FILTER_COUNT;filterId++)
	{
		app_porting_ca_demux_get_by_filter_id(filterId,&sfilter);
		if (( 0 != sfilter.usedStatus)&&(0 != sfilter.handle)&&(0 != sfilter.channelhandle)
				&&(wPid == sfilter.pid)&&(byReqID == sfilter.byReqID))
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
				counterU = 0;
			}

			if (EMM_TABLEID_G == byReqID)
			{
				counterG = 0;
			}

			if (EMM_TABLEID_A == byReqID)
			{
				counterA = 0;
			}

			app_porting_ca_demux_release_filter( filterId, releaseChannel);
			return ;

		}
	}

	return ;
}
#if 0
static int _get_take_millisecond(GxTime start, GxTime stop)
{
	int start_ms, stop_ms;

	start_ms = start.seconds * 1000 + start.microsecs / 1000;
	stop_ms = stop.seconds * 1000 + stop.microsecs / 1000;

	return (stop_ms - start_ms);
}
#endif
/*******CA 库内部调用**********/
MG_STATUS MG_STBPort_Send_Command(MG_U8 *MG_SendComm, MG_U8 MG_CommLen, MG_U8 *MG_RecvResp,MG_U16 *SW, MG_U8 * NumberRead)
{
	uint8_t sc_result;
	uint16_t reclen = 0;
	//GxTime time1,time2;

#ifdef _DEBUG
	CAS_DUMP("[MG CAS] Write:", MG_SendComm, MG_CommLen);
#endif

	//GxCore_GetLocalTime(&time1);

	sc_result =  app_porting_ca_smart_sc_apdu61(MG_SendComm,MG_CommLen,reply,&reclen);
	/*
	   GxCore_GetLocalTime(&time2);
	   printf("##########len:%d,take %d ms\n",MG_CommLen,_get_take_millisecond(time1,time2));   
	   */
	if (TRUE == sc_result)
	{
		if (reclen >= 2)
		{
#ifdef _DEBUG
			CAS_DUMP("[MG CAS] Read:", reply, reclen);
#endif
			*NumberRead = reclen-2;
			memcpy(MG_RecvResp, reply, *NumberRead);
			*SW = ((reply[ reclen-2]<<8)+reply[ reclen-1])&0xffff;
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
		printf("##MG_STBPort_Send_Command failed. \n");
		return (MG_FAILING);			
	}

}
MG_STATUS MG_STBPort_Create_Semaphore(void)
{
	app_porting_ca_os_sem_create(&semaphore,1);

	return(MG_SUCCESS);
}

MG_STATUS MG_STBPort_Obtain_Semaphore(void)
{
	app_porting_ca_os_sem_wait(semaphore);

	return(MG_SUCCESS);
}

MG_STATUS MG_STBPort_Release_Semaphore(void)
{
	app_porting_ca_os_sem_signal(semaphore);

	return(MG_SUCCESS);
}
/*****************/

//设置emc过滤器
uint8_t MGCA_SetEcmFilter(MG_U16       wPid)
{
	uint8_t match[8]={0};
	uint8_t mask[8]={0};
	match[0] = ECM_TABLEID_80;
	mask[0] = 0xFE;

	MGCA_SetPrivateDataFilter( ECM_TABLEID_80,
			match,
			mask,
			1,
			wPid,
			0 );	
	return 0;
}

void MGCA_ShowBugMessage(MG_S16 messageType)
{
	MG_U8  byMessageType = 0xff;

	switch(messageType)
	{
		case MG_SUCCESS:
			byMessageType = CA_MG_SUCCESS;
			break;
		case MG_ERR_SCARD_NOT_ACT :
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
		case MG_ERR_GET_CARD_DATA:
			byMessageType = CA_MG_ERR_GET_CARD_DATA;
			break;
		case MG_ERR_ILLEGAL_CARD:
			byMessageType = CA_MG_ERR_ILLEGAL_CARD;
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
		case MG_ERR_CONTROL_PLAY: 
			byMessageType = CA_MG_ERR_CONTROL_PLAY;
			break;
		case MG_ERR_AUTH_CLOSED:
			byMessageType = CA_MG_ERR_AUTH_CLOSED;
			break;
		case MG_ERR_AUTH_OVERDUE: 
			byMessageType = CA_MG_ERR_AUTH_OVERDUE;
			break;
		case MG_ERR_AUTH_UPDATE: 
			byMessageType = CA_MG_ERR_AUTH_UPDATE;
			break;
		case MG_ERR_ILLEGAL_AUTH: 
			byMessageType = CA_MG_ERR_ILLEGAL_AUTH;
			break;
		case MG_ERR_CARD_STATUS_RENEW:
			byMessageType = CA_MG_ERR_CARD_STATUS_RENEW;
			break;
		case MG_ERR_CARD_OPERATE: 
			byMessageType = CA_MG_ERR_CARD_OPERATE;
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
			if(messageType==MG_EMM_EN_PARTNER)
				byMessageType = CA_MG_EMM_EN_PARTNER;
			else if(messageType==MG_EMM_DIS_PARTNER)
				byMessageType = CA_MG_EMM_DIS_PARTNER;
			else /* MG_EMM_SONCARD */
				byMessageType = CA_MG_EMM_SONCARD;
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
		case MG_EMM_UPDATE_CRT:
			byMessageType = CA_MG_EMM_UPDATE_CRT;
			break;
		case MG_EMM_MODIFY_CTRLCODE:
			byMessageType = CA_MG_EMM_MODIFY_CTRLCODE;
			break;
		default:
			/* only for dbg */
			break;
	}

	if (0xff != byMessageType)
	{
		if (MSG_POP_COMMON_END+byMessageType+1 >= MESSAGE_MAX_COUNT)
			return ;			
	}
	if ( TRUE != app_play_get_msg_pop_type_state(MSG_POP_COMMON_END+byMessageType+1))
	{
		app_play_clear_ca_msg();
		app_play_set_msg_pop_type(MSG_POP_COMMON_END+byMessageType+1);

#ifdef _DEBUG
		printf("pop msgid:%d\n",MSG_POP_COMMON_END+byMessageType+1);
#endif

	}

	return ;

}
//ecm数据处理
/*
wEcmPID:pid
MG_ECM:ecm数据处理
*/
void MGCA_EcmDataGot(MG_U16 wEcmPID,MG_U8 *MG_ECM)
{
	MG_STATUS EcmResult;
	MG_U8 pCWUpd;
	MG_U8 pProgStatus;
	static MG_U8 pProgStatusbak = 0xfe;
	MG_U8 pOddCW[8], pEvenCW[8];
	static MG_U8 OldCw[8];
	static MG_U8 EvenCw[8];

	EcmResult = MG_ECM_Process(MG_ECM, zonecode,
			&pCWUpd, &pProgStatus);

	if(pCWUpd)
	{
		if(pCWUpd & MG_CWUPD_BIT_ODD) /* Odd CW changed */
		{
			MG_Get_Odd_CW(pOddCW);
			memcpy(OldCw,pOddCW,8);
			app_porting_ca_descramble_set_cw(wEcmPID,OldCw,EvenCw,8,0);
		}
		if(pCWUpd & MG_CWUPD_BIT_EVEN) /* Even CW changed */
		{
			MG_Get_Even_CW(pEvenCW);
			memcpy(EvenCw,pEvenCW,8);
			app_porting_ca_descramble_set_cw(wEcmPID,OldCw,EvenCw,8,0);
		}
#if 0
		//cth
		prog_print_time();
#endif

#ifdef _DEBUG
		CAS_DUMP("[OldCw] ->", OldCw, 8);
		CAS_DUMP("[EvenCw] ->", EvenCw, 8);
#endif

	}

	if(pProgStatus & MG_PROGSTA_BIT_PPVUpd)
	{
		/* disp ppv logo at screen corner,
		   delay 5 seconds then vanish */
		printf("ECM show 5s PPV LOGO.\n");
		MGCA_toShowPPVLogo(5,"PPV");
	}

	if (pProgStatus != pProgStatusbak)
	{
		pProgStatusbak = pProgStatus;
		if(pProgStatus & MG_PROGSTA_BIT_PreAuth)
		{   /* disp 预授权 logo */
			MGCA_toShowPPVLogo(3,"Pre PPV");
		}
	}


#ifdef _DEBUG
	printf("########MGCA_EcmDataGot:%d\n",EcmResult);
#endif

	/* checking ecm func result */
	if(EcmResult==MG_SUCCESS)
	{
		/* nothing should do */
		MGCA_ShowBugMessage(MG_SUCCESS);
		/* only for dbg */
	}
	else if(EcmResult == MG_FAILING)
	{
		/* nothing should do */
		/* only for dbg */
	}
	else if(EcmResult==MG_ERR_SCARD_NOT_ACT || 
			EcmResult==MG_ERR_SCARD_DISABLED)
	{
		MGCA_ShowBugMessage(CA_MG_ERR_SCARD_NOT_ACT);
	}
	else if(EcmResult==MG_ERR_ZONE_CODE)
	{
		MGCA_ShowBugMessage(MG_ERR_ZONE_CODE);
	}
	else if(EcmResult==MG_ERR_AUTHNEARBOOT_REMIND ||
			EcmResult==MG_ERR_AUTHNEARTIME_REMIND)
	{	/* vanish only remote key pressed */
		auth.status = EcmResult;
		app_cas_api_on_event(DVB_CA_ENTITLE_INFO,CAS_NAME, &auth, sizeof(mg_cas_auth_st));					
	}
	else if(EcmResult==MG_ERR_AUTHEXPIRE_REMIND)
	{	/* vanish only remote key pressed */
		auth.status = EcmResult;
		app_cas_api_on_event(DVB_CA_ENTITLE_INFO,CAS_NAME, &auth, sizeof(mg_cas_auth_st));					
	}
	else if(EcmResult==MG_ERR_NO_MONEY)
	{
		MGCA_ShowBugMessage(MG_ERR_NO_MONEY);

	}
	else if(EcmResult==MG_ERR_NOT_AUTHORED)
	{
		MGCA_ShowBugMessage(MG_ERR_NOT_AUTHORED);
	}
	else if(EcmResult==MG_ERR_CARD_LOCKED)
	{
		MGCA_ShowBugMessage(MG_ERR_CARD_LOCKED);
	}
	else if(EcmResult==MG_ERR_APP_LOCKED)
	{
		MGCA_ShowBugMessage(MG_ERR_APP_LOCKED);

	}
	else if(EcmResult==MG_ERR_PIN_AUTHEN)
	{

		MGCA_ShowBugMessage(MG_ERR_PIN_AUTHEN);		
	}
	else if(EcmResult==MG_ERR_EXTER_AUTHEN)
	{
		MGCA_ShowBugMessage(MG_ERR_EXTER_AUTHEN);

	}
	else if(EcmResult==MG_ERR_GET_CARD_DATA)
	{
		MGCA_ShowBugMessage(MG_ERR_GET_CARD_DATA);

	}
	else if(EcmResult==MG_ERR_ILLEGAL_CARD)
	{
		MGCA_ShowBugMessage(MG_ERR_ILLEGAL_CARD);

	}
	else if(EcmResult==MG_ERR_QUESTION_TELEVIEW_PPV || 
			EcmResult==MG_ERR_DONOT_TELEVIEW_PPV)
	{
		MGCA_ShowBugMessage(MG_ERR_DONOT_TELEVIEW_PPV);

	}
	else if(EcmResult==MG_ERR_CARD_NOTFOUND)
	{
		MGCA_ShowBugMessage(MG_ERR_CARD_NOTFOUND);

	}
	else if(EcmResult==MG_ERR_CARD_NOTMATCH)
	{
		MGCA_ShowBugMessage(MG_ERR_CARD_NOTMATCH);

	}
	else if(EcmResult==MG_ERR_UPDATE_GK)
	{
		MGCA_ShowBugMessage(MG_ERR_UPDATE_GK);

	}
	else if(EcmResult==MG_ERR_CONTROL_PLAY)
	{
		MGCA_ShowBugMessage(MG_ERR_CONTROL_PLAY);

	}
	else if(EcmResult==MG_ERR_AUTH_CLOSED)
	{
		MGCA_ShowBugMessage(MG_ERR_AUTH_CLOSED);

	}
	else if(EcmResult==MG_ERR_AUTH_OVERDUE)
	{
		MGCA_ShowBugMessage(MG_ERR_AUTH_OVERDUE);

	}
	else if(EcmResult==MG_ERR_AUTH_UPDATE)
	{
		MGCA_ShowBugMessage(MG_ERR_AUTH_UPDATE);

	}
	else if(EcmResult==MG_ERR_ILLEGAL_AUTH)
	{
		MGCA_ShowBugMessage(MG_ERR_ILLEGAL_AUTH);

	}
	else if(EcmResult==MG_ERR_CARD_STATUS_RENEW)
	{
		MGCA_ShowBugMessage(MG_ERR_CARD_STATUS_RENEW);

	}
	else if(EcmResult==MG_ERR_CARD_OPERATE)
	{
		MGCA_ShowBugMessage(MG_ERR_CARD_OPERATE);

	}
	else
	{
		/* nothing should do */
		//		STB_DbgMsg("wrong msg %d",EcmResult); /* only for dbg */
	}	

}

//Emm过滤器设置
void MGCA_SetEmmFilterU(uint16_t emm_pid)
{
	uint8_t match[16];
	uint8_t card_sn[6];
	uint8_t mask[16];
	uint16_t ucounter;
	uint16_t i;
	memset(match,0,sizeof(match));
	memset(mask,0,sizeof(mask));


	/* set condition*/
	if (MG_Get_Card_Ready() == MG_TRUE)
	{
		ucounter =  MG_Get_UCounter()+1;
		if(counterU == ucounter)
		{
			return;
		}
		counterU = ucounter;

		match[0] = EMM_TABLEID_U;
		mask[0] = 0xff;

		match[3] = (ucounter>>8)&0xff;
		match[4] = ucounter&0xff;
		mask[3] = 0xff;
		mask[4] = 0xff;

		MG_Get_Card_ID(card_sn);
		for( i=5 ; i<9 ; i++ )
		{
			match[i] = card_sn[i-5];
			mask[i] = 0xff;
		}

		MGCA_SetPrivateDataFilter( EMM_TABLEID_U,
				match,
				mask,
				10,
				emm_pid,
				20 );

		counterU = ucounter;

	}
	else
	{
		printf("MG_Get_Card_Ready = 0\n");
	}
}

void MGCA_SetEmmFilterG(uint16_t emm_pid)
{
	uint8_t match[16]={0};
	uint8_t mask[16]={0};
	uint16_t ucounter;
	memset(match,0,sizeof(match));
	memset(mask,0,sizeof(mask));

	/* set condition*/
	if (MG_Get_Card_Ready() == MG_TRUE)
	{
		ucounter =  MG_Get_GCounter()+1;
		if(counterG == ucounter)
		{

			return;
		}
		counterG = ucounter;

		match[0] = EMM_TABLEID_G;
		mask[0] = 0xff;

		match[3] = (ucounter>>8)&0xff;
		match[4] = ucounter&0xff;
		mask[3] = 0xff;
		mask[4] = 0xff;

		MGCA_SetPrivateDataFilter( EMM_TABLEID_G,
				match,
				mask,
				5,
				emm_pid,
				20 );

		counterG = ucounter;

	}
	else
	{
		printf("MG_Get_Card_Ready = 0\n");
	}

}

void MGCA_SetEmmFilterA(uint16_t emm_pid)
{
	uint8_t match[16];
	uint8_t mask[16];
	uint16_t ucounter;
	memset(match,0,sizeof(match));
	memset(mask,0,sizeof(mask));

	/* set condition*/
	if (MG_Get_Card_Ready() == MG_TRUE)
	{
		ucounter =  MG_Get_ACounter()+1;
		if(counterA == ucounter)
		{
			return;
		}
		counterA = ucounter;	

		match[0] = EMM_TABLEID_A;
		mask[0] = 0xff;

		match[3] = (ucounter>>8)&0xff;
		match[4] = ucounter&0xff;
		mask[3] = 0xff;
		mask[4] = 0xff;
		MGCA_SetPrivateDataFilter( EMM_TABLEID_A,
				match,
				mask,
				5,
				emm_pid,
				20 );
		counterA = ucounter;  
	}
	else
	{
		printf("MG_Get_Card_Ready = 0\n");
	}

}


/*++
  功能：显示新邮件的标志。
  参数：
byShow:					标志类型。含义如下：
MGCAS_NEW_EAMIL						新邮件通知。
MGCAS_NEW_EMAIL_NO_ROOM		有新邮件，但Email空间不够。
MGCAS_EMAIL_NONE					没有新邮件。
*/
void MGCA_toEmailNotify(uint8_t byShow)
{

	if ((MGCAS_EMAIL_NONE == byShow)
			&&(DVB_CA_EMAIL_FLAG_HIDE == emailflag.wStatus))
		return ;

	switch(byShow)
	{
		case MGCAS_EMAIL_NONE:
			emailflag.wStatus = DVB_CA_EMAIL_FLAG_HIDE;
			break;
		case MGCAS_NEW_EAMIL:
			emailflag.wStatus = DVB_CA_EMAIL_NEW;
			break;
		case MGCAS_NEW_EMAIL_NO_ROOM:
			emailflag.wStatus = DVB_CA_EMAIL_EXHAUST;				
			break;
		default:
			break;
	}

	app_cas_api_on_event(DVB_CA_EMAIL_INFO,CAS_NAME, &emailflag, sizeof(emailflag));
	return;
}



//EMM处理
void MGCA_EmmDataGot(MG_U16 wEcmPID,MG_U8 *MG_EMM)
{
	MG_STATUS EmmResult;
	MG_U8 EmmFunc;
	MG_U8 STBSerial[11] = {0,};


	if((EmmResult = MG_EMM_C_Process(MG_EMM,&EmmFunc)) == MG_SUCCESS)
	{
#ifdef _DEBUG
		printf("########MGCA_EmmDataGot:%d\n",EmmResult);
#endif
		switch(EmmFunc)
		{
			case MG_EMM_APP_STOP:
				MGCA_ShowBugMessage(MG_EMM_APP_STOP);
				break;

			case MG_EMM_APP_RESUME:
				MGCA_ShowBugMessage(MG_EMM_APP_RESUME);
				break;

			case MG_EMM_PROG_UNAUTH:
				MGCA_ShowBugMessage(MG_EMM_PROG_UNAUTH);
				break;

			case MG_EMM_PROG_AUTH:
				MGCA_ShowBugMessage(MG_EMM_PROG_AUTH);
				break;

			case MG_EMM_EN_PARTNER:
			case MG_EMM_DIS_PARTNER:
			case MG_EMM_SONCARD:
				{
					MG_U8 MG_PartnerSerials[11]={0};
					MG_Get_STB_Serial(MG_PartnerSerials);
					memset(STBSerial, 0, 11);
					memcpy(STBSerial, (char*)MG_PartnerSerials, 10);
					MGCA_Save_Config_Serial((const char*)STBSerial);

					if(EmmFunc==MG_EMM_EN_PARTNER)//启用机卡配对	
						MGCA_ShowBugMessage(MG_EMM_EN_PARTNER);
					else if(EmmFunc==MG_EMM_DIS_PARTNER)//取消机卡配对
						MGCA_ShowBugMessage(MG_EMM_DIS_PARTNER);
					else /* MG_EMM_SONCARD */
						MGCA_ShowBugMessage(MG_EMM_DIS_PARTNER);
				}
				break;

			case MG_EMM_UPDATE_GNO:
				MGCA_ShowBugMessage(MG_EMM_UPDATE_GNO);
				break;

			case MG_EMM_MSG:
				{
					MG_U8 *pCmdData;
					pCmdData=MG_Get_Command_Data();
					memset(&rollingmsg[MG_MSG_MSG],0,sizeof(mg_cas_rolling_message_st));
					rollingmsg[MG_MSG_MSG].message_type = MG_MSG_MSG;
					memcpy(rollingmsg[MG_MSG_MSG].content,pCmdData,128);
					app_cas_api_on_event(DVB_CA_ROLLING_INFO,CAS_NAME, &rollingmsg[MG_MSG_MSG], sizeof(mg_cas_rolling_message_st));						
				}
				break;

			case MG_EMM_UPDATE_GK:
				MGCA_ShowBugMessage(MG_EMM_UPDATE_GK);
				break;

			case MG_EMM_EMAIL://接收邮件
				{
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
					memset(EmailHead,0,sizeof(EmailHead));
					memset(EmailBody,0,sizeof(EmailBody));
					memcpy(EmailHead,pEmailHead,50);
					memcpy(EmailBody,pEmailBody,512);
					//保存邮件数据
					EmailHead_Analyze(EmailHead,Title,Sender,cTime,&ContentLen);
					if(ContentLen >=0 && ContentLen < 512)
					{
						EmailBody[ContentLen]='\0';
					}
					ret = EmailList_Update(Title,(char *)EmailBody,Sender,cTime,0);
					if(ret > 0)
					{
						MGCA_toEmailNotify(MGCAS_NEW_EAMIL);
					}
					else if(ret == 0)//邮件保存失败
					{
						MGCA_toEmailNotify(MGCAS_NEW_EMAIL_NO_ROOM);
					}

					printf("recv msg[MG_EMM_EMAIL]\n");
				}
				break;

			case MG_EMM_CHARGE:
				break;
			case MG_EMM_FIGEIDENT://指纹显示
				{
					MG_U8 Card_SN[8];
					char  s_text_buffer[70]={0};
					MG_Get_Card_SN(Card_SN);
					sprintf(s_text_buffer,"%02X%02X%02X%02X%02X%02X%02X%02X",
							Card_SN[0],Card_SN[1],Card_SN[2],Card_SN[3],Card_SN[4],Card_SN[5],Card_SN[6],Card_SN[7]);
					printf("recv msg[MG_EMM_FIGEIDENT]\n");
					printf("EMM show 3Min Finger,CA CardSN:%s\n",s_text_buffer);
					MGCA_toShowFinger(TRUE,s_text_buffer);
				}
				break;

			case MG_EMM_URGENT_SERVICE:
				{
					MG_U8 UrgentServiceID[2];
					uint16_t wServiceID;
					printf("#######MG_EMM_URGENT_SERVICE \n");
					MG_Get_ServiceID(UrgentServiceID);
					wServiceID = (UrgentServiceID[0]<<8)+UrgentServiceID[1];
					app_mg_cas_api_lock_service(wServiceID);
				}
				break;
			case MG_EMM_MODIFY_ZONECODE:
				MGCA_ShowBugMessage(MG_EMM_MODIFY_ZONECODE);
				break;

			case MG_EMM_OSD:
				{
					MG_U8 *pCmdData;
					int i,j;
					char *pstr = NULL;
					pCmdData=MG_Get_Command_Data();
					if ((0x21 == pCmdData[0])||(0x22 == pCmdData[0]))
					{
						GxCore_MutexLock (rollingmsg[MG_MSG_OSD].mutex);
						memset(rollingmsg[MG_MSG_OSD].content,0,MG_CA_MAX_LEN_OSD);
						rollingmsg[MG_MSG_OSD].message_type = MG_MSG_OSD;
						if (0x21 == pCmdData[0])
						{
							rollingmsg[MG_MSG_OSD].byStyle = MG_CA_OSD_TOP;
						}

						if (0x22 == pCmdData[0])
						{
							rollingmsg[MG_MSG_OSD].byStyle = MG_CA_OSD_BOTTOM;
						}

						i = 0;
						j = 0;
						pstr = (char *)(pCmdData+3);
						for (i = 0; i< strlen(pstr);i++)
						{
							if (j >= MG_CA_MAX_LEN_OSD)
							{
								rollingmsg[MG_MSG_OSD].content[j-1] = '\0';
								break;
							}

							if ((0x0d != pstr[i] )&&(0x0a != pstr[i]))
							{
								rollingmsg[MG_MSG_OSD].content[j++] = pstr[i];
							}
						}

						rollingmsg[MG_MSG_OSD].wTimes = pCmdData[1];	
						rollingmsg[MG_MSG_OSD].wStatus = TRUE;
						rollingmsg[MG_MSG_OSD].roll_status = FALSE;
						GxCore_MutexUnlock (rollingmsg[MG_MSG_OSD].mutex);
						app_cas_api_on_event(DVB_CA_ROLLING_INFO,CAS_NAME, &rollingmsg[MG_MSG_OSD], sizeof(mg_cas_rolling_message_st));						
					}
				}
				break;

			case MG_EMM_UPDATE_CRT:
				MGCA_ShowBugMessage(MG_EMM_UPDATE_CRT);
				break;

			case MG_EMM_MODIFY_CTRLCODE:
				MGCA_ShowBugMessage(MG_EMM_MODIFY_CTRLCODE);
				break;

			default:
				/* only for dbg */
				//STB_DbgMsg("other functions %d",EmmFunc); 
				break;
		}
	}
	else
	{
		//		STB_DbgMsg("wrong results %d",EmmResult); /* only for dbg */
	}	
}


/* 设置私有数据过滤器 */
uint8_t MGCA_SetPrivateDataFilter( MG_U8        byReqID,
		const MG_U8* pbyFilter,
		const MG_U8* pbyMask,
		MG_U8        byLen,
		MG_U16       wPid,
		MG_U8        byWaitSeconds )
{
	ca_filter_t filter = {0};
	bool repeatFlag = TRUE;
	uint8_t     match[18] = {0,};
	uint8_t     mask[18] = {0,};

	if(pbyFilter == NULL || pbyMask == NULL )
	{
		return FALSE;
	}

	memcpy(match,pbyFilter,byLen);
	memcpy(mask,pbyMask,byLen);

	//	DUMP(DEMUX,INFO,byLen,pbyFilter,"%x");
	//	DUMP(DEMUX,INFO,byLen,pbyMask,"%x");

	MGCA_ReleasePrivateDataFilter(   byReqID, wPid );

	filter.byReqID = byReqID;
	filter.crcFlag = FALSE;
	filter.filterLen = byLen;
	memcpy(filter.match,match,byLen);
	memcpy(filter.mask,mask,byLen);
	filter.nWaitSeconds = byWaitSeconds;
	filter.pid = wPid;
	filter.equalFlag = TRUE;
	filter.repeatFlag = repeatFlag;
	filter.Callback = app_mg_cas_filter_notify;//过滤器数据回调处理
	filter.timeOutCallback = app_mg_cas_filter_timeout_notify;//超时回调
	return app_porting_ca_demux_start_filter(&filter);//设置CA过滤器

}

