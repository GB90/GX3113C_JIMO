/*****************************************************************************
* 						   CONFIDENTIAL
*        Hangzhou GuoXin Science and Technology Co., Ltd.
*                      (C)2014, All right reserved
******************************************************************************

******************************************************************************
* File Name :	app_kpapp_cas_porting_stb_api.c
* Author    : 	chenth
* Project   :	goxceed dvbc
* Type      :
******************************************************************************
* Purpose   :	
******************************************************************************
* Release History:
VERSION	Date			  AUTHOR         Description
1.0  	2014.12.17		  chenth 	 			creation
*****************************************************************************/
#include <gxtype.h>
#include "gxapp_sys_config.h"
#include "gxmsg.h"
#include "app_common_play.h"
#include "app_common_table_pmt.h"
#include "app_common_porting_ca_os.h"
#include "app_common_porting_stb_api.h"
#include "app_common_porting_ca_flash.h"
#include "app_common_porting_ca_demux.h"
#include "app_common_porting_ca_descramble.h"
#include "app_common_porting_ca_smart.h"
#include "KPAPP.h"
#include "kp_cas.h"
#include "app_kp_cas_api_demux.h"
#include "app_kp_cas_api_event.h"
#include "app_kp_cas_api_rolling_message.h"
#include "app_kp_cas_api_email.h"
#include "app_kp_cas_api_worktime.h"
#include "app_kp_cas_api_watchlevel.h"
#include "app_kp_cas_api_pin.h"
#include "app_kp_cas_api_entitle.h"
#include "app_kp_cas_api_baseinfo.h"
#include "app_kp_cas_api_ippv_slot.h"

extern 	void app_kp_cas_hide_ca_prompt(void);
extern void app_kp_cas_show_ca_prompt(char* context);

void TaskKPCAAPP_timer(void* arg)
{
	while(1)
	{
        //KP_Dbg("+++++++++++KPCAAPP_timer1\n");
        KPCAAPP_timer();
        //KP_Dbg("+++++++++++KPCAAPP_timer2\n");
        app_porting_ca_os_sleep(50);//推荐值是100ms,可调
	}
}


void TaskKPCA_FlashSync_timer(void* arg)
{
    int n = FLASH_SYNC_TIME_SEC;
	while(1)
	{
        if(n ++ >= FLASH_SYNC_TIME_SEC)
        {
            //KP_Dbg("+++++++++++TaskKPCA_FlashSync_timer1\n");
            flash_write2flash();
            //KP_Dbg("+++++++++++TaskKPCA_FlashSync_timer2\n");
            n = 0;
        }
        app_porting_ca_os_sleep(1000);
	}
}

static void app_kp_init()
{
    filter_list_init();
    flash_init();
    CAS_Dbg("app_kp_init ok\n");
}

//初始化
uint8_t app_kp_cas_api_init(void)
{
    handle_t	handle;
	app_porting_ca_flash_init(KPCA_FLASH_SIZE+EMail_SIZE);//flash空间初始化
	app_porting_ca_demux_init();//初始化CA过滤器
	app_porting_ca_descramble_init();
	app_kp_cas_rolling_message_init();
    app_kp_cas_api_init_email_data();//email数据初始化
	app_porting_ca_create_filter_task();//ca过滤器任务
    app_kp_init();
	if(KP_RET_SUCCESS != KPCAAPP_init())
	{
        CAS_Dbg("KPCAAPP_init failed\n");	
	}
    CAS_Dbg("app_kp_cas_api_init OK.\n");	
    GxCore_ThreadCreate("kp_timer",&handle, TaskKPCAAPP_timer, NULL, 10 * 1024, GXOS_DEFAULT_PRIORITY-1);
    GxCore_ThreadCreate("kp_flashsync",&handle, TaskKPCA_FlashSync_timer, NULL, 10 * 1024, GXOS_DEFAULT_PRIORITY);
    //kp_cas_rolling_test();
	return TRUE;
}

/*
*  获取对应数据之前，初始化数据管理
(一般创建对应窗体之前或create中调用)。
* 如初始化邮件、授权等
*/
int32_t app_kp_cas_api_init_data(dvb_ca_data_type_t date_type)
{
	int32_t dvb_ca_flag =  app_flash_get_config_dvb_ca_flag();
	if (dvb_ca_flag != DVB_CA_TYPE_KP)
		return FALSE;

	switch(date_type)
		{
			case DVB_CA_BASE_INFO:  /*基本信息*/
			    return app_kp_cas_api_init_baseinfo_data();
			case DVB_CA_OPERATOR_INFO: /*运营商信息*/
				break;
			case DVB_CA_EMAIL_INFO: /*邮件*/
				return app_kp_cas_api_init_email_data();
			case DVB_CA_ENTITLE_INFO: /*授权*/		
                return app_kp_cas_api_init_entitle_data();
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
				return app_kp_cas_api_init_ippv_slot_data();
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
                return app_kp_cas_api_init_worktime_data();
			case DVB_CA_CURTAIN_INFO: /*窗帘信息*/
				break;
			case DVB_CA_PAIRED_INFO: /*配对信息*/
				break;
			default:
				break;	
		}

	
	return FALSE;		
}



char * app_kp_cas_api_get_data(ca_get_date_t *data)
{
	int32_t dvb_ca_flag =  app_flash_get_config_dvb_ca_flag();
	if (dvb_ca_flag != DVB_CA_TYPE_KP)
		return NULL;


	if (NULL == data)
		return NULL;	

	switch(data->date_type)
		{
			case DVB_CA_BASE_INFO:  /*基本信息*/
                  return app_kp_cas_api_get_baseinfo_data(data);							
			case DVB_CA_OPERATOR_INFO: /*运营商信息*/	
				break;
			case DVB_CA_EMAIL_INFO: /*邮件*/
                return app_kp_cas_api_get_email_data(data); 
			case DVB_CA_ENTITLE_INFO: /*授权*/
                return app_kp_cas_api_get_entitle_data(data); 
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
                return app_kp_cas_api_get_ippv_slot_data(data);
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
                return app_kp_cas_api_get_watchlevel_data(data);			
			case DVB_CA_WORK_TIME_INFO: /*工作时段信息*/
                return app_kp_cas_api_get_worktime_data(data);				
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
uint8_t app_kp_cas_api_update_data(ca_get_date_t *data)
{
	int32_t dvb_ca_flag =  app_flash_get_config_dvb_ca_flag();

	if (dvb_ca_flag != DVB_CA_TYPE_KP)
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

uint8_t app_kp_cas_api_get_count(ca_get_count_t *data)
{
	int32_t dvb_ca_flag =  app_flash_get_config_dvb_ca_flag();
	if (dvb_ca_flag != DVB_CA_TYPE_KP)
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
			return app_kp_cas_api_get_email_count(data);
		case DVB_CA_ENTITLE_INFO: /*授权*/
          	return app_kp_cas_api_get_entitle_count(data);
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
            return app_kp_cas_api_get_ippv_slot_count(data);
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

bool app_kp_cas_api_delete(ca_delete_data_t *data)
{
	int32_t dvb_ca_flag =  app_flash_get_config_dvb_ca_flag();
	if (dvb_ca_flag != DVB_CA_TYPE_KP)
		return FALSE;


	if (NULL == data)
		return FALSE;
	
	switch(data->date_type)
	{
		case DVB_CA_EMAIL_INFO: /*邮件*/
			return app_kp_cas_api_delete_email(data);
		case DVB_CA_DETITLE_INFO: /*反授权*/
			break;	
		default:
			break;	
	}
	
	return 0;	
}

bool app_kp_cas_api_delete_all(ca_delete_data_t *data)
{
	int32_t dvb_ca_flag =  app_flash_get_config_dvb_ca_flag();
	if (dvb_ca_flag != DVB_CA_TYPE_KP)
		return FALSE;

	if (NULL == data)
		return FALSE;
	
	switch(data->date_type)
	{
		case DVB_CA_EMAIL_INFO: /*邮件*/		
			return app_kp_cas_api_delete_all_email(data);
		case DVB_CA_DETITLE_INFO: /*反授权*/	
			break;
		default:
			break;	
	}

	
	return 0;		
}


//事件处理
//ca事件处理
int32_t app_kp_cas_gxmsg_ca_on_event_exec(GxMessage * msg)
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
					case DVB_CA_EMAIL_INFO:		/*邮件图标显示*/
						app_kp_cas_gxmsg_ca_on_event_email(event);
						break;
					case DVB_CA_ENTITLE_INFO: /*授权提示框*/
                        printf("event,key: DVB_CA_ENTITLE_INFO\n");
                        //app_mg_cas_gxmsg_ca_on_event_entitle(event);
						break;
					case DVB_CA_DETITLE_INFO: /*反授权*/							
						break;
					case DVB_CA_EMERGENCY_INFO:           /*应急广播*/
						break;
					case DVB_CA_ROLLING_INFO: /*OSD滚动消息*/
                        printf("event,key: DVB_CA_ROLLING_INFO\n");
                        app_kp_cas_gxmsg_ca_on_event_msg(event);
						break;
					case DVB_CA_FINGER_INFO:                  /*指纹*/
                        printf("event: DVB_KP_FINGER_INFO\n");
                        app_kp_cas_gxmsg_ca_on_event_finger(event);
						break;
					case DVB_CA_CARD_UPDATE_INFO:       /*卡升级*/
						break;
					case DVB_CA_FETURE_INFO:                  /*特征值*/
						break;
					case DVB_CA_IPPV_POP_INFO:  /*IPPV购买框信息*/
                        //printf("event: DVB_CA_IPPV_POP_INFO\n");
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

uint8_t app_kp_cas_api_is_ca_pop_msg_type(uint8_t type)
{
	if ((type >= MSG_POP_COMMON_END+CA_KPCA_MESSAGE_SUCCESS+1)&&(type <=MSG_POP_COMMON_END+CA_KPCA_MESSAGE_FREEZE+1))
		return TRUE;
	
	return FALSE;
}

//消息提示框
int32_t app_kp_cas_api_pop_msg_exec(uint8_t type)
{
	switch(type-MSG_POP_COMMON_END-1)
	{
		case CA_KPCA_MESSAGE_SUCCESS: /* 取消当前的显示 */
			app_kp_cas_hide_ca_prompt();
			break;
		case CA_KPCA_MESSAGE_NO_ACTIVE:
			app_kp_cas_show_ca_prompt("Err NOT ACTIVE");
			break;
        case CA_KPCA_MESSAGE_AREA_NOT_MATCH:
            app_kp_cas_show_ca_prompt("Err NOT MATCH");
			break;
        case CA_KPCA_MESSAGE_AREA_FORCE:
            app_kp_cas_show_ca_prompt("Err AREA FORCE");
			break;
        case CA_KPCA_MESSAGE_ERROR_PRODUCT:
            app_kp_cas_show_ca_prompt("Err PRODUCT");
            break;
        case CA_KPCA_MESSAGE_ERROR_PROVIDER:
            app_kp_cas_show_ca_prompt("Err PROVIDER");
            break;
        case CA_KPCA_MESSAGE_NO_ENTITLE:
            app_kp_cas_show_ca_prompt("Err NO ENTITLE");
            break;
        case CA_KPCA_MESSAGE_ENTITLE_EXPIRED:
            app_kp_cas_show_ca_prompt("Err ENTITLE EXPIRED");
            break;
        case CA_KPCA_MESSAGE_ENTITLE_NOSTART:
            app_kp_cas_show_ca_prompt("Err ENTITLE NOSTART");
            break;
        case CA_KPCA_MESSAGE_PRODUCT_CLASH:
            app_kp_cas_show_ca_prompt("Err PRODUCT CLASH");
            break;
        case CA_KPCA_MESSAGE_SERVICE_FORCE:
            app_kp_cas_show_ca_prompt("Err SERVICE FORCE");
            break;
        case CA_KPCA_MESSAGE_OUT_RATING:
            app_kp_cas_show_ca_prompt("Err OUT RATING");
            break;
        case CA_KPCA_MESSAGE_OUT_WORKTIME:
            app_kp_cas_show_ca_prompt("Err OUT WORKTIME");
            break;
        case CA_KPCA_MESSAGE_FREEZE:
            app_kp_cas_show_ca_prompt("Err FREEZE");
            break;
		default:
			break;
		}
	return 0;
}

//200ms定时器回调
int32_t app_kp_cas_api_osd_exec(void)
{
	app_kp_cas_rolling_message_exec();//滚动消息控制
	app_kp_cas_email_flag_exec();// 邮件图标显示、隐藏控制
	app_kp_cas_finger_exec();//指纹信息显示控制
	
	return 1;
}



APP_CAS_API_ControlBlock_t KpCasPortControlBlock=
{
	.m_cas_init_callback =  app_kp_cas_api_init,//ca模块初始化
	.m_cas_close_callback = NULL,
	.m_cas_api_card_in_callback = NULL,//智能卡插入
	.m_cas_api_card_out_callback = NULL,//智能卡移除
	.m_cas_api_release_ecm_filter_callback= app_kp_cas_api_release_ecm_filter,
	.m_cas_api_start_ecm_filter_callback = app_kp_cas_api_start_ecm_filter,//ecm过滤器
	.m_cas_api_release_emm_filter_callback = app_kp_cas_api_release_emm_filter,
	.m_cas_api_start_emm_filter_callback = app_kp_cas_api_start_emm_filter,//emm过滤器设置
	.m_cas_api_check_cat_ca_descriptor_valid_callback= app_kp_cas_api_check_cat_ca_descriptor_valid,//ca 模块cat表描述字校验
	.m_cas_api_check_pmt_ca_descriptor_valid_callback= app_kp_cas_api_check_pmt_ca_descriptor_valid,//ca模块pmt表描述字校验
	.m_cas_api_is_ca_pop_msg_type_callback= app_kp_cas_api_is_ca_pop_msg_type,//ca消息判断
	.m_cas_api_pop_msg_exec_callback= app_kp_cas_api_pop_msg_exec,//播放ca消息框
	.m_cas_api_osd_exec_callback= app_kp_cas_api_osd_exec,//200ms定时器任务
	.m_cas_api_gxmsg_ca_on_event_exec_callback= app_kp_cas_gxmsg_ca_on_event_exec, 	//ca事件处理							
    /********************************************/
    .m_cas_api_init_data_callback = app_kp_cas_api_init_data,//获取数据前的初始化操作
	.m_cas_api_get_data_callback = app_kp_cas_api_get_data,//获取数据接口
	.m_cas_api_get_count_callback = app_kp_cas_api_get_count,//获取整形数据值
	.m_cas_api_delete_callback = app_kp_cas_api_delete,//删除单封邮件操作
	.m_cas_api_delete_all_callback = app_kp_cas_api_delete_all,//删除所有邮件
	.m_cas_api_buy_ipp_callback = NULL,//iptv接口 (设置智能卡)
	.m_cas_api_get_lock_status_callback = NULL,//应急广播状态
	.m_cas_api_feed_mother_child_card_callback = NULL,
	.m_cas_api_change_pin_callback = app_kp_cas_api_change_pin,
	.m_cas_api_verify_pin_callback = NULL,
	.m_cas_api_change_rating_callback = app_kp_cas_api_change_watchlevel,
	.m_cas_api_change_worktime_callback= app_kp_cas_api_change_worktime,
};





