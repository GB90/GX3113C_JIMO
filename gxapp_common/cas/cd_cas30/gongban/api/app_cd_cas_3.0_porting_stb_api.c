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
* Purpose   :	ģ��ͷ�ļ�
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
* �����Ӳ�CA�ӿ�ͷ�ļ�
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
* ���½ӿ�CA�˵�����
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
*  ��ȡ��Ӧ����֮ǰ����ʼ�����ݹ���
(һ�㴴����Ӧ����֮ǰ��create�е���)��
* ���ʼ���ʼ�����Ȩ��
*/
int32_t app_cd_cas30_api_init_data(dvb_ca_data_type_t date_type)
{
	int32_t dvb_ca_flag =  app_flash_get_config_dvb_ca_flag();
	if (dvb_ca_flag != DVB_CA_TYPE_CDCAS30)
		return FALSE;

	switch(date_type)
		{
			case DVB_CA_BASE_INFO:  /*������Ϣ*/
				return app_cd_cas30_api_init_baseinfo_data();
			case DVB_CA_OPERATOR_INFO: /*��Ӫ����Ϣ*/
				break;
			case DVB_CA_EMAIL_INFO: /*�ʼ�*/
				return app_cd_cas30_api_init_email_data();
			case DVB_CA_ENTITLE_INFO: /*��Ȩ*/		
				return app_cd_cas30_api_init_entitle_data();
			case DVB_CA_DETITLE_INFO: /*����Ȩ*/		
				return app_cd_cas30_api_init_detitle_data();						
			case DVB_CA_CARD_UPDATE_INFO: /*������*/
				break;
			case DVB_CA_FETURE_INFO: /*����ֵ*/
				return app_cd_cas30_api_init_features_data();
				break;
			case DVB_CA_IPPV_POP_INFO: /*IPPV�������Ϣ*/
				break;
			case DVB_CA_IPPT_POP_INFO: /*IPPT�������Ϣ*/
				break;
			case DVB_CA_IPPV_PROG_INFO: /*IPPV�ѹ����Ŀ��Ϣ*/
				return app_cd_cas30_api_init_ippv_info_data();				
				break;
			case DVB_CA_IPPV_SLOT_INFO: /*IPPVǮ����Ϣ*/
				return app_cd_cas30_api_init_ippv_slot_data();
				break;
//			case DVB_CA_CHILD_CARD_INFO: /*�ӿ���Ϣ*/
//				break;
			case DVB_CA_MOTHER_CARD_INFO: /*ĸ����Ϣ*/
				return app_cd_cas30_api_init_mother_child_data();				
				break;
			case DVB_CA_CHANGE_PIN_INFO: /*�޸�������Ϣ*/
				break;
			case DVB_CA_PROGRESS_INFO: /*���ܿ�����������Ϣ*/
				break;
			case DVB_CA_RATING_INFO: /*���˼�����Ϣ*/
				break;
			case DVB_CA_WORK_TIME_INFO: /*����ʱ����Ϣ*/
				return app_cd_cas30_api_init_worktime_data();
			case DVB_CA_CURTAIN_INFO: /*������Ϣ*/
				break;
			case DVB_CA_PAIRED_INFO: /*�����Ϣ*/
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
			case DVB_CA_BASE_INFO:  /*������Ϣ*/
				return app_cd_cas30_api_get_baseinfo_data(data);							
			case DVB_CA_OPERATOR_INFO: /*��Ӫ����Ϣ*/	
				break;
			case DVB_CA_EMAIL_INFO: /*�ʼ�*/
				return app_cd_cas30_api_get_email_data(data);
			case DVB_CA_ENTITLE_INFO: /*��Ȩ*/		
				return app_cd_cas30_api_get_entitle_data(data);
			case DVB_CA_DETITLE_INFO: /*����Ȩ*/		
				return app_cd_cas30_api_get_detitle_data(data);						
			case DVB_CA_CARD_UPDATE_INFO: /*������*/
				break;
			case DVB_CA_FETURE_INFO: /*����ֵ*/
				return app_cd_cas30_api_get_features_data(data);				
				break;
			case DVB_CA_IPPV_POP_INFO: /*IPPV�������Ϣ*/
				break;
			case DVB_CA_IPPT_POP_INFO: /*IPPT�������Ϣ*/
				break;
			case DVB_CA_IPPV_PROG_INFO: /*IPPV�ѹ����Ŀ��Ϣ*/
				return app_cd_cas30_api_get_ippv_info_data(data);
				break;
			case DVB_CA_IPPV_SLOT_INFO: /*IPPVǮ����Ϣ*/
				return app_cd_cas30_api_get_ippv_slot_data(data);				
				break;
//			case DVB_CA_CHILD_CARD_INFO: /*�ӿ���Ϣ*/
//				break;
			case DVB_CA_MOTHER_CARD_INFO: /*ĸ����Ϣ*/
				return app_cd_cas30_api_get_mother_child_data(data);				
				break;
			case DVB_CA_CHANGE_PIN_INFO: /*�޸�������Ϣ*/
				break;
			case DVB_CA_PROGRESS_INFO: /*���ܿ�����������Ϣ*/
				break;
			case DVB_CA_RATING_INFO: /*���˼�����Ϣ*/
				return app_cd_cas30_api_get_rating_data(data);				
			case DVB_CA_WORK_TIME_INFO: /*����ʱ����Ϣ*/
				return app_cd_cas30_api_get_worktime_data(data);				
			case DVB_CA_CURTAIN_INFO: /*������Ϣ*/
				break;
			case DVB_CA_PAIRED_INFO: /*�����Ϣ*/
				return app_cd_cas30_api_get_paired_data(data);							
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
uint8_t app_cd_cas30_api_update_data(ca_get_date_t *data)
{
	int32_t dvb_ca_flag =  app_flash_get_config_dvb_ca_flag();

	if (dvb_ca_flag != DVB_CA_TYPE_CDCAS30)
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

uint8_t app_cd_cas30_api_get_count(ca_get_count_t *data)
{
	int32_t dvb_ca_flag =  app_flash_get_config_dvb_ca_flag();
	if (dvb_ca_flag != DVB_CA_TYPE_CDCAS30)
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
				return app_cd_cas30_api_get_email_count(data);
			case DVB_CA_ENTITLE_INFO: /*��Ȩ*/
				return app_cd_cas30_api_get_entitle_count(data);
			case DVB_CA_DETITLE_INFO: /*����Ȩ*/	
				return app_cd_cas30_api_get_detitle_count(data);													
			case DVB_CA_EMERGENCY_INFO: /*Ӧ���㲥*/
				break;
			case DVB_CA_ROLLING_INFO: /*OSD������Ϣ*/
				break;
			case DVB_CA_FINGER_INFO: /*ָ��*/
				break;
			case DVB_CA_CARD_UPDATE_INFO: /*������*/
				break;
			case DVB_CA_FETURE_INFO: /*����ֵ*/
				return app_cd_cas30_api_get_features_count(data);				
				break;
			case DVB_CA_IPPV_POP_INFO: /*IPPV�������Ϣ*/
				break;
			case DVB_CA_IPPT_POP_INFO: /*IPPT�������Ϣ*/
				break;
			case DVB_CA_IPPV_PROG_INFO: /*IPPV�ѹ����Ŀ��Ϣ*/
				return app_cd_cas30_api_get_ippv_info_count(data);				
				break;
			case DVB_CA_IPPV_SLOT_INFO: /*IPPVǮ����Ϣ*/
				return app_cd_cas30_api_get_ippv_slot_count(data);
				break;
//			case DVB_CA_CHILD_CARD_INFO: /*�ӿ���Ϣ*/
//				break;
			case DVB_CA_MOTHER_CARD_INFO: /*ĸ����Ϣ*/
				return app_cd_cas30_api_get_mother_child_count(data);				
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

bool app_cd_cas30_api_delete(ca_delete_data_t *data)
{
	int32_t dvb_ca_flag =  app_flash_get_config_dvb_ca_flag();
	if (dvb_ca_flag != DVB_CA_TYPE_CDCAS30)
		return FALSE;


	if (NULL == data)
		return FALSE;
	
	switch(data->date_type)
		{
			case DVB_CA_EMAIL_INFO: /*�ʼ�*/	
				return app_cd_cas30_api_delete_email(data);
			case DVB_CA_DETITLE_INFO: /*����Ȩ*/	
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
			case DVB_CA_EMAIL_INFO: /*�ʼ�*/	
				return app_cd_cas30_api_delete_all_email(data);
			case DVB_CA_DETITLE_INFO: /*����Ȩ*/	
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

					case DVB_CA_BASE_INFO:		       /*������Ϣ*/
						break;
					case DVB_CA_OPERATOR_INFO:		/*��Ӫ����Ϣ*/	
						break;
					case DVB_CA_EMAIL_INFO:		/*�ʼ�*/
						{
							app_cd_cas30_gxmsg_ca_on_event_email(event);
						}
						break;
					case DVB_CA_ENTITLE_INFO: /*��Ȩ*/							
						{
							app_cd_cas30_gxmsg_ca_on_event_entitle(event);
						}
						break;
					case DVB_CA_DETITLE_INFO: /*����Ȩ*/							
						{
							app_cd_cas30_gxmsg_ca_on_event_detitle(event);
						}
						break;
					case DVB_CA_EMERGENCY_INFO:           /*Ӧ���㲥*/
						break;
					case DVB_CA_ROLLING_INFO: /*OSD������Ϣ*/
						{
							app_cd_cas30_gxmsg_ca_on_event_rolling(event);
						}
						break;
					case DVB_CA_FINGER_INFO:                  /*ָ��*/
							{
							app_cd_cas30_gxmsg_ca_on_event_finger(event);
						}
						break;
					case DVB_CA_CARD_UPDATE_INFO:       /*������*/
							{
								app_cd_cas30_gxmsg_ca_on_event_cardupdating(event);
						}
						break;
					case DVB_CA_FETURE_INFO:                  /*����ֵ*/
						break;
					case DVB_CA_IPPV_POP_INFO:                /*IPPV�������Ϣ*/
							{
						app_cd_cas30_gxmsg_ca_on_event_ippv_buy(event);
						}
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
							{
								app_cd_cas30_gxmsg_ca_on_event_feed(event);								
						}
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
		case CDCA_MESSAGE_CANCEL_TYPE: /* ȡ����ǰ����ʾ */
			app_cd_cas30_hide_ca_prompt();
			break;
		case CDCA_MESSAGE_BADCARD_TYPE: /* �޷�ʶ�� */
			app_cd_cas30_show_ca_prompt("Unidentified Card");
			break;
		case CDCA_MESSAGE_EXPICARD_TYPE: /* ���ܿ����ڣ�������¿� */
			app_cd_cas30_show_ca_prompt("Card Is Expire");
			break;
		case CDCA_MESSAGE_INSERTCARD_TYPE: /* ���Ž�Ŀ����������ܿ� */
			app_cd_cas30_show_ca_prompt("Please insert the smart card");
			break;
		case CDCA_MESSAGE_NOOPER_TYPE: /* ���в����ڽ�Ŀ��Ӫ�� */
			app_cd_cas30_show_ca_prompt("Operator ID error");
			break;
		case CDCA_MESSAGE_BLACKOUT_TYPE: /* �������� */
			app_cd_cas30_show_ca_prompt("Conditional Blackout");
			break;
		case CDCA_MESSAGE_OUTWORKTIME_TYPE: /* ��ǰʱ�α��趨Ϊ���ܹۿ� */
			app_cd_cas30_show_ca_prompt("Out of Working Hours");
			break;
		case CDCA_MESSAGE_WATCHLEVEL_TYPE: /* ��Ŀ��������趨�Ĺۿ����� */
			app_cd_cas30_show_ca_prompt("Out of Teleview Rating");
			break;
		case CDCA_MESSAGE_PAIRING_TYPE: /* ���ܿ��뱾�����в���Ӧ */
			app_cd_cas30_show_ca_prompt("The card is not paired with this STB");
			break;
		case CDCA_MESSAGE_NOENTITLE_TYPE: /* û����Ȩ */
			app_cd_cas30_show_ca_prompt("No Entitlement");
			break;
		case CDCA_MESSAGE_DECRYPTFAIL_TYPE: /* ��Ŀ����ʧ�� */
			app_cd_cas30_show_ca_prompt("Decrypt fail");
			break;
		case CDCA_MESSAGE_NOMONEY_TYPE: /* ���ڽ��� */
			app_cd_cas30_show_ca_prompt("No enough money");
			break;
		case CDCA_MESSAGE_ERRREGION_TYPE: /* ������ȷ */
			app_cd_cas30_show_ca_prompt("Regional Lockout");
			break;
		case CDCA_MESSAGE_NEEDFEED_TYPE: /* �ӿ���Ҫ��ĸ����Ӧ�������ĸ�� */
			app_cd_cas30_show_ca_prompt("Please insert the parent-card of the current card");
			break;
		case CDCA_MESSAGE_ERRCARD_TYPE: /* ���ܿ�У��ʧ�ܣ�����ϵ��Ӫ�� */
			app_cd_cas30_show_ca_prompt("Card upgrade failed.Please contact the operator");		
			break;
		case CDCA_MESSAGE_UPDATE_TYPE: /* ���ܿ������У��벻Ҫ�ο����߹ػ� */
			app_cd_cas30_show_ca_prompt("Card in patching. Not to remove the card or power off");
			break;
		case CDCA_MESSAGE_LOWCARDVER_TYPE: /* ���������ܿ� */
			app_cd_cas30_show_ca_prompt("Please upgrade the smart card");
			break;
		case CDCA_MESSAGE_VIEWLOCK_TYPE: /* ����Ƶ���л�Ƶ�� */
			app_cd_cas30_show_ca_prompt("Please do not switch channels too frequently");
			break;
		case CDCA_MESSAGE_MAXRESTART_TYPE: /* ���ܿ���ʱ���ߣ���5���Ӻ����¿��� */
			app_cd_cas30_show_ca_prompt("Card is in dormancy. Please insert the card after 5 minutes");
			break;
		case CDCA_MESSAGE_FREEZE_TYPE: /*���ܿ��Ѷ��ᣬ����ϵ��Ӫ�� */
			app_cd_cas30_show_ca_prompt("Card is freezed. Please contact the operator");
			break;
		case CDCA_MESSAGE_CALLBACK_TYPE: /*���ܿ�����ͣ����ش����Ӽ�¼����Ӫ�� */
			app_cd_cas30_show_ca_prompt("Please send the IPPV records back to the operator");
			break;
		case CDCA_MESSAGE_CURTAIN_TYPE: /*������Ŀ������Ԥ���׶�*/
			app_cd_cas30_show_ca_prompt("Curtain Program.Can't be watched now");
			break;
		case CDCA_MESSAGE_CARDTESTSTART_TYPE: /*�������Կ�������...*/
			app_cd_cas30_show_ca_prompt("Card test start");
			break;
		case CDCA_MESSAGE_CARDTESTFAILD_TYPE: /*�������Կ�����ʧ�ܣ��������ͨѶģ��*/
			app_cd_cas30_show_ca_prompt("Card test failed");
			break;
		case CDCA_MESSAGE_CARDTESTSUCC_TYPE: /*�������Կ����Գɹ�*/
			app_cd_cas30_show_ca_prompt("Card test successfully");
			break;
		case CDCA_MESSAGE_NOCALIBOPER_TYPE: /*���в�������ֲ�ⶨ����Ӫ��*/
			app_cd_cas30_show_ca_prompt("No CA Lib Operator");
			break;
		case CDCA_MESSAGE_STBLOCKED_TYPE: /*������������*/
			app_cd_cas30_show_ca_prompt("Please restart the STB");
			break;
		case CDCA_MESSAGE_STBFREEZE_TYPE: /*�����б�����*/
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




