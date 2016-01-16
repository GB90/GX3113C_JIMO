/*****************************************************************************
* 						   CONFIDENTIAL
*        Hangzhou GuoXin Science and Technology Co., Ltd.
*                      (C)2012, All right reserved
******************************************************************************

******************************************************************************
* File Name :	app_ca_api.c
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

#include "gyca_def.h"
#include "gyca_pub.h"
#include "gyca_osp.h"
#include "gyca_interface.h"
#include "gyca_porting.h"

#include "app_gy_cas_api_pin.h"
#include "app_gy_cas_api_smart.h"
#include "app_gy_cas_api_demux.h"
#include "app_gy_cas_api_baseinfo.h"


int32_t app_gy_cas_api_init_baseinfo_data(void)
{
	return 0;
}



char * app_gy_cas_api_get_baseinfo_data(ca_get_date_t *data)
{
	unsigned int Pos = 0 ;                             
	unsigned int ID = 0;  
	int Err=GYCA_OK;
    static char  s_text_buffer[70]={0};

   
	if (NULL == data)
		return NULL;

	Pos = data->pos;
	ID = data->ID;

	memset(s_text_buffer,0,sizeof(s_text_buffer));

	switch(ID)
	{
		case GY_BASEINFO_OPERATE_ID:
		{
			T_CA_OperatorInfo tInfo;
			memset(&tInfo,0x00,sizeof(T_CA_OperatorInfo));
			Err = GYCA_GetOperatorInfo(&tInfo);
			if(Err == GYCA_OK)
			{
				sprintf(s_text_buffer,"%s",tInfo.m_szSPName);
			}
		}break;
		case GY_BASEINFO_CARD_STATUS_ID:
		{
			int status;
			
			status = GYCA_GetSMCEntitleInfo();

			switch(status)
			{
				case USER_ENS_UNKNOWNED:	/* 用户状态未知，可能因为卡错误或者未插卡*/
					sprintf(s_text_buffer,"%s","未知");
					break;
				case USER_ENS_PENDING:			/* 卡尚未被激活*/
					sprintf(s_text_buffer,"%s","未激活");
					break;
				case USER_ENS_ACTIVATE:		/* 正常激活的卡*/
					sprintf(s_text_buffer,"%s","正常激活的卡");
					break;
				case USER_ENS_ARREARAGE:		/* 欠费催缴*/
					sprintf(s_text_buffer,"%s","欠费催缴");
					break;
				case USER_ENS_DEBT_STOP:		/* 用户欠费，几经停用*/
					sprintf(s_text_buffer,"%s","已经停用");
					break;
				case USER_ENS_CALL_FOR_STOP:	/* 用户报停*/
					sprintf(s_text_buffer,"%s","用户报停");
					break;
				case USER_ENS_CANCELED:			/* 卡已经注销*/
					sprintf(s_text_buffer,"%s","已经注销");
					break;
				default:
					break;
			}
		}break;
		case GY_BASEINFO_AREACORD_ID:
		{
			Err = GYCA_GetCardArea();
			sprintf(s_text_buffer,"%d",Err);
		}break;
		case GY_BASEINFO_USER_TYPE_ID:
		{
			Err = GYCA_GetSMCUserType();
			switch(Err)
			{
				case SMC_NORMAL_USER:    	/* 普通用户*/
					sprintf(s_text_buffer,"%s","普通用户");
					break;
				case SMC_TEST_USER:		/* 测试用户*/
					sprintf(s_text_buffer,"%s","测试用户");
					break;
				case SMC_GROUP_USER:		/* 集团用户*/
					sprintf(s_text_buffer,"%s","集团用户");
					break;
				case SMC_DEMO_USER:        /* 演示用户*/
					sprintf(s_text_buffer,"%s","演示用户");
					break;
				case SMC_FOREIGN_USER:	/* 境外用户*/
					sprintf(s_text_buffer,"%s"," 境外用户");
					break;
				default:
					break;				
			}
		}break;
		case GY_BASEINFO_CARD_TYPE_ID:
		{
			unsigned char card;
			
			card = GYCA_IsSonCard();

			if(GYTRUE == card)
			{
				sprintf(s_text_buffer,"%s","子卡");
			}
			else
			{
				sprintf(s_text_buffer,"%s","主卡");
			}
		}break;
		case GY_BASEINFO_CARD_ISSUE_TIMER_ID:
		{
			unsigned char pbIssueDate[5]={0},pbExpireDate[5]={0};
			
			Err = GYCA_GetSMCIssueInfoEx(pbIssueDate,pbExpireDate);

			if(Err == GYCA_OK)
			{
				sprintf(s_text_buffer,"%d-%d-%d",(pbIssueDate[0])|(pbIssueDate[1]<<8),pbIssueDate[2],pbIssueDate[3]);
			}	
		}break;
		case GY_BASEINFO_CARD_EXPIRE_TIMER_ID:
		{
			unsigned char pbIssueDate[5]={0},pbExpireDate[5]={0};
			
			Err = GYCA_GetSMCIssueInfoEx(pbIssueDate,pbExpireDate);

			if(Err == GYCA_OK)
			{
				sprintf(s_text_buffer,"%d-%d-%d",(pbExpireDate[0])|(pbExpireDate[1]<<8),pbExpireDate[2],pbExpireDate[3]);
			}	
		}break;
		case GY_BASEINFO_CARD_SN_ID:
		{
			unsigned char pbCardID[10]={0};
			
			Err =  GYCA_GetSMCID(pbCardID);
			
			if(Err == GYCA_OK)
			{
				int i;
				for(i=0;i<8;i++)
				{
					s_text_buffer[i*2]= (char)(((pbCardID[i]>>4)&0x0F) + 0x30);
					s_text_buffer[i*2+1]=(char)((pbCardID[i]&0x0F) + 0x30);
				}
			}
		}break;
		case GY_BASEINFO_CARD_MONEY_ID:
		{
			Err = GYCA_GetSMCTokenInfo();

			if(Err>=0)
			{
				sprintf(s_text_buffer,"%d.%d 元",Err/100,Err%100);
			}
		}break;
		case GY_BASEINFO_VERSION_ID:
		{
			unsigned char *pVer=NULL;
			
			GYCA_GetVersionInfo(&pVer);

			if(pVer != NULL)
			{
				sprintf(s_text_buffer,"%s",pVer);
			}
		}break;
		default:
			break;
	}

	
	return s_text_buffer;

}






