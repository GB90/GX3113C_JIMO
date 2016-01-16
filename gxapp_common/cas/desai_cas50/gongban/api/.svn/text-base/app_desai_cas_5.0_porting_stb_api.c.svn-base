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
#include "app_common_play.h"
#include "app_common_table_pmt.h"
#include "app_common_porting_stb_api.h"
#include "app_common_lcn.h"


/*
* 德赛CA接口头文件
*/
#include "app_desai_cas_5.0_api_demux.h"
#include "app_desai_cas_5.0_api_smart.h"
//#include "app_desai_cas_5.0_api_entitle.h"
//#include "app_desai_cas_5.0_api_detitle.h"
#include "app_desai_cas_5.0_api_baseinfo.h"
//#include "app_desai_cas_5.0_api_lock_service.h"

extern  int32_t app_desai_cas50_api_osd_exec(void);
extern int32_t app_desai_cas50_gxmsg_ca_on_event_exec(GxMessage * msg);

extern uint8_t app_desai_cas50_api_get_lock_status(void);


/*
* 以下接口CA菜单调用
*/


/*
*  获取对应数据之前，初始化数据管理
(一般创建对应窗体之前或create中调用)。
* 如初始化邮件、授权等
*/
int32_t app_desai_cas50_api_init_data(dvb_ca_data_type_t date_type)
{
	int32_t dvb_ca_flag =  app_flash_get_config_dvb_ca_flag();
	if (dvb_ca_flag != DVB_CA_TYPE_DSCAS50)
		return FALSE;

	switch(date_type)
		{
			case DVB_CA_BASE_INFO:  /*基本信息*/
			case DVB_CA_OPERATOR_INFO: /*运营商信息*/		
			case DVB_CA_EMAIL_INFO: /*邮件*/		
			case DVB_CA_ENTITLE_INFO: /*授权*/		
				return 0;//app_cd_cas30_api_init_entitle_data();
			case DVB_CA_DETITLE_INFO: /*反授权*/		
				return 0;//app_cd_cas30_api_init_detitle_data();						
			case DVB_CA_CARD_UPDATE_INFO: /*卡升级*/
			case DVB_CA_FETURE_INFO: /*特征值*/
			case DVB_CA_IPPV_POP_INFO: /*IPPV购买框信息*/
			case DVB_CA_IPPT_POP_INFO: /*IPPT购买框信息*/
			case DVB_CA_IPPV_PROG_INFO: /*IPPV已购买节目信息*/
			case DVB_CA_IPPV_SLOT_INFO: /*IPPV钱包信息*/
		//	case DVB_CA_CHILD_CARD_INFO: /*子卡信息*/
			case DVB_CA_MOTHER_CARD_INFO: /*母卡信息*/
			case DVB_CA_CHANGE_PIN_INFO: /*修改密码信息*/
			case DVB_CA_PROGRESS_INFO: /*智能卡升级进度信息*/
			case DVB_CA_RATING_INFO: /*成人级别信息*/
			case DVB_CA_WORK_TIME_INFO: /*工作时段信息*/
			case DVB_CA_CURTAIN_INFO: /*窗帘信息*/
			case DVB_CA_PAIRED_INFO: /*配对信息*/
			default:
				break;	
		}

	
	return FALSE;		
}

char * app_desai_cas50_api_get_data(ca_get_date_t *data)
{
	int32_t dvb_ca_flag =  app_flash_get_config_dvb_ca_flag();
	if (dvb_ca_flag != DVB_CA_TYPE_DSCAS50)
		return NULL;


	if (NULL == data)
		return NULL;	

	switch(data->date_type)
		{
			case DVB_CA_BASE_INFO:  /*基本信息*/
				return (char *)app_desai_cas50_api_get_baseinfo_data(data);							
			case DVB_CA_OPERATOR_INFO: /*运营商信息*/		
			case DVB_CA_EMAIL_INFO: /*邮件*/		
			case DVB_CA_ENTITLE_INFO: /*授权*/		
				return 0;//app_cd_cas30_api_get_entitle_data(data);
			case DVB_CA_DETITLE_INFO: /*反授权*/		
				return 0;//app_cd_cas30_api_get_detitle_data(data);						
			case DVB_CA_CARD_UPDATE_INFO: /*卡升级*/
			case DVB_CA_FETURE_INFO: /*特征值*/
			case DVB_CA_IPPV_POP_INFO: /*IPPV购买框信息*/
			case DVB_CA_IPPT_POP_INFO: /*IPPT购买框信息*/
			case DVB_CA_IPPV_PROG_INFO: /*IPPV已购买节目信息*/
			case DVB_CA_IPPV_SLOT_INFO: /*IPPV钱包信息*/
		//	case DVB_CA_CHILD_CARD_INFO: /*子卡信息*/
			case DVB_CA_MOTHER_CARD_INFO: /*母卡信息*/
			case DVB_CA_CHANGE_PIN_INFO: /*修改密码信息*/
			case DVB_CA_PROGRESS_INFO: /*智能卡升级进度信息*/
			case DVB_CA_RATING_INFO: /*成人级别信息*/
			case DVB_CA_WORK_TIME_INFO: /*工作时段信息*/
			case DVB_CA_CURTAIN_INFO: /*窗帘信息*/
			case DVB_CA_PAIRED_INFO: /*配对信息*/
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
uint8_t app_desai_cas50_api_update_data(ca_get_date_t *data)
{
	int32_t dvb_ca_flag =  app_flash_get_config_dvb_ca_flag();

	if (dvb_ca_flag != DVB_CA_TYPE_DSCAS50)
		return 0;


	if (NULL == data)
		return 0;	

	switch(data->date_type)
		{
			case DVB_CA_BASE_INFO:  /*基本信息*/
			case DVB_CA_OPERATOR_INFO: /*运营商信息*/		
			case DVB_CA_EMAIL_INFO: /*邮件*/		
			case DVB_CA_ENTITLE_INFO: /*授权*/
			case DVB_CA_DETITLE_INFO: /*反授权*/		
			case DVB_CA_EMERGENCY_INFO: /*应急广播*/		
			case DVB_CA_ROLLING_INFO: /*OSD滚动消息*/
			case DVB_CA_FINGER_INFO: /*指纹*/
			case DVB_CA_CARD_UPDATE_INFO: /*卡升级*/
			case DVB_CA_FETURE_INFO: /*特征值*/
			case DVB_CA_IPPV_POP_INFO: /*IPPV购买框信息*/
			case DVB_CA_IPPT_POP_INFO: /*IPPT购买框信息*/
			case DVB_CA_IPPV_PROG_INFO: /*IPPV已购买节目信息*/
			case DVB_CA_IPPV_SLOT_INFO: /*IPPV钱包信息*/
		//	case DVB_CA_CHILD_CARD_INFO: /*子卡信息*/
			case DVB_CA_MOTHER_CARD_INFO: /*母卡信息*/
			case DVB_CA_CHANGE_PIN_INFO: /*修改密码信息*/
			case DVB_CA_PROGRESS_INFO: /*智能卡升级进度信息*/
			case DVB_CA_RATING_INFO: /*成人级别信息*/
			case DVB_CA_WORK_TIME_INFO: /*工作时段信息*/
			case DVB_CA_CURTAIN_INFO: /*窗帘信息*/
			case DVB_CA_PAIRED_INFO: /*配对信息*/
			default:
				break;	
		}
	
	return 0;	
}

uint8_t app_desai_cas50_api_get_count(ca_get_count_t *data)
{
	int32_t dvb_ca_flag =  app_flash_get_config_dvb_ca_flag();
	if (dvb_ca_flag != DVB_CA_TYPE_DSCAS50)
		return 0;

	if (NULL == data)
		return 0;
	

	switch(data->date_type)
		{
			case DVB_CA_BASE_INFO:  /*基本信息*/
			case DVB_CA_OPERATOR_INFO: /*运营商信息*/		
			case DVB_CA_EMAIL_INFO: /*邮件*/		
			case DVB_CA_ENTITLE_INFO: /*授权*/
				return 0;//app_cd_cas30_api_get_entitle_count(data);
			case DVB_CA_DETITLE_INFO: /*反授权*/	
				return 0;//app_cd_cas30_api_get_detitle_count(data);													
			case DVB_CA_EMERGENCY_INFO: /*应急广播*/		
			case DVB_CA_ROLLING_INFO: /*OSD滚动消息*/
			case DVB_CA_FINGER_INFO: /*指纹*/
			case DVB_CA_CARD_UPDATE_INFO: /*卡升级*/
			case DVB_CA_FETURE_INFO: /*特征值*/
			case DVB_CA_IPPV_POP_INFO: /*IPPV购买框信息*/
			case DVB_CA_IPPT_POP_INFO: /*IPPT购买框信息*/
			case DVB_CA_IPPV_PROG_INFO: /*IPPV已购买节目信息*/
			case DVB_CA_IPPV_SLOT_INFO: /*IPPV钱包信息*/
		//	case DVB_CA_CHILD_CARD_INFO: /*子卡信息*/
			case DVB_CA_MOTHER_CARD_INFO: /*母卡信息*/
			case DVB_CA_CHANGE_PIN_INFO: /*修改密码信息*/
			case DVB_CA_PROGRESS_INFO: /*智能卡升级进度信息*/
			case DVB_CA_RATING_INFO: /*成人级别信息*/
			case DVB_CA_WORK_TIME_INFO: /*工作时段信息*/
			case DVB_CA_CURTAIN_INFO: /*窗帘信息*/
			case DVB_CA_PAIRED_INFO: /*配对信息*/
			default:
				break;	
		}
	
	return 0;		
}

bool app_desai_cas50_api_delete(ca_delete_data_t *data)
{
	int32_t dvb_ca_flag =  app_flash_get_config_dvb_ca_flag();
	if (dvb_ca_flag != DVB_CA_TYPE_DSCAS50)
		return FALSE;


	if (NULL == data)
		return FALSE;
	
	switch(data->date_type)
		{
			case DVB_CA_EMAIL_INFO: /*邮件*/	
				break;
			case DVB_CA_DETITLE_INFO: /*反授权*/	
				return 0;//app_cd_cas30_api_delete_detitle(data);	
			default:
				break;	
		}
	
	return 0;	
}

bool app_desai_cas50_api_delete_all(ca_delete_data_t *data)
{
	int32_t dvb_ca_flag =  app_flash_get_config_dvb_ca_flag();
	if (dvb_ca_flag != DVB_CA_TYPE_DSCAS50)
		return FALSE;


	if (NULL == data)
		return FALSE;
	

	switch(data->date_type)
		{
			case DVB_CA_EMAIL_INFO: /*邮件*/	
				break;
			case DVB_CA_DETITLE_INFO: /*反授权*/	
				return 0;//app_cd_cas30_api_delete_all_detitle(data);	
			default:
				break;	
		}

	
	return 0;		
}

APP_CAS_API_ControlBlock_t DesaiCas50PortControlBlock=
{
	.m_cas_init_callback =  app_desai_cas50_api_init,
	.m_cas_close_callback = NULL,
	.m_cas_api_card_in_callback = app_desai_cas50_api_card_in,
	.m_cas_api_card_out_callback = app_desai_cas50_api_card_out,
	.m_cas_api_release_ecm_filter_callback= app_desai_cas50_api_release_ecm_filter,
	.m_cas_api_start_ecm_filter_callback = app_desai_cas50_api_start_ecm_filter,
	.m_cas_api_release_emm_filter_callback = app_desai_cas50_api_release_emm_filter,
	.m_cas_api_start_emm_filter_callback = app_desai_cas50_api_start_emm_filter,
	.m_cas_api_check_cat_ca_descriptor_valid_callback= app_desai_cas50_api_check_cat_ca_descriptor_valid,
	.m_cas_api_check_pmt_ca_descriptor_valid_callback= app_desai_cas50_api_check_pmt_ca_descriptor_valid,
	.m_cas_api_is_ca_pop_msg_type_callback= app_desai_cas50_api_is_ca_pop_msg_type,
	.m_cas_api_pop_msg_exec_callback= app_desai_cas50_api_pop_msg_exec,
	.m_cas_api_osd_exec_callback= app_desai_cas50_api_osd_exec,
	.m_cas_api_gxmsg_ca_on_event_exec_callback= app_desai_cas50_gxmsg_ca_on_event_exec, 								
	.m_cas_api_init_data_callback = app_desai_cas50_api_init_data,
	.m_cas_api_get_data_callback = app_desai_cas50_api_get_data,
	//.m_cas_api_update_data_callback = NULL,//app_cd_cas30_api_update_data;
	.m_cas_api_get_count_callback = app_desai_cas50_api_get_count,
	.m_cas_api_delete_callback = app_desai_cas50_api_delete,
	.m_cas_api_delete_all_callback = app_desai_cas50_api_delete_all,
	.m_cas_api_buy_ipp_callback = NULL,//app_cd_cas30_api_buy_ipp;
	.m_cas_api_get_lock_status_callback =	app_desai_cas50_api_get_lock_status,
	.m_cas_api_feed_mother_child_card_callback = NULL,//app_cd_cas30_api_read_data_from_mother_card;
//	.m_cas_api_write_data_to_child_card_callback = NULL,//app_cd_cas30_api_write_data_to_child_card;
	.m_cas_api_change_pin_callback = NULL,//app_cd_cas30_api_change_pin;
	.m_cas_api_change_rating_callback = NULL,//app_cd_cas30_api_change_rating;
	.m_cas_api_change_worktime_callback= NULL,//app_cd_cas30_api_change_worktime;
};



