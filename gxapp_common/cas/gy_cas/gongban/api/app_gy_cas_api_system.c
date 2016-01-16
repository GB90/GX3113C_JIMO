/*****************************************************************************
* 						   CONFIDENTIAL
*        Hangzhou GuoXin Science and Technology Co., Ltd.
*                      (C)2012, All right reserved
******************************************************************************

******************************************************************************
* File Name :	app_gy_cas_system.c
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

#include "app_gy_cas_api_pop.h"
#include "gyca_def.h"
#include "gyca_pub.h"
#include "gyca_osp.h"
#include "gyca_interface.h"
#include "gyca_porting.h"
#include "app_gy_cas_api_email.h"

#define GY_CA_HINT_INFO_LEN 	100

static gy_cas_rolling_message_st  rollingmsg = {0};
static gy_cas_finger_data_st  fingerdata = {0};
static gy_cas_ippv_buy_st ippvbuy;
static gy_cas_exigent_data_st gy_exigent_data={0};
static T_CARemoteControlInfo gy_ctrldata={0};
char EventStr[GY_CA_HINT_INFO_LEN];

GYS32 EventCallbackFunc(GYU32 evn,GYU32 param1,GYU32 param2, GYU32 param3)
{
	switch(evn)
	{
		case GYCA_NOTICE_SHOW:
		{
			memset(EventStr,0,GY_CA_HINT_INFO_LEN);
			if(param2 > GY_CA_HINT_INFO_LEN)
			{
				memcpy(EventStr,(GYS8 *)param1,GY_CA_HINT_INFO_LEN);
				EventStr[GY_CA_HINT_INFO_LEN-1] = '\0';
			}
			else
			{
				memcpy((void *)EventStr,(void *)param1,param2);
			}
			CAS_Dbg("GYCA_NOTICE_SHOW[%s]\n",EventStr);
			if ( TRUE == app_play_get_msg_pop_type_state(MSG_POP_COMMON_END+evn+1))
			{
				app_play_clear_ca_msg();
			}
			app_play_set_msg_pop_type(MSG_POP_COMMON_END+evn+1);
		}
		break;
		case GYCA_NOTICE_ERASE:
		{
			CAS_Dbg("GYCA_NOTICE_ERASE\n");
			if ( TRUE != app_play_get_msg_pop_type_state(MSG_POP_COMMON_END+evn+1))
			{
				app_play_clear_ca_msg();
				app_play_set_msg_pop_type(MSG_POP_COMMON_END+evn+1);
			}
		}
		break; 
		case GYCA_OSD_INFO:
		{
			T_CAOSDInfo  *CAOSDInfo = (T_CAOSDInfo*)param1;
			if(CAOSDInfo == NULL)
			{
				break;
			}
			// TODO:
		//	break;
			CAS_Dbg("GYCA_OSD_INFO,pos=%d,cycle=%d,len=%d,str=[%s]\n",
				CAOSDInfo->m_byOSD_Position,
				CAOSDInfo->m_wOSD_Show_Cycle,
				CAOSDInfo->m_byOSD_Text_length,
				CAOSDInfo->m_szOSD_Text);
			memset(&rollingmsg,0,sizeof(gy_cas_rolling_message_st))	;
			memcpy(rollingmsg.content,CAOSDInfo->m_szOSD_Text,150);
			if(CAOSDInfo->m_byOSD_Text_length < 150)
			{
				rollingmsg.content[CAOSDInfo->m_byOSD_Text_length] = '\0'; 
			}
			rollingmsg.byStyle = CAOSDInfo->m_byOSD_Position;
			rollingmsg.dwTime = CAOSDInfo->m_wOSD_Show_Cycle;
			rollingmsg.wStatus = TRUE;
			app_cas_api_on_event(DVB_CA_ROLLING_INFO,CAS_NAME, &rollingmsg, sizeof(gy_cas_rolling_message_st));
		}
		break;
		case GYCA_OSD_ERASE:
		{
			CAS_Dbg("GYCA_OSD_ERASE\n");
			// TODO:
		//	break;
			memset(&rollingmsg,0,sizeof(gy_cas_rolling_message_st)) ;
			rollingmsg.byStyle = param1;
			rollingmsg.wStatus = FALSE;
			app_cas_api_on_event(DVB_CA_ROLLING_INFO,CAS_NAME, &rollingmsg, sizeof(gy_cas_rolling_message_st));
		}
		break;
		case GYCA_RESTART_CHAN:
		{
			CAS_Dbg("GYCA_RESTART_CHAN\n");
			/*Replay current program*/
			app_play_reset_play_timer(0);
		}
		break;
		case GYCA_MAIL_INFO:
		{
			T_CAMailInfo *mailInfo = (T_CAMailInfo *)param1;
			// TODO:
			break;
			CAS_Dbg("GYCA_MAIL_INFO\n");
			if(mailInfo == NULL)
			{
				break;
			}
			app_gy_cas_api_add_email(mailInfo);
		}
		break;
		case GYCA_FINGER_SHOW:
		case GYCA_FINGER_ERASE:
		{
			T_CAFingerInfo *FingerInfo = (T_CAFingerInfo*)param1;

			if (evn == GYCA_FINGER_ERASE)
			{
				CAS_Dbg("GYCA_FINGER_ERASE\n");
				fingerdata.wStatus = FALSE; 			
			}
			else
			{
				CAS_Dbg("GYCA_FINGER_SHOW\n");
				fingerdata.wStatus = TRUE;
				memset(fingerdata.str,0x00,100);
				memcpy(fingerdata.str,FingerInfo->m_szFinger,FingerInfo->m_byFingerInfoLen);
			}
			app_cas_api_on_event(DVB_CA_FINGER_INFO,CAS_NAME, &fingerdata, sizeof(gy_cas_finger_data_st));
		}
		break;
		case GYCA_EXECUTE_OP_INFO:
		{
			CAS_Dbg("GYCA_EXECUTE_OP_INFO\n");
		}
		break;
		case GYCA_SW_UPDATE_INFO:
		{
			T_CASoftwareUpdateInfo *updateInfo = (T_CASoftwareUpdateInfo *)param1;
			CAS_Dbg("GYCA_SW_UPDATE_INFO\n");
			if(updateInfo == NULL)
			{
				break;
			}
		}
		break;
		case GYCA_TRANSPARENT_INFO:/*Force switch program*/
		{
			CAS_Dbg("GYCA_TRANSPARENT_INFO\n");
			if(!param1)
			{
				break;
			}
			memset(&gy_ctrldata,0,sizeof(T_CARemoteControlInfo));
			memcpy(&gy_ctrldata,(T_CARemoteControlInfo *)param1,sizeof(T_CARemoteControlInfo));
			gy_exigent_data.wStatus = TRUE;
			gy_exigent_data.buffer = (int32_t *)&gy_ctrldata;
			app_cas_api_on_event(DVB_CA_EMERGENCY_INFO,CAS_NAME, &gy_exigent_data,sizeof(gy_cas_exigent_data_st));
		}
		break;
		case GYCA_IPPV_INFO:
		{
			T_CA_IPPVInfo * IPPVInfo = (T_CA_IPPVInfo *)param1;
			CAS_Dbg("GYCA_IPPV_INFO\n");
			if(IPPVInfo == NULL)
			{
				break;
			}
			memset(&ippvbuy,0,sizeof(gy_cas_ippv_buy_st));
			ippvbuy.wStatus = TRUE;
			memcpy(&ippvbuy.ippvBuyInfo,IPPVInfo,sizeof(T_CA_IPPVInfo));
			app_cas_api_on_event(DVB_CA_IPPV_POP_INFO, CAS_NAME, &ippvbuy, sizeof(gy_cas_ippv_buy_st));			
		}
		break;
		case GYCA_ENTITLEMENT_CHANGED:
		{
			CAS_Dbg("GYCA_ENTITLEMENT_CHANGED\n");
		}
		break;
		case GYCA_SMC_STATUS:
		{
			CAS_Dbg("GYCA_SMC_STATUS\n");
		}
		break;
		default:
			CAS_Dbg("EventCallbackFunc:env=%d,param1=0x%x,param2=0x%x,param3=0x%x\n",evn,param1,param2,param3);
		break;
	}
	return GYSUCCESS;	
}
/* 消隐当前会话框 */
/**********************************************************************************************
** 函数名称：   GYCA_HideIppvDlg
***********************************************************************************************
**     提供者：           机顶盒。
**     使用者：           CAM模块。
**     何时使用：      CAM需要消隐当前会话框的时候
**     功能描述：      消隐当前会话框
**     输入参数：      pEcmInfo        当前节目的CAServiceInfo。
**     输出参数：     无。                                                
**     返回值：           无。                                  
**     其他说明：     无
***********************************************************************************************/
void GYCA_HideIppvDlg(T_CAServiceInfo* pEcmInfo)
{
	ippvbuy.wStatus = FALSE;
	app_cas_api_on_event(DVB_CA_IPPV_POP_INFO, CAS_NAME, &ippvbuy, sizeof(gy_cas_ippv_buy_st)); 		
}

int GYCA_Init(void)
{
	char caMatchData[]="GX3201-DVBC";
	char caMatchVer[]="V12.01";
	char * menu_language = NULL;
	
	if(GYSUCCESS !=GYCA_Initialize(GXOS_DEFAULT_PRIORITY))
	{
		CA_FAIL("\n ************ GYCA_Init: GYCA_Initialize Error ******************\n");
		return FALSE;
	}

	if (GYSUCCESS!=GYCA_RegisterAvaegisAPPEventCallback(EventCallbackFunc))
	{
		CA_FAIL("\n #### GYCA_RegisterAvaegisAPPEventCallback  Error ###\n");
		return FALSE;
	}
	menu_language = app_flash_get_config_osd_language();
	if(menu_language)
	{
		if(strcmp(menu_language,"English") == 0)
		{
			GYCA_SelectLanguage(1);
		}
		else if(strcmp(menu_language,"Chinese") == 0)
		{
			GYCA_SelectLanguage(0);
		}
	}

	if(GYSUCCESS !=GYCA_MachineInfoRegister((GYU8 *)caMatchData,
		sizeof(caMatchData),(GYU8 *)caMatchVer,sizeof(caMatchVer)))
	{
		CA_FAIL("\n ************ GYCA_Init: GYCA_MachineInfoRegister Error ******************\n");
		return FALSE;
	}

	return TRUE;
}




