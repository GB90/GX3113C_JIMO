/*****************************************************************************
* 						   CONFIDENTIAL
*        Hangzhou GuoXin Science and Technology Co., Ltd.
*                      (C)2014, All right reserved
******************************************************************************

******************************************************************************
* File Name :	app_mg_cas312_porting_stb_api.c
* Author    : 	wangjian
* Project   :	runde 3113c
* Type      :
******************************************************************************
* Purpose   :	
******************************************************************************
* Release History:
VERSION	Date			  AUTHOR         Description
1.0  	2014.12.16		  zhouhm 	 			creation
2.0  2014.12.16              wangjian         modify for Runde 3113c mgcas3.1.2
*****************************************************************************/
#include <gxtype.h>
#include "gxapp_sys_config.h"
#include "gxmsg.h"
#include "mg312def.h"
#include "app_common_porting_stb_api.h"
#include "app_common_play.h"
#include "app_common_porting_ca_demux.h"
#include "app_common_porting_ca_descramble.h"
#include "app_common_porting_ca_smart.h"
#include "app_common_porting_ca_flash.h"

#include "app_mg_cas312_api_demux.h"
#include "app_mg_cas312_api_smart.h"
#include "app_mg_cas312_api_baseinfo.h"
#include "app_mg_cas312_api_lock_service.h"
#include "app_mg_cas312_api_email.h"
#include "app_mg_cas312_api_event.h"
#include "app_mg_cas312_api_rolling_message.h"
#include "app_mg_cas312_api_ppv.h"


/*Marcos ----------------------------------------------------------*/
#define MGCAS_LIB_STATUS  (MG_LIB_FORMAL)

/*Extern ---------------------------------------------------------*/
extern 	void app_mg_cas312_hide_ca_prompt(void);
extern void app_mg_cas312_show_ca_prompt(char* context);


/*API---------------------------------------------------------------*/
uint8_t app_mg_cas312_api_init(void)
{
	app_porting_ca_flash_init(EMail_SIZE);//flash空间初始化
	app_porting_ca_demux_init();//初始化CA过滤器
	app_porting_ca_descramble_init();
	app_mg_cas312_rolling_message_init();
    app_mg_cas312_api_init_email_data();//email数据初始化
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
int32_t app_mg_cas312_api_init_data(dvb_ca_data_type_t date_type)
{
	int32_t dvb_ca_flag =  app_flash_get_config_dvb_ca_flag();
	if (dvb_ca_flag != DVB_CA_TYPE_MG312)
	{
		return FALSE;
	}

	switch (date_type)
	{
		case DVB_CA_BASE_INFO:  /*基本信息*/
		    return app_mg_cas312_api_init_baseinfo_data();
			
		case DVB_CA_EMAIL_INFO: /*邮件*/
			return app_mg_cas312_api_init_email_data();	
			
		case DVB_CA_IPPV_POP_INFO: /*IPPV购买框信息*/
            return app_mg_cas312_api_init_ppv_data();
			
		default:
			break;	
	}

	return FALSE;		
}

uint8_t app_mg_cas312_api_buy_ipp(ca_buy_ipp_t *data)
{
    int32_t dvb_ca_flag =  app_flash_get_config_dvb_ca_flag();
	if (dvb_ca_flag != DVB_CA_TYPE_MG312)
	{
		return 0;
	}

	if (NULL == data)
	{
		return 0;	
	}

	switch (data->date_type)
	{
	    case DVB_CA_IPPV_POP_INFO: /*IPPV购买框信息*/
	         return app_mg_cas312_api_buy_ppv_data(data);
	    default:
			break;	
	}
    
    return 1;
}

char* app_mg_cas312_api_get_data(ca_get_date_t *data)
{
	int32_t dvb_ca_flag =  app_flash_get_config_dvb_ca_flag();
	if (dvb_ca_flag != DVB_CA_TYPE_MG312)
	{
		return NULL;
	}

	if (NULL == data)
	{
		return NULL;	
	}

	switch (data->date_type)
	{
		case DVB_CA_BASE_INFO:  /*基本信息*/
		    return app_mg_cas312_api_get_baseinfo_data(data);	

		case DVB_CA_EMAIL_INFO: /*邮件*/
            return app_mg_cas312_api_get_email_data(data); 
        
		case DVB_CA_IPPV_POP_INFO: /*IPPV购买框信息*/
            return app_mg_cas312_api_get_ppv_data(data);
	
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
uint8_t app_mg_cas312_api_update_data(ca_get_date_t *data)
{
	int32_t dvb_ca_flag =  app_flash_get_config_dvb_ca_flag();

	if (dvb_ca_flag != DVB_CA_TYPE_MG312)
	{
		return 0;
	}
	
	if (NULL == data)
	{
		return 0;	
	}

	switch (data->date_type)
	{
		default:
		break;	
	}
	
	return 0;	
}

uint8_t app_mg_cas312_api_get_count(ca_get_count_t *data)
{
	int32_t dvb_ca_flag = app_flash_get_config_dvb_ca_flag();
	
	if (dvb_ca_flag != DVB_CA_TYPE_MG312)
	{
		return 0;
	}

	if (NULL == data)
	{
		return 0;
	}
	
	switch (data->date_type)
	{
		case DVB_CA_EMAIL_INFO: /*邮件*/
			return app_mg_cas312_api_get_email_count(data);
			break;
			
		default:
			break;	
	}
	
	return 0;		
}

bool app_mg_cas312_api_delete(ca_delete_data_t *data)
{
	int32_t dvb_ca_flag = app_flash_get_config_dvb_ca_flag();
	
	if (dvb_ca_flag != DVB_CA_TYPE_MG312)
	{
		return FALSE;
	}

	if (NULL == data)
	{
		return FALSE;
	}
	
	switch (data->date_type)
	{
		case DVB_CA_EMAIL_INFO: /*邮件*/
			return app_mg_cas312_api_delete_email(data);
			break;
			
		default:
			break;	
	}
	
	return 0;	
}

bool app_mg_cas312_api_delete_all(ca_delete_data_t *data)
{
	int32_t dvb_ca_flag =  app_flash_get_config_dvb_ca_flag();
	
	if (dvb_ca_flag != DVB_CA_TYPE_MG312)
	{
		return FALSE;
	}

	if (NULL == data)
	{
		return FALSE;
	}

	switch (data->date_type)
	{
		case DVB_CA_EMAIL_INFO: /*邮件*/		
			return app_mg_cas312_api_delete_all_email(data);
			break;
		
		default:
			break;	
	}

	return 0;		
}

int32_t app_mg_cas312_gxmsg_ca_on_event_exec(GxMessage* msg)
{
	switch (msg->msg_id)
	{
		case GXMSG_CA_ON_EVENT:
    	{
    		GxMsgProperty0_OnEvent *event = NULL;
			
    		event = GxBus_GetMsgPropertyPtr(msg, GxMsgProperty0_OnEvent);
    		switch (event->key) 
			{
				case DVB_CA_EMAIL_INFO:		/*邮件图标显示*/
					app_mg_cas312_gxmsg_ca_on_event_email(event);
					break;

				case DVB_CA_ROLLING_INFO: /*OSD滚动消息*/
                    app_mg_cas312_gxmsg_ca_on_event_msg(event);
					break;
					
				case DVB_CA_FINGER_INFO: /*指纹*/
                    app_mg_cas312_gxmsg_ca_on_event_finger(event);
					break;

				case DVB_CA_IPPV_POP_INFO:  /*IPPV购买框信息*/
					app_mg_cas312_gxmsg_ca_on_event_ippv_logo(event);
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

uint8_t app_mg_cas312_api_is_ca_pop_msg_type(uint8_t type)
{
	if ((type >= (MSG_POP_COMMON_END+CA_MG_SUCCESS+1)) 
		&& (type <= (MSG_POP_COMMON_END+CA_MG_ID_MAX+1)))
	{
		return TRUE;
	}
	
	return FALSE;
}

//消息提示框
int32_t app_mg_cas312_api_pop_msg_exec(uint8_t type)
{
	switch (type-MSG_POP_COMMON_END-1)
	{
		case CA_MG_SUCCESS: /* 取消当前的显示 */
			app_mg_cas312_hide_ca_prompt();
			break;
			
		case CA_MG_ERR_SCARD_NOT_ACT: /* 请激活子卡 */
			app_mg_cas312_show_ca_prompt("Err Scard Not Act");
			break;
			
		case CA_MG_ERR_ZONE_CODE: /* 智能卡区域码错误 */
			app_mg_cas312_show_ca_prompt("Err Zone Code");
			break;
			
		case CA_MG_ERR_NO_MONEY: /* 智能卡余额不足 */
			app_mg_cas312_show_ca_prompt("Err No Money");
			break;
			
		case CA_MG_ERR_NOT_AUTHORED: /* 本节目未授权 */
			app_mg_cas312_show_ca_prompt("Err Not Authored");
			break;
			
		case CA_MG_ERR_CARD_LOCKED: /* 智能卡锁定 */
			app_mg_cas312_show_ca_prompt("Error Card Locked");
			break;
			
		case CA_MG_ERR_APP_LOCKED: /* 服务锁定 */
			app_mg_cas312_show_ca_prompt("Err App Locked");
			break;
			
		case CA_MG_ERR_PIN_AUTHEN: /* 机卡配对错误： */
		{
			MG_U8 pinErrCnt = 0;
			char strTemp[50] = {0};
			
			pinErrCnt = MG_Get_PinErrCount();
			if (0x80 == (pinErrCnt & 0x80))
			{
				if (0x20 == (pinErrCnt & 0x20))
				{
					/*智能卡已配对，机顶盒未配对*/
					app_mg_cas312_show_ca_prompt("Err Pin Authen Card Paired");	
				}

				if (0x10 == (pinErrCnt & 0x10))
				{
					/*智能卡未配对，机顶盒已配对*/
					app_mg_cas312_show_ca_prompt("Err Pin Authen App Locked");
				}
			}
			else
			{
				sprintf(strTemp, "智能卡再插入%d次后将被锁定", (pinErrCnt & 0x0F));
				app_mg_cas312_show_ca_prompt(strTemp);
			}		
		}	
			break;
			
		case CA_MG_ERR_EXTER_AUTHEN: /* 智能卡认证失败 */
			app_mg_cas312_show_ca_prompt("Err Exter Authen");
			break;

		case CA_MG_ERR_DONOT_TELEVIEW_PPV: /* PPV节目，请按【？】键订购 */
			app_mg_cas312_show_ca_prompt("Err Don't Teleview PPV");
			break;
			
		case CA_MG_ERR_CARD_NOTFOUND: /* 请插入智能卡 */
			app_mg_cas312_show_ca_prompt("Error Card Not Found");
			break;
			
		case CA_MG_ERR_CARD_NOTMATCH: /* 智能卡不匹配 */
			app_mg_cas312_show_ca_prompt("Err Card Not Match");
			break;
			
		case CA_MG_ERR_UPDATE_GK: /* 智能卡升级失败 */
			app_mg_cas312_show_ca_prompt("Err Update GK");
			break;

		case CA_MG_EMM_APP_STOP: /*智能卡暂停服务*/
			app_mg_cas312_show_ca_prompt("Emm App Stop");
			break;
			
		case CA_MG_EMM_APP_RESUME: /*智能卡恢复服务*/
			app_mg_cas312_show_ca_prompt("Emm App Resume");
			break;
			
		case CA_MG_EMM_PROG_UNAUTH: /*取消节目订购*/
			app_mg_cas312_show_ca_prompt("Emm Prog UnAuth");
			break;
			
		case CA_MG_EMM_PROG_AUTH: /*增加节目订购*/
			app_mg_cas312_show_ca_prompt("Emm Prog Auth");
			break;
			
		case CA_MG_EMM_EN_PARTNER: /*启用机卡配对*/
			app_mg_cas312_show_ca_prompt("Emm En Partner");
			break;
			
		case CA_MG_EMM_DIS_PARTNER: /*取消机卡配对*/
			app_mg_cas312_show_ca_prompt("Emm Dis Partner");
			break;
			
		case CA_MG_EMM_SONCARD: /*设置子母卡*/
			app_mg_cas312_show_ca_prompt("Emm SonCard");
			break;
			
		case CA_MG_EMM_UPDATE_GNO: /*智能卡组号更新*/
			app_mg_cas312_show_ca_prompt("Emm Update Gno");
			break;
			
		case CA_MG_EMM_UPDATE_GK: /*智能卡升级，请拔插卡*/
			app_mg_cas312_show_ca_prompt("Emm312 Update GK");
			break;
			
		case CA_MG_EMM_CHARGE:
			{
                MG_U32 value, tmpHighV, tmpLowV;
				char strTemp[50] = {0};
				
				value = MG_Get_ChargeValue();
				tmpHighV = value / 10000;
				tmpLowV = (value - tmpHighV*10000) / 100;
				sprintf(strTemp, "智能卡成功充值:%d.%02d元", tmpHighV, tmpLowV);
				app_mg_cas312_show_ca_prompt(strTemp);
				printf("***[app_mg_cas312_api_pop_msg_exec](%s)@***\n", strTemp);
			}
			break;
			
		case CA_MG_EMM_URGENT_SERVICE: /*应急广播*/
			app_mg_cas312_show_ca_prompt("Emm Urgent Service");
			break;
			
		case CA_MG_EMM_MODIFY_ZONECODE: /*智能卡区域码更新*/
			app_mg_cas312_show_ca_prompt("Emm Modify ZoneCode");
			break;

		case CA_MG_EMM_UPDATE_COUT: /*智能卡特征值更新*/
			app_mg_cas312_show_ca_prompt("Emm Update Cout");
			break;	
			
		case CA_MG_EMM_UPDATE_CRT: /*授权更新*/
			app_mg_cas312_show_ca_prompt("Emm Update Crt");
			break;
			
		default:
			break;
	}
	
	return 0;
}

int32_t app_mg_cas312_api_osd_exec(void)
{
	app_mg_cas312_rolling_message_exec();//滚动消息控制
	app_mg_cas312_email_flag_exec();// 邮件图标显示、隐藏控制
	app_mg_cas312_finger_exec();//指纹信息显示控制
    
	return 1;
}


APP_CAS_API_ControlBlock_t MgCas312PortControlBlock =
{
	.m_cas_init_callback = app_mg_cas312_api_init,//ca模块初始化
	.m_cas_close_callback = NULL,
	.m_cas_api_card_in_callback = app_mg_cas312_api_card_in,//智能卡插入
	.m_cas_api_card_out_callback = app_mg_cas312_api_card_out,//智能卡移除
	.m_cas_api_release_ecm_filter_callback = app_mg_cas312_api_release_ecm_filter,
	.m_cas_api_start_ecm_filter_callback = app_mg_cas312_api_start_ecm_filter,//ecm过滤器
	.m_cas_api_release_emm_filter_callback = app_mg_cas312_api_release_emm_filter,
	.m_cas_api_start_emm_filter_callback = app_mg_cas312_api_start_emm_filter,//emm过滤器设置
	.m_cas_api_check_cat_ca_descriptor_valid_callback = app_mg_cas312_api_check_cat_ca_descriptor_valid,//ca 模块cat表描述字校验
	.m_cas_api_check_pmt_ca_descriptor_valid_callback = app_mg_cas312_api_check_pmt_ca_descriptor_valid,//ca模块pmt表描述字校验
	.m_cas_api_is_ca_pop_msg_type_callback = app_mg_cas312_api_is_ca_pop_msg_type,//ca消息判断
	.m_cas_api_pop_msg_exec_callback = app_mg_cas312_api_pop_msg_exec,//播放ca消息框
	.m_cas_api_osd_exec_callback = app_mg_cas312_api_osd_exec,//200ms定时器任务
	.m_cas_api_gxmsg_ca_on_event_exec_callback = app_mg_cas312_gxmsg_ca_on_event_exec, 	//ca事件处理							

    .m_cas_api_init_data_callback = app_mg_cas312_api_init_data,//获取数据前的初始化操作
	.m_cas_api_get_data_callback = app_mg_cas312_api_get_data,//获取数据接口
	.m_cas_api_get_count_callback = app_mg_cas312_api_get_count,//获取整形数据值
	.m_cas_api_delete_callback = app_mg_cas312_api_delete,//删除单封邮件操作
	.m_cas_api_delete_all_callback = app_mg_cas312_api_delete_all,//删除所有邮件
	.m_cas_api_buy_ipp_callback = app_mg_cas312_api_buy_ipp,//iptv接口 (设置智能卡)
	.m_cas_api_get_lock_status_callback = app_mg_cas312_api_get_lock_status, //应急广播状态
	.m_cas_api_feed_mother_child_card_callback = NULL,
	.m_cas_api_change_pin_callback = NULL,
	.m_cas_api_verify_pin_callback = NULL,
	.m_cas_api_change_rating_callback = NULL,
	.m_cas_api_change_worktime_callback = NULL,
};


