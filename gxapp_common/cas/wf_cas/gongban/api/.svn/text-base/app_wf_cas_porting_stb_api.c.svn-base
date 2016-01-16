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
1.0  	2012.12.02		  zhouhm 	 			creation
*****************************************************************************/
#include <gxtype.h>
#include "gxfrontend.h"
#include "gxapp_sys_config.h"
#include "app_common_play.h"
#include "app_common_table_pmt.h"
#include "app_common_porting_stb_api.h"
#include "app_common_porting_ca_flash.h"
#include "app_common_porting_ca_demux.h"
#include "app_common_porting_ca_descramble.h"
#include "app_common_search.h"
#include "app_wf_cas_api_smart.h"
#include "app_wf_cas_api_demux.h"
#include "app_common_porting_ca_smart.h"

#include "wanfacas.h"

#include "app_wf_cas_api_entitle.h"
#include "app_wf_cas_api_baseinfo.h"
#include "app_wf_cas_api_email.h"
#include "app_wf_cas_api_rolling_message.h"
//#include "app_wf_cas_api_pptv_buy.h"
/*
#include "app_by_cas_api_demux.h"
#include "app_by_cas_api_smart.h"
#include "app_by_cas_api_entitle.h"
#include "app_by_cas_api_email.h"
#include "app_by_cas_api_baseinfo.h"
#include "app_by_cas_api_lock_service.h"
#include "app_by_cas_api_rolling_message.h"
#include "app_by_cas_api_finger.h"
*/
extern void app_wf_cas_hide_ca_prompt(void);
extern void app_wf_cas_show_ca_prompt(char* context);

extern void app_open_mail_area(void);

extern int32_t app_wf_cas_gxmsg_ca_on_event_pptv_buy(GxMsgProperty0_OnEvent* event);


uint8_t app_wf_cas_api_init(void)
{
	U32 nRet = 0;
	WFSTB_Identity_t	wfca_stb_identity;
//	app_porting_ca_flash_init(64*1024);
	app_open_mail_area();
	app_porting_ca_demux_init();
	app_porting_ca_descramble_init();
	app_porting_ca_create_filter_task();

	wfca_stb_identity.Delivery = TRUE;
	wfca_stb_identity.Identity[0] = 0x0d;
	wfca_stb_identity.Identity[1] = 0x67;
	wfca_stb_identity.Identity[2] = 0x24;
	wfca_stb_identity.Identity[3] = 0x84;
	wfca_stb_identity.Identity[4] = 0x9f;
	wfca_stb_identity.Identity[5] = 0xcb;
	wfca_stb_identity.Identity[6] = 0x08;
	wfca_stb_identity.Identity[7] = 0xf9;
	nRet = WFCAS_Initialize(&wfca_stb_identity);
	CAS_Dbg("-------WFCAS_Initialize err=%x\n", (unsigned int)nRet);
	if (nRet != WFERR_COMMON_NO)
	{
		CAS_Dbg("WFCAS_Initialize err=%x\n", (unsigned int)nRet);
		return FALSE;
	}
	
	app_wf_cas_api_smart_init();
	app_porting_ca_smart_create_task();

	return TRUE;
	
}

/*
* 以下接口CA菜单调用
*/


/*
*  获取对应数据之前，初始化数据管理
(一般创建对应窗体之前或create中调用)。
* 如初始化邮件、授权等
*/
int32_t app_wf_cas_api_init_data(dvb_ca_data_type_t date_type)
{
	int32_t dvb_ca_flag =  app_flash_get_config_dvb_ca_flag();
	if (dvb_ca_flag != DVB_CA_TYPE_WF)
		return FALSE;

	switch(date_type)
		{
			case DVB_CA_BASE_INFO:	
				return app_wf_cas_api_init_baseinfo_data();
			case DVB_CA_EMAIL_INFO: 
				return app_wf_cas_api_init_email_data();							
			case DVB_CA_ENTITLE_INFO:
				return app_wf_cas_api_init_entitle_data();
			default:
				break;	
		}


	
	return FALSE;		
}

char* app_wf_cas_api_get_data(ca_get_date_t *data)
{
	int32_t dvb_ca_flag =  app_flash_get_config_dvb_ca_flag();
	if (dvb_ca_flag != DVB_CA_TYPE_WF)
		return NULL;


	if (NULL == data)
		return NULL;	

	switch(data->date_type)
		{
			case DVB_CA_BASE_INFO:	
				return app_wf_cas_api_get_baseinfo_data(data);
			case DVB_CA_EMAIL_INFO: 
				return app_wf_cas_api_get_email_data(data); 						
			case DVB_CA_ENTITLE_INFO: 
				return app_wf_cas_api_get_entitle_data(data);
			default:
				break;	
		}
	
	return NULL;	
}


bool app_wf_cas_api_delete(ca_delete_data_t *data)
{
	int32_t dvb_ca_flag =  app_flash_get_config_dvb_ca_flag();
	if (dvb_ca_flag != DVB_CA_TYPE_WF)
		return FALSE;


	if (NULL == data)
		return FALSE;
	
	switch(data->date_type)
		{
			case DVB_CA_EMAIL_INFO: /*邮件*/	
				return app_wf_cas_api_delete_email(data);
			default:
				break;	
		}
	
	return 0;	
}

bool app_wf_cas_api_delete_all(ca_delete_data_t *data)
{
	int32_t dvb_ca_flag =  app_flash_get_config_dvb_ca_flag();
	if (dvb_ca_flag != DVB_CA_TYPE_WF)
		return FALSE;


	if (NULL == data)
		return FALSE;
	

	switch(data->date_type)
		{
			case DVB_CA_EMAIL_INFO: /*邮件*/	
				return app_wf_cas_api_delete_all_email(data);
			default:
				break;	
		}

	
	return 0;		
}


uint8_t app_wf_cas_api_get_count(ca_get_count_t *data)
{
	int32_t dvb_ca_flag =  app_flash_get_config_dvb_ca_flag();
	if (dvb_ca_flag != DVB_CA_TYPE_WF)
		return 0;

	if (NULL == data)
		return 0;
	

	switch(data->date_type)
		{
			case DVB_CA_EMAIL_INFO: 
				return app_wf_cas_api_get_email_count(data);							
			case DVB_CA_ENTITLE_INFO:
				return app_wf_cas_api_get_entitle_count(data);
			default:
				break;	
		}

	
	return 0;		
}

int32_t app_wf_cas_gxmsg_ca_on_event_exec(GxMessage * msg)
{
	switch(msg->msg_id)
		{
			case GXMSG_CA_ON_EVENT:
					{
						GxMsgProperty0_OnEvent* event;
						event = GxBus_GetMsgPropertyPtr(msg, GxMsgProperty0_OnEvent);
						printf("app_wf_cas_gxmsg_ca_on_event_exec GXMSG_CA_ON_EVENT\n");
						switch (event->key) {
								case DVB_CA_BASE_INFO:		       /*基本信息*/
									break;
								case DVB_CA_OPERATOR_INFO:		/*运营商信息*/	
									break;
								case DVB_CA_EMAIL_INFO:			/*邮件*/
									{
										app_wf_cas_gxmsg_ca_on_event_email(event);											
									}
									break;
								case DVB_CA_ENTITLE_INFO :			/*授权*/
									break;
								case DVB_CA_DETITLE_INFO:                /*反授权*/
									break;
								case DVB_CA_EMERGENCY_INFO:           /*应急广播*/
									break;
								case DVB_CA_ROLLING_INFO:               /*OSD滚动消息*/
									app_wf_cas_gxmsg_ca_on_event_rolling(event);
									break;
								case DVB_CA_FINGER_INFO:                  /*指纹*/
								//	app_by_cas_gxmsg_ca_on_event_finger(event);
									break;
								case DVB_CA_CARD_UPDATE_INFO:       /*卡升级*/
									break;
								case DVB_CA_FETURE_INFO:                  /*特征值*/
									break;
								case DVB_CA_IPPV_POP_INFO:                /*IPPV购买框信息*/
									app_wf_cas_gxmsg_ca_on_event_pptv_buy(event);
									break;
								case DVB_CA_IPPT_POP_INFO:                /*IPPT购买框信息*/
									break;
								case DVB_CA_IPPV_PROG_INFO:            /*IPPV已购买节目信息*/
									break;
								case DVB_CA_IPPV_SLOT_INFO:             /*IPPV钱包信息*/
									break;
//								case DVB_CA_CHILD_CARD_INFO:            /*子卡信息*/
//									break;
								case DVB_CA_MOTHER_CARD_INFO:            /*母卡信息*/
									break;
								case DVB_CA_CHANGE_PIN_INFO:             /*修改密码信息*/
									break;
								case DVB_CA_PROGRESS_INFO:             /*智能卡升级进度信息*/
									break;
								case DVB_CA_RATING_INFO:                /*成人级别信息*/
									break;
								case DVB_CA_WORK_TIME_INFO:          /*工作时段信息*/
									break;
								case DVB_CA_CURTAIN_INFO:          /*窗帘信息*/
									break;
								case DVB_CA_PAIRED_INFO:          /*配对信息*/
									break;
								default:
									break;
							}
				}
				break;
			default:
				break;
		}
	return 0;
}


uint8_t app_wf_cas_api_is_ca_pop_msg_type(uint8_t type)
{
	if ((type >= MSG_POP_COMMON_END+0+1)&&(type <=MSG_POP_COMMON_END+40+1))
		return TRUE;
	return FALSE;
}

int32_t app_wf_cas_api_pop_msg_exec(uint8_t type)
{
	CAS_Dbg("%s	type=%d\n",__FUNCTION__,type-MSG_POP_COMMON_END-1);
	switch(type-MSG_POP_COMMON_END-1)
	{
		case WFERR_COMMON_NO: /*没有错误，取消上次显示*/
			app_wf_cas_hide_ca_prompt();
			break;
		case 7:
			app_wf_cas_show_ca_prompt("NO CARD");
			break;			
		case 8:
			app_wf_cas_show_ca_prompt("INVALD CARD");
			break;
		case 40:
			app_wf_cas_show_ca_prompt("READING CARD");
			break;
		case 11:
			app_wf_cas_show_ca_prompt("STB TO CARD PAIRED ERROR");
			break;
		case 12:
			app_wf_cas_show_ca_prompt("NO AUTHORITY");
			break;
		case 13:
			app_wf_cas_show_ca_prompt("NOT IN AUTHORITIME");
			break;
		case 15:
			app_wf_cas_show_ca_prompt("INVALD CMD");
			break;
		case 18:
			app_wf_cas_show_ca_prompt("OWING FEE!");
			break;
		case 21:
			app_wf_cas_show_ca_prompt("WRONG AREA");
			break;
		case 14:
			app_wf_cas_show_ca_prompt("Being Logout!");
			break;
		case 20:
			app_wf_cas_show_ca_prompt("EXPIRED CHILD AUTHORITY");
			break;
		case 19:
			app_wf_cas_show_ca_prompt("CHILD TO MOTHER ERROR");
			break;
		default:
			break;
	}
	return 0;
}

int32_t app_wf_cas_api_osd_exec(void)
{
/*
* 滚动消息显示、隐藏
*/
	app_wf_cas_rolling_message_exec();

/*
* 邮件图标显示、隐藏
*/
	app_wf_cas_email_flag_exec();

/*
* 指纹显示、隐藏
*/
//	app_wf_cas_finger_exec();
	return 0;
}



APP_CAS_API_ControlBlock_t WfCasPortControlBlock=
{
	.m_cas_init_callback =  app_wf_cas_api_init,
	.m_cas_close_callback = NULL,
	.m_cas_api_card_in_callback = app_wf_cas_api_card_in,
	.m_cas_api_card_out_callback = app_wf_cas_api_card_out,
	.m_cas_api_release_ecm_filter_callback= app_wf_cas_api_release_ecm_filter,
	.m_cas_api_start_ecm_filter_callback = app_wf_cas_api_start_ecm_filter,
	.m_cas_api_release_emm_filter_callback = app_wf_cas_api_release_emm_filter,
	.m_cas_api_start_emm_filter_callback = app_wf_cas_api_start_emm_filter,
	.m_cas_api_check_cat_ca_descriptor_valid_callback= app_wf_cas_api_check_cat_ca_descriptor_valid,
	.m_cas_api_check_pmt_ca_descriptor_valid_callback= app_wf_cas_api_check_pmt_ca_descriptor_valid,
	.m_cas_api_is_ca_pop_msg_type_callback= app_wf_cas_api_is_ca_pop_msg_type,
	.m_cas_api_pop_msg_exec_callback= app_wf_cas_api_pop_msg_exec,
	.m_cas_api_osd_exec_callback= app_wf_cas_api_osd_exec,
	.m_cas_api_gxmsg_ca_on_event_exec_callback= app_wf_cas_gxmsg_ca_on_event_exec, 								
	.m_cas_api_init_data_callback = app_wf_cas_api_init_data,
	.m_cas_api_get_data_callback = app_wf_cas_api_get_data,
	.m_cas_api_get_count_callback = app_wf_cas_api_get_count,
	.m_cas_api_delete_callback = app_wf_cas_api_delete,
	.m_cas_api_delete_all_callback = app_wf_cas_api_delete_all,
	.m_cas_api_buy_ipp_callback = NULL,
	.m_cas_api_get_lock_status_callback = NULL,
	.m_cas_api_feed_mother_child_card_callback = NULL,
	.m_cas_api_change_pin_callback = NULL,
	.m_cas_api_change_rating_callback = NULL,
	.m_cas_api_change_worktime_callback= NULL,
};




