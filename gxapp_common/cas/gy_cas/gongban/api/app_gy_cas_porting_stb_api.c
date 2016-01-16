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
#include "app_common_search.h"
#include "app_common_prog.h"
#include "app_common_porting_ca_os.h"

#include "gyca_def.h"
#include "gyca_pub.h"
#include "gyca_osp.h"
#include "gyca_interface.h"
#include "gyca_porting.h"

#include "app_gy_cas_api_pin.h"
#include "app_gy_cas_api_smart.h"
#include "app_gy_cas_api_demux.h"
#include "app_gy_cas_api_rating.h"
#include "app_gy_cas_api_pop.h"
#include "app_gy_cas_api_rolling_message.h"
#include "app_gy_cas_api_finger.h"
#include "app_gy_cas_api_email.h"
#include "app_gy_cas_api_entitle.h"
#include "app_gy_cas_api_paired.h"
#include "app_gy_cas_api_system.h"
#include "app_gy_cas_api_baseinfo.h"


extern char EventStr[100];
extern void win_gy_cas_exigent_prompt(uint8_t opcode);
extern void app_prog_get_pos_in_group_by_service_id(uint32_t service_id,uint32_t *ppos);
extern void app_gy_cas_show_ca_prompt(char* context);
extern void app_gy_cas_hide_ca_prompt(void);
extern int32_t app_gy_cas_gxmsg_ca_on_event_entitle(GxMsgProperty0_OnEvent* event);
extern int32_t app_gy_cas_gxmsg_ca_on_event_ippv_buy(GxMsgProperty0_OnEvent* event);

static void app_gy_cas_gxmsg_ca_on_event_exigent_data(GxMsgProperty0_OnEvent* event)
{
	gy_cas_exigent_data_st * exigent_data = NULL;
	T_CARemoteControlInfo * ctrlData = NULL;
	search_dvbc_param tmp_param;
	uint32_t ppos;
	if(event == NULL)
	{
		return;
	}
	exigent_data = (gy_cas_exigent_data_st *)event->buf;
	if(exigent_data == NULL)
	{
		return;
	}
	if(exigent_data->wStatus == FALSE)
	{
		return;
	}
	if(exigent_data->buffer == NULL)
	{
		return;
	}
	ctrlData = (T_CARemoteControlInfo *)exigent_data->buffer;
	if(ctrlData->m_byOpCode == 0)
	{
		/*Manul search*/
		tmp_param.fre = ctrlData->m_dwFrequency/1000;
		tmp_param.qam = 2;
		tmp_param.symbol_rate = 6875;
		
		app_search_lock_tp(tmp_param.fre, tmp_param.symbol_rate,
					INVERSION_OFF, tmp_param.qam,200);
		win_gy_cas_exigent_prompt(ctrlData->m_byOpCode);
		app_search_scan_manual_mode(tmp_param.fre,tmp_param.symbol_rate,tmp_param.qam);
		
	}
	else if(ctrlData->m_byOpCode == 1)
	{
		/*Force switch channel*/
		app_prog_get_pos_in_group_by_service_id(ctrlData->m_wServiceID,&ppos);
		
		if(ppos != 0)
		{
			win_gy_cas_exigent_prompt(ctrlData->m_byOpCode);
			app_prog_save_playing_pos_in_group(ppos);
			app_play_switch_prog_clear_msg();	
			app_play_reset_play_timer(0);			
		}
		else
		{
			CAS_Dbg("ppos == 0\n");
		}
	}
	
}
/*extern	int8_t app_gy_cas_rolling_message_init(void);
extern  int32_t app_gy_cas_finger_init(void);
extern  int GYCA_Init();
extern 	uint8_t app_gy_cas_api_init_email_data(void);
extern int32_t app_gy_cas_api_init_entitle_data(void);
extern int32_t app_gy_cas_api_init_paired_data(void);
*/
uint8_t app_gy_cas_api_init(void)
{

	app_porting_ca_flash_init(FLASH_GY_CAS_SIZE*2);

	app_porting_ca_demux_init();
	
	app_porting_ca_descramble_init();
	app_gy_cas_rolling_message_init();
	app_gy_cas_finger_init();
	app_gy_cas_api_init_email_data();
	GYCA_Init();
	app_porting_ca_create_filter_task();
	
	app_porting_ca_smart_create_task();
		
	return TRUE;
}

/*
*  获取对应数据之前，初始化数据管理
(一般创建对应窗体之前或create中调用)。
* 如初始化邮件、授权等
*/
int32_t app_gy_cas_api_init_data(dvb_ca_data_type_t date_type)
{
	int32_t dvb_ca_flag =  app_flash_get_config_dvb_ca_flag();
	if (dvb_ca_flag != DVB_CA_TYPE_GY)
		return FALSE;

	switch(date_type)
	{
		case DVB_CA_EMAIL_INFO:
			return (int32_t)app_gy_cas_api_init_email_data();
			break;
		case DVB_CA_ENTITLE_INFO:
			return app_gy_cas_api_init_entitle_data();
			break;
		case DVB_CA_IPPV_PROG_INFO:
			break;
		case DVB_CA_IPPV_SLOT_INFO:
			break;
		case DVB_CA_RATING_INFO:
			return app_gy_cas_api_init_rating_data();
			break;
		case DVB_CA_PAIRED_INFO:
			return app_gy_cas_api_init_paired_data();
			break;
		default:
			break;	
	}

	
	return FALSE;		
}


char * app_gy_cas_api_get_data(ca_get_date_t *data)
{
	int32_t dvb_ca_flag =  app_flash_get_config_dvb_ca_flag();
	if (dvb_ca_flag != DVB_CA_TYPE_GY)
		return NULL;


	if (NULL == data)
		return NULL;	

	switch(data->date_type)
	{
		case DVB_CA_BASE_INFO: 
			return (char*)app_gy_cas_api_get_baseinfo_data(data);
			break;
		case DVB_CA_EMAIL_INFO:
			return (char*)app_gy_cas_api_get_email_data(data);
			break;
		case DVB_CA_ENTITLE_INFO:
			return (char*)app_gy_cas_api_get_entitle_data(data);
			break;
		case DVB_CA_IPPV_PROG_INFO:
			break;
		case DVB_CA_IPPV_SLOT_INFO:
			break;
		case DVB_CA_RATING_INFO:
			return (char*)app_gy_cas_api_get_rating_data(data);
			break;
		case DVB_CA_PAIRED_INFO:
			return (char*)app_gy_cas_api_get_paired_data(data);
			break;
		default:
			break;	
	}

	
	return NULL;	
}


uint8_t app_gy_cas_api_get_count(ca_get_count_t *data)
{
	int32_t dvb_ca_flag =  app_flash_get_config_dvb_ca_flag();
	if (dvb_ca_flag != DVB_CA_TYPE_GY)
		return 0;

	if (NULL == data)
		return 0;
	

	switch(data->date_type)
	{
		case DVB_CA_EMAIL_INFO:
			return app_gy_cas_api_get_email_count(data);
			break;
		case DVB_CA_ENTITLE_INFO:
			return app_gy_cas_api_get_entitle_count(data);
			break;
		case DVB_CA_IPPV_PROG_INFO:
	//		return app_cd_cas30_api_get_ippv_info_count(data);				
			break;
		case DVB_CA_IPPV_SLOT_INFO:
	//		return app_cd_cas30_api_get_ippv_slot_count(data);
			break;
		case DVB_CA_RATING_INFO:
			break;
		case DVB_CA_PAIRED_INFO:
			break;
		default:
			break;	
	}
	
	return 0;		
}

uint8_t app_gy_cas_api_is_ca_pop_msg_type(uint8_t type)
{
	if ((type >= MSG_POP_COMMON_END+GYCA_NOTICE_SHOW+1)&&(type <=MSG_POP_COMMON_END+GYCA_SMC_STATUS+1))
		return TRUE;
	
	return FALSE;
}


int32_t app_gy_cas_api_pop_msg_exec(uint8_t type)
{
	switch(type-MSG_POP_COMMON_END-1)
	{
		case GYCA_NOTICE_SHOW:		/* 显示节目不能播放的提示信息*/
			app_gy_cas_show_ca_prompt(EventStr);
			break;
		case GYCA_NOTICE_ERASE:		/* 擦除提示信息*/
			app_gy_cas_hide_ca_prompt();
			break;
		case GYCA_OSD_ERASE:			/* 擦除OSD信息*/
			//app_gy_cas_hide_ca_prompt();
			break;
		case GYCA_RESTART_CHAN:		/* 请求应用程序重新播放当前节目*/
			break;
		case GYCA_OSD_INFO:			/* 有EMM,PEMM OSD信息通知*/
			break;
		case GYCA_EXECUTE_OP_INFO:	/* 要求机顶盒执行具体的机顶盒操作*/
			break;
		case GYCA_SW_UPDATE_INFO:		/* 有EMM,PEMM 软件更新信息通知*/
			break;
		case GYCA_TRANSPARENT_INFO:	/* CA 透传给机顶盒的私有信息*/
			break;
		case GYCA_FINGER_SHOW:		/* 要求显示指纹信息*/
			break;
		case GYCA_FINGER_ERASE:		/* 要求擦除指纹信息*/
			break;
		case GYCA_IPPV_INFO:			/* IPPV 信息*/
			break;
		case GYCA_ENTITLEMENT_CHANGED:  /*授权变化通知消息*/
			break;
		case GYCA_SMC_STATUS: 		/* 智能卡状态已改变*/ 
			break;
		default:
			break;
	}
	return 0;
}


int32_t app_gy_cas_gxmsg_ca_on_event_exec(GxMessage * msg)
{
	switch(msg->msg_id)
		{
			case GXMSG_CA_ON_EVENT:
			{
				GxMsgProperty0_OnEvent* event;
				event = GxBus_GetMsgPropertyPtr(msg, GxMsgProperty0_OnEvent);
				switch (event->key) {

					case DVB_CA_BASE_INFO:		       /*基本信息*/
						break;
					case DVB_CA_OPERATOR_INFO:		/*运营商信息*/	
						break;
					case DVB_CA_EMAIL_INFO:		/*邮件*/
						{
							app_gy_cas_gxmsg_ca_on_event_email(event);
						}
						break;
					case DVB_CA_ENTITLE_INFO: /*授权*/							
						{
							app_gy_cas_gxmsg_ca_on_event_entitle(event);
						}
						break;
					case DVB_CA_DETITLE_INFO: /*反授权*/							
						{
				//			app_gy_cas_gxmsg_ca_on_event_detitle(event);
						}
						break;
					case DVB_CA_EMERGENCY_INFO:           /*应急广播*/
						app_gy_cas_gxmsg_ca_on_event_exigent_data(event);
						break;
					case DVB_CA_ROLLING_INFO: /*OSD滚动消息*/
						{
							app_gy_cas_gxmsg_ca_on_event_rolling(event);
						}
						break;
					case DVB_CA_FINGER_INFO:                  /*指纹*/
							{
							app_gy_cas_gxmsg_ca_on_event_finger(event);
						}
						break;
					case DVB_CA_CARD_UPDATE_INFO:       /*卡升级*/
							{
				//				app_cd_cas30_gxmsg_ca_on_event_cardupdating(event);
						}
						break;
					case DVB_CA_FETURE_INFO:                  /*特征值*/
						break;
					case DVB_CA_IPPV_POP_INFO:                /*IPPV购买框信息*/
							{
						app_gy_cas_gxmsg_ca_on_event_ippv_buy(event);
						}
						break;
					case DVB_CA_IPPT_POP_INFO:                /*IPPT购买框信息*/
						break;
					case DVB_CA_IPPV_PROG_INFO:            /*IPPV已购买节目信息*/
						break;
					case DVB_CA_IPPV_SLOT_INFO:             /*IPPV钱包信息*/
						break;
//					case DVB_CA_CHILD_CARD_INFO:            /*子卡信息*/
//						break;
					case DVB_CA_MOTHER_CARD_INFO:            /*母卡信息*/
							{
				//				app_cd_cas30_gxmsg_ca_on_event_feed(event);								
						}
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
			default:
				break;
		}
	return 0;
}

int32_t app_gy_cas_api_osd_exec(void)
{
	app_gy_cas_rolling_message_exec();
	app_gy_cas_email_flag_exec();
//	app_gy_cas_detitle_flag_exec();
	app_gy_cas_finger_exec();	
	return 1;
}


bool app_gy_cas_api_delete(ca_delete_data_t *data)
{
	int32_t dvb_ca_flag =  app_flash_get_config_dvb_ca_flag();
	if (dvb_ca_flag != DVB_CA_TYPE_GY)
		return FALSE;


	if (NULL == data)
		return FALSE;
	
	switch(data->date_type)
		{
			case DVB_CA_EMAIL_INFO: /*邮件*/	
				return app_gy_cas_api_delete_email(data);
			case DVB_CA_DETITLE_INFO: /*反授权*/	
//				return app_gy_cas_api_delete_detitle(data);	
			default:
				break;	
		}
	
	return 0;	
}

bool app_gy_cas_api_delete_all(ca_delete_data_t *data)
{
	int32_t dvb_ca_flag =  app_flash_get_config_dvb_ca_flag();
	if (dvb_ca_flag != DVB_CA_TYPE_GY)
		return FALSE;


	if (NULL == data)
		return FALSE;
	

	switch(data->date_type)
		{
			case DVB_CA_EMAIL_INFO: /*邮件*/	
				return app_gy_cas_api_delete_all_email(data);
			case DVB_CA_DETITLE_INFO: /*反授权*/	
//				return app_gy_cas_api_delete_all_detitle(data);	
			default:
				break;	
		}

	
	return 0;		
}



APP_CAS_API_ControlBlock_t GyCasPortControlBlock=
{
	.m_cas_init_callback =  app_gy_cas_api_init,
	.m_cas_close_callback = NULL,
	.m_cas_api_card_in_callback = app_gy_cas_api_card_in,
	.m_cas_api_card_out_callback = app_gy_cas_api_card_out,
	.m_cas_api_release_ecm_filter_callback= app_gy_cas_api_release_ecm_filter,
	.m_cas_api_start_ecm_filter_callback = app_gy_cas_api_start_ecm_filter,
	.m_cas_api_release_emm_filter_callback = app_gy_cas_api_release_emm_filter,
	.m_cas_api_start_emm_filter_callback = app_gy_cas_api_start_emm_filter,
	.m_cas_api_check_cat_ca_descriptor_valid_callback= app_gy_cas_api_check_cat_ca_descriptor_valid,
	.m_cas_api_check_pmt_ca_descriptor_valid_callback= app_gy_cas_api_check_pmt_ca_descriptor_valid,
	.m_cas_api_is_ca_pop_msg_type_callback= app_gy_cas_api_is_ca_pop_msg_type,
	.m_cas_api_pop_msg_exec_callback= app_gy_cas_api_pop_msg_exec,
	.m_cas_api_osd_exec_callback= app_gy_cas_api_osd_exec,
	.m_cas_api_gxmsg_ca_on_event_exec_callback= app_gy_cas_gxmsg_ca_on_event_exec, 								
	.m_cas_api_init_data_callback = app_gy_cas_api_init_data,
	.m_cas_api_get_data_callback = app_gy_cas_api_get_data,
	.m_cas_api_get_count_callback = app_gy_cas_api_get_count,
	.m_cas_api_delete_callback = app_gy_cas_api_delete,
	.m_cas_api_delete_all_callback = app_gy_cas_api_delete_all,
	.m_cas_api_buy_ipp_callback = NULL,
	.m_cas_api_get_lock_status_callback =	NULL,
	.m_cas_api_feed_mother_child_card_callback = NULL,
	.m_cas_api_change_pin_callback = app_gy_cas_api_change_pin,
	.m_cas_api_verify_pin_callback = NULL,
	.m_cas_api_change_rating_callback = app_gy_cas_api_change_rating,
	.m_cas_api_change_worktime_callback= NULL,
};




