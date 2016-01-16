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
#include "app_by_cas_api_smart.h"
#include "app_by_cas_api_demux.h"
#include "BYCApubtype.h"
#include "BYCAca_stb.h"
#include "app_common_porting_ca_smart.h"

/*
* ??ԶCA\u0153ӿ?ͷ??\u0152?
*/
#include "app_by_cas_api_demux.h"
#include "app_by_cas_api_smart.h"
#include "app_by_cas_api_entitle.h"
#include "app_by_cas_api_email.h"
#include "app_by_cas_api_baseinfo.h"
#include "app_by_cas_api_lock_service.h"
#include "app_by_cas_api_rolling_message.h"
#include "app_by_cas_api_finger.h"
extern 	void app_by_cas_hide_ca_prompt(void);
extern void app_by_cas_show_ca_prompt(char* context);

extern uint8_t bat_recieve_flag ;

uint8_t app_by_cas_api_init(void)
{

	app_porting_ca_flash_init(64*1024);
	app_porting_ca_demux_init();
	app_porting_ca_descramble_init();
	app_porting_ca_create_filter_task();
	if (BYCA_ERROR== BYCASTB_Init(1))
	{
		CA_FAIL(" BYCASTB_Init  failed \n");
		return FALSE;
	}
	


//	app_by_cas_api_smart_init();
	app_porting_ca_smart_create_task();
//	app_flash_save_config_center_freq(858);


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
int32_t app_by_cas_api_init_data(dvb_ca_data_type_t date_type)
{
	int32_t dvb_ca_flag =  app_flash_get_config_dvb_ca_flag();
	if (dvb_ca_flag != DVB_CA_TYPE_BY)
		return FALSE;

	switch(date_type)
		{
			case DVB_CA_BASE_INFO:	/*???\u0178??Ϣ*/
				return app_by_cas_api_init_baseinfo_data();
			case DVB_CA_EMAIL_INFO: /*??\u0152?*/	
				return app_by_cas_api_init_email_data();							
			case DVB_CA_ENTITLE_INFO: /*???\u0161*/ 
				return app_by_cas_api_init_entitle_data();
			default:
				break;	
		}


	
	return FALSE;		
}

char* app_by_cas_api_get_data(ca_get_date_t *data)
{
	int32_t dvb_ca_flag =  app_flash_get_config_dvb_ca_flag();
	if (dvb_ca_flag != DVB_CA_TYPE_BY)
		return NULL;


	if (NULL == data)
		return NULL;	

	switch(data->date_type)
		{
			case DVB_CA_BASE_INFO:	/*???\u0178??Ϣ*/
				return app_by_cas_api_get_baseinfo_data(data);
			case DVB_CA_EMAIL_INFO: /*??\u0152?*/	
				return app_by_cas_api_get_email_data(data); 						
			case DVB_CA_ENTITLE_INFO: /*???\u0161*/
				return app_by_cas_api_get_entitle_data(data);
			default:
				break;	
		}
	
	return NULL;	
}


bool app_by_cas_api_delete(ca_delete_data_t *data)
{
	int32_t dvb_ca_flag =  app_flash_get_config_dvb_ca_flag();
	if (dvb_ca_flag != DVB_CA_TYPE_BY)
		return FALSE;


	if (NULL == data)
		return FALSE;
	
	switch(data->date_type)
		{
			case DVB_CA_EMAIL_INFO: /*?ʼ?*/	
				return app_by_cas_api_delete_email(data);
			default:
				break;	
		}
	
	return 0;	
}

bool app_by_cas_api_delete_all(ca_delete_data_t *data)
{
	int32_t dvb_ca_flag =  app_flash_get_config_dvb_ca_flag();
	if (dvb_ca_flag != DVB_CA_TYPE_BY)
		return FALSE;


	if (NULL == data)
		return FALSE;
	

	switch(data->date_type)
		{
			case DVB_CA_EMAIL_INFO: /*?ʼ?*/	
				return app_by_cas_api_delete_all_email(data);
			default:
				break;	
		}

	
	return 0;		
}


uint8_t app_by_cas_api_get_count(ca_get_count_t *data)
{
	int32_t dvb_ca_flag =  app_flash_get_config_dvb_ca_flag();
	if (dvb_ca_flag != DVB_CA_TYPE_BY)
		return 0;

	if (NULL == data)
		return 0;
	

	switch(data->date_type)
		{
			case DVB_CA_EMAIL_INFO: /*??\u0152?*/
				return app_by_cas_api_get_email_count(data);							
			case DVB_CA_ENTITLE_INFO: /*???\u0161*/
				return app_by_cas_api_get_entitle_count(data);
			default:
				break;	
		}

	
	return 0;		
}

int32_t app_by_cas_gxmsg_ca_on_event_exec(GxMessage * msg)
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
										{
											app_by_cas_gxmsg_ca_on_event_email(event);											
									}
									break;
								case DVB_CA_ENTITLE_INFO :			/*??Ȩ*/
									break;
								case DVB_CA_DETITLE_INFO:                /*????Ȩ*/
									break;
								case DVB_CA_EMERGENCY_INFO:           /*Ӧ???㲥*/
									break;
								case DVB_CA_ROLLING_INFO:               /*OSD??????Ϣ*/
									app_by_cas_gxmsg_ca_on_event_rolling(event);
									break;
								case DVB_CA_FINGER_INFO:                  /*ָ??*/
									app_by_cas_gxmsg_ca_on_event_finger(event);
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


uint8_t app_by_cas_api_is_ca_pop_msg_type(uint8_t type)
{
	if ((type >= MSG_POP_COMMON_END+BYCA_NO_ERROR+1)&&(type <=MSG_POP_COMMON_END+1+BYCA_ERROR_STBFACTORY_ID_ERROR))
		return TRUE;
	return FALSE;
}

int32_t app_by_cas_api_pop_msg_exec(uint8_t type)
{
	switch(type-MSG_POP_COMMON_END-1)
		{
		case BYCA_NO_ERROR: /*û?д?????ȡ???ϴ???ʾ*/
			app_by_cas_hide_ca_prompt();
			break;
		case BYCA_ERROR_InitCardCommand: /*???ܿ???ʼ?????? 0x01*/
			app_by_cas_show_ca_prompt("ERROR InitCardCommand");
			break;
		case BYCA_ERROR_ReadCardCommand: /*???ܿ??��??????????? 0x02*/
			app_by_cas_show_ca_prompt("ERROR ReadCardCommand");
			break;
		case BYCA_ERROR_NO_CARD: /*?????????ܿ?*/
			app_by_cas_show_ca_prompt("ERROR NO CARD");
			break;
		case BYCA_ERROR_EMM_RET: /*EMM???? 0x04*/
			app_by_cas_show_ca_prompt("ERROR EMM RET");
			break;
		case BYCA_ERROR_ECM_RET: /*ECM???? 0x05*/
			app_by_cas_show_ca_prompt("ERROR ECM RET");
			break;
		case BYCA_ECM_RET_READCOMM_ERROR: /*???ܿ?????ECM???ݴ??? 0x06*/
			app_by_cas_show_ca_prompt("ECM RET READCOMM ERROR");
			break;
		case BYCA_EMMECM_RET_P1P2_ERROR: /*???ܿ?????ECM EMM P1P2???ݴ??? 0x07*/
			app_by_cas_show_ca_prompt("EMMECM RET P1P2 ERROR");
			break;
		case BYCA_EMMECM_RET_P3_ERROR: /*???ܿ?????ECM EMM P3???ݴ??? 0x08*/
			app_by_cas_show_ca_prompt("EMMECM RET P3 ERROR");
			break;
		case BYCA_EMMECM_RET_DES_ERROR: /*???ܿ??û?????Ȩ*/
			app_by_cas_show_ca_prompt("EMMECM RET DES ERROR");
			break;
		case BYCA_ECM_RET_NOENTITLE_ERROR: /*???ܿ??û?????Ȩ*/
			app_by_cas_show_ca_prompt("ECM RET NOENTITLE ERROR");
			break;
		case BYCA_EMMECM_RET_STORE_ERROR:/*???ܿ????ش??? 0x0a*/
			app_by_cas_show_ca_prompt("EMMECM RET STORE ERROR");
			break;
		case BYCA_EMMECM_RET_CHECK_ERROR: /*???ܿ??û?????Ȩ*/
			app_by_cas_show_ca_prompt("EMMECM RET CHECK ERROR");
			break;
		case BYCA_EMM_NOKEY_OR_NOFILE_ERROR: /*???ܿ??û?????Ȩ*/
			app_by_cas_show_ca_prompt("EMM NOKEY OR NOFILE ERROR");
			break;
		case BYCA_ECM_OUTDATE_ERROR: /*???ܿ??û?????Ȩ*/
			app_by_cas_show_ca_prompt("ECM OUTDATE ERROR");
			break;
		case BYCA_EMMECM_RET_UNKNOW_ERROR: /*???ܿ?ECM EMM????δ֪???? 0x0f*/
			app_by_cas_show_ca_prompt("EMMECM RET UNKNOW ERROR");
			break;
		case BYCA_CARD_NEED_PAIRING: /*??ĿҪ??????????*/
			app_by_cas_show_ca_prompt("CARD NEED PAIRING");
			break;
		case BYCA_CARD_PAIRING_OK: /*??Ŀ???????Գɹ?*/
			app_by_cas_show_ca_prompt("CARD PAIRING OK");
			break;
		case BYCA_NO_ECM_INFO: /*û??ECM????Ϣ*/
			app_by_cas_show_ca_prompt("NO ECM INFO");
			break;
		case BYCA_ZONE_FORBID_ERROR: /*?ý?Ŀ???????޲?*/
			app_by_cas_show_ca_prompt("ZONE FORBID ERROR");
			break;
		case BYCA_UserCdn_Limit_Ctrl_Enable: /*???û????????޲?*/
			app_by_cas_show_ca_prompt("UserCdn Limit Ctrl Enable");
			break;
		case BYCA_UserCdn_Limit_Ctrl_Disable: /*???û???ȡ???????޲?*/
			app_by_cas_show_ca_prompt("UserCdn Limit Ctrl Disable");
			break;
		case BYCA_ZoneCdn_Limit_Ctrl_Enable: /*?????????????޲?*/
			app_by_cas_show_ca_prompt("ZoneCdn Limit Ctrl Enable");
			break;
		case BYCA_ZoneCdn_Limit_Ctrl_Disable: /*????????ȡ???????޲?*/
			app_by_cas_show_ca_prompt("ZoneCdn Limit Ctrl Disable");
			break;
		case BYCA_PAIRED_OtherCARD_ERROR:  /*?û???????????????*/
			app_by_cas_show_ca_prompt("PAIRED OtherCARD ERROR");
			break;
		case BYCA_PAIRED_OtherMachine_ERROR:  /*?ÿ????˻?????????*/
			app_by_cas_show_ca_prompt("PAIRED OtherMachine ERROR");
			break;
		case BYCA_MASTERSLAVE_PAIRED_OK: /*??ĸ?????Գɹ?*/
			app_by_cas_show_ca_prompt("MASTERSLAVE PAIRED OK");
			break;
		case BYCA_MASTERSLAVE_PAIRED_TIMEUPDATE:/*??ĸ??????ʱ??????*/
			app_by_cas_show_ca_prompt("MASTERSLAVE PAIRED TIMEUPDATE");
			break;
		case BYCA_MASTERSLAVE_NEEDPAIRED:/*??ĸ????Ҫ????*/
			app_by_cas_show_ca_prompt("MASTERSLAVE PAIRED TIMEUPDATE");
			break;
		case BYCA_CARD_CANCEL_PAIRED_OK:/*????????ȡ???ɹ?*/
			app_by_cas_show_ca_prompt("CARD CANCEL PAIRED OK");
			break;
		case STATUE_CA_CARD_ZONE_INVALID:/*???????ܿ????ٷ?????*/
			app_by_cas_show_ca_prompt("STATUE CA CARD ZONE INVALID");
			break;
		case STATUE_CA_CARD_ZONE_CHECK_ENTER:/*????Ѱ?????磬???ȴ?*/
			app_by_cas_show_ca_prompt("STATUE CA CARD ZONE CHECK ENTER");
			break;
		case STATUE_CA_CARD_ZONE_CHEKC_OK:/*???ι??ܿ?????????*/
			app_by_cas_show_ca_prompt("STATUE CA CARD ZONE CHEKC OK");
			break;
		case BYCA_ERROR_CardSystemID_ERROR:
			app_by_cas_show_ca_prompt("ERROR CardSystemID ERROR");
			break;
		case BYCA_ERROR_STBFACTORY_ID_ERROR:
			app_by_cas_show_ca_prompt("ERROR STBFACTORY ID ERROR");
			break;
		default:
			break;
		}
	return 0;
}






int32_t app_by_cas_api_osd_exec(void)
{
/*
* ??????Ϣ??ʾ??????
*/
	app_by_cas_rolling_message_exec();

/*
* ?ʼ?ͼ????ʾ??????
*/
	app_by_cas_email_flag_exec();

/*
* ָ????ʾ??????
*/
	app_by_cas_finger_exec();
	return 0;
}



APP_CAS_API_ControlBlock_t ByCasPortControlBlock=
{
	.m_cas_init_callback =  app_by_cas_api_init,
	.m_cas_close_callback = NULL,
	.m_cas_api_card_in_callback = app_by_cas_api_card_in,
	.m_cas_api_card_out_callback = app_by_cas_api_card_out,
	.m_cas_api_release_ecm_filter_callback= app_by_cas_api_release_ecm_filter,
	.m_cas_api_start_ecm_filter_callback = app_by_cas_api_start_ecm_filter,
	.m_cas_api_release_emm_filter_callback = app_by_cas_api_release_emm_filter,
	.m_cas_api_start_emm_filter_callback = app_by_cas_api_start_emm_filter,
	.m_cas_api_check_cat_ca_descriptor_valid_callback= app_by_cas_api_check_cat_ca_descriptor_valid,
	.m_cas_api_check_pmt_ca_descriptor_valid_callback= app_by_cas_api_check_pmt_ca_descriptor_valid,
	.m_cas_api_is_ca_pop_msg_type_callback= app_by_cas_api_is_ca_pop_msg_type,
	.m_cas_api_pop_msg_exec_callback= app_by_cas_api_pop_msg_exec,
	.m_cas_api_osd_exec_callback= app_by_cas_api_osd_exec,
	.m_cas_api_gxmsg_ca_on_event_exec_callback= app_by_cas_gxmsg_ca_on_event_exec, 								
	.m_cas_api_init_data_callback = app_by_cas_api_init_data,
	.m_cas_api_get_data_callback = app_by_cas_api_get_data,
	.m_cas_api_get_count_callback = app_by_cas_api_get_count,
	.m_cas_api_delete_callback = app_by_cas_api_delete,
	.m_cas_api_delete_all_callback = app_by_cas_api_delete_all,
	.m_cas_api_buy_ipp_callback = NULL,
	.m_cas_api_get_lock_status_callback =	app_by_cas_api_get_lock_status,
	.m_cas_api_feed_mother_child_card_callback = NULL,
	.m_cas_api_change_pin_callback = NULL,
	.m_cas_api_verify_pin_callback = NULL,
	.m_cas_api_change_rating_callback = NULL,
	.m_cas_api_change_worktime_callback= NULL,
};




