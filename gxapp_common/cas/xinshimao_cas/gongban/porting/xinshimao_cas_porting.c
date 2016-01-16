/*****************************************************************************
* 						   CONFIDENTIAL
*        Hangzhou GuoXin Science and Technology Co., Ltd.
*                      (C)2012, All right reserved
******************************************************************************

******************************************************************************
* File Name :	xinshimao_cas_porting.c
* Author    : 	zhouhm
* Project   :	goxceed dvbc
* Type      :
******************************************************************************
* Purpose   :	Ä£¿éÍ·ÎÄ¼þ
******************************************************************************
* Release History:
VERSION	Date			  AUTHOR         Description
1.0  	2013.09.20		  zhouhm 	 			creation
*****************************************************************************/
#include "xinshimao_ca.h"
#include "app_common_porting_ca_flash.h"
#include "app_common_porting_ca_demux.h"
#include "app_common_porting_ca_os.h"
#include "app_common_porting_ca_smart.h"
#include "app_common_porting_ca_descramble.h"
#include "app_common_prog.h"
#include "app_common_play.h"
#include "app_xinshimao_cas_api_demux.h"
#include "app_xinshimao_cas_api_pop.h"
#include "app_common_porting_stb_api.h"
#include "app_xinshimao_cas_api_smart.h"
#include "gxprogram_manage_berkeley.h"
#include "gxplayer_url.h"
#include "app_xinshimao_cas_api_rolling_message.h"

#define ONE_SECTOR_SIZE (64*1024)

char buf[1024]={0};
uint8_t reply[1024] = {0};

/* �ͷ�˽�����ݹ����� */
void XinShiMao_ReleasePrivateDataFilter( uint8_t  byReqID,
                                              uint16_t wPid )
{
	uint32_t filterId = 0;
	bool releaseChannel = TRUE;
	ca_filter_t sfilter = {0};

	/*
	* ���Ҷ�Ӧchannel id , filter id
	*/
	
//	CAS_Dbg("XinShiMao_ReleasePrivateDataFilter  byReqID=0x%x,  wPid=0x%x \n",
//		byReqID,wPid);

	for (filterId = 0; filterId< MAX_FILTER_COUNT;filterId++)
	{
		app_porting_ca_demux_get_by_filter_id(filterId,&sfilter);
		if (( 0 != sfilter.usedStatus)&&(0 != sfilter.handle)&&(0 != sfilter.channelhandle)
			&&(byReqID == sfilter.byReqID))
		{
			if (0x10 == wPid)
				{
					releaseChannel = FALSE;
				}
			else
				{
					releaseChannel = TRUE;
				}

			  app_porting_ca_demux_release_filter( filterId, releaseChannel);
			  return ;

		}
	}

	return ;
}


/* --------------------------------------ca event-------------------------------*/
/*Ìá ¹© Õß£º »ú¶¥ºÐ 

ÊäÈë²ÎÊý£º event ÎªÏûÏ¢ÀàÐÍ1: ÎªOSDÏûÏ¢£»4ÎªÓÊŒþÏûÏ¢
				2,3 Žó»­ÃæµÄÏûÏ¢ÌáÊŸ:param1 ÎªÏûÏ¢ÌáÊŸµÄÄÚÈÝ */
void  XinShiMao_EventHandle(UINT32 event,UINT32 param1,UINT32 param2,UINT32 param3)
{
    char buf[128];
	CAS_Dbg("XinShiMao_EventHandle -------------------- %d \n",(unsigned int)param1 );
    
	if(event == 1)
	{
		//osd
		CAS_Dbg(" osd osd  \n");
        char buf[CA_MAX_LEN_OSD];
        int len = (param2<sizeof(buf)-1)?param2:sizeof(buf)-1;
        memset(buf,0,sizeof(buf));
        memcpy(buf, (void *)param1,len );
        xinshimao_cas_sent_osdmsg(CA_OSD_TOP,buf);		
		return;
	}

	if(event == 4)
	{
		//new email
		CAS_Dbg(" email email  \n" );
		return;
	}

    if(event == 2 || event == 3)
    {
        CAS_Dbg(" msg \n" );
        switch(param1)
	    {
        case  SC_UPDATAINT:/*���ܿ����ݸ��£�����ϵ�!*/
            sprintf(buf,"%s","���ܿ����ݸ��£�����ϵ�!");
            xinshimao_cas_sent_msg(buf);
            break;
        case  SC_EXIRATION_REMINDER:/*��������!*/
            sprintf(buf,"���ܿ�ʣ��%d�쵽��!",(int)param2);
            xinshimao_cas_sent_msg(buf);
            break;
        default:
			break;
        }
		return;
    }

	switch(param1)
	{
		case     SC_NORMAL:/*����״̬*/
		case     SC_NO_CARD: /*δ�忨*/
		case     SC_NO_PPV_RIGHT:/*û��PPV��Ȩ*/
		case     SC_NO_PPC_RIGHT: /*û��PPC��Ȩ*/
		case     SC_PARENT_CTRL : /*�ҳ�����*/
		case     SC_NO_CONDITION :/*�����޲�*/
		case     SC_INVALID_CARD :   /*��Ч��*/
		case	 SC_TYPEERROR: /*��ĸ��ι��ʧ��,�������ܿ����ʹ���*/
		case     SC_ZONEERROR :/*�������*/
		case     SC_FEEDTIMEERROR: /*ι��ʱ�����*/
		case     SC_FEEDDATEERROR: /*ι�����ڴ���*/
		case     SC_FEEDSYSTEMTIMEERROR: /*ϵͳʱ��û����ȷ��ȡ,ι��ʧ��*/
		case     SC_ACSEVICEIDEERROR: /*���Ż�AC ����*/
		case     SC_CARDSTBNOTPAIR:
        //case     SC_UPDATAINT:
			if (MSG_POP_COMMON_END+param1+1 >=64)
				return ;
			
			if ( TRUE != app_play_get_msg_pop_type_state(MSG_POP_COMMON_END+param1+1))
				{
					app_play_clear_ca_msg();
					app_play_set_msg_pop_type(MSG_POP_COMMON_END+param1+1);
				}
			break;
		case SC_NEEDFEED:  /*�ӿ���Ҫ��ĸ����Ӧ,��ι���ӿ�*/
/*			{
				if(g_CasState!=param1)
				{
					g_CasState = param1;
					nc_ca_osd_send_msg(MSG_CAOSD_NOTIFY_CURRENT_STATE, (U32)param1,0);
				}
				break;
			}*/
			break;
		default:
			break;
	}

	return ;
}


/* --------------------------------------ca smart -------------------------------*/
INT32 XinShiMao_SC_DRV_Initialize(void)
{
	return CAS_SUCCESS;
}
//bCardNumber Ä¬ÈÏ0
void  XinShiMao_SC_DRV_ResetCard(UINT8 bCardNumber)
{
	uint8_t                     len=0;
    uint8_t               atr[256] = {0};

	CAS_Dbg("[SC] XinShiMao_SC_DRV_ResetCard!\n");

    app_xinshimao_cas_api_smart_reset(atr,&len);

	return ;

}
/*Ìá ¹© Õß£º »ú¶¥ºÐ¡£ 
ÊäÈë²ÎÊý£º 
		bCardNumber£º ÔÝÊ±±£Áô²»ÓÃ£» 
		bLength£º pabMessageµÄ³€¶È£» 
		pabMessage£º ·¢ËÍÃüÁîµÄÏûÏ¢ÖžÕë£» 
		pabResponse£º œÓÊÕÏìÓŠœá¹ûµÄÊýŸÝ¿éµÄÖžÕë£» 
		bRLength£º ÏìÓŠœá¹ûµÄÊýŸÝ¿é³€¶ÈÖžÕë¡£ 
Êä³ö²ÎÊý£º 
		pabResponse£º ÏìÓŠœá¹ûµÄÊýŸÝ¿é£» 
		bRLength£º ÏìÓŠœá¹ûµÄÊýŸÝ¿é³€¶È¡£ 
		pbSW1 ÖÇÄÜ¿š×ŽÌ¬×ÖœÚ1 
		pbSW2 ÖÇÄÜ¿š×ŽÌ¬×ÖœÚ2 
·µ »Ø Öµ£º 
		CAS_TRUE£º³É¹Š
		CAS_FALSE£ºÊ§°Ü */
INT32 XinShiMao_SC_DRV_SendDataEx(UINT8 bCardNumber,UINT8 bLength,
	                                                      UINT8 * pabMessage,
	                                                      UINT8 * pabResponse,
	                                                      UINT8 * bRLength,
	                                                      UINT8 * pbSW1,
	                                                      UINT8 * pbSW2)
{
	
	 uint8_t sc_result;
//	 uint16_t i = 0;
	 uint16_t reclen = 0;
	 uint16_t sendlen = bLength;
	 memset(reply,0,1024);
	 
#if 0
	 printf("XinShiMao_SC_DRV_SendDataEx bLength = %d pabMessage[4]=0x%x\n",
	 	bLength,pabMessage[4]);
	 for (i=0;i<bLength;i++)
	 	{
	 		printf(" 0x%02x",pabMessage[i]);
	 	}
	 
	 printf("\n");
#endif
	 
	 sc_result =  app_porting_ca_smart_sc_apdu(pabMessage,sendlen,reply,&reclen);
	 if (TRUE == sc_result)
		 {
#if 0
		 	printf("reclen = 0x%x\n",reclen);
			for (i=0;i<reclen;i++)
			   {
				   printf(" 0x%02x",reply[i]);
			   }
			printf("\n");
#endif

		 	if (reclen >= 2)
		 		{
		 			*bRLength = reclen-2;
			 		memcpy(pabResponse, reply, *bRLength);
					*pbSW1 = reply[ reclen-2];
					*pbSW2 = reply[ reclen-1];
#if 0
					printf("*pbSW1 = 0x%x\n",*pbSW1);
					printf("*pbSW2 = 0x%x\n",*pbSW2);
#endif
		 		}
			else
				{
					*bRLength = 0;
					CA_FAIL("XinShiMao_SC_DRV_SendDataEx 1 *bRLength = 0\n");
					return CAS_FALSE;
				}	 
		 }
	 else
	 	{
		 	CA_FAIL("XinShiMao_SC_DRV_SendDataEx 2 *bRLength = 0\n");
	 		return CAS_FALSE;
	 	}
	return CAS_TRUE;
	
}


/* --------------------------------------ca demux -------------------------------*/
/*Ìá ¹© Õß£º »ú¶¥ºÐ 
Ê¹ ÓÃ Õß£º CAÄ£¿é 
¹ŠÄÜÃèÊö£º»ñµÃµ±Ç°œÚÄ¿µÄServiceID 
ÊäÈë²ÎÊý£º ÎÞ
Êä³ö²ÎÊý£º ÎÞ¡£ 
·µ »Ø Öµ£º µ±Ç°œÚÄ¿µÄServiceID*/
UINT16 XinShiMao_GetCurr_ServiceID(void)
{
	GxBusPmDataProg Prog={0};
    uint32_t pos=0;
	UINT16 service_id = 0;

	if (app_prog_get_num_in_group()>0)
	{	
		app_prog_get_playing_pos_in_group(&pos);
		GxBus_PmProgGetByPos(pos,1,&Prog);
		service_id = Prog.service_id;
		return service_id;
	}
	return 0;
}
/*Ìá ¹© Õß£º »ú¶¥ºÐ 
Ê¹ ÓÃ Õß£º CAÄ£¿é 
¹ŠÄÜÃèÊö£º»ñµÃµ±Ç°µÄEmm Pid 
ÊäÈë²ÎÊý£º ÎÞ
Êä³ö²ÎÊý£º ÎÞ¡£ 
·µ »Ø Öµ£º µ±Ç°µÄEmm Pid*/
UINT16 XinShiMao_GetCurr_EmmID(void)
{
	return app_xinshimao_cas_api_get_emm_pid();
}
/*Ìá ¹© Õß£º »ú¶¥ºÐ 
Ê¹ ÓÃ Õß£º CAÄ£¿é 
¹ŠÄÜÃèÊö£º»ñµÃµ±Ç°µÄEcm Pid 
ÊäÈë²ÎÊý£º ÎÞ
Êä³ö²ÎÊý£º ÎÞ¡£ 
·µ »Ø Öµ£º µ±Ç°µÄEcm Pid*/
UINT16 XinShiMao_GetCurr_EcmID(void)
{
	return app_xinshimao_cas_api_get_ecm_pid();		
}
//²å°Î¿šÊ±ÍšÖªCA¿â£¬

/*Ìá ¹© Õß£º »ú¶¥ºÐ¡£ 
Ê¹ ÓÃ Õß£º CAÄ£¿é¡£ 
¹ŠÄÜÃèÊö£º Æô¶¯¹ýÂËÆ÷È¥œÓÊÕCAµÄÊýŸÝ¡£ 
ÊäÈë²ÎÊý£º 
		iRequestID     ,     ÎªÈçÏÂŒžžöÖµÖ®Ò»
						CA_STB_FILTER_1,
						CA_STB_FILTER_2
						CA_STB_FILTER_3
						CA_STB_FILTER_4
		pbFilterMatch1£¬ Èç¹ûÊÇ¹ýÂËEcmÊýŸÝ£¬ÔòžÃ²ÎÊýÎªEcm filterµÄÊýŸÝ£»
						 Èç¹ûÊÇ¹ýÂËEmmÊýŸÝ£¬ÔòžÃ²ÎÊýÎªEmm filter1µÄÊýŸÝ£»
		pbFilterMask1£¬  Èç¹ûÊÇ¹ýÂËEcmÊýŸÝ£¬ÔòžÃ²ÎÊýÎªEcm filterµÄmask£»
						 Èç¹ûÊÇ¹ýÂËEmmÊýŸÝ£¬ÔòžÃ²ÎÊýÎªEmm filter1µÄmask£»
		bLen£¬ filterµÄ³€¶È¡£ 
		wPid£¬ ÍšµÀµÄPIDÖµ¡£ 
		bWaitTime£¬ ÍšµÀ¹ýÆÚÊ±Œä£¬µ¥Î»ºÁÃë£¬³¬Ê±Ê±ŒäŒûºê¶šÒå ECM_FITLER_TIMEOUT, EMM_FITLER_TIMEOUT
	       if ( wPid <0 && wPid >0x1fff ) ÌáÊŸ»ú¶¥ºÐDEMMUXÐèÒªÊÍ·Å¹ýÂËÆ÷¿ÕŒä£¬EMM¿ÉÒÔ²»ÊÍ·Å£¬ECM±ØÐëÊÍ·Å
	       ECM¹ýÂËÊýŸÝTABILE_ID:0X80,0X81;emm ÊÚÈš¹ýÂËTABLE id:0X82;osd,email table id:0x83
·µ »Ø Öµ£º 
		SUCCESS£º³É¹Š£¬ 
		FAILURE£º Ê§°Ü¡£ */
INT32  XinShiMao_TableStart(UINT8 iRequestID,
						  const UINT8 * pbFilterMatch1,
	                                     const UINT8 * pbFilterMask1,
	                                     UINT8 bLen,
	                                     UINT16 wPid,
	                                     UINT8 bWaitTime)
{
	if ((CA_STB_FILTER_2 == iRequestID)||(CA_STB_FILTER_3 == iRequestID))
		{
			return CAS_SUCCESS;
		}




	if ( wPid > 0 && wPid < 0x1fff )
		{
			ca_filter_t filter = {0};
			bool repeatFlag = TRUE;
			uint8_t 	match[18] = {0,};
			uint8_t 	mask[18] = {0,};
			uint16_t	 i=0;
		
			if(pbFilterMatch1 == NULL || pbFilterMask1 == NULL )
			{
				return CAS_FAILTURE;
			}
		
			if (CA_STB_FILTER_4 == iRequestID)
			{
				repeatFlag = FALSE;
			}

//			printf("iRequestID=0x%x wPid=0x%x bWaitTime=%d\n",iRequestID,wPid,bWaitTime);
			for (i = 0; i< bLen; i++)
			{
				match[i]	 = pbFilterMatch1[i];
				mask[i] = pbFilterMask1[i];
			}
		
			XinShiMao_ReleasePrivateDataFilter(	iRequestID,
													  wPid );
		
			if (FALSE == repeatFlag)
			{
				for (i = MSG_POP_COMMON_END+SC_NO_CARD+1;i<=MSG_POP_COMMON_END+SC_CARDENTITLEREMAINTDAY;i++)
					{
						if (TRUE == app_play_get_msg_pop_type_state(i))
							{
								GxCore_ThreadDelay(20); 
								break;
							}
					}
			}
		
			filter.byReqID = iRequestID;
			filter.crcFlag = TRUE;
			filter.filterLen = bLen;
			memcpy(filter.match,match,bLen);
			memcpy(filter.mask,mask,bLen);
			if (0 != bWaitTime)
				{
					if (bWaitTime < 1000)
						filter.nWaitSeconds = 1;
					else
						filter.nWaitSeconds = bWaitTime/1000;
				}
			else
				{
					filter.nWaitSeconds = bWaitTime;	
				}
			filter.pid = wPid;
			filter.equalFlag = TRUE;
			filter.repeatFlag = repeatFlag;
			filter.Callback = app_xinshimao_cas_filter_notify;
			filter.timeOutCallback = app_xinshimao_cas_filter_timeout_notify;
			app_porting_ca_demux_start_filter(&filter);		
		}
	else
		{
			XinShiMao_ReleasePrivateDataFilter(iRequestID, wPid);
		}

	return CAS_SUCCESS;
}
/*Ìá ¹© Õß£º »ú¶¥ºÐ¡£ 
Ê¹ ÓÃ Õß£º CAÄ£¿é¡£ 
¹ŠÄÜÃèÊö£º ÉèÖÃœâÈÅÍšµÀµÄ¿ØÖÆ×Ö¡£ 
ÊäÈë²ÎÊý£º 
		wEcmPid£¬ ¿ØÖÆ×ÖµÄËùÔÚµÄecm°üµÄPIDÖµ¡£ 
		szOddKey£¬ Ææ¿ØÖÆ×ÖÊýŸÝµÄÖžÕë¡£ 
		szEvenKey£¬ ÅŒ¿ØÖÆ×ÖÊýŸÝµÄÖžÕë¡£ 
		bKeyLen£¬ ¿ØÖÆ×Ö³€¶È¡£ 
		bReservedFlag, ±£Áô¡£ 
*/
void XinShiMao_SetCW(UINT16 wEcmPid,const UINT8 * szOddKey,const UINT8 * szEvenKey,
	                              UINT8 bKeyLen,UINT8 bReservedFlag)
{
	app_porting_ca_descramble_set_cw(wEcmPid,szOddKey,szEvenKey,bKeyLen,bReservedFlag);
	return ;
}


/* --------------------------------------ca flash -------------------------------*/
/*Ìá ¹© Õß£º »ú¶¥ºÐ¡£ 
Ê¹ ÓÃ Õß£º CAÄ£¿é¡£ 
¹ŠÄÜÃèÊö£º »ñµÃ»ú¶¥ºÐÌá¹©žøCAÄ£¿é±£ŽæÐÅÏ¢µÄÆðÊŒµØÖ·ºÍ¿ÕŒäŽóÐ¡ 
ÊäÈë²ÎÊý£º 
Êä³ö²ÎÊý£º 
		lStartAddr »ú¶¥ºÐ·ÖÅä¿ÕŒäµÄÆðÊŒµØÖ· 
		lSize »ú¶¥ºÐ·ÖÅä¿ÕŒäµÄŽóÐ¡ 
·µ »Ø Öµ£º 
		TRUE ³É¹Š 
		FALSE Ê§°Ü */
CAS_BOOL XinShiMao_GetBuffer(UINT32 *lStartAddr,UINT32 * lSize)
{
	if ((NULL == lStartAddr)||(NULL ==lSize ))
		{
		  	CAS_Dbg("BYSTBCA_GetDataBufferAddr lSize=0x%x lStartAddr=0x%x\n",(unsigned int)lSize,(unsigned int)lStartAddr);		
			return CAS_FALSE;
		}

	
    *lStartAddr = ONE_SECTOR_SIZE;
	*lSize = ONE_SECTOR_SIZE;
	return CAS_TRUE;

}
/*Ìá ¹© Õß£º »ú¶¥ºÐ¡£ 
Ê¹ ÓÃ Õß£º CAÄ£¿é¡£ 
¹ŠÄÜÃèÊö£º ŽÓ»ú¶¥ºÐ·ÖÅä¿ÕŒäµÄÖž¶šÆðÊŒµØÖ·¶ÁÖž¶š³€¶ÈµÄÊýŸÝ¡£ 
ÊäÈë²ÎÊý£º 
		lStartAddr£º Òª¶ÁÈ¡ÊýŸÝµÄŽæŽ¢¿ÕŒäµÄµØÖ·¡£ 
		pbData£º ±»¶ÁÈ¡ÊýŸÝµÄŽæ·ÅµØÖ·ÖžÕë¡£ 
		nLen£º Òª¶ÁµÄÊýŸÝµÄ³€¶È 
Êä³ö²ÎÊý£º 
		pbData£º ±»¶Á³öµÄÊýŸÝ¡£ 
·µ »Ø Öµ£º 
		·µ»ØÊµŒÊ¶ÁµœµÄ×ÖœÚÊý*/
UINT32 XinShiMao_ReadBuffer(const UINT8 *lStartAddr,UINT8 *pData,INT32 nLen)
{
	uint32_t  Offset = (uint32_t)lStartAddr-ONE_SECTOR_SIZE;
	uint32_t len = nLen;
	
       if ((NULL ==lStartAddr )||(NULL == pData))
       	{
		  	CAS_Dbg("XinShiMao_ReadBuffer lStartAddr=0x%x pData=0x%x\n",(unsigned int)lStartAddr,(unsigned int)pData);		       	
			return -1;
       	}
	   
	if (Offset + nLen> ONE_SECTOR_SIZE)
		{
		  	CAS_Dbg("XinShiMao_ReadBuffer Offset=0x%x nLen=0x%x\n",(unsigned int)Offset,(unsigned int)nLen);		       	
			return -1;
		}
	
	app_porting_ca_flash_read_data(Offset,(uint8_t *)pData,&len);
	return len;
}

/*Ìá ¹© Õß£º »ú¶¥ºÐ¡£ 
Ê¹ ÓÃ Õß£º CAÄ£¿é¡£ 
¹ŠÄÜÃèÊö£º Ïò»ú¶¥ºÐµÄŽæŽ¢¿ÕŒäÐŽÐÅÏ¢ 
ÊäÈë²ÎÊý£º 
		lStartAddr£º ÒªÐŽµÄŽæŽ¢¿ÕŒäµÄÄ¿±êµØÖ·¡£ 
		pData£º ÒªÐŽµÄÊýŸÝ 
		nLen£º ÒªÐŽµÄÊýŸÝµÄ³€¶È 
Êä³ö²ÎÊý£º ÎÞ¡£ 
·µ »Ø Öµ£º 
		TRUE£º³É¹Š
		FALSE£º Ê§°Ü¡£*/
CAS_BOOL XinShiMao_WriteBuffer(const UINT32 lStartAddr,const UINT8 *pData,INT32 nLen)
{
	uint32_t  Offset = lStartAddr-ONE_SECTOR_SIZE;

	if (NULL == pData)
		{
			CAS_Dbg("XinShiMao_WriteBuffer pData=0x%x \n",(unsigned int)pData);		       	
			return CAS_FALSE;
		}
	
	if (Offset + nLen> ONE_SECTOR_SIZE)
		{
		  	CAS_Dbg("XinShiMao_WriteBuffer Offset=0x%x DataLen=0x%x\n",(unsigned int)Offset,(unsigned int)nLen);		       	
			return CAS_FALSE;
		}
	
	app_porting_ca_flash_write_data(Offset,(uint8_t *)pData,nLen);
	return CAS_TRUE;
//	return 0;
	
}

/* --------------------------------------ca os -------------------------------*/

/*Ìá ¹© Õß£º »ú¶¥ºÐ¡£ 
Ê¹ ÓÃ Õß£º CAÄ£¿é¡£ 
¹ŠÄÜÃèÊö£º ŽŽœšÈÎÎñ 
ÊäÈë²ÎÊý£º 
		name[]£¬ 4žö×ÖœÚµÄÈÎÎñÃû³Æ¡£ 
		stackSize£¬ ÈÎÎñËùÊ¹ÓÃµÄ¶ÑÕ»µÄŽóÐ¡¡£ 
		entryPoint£¬ ÈÎÎñµÄÈë¿ÚµØÖ·¡£ 
		priority£¬ ÈÎÎñµÄÓÅÏÈŒ¶¡£ 
		arg1£¬ Ž«µÝžøÈÎÎñµÄµÚÒ»žö²ÎÊý¡£ 
		arg2£¬ Ž«µÝžøÈÎÎñµÄµÚ¶þžö²ÎÊý¡£ taskId£¬ ÈÎÎñµÄID¡£ 
Êä³ö²ÎÊý£º ÎÞ¡£ 
·µ »Ø Öµ£º 
		SUCCESS£º³É¹Š£»
		FAILURE£º ·¢ÉúŽíÎó¡£*/
 INT32 XinShiMao_OSPTaskCreate(char name[],UINT32 stackSize,
	                                   void (*entryPoint)(void*),
	                                   INT32 priority,
	                                   UINT32 arg1,
	                                   UINT32 arg2,
	                                   UINT32 * taskId)
{
    int         ret;
    handle_t    handle;

	if (NULL == entryPoint)
	{
		return CAS_FAILTURE;
	}

//	printf("XinShiMao_OSPTaskCreate szName=%s stackSize=%d\n",name,stackSize);


    ret = app_porting_ca_os_create_task(name, &handle, entryPoint,
                              NULL, stackSize, GXOS_DEFAULT_PRIORITY);
	*taskId = handle;


	if (ret != GXCORE_SUCCESS)
	{
		CA_FAIL("XinShiMao_OSPTaskCreate return error = 0x%x\n", ret);
		return CAS_FAILTURE;
	}
	return CAS_SUCCESS;

}
INT32 XinShiMao_OSPTaskTemporarySleep(UINT32 milliSecsToWait)
{
	app_porting_ca_os_sleep(milliSecsToWait);
	return CAS_SUCCESS;
}
/*Ìá ¹© Õß£º »ú¶¥ºÐ¡£ 
Ê¹ ÓÃ Õß£º CAÄ£¿é¡£ 
¹ŠÄÜÃèÊö£º ŽŽœšÒ»žöÏûÏ¢¶ÓÁÐ¡£ 
ÊäÈë²ÎÊý£º 
		name[]£¬ 4žö×ÖœÚµÄ¶ÓÁÐÃû³Æ¡£ 
		maxQueueLength£¬ÏûÏ¢¶ÓÁÐÖÐ¿ÉÒÔŽæ·ÅµÄÏûÏ¢µÄÊýÁ¿¡£µ±ÏûÏ¢¶ÓÁÐÖÐžÃÊýÁ¿Žïµœ žÃÊýÁ¿Ê±£¬ÔÙÍùžÃÏûÏ¢¶ÓÁÐ·¢ÏûÏ¢œ«»áÊ§°Ü¡£ 
		taskWaitMode:¿ÉÒÔ²»¹Ü
		queueId£¬ ÏûÏ¢¶ÓÁÐµÄID¡£ 
Êä³ö²ÎÊý£º ÎÞ¡£ 
·µ »Ø Öµ£º 
		SUCCESS£º³É¹Š£»
		FAILURE£º ·¢ÉúŽíÎó*/

INT32 XinShiMao_OSPQueueCreate(char name[],UINT32 maxQueueLength,UINT32 taskWaitMode,UINT32 * queueId)
{
	handle_t  queuedata = 0;
 	if(GXCORE_SUCCESS==GxCore_QueueCreate(&queuedata,/*maxQueueLength*/20,sizeof(XinShiMao_Queue_message)))
	{
		*queueId=(UINT32)queuedata;
	}
	else
	{
		CAS_Dbg("XinShiMao_OSPQueueCreate failed !\n");
	}
	return CAS_SUCCESS;
//	return CAS_TRUE;
}
/*Ìá ¹© Õß£º »ú¶¥ºÐ¡£ 
Ê¹ ÓÃ Õß£º CAÄ£¿é¡£ 
¹ŠÄÜÃèÊö£º 
ÊäÈë²ÎÊý£º 
		queueId£¬ ËùÒªÈ¡µÃµÄÏûÏ¢µÄID¡£ 
		messsage£¬ ÎªÏûÏ¢µÄžñÊœ¡£²ÎÕÕXINSHIMAO_QUEUE_MESSAGE¡£ 
		waitMode:Ä¿Ç°Ö»ÓÃµœ£¬µÈŽýÖ±µœµÃµœÏûÏ¢ 

		SUCCESS£º³É¹Š£»
		FAILURE£º ·¢ÉúŽíÎó¡£*/
INT32 XinShiMao_OSPQueueGetMessage(UINT32 queueId,XinShiMao_Queue_message * message,UINT32 waitMode,UINT32 milliSecsToWait)
{
	uint32_t    sz=0;
	int32_t     timeout;
	int         ret;

#if defined(ECOS_OS)
	timeout = -1;
#elif defined(LINUX_OS)
	timeout = 0;
#endif

	ret = GxCore_QueueGet(queueId, message, sizeof(XinShiMao_Queue_message), &sz,
		timeout);

	if (ret == GXCORE_SUCCESS)
	{
		if (0 == sz )
		{
			CAS_Dbg("XinShiMao_OSPQueueGetMessage sz = 0...\n");
			return CAS_FAILTURE;
		}
		
		return CAS_SUCCESS;
	}
	else
	{
		CAS_Dbg("XinShiMao_OSPQueueGetMessage Return value is error...\n");
		return CAS_FAILTURE;
	}
	return CAS_SUCCESS;

}
/*Ìá ¹© Õß£º »ú¶¥ºÐ¡£ 
Ê¹ ÓÃ Õß£º CAÄ£¿é¡£ 
¹ŠÄÜÃèÊö£º ·¢ËÍÏûÏ¢µœÏûÏ¢¶ÓÁÐ¡£ 
ÊäÈë²ÎÊý£º 
		queueId£¬ ÏûÏ¢¶ÓÁÐID¡£ 
		message£¬ Òª·¢ËÍµÄÏûÏ¢¡£ÆäžñÊœŒûXINSHIMAO_QUEUE_MESSAGEœá¹¹¡£ 
Êä³ö²ÎÊý£º ÎÞ¡£ 
·µ »Ø Öµ£º 
		SUCCESS£º³É¹Š£»
		FAILURE£º ·¢ÉúŽíÎó¡£*/
INT32 XinShiMao_OSPQueueSendMessage(UINT32 queueId,XinShiMao_Queue_message * message)
{
	int   ret ;

	if(message == NULL)
	{
		CAS_Dbg("XinShiMao_OSPQueueSendMessage Parameter is NULL\n");
		return CAS_FAILTURE;
	}

	ret = GxCore_QueuePut(queueId, message, sizeof(XinShiMao_Queue_message),0);

	if(ret == GXCORE_SUCCESS )
	{
		return CAS_SUCCESS;
	}
	else
	{
		CAS_Dbg("XinShiMao_OSPQueueSendMessage pipeID=0x%x Error\n", (unsigned int)queueId);
		return CAS_FAILTURE;
	}
	return CAS_SUCCESS;


}
UINT32 XinShiMao_Printf(const char * fmt,...)
{
	return CAS_TRUE;

	va_list args;
	int i;

	va_start(args,fmt);
	i = Vsnprintf(buf,sizeof(buf), fmt, args);
	va_end(args);

	printf("%s", buf);
	return CAS_TRUE;
}
void XinShiMao_memcpy(void* pDestBuf,const void* pSrcBuf,UINT32  wSize)
{
    if(pDestBuf && pSrcBuf && wSize>0)
        memcpy( pDestBuf, pSrcBuf, wSize );
}
void XinShiMao_memset(void* pDestBuf,UINT8 c,UINT32 wSize)
{
    memset( pDestBuf, c, wSize );
}
INT32  XinShiMao_memcmp(const void *buf1, const void *buf2, UINT32 count)
{
	return memcmp(buf1, buf2, count);
}
//ÉèÖÃ»ú¶¥ºÐÊ±Œä£¬»ú¶¥ºÐÎÞÐè¹ýÂËTDT±í£¬ÓÉCAÀŽÌá¹©Ê±Œä»ù×Œ
void	XinShiMao_SetStbTime(TXinShiMao_DATETIME* ca_time)
{
	return;
}

void	XinShiMao_UpStbId(char* p_stbid)
{
	return;
}

UINT32   XinShiMao_OS_CreateSemaphore(UINT32 ulInitialCount)  /*�����ź���*/
{
    handle_t semaphore = 0;	
    app_porting_ca_os_sem_create(&semaphore,ulInitialCount);
    return semaphore;
}
void  XinShiMao_OS_WaitSemaphore(UINT32 ulSemaphore)       /*�ȴ��ź���*/
{
    app_porting_ca_os_sem_wait(ulSemaphore);
}
void  XinShiMao_OS_SignalSemaphore(UINT32 ulSemaphore)      /*�ͷ��ź���*/
{
    app_porting_ca_os_sem_signal(ulSemaphore);
}



