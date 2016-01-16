/*****************************************************************************
* 						   CONFIDENTIAL
*        Hangzhou GuoXin Science and Technology Co., Ltd.
*                      (C)2012, All right reserved
******************************************************************************

******************************************************************************
* File Name :	app_xinshimao_cas_porting_stb_api.c
* Author    : 	zhouhm
* Project   :	goxceed dvbc
* Type      :
******************************************************************************
* Purpose   :	模块头文件
******************************************************************************
* Release History:
VERSION	Date			  AUTHOR         Description
1.0  	2013.09.17		  zhouhm 	 			creation
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
#include "xinshimao_ca.h"
#include "app_xinshimao_cas_api_baseinfo.h"
#include "app_xinshimao_cas_api_rolling_message.h"
/*
* 永新视博CA接口头文件
*/
#include "app_xinshimao_cas_api_demux.h"
#include "app_xinshimao_cas_api_smart.h"

extern 	void app_xinshimao_cas_hide_ca_prompt(void);
extern void app_xinshimao_cas_show_ca_prompt(char* context);
extern 	void app_porting_ca_smart_check_status(void);
/*
* 以下接口CA菜单调用
*/

uint8_t app_xinshimao_cas_api_init(void)
{
	app_porting_ca_flash_init(64*1024);
	app_porting_ca_demux_init();
	app_porting_ca_descramble_init();
	app_porting_ca_create_filter_task();
    app_xinshimao_cas_rolling_message_init();
	XinShiMao_Initialize(0);
	app_porting_ca_smart_check_status();
	app_porting_ca_smart_create_task();
	return TRUE;
	
}

/*
*  获取对应数据之前，初始化数据管理
(一般创建对应窗体之前或create中调用)。
* 如初始化邮件、授权等
*/
int32_t app_xinshimao_cas_api_init_data(dvb_ca_data_type_t date_type)
{
	int32_t dvb_ca_flag =  app_flash_get_config_dvb_ca_flag();
	if (dvb_ca_flag != DVB_CA_TYPE_XINSHIMAO)
		return FALSE;

	switch(date_type)
		{
			case DVB_CA_BASE_INFO:  /*基本信息*/
				return app_xinshimao_cas_api_init_baseinfo_data();
				break;
			default:
				break;	
		}

	
	return FALSE;		
}

char * app_xinshimao_cas_api_get_data(ca_get_date_t *data)
{
	int32_t dvb_ca_flag =  app_flash_get_config_dvb_ca_flag();
	if (dvb_ca_flag != DVB_CA_TYPE_XINSHIMAO)
		return NULL;


	if (NULL == data)
		return NULL;	

	switch(data->date_type)
		{
			case DVB_CA_BASE_INFO:  /*基本信息*/
				return app_xinshimao_cas_api_get_baseinfo_data(data);		
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
uint8_t app_xinshimao_cas_api_update_data(ca_get_date_t *data)
{
	int32_t dvb_ca_flag =  app_flash_get_config_dvb_ca_flag();

	if (dvb_ca_flag != DVB_CA_TYPE_XINSHIMAO)
		return 0;


	if (NULL == data)
		return 0;	

	switch(data->date_type)
		{
			case DVB_CA_BASE_INFO:  /*基本信息*/
				break;
			default:
				break;	
		}
	
	return 0;	
}

uint8_t app_xinshimao_cas_api_get_count(ca_get_count_t *data)
{
	int32_t dvb_ca_flag =  app_flash_get_config_dvb_ca_flag();
	if (dvb_ca_flag != DVB_CA_TYPE_XINSHIMAO)
		return 0;

	if (NULL == data)
		return 0;
	

	switch(data->date_type)
		{
			case DVB_CA_BASE_INFO:  /*基本信息*/
				break;
			default:
				break;	
		}
	
	return 0;		
}

bool app_xinshimao_cas_api_delete(ca_delete_data_t *data)
{
	int32_t dvb_ca_flag =  app_flash_get_config_dvb_ca_flag();
	if (dvb_ca_flag != DVB_CA_TYPE_XINSHIMAO)
		return FALSE;


	if (NULL == data)
		return FALSE;
	
	switch(data->date_type)
		{
			case DVB_CA_EMAIL_INFO: /*邮件*/	
//				return app_xinshimao_cas_api_delete_email(data);
				break;
			default:
				break;	
		}
	
	return 0;	
}

bool app_xinshimao_cas_api_delete_all(ca_delete_data_t *data)
{
	int32_t dvb_ca_flag =  app_flash_get_config_dvb_ca_flag();
	if (dvb_ca_flag != DVB_CA_TYPE_XINSHIMAO)
		return FALSE;


	if (NULL == data)
		return FALSE;
	

	switch(data->date_type)
		{
			case DVB_CA_EMAIL_INFO: /*邮件*/	
//				return app_xinshimao_cas_api_delete_all_email(data);
				break;	
			default:
				break;	
		}

	
	return 0;		
}

int32_t app_xinshimao_cas_gxmsg_ca_on_event_exec(GxMessage * msg)
{
	switch(msg->msg_id)
		{
			case GXMSG_CA_ON_EVENT:
			{
				GxMsgProperty0_OnEvent* event;
				event = GxBus_GetMsgPropertyPtr(msg, GxMsgProperty0_OnEvent);
				switch (event->key) {
					case DVB_CA_ROLLING_INFO: /*OSD滚动消息*/
					{
                        app_xinshimao_cas_gxmsg_ca_on_event_msg(event);
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

uint8_t app_xinshimao_cas_api_is_ca_pop_msg_type(uint8_t type)
{
	if ((type >= MSG_POP_COMMON_END+SC_NORMAL+1)&&(type <=MSG_POP_COMMON_END+SC_CARDENTITLEREMAINTDAY+1))
		return TRUE;
	
	return FALSE;
}


int32_t app_xinshimao_cas_api_pop_msg_exec(uint8_t type)
{
	switch(type-MSG_POP_COMMON_END-1)
		{
		case SC_NORMAL: /*正常状态*/
			app_xinshimao_cas_hide_ca_prompt();
			break;
		case SC_NO_CARD: /*未插卡*/
			app_xinshimao_cas_show_ca_prompt("Sc No Card");
			break;
		case SC_NO_PPV_RIGHT: /*没有PPV授权*/
			app_xinshimao_cas_show_ca_prompt("Sc No PPV Right");
			break;
		case SC_NO_PPC_RIGHT: /*没有PPC授权*/
			app_xinshimao_cas_show_ca_prompt("Sc No PPC Right");
			break;
		case SC_PARENT_CTRL: /*家长控制*/
			app_xinshimao_cas_show_ca_prompt("Sc Parent Ctrl");
			break;
		case SC_NO_CONDITION: /*条件限播*/
			app_xinshimao_cas_show_ca_prompt("Sc No Condition");
			break;
		case SC_INVALID_CARD: /*无效卡*/
			app_xinshimao_cas_show_ca_prompt("Sc Invalid Card");
			break;
		case SC_TYPEERROR: /*子母卡喂养失败,插入智能卡类型错误*/
			app_xinshimao_cas_show_ca_prompt("Sc TypeError");
			break;
		case SC_NEEDFEED: /*子卡需要与母卡对应,请喂养子卡*/
			app_xinshimao_cas_show_ca_prompt("Sc Need Feed");
			break;
		case SC_ZONEERROR: /*区域错误*/
			app_xinshimao_cas_show_ca_prompt("Sc Zone Error");
			break;
		case SC_FEEDTIMEERROR: /*喂养时间错误*/
			app_xinshimao_cas_show_ca_prompt("Sc Feed Time Error");
			break;
		case SC_FEEDDATEERROR: /*喂养日期错误*/
			app_xinshimao_cas_show_ca_prompt("Sc Feed Date Error");
			break;
		case SC_FEEDSYSTEMTIMEERROR: /*系统时间没有正确获取,喂养失败*/
			app_xinshimao_cas_show_ca_prompt("Sc Feed SystemTime Error");
			break;
		case SC_ACSEVICEIDEERROR: /*加扰机AC 错误*/
			app_xinshimao_cas_show_ca_prompt("Sc AC ServiceId Error");
			break;
		case SC_CARDSTBNOTPAIR: /*加扰机AC 错误*/
			app_xinshimao_cas_show_ca_prompt("Sc Card Stb Not Pair");
			break;
		case SC_CARDENTITLEREMAINTDAY: /*授权剩余天数*/
			app_xinshimao_cas_show_ca_prompt("Sc Card Entitle Remaint Day");
			break;
		default:
			break;
		}
	return 0;
}

int32_t app_xinshimao_cas_api_osd_exec(void)
{
	app_xinshimao_cas_rolling_message_exec();
	//app_xinshimao_cas_email_flag_exec();
	//app_xinshimao_cas_finger_exec();
	return 1;
}



APP_CAS_API_ControlBlock_t XinShiMaoCaPortControlBlock=
{
	.m_cas_init_callback =  app_xinshimao_cas_api_init,
	.m_cas_close_callback = NULL,
	.m_cas_api_card_in_callback = app_xinshimao_cas_api_card_in,
	.m_cas_api_card_out_callback = app_xinshimao_cas_api_card_out,
	.m_cas_api_release_ecm_filter_callback= app_xinshimao_cas_api_release_ecm_filter,
	.m_cas_api_start_ecm_filter_callback = app_xinshimao_cas_api_start_ecm_filter,
	.m_cas_api_release_emm_filter_callback = app_xinshimao_cas_api_release_emm_filter,
	.m_cas_api_start_emm_filter_callback = app_xinshimao_cas_api_start_emm_filter,
	.m_cas_api_check_cat_ca_descriptor_valid_callback= app_xinshimao_cas_api_check_cat_ca_descriptor_valid,
	.m_cas_api_check_pmt_ca_descriptor_valid_callback= app_xinshimao_cas_api_check_pmt_ca_descriptor_valid,
	.m_cas_api_is_ca_pop_msg_type_callback= app_xinshimao_cas_api_is_ca_pop_msg_type,
	.m_cas_api_pop_msg_exec_callback= app_xinshimao_cas_api_pop_msg_exec,
	.m_cas_api_osd_exec_callback= app_xinshimao_cas_api_osd_exec,
	.m_cas_api_gxmsg_ca_on_event_exec_callback= app_xinshimao_cas_gxmsg_ca_on_event_exec, 								
	.m_cas_api_init_data_callback = app_xinshimao_cas_api_init_data,
	.m_cas_api_get_data_callback = app_xinshimao_cas_api_get_data,
	.m_cas_api_get_count_callback = app_xinshimao_cas_api_get_count,
	.m_cas_api_delete_callback = app_xinshimao_cas_api_delete,
	.m_cas_api_delete_all_callback = app_xinshimao_cas_api_delete_all,
	.m_cas_api_buy_ipp_callback = NULL,//;
	.m_cas_api_get_lock_status_callback = NULL,
	.m_cas_api_feed_mother_child_card_callback = NULL,
	.m_cas_api_change_pin_callback = NULL,//,
	.m_cas_api_change_pin_callback = NULL,
	.m_cas_api_change_rating_callback = NULL,//,
	.m_cas_api_change_worktime_callback= NULL,//,
};




