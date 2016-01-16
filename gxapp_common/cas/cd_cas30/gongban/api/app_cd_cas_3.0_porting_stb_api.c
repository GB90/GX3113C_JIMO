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
#include "gxapp_sys_config.h"
#include "gxmsg.h"
#include "app_common_play.h"
#include "app_common_table_pmt.h"
#include "app_common_porting_stb_api.h"
#include "app_common_porting_ca_flash.h"
#include "app_common_porting_ca_demux.h"
#include "app_common_porting_ca_descramble.h"
#include "app_common_porting_ca_smart.h"

/*
* 永新视博CA接口头文件
*/
#include "app_cd_cas_3.0_api_demux.h"
#include "app_cd_cas_3.0_api_smart.h"
#include "app_cd_cas_3.0_api_entitle.h"
#include "app_cd_cas_3.0_api_detitle.h"
#include "app_cd_cas_3.0_api_baseinfo.h"
#include "app_cd_cas_3.0_api_worktime.h"
#include "app_cd_cas_3.0_api_ippv_slot.h"
#include "app_cd_cas_3.0_api_features.h"
#include "app_cd_cas_3.0_api_mother_child.h"
#include "app_cd_cas_3.0_api_ippv_info.h"
#include "app_cd_cas_3.0_api_rating.h"
#include "app_cd_cas_3.0_api_pin.h"
#include "app_cd_cas_3.0_api_paired.h"
#include "app_cd_cas_3.0_api_email.h"
#include "app_cd_cas_3.0_api_rolling_message.h"
#include "app_cd_cas_3.0_api_lock_service.h"
extern 	int32_t app_cd_cas30_gxmsg_ca_on_event_finger(GxMsgProperty0_OnEvent* event);
extern int32_t app_cd_cas30_gxmsg_ca_on_event_entitle(GxMsgProperty0_OnEvent* event);
extern 	int32_t app_cd_cas30_gxmsg_ca_on_event_cardupdating(GxMsgProperty0_OnEvent* event);
extern 	int32_t app_cd_cas30_gxmsg_ca_on_event_ippv_buy(GxMsgProperty0_OnEvent* event);
extern 	int32_t app_cd_cas30_gxmsg_ca_on_event_feed(GxMsgProperty0_OnEvent* event);
extern 	int32_t app_cd_cas30_finger_exec(void);
extern 	void app_cd_cas30_hide_ca_prompt(void);
extern void app_cd_cas30_show_ca_prompt(char* context);

extern void Init_CAS(unsigned char bDebugOpen);

/*
* 以下接口CA菜单调用
*/

uint8_t app_cd_cas30_api_init(void)
{
    Init_CAS(0);
	app_porting_ca_flash_init(64*1024*2);
	app_porting_ca_demux_init();
	app_porting_ca_descramble_init();
	app_cd_cas30_rolling_message_init();
	app_porting_ca_create_filter_task();
    
	if (CDCA_FALSE== CDCASTB_Init(GXOS_DEFAULT_PRIORITY))
	{
		CA_FAIL(" CDCASTB_Init  failed \n");
		return FALSE;
	}
//	app_cd_cas30_api_smart_init();
	app_porting_ca_smart_create_task();
	return TRUE;
}

/*
*  获取对应数据之前，初始化数据管理
(一般创建对应窗体之前或create中调用)。
* 如初始化邮件、授权等
*/
int32_t app_cd_cas30_api_init_data(dvb_ca_data_type_t date_type)
{
	int32_t dvb_ca_flag =  app_flash_get_config_dvb_ca_flag();
	if (dvb_ca_flag != DVB_CA_TYPE_CDCAS30)
		return FALSE;

	switch(date_type)
		{
			case DVB_CA_BASE_INFO:  /*基本信息*/
				return app_cd_cas30_api_init_baseinfo_data();
			case DVB_CA_OPERATOR_INFO: /*运营商信息*/
				break;
			case DVB_CA_EMAIL_INFO: /*邮件*/
				return app_cd_cas30_api_init_email_data();
			case DVB_CA_ENTITLE_INFO: /*授权*/		
				return app_cd_cas30_api_init_entitle_data();
			case DVB_CA_DETITLE_INFO: /*反授权*/		
				return app_cd_cas30_api_init_detitle_data();						
			case DVB_CA_CARD_UPDATE_INFO: /*卡升级*/
				break;
			case DVB_CA_FETURE_INFO: /*特征值*/
				return app_cd_cas30_api_init_features_data();
				break;
			case DVB_CA_IPPV_POP_INFO: /*IPPV购买框信息*/
				break;
			case DVB_CA_IPPT_POP_INFO: /*IPPT购买框信息*/
				break;
			case DVB_CA_IPPV_PROG_INFO: /*IPPV已购买节目信息*/
				return app_cd_cas30_api_init_ippv_info_data();				
				break;
			case DVB_CA_IPPV_SLOT_INFO: /*IPPV钱包信息*/
				return app_cd_cas30_api_init_ippv_slot_data();
				break;
//			case DVB_CA_CHILD_CARD_INFO: /*子卡信息*/
//				break;
			case DVB_CA_MOTHER_CARD_INFO: /*母卡信息*/
				return app_cd_cas30_api_init_mother_child_data();				
				break;
			case DVB_CA_CHANGE_PIN_INFO: /*修改密码信息*/
				break;
			case DVB_CA_PROGRESS_INFO: /*智能卡升级进度信息*/
				break;
			case DVB_CA_RATING_INFO: /*成人级别信息*/
				break;
			case DVB_CA_WORK_TIME_INFO: /*工作时段信息*/
				return app_cd_cas30_api_init_worktime_data();
			case DVB_CA_CURTAIN_INFO: /*窗帘信息*/
				break;
			case DVB_CA_PAIRED_INFO: /*配对信息*/
				return app_cd_cas30_api_init_paired_data();
			default:
				break;	
		}

	
	return FALSE;		
}

char * app_cd_cas30_api_get_data(ca_get_date_t *data)
{
	int32_t dvb_ca_flag =  app_flash_get_config_dvb_ca_flag();
	if (dvb_ca_flag != DVB_CA_TYPE_CDCAS30)
		return NULL;


	if (NULL == data)
		return NULL;	

	switch(data->date_type)
		{
			case DVB_CA_BASE_INFO:  /*基本信息*/
				return app_cd_cas30_api_get_baseinfo_data(data);							
			case DVB_CA_OPERATOR_INFO: /*运营商信息*/	
				break;
			case DVB_CA_EMAIL_INFO: /*邮件*/
				return app_cd_cas30_api_get_email_data(data);
			case DVB_CA_ENTITLE_INFO: /*授权*/		
				return app_cd_cas30_api_get_entitle_data(data);
			case DVB_CA_DETITLE_INFO: /*反授权*/		
				return app_cd_cas30_api_get_detitle_data(data);						
			case DVB_CA_CARD_UPDATE_INFO: /*卡升级*/
				break;
			case DVB_CA_FETURE_INFO: /*特征值*/
				return app_cd_cas30_api_get_features_data(data);				
				break;
			case DVB_CA_IPPV_POP_INFO: /*IPPV购买框信息*/
				break;
			case DVB_CA_IPPT_POP_INFO: /*IPPT购买框信息*/
				break;
			case DVB_CA_IPPV_PROG_INFO: /*IPPV已购买节目信息*/
				return app_cd_cas30_api_get_ippv_info_data(data);
				break;
			case DVB_CA_IPPV_SLOT_INFO: /*IPPV钱包信息*/
				return app_cd_cas30_api_get_ippv_slot_data(data);				
				break;
//			case DVB_CA_CHILD_CARD_INFO: /*子卡信息*/
//				break;
			case DVB_CA_MOTHER_CARD_INFO: /*母卡信息*/
				return app_cd_cas30_api_get_mother_child_data(data);				
				break;
			case DVB_CA_CHANGE_PIN_INFO: /*修改密码信息*/
				break;
			case DVB_CA_PROGRESS_INFO: /*智能卡升级进度信息*/
				break;
			case DVB_CA_RATING_INFO: /*成人级别信息*/
				return app_cd_cas30_api_get_rating_data(data);				
			case DVB_CA_WORK_TIME_INFO: /*工作时段信息*/
				return app_cd_cas30_api_get_worktime_data(data);				
			case DVB_CA_CURTAIN_INFO: /*窗帘信息*/
				break;
			case DVB_CA_PAIRED_INFO: /*配对信息*/
				return app_cd_cas30_api_get_paired_data(data);							
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
uint8_t app_cd_cas30_api_update_data(ca_get_date_t *data)
{
	int32_t dvb_ca_flag =  app_flash_get_config_dvb_ca_flag();

	if (dvb_ca_flag != DVB_CA_TYPE_CDCAS30)
		return 0;


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

uint8_t app_cd_cas30_api_get_count(ca_get_count_t *data)
{
	int32_t dvb_ca_flag =  app_flash_get_config_dvb_ca_flag();
	if (dvb_ca_flag != DVB_CA_TYPE_CDCAS30)
		return 0;

	if (NULL == data)
		return 0;
	

	switch(data->date_type)
		{
			case DVB_CA_BASE_INFO:  /*基本信息*/
				break;
			case DVB_CA_OPERATOR_INFO: /*运营商信息*/
				break;
			case DVB_CA_EMAIL_INFO: /*邮件*/
				return app_cd_cas30_api_get_email_count(data);
			case DVB_CA_ENTITLE_INFO: /*授权*/
				return app_cd_cas30_api_get_entitle_count(data);
			case DVB_CA_DETITLE_INFO: /*反授权*/	
				return app_cd_cas30_api_get_detitle_count(data);													
			case DVB_CA_EMERGENCY_INFO: /*应急广播*/
				break;
			case DVB_CA_ROLLING_INFO: /*OSD滚动消息*/
				break;
			case DVB_CA_FINGER_INFO: /*指纹*/
				break;
			case DVB_CA_CARD_UPDATE_INFO: /*卡升级*/
				break;
			case DVB_CA_FETURE_INFO: /*特征值*/
				return app_cd_cas30_api_get_features_count(data);				
				break;
			case DVB_CA_IPPV_POP_INFO: /*IPPV购买框信息*/
				break;
			case DVB_CA_IPPT_POP_INFO: /*IPPT购买框信息*/
				break;
			case DVB_CA_IPPV_PROG_INFO: /*IPPV已购买节目信息*/
				return app_cd_cas30_api_get_ippv_info_count(data);				
				break;
			case DVB_CA_IPPV_SLOT_INFO: /*IPPV钱包信息*/
				return app_cd_cas30_api_get_ippv_slot_count(data);
				break;
//			case DVB_CA_CHILD_CARD_INFO: /*子卡信息*/
//				break;
			case DVB_CA_MOTHER_CARD_INFO: /*母卡信息*/
				return app_cd_cas30_api_get_mother_child_count(data);				
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

bool app_cd_cas30_api_delete(ca_delete_data_t *data)
{
	int32_t dvb_ca_flag =  app_flash_get_config_dvb_ca_flag();
	if (dvb_ca_flag != DVB_CA_TYPE_CDCAS30)
		return FALSE;


	if (NULL == data)
		return FALSE;
	
	switch(data->date_type)
		{
			case DVB_CA_EMAIL_INFO: /*邮件*/	
				return app_cd_cas30_api_delete_email(data);
			case DVB_CA_DETITLE_INFO: /*反授权*/	
				return app_cd_cas30_api_delete_detitle(data);	
			default:
				break;	
		}
	
	return 0;	
}

bool app_cd_cas30_api_delete_all(ca_delete_data_t *data)
{
	int32_t dvb_ca_flag =  app_flash_get_config_dvb_ca_flag();
	if (dvb_ca_flag != DVB_CA_TYPE_CDCAS30)
		return FALSE;


	if (NULL == data)
		return FALSE;
	

	switch(data->date_type)
		{
			case DVB_CA_EMAIL_INFO: /*邮件*/	
				return app_cd_cas30_api_delete_all_email(data);
			case DVB_CA_DETITLE_INFO: /*反授权*/	
				return app_cd_cas30_api_delete_all_detitle(data);	
			default:
				break;	
		}

	
	return 0;		
}

int32_t app_cd_cas30_gxmsg_ca_on_event_exec(GxMessage * msg)
{
	switch(msg->msg_id)
		{
			case GXMSG_CA_ON_EVENT:
			{
				GxMsgProperty0_OnEvent* event;
				event = GxBus_GetMsgPropertyPtr(msg, GxMsgProperty0_OnEvent);
				printf("app_cd_cas30_gxmsg_ca_on_event_exec GXMSG_CA_ON_EVENT event->key=%d\n",event->key);
				switch (event->key) {

					case DVB_CA_BASE_INFO:		       /*基本信息*/
						break;
					case DVB_CA_OPERATOR_INFO:		/*运营商信息*/	
						break;
					case DVB_CA_EMAIL_INFO:		/*邮件*/
						{
							app_cd_cas30_gxmsg_ca_on_event_email(event);
						}
						break;
					case DVB_CA_ENTITLE_INFO: /*授权*/							
						{
							app_cd_cas30_gxmsg_ca_on_event_entitle(event);
						}
						break;
					case DVB_CA_DETITLE_INFO: /*反授权*/							
						{
							app_cd_cas30_gxmsg_ca_on_event_detitle(event);
						}
						break;
					case DVB_CA_EMERGENCY_INFO:           /*应急广播*/
						break;
					case DVB_CA_ROLLING_INFO: /*OSD滚动消息*/
						{
							app_cd_cas30_gxmsg_ca_on_event_rolling(event);
						}
						break;
					case DVB_CA_FINGER_INFO:                  /*指纹*/
							{
							app_cd_cas30_gxmsg_ca_on_event_finger(event);
						}
						break;
					case DVB_CA_CARD_UPDATE_INFO:       /*卡升级*/
							{
								app_cd_cas30_gxmsg_ca_on_event_cardupdating(event);
						}
						break;
					case DVB_CA_FETURE_INFO:                  /*特征值*/
						break;
					case DVB_CA_IPPV_POP_INFO:                /*IPPV购买框信息*/
							{
						app_cd_cas30_gxmsg_ca_on_event_ippv_buy(event);
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
								app_cd_cas30_gxmsg_ca_on_event_feed(event);								
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

uint8_t app_cd_cas30_api_is_ca_pop_msg_type(uint8_t type)
{
	if ((type >= MSG_POP_COMMON_END+CDCA_MESSAGE_CANCEL_TYPE+1)&&(type <=MSG_POP_COMMON_END+CDCA_MESSAGE_STBFREEZE_TYPE+1))
		return TRUE;
	
	return FALSE;
}

int32_t app_cd_cas30_api_pop_msg_exec(uint8_t type)
{
	switch(type-MSG_POP_COMMON_END-1)
		{
		case CDCA_MESSAGE_CANCEL_TYPE: /* 取消当前的显示 */
			app_cd_cas30_hide_ca_prompt();
			break;
		case CDCA_MESSAGE_BADCARD_TYPE: /* 无法识别卡 */
			app_cd_cas30_show_ca_prompt("Unidentified Card");
			break;
		case CDCA_MESSAGE_EXPICARD_TYPE: /* 智能卡过期，请更换新卡 */
			app_cd_cas30_show_ca_prompt("Card Is Expire");
			break;
		case CDCA_MESSAGE_INSERTCARD_TYPE: /* 加扰节目，请插入智能卡 */
			app_cd_cas30_show_ca_prompt("Please insert the smart card");
			break;
		case CDCA_MESSAGE_NOOPER_TYPE: /* 卡中不存在节目运营商 */
			app_cd_cas30_show_ca_prompt("Operator ID error");
			break;
		case CDCA_MESSAGE_BLACKOUT_TYPE: /* 条件禁播 */
			app_cd_cas30_show_ca_prompt("Conditional Blackout");
			break;
		case CDCA_MESSAGE_OUTWORKTIME_TYPE: /* 当前时段被设定为不能观看 */
			app_cd_cas30_show_ca_prompt("Out of Working Hours");
			break;
		case CDCA_MESSAGE_WATCHLEVEL_TYPE: /* 节目级别高于设定的观看级别 */
			app_cd_cas30_show_ca_prompt("Out of Teleview Rating");
			break;
		case CDCA_MESSAGE_PAIRING_TYPE: /* 智能卡与本机顶盒不对应 */
			app_cd_cas30_show_ca_prompt("The card is not paired with this STB");
			break;
		case CDCA_MESSAGE_NOENTITLE_TYPE: /* 没有授权 */
			app_cd_cas30_show_ca_prompt("No Entitlement");
			break;
		case CDCA_MESSAGE_DECRYPTFAIL_TYPE: /* 节目解密失败 */
			app_cd_cas30_show_ca_prompt("Decrypt fail");
			break;
		case CDCA_MESSAGE_NOMONEY_TYPE: /* 卡内金额不足 */
			app_cd_cas30_show_ca_prompt("No enough money");
			break;
		case CDCA_MESSAGE_ERRREGION_TYPE: /* 区域不正确 */
			app_cd_cas30_show_ca_prompt("Regional Lockout");
			break;
		case CDCA_MESSAGE_NEEDFEED_TYPE: /* 子卡需要和母卡对应，请插入母卡 */
			app_cd_cas30_show_ca_prompt("Please insert the parent-card of the current card");
			break;
		case CDCA_MESSAGE_ERRCARD_TYPE: /* 智能卡校验失败，请联系运营商 */
			app_cd_cas30_show_ca_prompt("Card upgrade failed.Please contact the operator");		
			break;
		case CDCA_MESSAGE_UPDATE_TYPE: /* 智能卡升级中，请不要拔卡或者关机 */
			app_cd_cas30_show_ca_prompt("Card in patching. Not to remove the card or power off");
			break;
		case CDCA_MESSAGE_LOWCARDVER_TYPE: /* 请升级智能卡 */
			app_cd_cas30_show_ca_prompt("Please upgrade the smart card");
			break;
		case CDCA_MESSAGE_VIEWLOCK_TYPE: /* 请勿频繁切换频道 */
			app_cd_cas30_show_ca_prompt("Please do not switch channels too frequently");
			break;
		case CDCA_MESSAGE_MAXRESTART_TYPE: /* 智能卡暂时休眠，请5分钟后重新开机 */
			app_cd_cas30_show_ca_prompt("Card is in dormancy. Please insert the card after 5 minutes");
			break;
		case CDCA_MESSAGE_FREEZE_TYPE: /*智能卡已冻结，请联系运营商 */
			app_cd_cas30_show_ca_prompt("Card is freezed. Please contact the operator");
			break;
		case CDCA_MESSAGE_CALLBACK_TYPE: /*智能卡已暂停，请回传收视记录给运营商 */
			app_cd_cas30_show_ca_prompt("Please send the IPPV records back to the operator");
			break;
		case CDCA_MESSAGE_CURTAIN_TYPE: /*窗帘节目，不可预览阶段*/
			app_cd_cas30_show_ca_prompt("Curtain Program.Can't be watched now");
			break;
		case CDCA_MESSAGE_CARDTESTSTART_TYPE: /*升级测试卡测试中...*/
			app_cd_cas30_show_ca_prompt("Card test start");
			break;
		case CDCA_MESSAGE_CARDTESTFAILD_TYPE: /*升级测试卡测试失败，请检查机卡通讯模块*/
			app_cd_cas30_show_ca_prompt("Card test failed");
			break;
		case CDCA_MESSAGE_CARDTESTSUCC_TYPE: /*升级测试卡测试成功*/
			app_cd_cas30_show_ca_prompt("Card test successfully");
			break;
		case CDCA_MESSAGE_NOCALIBOPER_TYPE: /*卡中不存在移植库定制运营商*/
			app_cd_cas30_show_ca_prompt("No CA Lib Operator");
			break;
		case CDCA_MESSAGE_STBLOCKED_TYPE: /*请重启机顶盒*/
			app_cd_cas30_show_ca_prompt("Please restart the STB");
			break;
		case CDCA_MESSAGE_STBFREEZE_TYPE: /*机顶盒被冻结*/
			app_cd_cas30_show_ca_prompt("STB is freezed. Please contact the operator");
			break;
		default:
			break;
		}
	return 0;
}

int32_t app_cd_cas30_api_osd_exec(void)
{
	app_cd_cas30_rolling_message_exec();
	app_cd_cas30_email_flag_exec();
	app_cd_cas30_detitle_flag_exec();
	app_cd_cas30_finger_exec();	
	return 1;
}



APP_CAS_API_ControlBlock_t CdCas30PortControlBlock=
{
	.m_cas_init_callback =  app_cd_cas30_api_init,
	.m_cas_close_callback = NULL,
	.m_cas_api_card_in_callback = app_cd_cas30_api_card_in,
	.m_cas_api_card_out_callback = app_cd_cas30_api_card_out,
	.m_cas_api_release_ecm_filter_callback= app_cd_cas30_api_release_ecm_filter,
	.m_cas_api_start_ecm_filter_callback = app_cd_cas30_api_start_ecm_filter,
	.m_cas_api_release_emm_filter_callback = app_cd_cas30_api_release_emm_filter,
	.m_cas_api_start_emm_filter_callback = app_cd_cas30_api_start_emm_filter,
	.m_cas_api_check_cat_ca_descriptor_valid_callback= app_cd_cas30_api_check_cat_ca_descriptor_valid,
	.m_cas_api_check_pmt_ca_descriptor_valid_callback= app_cd_cas30_api_check_pmt_ca_descriptor_valid,
	.m_cas_api_is_ca_pop_msg_type_callback= app_cd_cas30_api_is_ca_pop_msg_type,
	.m_cas_api_pop_msg_exec_callback= app_cd_cas30_api_pop_msg_exec,
	.m_cas_api_osd_exec_callback= app_cd_cas30_api_osd_exec,
	.m_cas_api_gxmsg_ca_on_event_exec_callback= app_cd_cas30_gxmsg_ca_on_event_exec, 								
	.m_cas_api_init_data_callback = app_cd_cas30_api_init_data,
	.m_cas_api_get_data_callback = app_cd_cas30_api_get_data,
	.m_cas_api_get_count_callback = app_cd_cas30_api_get_count,
	.m_cas_api_delete_callback = app_cd_cas30_api_delete,
	.m_cas_api_delete_all_callback = app_cd_cas30_api_delete_all,
	.m_cas_api_buy_ipp_callback = NULL,//app_cd_cas30_api_buy_ipp;
	.m_cas_api_get_lock_status_callback =	app_cd_cas30_api_get_lock_status,
	.m_cas_api_feed_mother_child_card_callback = app_cd_cas30_api_feed_mother_child_card,
	.m_cas_api_change_pin_callback = app_cd_cas30_api_change_pin,
	.m_cas_api_verify_pin_callback = NULL,
	.m_cas_api_change_rating_callback = app_cd_cas30_api_change_rating,
	.m_cas_api_change_worktime_callback= app_cd_cas30_api_change_worktime,
};




