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
* Purpose   :	ģ??ͷ?ļ?
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
#include "app_divitone_cas_api_smart.h"
//#include "app_by_cas_api_demux.h"
#include "DiviGuardCa.h"
#include "app_common_porting_ca_smart.h"

#if 0
#include "app_by_cas_api_demux.h"
#include "app_by_cas_api_smart.h"
#include "app_by_cas_api_entitle.h"
#include "app_by_cas_api_email.h"
#include "app_by_cas_api_baseinfo.h"
#include "app_by_cas_api_lock_service.h"
#include "app_by_cas_api_rolling_message.h"
#include "app_by_cas_api_finger.h"
#endif
#include "app_divitone_cas_api_cardinfo.h"
#include "app_divitone_cas_api_email.h"
#include "app_divitone_cas_api_worktime.h"
#include "app_divitone_cas_api_watchlevel.h"
#include "app_divitone_cas_api_demux.h"
#include "app_divitone_cas_api_pin.h"
#include "app_divitone_cas_api_entitle.h"
#include "app_divitone_cas_api_rolling_message.h"

extern int32_t app_divitone_cas_gxmsg_ca_on_event_activechild(GxMsgProperty0_OnEvent* event);

extern void app_divitone_cas_hide_ca_prompt(void);
extern void app_divitone_cas_show_ca_prompt(char* context);

extern uint8_t bat_recieve_flag ;

extern void ca_init_mutex(void);
uint8_t app_divitone_cas_api_init(void)
{

	app_porting_ca_flash_init(2*64*1024);
	app_porting_ca_demux_init();
	app_porting_ca_descramble_init();
	app_porting_ca_create_filter_task();

	ca_init_mutex();

	if (SUCCESS != DIVI_Initialize(0))
	{
		CA_FAIL(" DIVICASTB_Init  failed \n");
		return FALSE;
	}
	
	app_divitone_cas_api_smart_init();
	app_porting_ca_smart_create_task();

	app_divitone_cas_api_init_email_data();
#if 0
	/*
	* ???ι???
	*/
	{
		uint32_t symbol_rate = 0;
		uint32_t qam = 0;
		uint32_t fre= 0;
		GxTime stoptime={0};
		GxTime starttime={0};
		uint32_t starts,endMs;
//		int32_t ret;


		/*
		* ????Ƶ??
		*/
		symbol_rate = app_flash_get_config_center_freq_symbol_rate();
		qam = app_flash_get_config_center_freq_qam();
		fre = app_flash_get_config_center_freq();

		if (0 == app_search_lock_tp(fre, symbol_rate,
						INVERSION_OFF, qam,1000))
			{
				GxCore_GetTickTime(&starttime);
				starts = starttime.seconds;

				while(TRUE)
					{
						if (TRUE == bat_recieve_flag )
							{
								GxCore_ThreadDelay(1000);
								break;
							}
						GxCore_GetTickTime(&stoptime);
						endMs = stoptime.seconds;
						if (endMs - starts >= 2)
							{
								bat_recieve_flag = TRUE;
								break;
							}
						GxCore_ThreadDelay(200);
					}				
			}

		bat_recieve_flag = TRUE;
	}
#endif
	return TRUE;
	
}

/*
* ???½ӿ?CA?˵?????
*/


/*
*  ??ȡ??Ӧ????֮ǰ????ʼ?????ݹ???
(һ?㴴????Ӧ????֮ǰ??create?е???)??
* ????ʼ???ʼ?????Ȩ??
*/
int32_t app_divitone_cas_api_init_data(dvb_ca_data_type_t date_type)
{
	int32_t dvb_ca_flag =  app_flash_get_config_dvb_ca_flag();
	if (dvb_ca_flag != DVB_CA_TYPE_DIVITONE)
		return FALSE;

	switch(date_type)
		{
			case DVB_CA_BASE_INFO:	/*???\u0178??Ϣ*/
				return app_divitone_cas_api_init_baseinfo_data();
			case DVB_CA_EMAIL_INFO: /*??\u0152?*/	
				return app_divitone_cas_api_init_email_data();							
			case DVB_CA_WORK_TIME_INFO: /*????ʱ????Ϣ*/
				return app_divitone_cas_api_init_worktime_data();
			case DVB_CA_ENTITLE_INFO:
				return app_divitone_cas_api_init_entitle_data();				
			default:
				break;	
		}


	
	return FALSE;		
}

char* app_divitone_cas_api_get_data(ca_get_date_t *data)
{
	int32_t dvb_ca_flag =  app_flash_get_config_dvb_ca_flag();
	if (dvb_ca_flag != DVB_CA_TYPE_DIVITONE)
		return NULL;


	if (NULL == data)
		return NULL;	

	switch(data->date_type)
		{
			case DVB_CA_BASE_INFO:	/*???\u0178??Ϣ*/
				return app_divitone_cas_api_get_baseinfo_data(data);
			case DVB_CA_EMAIL_INFO: /*??\u0152?*/	
				return app_divitone_cas_api_get_email_data(data); 						
			case DVB_CA_WORK_TIME_INFO: /*????ʱ????Ϣ*/
				return app_divitone_cas_api_get_worktime_data(data);
			case DVB_CA_RATING_INFO:
				return app_divitone_cas_api_get_watchlevel_data(data);
			case DVB_CA_ENTITLE_INFO:
				return app_divitone_cas_api_get_entitle_data(data);
			default:
				break;	
		}
	
	return NULL;	
}


bool app_divitone_cas_api_delete(ca_delete_data_t *data)
{
	int32_t dvb_ca_flag =  app_flash_get_config_dvb_ca_flag();
	if (dvb_ca_flag != DVB_CA_TYPE_DIVITONE)
		return FALSE;


	if (NULL == data)
		return FALSE;
	
	switch(data->date_type)
		{
			case DVB_CA_EMAIL_INFO: /*?ʼ?*/	
				return app_divitone_cas_api_delete_email(data);
			default:
				break;	
		}
	
	return 0;	
}

bool app_divitone_cas_api_delete_all(ca_delete_data_t *data)
{
	int32_t dvb_ca_flag =  app_flash_get_config_dvb_ca_flag();
	if (dvb_ca_flag != DVB_CA_TYPE_DIVITONE)
		return FALSE;


	if (NULL == data)
		return FALSE;
	

	switch(data->date_type)
		{
			case DVB_CA_EMAIL_INFO: /*?ʼ?*/	
				return app_divitone_cas_api_delete_all_email(data);
			default:
				break;	
		}

	
	return 0;		
}


uint8_t app_divitone_cas_api_get_count(ca_get_count_t *data)
{
	int32_t dvb_ca_flag =  app_flash_get_config_dvb_ca_flag();
	if (dvb_ca_flag != DVB_CA_TYPE_DIVITONE)
		return 0;

	if (NULL == data)
		return 0;
	

	switch(data->date_type)
		{
			case DVB_CA_EMAIL_INFO: /*??\u0152?*/
				return app_divitone_cas_api_get_email_count(data);							
			case DVB_CA_ENTITLE_INFO: /*???\u0161*/
				return app_divitone_cas_api_get_entitle_count(data);
			default:
				break;	
		}

	
	return 0;		
}

int32_t app_divitone_cas_gxmsg_ca_on_event_exec(GxMessage * msg)
{
	switch(msg->msg_id)
		{
			case GXMSG_CA_ON_EVENT:
					{
						GxMsgProperty0_OnEvent* event;
						event = GxBus_GetMsgPropertyPtr(msg, GxMsgProperty0_OnEvent);
						printf("app_by_cas_gxmsg_ca_on_event_exec GXMSG_CA_ON_EVENT\n");
						switch (event->key) {
								case DVB_CA_BASE_INFO:		       /*??????Ϣ*/
									break;
								case DVB_CA_OPERATOR_INFO:		/*??Ӫ????Ϣ*/	
									break;
								case DVB_CA_EMAIL_INFO:		/*?ʼ?*/
									app_divitone_cas_gxmsg_ca_on_event_email(event);	
									break;
								case DVB_CA_ENTITLE_INFO :			/*??Ȩ*/
									break;
								case DVB_CA_DETITLE_INFO:                /*????Ȩ*/
									break;
								case DVB_CA_EMERGENCY_INFO:           /*Ӧ???㲥*/
									break;
								case DVB_CA_ROLLING_INFO:               /*OSD??????Ϣ*/
									app_divitone_cas_gxmsg_ca_on_event_rolling(event);
									break;
								case DVB_CA_FINGER_INFO:                  /*ָ??*/
								//	app_divitone_cas_gxmsg_ca_on_event_finger(event);
									break;
								case DVB_CA_CARD_UPDATE_INFO:       /*??????*/
									break;
								case DVB_CA_FETURE_INFO:                  /*????ֵ*/
									break;
								case DVB_CA_IPPV_POP_INFO:                /*IPPV????????Ϣ*/
									break;
								case DVB_CA_IPPT_POP_INFO:                /*IPPT????????Ϣ*/
									break;
								case DVB_CA_IPPV_PROG_INFO:            /*IPPV?ѹ?????Ŀ??Ϣ*/
									break;
								case DVB_CA_IPPV_SLOT_INFO:             /*IPPVǮ????Ϣ*/
									break;
//								case DVB_CA_CHILD_CARD_INFO:            /*?ӿ???Ϣ*/
//									break;
								case DVB_CA_MOTHER_CARD_INFO:            /*ĸ????Ϣ*/
									app_divitone_cas_gxmsg_ca_on_event_activechild(event);
									break;
								case DVB_CA_CHANGE_PIN_INFO:             /*?޸???????Ϣ*/
									break;
								case DVB_CA_PROGRESS_INFO:             /*???ܿ???????????Ϣ*/
									break;
								case DVB_CA_RATING_INFO:                /*???˼?????Ϣ*/
									break;
								case DVB_CA_WORK_TIME_INFO:          /*????ʱ????Ϣ*/
									break;
								case DVB_CA_CURTAIN_INFO:          /*??��??Ϣ*/
									break;
								case DVB_CA_PAIRED_INFO:          /*??????Ϣ*/
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


uint8_t app_divitone_cas_api_is_ca_pop_msg_type(uint8_t type)
{
	if ((type >= MSG_POP_COMMON_END+0+1)&&(type <=MSG_POP_COMMON_END+1+23))
		return TRUE;
	return FALSE;
}

int32_t app_divitone_cas_api_pop_msg_exec(uint8_t type)
{

	switch(type-MSG_POP_COMMON_END-1)
	{
		case 0:
		case 2: 
		case 9: 
		case 18: 
			app_divitone_cas_hide_ca_prompt();
			break;
		case 1:
			app_divitone_cas_show_ca_prompt("NO AUTHORITY!");
			break;
		case 3: 
			app_divitone_cas_show_ca_prompt("CONDITIONIAL_LIMITED PLAY!");
			break;
		case 7: 
			app_divitone_cas_show_ca_prompt("SMART CARD Initializing,NOT Available!");
			break;
		case 8: 
			app_divitone_cas_show_ca_prompt("Mounting SMART CARD,Waiting be Enabled!");
			break;
		case 10:
			app_divitone_cas_show_ca_prompt("OWING FEE!");
			break;
		case 11: 
			app_divitone_cas_show_ca_prompt("Been Repairing");
			break;
		case 12: 
			app_divitone_cas_show_ca_prompt("Apply for stoping service!");
			break;
		case 13: 
			app_divitone_cas_show_ca_prompt("Being Logout!");
			break;
		case 15:
			app_divitone_cas_show_ca_prompt("OUT OF WORKTIME!");
			break;
		case 17:
			app_divitone_cas_show_ca_prompt("OUT OF RATING!");
			break;
		case 19: 
			app_divitone_cas_show_ca_prompt("CHILD CARD CAN ACTIVED TEMPARAY");
			break;
		case 20: 
			app_divitone_cas_show_ca_prompt("CHILD CARD HAS BEEN EXPIRED!");
			break;
		case 21:
			app_divitone_cas_show_ca_prompt("STB TO CARD PAIRED ERROR!");
			break;
		case 22: 
			app_divitone_cas_show_ca_prompt("INVALID SMARTCARD!");
			break;
		case 23: 
			app_divitone_cas_show_ca_prompt("NO SMARTCARD!");
			break;
		default:
			break;
	}
	return 0;

}

int32_t app_divitone_cas_api_osd_exec(void)
{
/*
* ??????Ϣ??ʾ??????
*/
	app_divitone_cas_rolling_message_exec();

/*
* ?ʼ?ͼ????ʾ??????
*/
	app_divitone_cas_email_flag_exec();

/*
* ָ????ʾ??????
*/
//	app_divitone_cas_finger_exec();
	return 0;
}



APP_CAS_API_ControlBlock_t DivitoneCasPortControlBlock=
{
	.m_cas_init_callback =  app_divitone_cas_api_init,
	.m_cas_close_callback = NULL,
	.m_cas_api_card_in_callback = app_divitone_cas_api_card_in,
	.m_cas_api_card_out_callback = app_divitone_cas_api_card_out,
	.m_cas_api_release_ecm_filter_callback= app_divitone_cas_api_release_ecm_filter,
	.m_cas_api_start_ecm_filter_callback = app_divitone_cas_api_start_ecm_filter,
	.m_cas_api_release_emm_filter_callback = app_divitone_cas_api_release_emm_filter,
	.m_cas_api_start_emm_filter_callback = app_divitone_cas_api_start_emm_filter,
	.m_cas_api_check_cat_ca_descriptor_valid_callback= app_divitone_cas_api_check_cat_ca_descriptor_valid,
	.m_cas_api_check_pmt_ca_descriptor_valid_callback= app_divitone_cas_api_check_pmt_ca_descriptor_valid,
	.m_cas_api_is_ca_pop_msg_type_callback= app_divitone_cas_api_is_ca_pop_msg_type,
	.m_cas_api_pop_msg_exec_callback= app_divitone_cas_api_pop_msg_exec,
	.m_cas_api_osd_exec_callback= app_divitone_cas_api_osd_exec,
	.m_cas_api_gxmsg_ca_on_event_exec_callback= app_divitone_cas_gxmsg_ca_on_event_exec, 								
	.m_cas_api_init_data_callback = app_divitone_cas_api_init_data,
	.m_cas_api_get_data_callback = app_divitone_cas_api_get_data,
	.m_cas_api_get_count_callback = app_divitone_cas_api_get_count,
	.m_cas_api_delete_callback = app_divitone_cas_api_delete,
	.m_cas_api_delete_all_callback = app_divitone_cas_api_delete_all,
	.m_cas_api_buy_ipp_callback = NULL,
	.m_cas_api_get_lock_status_callback =	NULL,//app_divitone_cas_api_get_lock_status,
	.m_cas_api_feed_mother_child_card_callback = NULL,
	.m_cas_api_change_pin_callback = app_divitone_cas_api_change_pin,
	.m_cas_api_verify_pin_callback = NULL,
	.m_cas_api_change_rating_callback = app_divitone_cas_api_change_watchlevel,
	.m_cas_api_change_worktime_callback= app_divitone_cas_api_change_worktime,
};




