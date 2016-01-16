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
* Purpose   :	模块头文件
******************************************************************************
* Release History:
VERSION	Date			  AUTHOR         Description
1.0  	2013.5.14		  huangyp 	 	creation
*****************************************************************************/
#include "desai_ca_api.h"
#include "app_common_porting_ca_flash.h"
#include "app_common_porting_ca_demux.h"
#include "app_common_porting_ca_os.h"
#include "app_common_porting_ca_smart.h"
#include "app_common_porting_ca_descramble.h"
#include "app_common_play.h"
#include "app_desai_cas_5.0_api_demux.h"
#include "app_common_porting_stb_api.h"
#include "app_desai_cas_5.0_api_smart.h"
#include "gxplayer_url.h"
#include "gxprogram_manage_berkeley.h"
#include "app_desai_cas_5.0_api_pop.h"
#include "app_common_prog.h"
#if defined (DVB_AD_TYPE_DS_FLAG)||defined (DVB_AD_TYPE_DS_FLAG41)
#include "app_ds_ads_porting_stb_api.h"
#endif
extern uint8_t Show_time_short;		//huangyp		add
static INT32 pos_prog=0;		//紧急广播功能

static desai_cas50_finger_data_st  fingerdata = {0};
static desai_cas50_osd_data_st  osddata = {0};
static desai_cas50_email_st emailflag= {0};
static desai_cas50_exigent_data_st exigentdata = {0};
static desai_cas50_ippv_buy_st ippvbuy = {0};
static uint8_t ds_ca_osd_need_show = 0;
uint32_t g_smartcard_upgrade_num = 0;
static DESAI_CAS50_CARD_UPDATE_S smardcard_update = {0};
#if 1

#define DS_ASSERT( expression )                    assert( (expression) )
#define DSCAS_Dbg(...)                          do {                                            \
                                                    printf("[DSCAS-STB]\t");                        \
                                                    printf(__VA_ARGS__);                        \
                                                } while(0)
#else
#define DS_ASSERT( expression )
#define DSCAS_Dbg(...)
#endif


#define DSCAS_P(...)                          do {                                            \
                                                   /* printf("[DSCA]");                 */       \
                                                    printf(__VA_ARGS__);                        \
                                                } while(0)

void DSCA_printf(IN const INT8 *string,...)
{
#if 0
	static char buf[1024];
	va_list args;
	int i;
	va_start(args,string);
	i = Vsnprintf(buf,sizeof(buf), string, args);
	va_end(args);

	DSCAS_P("%s", buf);
#endif
}
#define DSCA_FLASH_START_ADDR_OFFSET  0x100
void DSCA_GetBuffer(IN UINT32 * puiStartAddr,  IN UINT32 * puiSize)
{

	DSCAS_Dbg("%s\n",__FUNCTION__);
	/*由于CA库中对起始地址要求不能为0，
	因此手动偏移任何非0个字节*/
	*puiStartAddr = DSCA_FLASH_START_ADDR_OFFSET;
	*puiSize = 0x10000;

    	return;
}

void DSCA_FlashRead(UINT32 uiStartAddr,  IN UINT8 * pucData,  OUT UINT32 * puiLen)
{
	UINT32 startAddr = uiStartAddr;
	if (NULL == pucData)
	{
		CA_FAIL(("STBCA_ReadBuffer   failed \n"));
		return ;
	}
	if(startAddr >= DSCA_FLASH_START_ADDR_OFFSET)
	{
		startAddr -= DSCA_FLASH_START_ADDR_OFFSET;
	}
	DSCAS_Dbg("%s	uiStartAddr:0x%x\n",__FUNCTION__,startAddr);
	app_porting_ca_flash_read_data(startAddr,(uint8_t *)pucData,(uint32_t*)puiLen);

	return ;
}

void DSCA_FlashWrite(UINT32 uiStartAddr, OUT UINT8* pucData, UINT32 uiLen)
{
	UINT32 startAddr = uiStartAddr;

	if (NULL == pucData)
	{
		CA_FAIL(("STBCA_WriteBuffer   failed \n"));
		return ;
	}
	if(startAddr >= DSCA_FLASH_START_ADDR_OFFSET)
	{
		startAddr -= DSCA_FLASH_START_ADDR_OFFSET;
	}
	DSCAS_Dbg("%s	uiStartAddr:0x%x\n",__FUNCTION__,startAddr);
	app_porting_ca_flash_write_data(startAddr,(uint8_t*)pucData,uiLen);

	return ;
}


void DSCA_FlashErase(void)
{

	DSCAS_Dbg("%s\n",__FUNCTION__);

	app_porting_ca_flash_erase(0,64*1024);

	return ;

}

void* DSCA_Malloc(UINT16 usBufferSize)
{
//	DSCAS_Dbg("%s:%d\n",__FUNCTION__,usBufferSize);
	return app_porting_ca_os_malloc(usBufferSize);
}
void DSCA_Memset(IN void* pucBuffer, UINT8 ucValue, UINT32 uiSize)
{
//	DSCAS_Dbg("%s\n",__FUNCTION__);
	memset(pucBuffer,ucValue,uiSize);
}
void DSCA_Memcpy(IN void* pucDestBuffer, IN void* pucSourceBuffer, UINT32 uiSize)
{
//	DSCAS_Dbg("%s\n",__FUNCTION__);
	memcpy(pucDestBuffer,pucSourceBuffer,uiSize);
	if(uiSize>20)
	{
		DSCA_Sleep(10);
	}
}
INT32 DSCA_Memcmp(IN void* pucDestBuffer, IN void* pucSourceBuffer, UINT32 uiSize)
{
//	DSCAS_Dbg("%s\n",__FUNCTION__);
	return memcmp(pucDestBuffer,pucSourceBuffer,uiSize);
}
INT32 DSCA_Sprintf(IN UINT8* pucDestBuffer, IN const UINT8* pucFormatBuffer, ...)
{
	DSCAS_Dbg("%s\n",__FUNCTION__);
	va_list args;
	int i;

	va_start(args, pucFormatBuffer);
	i = vsprintf((char*)pucDestBuffer, (char*)pucFormatBuffer, args);
	va_end(args);

	return i;
}
INT32 DSCA_StrLen(IN const UINT8* pucFormatBuffer)
{
//	DSCAS_Dbg("%s\n",__FUNCTION__);
	return strlen((char*)pucFormatBuffer);
}
void DSCA_Free(IN void* pucBuffer)
{
//	DSCAS_Dbg("%s\n",__FUNCTION__);
	app_porting_ca_os_free(pucBuffer);
}
void DSCA_Sleep(UINT16 usMilliSeconds)
{
//	DSCAS_Dbg("%s:%d\n",__FUNCTION__,usMilliSeconds);
	app_porting_ca_os_sleep(usMilliSeconds);
	return ;
}
void DSCA_SemaphoreInit(IN DSCA_Semaphore* puiSemaphore , UINT8 ucInitVal)
{
	handle_t semaphore = 0;

	if (NULL == puiSemaphore)
	{
		return ;
	}
    	app_porting_ca_os_sem_create(&semaphore,ucInitVal);
	*puiSemaphore = (handle_t)semaphore;
	DSCAS_Dbg("%s:		%d		%d\n",__FUNCTION__,*puiSemaphore,ucInitVal);
	return ;
}
void DSCA_ReleaseSemaphore(IN DSCA_Semaphore* puiSemaphore)
{
//	DSCAS_Dbg("%s:		%d\n",__FUNCTION__,*puiSemaphore);
	if (NULL == puiSemaphore)
	{
		return ;
	}
	app_porting_ca_os_sem_signal((handle_t)*puiSemaphore);

	return ;
}
void DSCA_WaitSemaphore(IN DSCA_Semaphore* puiSemaphore)
{
//	DSCAS_Dbg("%s:		%d\n",__FUNCTION__,*puiSemaphore);
	if (NULL == puiSemaphore)
	{
		return ;
	}
	app_porting_ca_os_sem_wait((handle_t)*puiSemaphore);

	return ;
}

void DSCA_ShowLong_Message(UINT8 ucMessageType,EN_CAS_SHOW_MSG_TYPE  enShowType)
{
	DSCAS_Dbg("%s,ucMessageType=0x%x,enShowType=0x%x\n",
		__FUNCTION__,ucMessageType,enShowType);
	UINT8 ucMessageType_new = 0;
	if(ucMessageType <= MESSAGE_CARD_INIT_OK_TYPE)
		ucMessageType_new = ucMessageType;
	else if((ucMessageType > MESSAGE_CARD_INIT_OK_TYPE)&&(ucMessageType < MESSAGE_NOOPER_TYPE))
		return;
	else if(ucMessageType <= MESSAGE_PAIRING_TYPE)
		ucMessageType_new = ucMessageType - 8;
	else if((ucMessageType > MESSAGE_PAIRING_TYPE)&&(ucMessageType < MESSAGE_NOENTITLE_TYPE))
		return;
	else if(ucMessageType <= MESSAGE_FINGER_SHOW_TYPE)
		ucMessageType_new = ucMessageType - 19;
	else if((ucMessageType > MESSAGE_FINGER_SHOW_TYPE)&&(ucMessageType < MESSAGE_FINGER_DISAPPEAR_TYPE))
		return;
	else if(ucMessageType <= MESSAGE_CHANGEPRO_TOO_FAST_TYPE)
		ucMessageType_new = ucMessageType - 30;
	else if((ucMessageType > MESSAGE_CHANGEPRO_TOO_FAST_TYPE)&&(ucMessageType < MESSAGE_MS_PAIR_OK_TYPE))
		return;
	else if(ucMessageType <= MESSAGE_CANCEL_MATCH_TYPE)
		ucMessageType_new = ucMessageType - 38;
	else if((ucMessageType > MESSAGE_CANCEL_MATCH_TYPE)&&(ucMessageType < MESSAGE_NEWEMAIL_TYPE))
		return;
	else if(ucMessageType <= MESSAGE_NET_ERR_UNLOCK_TYPE)
		ucMessageType_new = ucMessageType - 48;
	else if(ucMessageType <= MESSAGE_HIDEOSD_TYPE)
		ucMessageType_new = ucMessageType - 54;
	else if(ucMessageType <= MESSAGE_CARD_NEED_UPDATE_TYPE)
		ucMessageType_new = ucMessageType - 56;
	else if(ucMessageType == MESSAGE_IPPV_OUTTIME_TYPE)
	{
		ucMessageType_new = ucMessageType - 64;		//IPPV节目过期
		DSCAS_Dbg("%s======other message:%d\n",__FUNCTION__,ucMessageType);
	}

	DSCAS_Dbg("************ucMessageType_new = %d*******************\n",ucMessageType_new);
#if defined (DVB_AD_TYPE_DS_FLAG)||defined (DVB_AD_TYPE_DS_FLAG41)
	if(ucMessageType == MESSAGE_DESCRA_NOENTITLE_TYPE
		|| ucMessageType == MESSAGE_NOENTITLE_TYPE)
	{
		app_ds_ad_set_unauthorized_status(TRUE);
	}
	else if(ucMessageType == MESSAGE_INSERTCARD_TYPE
		|| ucMessageType == MESSAGE_STB_NO_CARD_TYPE)
	{
		app_ds_ad_set_unauthorized_status(FALSE);
	}
	else if (ucMessageType == MESSAGE_CARD_UPDATE_END_TYPE)
	{
		smardcard_update.bStatus = DESAI_CAS50_CARD_UPDATE_SUCCESS;
		app_cas_api_on_event(DVB_CA_CARD_UPDATE_INFO,CAS_NAME, &smardcard_update, sizeof(smardcard_update));
	}
#endif
	if (MSG_POP_COMMON_END+ucMessageType_new+1 >=64)
			return ;

	if ( TRUE != app_play_get_msg_pop_type_state(MSG_POP_COMMON_END+ucMessageType_new+1))
	{
		if(MESSAGE_CANCEL_TYPE == ucMessageType){
			app_play_clear_ca_msg();
		}
		app_play_set_msg_pop_type(MSG_POP_COMMON_END+ucMessageType_new+1);
	}

	return;

}

void DSCA_ShowNone_Message(UINT8 ucMessageType,UINT8 * pucMessage)
{
	static ST_FINGER_SHOW_INFO fingerBuffer;
	DSCAS_Dbg("%s ucMessageType=0x%x\n",__FUNCTION__,ucMessageType);
	switch(ucMessageType)
	{
		case MESSAGE_CARD_INIT_OK_TYPE:
		{
#if defined (DVB_AD_TYPE_DS_FLAG)||defined (DVB_AD_TYPE_DS_FLAG41)
			app_ds_ad_set_unauthorized_status(FALSE);
#endif
			app_play_clear_msg_pop_type(MSG_POP_COMMON_END+MESSAGE_INSERTCARD_TYPE+1);
			app_play_clear_msg_pop_type(MSG_POP_COMMON_END+MESSAGE_STB_NO_CARD_TYPE+1);
			app_play_clear_msg_pop_type(MSG_POP_COMMON_END+MESSAGE_CARD_INIT_TYPE+1);
			//app_play_clear_ca_msg();
			break;
		}
		case MESSAGE_CANCEL_TYPE :
		{
#if defined (DVB_AD_TYPE_DS_FLAG)||defined (DVB_AD_TYPE_DS_FLAG41)

			app_ds_ad_set_unauthorized_status(FALSE);
#endif
			app_play_clear_ca_msg();
			//app_hide_prompt();
			//app_play_set_msg_pop_type(MSG_POP_COMMON_END+0+1);
			break;
		}
		case MESSAGE_EXIGENT_PROG_TYPE :
		{
			DSCAS_Dbg("EXIGENT_PROGRAM\n\n");
			exigentdata.wStatus = TRUE;
			exigentdata.buffer = &pos_prog;
			app_cas_api_on_event(DVB_CA_EMERGENCY_INFO,CAS_NAME, &exigentdata, sizeof(desai_cas50_exigent_data_st));
			break;
		}
		case MESSAGE_EXIGENT_PROG_STOP_TYPE :
		{
			DSCAS_Dbg("STOP_EXIGENT_PROGRAM\n\n");
			exigentdata.wStatus = FALSE;
			app_cas_api_on_event(DVB_CA_EMERGENCY_INFO,CAS_NAME, &exigentdata, sizeof(desai_cas50_exigent_data_st));
			break;
		}
		case MESSAGE_HIDEOSD_TYPE:
		{
			osddata.wStatus = FALSE;
			osddata.buffer = NULL;
			osddata.from = 1;
			ds_ca_osd_need_show = 0;
#if defined (DVB_AD_TYPE_DS_FLAG)||defined (DVB_AD_TYPE_DS_FLAG41)
			if(app_ds_ad_osd_display_status() != -1)
			{
				break;
			}
#endif
			app_cas_api_on_event(DVB_CA_ROLLING_INFO,CAS_NAME, &osddata, sizeof(desai_cas50_osd_data_st));
			break;
		}
		case MESSAGE_FINGER_DISAPPEAR_TYPE :
		{
			memset(&fingerBuffer,0,sizeof(ST_FINGER_SHOW_INFO));
			fingerdata.wStatus = FALSE;
			fingerdata.wShowStatus = FALSE;
			if(pucMessage)
			{
				memcpy(&fingerBuffer,pucMessage,sizeof(ST_FINGER_SHOW_INFO));
				fingerdata.buffer = (uint8_t *)&fingerBuffer;
				fingerdata.position = fingerBuffer.enShowPostion;
			}
			else
			{
				fingerdata.buffer = NULL;
			}
			app_cas_api_on_event(DVB_CA_FINGER_INFO,CAS_NAME, &fingerdata, sizeof(desai_cas50_finger_data_st));
			break;
		}
		case MESSAGE_HIDE_NEWEMAIL_TYPE:
			emailflag.wStatus = ucMessageType;
			app_cas_api_on_event(DVB_CA_EMAIL_INFO,CAS_NAME, &emailflag, sizeof(desai_cas50_email_st));
			break;
		case MESSAGE_HIDE_IPPVBUY_TYPE :
		{
			ippvbuy.wStatus = FALSE;
			ippvbuy.byMessageType = ucMessageType;
			memcpy(&ippvbuy.ippvBuyInfo,(ST_IPPV_INFO *)pucMessage,sizeof(ST_IPPV_INFO));
			app_cas_api_on_event(DVB_CA_IPPV_POP_INFO,CAS_NAME, &ippvbuy, sizeof(desai_cas50_ippv_buy_st));
#if defined (DVB_AD_TYPE_DS_FLAG)||defined (DVB_AD_TYPE_DS_FLAG41)
			app_ds_ad_set_unauthorized_status(FALSE);
#endif
			break;
		}
		default :
			break;
	}

}
void DSCA_ShowFINGER_Message(UINT8 ucMessageType,UINT8 * pucMessage)
{
	static ST_FINGER_SHOW_INFO fingerBuffer;

	memset(&fingerBuffer,0,sizeof(ST_FINGER_SHOW_INFO));
	fingerdata.wStatus = TRUE;
	fingerdata.wShowStatus = FALSE;
	if(pucMessage)
	{
		memcpy(&fingerBuffer,pucMessage,sizeof(ST_FINGER_SHOW_INFO));
		fingerdata.buffer = (uint8_t *)&fingerBuffer;
		fingerdata.position = fingerBuffer.enShowPostion;
	}
	else
	{
		fingerdata.buffer = NULL;
	}
	app_cas_api_on_event(DVB_CA_FINGER_INFO,CAS_NAME, &fingerdata, sizeof(desai_cas50_finger_data_st));

}

void DSCA_ShowOSD_Message(UINT8 ucMessageType,UINT8 * pucMessage)
{
	osddata.wStatus = TRUE;
	osddata.roll_status = FALSE;
	osddata.buffer = pucMessage;
	osddata.from = 1;
	ds_ca_osd_need_show= 1;
#if defined (DVB_AD_TYPE_DS_FLAG)||defined (DVB_AD_TYPE_DS_FLAG41)
	if(app_ds_ad_osd_display_status() != -1)
	{
		return;
	}
#endif
	app_cas_api_on_event(DVB_CA_ROLLING_INFO,CAS_NAME, &osddata, sizeof(desai_cas50_osd_data_st));

}

void DSCA_ShowDialog_Message(UINT8 ucMessageType,UINT8 * pucMessage)
{
	DSCAS_Dbg("%s\n",__FUNCTION__);
	switch(ucMessageType)
	{
		case MESSAGE_IPPVBUY_TYPE :
		{
			ippvbuy.wStatus = TRUE;
			ippvbuy.byMessageType = ucMessageType;
			memcpy(&ippvbuy.ippvBuyInfo,(ST_IPPV_INFO *)pucMessage,sizeof(ST_IPPV_INFO));
			DSCAS_Dbg("IPPV		ippvbuy.ippvBuyInfo.ucProviderID=%d		ippvbuy.ippvBuyInfo.usPrice=%d\n",ippvbuy.ippvBuyInfo.ucProviderID,ippvbuy.ippvBuyInfo.usPrice);
			app_cas_api_on_event(DVB_CA_IPPV_POP_INFO,CAS_NAME, &ippvbuy, sizeof(desai_cas50_ippv_buy_st));

			break;
		}
		case MESSAGE_IPPV_OUTTIME_TYPE :
		{
#if defined (DVB_AD_TYPE_DS_FLAG)||defined (DVB_AD_TYPE_DS_FLAG41)
//			app_ds_ad_set_unauthorized_status(TRUE);
#endif
			DSCAS_Dbg("*********************MESSAGE_IPPV_OUTTIME_TYPE******************\n\n\n\n");
			break;
		}
		case MESSAGE_CARDISDUE_TYPE:
		{
			app_cas_api_on_event(DVB_CA_IPPV_SLOT_INFO,CAS_NAME, NULL, 0);
			break;
		}
		case MESSAGE_CONDITION_SEARCH_TYPE:

			app_cas_api_on_event(DVB_CA_CONDITION_SEARCH,CAS_NAME, NULL, 0);
			//app_win_auto_search();
			break;
		default :
			break;
	}



	return;


}

void DSCA_ShowEmail(ucMessageType,pucMessage)
{

	DSCAS_Dbg("%s	ucMessageType=0x%x\n",__FUNCTION__,ucMessageType);
	switch(ucMessageType)
	{
		case MESSAGE_NEWEMAIL_TYPE :
		{
			break;
		}
		case MESSAGE_OVEREMAIL_TYPE :
		{
			break;
		}
		case MESSAGE_HIDE_NEWEMAIL_TYPE :
		{
			break;
		}
		default :
			break;
	}
	emailflag.wStatus = ucMessageType;
	app_cas_api_on_event(DVB_CA_EMAIL_INFO,CAS_NAME, &emailflag, sizeof(desai_cas50_email_st));

}

void DSCA_ShowMessage(UINT8 ucMessageType, EN_CAS_SHOW_MSG_TYPE  enShowType, IN UINT8 * pucMessage)
{
	DSCAS_Dbg("ShowMessage MessageType=0x%0x  ShowType=%0x Message=%d\n",ucMessageType,enShowType,*pucMessage);
	if(ucMessageType == MESSAGE_OVEREMAIL_TYPE)
	{
		DSCA_ShowEmail(ucMessageType,pucMessage);
		return;
	}

	switch(enShowType)
	{
		//显示时间很短的消息
		case CAS_MSG_TYPE_NOTE_SHORT_TIME :
		{
			DSCA_ShowLong_Message(ucMessageType,enShowType);
			Show_time_short = 1;
			break;
		}
		//长期显示的消息
		case CAS_MSG_TYPE_NOTE_LONG_TIME :
		{
			app_play_clear_ca_msg();
			DSCA_ShowLong_Message(ucMessageType,enShowType);

			break;
		}
		//只是进行某种操作，不涉及到显示
		case CAS_MSG_TYPE_NOT_SHOW:
		{
			DSCA_ShowNone_Message(ucMessageType,pucMessage);
			break;
		}
		//显示指纹
		case CAS_MSG_TYPE_FINGER_SHOW:
		{
			DSCA_ShowFINGER_Message(ucMessageType,pucMessage);
			break;
		}
		//显示osd
		case CAS_MSG_TYPE_OSD:
		{
			DSCA_ShowOSD_Message(ucMessageType,pucMessage);
			break;
		}
		//对话框
		case CAS_MSG_TYPE_DIALOG :
		{
			app_play_clear_ca_msg();
			DSCA_ShowDialog_Message(ucMessageType,pucMessage);

			break;
		}
		//显示图片
		case CAS_MSG_TYPE_PICTURE :
		{
			DSCA_ShowEmail(ucMessageType,pucMessage);
			break;
		}
		//进度条
		case CAS_MSG_TYPE_PROGRESS:
		{
			if(ucMessageType == MESSAGE_CARD_UPDATE_BEGIN_TYPE)
			{
				smardcard_update.bStatus = DESAI_CAS50_CARD_UPDATE_START;
				smardcard_update.bProgress = 0;

				app_cas_api_on_event(DVB_CA_CARD_UPDATE_INFO,CAS_NAME, &smardcard_update, sizeof(smardcard_update));
			}
			else if(ucMessageType == MESSAGE_CARD_UPDATE_PROGRESS_TYPE && pucMessage)
			{
				smardcard_update.bStatus = DESAI_CAS50_CARD_UPDATE_PROCESS;
				smardcard_update.bProgress = *pucMessage;

				app_cas_api_on_event(DVB_CA_CARD_UPDATE_INFO,CAS_NAME, &smardcard_update, sizeof(smardcard_update));

			}
			else if(ucMessageType == MESSAGE_CARD_UPDATE_ERR_TYPE && pucMessage)
			{
				smardcard_update.bStatus = DESAI_CAS50_CARD_UPDATE_ERR;
				smardcard_update.bProgress = *pucMessage;

				app_cas_api_on_event(DVB_CA_CARD_UPDATE_INFO,CAS_NAME, &smardcard_update, sizeof(smardcard_update));
            }

			break;
		}
		default :
		{
			break;
		}
	}


}

void DSCA_SetCW(UINT16 usEcmPID, IN UINT8* pucOddKey, IN UINT8* pucEvenKey, IN UINT8* pucKeyLen)
{
	//CAS_Dbg("%s	usEcmPID=0x%x\n",__FUNCTION__,usEcmPID);
	//CAS_DUMP("pucOddKey",pucOddKey,*pucKeyLen);
	//CAS_DUMP("pucEvenKey",pucEvenKey,*pucKeyLen);
	app_porting_ca_descramble_set_cw(usEcmPID,pucOddKey,pucEvenKey,*pucKeyLen,0);

	return ;
}
void DSCA_StopCurrentProgram(void)
{
	DSCAS_Dbg("%s...\n",__FUNCTION__);
    //在第一次调用app_play_stop后，再次调用就无效了，因此需要直接调用发送stop消息
	app_play_stop();
	app_send_msg(GXMSG_PLAYER_STOP,PLAYER0);
	DSCAS_Dbg("%s, end.\n",__FUNCTION__);
}
void DSCA_PlayCurrentProgram(void)
{
	int32_t ppos = -1;
	DSCAS_Dbg("%s\n",__FUNCTION__);
	if(0== app_win_is_full_window())
	{	
		//进入全屏后才能播放
		return ;
	}
	
	app_prog_get_playing_pos_in_group((uint32_t *)&ppos);
    if (ppos>=0)
    {
	    app_play_video_audio_ds(ppos);
    }
}
handle_t g_SCCardSem = 0;

#if 0
extern handle_t smart_handle;

void DSCA_SendDataToSmartCard_Sub(OUT UINT8* pucSend, OUT UINT8 *pucSendLen, IN UINT8* pucReceive,
		IN UINT8 *pucReceiveLen, UINT32 uiTimeout, IN UINT8* pucStatusWord )
{
	//DSCAS_Dbg("%s\n",__FUNCTION__);
	INT32  len;
	UINT8 reply[256] = {0,};
	//printf("command %x,len %d\n",(INT32)pucSend,*pucSendLen);
	//CAS_DUMP("Sent", pucSend, *pucSendLen);
	if(pucSend==NULL||*pucSendLen==0)
	{
		*pucReceiveLen=0;
		*pucStatusWord =0;
		*(pucStatusWord+1) =0;
		printf("---illage para---\n");
		return;
	}

	DS_ASSERT(pucSend != NULL);
	//if(test_send==1)
	//*pucSendLen=0;
	//test_send=0;
	//card_Start_Send();
	len = GxSmc_SendCmd(smart_handle, pucSend, *pucSendLen);
	//GxCore_ThreadDelay(50000);

	printf("command send-len %d\n",len);

	len = GxSmc_GetReply(smart_handle, reply, 256);
	//card_Over_Send();
	//CAS_DUMP("Get", reply, len);
    printf("recv len %d\n",len);
	if(len  > 2)
	{
		memcpy(pucReceive,reply,len-2);
		*pucReceiveLen = len-2;
		memcpy(pucStatusWord,&reply[len-2],2);
	}
	else if(len == 2)
	{
		*pucReceiveLen = 0;
		memcpy(pucStatusWord,&reply[len-2],2);
	}
	else if(len== 1)
	{
		*pucReceiveLen = len;
		memcpy(pucReceive,reply,len);
		*pucStatusWord =0;
		*(pucStatusWord+1) =0;
	}
	else //if(len  == 0)
	{
		*pucReceiveLen=0;
		*pucStatusWord =0;
		*(pucStatusWord+1) =0;
	}
	//DUMP("Get", reply, len);
}

void DSCA_SendDataToSmartCard(IN UINT8* pucSend, IN UINT8 *pucSendLen, OUT UINT8* pucReceive,
											OUT UINT8 *pucReceiveLen, UINT32 uiTimeout, OUT UINT8* pucStatusWord )
{
	DSCAS_Dbg("%s\n",__FUNCTION__);
	if(!g_SCCardSem)
	{
    	GxCore_SemCreate(&g_SCCardSem, 1);
	}
	GxCore_SemWait(g_SCCardSem);
	if(*pucSendLen > 5 )
	{
		UINT8 RequestLength = 0;

		RequestLength = 5 ;
		//DSCAS_Dbg("Sent first five count\t");
		DSCA_SendDataToSmartCard_Sub(pucSend,&RequestLength,pucReceive,pucReceiveLen,uiTimeout,pucStatusWord);
		if(pucSend[1] == pucReceive[0])
		{
			//DSCAS_Dbg("Sent left count\t");
			RequestLength = (*pucSendLen) - 5 ;
			DSCA_SendDataToSmartCard_Sub(pucSend+5 ,&RequestLength,pucReceive,pucReceiveLen,
			uiTimeout,pucStatusWord);
		}
	}
	else
	{
		//DSCAS_Dbg("Sent 5 count\t");
		DSCA_SendDataToSmartCard_Sub(pucSend,pucSendLen,pucReceive,pucReceiveLen,uiTimeout,pucStatusWord);
		if(pucSend[1] == pucReceive[0])
		{
			//DSCAS_Dbg("Sent five count\t");
			(*pucReceiveLen)--;
			memcpy(&pucReceive[0],&pucReceive[1],*pucReceiveLen);
		}
	}
	GxCore_SemPost(g_SCCardSem);

	//	CAS_DUMP("[SC] DSCA_SendDataToSmartCard pucSend ", pucSend, *pucSendLen);
	//	CAS_DUMP("[SC] DSCA_SendDataToSmartCard pucReceive ", pucReceive, *pucReceiveLen);
	//	CAS_DUMP("[SC] DSCA_SendDataToSmartCard pucStatusWord ", pucStatusWord, 2);

	return;
}
#else
void DSCA_SendDataToSmartCard(IN UINT8* pucSend, IN UINT8 *pucSendLen, OUT UINT8* pucReceive,
											OUT UINT8 *pucReceiveLen, UINT32 uiTimeout, OUT UINT8* pucStatusWord )
{
	uint8_t reply[1024] = {0};
	uint8_t sc_result;
	uint16_t ReceiveLen;
	uint16_t sendLen;
	uint8_t i = 1;
	GxCore_SemWait(g_SCCardSem);

	//DSCAS_Dbg("%s>>sendLen=%d\n",__FUNCTION__,*pucSendLen);
	while(i--)
	{
		sendLen = *pucSendLen;
		sc_result =  app_porting_ca_smart_sc_apdu(pucSend,sendLen,reply,&ReceiveLen);
		if (TRUE == sc_result)
		{
			memcpy(pucReceive, reply, ReceiveLen - 2);
			*pucReceiveLen = ReceiveLen - 2;
			*pucStatusWord = reply[ReceiveLen - 2];
			*(pucStatusWord + 1) = reply[ReceiveLen - 1];
			//	CAS_DUMP("[SC] pucSend", pucSend, *pucSendLen);
			//	CAS_DUMP("[SC] pucReceive", pucReceive, *pucReceiveLen);
			//	CAS_DUMP("[SC] pucStatusWord", pucStatusWord, 2);
			GxCore_SemPost(g_SCCardSem);
			return;
		}
	}
	DSCA_SCReset(reply, &sc_result);
	GxCore_SemPost(g_SCCardSem);

}
#endif
BOOL DSCA_RegisterTask (IN INT8* pucName, UINT8 ucPriority, IN void* pTaskFun )
{
    	int         ret;
    	handle_t    handle;
	uint32_t priority = GXOS_DEFAULT_PRIORITY;

	if ((NULL == pucName)||(NULL == pTaskFun))
	{
		CAS_Dbg("STBCA_RegisterTask szName=%s  pTaskFun=0x%x\n",pucName,(unsigned int)pTaskFun);
		return 0;
	}
	if(0 == strcmp(pucName,"ecm"))
	{
		priority = GXOS_DEFAULT_PRIORITY-1;

	}

	printf("STBCA_RegisterTask szName=%s	ucPriority=%d\n",pucName,priority);

      ret = app_porting_ca_os_create_task(pucName, &handle, (void*)pTaskFun,
                  NULL, 1024*20, priority);

	return 1;
}

BOOL DSCA_SCReset(UINT8 *pucAtr, UINT8 *pucLen)
{
	DSCAS_Dbg("%s\n",__FUNCTION__);
	return app_desai_cas50_api_smart_reset(pucAtr,pucLen);
}
BOOL DSCA_SetFilter(EN_CA_DATA_ID enDataID, IN UINT8* pucFilter, IN UINT8* pucMask,
							UINT8 ucFilterLen, UINT16 usPID, UINT8 ucWaitSeconds)
{
	//uint16_t			filterID;
	//ca_filter_t* sfilter = NULL;
	ca_filter_t filter = {0};
	bool repeatFlag = TRUE;
	uint8_t 	match[18] = {0,};
	uint8_t 	mask[18] = {0,};
	uint8_t* pbyFilter = pucFilter;
	uint8_t* pbyMask = pucMask;
	uint16_t	 i=0;
	uint8_t    bRet = 0;
	uint8_t    TempenDataID = 0;
	play_scramble_para_t playpara = {0};
	app_play_get_playing_para(&playpara);

	//		CAS_Dbg("DSCA_SetFilter enDataID=0x%x,usPID=0x%x ucFilterLen=0x%x,ucWaitSeconds=%d\n",
	//		enDataID,usPID,ucFilterLen,ucWaitSeconds);
	if((enDataID == ECM_DATA_ID)
		&&(0x1FFF == playpara.p_ecm_pid_video)
		&&(0x1FFF == playpara.p_ecm_pid_audio)
		&&(0x1FFF == playpara.p_ecm_pid))
	{
		CAS_Dbg("%s.%d>>switching program,do not set ECM filter\n",__FUNCTION__,__LINE__);
		return 0;
	}

	if(pbyFilter == NULL || pbyMask == NULL )
	{
		CA_FAIL("DSCA_SetFilter pbyFilter=0x%x  pbyMask=0x%x\n",(unsigned int)pbyFilter,(unsigned int)pbyMask);
		return 0;
	}
	if(ucFilterLen > 16)
	{
		CA_FAIL("Filter depth is too long %d!!\n",ucFilterLen);
		return 0;
	}

/*	if (0x80 ==( 0x80&byReqID))
	{
		repeatFlag = FALSE;
	}*/

//		CAS_Dbg("DSCA_SetFilter enDataID=0x%x,usPID=0x%x ucFilterLen=0x%x,ucWaitSeconds=%d\n",
//		enDataID,usPID,ucFilterLen,ucWaitSeconds);

//	CAS_DUMP("BYSTBCA_SetStreamGuardFilter pbyFilter", pbyFilter , byLen);
//	CAS_DUMP("BYSTBCA_SetStreamGuardFilter pbyMask", pbyMask , byLen);


	for (i = 0; i< ucFilterLen; i++)
	{
		if (0 == i)
		{
			match[i]	 = pbyFilter[i];
			mask[i] = pbyMask[i];
		}
		else
		{
			match[i+2]	 = pbyFilter[i];
			mask[i+2] = pbyMask[i];
		}
	}
	TempenDataID = enDataID|0x80;
//	CAS_Dbg("DSCA_SetFilter TempenDataID=0x%x\n",TempenDataID);
	DSCA_StopFilter(enDataID, usPID);

	filter.byReqID = TempenDataID;
//	filter.crcFlag = FALSE;
	filter.crcFlag = TRUE;
	filter.filterLen = ucFilterLen+2;
	if(filter.filterLen <= 3)
	{
		filter.filterLen = 4;
	}
	memcpy(filter.match,match,ucFilterLen+2);
	memcpy(filter.mask,mask,ucFilterLen+2);
	filter.nWaitSeconds = ucWaitSeconds;
	filter.pid = usPID;
	filter.equalFlag = TRUE;
	filter.repeatFlag = repeatFlag;
	//filter.sw_filter_en = FALSE;

	filter.Callback = app_desai_cas50_filter_notify;
	filter.timeOutCallback = app_desai_cas50_filter_timeout_notify;
	bRet = app_porting_ca_demux_start_filter(&filter);

	if(bRet)
	{
		//CAS_Dbg("%s.%d>>exit\n",__FUNCTION__,__LINE__);
	}
	else
	{
		CAS_Dbg("%s.%d>>exit\n",__FUNCTION__,__LINE__);
	}
	return bRet;

}
BOOL DSCA_StopFilter(EN_CA_DATA_ID enDataID, UINT16 usPID)
{
	uint32_t filterId = 0;
	bool releaseChannel = TRUE;
	ca_filter_t sfilter = {0};
	uint8_t    TempenDataID = 0;

	/*
	* 查找对应channel id , filter id
	*/

	if(enDataID == EMM_DATA_ID)
	{
		//CAS_Dbg("DSCA_SetFilter enDataID=0x%x,usPID=0x%x \n",
		//enDataID,usPID);
	}
	TempenDataID = enDataID|0x80;

	for (filterId = 0; filterId< MAX_FILTER_COUNT;filterId++)
	{
		app_porting_ca_demux_get_by_filter_id(filterId,&sfilter);
		if (( 0 != sfilter.usedStatus)&&(0 != sfilter.handle)&&(0 != sfilter.channelhandle)
			/*&&(usPID == sfilter.pid)*/&&(TempenDataID == sfilter.byReqID))
		{
			if (0x10 == usPID)
				{
					releaseChannel = FALSE;
				}
			else
				{
					releaseChannel = TRUE;
				}

			  app_porting_ca_demux_release_filter( filterId, releaseChannel);
			  return TRUE;

		}
	}

	return TRUE;
}
BOOL DSCA_QueryProgram(UINT16 usNetWorkID, UINT16 usTransportStreamID, UINT16 usServiceID)
{
	DSCAS_Dbg("%s usNetWorkID=%d  usTransportStreamID =%d  usServiceID=%d\n\n",__FUNCTION__,usNetWorkID,usTransportStreamID,usServiceID);
	INT32 total_num=0;
	INT32 i=0;
	INT32 ret=0;
	GxBusPmDataProg prog_data;
	//GxBusPmViewInfo sysinfo;

	total_num=GxBus_PmProgNumGet();
	DSCAS_Dbg("total_num=%d\n\n\n",total_num);
	if(total_num == 0)
		return 0;
	for(i=0;i<total_num;i++)
	{
		ret=GxBus_PmProgGetByPos(i,1,(void*)&prog_data);
		if(ret !=-1)
		{
			DSCAS_Dbg("NetWorkID=%d  usTransportStreamID =%d  ServiceID=%d\n\n",prog_data.original_id,prog_data.ts_id,prog_data.service_id);
			if(/**/usNetWorkID ==prog_data.original_id && usTransportStreamID ==prog_data.ts_id && usServiceID ==prog_data.service_id)
			{
				pos_prog =i;
				DSCAS_Dbg("pos_prog = %d\n\n\n",pos_prog);
				return 1;
			}
		}
	}
	return 0;

}
BOOL DSCA_MsgQueueInit(IN INT8* pucName, UINT32* uiMsgQueueHandle, UINT32 uiMaxMsgLen, EN_CA_MSG_QUEUE_MODE enMode)
{
	DSCAS_Dbg("%s  pucName=%s\t",__FUNCTION__,pucName);
	static handle_t  queuedata;
 	if(GXCORE_SUCCESS==GxCore_QueueCreate(&queuedata,uiMaxMsgLen,sizeof(ST_CA_MSG_QUEUE *)))
	{
		*uiMsgQueueHandle=(UINT32)queuedata;
		DSCAS_Dbg("success!size=%d depth=%d queue=%u	enMode=%d\n",sizeof(ST_CA_MSG_QUEUE *),uiMaxMsgLen,*uiMsgQueueHandle,enMode);
		return TRUE;
	}
	else
	{
		DSCAS_Dbg(" failed !\n");
		return FALSE;
	}
}
BOOL DSCA_MsgQueueGetMsg(UINT32 uiMsgHandle, ST_CA_MSG_QUEUE* pstMsg, EN_CA_MSG_QUEUE_MODE enMode, UINT32 uiWaitMilliSecond)
{
	//DSCAS_Dbg("%s queue=%u",__FUNCTION__,uiMsgHandle);
	static uint32_t size_cpy;
	int ret;
	if(enMode==CA_MSG_QUEUE_NOWAIT)
		uiWaitMilliSecond=0;
	ret=GxCore_QueueGet(uiMsgHandle, pstMsg,sizeof(ST_CA_MSG_QUEUE *),&size_cpy,uiWaitMilliSecond);

	if(GXCORE_SUCCESS==ret && size_cpy !=0)
	{

		//DSCAS_Dbg(" success ! size=%d  size_cpy=%d\n",sizeof(ST_CA_MSG_QUEUE *),size_cpy);
		return TRUE;
	}
	else
	{
		//DSCAS_Dbg(" failed size_cpy=%d!\n",size_cpy);
		return FALSE;
	}
}
BOOL DSCA_MsgQueueSendMsg(UINT32 uiMsgHandle, ST_CA_MSG_QUEUE* pstMsg)
{
	//DSCAS_Dbg("%s\n",__FUNCTION__);

	if(GXCORE_SUCCESS == GxCore_QueuePut(uiMsgHandle,pstMsg,sizeof(ST_CA_MSG_QUEUE *),0))
	{
		//DSCAS_Dbg("MsgQueueSendMsg success !size=%d  \n",sizeof(ST_CA_MSG_QUEUE *));
		return TRUE;
	}
	else
	{
		//DSCAS_Dbg("MsgQueueSendMsg failure !\n");
		return FALSE;
	}
}
void dsca_show_osd_retry(void)
{
	if(ds_ca_osd_need_show)
	{
#if defined (DVB_AD_TYPE_DS_FLAG)||defined (DVB_AD_TYPE_DS_FLAG41)
		if(app_ds_ad_osd_display_status() != -1)
		{
			return;
		}
#endif
		app_cas_api_on_event(DVB_CA_ROLLING_INFO,CAS_NAME, &osddata, sizeof(desai_cas50_osd_data_st));

	}
}

