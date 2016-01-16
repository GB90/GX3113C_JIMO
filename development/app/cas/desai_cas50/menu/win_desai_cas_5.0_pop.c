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
#include "app_common_play.h"
#include "app_common_porting_stb_api.h"
#include "app_win_interface.h"
#include "win_desai_cas_5.0_pop.h"
#include "app_common_porting_ca_flash.h"
#include "app_common_porting_ca_smart.h"
#include "app_common_porting_ca_demux.h"
#include "app_common_porting_ca_descramble.h"

#include "app_desai_cas_5.0_api_demux.h"
#include "win_desai_cas_5.0_rolling_message.h"
#include "win_desai_cas_5.0_finger.h"
#include "win_desai_cas_5.0_exigent.h"
//#include "win_cd_cas_3.0_detitle.h"
//#include "win_cd_cas_3.0_entitle.h"
#include "win_desai_cas_5.0_email.h"
#include "win_desai_cas_5.0_ippv_buy.h"

#include "desai_ca_api.h"

#include "app_desai_cas_5.0_api_pop.h"

extern uint32_t g_smartcard_upgrade_num;
extern void app_show_prompt(char* context);
extern void app_hide_prompt(void);
extern int32_t app_desai_cas50_gxmsg_ca_on_event_updating(GxMsgProperty0_OnEvent* event);

//static int32_t app_desai_cas50_api_set_card_update_status(GxMsgProperty0_OnEvent* event);

int32_t app_desai_cas50_gxmsg_ca_on_event_exec(GxMessage * msg)
{
	switch(msg->msg_id)
		{
			case GXMSG_CA_ON_EVENT:
			{
				GxMsgProperty0_OnEvent* event;
				event = GxBus_GetMsgPropertyPtr(msg, GxMsgProperty0_OnEvent);
				printf("service_msg_to_app GXMSG_CA_ON_EVENT event->key=%d\n",event->key);
				switch (event->key) {

					case DVB_CA_BASE_INFO:		       /*������Ϣ*/
						break;
					case DVB_CA_OPERATOR_INFO:		/*��Ӫ����Ϣ*/	
						break;
					case DVB_CA_EMAIL_INFO:		/*�ʼ�*/
						{
							app_desai_cas50_gxmsg_ca_on_event_email(event);
						}
						break;
					case DVB_CA_ENTITLE_INFO: /*��Ȩ*/							
						break;
					case DVB_CA_DETITLE_INFO: /*����Ȩ*/							
						break;
					case DVB_CA_EMERGENCY_INFO:           /*Ӧ���㲥*/
						{
							app_desai_cas50_gxmsg_ca_on_event_exigent(event);
						}
						break;
					case DVB_CA_ROLLING_INFO: /*OSD������Ϣ*/
						{
							app_desai_cas50_gxmsg_ca_on_event_rolling(event);
						}
						break;
					case DVB_CA_FINGER_INFO:                  /*ָ��*/
						{
							app_desai_cas50_gxmsg_ca_on_event_finger(event);
						}
						break;
					case DVB_CA_CARD_UPDATE_INFO:       /*������*/
                        app_desai_cas50_gxmsg_ca_on_event_updating(event);
						break;
					case DVB_CA_FETURE_INFO:                  /*����ֵ*/
						break;
					case DVB_CA_IPPV_POP_INFO:                /*IPPV�������Ϣ*/
						{
							app_desai_cas50_gxmsg_ca_on_event_ippv_buy(event);
						}
						break;
					case DVB_CA_IPPT_POP_INFO:                /*IPPT�������Ϣ*/
						break;
					case DVB_CA_IPPV_PROG_INFO:            /*IPPV�ѹ����Ŀ��Ϣ*/
						break;
					case DVB_CA_IPPV_SLOT_INFO:             /*IPPVǮ����Ϣ*/
						{
							app_desai_cas50_gxmsg_ca_on_event_ippv_slot(event);
						}
						break;
				//	case DVB_CA_CHILD_CARD_INFO:            /*�ӿ���Ϣ*/
				//		break;
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

uint8_t app_desai_cas50_api_is_ca_pop_msg_type(uint8_t type)
{
	if ((type >= MSG_POP_COMMON_END+MESSAGE_CANCEL_TYPE+1)&&(type <=MSG_POP_COMMON_END+MESSAGE_CARD_NEED_UPDATE_TYPE-49+1))
		return TRUE;
	
	return FALSE;
}

/*
* app_cd_cas30_show_ca_prompt��app_show_promptʵ�ֹ����п��ܴ��ڲ��
* 
*/
static void app_desai_cas50_show_ca_prompt(char* context)
{
	app_show_prompt(context);
	return;
}

/*
* app_cd_cas30_hide_ca_prompt��app_hide_promptʵ�ֹ����п��ܴ��ڲ��
* 
*/
static void app_desai_cas50_hide_ca_prompt(void)
{
	app_hide_prompt();
	return;		
}

	
int32_t app_desai_cas50_api_pop_msg_exec(uint8_t type)
{
	uint8_t type_bak = 0;
	uint8_t type_bak1 = 0;
	char hintStr[100];
	char * language =NULL;
	
	type_bak1 = type - MSG_POP_COMMON_END-1;
	
	if(type_bak1 < 8)
		type_bak = type;
	else if(type_bak1 < 13)
		type_bak = type+8;
	else if(type_bak1 < 18)
		type_bak = type+19;
	else if(type_bak1 < 26)
		type_bak = type+30;
	else if(type_bak1 < 32)
		type_bak = type+38;
	else if(type_bak1 < 42)
		type_bak = type+48;
	else if(type_bak1 < 45)
		type_bak = type+54;	
	else if(type_bak1 < 50)
		type_bak = type+56;
	else if(type_bak1 == 50)
		type_bak = type+64;
	
	switch(type_bak-MSG_POP_COMMON_END-1)
		{
		case MESSAGE_CANCEL_TYPE: /* ȡ����ǰ����ʾ */
			printf("cancel show.\n");
			app_desai_cas50_hide_ca_prompt();
			break;
		case MESSAGE_BADCARD_TYPE: /* �޷�ʶ�� */
			printf("Can't Identify Card!.\n");

			app_desai_cas50_show_ca_prompt("(DSCA)Can't Identify Card!");
			break;
		case MESSAGE_EXPICARD_TYPE: /* ���ܿ����ڣ�������¿� */
			printf("Card overdueplease change card!\n");

			app_desai_cas50_show_ca_prompt("(DSCA)Card overdueplease change card!");
			break;
		case MESSAGE_CARD_INIT_TYPE: /* ���ܿ����ڳ�ʼ�� */
			printf("(DSCA)Smart card initializating\n");

			app_desai_cas50_show_ca_prompt("(DSCA)Smart card initializating");
			break;
		case MESSAGE_INSERTCARD_TYPE: /* ���Ž�Ŀ����������ܿ� */
			printf("(DSCA)Encrypted program, please insert card!\n");

			app_desai_cas50_show_ca_prompt("(DSCA)Encrypted program, please insert card!");
			break;
		case MESSAGE_CARD_INIT_ERR_TYPE: /*���ܿ���ʼ��ʧ��*/
			printf("(DSCA)Initialization failed\n");

			app_desai_cas50_show_ca_prompt("(DSCA)Initialization failed");
			break;
		case MESSAGE_STB_NO_CARD_TYPE:		/*�������в��������ܿ�*/
			printf("(DSCA)No card inside STB\n");

			app_desai_cas50_show_ca_prompt("(DSCA)No card inside STB");
			break;
		case MESSAGE_CARD_INIT_OK_TYPE:		/*���ܿ���ʼ�����*/
			printf("(DSCA)Initialization complete\n");

			app_desai_cas50_show_ca_prompt("(DSCA)Initialization complete");
			break;
		case MESSAGE_NOOPER_TYPE:			/*���в����ڽ�Ŀ��Ӫ��*/
			printf("(DSCA)No operation infor in smart card\n");

			app_desai_cas50_show_ca_prompt("(DSCA)No operation infor in smart card");
			break;
		case MESSAGE_CARDSTOP_TYPE:		/*���ܿ���ͣ��*/
			printf("(DSCA)Smart card freezed\n");

			app_desai_cas50_show_ca_prompt("(DSCA)Smart card freezed");
			break;
		case MESSAGE_OUTWORKTIME_TYPE:		/*���ڹ���ʱ����*/
			printf("(DSCA)Work time not correct\n");

			app_desai_cas50_show_ca_prompt("(DSCA)Work time not correct");
			break;
		case MESSAGE_WATCHLEVEL_TYPE:		/*��Ŀ��������趨�ۿ�����*/
			printf("(DSCA)Program level is higher than rating\n");

			app_desai_cas50_show_ca_prompt("(DSCA)Program level is higher than rating");
			break;
		case MESSAGE_PAIRING_TYPE:			/*����û�ж�Ӧ*/
			printf("(DSCA)Program need pairded!\n");

			app_desai_cas50_show_ca_prompt("(DSCA)Program need pairded!");
			break;
		case MESSAGE_NOENTITLE_TYPE:		/*��û�й���ý�Ŀ*/
			printf("(DSCA)Watch the program need bought\n");

			app_desai_cas50_show_ca_prompt("(DSCA)Watch the program need bought");
			break;
		case MESSAGE_DECRYPTFAIL_TYPE:		/*��Ŀ����ʧ��*/
			printf("(DSCA)Unlock the program failed\n");

			app_desai_cas50_show_ca_prompt("(DSCA)Unlock the program failed");
			break;
		case MESSAGE_NOMONEY_TYPE:		/*���ڽ���*/
			printf("(DSCA)No enough money in the card\n");

			app_desai_cas50_show_ca_prompt("(DSCA)No enough money in the card");
			break;
		case MESSAGE_ERRREGION_TYPE:		/*������ȷ*/
			printf("DSCA)Area not correct\n");

			app_desai_cas50_show_ca_prompt("(DSCA)Area not correct");
			break;
	//	case MESSAGE_FINGER_SHOW_TYPE:		/*ָ����ʾ*/
	//		app_desai_cas50_show_ca_prompt("ָ����ʾ");
	//		break;
	//	case MESSAGE_FINGER_DISAPPEAR_TYPE:		/*ָ������*/
	//		app_desai_cas50_show_ca_prompt("ָ������");
	//		break;
		case MESSAGE_CARDNOTACTIVE_TYPE:		/*���ܿ�δ����*/
			printf("(DSCA)The smartcard not actived\n");

			app_desai_cas50_show_ca_prompt("(DSCA)The smartcard not actived");
			break;
		case MESSAGE_CARDLOGOUT_TYPE:		/*���ܿ���ע��*/
			printf("(DSCA)Smart card deleted\n");

			app_desai_cas50_show_ca_prompt("(DSCA)Smart card deleted");
			break;
		case MESSAGE_CONDITION_CTL_TYPE:		/*�ò�Ʒ���޲�*/
			printf("(DSCA)The program is freezed\n");

			app_desai_cas50_show_ca_prompt("(DSCA)The program is freezed");
			break;
		case MESSAGE_DESCRA_NOENTITLE_TYPE:		/*��Ȩ�ѵ��ڣ�����ϵ��Ӫ��*/
			printf("(DSCA)No entitle,please contract operator.\n");

			app_desai_cas50_show_ca_prompt("(DSCA)No entitle,please contract operator");
			break;
		case MESSAGE_ENTITLE_UPDATE_TYPE:		/*��Ȩ�����У����Եȣ�*/
			printf("(DSCA)Please wait ,entitle the card now\n");

			app_desai_cas50_show_ca_prompt("(DSCA)Please wait ,entitle the card now");
			break;
		case MESSAGE_CARD_UNWONTED_TYPE:		/*���ܿ��ڲ��쳣*/
			printf("(DSCA)Data error inside card.\n");

			app_desai_cas50_show_ca_prompt("(DSCA)Data error inside card");
			break;
		case MESSAGE_CHANGEPRO_TOO_FAST_TYPE:		/*�벻ҪƵ����̨*/
			printf("(DSCA)Please don't switch program so fast\n");

			app_desai_cas50_show_ca_prompt("(DSCA)Please don't switch program so fast");
			break;
		case MESSAGE_STB_NEED_UPDATE_TYPE: /* ��������Ҫ���������Ե� */
			printf("(DSCA)Updated stb. please wait!\n");

			app_desai_cas50_show_ca_prompt("(DSCA)Updated stb. please wait!");

			break;
		case MESSAGE_MS_PAIR_OK_TYPE:		/* ��ĸ����Գɹ� */
			printf("(DSCA)Pairing successful\n");

			app_desai_cas50_show_ca_prompt("(DSCA)Pairing successful");
			break;
		case MESSAGE_MS_PAIR_FAILED_TYPE:		/* ��ĸ�����ʧ��,�����²�����ȷ��ĸ�� */
			printf("(DSCA)Pairing failed,insert the master card again\n");

            app_desai_cas50_show_ca_prompt("(DSCA)Pairing failed,insert the master card again.");
			break;
		case MESSAGE_MS_REPAIR_TYPE:		/* �ӿ������º�ĸ����� */
			printf("(DSCA)Slave card need repair with master card.\n");

			app_desai_cas50_show_ca_prompt("(DSCA)Slave card need repair with master card");
			break;
		case MESSAGE_CARD_MATCH_OK_TYPE:		/*������Գɹ�*/
			printf("(DSCA)STB and smart card pairing successful\n");

			app_desai_cas50_show_ca_prompt("(DSCA)STB and smart card pairing successful");
			break;
		case MESSAGE_CARD_REMATCH_TYPE:		/*���»������*/
			printf("(DSCA)Repairing the STB and smart card.\n");

			app_desai_cas50_show_ca_prompt("(DSCA)Repairing the STB and smart card");
			break;
		case MESSAGE_CANCEL_MATCH_TYPE:		/*ȡ���������*/
			printf("(DSCA)STB and smart card pairing cancelled\n");

			app_desai_cas50_show_ca_prompt("(DSCA)STB and smart card pairing cancelled");
			break;
	//	case MESSAGE_NEWEMAIL_TYPE:		/* ��ʾ���ʼ�ͼ�� */  
	//		app_desai_cas50_show_ca_prompt("��ʾ���ʼ�ͼ�� ");
	//		break;
	//	case MESSAGE_OVEREMAIL_TYPE:		/* ��ʾ�ʼ����� */
	//		app_desai_cas50_show_ca_prompt("��ʾ�ʼ�����");
	//		break;
	//	case MESSAGE_HIDE_NEWEMAIL_TYPE:		/* �������ʼ�ͼ��*/
	//		app_desai_cas50_show_ca_prompt(" �������ʼ�ͼ��");
	//		break;			
		case MESSAGE_EXIGENT_PROG_TYPE:		/*�����㲥*/
			//app_desai_cas50_show_ca_prompt("(DSCA)Emergent broadcast open");
			break;
		case MESSAGE_EXIGENT_PROG_STOP_TYPE:		/*ֹͣ�����㲥*/
			//app_desai_cas50_show_ca_prompt("Emergent broadcast close");
			break;
		case MESSAGE_CARD_UNLOCK_OK_TYPE:		/*���ܿ���������ɹ�*/
			printf("(DSCA)Smart card PIN unloc.\n");

			app_desai_cas50_show_ca_prompt("(DSCA)Smart card PIN unlock");
			break;
		case MESSAGE_PURSER_UPDATE_TYPE:		/*����Ǯ������*/
			printf("(DSCA)Wallet update\n");

			app_desai_cas50_show_ca_prompt("(DSCA)Wallet update");
			break;
		case MESSAGE_NET_ERR_LOCK_TYPE:		/*�����쳣����������*/
			printf("(DSCA)Net error, smartcard locked\n");

			app_desai_cas50_show_ca_prompt("(DSCA)Net error, smartcard locked");
			break;
		case MESSAGE_SYSTEM_ERR_LOCK_TYPE:		/*ϵͳ�쳣����������*/
			printf("(DSCA)System error, smart card locked\n");

			app_desai_cas50_show_ca_prompt("(DSCA)System error, smart card locked");
			break;
		case MESSAGE_NET_ERR_UNLOCK_TYPE:		/*�����쳣����*/
			printf("(DSCA)Net error to unlock card\n");

			app_desai_cas50_show_ca_prompt("(DSCA)Net error to unlock card");
			break;
		case MESSAGE_SYSTEM_ERR_UNLOCK_TYPE:		/*ϵͳ�쳣����*/
			printf("(DSCA)System error to unlock card\n");

			app_desai_cas50_show_ca_prompt("(DSCA)System error to unlock card");
			break;
		case MESSAGE_CARD_UPDATE_BEGIN_TYPE:
			printf("(DSCA)Updated smart card. please wait!\n");

			app_desai_cas50_show_ca_prompt("(DSCA)Updated smart card. please wait!");
			break;
		case MESSAGE_CARD_UPDATE_PROGRESS_TYPE:
			language = app_flash_get_config_osd_language();
			if(language &&strcmp(language,"English") == 0)
				sprintf(hintStr,"Updating smart card. please wait! %d%%",g_smartcard_upgrade_num);
			else
				sprintf(hintStr,"���ܿ����ڸ��£�����ϵ�! %d%%",g_smartcard_upgrade_num);
			app_desai_cas50_show_ca_prompt(hintStr);
			app_play_clear_ca_msg();
			app_play_set_msg_pop_type_record_pos(MSG_POP_NONE);
			break;
		case MESSAGE_CARD_UPDATE_ERR_TYPE:
			language = app_flash_get_config_osd_language();
			if(language &&strcmp(language,"English") == 0)
				sprintf(hintStr,"Update error. error code %d",g_smartcard_upgrade_num);
			else
				sprintf(hintStr,"���³����������%d",g_smartcard_upgrade_num);
			app_desai_cas50_show_ca_prompt(hintStr);
			app_play_clear_ca_msg();
			break;
		case MESSAGE_CARD_NEED_UPDATE_TYPE:		/*���ܿ���Ҫ����*/
			app_desai_cas50_show_ca_prompt("(DSCA)Updated smart card. please wait!");
			break;
		case MESSAGE_IPPV_OUTTIME_TYPE:		/*IPPV��Ŀ����*/
			app_desai_cas50_show_ca_prompt("(DSCA)Ippv program out of date");
			break;
		default:
			break;
		}
	return 0;
}

int32_t app_desai_cas50_api_osd_exec(void)
{
//	app_cd_cas30_rolling_message_exec();
//	app_cd_cas30_email_flag_exec();
//	app_cd_cas30_detitle_flag_exec();
//	app_cd_cas30_finger_exec();

	app_desai_cas50_rolling_message_exec();
	app_desai_cas50_email_flag_exec();
	app_desai_cas50_finger_exec();	
	return 1;
}
/**/
