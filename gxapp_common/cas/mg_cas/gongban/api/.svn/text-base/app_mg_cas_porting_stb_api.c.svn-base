/*****************************************************************************
* 						   CONFIDENTIAL
*        Hangzhou GuoXin Science and Technology Co., Ltd.
*                      (C)2014, All right reserved
******************************************************************************

******************************************************************************
* File Name :	app_mg_cas_porting_stb_api.c
* Author    : 	zhouhm
* Project   :	goxceed dvbc
* Type      :
******************************************************************************
* Purpose   :	
******************************************************************************
* Release History:
VERSION	Date			  AUTHOR         Description
1.0  	2014.04.11		  zhouhm 	 			creation
*****************************************************************************/
#include <gxtype.h>
#include "gxapp_sys_config.h"
#include "gxmsg.h"
#include "mgdef.h"
#include "app_common_play.h"
#include "app_common_table_pmt.h"
#include "app_common_porting_stb_api.h"
#include "app_common_porting_ca_flash.h"
#include "app_common_porting_ca_demux.h"
#include "app_common_porting_ca_descramble.h"
#include "app_common_porting_ca_smart.h"
#include "app_mg_cas_api_lock_service.h"
#include "app_mg_cas_api_baseinfo.h"
#include "app_mg_cas_api_demux.h"
#include "app_mg_cas_api_smart.h"
#include "app_mg_cas_api_rolling_message.h"
#include "app_mg_cas_api_baseinfo.h"
#include "app_mg_cas_api_ppv.h"
#include "app_mg_cas_api_event.h"
#include "app_mg_cas_api_email.h"

extern 	void app_mg_cas_hide_ca_prompt(void);
extern void app_mg_cas_show_ca_prompt(char* context);

/*
* 以下接口CA菜单调用
*/

#define MGCAS_LIB_STATUS      MG_LIB_FORMAL//	MG_LIB_TEST /* MG_LIB_FORMAL */


//初始化
uint8_t app_mg_cas_api_init(void)
{
	app_porting_ca_flash_init(EMail_SIZE);//flash空间初始化
	app_porting_ca_demux_init();//初始化CA过滤器
	app_porting_ca_descramble_init();
	app_mg_cas_rolling_message_init();
    app_mg_cas_api_init_email_data();//email数据初始化
	app_porting_ca_create_filter_task();//ca过滤器任务
	MG_Init_CAS(MGCAS_LIB_STATUS);
	app_porting_ca_smart_create_task();//智能卡状态检测任

	return TRUE;
}

/*
*  获取对应数据之前，初始化数据管理
(一般创建对应窗体之前或create中调用)。
* 如初始化邮件、授权等
*/
int32_t app_mg_cas_api_init_data(dvb_ca_data_type_t date_type)
{
	int32_t dvb_ca_flag =  app_flash_get_config_dvb_ca_flag();
	if (dvb_ca_flag != DVB_CA_TYPE_MG)
		return FALSE;

	switch(date_type)
		{
			case DVB_CA_BASE_INFO:  /*基本信息*/
			    return app_mg_cas_api_init_baseinfo_data();
			case DVB_CA_OPERATOR_INFO: /*运营商信息*/
				break;
			case DVB_CA_EMAIL_INFO: /*邮件*/
				return app_mg_cas_api_init_email_data();
			case DVB_CA_ENTITLE_INFO: /*授权*/		
				break;
			case DVB_CA_DETITLE_INFO: /*反授权*/	
				break;					
			case DVB_CA_CARD_UPDATE_INFO: /*卡升级*/
				break;
			case DVB_CA_FETURE_INFO: /*特征值*/
				break;
//				return app_mg_cas_api_init_features_data();
			case DVB_CA_IPPV_POP_INFO: /*IPPV购买框信息*/
                return app_mg_cas_api_init_ppv_data();
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

	
	return FALSE;		
}


uint8_t app_mg_cas_api_buy_ipp(ca_buy_ipp_t *data)
{
    int32_t dvb_ca_flag =  app_flash_get_config_dvb_ca_flag();
	if (dvb_ca_flag != DVB_CA_TYPE_MG)
		return 0;


	if (NULL == data)
		return 0;	

	switch(data->date_type)
	{
    case DVB_CA_IPPV_POP_INFO: /*IPPV购买框信息*/
         return app_mg_cas_api_buy_ppv_data(data);
    default:
		break;	
	}
    
    return 1;
}



char * app_mg_cas_api_get_data(ca_get_date_t *data)
{
	int32_t dvb_ca_flag =  app_flash_get_config_dvb_ca_flag();
	if (dvb_ca_flag != DVB_CA_TYPE_MG)
		return NULL;


	if (NULL == data)
		return NULL;	

	switch(data->date_type)
		{
			case DVB_CA_BASE_INFO:  /*基本信息*/
			    return app_mg_cas_api_get_baseinfo_data(data);							
			case DVB_CA_OPERATOR_INFO: /*运营商信息*/	
				break;
			case DVB_CA_EMAIL_INFO: /*邮件*/
                return app_mg_cas_api_get_email_data(data); 
			case DVB_CA_ENTITLE_INFO: /*授权*/
                return app_mg_cas_api_get_entitle_data(data);	
				break;
			case DVB_CA_DETITLE_INFO: /*反授权*/
				break;									
			case DVB_CA_CARD_UPDATE_INFO: /*卡升级*/
				break;
			case DVB_CA_FETURE_INFO: /*特征值*/
				break;			
			case DVB_CA_IPPV_POP_INFO: /*IPPV购买框信息*/
                return app_mg_cas_api_get_ppv_data(data);
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
//				return app_mg_cas_api_get_mother_child_data(data);				
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

	
	return NULL;	
}

/*
*  更新CA数据
(CA数据发生变更时调用)。
* 如删除邮件、反授权，CA通知授权信息变化等
*/
uint8_t app_mg_cas_api_update_data(ca_get_date_t *data)
{
	int32_t dvb_ca_flag =  app_flash_get_config_dvb_ca_flag();

	if (dvb_ca_flag != DVB_CA_TYPE_MG)
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

uint8_t app_mg_cas_api_get_count(ca_get_count_t *data)
{
	int32_t dvb_ca_flag =  app_flash_get_config_dvb_ca_flag();
	if (dvb_ca_flag != DVB_CA_TYPE_MG)
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
			return app_mg_cas_api_get_email_count(data);
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
//				return app_mg_cas_api_get_features_count(data);				
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
//				return app_mg_cas_api_get_mother_child_count(data);				
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

bool app_mg_cas_api_delete(ca_delete_data_t *data)
{
	int32_t dvb_ca_flag =  app_flash_get_config_dvb_ca_flag();
	if (dvb_ca_flag != DVB_CA_TYPE_MG)
		return FALSE;


	if (NULL == data)
		return FALSE;
	
	switch(data->date_type)
	{
		case DVB_CA_EMAIL_INFO: /*邮件*/
			return app_mg_cas_api_delete_email(data);
		case DVB_CA_DETITLE_INFO: /*反授权*/
			break;	
		default:
			break;	
	}
	
	return 0;	
}

bool app_mg_cas_api_delete_all(ca_delete_data_t *data)
{
	int32_t dvb_ca_flag =  app_flash_get_config_dvb_ca_flag();
	if (dvb_ca_flag != DVB_CA_TYPE_MG)
		return FALSE;


	if (NULL == data)
		return FALSE;
	

	switch(data->date_type)
	{
		case DVB_CA_EMAIL_INFO: /*邮件*/		
			return app_mg_cas_api_delete_all_email(data);
		case DVB_CA_DETITLE_INFO: /*反授权*/	
			break;
		default:
			break;	
	}

	
	return 0;		
}


//事件处理
//ca事件处理
int32_t app_mg_cas_gxmsg_ca_on_event_exec(GxMessage * msg)
{
	switch(msg->msg_id)
	{
		case GXMSG_CA_ON_EVENT:
    	{
    		GxMsgProperty0_OnEvent* event;
    		event = GxBus_GetMsgPropertyPtr(msg, GxMsgProperty0_OnEvent);
    		//printf("app_mg_cas_gxmsg_ca_on_event_exec GXMSG_CA_ON_EVENT ,key=%d\n",event->key);
    		switch (event->key) {


					case DVB_CA_BASE_INFO:		       /*基本信息*/
						break;
					case DVB_CA_OPERATOR_INFO:		/*运营商信息*/	
						break;
					case DVB_CA_EMAIL_INFO:		/*邮件图标显示*/
						app_mg_cas_gxmsg_ca_on_event_email(event);
						break;
					case DVB_CA_ENTITLE_INFO: /*授权提示框*/
                        printf("event,key: DVB_CA_ENTITLE_INFO\n");
                        app_mg_cas_gxmsg_ca_on_event_entitle(event);
						break;
					case DVB_CA_DETITLE_INFO: /*反授权*/							
						break;
					case DVB_CA_EMERGENCY_INFO:           /*应急广播*/
						break;
					case DVB_CA_ROLLING_INFO: /*OSD滚动消息*/
						{
                            app_mg_cas_gxmsg_ca_on_event_msg(event);
						}
						break;
					case DVB_CA_FINGER_INFO:                  /*指纹*/
                        printf("event: DVB_CA_FINGER_INFO\n");
                        app_mg_cas_gxmsg_ca_on_event_finger(event);
						break;
					case DVB_CA_CARD_UPDATE_INFO:       /*卡升级*/
						break;
					case DVB_CA_FETURE_INFO:                  /*特征值*/
						break;
					case DVB_CA_IPPV_POP_INFO:  /*IPPV购买框信息*/
                        printf("event: DVB_CA_IPPV_POP_INFO\n");
						app_mg_cas_gxmsg_ca_on_event_ippv_logo(event);
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

uint8_t app_mg_cas_api_is_ca_pop_msg_type(uint8_t type)
{
	if ((type >= MSG_POP_COMMON_END+CA_MG_SUCCESS+1)&&(type <=MSG_POP_COMMON_END+CA_MG_EMM_MODIFY_CTRLCODE+1))
		return TRUE;
	
	return FALSE;
}

//消息提示框
int32_t app_mg_cas_api_pop_msg_exec(uint8_t type)
{
	switch(type-MSG_POP_COMMON_END-1)
	{
		case CA_MG_SUCCESS: /* 取消当前的显示 */
			app_mg_cas_hide_ca_prompt();
			break;
		case CA_MG_ERR_SCARD_NOT_ACT: /* 请激活子卡 */
			app_mg_cas_show_ca_prompt("Err Scard Not Act");
			break;
		case CA_MG_ERR_ZONE_CODE: /* 智能卡区域码错误 */
			app_mg_cas_show_ca_prompt("Err Zone Code");
			break;
		case CA_MG_ERR_NO_MONEY: /* 智能卡余额不足 */
			app_mg_cas_show_ca_prompt("Err No Money");
			break;
		case CA_MG_ERR_NOT_AUTHORED: /* 本节目未授权 */
			app_mg_cas_show_ca_prompt("Err Not Authored");
			break;
		case CA_MG_ERR_CARD_LOCKED: /* 智能卡锁定 */
			app_mg_cas_show_ca_prompt("Error Card Locked");
			break;
		case CA_MG_ERR_APP_LOCKED: /* 服务锁定 */
			app_mg_cas_show_ca_prompt("Err App Locked");
			break;
		case CA_MG_ERR_PIN_AUTHEN: /* 机卡配对错误： */
			{
				MG_U8 PinErrSta, TryTimes;
				PinErrSta=MG_Get_PinErr_Status(&TryTimes);
				char strTemp[50]={0};
				if(PinErrSta==MG_PINERR_STB_PAIRED)
				{
					/*智能卡未配对，机顶盒已配对*/
					app_mg_cas_show_ca_prompt("Err Pin Authen Stb Paired");
				}
				else if(PinErrSta==MG_PINERR_CARD_PAIRED)
				{
					/*智能卡已配对，机顶盒未配对*/
					app_mg_cas_show_ca_prompt("Err Pin Authen Card Paired");			
				}
				else
				{
					sprintf(strTemp,"智能卡再插入%d次后将被锁定",
					TryTimes);
					app_mg_cas_show_ca_prompt(strTemp);
				}		
			}	
			break;
		case CA_MG_ERR_EXTER_AUTHEN: /* 智能卡认证失败 */
			app_mg_cas_show_ca_prompt("Err Exter Authen");
			break;
		case CA_MG_ERR_GET_CARD_DATA: /* 智能卡数据错误 */
			app_mg_cas_show_ca_prompt("Err Get Card Data");
			break;
		case CA_MG_ERR_ILLEGAL_CARD: /* 智能卡通信错误*/
			app_mg_cas_show_ca_prompt("Err Illegal Card");
			break;
		case CA_MG_ERR_DONOT_TELEVIEW_PPV: /* PPV节目，请按【？】键订购 */
			app_mg_cas_show_ca_prompt("Err Don't Teleview PPV");
			break;
		case CA_MG_ERR_CARD_NOTFOUND: /* 请插入智能卡 */
			app_mg_cas_show_ca_prompt("Error Card Not Found");
			break;
		case CA_MG_ERR_CARD_NOTMATCH: /* 智能卡不匹配 */
			app_mg_cas_show_ca_prompt("Err Card Not Match");
			break;
		case CA_MG_ERR_UPDATE_GK: /* 智能卡升级失败 */
			app_mg_cas_show_ca_prompt("Err Update GK");
			break;
		case CA_MG_ERR_CONTROL_PLAY: /* 本节目条件限播*/
			app_mg_cas_show_ca_prompt("Err Control Play");
			break;
		case CA_MG_ERR_AUTH_CLOSED: /* 本节目授权关闭 */
			app_mg_cas_show_ca_prompt("Err Auth Closed");
			break;
		case CA_MG_ERR_AUTH_OVERDUE: /* 智能卡授权过期*/
			app_mg_cas_show_ca_prompt("Err Auth Overdue");
			break;
		case CA_MG_ERR_AUTH_UPDATE: /* 授权刷新，请稍候*/
			app_mg_cas_show_ca_prompt("Err Auth Update");
			break;
		case CA_MG_ERR_ILLEGAL_AUTH: /*智能卡授权错误*/
			app_mg_cas_show_ca_prompt("Err Illegal Auth");
			break;
		case CA_MG_ERR_CARD_STATUS_RENEW: /*智能卡状态更新，请重启机顶盒*/
			app_mg_cas_show_ca_prompt("Err Card Status Renew");
			break;
		case CA_MG_ERR_CARD_OPERATE: /*智能卡操作失败*/
			app_mg_cas_show_ca_prompt("Err Card Operate");
			break;
		case CA_MG_EMM_APP_STOP: /*智能卡暂停服务*/
			app_mg_cas_show_ca_prompt("Emm App Stop");
			break;
		case CA_MG_EMM_APP_RESUME: /*智能卡恢复服务*/
			app_mg_cas_show_ca_prompt("Emm App Resume");
			break;
		case CA_MG_EMM_PROG_UNAUTH: /*取消节目订购*/
			app_mg_cas_show_ca_prompt("Emm Prog UnAuth");
			break;
		case CA_MG_EMM_PROG_AUTH: /*增加节目订购*/
			app_mg_cas_show_ca_prompt("Emm Prog Auth");
			break;
		case CA_MG_EMM_EN_PARTNER: /*启用机卡配对*/
			app_mg_cas_show_ca_prompt("Emm En Partner");
			break;
		case CA_MG_EMM_DIS_PARTNER: /*取消机卡配对*/
			app_mg_cas_show_ca_prompt("Emm Dis Partner");
			break;
		case CA_MG_EMM_SONCARD: /*设置子母卡*/
			app_mg_cas_show_ca_prompt("Emm SonCard");
			break;
		case CA_MG_EMM_UPDATE_GNO: /*智能卡组号更新*/
			app_mg_cas_show_ca_prompt("Emm Update Gno");
			break;
		case CA_MG_EMM_UPDATE_GK: /*智能卡升级，请重启机顶盒*/
			app_mg_cas_show_ca_prompt("Emm Update GK");
			break;
		case CA_MG_EMM_CHARGE:
			{
                MG_U32 ChgAmt;
                char strTemp[50]={0};

                ChgAmt=MG_Get_ChargeAmount();
                sprintf(strTemp,"电子钱包充值成功，充值金额为%d点",ChgAmt);
                app_mg_cas_show_ca_prompt(strTemp);
			}
			break;
		case CA_MG_EMM_URGENT_SERVICE: /*应急广播*/
			app_mg_cas_show_ca_prompt("Emm Urgent Service");
			break;
		case CA_MG_EMM_MODIFY_ZONECODE: /*智能卡区域码更新*/
			app_mg_cas_show_ca_prompt("Emm Modify ZoneCode");
			break;
		case CA_MG_EMM_UPDATE_CRT: /*授权更新*/
			app_mg_cas_show_ca_prompt("Emm Update Crt");
			break;
		case CA_MG_EMM_MODIFY_CTRLCODE: /*智能卡限播码更新*/
			app_mg_cas_show_ca_prompt("Emm Modify CtrlCode");
			break;
		default:
			break;
		}
	return 0;
}

//200ms定时器回调
int32_t app_mg_cas_api_osd_exec(void)
{
	app_mg_cas_rolling_message_exec();//滚动消息控制
	app_mg_cas_email_flag_exec();// 邮件图标显示、隐藏控制
//	app_mg_cas_detitle_flag_exec();//授权信息
	app_mg_cas_finger_exec();//指纹信息显示控制
    
	return 1;
}



APP_CAS_API_ControlBlock_t MgCasPortControlBlock=
{
	.m_cas_init_callback =  app_mg_cas_api_init,//ca模块初始化
	.m_cas_close_callback = NULL,
	.m_cas_api_card_in_callback = app_mg_cas_api_card_in,//智能卡插入
	.m_cas_api_card_out_callback = app_mg_cas_api_card_out,//智能卡移除
	.m_cas_api_release_ecm_filter_callback= app_mg_cas_api_release_ecm_filter,
	.m_cas_api_start_ecm_filter_callback = app_mg_cas_api_start_ecm_filter,//ecm过滤器
	.m_cas_api_release_emm_filter_callback = app_mg_cas_api_release_emm_filter,
	.m_cas_api_start_emm_filter_callback = app_mg_cas_api_start_emm_filter,//emm过滤器设置
	.m_cas_api_check_cat_ca_descriptor_valid_callback= app_mg_cas_api_check_cat_ca_descriptor_valid,//ca 模块cat表描述字校验
	.m_cas_api_check_pmt_ca_descriptor_valid_callback= app_mg_cas_api_check_pmt_ca_descriptor_valid,//ca模块pmt表描述字校验
	.m_cas_api_is_ca_pop_msg_type_callback= app_mg_cas_api_is_ca_pop_msg_type,//ca消息判断
	.m_cas_api_pop_msg_exec_callback= app_mg_cas_api_pop_msg_exec,//播放ca消息框
	.m_cas_api_osd_exec_callback= app_mg_cas_api_osd_exec,//200ms定时器任务
	.m_cas_api_gxmsg_ca_on_event_exec_callback= app_mg_cas_gxmsg_ca_on_event_exec, 	//ca事件处理							
    /********************************************/
    .m_cas_api_init_data_callback = app_mg_cas_api_init_data,//获取数据前的初始化操作
	.m_cas_api_get_data_callback = app_mg_cas_api_get_data,//获取数据接口
	.m_cas_api_get_count_callback = app_mg_cas_api_get_count,//获取整形数据值
	.m_cas_api_delete_callback = app_mg_cas_api_delete,//删除单封邮件操作
	.m_cas_api_delete_all_callback = app_mg_cas_api_delete_all,//删除所有邮件
	.m_cas_api_buy_ipp_callback = app_mg_cas_api_buy_ipp,//iptv接口 (设置智能卡)
	.m_cas_api_get_lock_status_callback = app_mg_cas_api_get_lock_status,//应急广播状态
	.m_cas_api_feed_mother_child_card_callback = NULL,
	.m_cas_api_change_pin_callback = NULL,
	.m_cas_api_verify_pin_callback = NULL,
	.m_cas_api_change_rating_callback = NULL,
	.m_cas_api_change_worktime_callback= NULL,
};




