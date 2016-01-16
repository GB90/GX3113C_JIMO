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
#include "ca_process.h"
#include "app_common_play.h"
#include "app_common_table_pmt.h"
#include "app_common_porting_stb_api.h"
#include "app_common_porting_ca_flash.h"
#include "app_common_porting_ca_demux.h"
#include "app_common_porting_ca_descramble.h"
#include "app_common_porting_ca_smart.h"
#include "app_qz_cas_api_demux.h"
#include "app_qz_cas_api_smart.h"
#include "app_qz_cas_api_email.h"
#include "app_qz_cas_api_rolling_message.h"
#include "app_qz_cas_api_baseinfo.h"

extern 	void app_qz_cas_hide_ca_prompt(void);
extern void app_qz_cas_show_ca_prompt(char* context);
extern void QZCA_InitAreaCode();
int32_t qzcas_get_config_hs_mode(void)
{
	int32_t config = 0;
	GxBus_ConfigGetInt("QZHostSlaveMode", &config, HOST_MODE);
	return config;
}

int32_t qzcas_set_config_hs_mode(int32_t config)
{
	GxBus_ConfigSetInt("QZHostSlaveMode",config);
	return 0;
}
//初始化
uint8_t app_qz_cas_api_init(void)
{
    printf("==========app_qz_cas_api_init\n");
	app_porting_ca_flash_init(EMail_SIZE);//flash空间初始化
	app_porting_ca_demux_init();//初始化CA过滤器
	app_porting_ca_descramble_init();
    app_qz_cas_rolling_message_init();
    app_qz_cas_api_init_email_data();//email数据初始化
	app_porting_ca_create_filter_task();//ca过滤器任务
	QZ_Init_CAS(0);
    QZCA_InitAreaCode();
	app_porting_ca_smart_create_task();//智能卡状态检测任
	return TRUE;
}

/*
*  获取对应数据之前，初始化数据管理
(一般创建对应窗体之前或create中调用)。
* 如初始化邮件、授权等
*/
int32_t app_qz_cas_api_init_data(dvb_ca_data_type_t date_type)
{
	int32_t dvb_ca_flag =  app_flash_get_config_dvb_ca_flag();
	if (dvb_ca_flag != DVB_CA_TYPE_QZ)
		return FALSE;

	switch(date_type)
		{
			case DVB_CA_BASE_INFO:  /*基本信息*/
                break;
			case DVB_CA_OPERATOR_INFO: /*运营商信息*/
				break;
			case DVB_CA_EMAIL_INFO: /*邮件*/
				return app_qz_cas_api_init_email_data();
			case DVB_CA_ENTITLE_INFO: /*授权*/		
				break;
			case DVB_CA_DETITLE_INFO: /*反授权*/	
				break;					
			case DVB_CA_CARD_UPDATE_INFO: /*卡升级*/
				break;
			case DVB_CA_FETURE_INFO: /*特征值*/
				break;
			case DVB_CA_IPPV_POP_INFO: /*IPPV购买框信息*/
                //return app_mg_cas_api_init_ppv_data();
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

	
	return FALSE;		
}

char * app_qz_cas_api_get_data(ca_get_date_t *data)
{
	int32_t dvb_ca_flag =  app_flash_get_config_dvb_ca_flag();
	if (dvb_ca_flag != DVB_CA_TYPE_QZ)
		return NULL;


	if (NULL == data)
		return NULL;	

	switch(data->date_type)
	{
		case DVB_CA_BASE_INFO:  /*基本信息*/
		    return app_qz_cas_api_get_baseinfo_data(data);	
		    break;
		case DVB_CA_OPERATOR_INFO: /*运营商信息*/	
			break;
		case DVB_CA_EMAIL_INFO: /*邮件*/
            return app_qz_cas_api_get_email_data(data); 
            break;
		case DVB_CA_ENTITLE_INFO: /*授权*/
			break;
		case DVB_CA_DETITLE_INFO: /*反授权*/
			break;									
		case DVB_CA_CARD_UPDATE_INFO: /*卡升级*/
			break;
		case DVB_CA_FETURE_INFO: /*特征值*/
			break;			
		case DVB_CA_IPPV_POP_INFO: /*IPPV购买框信息*/
            //return app_mg_cas_api_get_ppv_data(data);
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

	
	return NULL;	
}

/*
*  更新CA数据
(CA数据发生变更时调用)。
* 如删除邮件、反授权，CA通知授权信息变化等
*/
uint8_t app_qz_cas_api_update_data(ca_get_date_t *data)
{
	int32_t dvb_ca_flag =  app_flash_get_config_dvb_ca_flag();

	if (dvb_ca_flag != DVB_CA_TYPE_QZ)
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

uint8_t app_qz_cas_api_get_count(ca_get_count_t *data)
{
	int32_t dvb_ca_flag =  app_flash_get_config_dvb_ca_flag();
	if (dvb_ca_flag != DVB_CA_TYPE_QZ)
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
			return app_qz_cas_api_get_email_count(data);
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

bool app_qz_cas_api_delete(ca_delete_data_t *data)
{
	int32_t dvb_ca_flag =  app_flash_get_config_dvb_ca_flag();
	if (dvb_ca_flag != DVB_CA_TYPE_QZ)
		return FALSE;


	if (NULL == data)
		return FALSE;
	
	switch(data->date_type)
	{
		case DVB_CA_EMAIL_INFO: /*邮件*/
			return app_qz_cas_api_delete_email(data);	
			break;
		default:
			break;	
	}
	
	return 0;	
}

bool app_qz_cas_api_delete_all(ca_delete_data_t *data)
{
	int32_t dvb_ca_flag =  app_flash_get_config_dvb_ca_flag();
	if (dvb_ca_flag != DVB_CA_TYPE_QZ)
		return FALSE;


	if (NULL == data)
		return FALSE;
	

	switch(data->date_type)
	{
		case DVB_CA_EMAIL_INFO: /*邮件*/		
			return app_qz_cas_api_delete_all_email(data);
			break;
		default:
			break;	
	}

	
	return 0;		
}


//事件处理
//ca事件处理
int32_t app_qz_cas_gxmsg_ca_on_event_exec(GxMessage * msg)
{
	switch(msg->msg_id)
	{
		case GXMSG_CA_ON_EVENT:
    	{
    		GxMsgProperty0_OnEvent* event;
    		event = GxBus_GetMsgPropertyPtr(msg, GxMsgProperty0_OnEvent);
    		//printf("app_qz_cas_gxmsg_ca_on_event_exec GXMSG_CA_ON_EVENT event->key=%d\n",event->key);
    		switch (event->key) {
					case DVB_CA_BASE_INFO:		       /*基本信息*/
						break;
					case DVB_CA_OPERATOR_INFO:		/*运营商信息*/	
						break;
					case DVB_CA_EMAIL_INFO:		/*邮件图标显示*/
                        {
                        //printf("1111,email\n");
						app_qz_cas_gxmsg_ca_on_event_email(event);
                        //printf("1111#\n");
					    }
						break;
					case DVB_CA_ENTITLE_INFO: /*授权提示框*/
						break;
					case DVB_CA_DETITLE_INFO: /*反授权*/							
						break;
					case DVB_CA_EMERGENCY_INFO:           /*应急广播*/
						break;
					case DVB_CA_ROLLING_INFO: /*OSD滚动消息*/
						{
                          //printf("2222,rolling\n");
                          app_qz_cas_gxmsg_ca_on_event_msg(event);
                          //printf("2222#\n");
						}
						break;
					case DVB_CA_FINGER_INFO:                  /*指纹*/
						break;
					case DVB_CA_CARD_UPDATE_INFO:       /*卡升级*/
						break;
					case DVB_CA_FETURE_INFO:                  /*特征值*/
						break;
					case DVB_CA_IPPV_POP_INFO:  /*IPPV购买框信息*/
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

uint8_t app_qz_cas_api_is_ca_pop_msg_type(uint8_t type)
{
	if ((type >= MSG_POP_COMMON_END+CA_QZ_SUCCESS+1)&&(type <=MSG_POP_COMMON_END+CA_QZ_ERR_CARD_NOTFOUND+1))
		return TRUE;
	
	return FALSE;
}

//消息提示框
int32_t app_qz_cas_api_pop_msg_exec(uint8_t type)
{
	switch(type-MSG_POP_COMMON_END-1)
	{
		case CA_QZ_SUCCESS: /* 取消当前的显示 */
			app_qz_cas_hide_ca_prompt();
			break;
        case CA_QZ_ERR_NO_MEMORY:
            app_qz_cas_show_ca_prompt("Err Scard No Memory");
			break;
        case CA_QZ_ERR_ILLEGAL_CARD: /* 智能卡通信错误*/
			app_qz_cas_show_ca_prompt("Err Illegal Card");
			break;
        case CA_QZ_ERR_AUTH_OVERDUE: /* 智能卡授权过期*/
			app_qz_cas_show_ca_prompt("Err Auth Overdue");
			break;
        case CA_QZ_ERR_AUTH_NOTSTART: /*未到授权起始日期*/
			app_qz_cas_show_ca_prompt("Err Auth Overdue");
			break;
        case CA_QZ_ERR_CARD_NOTMATCH: /* 智能卡不匹配 */
			app_qz_cas_show_ca_prompt("Err Card Not Match");
			break;
         case CA_QZ_ERR_CARD_NOTINIT: /*智能卡未初始化*/
			app_qz_cas_show_ca_prompt("Err Card Not Init");
			break;
        case CA_QZ_ERR_PPV_OUTDATE: /*PPV过期*/
			app_qz_cas_show_ca_prompt("Err PPV Out of Date");
			break;
        case CA_QZ_ERR_PINERR_CARD_PAIRED: /*智能卡没有设置为机卡配对*/
			app_qz_cas_show_ca_prompt("Err Pin Authen Card Paired");
			break;
        case CA_QZ_ERR_ECM_NOT_AUTHORIZED:/*频道未授权*/
            app_qz_cas_show_ca_prompt("Err Not Authored");
            break;
         case CA_QZ_ERR_ZONE_CODE:/*区域限播*/
            app_qz_cas_show_ca_prompt("Err Limit Area");
            break;
         case CA_QZ_ERR_LIMIT_CARD:/*卡限播*/
            app_qz_cas_show_ca_prompt("Err Limit Card");
            break;
         case CA_QZ_ERR_NOT_CHILD_MODE:/*智能卡没有设置为子卡模式*/
            app_qz_cas_show_ca_prompt("Err Not Child Mode");
            break;
		case CA_QZ_ERR_NO_CHILD_AUTH: /* 请激活子卡 */
			app_qz_cas_show_ca_prompt("Err Scard Not Act");
			break;
        case CA_QZ_ERR_CHILD_NOT_AUTHORIZED:/*子卡未授权*/
            app_qz_cas_show_ca_prompt("Err Scard Not Authored");
            break;
        case CA_QZ_ERR_CHILD_AUTH_TIMEOUT:/*设置子卡授权超时*/
            app_qz_cas_show_ca_prompt("Err Scard Auth Timeout");
            break;
        case CA_QZ_ERR_CHILD_AUTH_EXPIRED:/*子卡授权过期*/
            app_qz_cas_show_ca_prompt("Err Scard Auth Timeout");
            break;
        case CA_QZ_ERR_CARD_NOTFOUND: /* 请插入智能卡 */
			app_qz_cas_show_ca_prompt("Error Card Not Found");
			break;
		default:
			break;
		}
	return 0;
}

//200ms定时器回调
int32_t app_qz_cas_api_osd_exec(void)
{
	app_qz_cas_rolling_message_exec();//滚动消息控制
	app_qz_cas_email_flag_exec();// 邮件图标显示、隐藏控制
	return 1;
}

APP_CAS_API_ControlBlock_t QzCasPortControlBlock=
{
	.m_cas_init_callback =  app_qz_cas_api_init,//ca模块初始化
	.m_cas_close_callback = NULL,
	.m_cas_api_card_in_callback = app_qz_cas_api_card_in,//智能卡插入
	.m_cas_api_card_out_callback = app_qz_cas_api_card_out,//智能卡移除
	.m_cas_api_release_ecm_filter_callback= app_qz_cas_api_release_ecm_filter,
	.m_cas_api_start_ecm_filter_callback = app_qz_cas_api_start_ecm_filter,//ecm过滤器
	.m_cas_api_release_emm_filter_callback = app_qz_cas_api_release_emm_filter,
	.m_cas_api_start_emm_filter_callback = app_qz_cas_api_start_emm_filter,//emm过滤器设置
	.m_cas_api_check_cat_ca_descriptor_valid_callback= app_qz_cas_api_check_cat_ca_descriptor_valid,//ca 模块cat表描述字校验
	.m_cas_api_check_pmt_ca_descriptor_valid_callback= app_qz_cas_api_check_pmt_ca_descriptor_valid,//ca模块pmt表描述字校验
	.m_cas_api_is_ca_pop_msg_type_callback= app_qz_cas_api_is_ca_pop_msg_type,//ca消息判断
	.m_cas_api_pop_msg_exec_callback= app_qz_cas_api_pop_msg_exec,//播放ca消息框
	.m_cas_api_osd_exec_callback= app_qz_cas_api_osd_exec,//200ms定时器任务
	.m_cas_api_gxmsg_ca_on_event_exec_callback= app_qz_cas_gxmsg_ca_on_event_exec, 	//ca事件处理							
    /********************************************/
    .m_cas_api_init_data_callback = app_qz_cas_api_init_data,//获取数据前的初始化操作
	.m_cas_api_get_data_callback = app_qz_cas_api_get_data,//获取数据接口
	.m_cas_api_get_count_callback = app_qz_cas_api_get_count,//获取整形数据值
	.m_cas_api_delete_callback = app_qz_cas_api_delete,//删除单封邮件操作
	.m_cas_api_delete_all_callback = app_qz_cas_api_delete_all,//删除所有邮件
	.m_cas_api_buy_ipp_callback = NULL,//iptv接口 (设置智能卡)
	.m_cas_api_get_lock_status_callback = NULL,//应急广播状态
	.m_cas_api_feed_mother_child_card_callback = NULL,
	.m_cas_api_change_pin_callback = NULL,
	.m_cas_api_verify_pin_callback = NULL,
	.m_cas_api_change_rating_callback = NULL,
	.m_cas_api_change_worktime_callback= NULL,
};




