/*****************************************************************************
* 						   CONFIDENTIAL
*        Hangzhou GuoXin Science and Technology Co., Ltd.
*                      (C)2014, All right reserved
******************************************************************************

******************************************************************************
* File Name :	app_gos_cas_porting_stb_api.c
* Author    : 	zhouhm
* Project   :	goxceed dvbc
* Type      :
******************************************************************************
* Purpose   :	
******************************************************************************
* Release History:
VERSION	Date			  AUTHOR         Description
1.0  	2014.04.22		  zhouhm 	 			creation
*****************************************************************************/
#include <gxtype.h>
#include "gxapp_sys_config.h"
#include "gxmsg.h"
#include "app_common_play.h"
#include "app_common_table_pmt.h"
#include "app_common_porting_stb_api.h"
#include "app_common_porting_ca_flash.h"
#include "app_common_porting_ca_demux.h"
#include "app_common_porting_ca_descramble.h"
#include "app_common_porting_ca_smart.h"
#include "GOSCAS_STBDataPublic.h"
#include "GOSCAS_STBInf.h"
#include "app_gos_cas_api_pop.h"
#include "app_gos_cas_api_baseinfo.h"
/*
* 永新视博CA接口头文件
*/
//#include "app_gos_cas_api_finger.h"
//#include "app_gos_cas_api_operator.h"
#include "app_gos_cas_api_demux.h"
#include "app_gos_cas_api_smart.h"
//#include "app_gos_cas_api_entitle.h"
//#include "app_gos_cas_api_baseinfo.h"
//#include "app_gos_cas_api_worktime.h"
//#include "app_gos_cas_api_rating.h"
//#include "app_gos_cas_api_pin.h"
//#include "app_gos_cas_api_email.h"
//#include "app_gos_cas_api_rolling_message.h"
//#include "app_gos_cas_api_lock_service.h"
extern 	int32_t app_gos_cas_gxmsg_ca_on_event_finger(GxMsgProperty0_OnEvent* event);
//extern 	int32_t app_gos_cas_finger_exec(void);
extern 	void app_gos_cas_hide_ca_prompt(void);
extern void app_gos_cas_show_ca_prompt(char* context);
extern void app_gos_cas_show_ca_prompt_message(uint8_t type);

uint8_t app_gos_cas_api_init(void)
{
	app_porting_ca_flash_init(64*1024*2);
	app_porting_ca_demux_init();
	app_porting_ca_descramble_init();
//	app_gos_cas_rolling_message_init();
	app_porting_ca_create_filter_task();
	if (FALSE == GOSCASTB_Init(0))
	{
		CA_FAIL((" GOSCASTB_Init  failed \n"));
		return FALSE;
	}
	GOSCASTB_AddDebugMsgSign(FALSE);//开启调试信息
	app_porting_ca_smart_create_task();
	return TRUE;
}

/*
*  获取对应数据之前，初始化数据管理
(一般创建对应窗体之前或create中调用)。
* 如初始化邮件、授权等
*/
int32_t app_gos_cas_api_init_data(dvb_ca_data_type_t date_type)
{
	switch(date_type)
		{
			case DVB_CA_BASE_INFO:  /*基本信息*/
				break;
			case DVB_CA_OPERATOR_INFO: /*运营商信息*/
//				return app_gos_cas_api_init_operator_data();
				break;
			case DVB_CA_EMAIL_INFO: /*邮件*/
//				return app_gos_cas_api_init_email_data();
				break;
			case DVB_CA_ENTITLE_INFO: /*授权*/		
//				return app_gos_cas_api_init_entitle_data();			
				break;
			case DVB_CA_DETITLE_INFO: /*反授权*/		
				break;
			case DVB_CA_CARD_UPDATE_INFO: /*卡升级*/
				break;
			case DVB_CA_FETURE_INFO: /*特征值*/
				break;
			case DVB_CA_IPPV_POP_INFO: /*IPPV购买框信息*/
				break;
			case DVB_CA_IPPT_POP_INFO: /*IPPT购买框信息*/
				break;
			case DVB_CA_IPPV_PROG_INFO: /*IPPV已购买节目信息*/			
				break;
			case DVB_CA_IPPV_SLOT_INFO: /*IPPV钱包信息*/
				break;
			case DVB_CA_MOTHER_CARD_INFO: /*母卡信息*/			
				break;
			case DVB_CA_CHANGE_PIN_INFO: /*修改密码信息*/
				break;
			case DVB_CA_PROGRESS_INFO: /*智能卡升级进度信息*/
				break;
			case DVB_CA_RATING_INFO: /*成人级别信息*/
//				return app_gos_cas_api_init_rating_data();	
				break;
			case DVB_CA_WORK_TIME_INFO: /*工作时段信息*/
//				return app_gos_cas_api_init_worktime_data();
				break;
			case DVB_CA_CURTAIN_INFO: /*窗帘信息*/
				break;
			case DVB_CA_PAIRED_INFO: /*配对信息*/
			default:
				break;	
		}

	
	return FALSE;		
}

char * app_gos_cas_api_get_data(ca_get_date_t *data)
{

	if (NULL == data)
		return NULL;	

	switch(data->date_type)
		{
			case DVB_CA_BASE_INFO:  /*基本信息*/
				return app_gos_cas_api_get_baseinfo_data(data);
			case DVB_CA_OPERATOR_INFO: /*运营商信息*/	
//				return app_gos_cas_api_get_operator_data(data);
				break;
			case DVB_CA_EMAIL_INFO: /*邮件*/
//				return app_gos_cas_api_get_email_data(data);
				break;
			case DVB_CA_ENTITLE_INFO: /*授权*/	
//				return app_gos_cas_api_get_entitle_data(data);
				break;
			case DVB_CA_DETITLE_INFO: /*反授权*/	
				break;					
			case DVB_CA_CARD_UPDATE_INFO: /*卡升级*/
				break;
			case DVB_CA_FETURE_INFO: /*特征值*/			
				break;
			case DVB_CA_IPPV_POP_INFO: /*IPPV购买框信息*/
				break;
			case DVB_CA_IPPT_POP_INFO: /*IPPT购买框信息*/
				break;
			case DVB_CA_IPPV_PROG_INFO: /*IPPV已购买节目信息*/
				break;
			case DVB_CA_IPPV_SLOT_INFO: /*IPPV钱包信息*/			
				break;
			case DVB_CA_MOTHER_CARD_INFO: /*母卡信息*/				
				break;
			case DVB_CA_CHANGE_PIN_INFO: /*修改密码信息*/
				break;
			case DVB_CA_PROGRESS_INFO: /*智能卡升级进度信息*/
				break;
			case DVB_CA_RATING_INFO: /*成人级别信息*/
//				return app_gos_cas_api_get_rating_data(data);
				break;
			case DVB_CA_WORK_TIME_INFO: /*工作时段信息*/
//				return app_gos_cas_api_get_worktime_data(data);
				break;
			case DVB_CA_CURTAIN_INFO: /*窗帘信息*/
				break;
			case DVB_CA_PAIRED_INFO: /*配对信息*/
				break;							
			default:
				break;	
		}

	
	return NULL;	
}

/*
*  更新CA数据
(CA数据发生变更时调用)。
* 如删除邮件、反授权，CA通知授权信息变化等
*/
uint8_t app_gos_cas_api_update_data(ca_get_date_t *data)
{

	if (NULL == data)
		return 0;	

	switch(data->date_type)
		{
			case DVB_CA_BASE_INFO:  /*基本信息*/
				break;
			case DVB_CA_OPERATOR_INFO: /*运营商信息*/
				break;
			case DVB_CA_EMAIL_INFO: /*邮件*/	
				break;
			case DVB_CA_ENTITLE_INFO: /*授权*/
				break;
			case DVB_CA_DETITLE_INFO: /*反授权*/
				break;
			case DVB_CA_EMERGENCY_INFO: /*应急广播*/
				break;
			case DVB_CA_ROLLING_INFO: /*OSD滚动消息*/
				break;
			case DVB_CA_FINGER_INFO: /*指纹*/
				break;
			case DVB_CA_CARD_UPDATE_INFO: /*卡升级*/
				break;
			case DVB_CA_FETURE_INFO: /*特征值*/
				break;
			case DVB_CA_IPPV_POP_INFO: /*IPPV购买框信息*/
				break;
			case DVB_CA_IPPT_POP_INFO: /*IPPT购买框信息*/
				break;
			case DVB_CA_IPPV_PROG_INFO: /*IPPV已购买节目信息*/
				break;
			case DVB_CA_IPPV_SLOT_INFO: /*IPPV钱包信息*/
				break;
//			case DVB_CA_CHILD_CARD_INFO: /*子卡信息*/
//				break;
			case DVB_CA_MOTHER_CARD_INFO: /*母卡信息*/
				break;
			case DVB_CA_CHANGE_PIN_INFO: /*修改密码信息*/
				break;
			case DVB_CA_PROGRESS_INFO: /*智能卡升级进度信息*/
				break;
			case DVB_CA_RATING_INFO: /*成人级别信息*/
				break;
			case DVB_CA_WORK_TIME_INFO: /*工作时段信息*/
				break;
			case DVB_CA_CURTAIN_INFO: /*窗帘信息*/
				break;
			case DVB_CA_PAIRED_INFO: /*配对信息*/
				break;
			default:
				break;	
		}
	
	return 0;	
}

uint8_t app_gos_cas_api_get_count(ca_get_count_t *data)
{
	if (NULL == data)
		return 0;
	

	switch(data->date_type)
		{
			case DVB_CA_BASE_INFO:  /*基本信息*/
				break;
			case DVB_CA_OPERATOR_INFO: /*运营商信息*/
//				return app_gos_cas_api_get_operator_count(data);
				break;
			case DVB_CA_EMAIL_INFO: /*邮件*/
//				return app_gos_cas_api_get_email_count(data);
				break;
			case DVB_CA_ENTITLE_INFO: /*授权*/
//				return app_gos_cas_api_get_entitle_count(data);
				break;
			case DVB_CA_DETITLE_INFO: /*反授权*/	
				break;													
			case DVB_CA_EMERGENCY_INFO: /*应急广播*/
				break;
			case DVB_CA_ROLLING_INFO: /*OSD滚动消息*/
				break;
			case DVB_CA_FINGER_INFO: /*指纹*/
				break;
			case DVB_CA_CARD_UPDATE_INFO: /*卡升级*/
				break;
			case DVB_CA_FETURE_INFO: /*特征值*/			
				break;
			case DVB_CA_IPPV_POP_INFO: /*IPPV购买框信息*/
				break;
			case DVB_CA_IPPT_POP_INFO: /*IPPT购买框信息*/
				break;
			case DVB_CA_IPPV_PROG_INFO: /*IPPV已购买节目信息*/				
				break;
			case DVB_CA_IPPV_SLOT_INFO: /*IPPV钱包信息*/
				break;
			case DVB_CA_MOTHER_CARD_INFO: /*母卡信息*/			
				break;
			case DVB_CA_CHANGE_PIN_INFO: /*修改密码信息*/
				break;
			case DVB_CA_PROGRESS_INFO: /*智能卡升级进度信息*/
				break;
			case DVB_CA_RATING_INFO: /*成人级别信息*/
				break;
			case DVB_CA_WORK_TIME_INFO: /*工作时段信息*/
				break;
			case DVB_CA_CURTAIN_INFO: /*窗帘信息*/
				break;
			case DVB_CA_PAIRED_INFO: /*配对信息*/
				break;
			default:
				break;	
		}
	
	return 0;		
}

bool app_gos_cas_api_delete(ca_delete_data_t *data)
{
	if (NULL == data)
		return FALSE;
	
	switch(data->date_type)
		{
			case DVB_CA_EMAIL_INFO: /*邮件*/
//				return app_dvt_cas_api_delete_email(data);
				break;
			case DVB_CA_DETITLE_INFO: /*反授权*/	
				break;
			default:
				break;	
		}
	
	return 0;	
}

bool app_gos_cas_api_delete_all(ca_delete_data_t *data)
{
	if (NULL == data)
		return FALSE;
	

	switch(data->date_type)
		{
			case DVB_CA_EMAIL_INFO: /*邮件*/
//				return app_gos_cas_api_delete_all_email(data);
				break;
			case DVB_CA_DETITLE_INFO: /*反授权*/	
				break;
			default:
				break;	
		}

	
	return 0;		
}

int32_t app_gos_cas_gxmsg_ca_on_event_exec(GxMessage * msg)
{
	switch(msg->msg_id)
		{
			case GXMSG_CA_ON_EVENT:
			{
				GxMsgProperty0_OnEvent* event;
				event = GxBus_GetMsgPropertyPtr(msg, GxMsgProperty0_OnEvent);
				//printf("app_gos_cas_gxmsg_ca_on_event_exec GXMSG_CA_ON_EVENT event->key=%d\n",event->key);
				switch (event->key) {

					case DVB_CA_BASE_INFO:		       /*基本信息*/
						break;
					case DVB_CA_OPERATOR_INFO:		/*运营商信息*/	
						break;
					case DVB_CA_EMAIL_INFO:		/*邮件*/
						{
							//app_gos_cas_gxmsg_ca_on_event_email(event);
						}
						break;
					case DVB_CA_ENTITLE_INFO: /*授权*/							
						break;
					case DVB_CA_DETITLE_INFO: /*反授权*/							
						break;
					case DVB_CA_EMERGENCY_INFO:           /*应急广播*/
						break;
					case DVB_CA_ROLLING_INFO: /*OSD滚动消息*/
						{
							//app_gos_cas_gxmsg_ca_on_event_rolling(event);
						}
						break;
					case DVB_CA_FINGER_INFO:                  /*指纹*/
							{
							//app_gos_cas_gxmsg_ca_on_event_finger(event);
						}
						break;
					case DVB_CA_CARD_UPDATE_INFO:       /*卡升级*/
						break;
					case DVB_CA_FETURE_INFO:                  /*特征值*/
						break;
					case DVB_CA_IPPV_POP_INFO:                /*IPPV购买框信息*/
						break;
					case DVB_CA_IPPT_POP_INFO:                /*IPPT购买框信息*/
						break;
					case DVB_CA_IPPV_PROG_INFO:            /*IPPV已购买节目信息*/
						break;
					case DVB_CA_IPPV_SLOT_INFO:             /*IPPV钱包信息*/
						break;
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
			default:
				break;
		}
	return 0;
}

uint8_t app_gos_cas_api_is_ca_pop_msg_type(uint8_t type)
{
	if ((type >= MSG_POP_COMMON_END+CA_GOS_No_OSDPromt+1)&&(type <=MSG_POP_COMMON_END+CA_GOS_SMART_OUT+1))
		return TRUE;
	
	return FALSE;
}

int32_t app_gos_cas_api_pop_msg_exec(uint8_t type)
{
	switch(type-MSG_POP_COMMON_END-1)
		{
		case CA_GOS_No_OSDPromt:			/*无OSD提示*/
			app_gos_cas_hide_ca_prompt();
			break;
		case CA_GOS_SMART_STOP:				/*卡已被禁用*/
			app_gos_cas_show_ca_prompt("Err Smart Stop");
			break;	
		case CA_GOS_SMART_AREA_NOPAIR:		/*流卡区域不匹配*/
			app_gos_cas_show_ca_prompt("Err Smart Area NoPair");
			break;
		case CA_GOS_SMART_IPPT_SWITCH:		/*IPPT用户提示*/
			app_gos_cas_show_ca_prompt("Err Smart Ippt Switch");
			break;			
		case CA_GOS_SMART_STB_NOPAIR:		/*机卡不配对*/
			app_gos_cas_show_ca_prompt("Err Smart Stb NoPair");
			break;	
		case CA_GOS_SMART_PROG_NOENTITLE:	/*节目未授权*/
			app_gos_cas_show_ca_prompt("Err Prog NoEntitle");
			break;
		case CA_GOS_SMART_PROG_NOENTITLE_IPPT:/*IPPT 节目未授权*/
			app_gos_cas_show_ca_prompt("Err Prog NoEntitle Ippt");
			break;			
		case CA_GOS_SMART_MOTHER_SON_PAIR:	/*子母卡需从新配对*/
			app_gos_cas_show_ca_prompt("Err Smart Mother Son Pair");
			break;	
		case CA_GOS_SMART_MOTHER_SON_TIMEOUT:	/*子母卡配对超时*/
			app_gos_cas_show_ca_prompt("Err Smart Mother Son Timeout");
			break;	
		case CA_GOS_SMART_PAIR_OK:			/*机卡对应OK*/
			app_gos_cas_show_ca_prompt("Smart Pair Ok");
			break;
		case CA_GOS_SMART_LAST_DAY:			/*最后提示天数*/
			app_gos_cas_show_ca_prompt("Smart Last Day");
			app_gos_cas_show_ca_prompt_message(type);			
			break;
		case CA_GOS_SMART_AREA_LOCK: 		/*卡区域锁定 */
			app_gos_cas_show_ca_prompt("Smart Area Lock");
			break;			
		case CA_GOS_SMART_CARD_LOCK: 		/*卡已被锁定 */
			app_gos_cas_show_ca_prompt("Smart Card Lock");
			break;
		case CA_GOS_SMART_IPPT:				/*IPPT提示*/
			app_gos_cas_show_ca_prompt("Smart Ippt");
			app_gos_cas_show_ca_prompt_message(type);
			break;	
		case CA_GOS_SMART_LIMIT_WATCH:		/*卡限制收看*/
			app_gos_cas_show_ca_prompt("Smart Limit Watch");
			break;			
		case CA_GOS_SMART_NO_WORK_TIME:		/*卡不在工作时间段*/
			app_gos_cas_show_ca_prompt("Smart No Work Time");
			break;	
		case CA_GOS_SMART_OUT:				/*节目加扰，请插卡*/
			app_gos_cas_show_ca_prompt("Smart Out");
			break;	
		default:
			break;
		}
	return 0;
}

int32_t app_gos_cas_api_osd_exec(void)
{
//	app_gos_cas_rolling_message_exec();
//	app_gos_cas_email_flag_exec();
//	app_gos_cas_finger_exec();	
	return 1;
}



APP_CAS_API_ControlBlock_t GosCasPortControlBlock=
{
	.m_cas_init_callback =  app_gos_cas_api_init,
	.m_cas_close_callback = NULL,
	.m_cas_api_card_in_callback = app_gos_cas_api_card_in,
	.m_cas_api_card_out_callback = app_gos_cas_api_card_out,
	.m_cas_api_release_ecm_filter_callback= app_gos_cas_api_release_ecm_filter,
	.m_cas_api_start_ecm_filter_callback = app_gos_cas_api_start_ecm_filter,
	.m_cas_api_release_emm_filter_callback = app_gos_cas_api_release_emm_filter,
	.m_cas_api_start_emm_filter_callback = app_gos_cas_api_start_emm_filter,
	.m_cas_api_check_cat_ca_descriptor_valid_callback= app_gos_cas_api_check_cat_ca_descriptor_valid,
	.m_cas_api_check_pmt_ca_descriptor_valid_callback= app_gos_cas_api_check_pmt_ca_descriptor_valid,
	.m_cas_api_is_ca_pop_msg_type_callback= app_gos_cas_api_is_ca_pop_msg_type,
	.m_cas_api_pop_msg_exec_callback= app_gos_cas_api_pop_msg_exec,
	.m_cas_api_osd_exec_callback= app_gos_cas_api_osd_exec,
	.m_cas_api_gxmsg_ca_on_event_exec_callback= app_gos_cas_gxmsg_ca_on_event_exec, 								
	.m_cas_api_init_data_callback = app_gos_cas_api_init_data,
	.m_cas_api_get_data_callback = app_gos_cas_api_get_data,
	.m_cas_api_get_count_callback = app_gos_cas_api_get_count,
	.m_cas_api_delete_callback = app_gos_cas_api_delete,
	.m_cas_api_delete_all_callback = app_gos_cas_api_delete_all,
	.m_cas_api_buy_ipp_callback = NULL,//;
	.m_cas_api_get_lock_status_callback = NULL,//app_gos_cas_api_get_lock_status,
	.m_cas_api_feed_mother_child_card_callback = NULL,
	.m_cas_api_change_pin_callback = NULL,//app_gos_cas_api_change_pin,
	.m_cas_api_verify_pin_callback = NULL,//app_gos_cas_api_verify_pin,
	.m_cas_api_change_rating_callback = NULL,//app_gos_cas_api_change_rating,
	.m_cas_api_change_worktime_callback= NULL,//app_gos_cas_api_change_worktime,
};




