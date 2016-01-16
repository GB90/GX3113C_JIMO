/*****************************************************************************
* 						   CONFIDENTIAL
*        Hangzhou GuoXin Science and Technology Co., Ltd.
*                      (C)2012, All right reserved
******************************************************************************

******************************************************************************
* File Name :	app_dvt_cas_porting_stb_api.c
* Author    : 	zhouhm
* Project   :	goxceed dvbc
* Type      :
******************************************************************************
* Purpose   :	ģ��ͷ�ļ�
******************************************************************************
* Release History:
VERSION	Date			  AUTHOR         Description
1.0  	2013.11.29		  zhouhm 	 			creation
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
#include "DVTCAS_STBDataPublic.h"
#include "DVTCAS_STBInf.h"

/*
* �����Ӳ�CA�ӿ�ͷ�ļ�
*/
#include "app_dvt_cas_api_finger.h"
#include "app_dvt_cas_api_operator.h"
#include "app_dvt_cas_api_demux.h"
#include "app_dvt_cas_api_smart.h"
#include "app_dvt_cas_api_entitle.h"
#include "app_dvt_cas_api_baseinfo.h"
#include "app_dvt_cas_api_worktime.h"
#include "app_dvt_cas_api_rating.h"
#include "app_dvt_cas_api_pin.h"
#include "app_dvt_cas_api_email.h"
#include "app_dvt_cas_api_rolling_message.h"
#include "app_dvt_cas_api_lock_service.h"

extern char * GUI_GetFocusWindow(void);
extern 	int32_t app_dvt_cas_gxmsg_ca_on_event_finger(GxMsgProperty0_OnEvent* event);
//extern 	int32_t app_dvt_cas_finger_exec(void);
extern 	void app_dvt_cas_hide_ca_prompt(void);
extern void app_dvt_cas_show_ca_prompt(char* context);

/*
* ���½ӿ�CA�˵�����
*/
extern 	void JudgeSwitchChannel( void* arg );
void TaskJudgeSwitchChannel( void* arg )
{
	while(1)
	{
		JudgeSwitchChannel(arg);
	}

}
uint8_t app_dvt_cas_api_init(void)
{
	handle_t	handle;
	app_porting_ca_flash_init(64*1024*2);
	app_porting_ca_demux_init();
	app_porting_ca_descramble_init();
	app_dvt_cas_rolling_message_init();
	app_porting_ca_create_filter_task();
	GxCore_ThreadCreate("switch_channel",&handle, TaskJudgeSwitchChannel, NULL, 10 * 1024, /*30*/10);
	if (FALSE == DVTCASTB_Init())
	{
		CA_FAIL((" DVTCASTB_Init  failed \n"));
		return FALSE;
	}
	DVTCASTB_AddDebugMsgSign(FALSE);
//	DVTCASTB_AddDebugMsgSign(TRUE);

	app_porting_ca_smart_create_task();
	return TRUE;
	
}

/*
*  ��ȡ��Ӧ����֮ǰ����ʼ�����ݹ���
(һ�㴴����Ӧ����֮ǰ��create�е���)��
* ���ʼ���ʼ�����Ȩ��
*/
int32_t app_dvt_cas_api_init_data(dvb_ca_data_type_t date_type)
{
	int32_t dvb_ca_flag =  app_flash_get_config_dvb_ca_flag();
	if (dvb_ca_flag != DVB_CA_TYPE_DVT)
		return FALSE;

	switch(date_type)
		{
			case DVB_CA_BASE_INFO:  /*������Ϣ*/
				return app_dvt_cas_api_init_baseinfo_data();
			case DVB_CA_OPERATOR_INFO: /*��Ӫ����Ϣ*/
				return app_dvt_cas_api_init_operator_data();
			case DVB_CA_EMAIL_INFO: /*�ʼ�*/
				return app_dvt_cas_api_init_email_data();
			case DVB_CA_ENTITLE_INFO: /*��Ȩ*/		
				return app_dvt_cas_api_init_entitle_data();
			case DVB_CA_DETITLE_INFO: /*����Ȩ*/		
//				return app_cd_cas30_api_init_detitle_data();	
				break;
			case DVB_CA_CARD_UPDATE_INFO: /*������*/
				break;
			case DVB_CA_FETURE_INFO: /*����ֵ*/
//				return app_cd_cas30_api_init_features_data();
				break;
			case DVB_CA_IPPV_POP_INFO: /*IPPV�������Ϣ*/
				break;
			case DVB_CA_IPPT_POP_INFO: /*IPPT�������Ϣ*/
				break;
			case DVB_CA_IPPV_PROG_INFO: /*IPPV�ѹ����Ŀ��Ϣ*/
//				return app_cd_cas30_api_init_ippv_info_data();				
				break;
			case DVB_CA_IPPV_SLOT_INFO: /*IPPVǮ����Ϣ*/
//				return app_cd_cas30_api_init_ippv_slot_data();
				break;
//			case DVB_CA_CHILD_CARD_INFO: /*�ӿ���Ϣ*/
//				break;
			case DVB_CA_MOTHER_CARD_INFO: /*ĸ����Ϣ*/
//				return app_cd_cas30_api_init_mother_child_data();				
				break;
			case DVB_CA_CHANGE_PIN_INFO: /*�޸�������Ϣ*/
				break;
			case DVB_CA_PROGRESS_INFO: /*���ܿ�����������Ϣ*/
				break;
			case DVB_CA_RATING_INFO: /*���˼�����Ϣ*/
				return app_dvt_cas_api_init_rating_data();				
			case DVB_CA_WORK_TIME_INFO: /*����ʱ����Ϣ*/
				return app_dvt_cas_api_init_worktime_data();
			case DVB_CA_CURTAIN_INFO: /*������Ϣ*/
				break;
			case DVB_CA_PAIRED_INFO: /*�����Ϣ*/
//				return app_cd_cas30_api_init_paired_data();
			default:
				break;	
		}

	
	return FALSE;		
}

char * app_dvt_cas_api_get_data(ca_get_date_t *data)
{
	int32_t dvb_ca_flag =  app_flash_get_config_dvb_ca_flag();
	if (dvb_ca_flag != DVB_CA_TYPE_DVT)
		return NULL;


	if (NULL == data)
		return NULL;	

	switch(data->date_type)
		{
			case DVB_CA_BASE_INFO:  /*������Ϣ*/
				return app_dvt_cas_api_get_baseinfo_data(data);							
			case DVB_CA_OPERATOR_INFO: /*��Ӫ����Ϣ*/	
				return app_dvt_cas_api_get_operator_data(data);							
			case DVB_CA_EMAIL_INFO: /*�ʼ�*/
				return app_dvt_cas_api_get_email_data(data);
			case DVB_CA_ENTITLE_INFO: /*��Ȩ*/	
				return app_dvt_cas_api_get_entitle_data(data);
			case DVB_CA_DETITLE_INFO: /*����Ȩ*/	
				break;
//				return app_cd_cas30_api_get_detitle_data(data);						
			case DVB_CA_CARD_UPDATE_INFO: /*������*/
				break;
			case DVB_CA_FETURE_INFO: /*����ֵ*/
//				return app_cd_cas30_api_get_features_data(data);				
				break;
			case DVB_CA_IPPV_POP_INFO: /*IPPV�������Ϣ*/
				break;
			case DVB_CA_IPPT_POP_INFO: /*IPPT�������Ϣ*/
				break;
			case DVB_CA_IPPV_PROG_INFO: /*IPPV�ѹ����Ŀ��Ϣ*/
//				return app_cd_cas30_api_get_ippv_info_data(data);
				break;
			case DVB_CA_IPPV_SLOT_INFO: /*IPPVǮ����Ϣ*/
//				return app_cd_cas30_api_get_ippv_slot_data(data);				
				break;
//			case DVB_CA_CHILD_CARD_INFO: /*�ӿ���Ϣ*/
//				break;
			case DVB_CA_MOTHER_CARD_INFO: /*ĸ����Ϣ*/
//				return app_cd_cas30_api_get_mother_child_data(data);				
				break;
			case DVB_CA_CHANGE_PIN_INFO: /*�޸�������Ϣ*/
				break;
			case DVB_CA_PROGRESS_INFO: /*���ܿ�����������Ϣ*/
				break;
			case DVB_CA_RATING_INFO: /*���˼�����Ϣ*/
				return app_dvt_cas_api_get_rating_data(data);				
			case DVB_CA_WORK_TIME_INFO: /*����ʱ����Ϣ*/
				return app_dvt_cas_api_get_worktime_data(data);				
			case DVB_CA_CURTAIN_INFO: /*������Ϣ*/
				break;
			case DVB_CA_PAIRED_INFO: /*�����Ϣ*/
				break;
//				return app_cd_cas30_api_get_paired_data(data);							
			default:
				break;	
		}

	
	return NULL;	
}

/*
*  ����CA����
(CA���ݷ������ʱ����)��
* ��ɾ���ʼ�������Ȩ��CA֪ͨ��Ȩ��Ϣ�仯��
*/
uint8_t app_dvt_cas_api_update_data(ca_get_date_t *data)
{
	int32_t dvb_ca_flag =  app_flash_get_config_dvb_ca_flag();

	if (dvb_ca_flag != DVB_CA_TYPE_DVT)
		return 0;


	if (NULL == data)
		return 0;	

	switch(data->date_type)
		{
			case DVB_CA_BASE_INFO:  /*������Ϣ*/
				break;
			case DVB_CA_OPERATOR_INFO: /*��Ӫ����Ϣ*/
				break;
			case DVB_CA_EMAIL_INFO: /*�ʼ�*/	
				break;
			case DVB_CA_ENTITLE_INFO: /*��Ȩ*/
				break;
			case DVB_CA_DETITLE_INFO: /*����Ȩ*/
				break;
			case DVB_CA_EMERGENCY_INFO: /*Ӧ���㲥*/
				break;
			case DVB_CA_ROLLING_INFO: /*OSD������Ϣ*/
				break;
			case DVB_CA_FINGER_INFO: /*ָ��*/
				break;
			case DVB_CA_CARD_UPDATE_INFO: /*������*/
				break;
			case DVB_CA_FETURE_INFO: /*����ֵ*/
				break;
			case DVB_CA_IPPV_POP_INFO: /*IPPV�������Ϣ*/
				break;
			case DVB_CA_IPPT_POP_INFO: /*IPPT�������Ϣ*/
				break;
			case DVB_CA_IPPV_PROG_INFO: /*IPPV�ѹ����Ŀ��Ϣ*/
				break;
			case DVB_CA_IPPV_SLOT_INFO: /*IPPVǮ����Ϣ*/
				break;
//			case DVB_CA_CHILD_CARD_INFO: /*�ӿ���Ϣ*/
//				break;
			case DVB_CA_MOTHER_CARD_INFO: /*ĸ����Ϣ*/
				break;
			case DVB_CA_CHANGE_PIN_INFO: /*�޸�������Ϣ*/
				break;
			case DVB_CA_PROGRESS_INFO: /*���ܿ�����������Ϣ*/
				break;
			case DVB_CA_RATING_INFO: /*���˼�����Ϣ*/
				break;
			case DVB_CA_WORK_TIME_INFO: /*����ʱ����Ϣ*/
				break;
			case DVB_CA_CURTAIN_INFO: /*������Ϣ*/
				break;
			case DVB_CA_PAIRED_INFO: /*�����Ϣ*/
				break;
			default:
				break;	
		}
	
	return 0;	
}

uint8_t app_dvt_cas_api_get_count(ca_get_count_t *data)
{
	int32_t dvb_ca_flag =  app_flash_get_config_dvb_ca_flag();
	if (dvb_ca_flag != DVB_CA_TYPE_DVT)
		return 0;

	if (NULL == data)
		return 0;
	

	switch(data->date_type)
		{
			case DVB_CA_BASE_INFO:  /*������Ϣ*/
				break;
			case DVB_CA_OPERATOR_INFO: /*��Ӫ����Ϣ*/
				return app_dvt_cas_api_get_operator_count(data);
			case DVB_CA_EMAIL_INFO: /*�ʼ�*/
				return app_dvt_cas_api_get_email_count(data);
			case DVB_CA_ENTITLE_INFO: /*��Ȩ*/
				return app_dvt_cas_api_get_entitle_count(data);
			case DVB_CA_DETITLE_INFO: /*����Ȩ*/	
				break;													
			case DVB_CA_EMERGENCY_INFO: /*Ӧ���㲥*/
				break;
			case DVB_CA_ROLLING_INFO: /*OSD������Ϣ*/
				break;
			case DVB_CA_FINGER_INFO: /*ָ��*/
				break;
			case DVB_CA_CARD_UPDATE_INFO: /*������*/
				break;
			case DVB_CA_FETURE_INFO: /*����ֵ*/			
				break;
			case DVB_CA_IPPV_POP_INFO: /*IPPV�������Ϣ*/
				break;
			case DVB_CA_IPPT_POP_INFO: /*IPPT�������Ϣ*/
				break;
			case DVB_CA_IPPV_PROG_INFO: /*IPPV�ѹ����Ŀ��Ϣ*/				
				break;
			case DVB_CA_IPPV_SLOT_INFO: /*IPPVǮ����Ϣ*/
				break;
			case DVB_CA_MOTHER_CARD_INFO: /*ĸ����Ϣ*/			
				break;
			case DVB_CA_CHANGE_PIN_INFO: /*�޸�������Ϣ*/
				break;
			case DVB_CA_PROGRESS_INFO: /*���ܿ�����������Ϣ*/
				break;
			case DVB_CA_RATING_INFO: /*���˼�����Ϣ*/
				break;
			case DVB_CA_WORK_TIME_INFO: /*����ʱ����Ϣ*/
				break;
			case DVB_CA_CURTAIN_INFO: /*������Ϣ*/
				break;
			case DVB_CA_PAIRED_INFO: /*�����Ϣ*/
				break;
			default:
				break;	
		}
	
	return 0;		
}

bool app_dvt_cas_api_delete(ca_delete_data_t *data)
{
	int32_t dvb_ca_flag =  app_flash_get_config_dvb_ca_flag();
	if (dvb_ca_flag != DVB_CA_TYPE_DVT)
		return FALSE;


	if (NULL == data)
		return FALSE;
	
	switch(data->date_type)
		{
			case DVB_CA_EMAIL_INFO: /*�ʼ�*/
				return app_dvt_cas_api_delete_email(data);
			case DVB_CA_DETITLE_INFO: /*����Ȩ*/	
				break;
			default:
				break;	
		}
	
	return 0;	
}

bool app_dvt_cas_api_delete_all(ca_delete_data_t *data)
{
	int32_t dvb_ca_flag =  app_flash_get_config_dvb_ca_flag();
	if (dvb_ca_flag != DVB_CA_TYPE_DVT)
		return FALSE;


	if (NULL == data)
		return FALSE;
	

	switch(data->date_type)
		{
			case DVB_CA_EMAIL_INFO: /*�ʼ�*/
				return app_dvt_cas_api_delete_all_email(data);
			case DVB_CA_DETITLE_INFO: /*����Ȩ*/	
				break;
//				return app_cd_cas30_api_delete_all_detitle(data);	
			default:
				break;	
		}

	
	return 0;		
}

int32_t app_dvt_cas_gxmsg_ca_on_event_exec(GxMessage * msg)
{
	switch(msg->msg_id)
		{
			case GXMSG_CA_ON_EVENT:
			{
				GxMsgProperty0_OnEvent* event;
				event = GxBus_GetMsgPropertyPtr(msg, GxMsgProperty0_OnEvent);
				printf("app_dvt_cas_gxmsg_ca_on_event_exec GXMSG_CA_ON_EVENT event->key=%d\n",event->key);
				switch (event->key) {

					case DVB_CA_BASE_INFO:		       /*������Ϣ*/
						break;
					case DVB_CA_OPERATOR_INFO:		/*��Ӫ����Ϣ*/	
						break;
					case DVB_CA_EMAIL_INFO:		/*�ʼ�*/
						{
							app_dvt_cas_gxmsg_ca_on_event_email(event);
						}
						break;
					case DVB_CA_ENTITLE_INFO: /*��Ȩ*/							
						break;
					case DVB_CA_DETITLE_INFO: /*����Ȩ*/							
						break;
					case DVB_CA_EMERGENCY_INFO:           /*Ӧ���㲥*/
						break;
					case DVB_CA_ROLLING_INFO: /*OSD������Ϣ*/
						{
							app_dvt_cas_gxmsg_ca_on_event_rolling(event);
						}
						break;
					case DVB_CA_FINGER_INFO:                  /*ָ��*/
						{
							/*2014-03-05,ȫ������ָ��*/
							char* focus_win=GUI_GetFocusWindow();
							if(focus_win)
							{
								printf("focus_win=%s",focus_win);
								if((strcmp(focus_win,"win_full_screen")==0))
								{
									app_dvt_cas_gxmsg_ca_on_event_finger(event);
								}
							}
						}
						break;
					case DVB_CA_CARD_UPDATE_INFO:       /*������*/
						break;
					case DVB_CA_FETURE_INFO:                  /*����ֵ*/
						break;
					case DVB_CA_IPPV_POP_INFO:                /*IPPV�������Ϣ*/
						break;
					case DVB_CA_IPPT_POP_INFO:                /*IPPT�������Ϣ*/
						break;
					case DVB_CA_IPPV_PROG_INFO:            /*IPPV�ѹ����Ŀ��Ϣ*/
						break;
					case DVB_CA_IPPV_SLOT_INFO:             /*IPPVǮ����Ϣ*/
						break;
//					case DVB_CA_CHILD_CARD_INFO:            /*�ӿ���Ϣ*/
//						break;
					case DVB_CA_MOTHER_CARD_INFO:            /*ĸ����Ϣ*/
						break;
					case DVB_CA_CHANGE_PIN_INFO:             /*�޸�������Ϣ*/
						break;
					case DVB_CA_PROGRESS_INFO:             /*���ܿ�����������Ϣ*/
						break;
					case DVB_CA_RATING_INFO:                /*���˼�����Ϣ*/
						break;
					case DVB_CA_WORK_TIME_INFO:          /*����ʱ����Ϣ*/
						break;
					case DVB_CA_CURTAIN_INFO:          /*������Ϣ*/
						break;
					case DVB_CA_PAIRED_INFO:          /*�����Ϣ*/
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

uint8_t app_dvt_cas_api_is_ca_pop_msg_type(uint8_t type)
{
	if ((type >= MSG_POP_COMMON_END+DVTCA_RATING_TOO_LOW+1)&&(type <=MSG_POP_COMMON_END+DVTCA_SWTICH_CHANNEL+1))
		return TRUE;
	
	return FALSE;
}

int32_t app_dvt_cas_api_pop_msg_exec(uint8_t type)
{
	switch(type-MSG_POP_COMMON_END-1)
		{		
		case DVTCA_RATING_TOO_LOW: /*DVTCA_RATING_TOO_LOW*/
			app_dvt_cas_show_ca_prompt("Level limit");
			break;	
		case DVTCA_NOT_IN_WATCH_TIME: /*DVTCA_NOT_IN_WATCH_TIME*/
			app_dvt_cas_show_ca_prompt("Time limit");
			break;					
		case DVTCA_NOT_PAIRED: /*DVTCA_NOT_PAIRED*/
			app_dvt_cas_show_ca_prompt("Modem card not match");
			break;
		case DVTCA_IS_PAIRED_OTHER: /*DVTCA_IS_PAIRED_OTHER*/
			app_dvt_cas_show_ca_prompt("IC card match to other STB");
			break;
		case DVTCA_PLEASE_INSERT_CARD: /*DVTCA_PLEASE_INSERT_CARD*/
			app_dvt_cas_show_ca_prompt("Input smartcard");
			break;
		case DVTCA_NO_ENTITLE: /*DVTCA_NO_ENTITLE*/
			app_dvt_cas_show_ca_prompt("No pay this program");
			break;
		case DVTCA_PRODUCT_RESTRICT: /*DVTCA_PRODUCT_RESTRICT*/
			app_dvt_cas_show_ca_prompt("Corporation limit the program");
			break;
		case DVTCA_AREA_RESTRICT: /*DVTCA_AREA_RESTRICT*/
			app_dvt_cas_show_ca_prompt("Corporation limit the area");
			break;
		case DVTCA_MOTHER_RESTRICT: /*DVTCA_MOTHER_RESTRICT*/
			app_dvt_cas_show_ca_prompt("Daughter card has been limit, associate to master card");
			break;
		case DVTCA_NO_MONEY: /*DVTCA_NO_MONEY*/
			app_dvt_cas_show_ca_prompt("Money is not enough to watch, recharge please");
			break;
		case DVTCA_IPPV_NO_CONFIRM: /*DVTCA_IPPV_NO_CONFIRM*/
			app_dvt_cas_show_ca_prompt("IPPV program, make sure to buy or not");
			break;
		case DVTCA_IPPV_NO_BOOK: /*DVTCA_IPPV_NO_BOOK*/
			app_dvt_cas_show_ca_prompt("IPPV program, no pay no watch");
			break;
		case DVTCA_IPPT_NO_CONFIRM: /*DVTCA_IPPT_NO_CONFIRM*/
			app_dvt_cas_show_ca_prompt("IPPT program, make sure to buy or not");
			break;
		case DVTCA_IPPT_NO_BOOK: /*DVTCA_IPPT_NO_BOOK*/
			app_dvt_cas_show_ca_prompt("IPPT program, no pay no watch");
			break;
		case DVTCA_IPPV_NO_MONEY: /*DVTCA_IPPV_NO_MONEY*/
			app_dvt_cas_show_ca_prompt("IPPV program, money not enough, recharge please");
			break;
		case DVTCA_IPPT_NO_MONEY: /*DVTCA_IPPT_NO_MONEY*/
			app_dvt_cas_show_ca_prompt("IPPT program, money not enough, recharge please");
			break;						
		case DVTCA_DATA_INVALID: /*DVTCA_DATA_INVALID*/
			app_dvt_cas_show_ca_prompt("Invalid data, no tips.Due to CW");
			break;	
		case DVTCA_SLOT_NOT_FOUND: /*DVTCA_SLOT_NOT_FOUND*/
			app_dvt_cas_show_ca_prompt("Wallet not exist");
			break;	
		case DVTCA_SC_NOT_SERVER: /*DVTCA_SC_NOT_SERVER*/
			app_dvt_cas_show_ca_prompt("IC card service have been forbidden");
			break;
		case DVTCA_TVS_NOT_FOUND: /*DVTCA_TVS_NOT_FOUND*/
			app_dvt_cas_show_ca_prompt("Corporation not exist");
			break;					
		case DVTCA_KEY_NOT_FOUND: /*DVTCA_KEY_NOT_FOUND*/
			app_dvt_cas_show_ca_prompt("Card inactive, please call to corporation");
			break;	
		case DVTCA_IPPNEED_CALLBACK: /*DVTCA_IPPNEED_CALLBACK*/
			app_dvt_cas_show_ca_prompt("Please call to corporation, send IPP program back");
			break;			
		/*case DVTCA_FREE_PREVIEWING:
			app_dvt_cas_show_ca_prompt("No pay this program,free preview now...");
			break;		
			*/
		case DVTCA_SWTICH_CHANNEL:
			app_dvt_cas_show_ca_prompt("Accessing to information, please wait...");
			break;	
		default:
			break;
		}
	return 0;
}

int32_t app_dvt_cas_api_osd_exec(void)
{
	app_dvt_cas_rolling_message_exec();
	app_dvt_cas_email_flag_exec();
	app_dvt_cas_finger_exec();	
	return 1;
}



APP_CAS_API_ControlBlock_t DvtCasPortControlBlock=
{
	.m_cas_init_callback =  app_dvt_cas_api_init,
	.m_cas_close_callback = NULL,
	.m_cas_api_card_in_callback = app_dvt_cas_api_card_in,
	.m_cas_api_card_out_callback = app_dvt_cas_api_card_out,
	.m_cas_api_release_ecm_filter_callback= app_dvt_cas_api_release_ecm_filter,
	.m_cas_api_start_ecm_filter_callback = app_dvt_cas_api_start_ecm_filter,
	.m_cas_api_release_emm_filter_callback = app_dvt_cas_api_release_emm_filter,
	.m_cas_api_start_emm_filter_callback = app_dvt_cas_api_start_emm_filter,
	.m_cas_api_check_cat_ca_descriptor_valid_callback= app_dvt_cas_api_check_cat_ca_descriptor_valid,
	.m_cas_api_check_pmt_ca_descriptor_valid_callback= app_dvt_cas_api_check_pmt_ca_descriptor_valid,
	.m_cas_api_is_ca_pop_msg_type_callback= app_dvt_cas_api_is_ca_pop_msg_type,
	.m_cas_api_pop_msg_exec_callback= app_dvt_cas_api_pop_msg_exec,
	.m_cas_api_osd_exec_callback= app_dvt_cas_api_osd_exec,
	.m_cas_api_gxmsg_ca_on_event_exec_callback= app_dvt_cas_gxmsg_ca_on_event_exec, 								
	.m_cas_api_init_data_callback = app_dvt_cas_api_init_data,
	.m_cas_api_get_data_callback = app_dvt_cas_api_get_data,
	.m_cas_api_get_count_callback = app_dvt_cas_api_get_count,
	.m_cas_api_delete_callback = app_dvt_cas_api_delete,
	.m_cas_api_delete_all_callback = app_dvt_cas_api_delete_all,
	.m_cas_api_buy_ipp_callback = NULL,//;
	.m_cas_api_get_lock_status_callback = app_dvt_cas_api_get_lock_status,
	.m_cas_api_feed_mother_child_card_callback = NULL,
	.m_cas_api_change_pin_callback = app_dvt_cas_api_change_pin,
	.m_cas_api_verify_pin_callback = app_dvt_cas_api_verify_pin,
	.m_cas_api_change_rating_callback = app_dvt_cas_api_change_rating,
	.m_cas_api_change_worktime_callback= app_dvt_cas_api_change_worktime,
};




