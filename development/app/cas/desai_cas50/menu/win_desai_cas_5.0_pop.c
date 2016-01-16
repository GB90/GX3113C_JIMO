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

					case DVB_CA_BASE_INFO:		       /*基本信息*/
						break;
					case DVB_CA_OPERATOR_INFO:		/*运营商信息*/	
						break;
					case DVB_CA_EMAIL_INFO:		/*邮件*/
						{
							app_desai_cas50_gxmsg_ca_on_event_email(event);
						}
						break;
					case DVB_CA_ENTITLE_INFO: /*授权*/							
						break;
					case DVB_CA_DETITLE_INFO: /*反授权*/							
						break;
					case DVB_CA_EMERGENCY_INFO:           /*应急广播*/
						{
							app_desai_cas50_gxmsg_ca_on_event_exigent(event);
						}
						break;
					case DVB_CA_ROLLING_INFO: /*OSD滚动消息*/
						{
							app_desai_cas50_gxmsg_ca_on_event_rolling(event);
						}
						break;
					case DVB_CA_FINGER_INFO:                  /*指纹*/
						{
							app_desai_cas50_gxmsg_ca_on_event_finger(event);
						}
						break;
					case DVB_CA_CARD_UPDATE_INFO:       /*卡升级*/
                        app_desai_cas50_gxmsg_ca_on_event_updating(event);
						break;
					case DVB_CA_FETURE_INFO:                  /*特征值*/
						break;
					case DVB_CA_IPPV_POP_INFO:                /*IPPV购买框信息*/
						{
							app_desai_cas50_gxmsg_ca_on_event_ippv_buy(event);
						}
						break;
					case DVB_CA_IPPT_POP_INFO:                /*IPPT购买框信息*/
						break;
					case DVB_CA_IPPV_PROG_INFO:            /*IPPV已购买节目信息*/
						break;
					case DVB_CA_IPPV_SLOT_INFO:             /*IPPV钱包信息*/
						{
							app_desai_cas50_gxmsg_ca_on_event_ippv_slot(event);
						}
						break;
				//	case DVB_CA_CHILD_CARD_INFO:            /*子卡信息*/
				//		break;
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

uint8_t app_desai_cas50_api_is_ca_pop_msg_type(uint8_t type)
{
	if ((type >= MSG_POP_COMMON_END+MESSAGE_CANCEL_TYPE+1)&&(type <=MSG_POP_COMMON_END+MESSAGE_CARD_NEED_UPDATE_TYPE-49+1))
		return TRUE;
	
	return FALSE;
}

/*
* app_cd_cas30_show_ca_prompt与app_show_prompt实现功能有可能存在差别
* 
*/
static void app_desai_cas50_show_ca_prompt(char* context)
{
	app_show_prompt(context);
	return;
}

/*
* app_cd_cas30_hide_ca_prompt与app_hide_prompt实现功能有可能存在差别
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
		case MESSAGE_CANCEL_TYPE: /* 取消当前的显示 */
			printf("cancel show.\n");
			app_desai_cas50_hide_ca_prompt();
			break;
		case MESSAGE_BADCARD_TYPE: /* 无法识别卡 */
			printf("Can't Identify Card!.\n");

			app_desai_cas50_show_ca_prompt("(DSCA)Can't Identify Card!");
			break;
		case MESSAGE_EXPICARD_TYPE: /* 智能卡过期，请更换新卡 */
			printf("Card overdueplease change card!\n");

			app_desai_cas50_show_ca_prompt("(DSCA)Card overdueplease change card!");
			break;
		case MESSAGE_CARD_INIT_TYPE: /* 智能卡正在初始化 */
			printf("(DSCA)Smart card initializating\n");

			app_desai_cas50_show_ca_prompt("(DSCA)Smart card initializating");
			break;
		case MESSAGE_INSERTCARD_TYPE: /* 加扰节目，请插入智能卡 */
			printf("(DSCA)Encrypted program, please insert card!\n");

			app_desai_cas50_show_ca_prompt("(DSCA)Encrypted program, please insert card!");
			break;
		case MESSAGE_CARD_INIT_ERR_TYPE: /*智能卡初始化失败*/
			printf("(DSCA)Initialization failed\n");

			app_desai_cas50_show_ca_prompt("(DSCA)Initialization failed");
			break;
		case MESSAGE_STB_NO_CARD_TYPE:		/*机顶盒中不存在智能卡*/
			printf("(DSCA)No card inside STB\n");

			app_desai_cas50_show_ca_prompt("(DSCA)No card inside STB");
			break;
		case MESSAGE_CARD_INIT_OK_TYPE:		/*智能卡初始化完成*/
			printf("(DSCA)Initialization complete\n");

			app_desai_cas50_show_ca_prompt("(DSCA)Initialization complete");
			break;
		case MESSAGE_NOOPER_TYPE:			/*卡中不存在节目运营商*/
			printf("(DSCA)No operation infor in smart card\n");

			app_desai_cas50_show_ca_prompt("(DSCA)No operation infor in smart card");
			break;
		case MESSAGE_CARDSTOP_TYPE:		/*智能卡已停用*/
			printf("(DSCA)Smart card freezed\n");

			app_desai_cas50_show_ca_prompt("(DSCA)Smart card freezed");
			break;
		case MESSAGE_OUTWORKTIME_TYPE:		/*不在工作时段内*/
			printf("(DSCA)Work time not correct\n");

			app_desai_cas50_show_ca_prompt("(DSCA)Work time not correct");
			break;
		case MESSAGE_WATCHLEVEL_TYPE:		/*节目级别高于设定观看级别*/
			printf("(DSCA)Program level is higher than rating\n");

			app_desai_cas50_show_ca_prompt("(DSCA)Program level is higher than rating");
			break;
		case MESSAGE_PAIRING_TYPE:			/*机卡没有对应*/
			printf("(DSCA)Program need pairded!\n");

			app_desai_cas50_show_ca_prompt("(DSCA)Program need pairded!");
			break;
		case MESSAGE_NOENTITLE_TYPE:		/*您没有购买该节目*/
			printf("(DSCA)Watch the program need bought\n");

			app_desai_cas50_show_ca_prompt("(DSCA)Watch the program need bought");
			break;
		case MESSAGE_DECRYPTFAIL_TYPE:		/*节目解密失败*/
			printf("(DSCA)Unlock the program failed\n");

			app_desai_cas50_show_ca_prompt("(DSCA)Unlock the program failed");
			break;
		case MESSAGE_NOMONEY_TYPE:		/*卡内金额不足*/
			printf("(DSCA)No enough money in the card\n");

			app_desai_cas50_show_ca_prompt("(DSCA)No enough money in the card");
			break;
		case MESSAGE_ERRREGION_TYPE:		/*区域不正确*/
			printf("DSCA)Area not correct\n");

			app_desai_cas50_show_ca_prompt("(DSCA)Area not correct");
			break;
	//	case MESSAGE_FINGER_SHOW_TYPE:		/*指纹显示*/
	//		app_desai_cas50_show_ca_prompt("指纹显示");
	//		break;
	//	case MESSAGE_FINGER_DISAPPEAR_TYPE:		/*指纹隐藏*/
	//		app_desai_cas50_show_ca_prompt("指纹隐藏");
	//		break;
		case MESSAGE_CARDNOTACTIVE_TYPE:		/*智能卡未激活*/
			printf("(DSCA)The smartcard not actived\n");

			app_desai_cas50_show_ca_prompt("(DSCA)The smartcard not actived");
			break;
		case MESSAGE_CARDLOGOUT_TYPE:		/*智能卡已注销*/
			printf("(DSCA)Smart card deleted\n");

			app_desai_cas50_show_ca_prompt("(DSCA)Smart card deleted");
			break;
		case MESSAGE_CONDITION_CTL_TYPE:		/*该产品被限播*/
			printf("(DSCA)The program is freezed\n");

			app_desai_cas50_show_ca_prompt("(DSCA)The program is freezed");
			break;
		case MESSAGE_DESCRA_NOENTITLE_TYPE:		/*授权已到期，请联系运营商*/
			printf("(DSCA)No entitle,please contract operator.\n");

			app_desai_cas50_show_ca_prompt("(DSCA)No entitle,please contract operator");
			break;
		case MESSAGE_ENTITLE_UPDATE_TYPE:		/*授权更新中，请稍等！*/
			printf("(DSCA)Please wait ,entitle the card now\n");

			app_desai_cas50_show_ca_prompt("(DSCA)Please wait ,entitle the card now");
			break;
		case MESSAGE_CARD_UNWONTED_TYPE:		/*智能卡内部异常*/
			printf("(DSCA)Data error inside card.\n");

			app_desai_cas50_show_ca_prompt("(DSCA)Data error inside card");
			break;
		case MESSAGE_CHANGEPRO_TOO_FAST_TYPE:		/*请不要频繁切台*/
			printf("(DSCA)Please don't switch program so fast\n");

			app_desai_cas50_show_ca_prompt("(DSCA)Please don't switch program so fast");
			break;
		case MESSAGE_STB_NEED_UPDATE_TYPE: /* 机顶盒需要升级，请稍等 */
			printf("(DSCA)Updated stb. please wait!\n");

			app_desai_cas50_show_ca_prompt("(DSCA)Updated stb. please wait!");

			break;
		case MESSAGE_MS_PAIR_OK_TYPE:		/* 子母卡配对成功 */
			printf("(DSCA)Pairing successful\n");

			app_desai_cas50_show_ca_prompt("(DSCA)Pairing successful");
			break;
		case MESSAGE_MS_PAIR_FAILED_TYPE:		/* 子母卡配对失败,请重新插入正确的母卡 */
			printf("(DSCA)Pairing failed,insert the master card again\n");

            app_desai_cas50_show_ca_prompt("(DSCA)Pairing failed,insert the master card again.");
			break;
		case MESSAGE_MS_REPAIR_TYPE:		/* 子卡需重新和母卡配对 */
			printf("(DSCA)Slave card need repair with master card.\n");

			app_desai_cas50_show_ca_prompt("(DSCA)Slave card need repair with master card");
			break;
		case MESSAGE_CARD_MATCH_OK_TYPE:		/*机卡配对成功*/
			printf("(DSCA)STB and smart card pairing successful\n");

			app_desai_cas50_show_ca_prompt("(DSCA)STB and smart card pairing successful");
			break;
		case MESSAGE_CARD_REMATCH_TYPE:		/*重新机卡配对*/
			printf("(DSCA)Repairing the STB and smart card.\n");

			app_desai_cas50_show_ca_prompt("(DSCA)Repairing the STB and smart card");
			break;
		case MESSAGE_CANCEL_MATCH_TYPE:		/*取消机卡配对*/
			printf("(DSCA)STB and smart card pairing cancelled\n");

			app_desai_cas50_show_ca_prompt("(DSCA)STB and smart card pairing cancelled");
			break;
	//	case MESSAGE_NEWEMAIL_TYPE:		/* 显示新邮件图标 */  
	//		app_desai_cas50_show_ca_prompt("显示新邮件图标 ");
	//		break;
	//	case MESSAGE_OVEREMAIL_TYPE:		/* 显示邮件存满 */
	//		app_desai_cas50_show_ca_prompt("显示邮件存满");
	//		break;
	//	case MESSAGE_HIDE_NEWEMAIL_TYPE:		/* 隐藏新邮件图标*/
	//		app_desai_cas50_show_ca_prompt(" 隐藏新邮件图标");
	//		break;			
		case MESSAGE_EXIGENT_PROG_TYPE:		/*紧急广播*/
			//app_desai_cas50_show_ca_prompt("(DSCA)Emergent broadcast open");
			break;
		case MESSAGE_EXIGENT_PROG_STOP_TYPE:		/*停止紧急广播*/
			//app_desai_cas50_show_ca_prompt("Emergent broadcast close");
			break;
		case MESSAGE_CARD_UNLOCK_OK_TYPE:		/*智能卡密码解锁成功*/
			printf("(DSCA)Smart card PIN unloc.\n");

			app_desai_cas50_show_ca_prompt("(DSCA)Smart card PIN unlock");
			break;
		case MESSAGE_PURSER_UPDATE_TYPE:		/*电子钱包更新*/
			printf("(DSCA)Wallet update\n");

			app_desai_cas50_show_ca_prompt("(DSCA)Wallet update");
			break;
		case MESSAGE_NET_ERR_LOCK_TYPE:		/*网络异常，卡被锁定*/
			printf("(DSCA)Net error, smartcard locked\n");

			app_desai_cas50_show_ca_prompt("(DSCA)Net error, smartcard locked");
			break;
		case MESSAGE_SYSTEM_ERR_LOCK_TYPE:		/*系统异常，卡被锁定*/
			printf("(DSCA)System error, smart card locked\n");

			app_desai_cas50_show_ca_prompt("(DSCA)System error, smart card locked");
			break;
		case MESSAGE_NET_ERR_UNLOCK_TYPE:		/*网络异常解锁*/
			printf("(DSCA)Net error to unlock card\n");

			app_desai_cas50_show_ca_prompt("(DSCA)Net error to unlock card");
			break;
		case MESSAGE_SYSTEM_ERR_UNLOCK_TYPE:		/*系统异常解锁*/
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
				sprintf(hintStr,"智能卡正在更新，请勿断电! %d%%",g_smartcard_upgrade_num);
			app_desai_cas50_show_ca_prompt(hintStr);
			app_play_clear_ca_msg();
			app_play_set_msg_pop_type_record_pos(MSG_POP_NONE);
			break;
		case MESSAGE_CARD_UPDATE_ERR_TYPE:
			language = app_flash_get_config_osd_language();
			if(language &&strcmp(language,"English") == 0)
				sprintf(hintStr,"Update error. error code %d",g_smartcard_upgrade_num);
			else
				sprintf(hintStr,"更新出错，错误代码%d",g_smartcard_upgrade_num);
			app_desai_cas50_show_ca_prompt(hintStr);
			app_play_clear_ca_msg();
			break;
		case MESSAGE_CARD_NEED_UPDATE_TYPE:		/*智能卡需要更新*/
			app_desai_cas50_show_ca_prompt("(DSCA)Updated smart card. please wait!");
			break;
		case MESSAGE_IPPV_OUTTIME_TYPE:		/*IPPV节目过期*/
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
