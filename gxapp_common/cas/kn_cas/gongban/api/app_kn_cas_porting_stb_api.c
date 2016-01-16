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
* Purpose   :	Ä£¿éÍ·ÎÄ¼þ
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
#include "app_kn_cas_api_smart.h"
#include "app_kn_cas_api_demux.h"
#include "cas_hcn_Public.h"
#include "app_common_porting_ca_smart.h"


#include "app_kn_cas_api_demux.h"
#include "app_kn_cas_api_smart.h"
#include "app_kn_cas_api_entitle.h"
#include "app_kn_cas_api_email.h"
#include "app_kn_cas_api_baseinfo.h"
#include "app_kn_cas_api_lock_service.h"
#include "app_kn_cas_api_rolling_message.h"
#include "app_kn_cas_api_finger.h"
extern void app_kn_cas_show_ca_prompt(char* context);
extern void app_kn_cas_hide_ca_prompt(void);
extern void app_kn_cas_show_tip_prompt(char* context);
extern void app_kn_cas_hide_tip_prompt(void);

extern uint8_t bat_recieve_flag ;
extern uint8_t zone_flag;
extern void channel_filter_init(void);
uint8_t app_kn_cas_api_init(void)
{

	app_porting_ca_flash_init(64*1024);
	app_porting_ca_demux_init();
	channel_filter_init();
	app_porting_ca_descramble_init();
    app_kn_cas_rolling_message_init();
	app_porting_ca_create_filter_task();
	if (HCN_RET_SUCCESS!= hcncas_init())
	{
		CA_FAIL(" KNCASTB_Init  failed \n");
		return FALSE;
	}
	
	app_porting_ca_smart_create_task();	
    //kn_cas_rolling_test();
	return TRUE;
	
}

/*
* ÒÔÏÂ½Ó¿ÚCA²Ëµ¥µ÷ÓÃ
*/


/*
*  »ñÈ¡¶ÔÓ¦Êý¾ÝÖ®Ç°£¬³õÊ¼»¯Êý¾Ý¹ÜÀí
(Ò»°ã´´½¨¶ÔÓ¦´°ÌåÖ®Ç°»òcreateÖÐµ÷ÓÃ)¡£
* Èç³õÊ¼»¯ÓÊ¼þ¡¢ÊÚÈ¨µÈ
*/
int32_t app_kn_cas_api_init_data(dvb_ca_data_type_t date_type)
{
	int32_t dvb_ca_flag =  app_flash_get_config_dvb_ca_flag();
	if (dvb_ca_flag != DVB_CA_TYPE_KN)
		return FALSE;

	switch(date_type)
		{
			case DVB_CA_BASE_INFO:	/*»ù±\u0178ÐÅÏ¢*/
				return app_kn_cas_api_init_baseinfo_data();
			case DVB_CA_EMAIL_INFO: /*ÓÊ\u0152þ*/	
				return app_kn_cas_api_init_email_data();							
			case DVB_CA_ENTITLE_INFO: /*ÊÚÈ\u0161*/ 
				return app_kn_cas_api_init_entitle_data();
			default:
				break;	
		}


	
	return FALSE;		
}

char* app_kn_cas_api_get_data(ca_get_date_t *data)
{
	int32_t dvb_ca_flag =  app_flash_get_config_dvb_ca_flag();
	if (dvb_ca_flag != DVB_CA_TYPE_KN)
		return NULL;


	if (NULL == data)
		return NULL;	

	switch(data->date_type)
		{
			case DVB_CA_BASE_INFO:	/*»ù±\u0178ÐÅÏ¢*/
				return app_kn_cas_api_get_baseinfo_data(data);
			case DVB_CA_EMAIL_INFO: /*ÓÊ\u0152þ*/	
				return app_kn_cas_api_get_email_data(data); 						
			case DVB_CA_ENTITLE_INFO: /*ÊÚÈ\u0161*/
				return app_kn_cas_api_get_entitle_data(data);
			default:
				break;	
		}
	
	return NULL;	
}


bool app_kn_cas_api_delete(ca_delete_data_t *data)
{
	int32_t dvb_ca_flag =  app_flash_get_config_dvb_ca_flag();
	if (dvb_ca_flag != DVB_CA_TYPE_KN)
		return FALSE;


	if (NULL == data)
		return FALSE;
	
	switch(data->date_type)
		{
			case DVB_CA_EMAIL_INFO: /*ÓÊ¼þ*/	
				return app_kn_cas_api_delete_email(data);
			default:
				break;	
		}
	
	return 0;	
}

bool app_kn_cas_api_delete_all(ca_delete_data_t *data)
{
	int32_t dvb_ca_flag =  app_flash_get_config_dvb_ca_flag();
	if (dvb_ca_flag != DVB_CA_TYPE_KN)
		return FALSE;


	if (NULL == data)
		return FALSE;
	

	switch(data->date_type)
		{
			case DVB_CA_EMAIL_INFO: /*ÓÊ¼þ*/	
				return app_kn_cas_api_delete_all_email(data);
			default:
				break;	
		}

	
	return 0;		
}


uint8_t app_kn_cas_api_get_count(ca_get_count_t *data)
{
	int32_t dvb_ca_flag =  app_flash_get_config_dvb_ca_flag();
	if (dvb_ca_flag != DVB_CA_TYPE_KN)
		return 0;

	if (NULL == data)
		return 0;
	

	switch(data->date_type)
		{
			case DVB_CA_EMAIL_INFO: /*ÓÊ\u0152þ*/
				return app_kn_cas_api_get_email_count(data);							
			case DVB_CA_ENTITLE_INFO: /*ÊÚÈ\u0161*/
				return app_kn_cas_api_get_entitle_count(data);
			default:
				break;	
		}

	
	return 0;		
}

int32_t app_kn_cas_gxmsg_ca_on_event_exec(GxMessage * msg)
{
	switch(msg->msg_id)
		{
			case GXMSG_CA_ON_EVENT:
					{
						GxMsgProperty0_OnEvent* event;
						event = GxBus_GetMsgPropertyPtr(msg, GxMsgProperty0_OnEvent);
						//printf("app_KN_cas_gxmsg_ca_on_event_exec GXMSG_CA_ON_EVENT\n");
						switch (event->key) {
								case DVB_CA_BASE_INFO:		       /*»ù±¾ÐÅÏ¢*/
									break;
								case DVB_CA_OPERATOR_INFO:		/*ÔËÓªÉÌÐÅÏ¢*/	
									break;
								case DVB_CA_EMAIL_INFO:		/*ÓÊ¼þ*/
								    {
									//app_by_cas_gxmsg_ca_on_event_email(event);											
									}
									break;
								case DVB_CA_ENTITLE_INFO :			/*ÊÚÈ¨*/
									break;
								case DVB_CA_DETITLE_INFO:                /*·´ÊÚÈ¨*/
									break;
								case DVB_CA_EMERGENCY_INFO:           /*Ó¦¼±¹ã²¥*/
									break;
								case DVB_CA_ROLLING_INFO:               /*OSD¹ö¶¯ÏûÏ¢*/
									app_kn_cas_gxmsg_ca_on_event_rolling(event);
									break;
								case DVB_CA_FINGER_INFO:                  /*Ö¸ÎÆ*/
									app_kn_cas_gxmsg_ca_on_event_finger(event);
									break;
								case DVB_CA_CARD_UPDATE_INFO:       /*¿¨Éý¼¶*/
									break;
								case DVB_CA_FETURE_INFO:                  /*ÌØÕ÷Öµ*/
									break;
								case DVB_CA_IPPV_POP_INFO:                /*IPPV¹ºÂò¿òÐÅÏ¢*/
									break;
								case DVB_CA_IPPT_POP_INFO:                /*IPPT¹ºÂò¿òÐÅÏ¢*/
									break;
								case DVB_CA_IPPV_PROG_INFO:            /*IPPVÒÑ¹ºÂò½ÚÄ¿ÐÅÏ¢*/
									break;
								case DVB_CA_IPPV_SLOT_INFO:             /*IPPVÇ®°üÐÅÏ¢*/
									break;
//								case DVB_CA_CHILD_CARD_INFO:            /*×Ó¿¨ÐÅÏ¢*/
//									break;
								case DVB_CA_MOTHER_CARD_INFO:            /*Ä¸¿¨ÐÅÏ¢*/
									break;
								case DVB_CA_CHANGE_PIN_INFO:             /*ÐÞ¸ÄÃÜÂëÐÅÏ¢*/
									break;
								case DVB_CA_PROGRESS_INFO:             /*ÖÇÄÜ¿¨Éý¼¶½ø¶ÈÐÅÏ¢*/
									break;
								case DVB_CA_RATING_INFO:                /*³ÉÈË¼¶±ðÐÅÏ¢*/
									break;
								case DVB_CA_WORK_TIME_INFO:          /*¹¤×÷Ê±¶ÎÐÅÏ¢*/
									break;
								case DVB_CA_CURTAIN_INFO:          /*´°Á±ÐÅÏ¢*/
									break;
								case DVB_CA_PAIRED_INFO:          /*Åä¶ÔÐÅÏ¢*/
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


uint8_t app_kn_cas_api_is_ca_pop_msg_type(uint8_t type)
{
	if ((type >= DISP_NO_MSG+MSG_POP_COMMON_END+1)&&(type <=MSG_POP_COMMON_END+1+HCNCAS_REFILT_PMT))
		return TRUE;
	return FALSE;
}

int32_t app_kn_cas_api_pop_msg_exec(uint8_t type)
{
	uint8_t remain_day=0;
	char buf[512]={0,};
	switch(type-MSG_POP_COMMON_END-1)
	{
			case DISP_NO_MSG:
				app_kn_cas_hide_ca_prompt();
				app_kn_cas_hide_tip_prompt();
				printf(" È¡Ïûµ±Ç°µÄÏÔÊ¾\n");
				break;
			case DISP_PLS_INSERT_SMC:
                app_kn_cas_show_ca_prompt("ERROR NO CARD");
			    break;
			case DISP_SMC_INSERT:
		//	case DISP_SMC_INIT_SUCCEES:
			//case DISP_SMC_INIT_FAIL:
			//case DISP_SMC_READ_ERROR:
			case DISP_CARD_NOMATCH:
                app_kn_cas_show_ca_prompt("Sc Card Stb Not Pair");
			    break;
			case DISP_MATCH_SUCCESS:
               app_kn_cas_show_ca_prompt("CARD PAIRING OK");
			    break;
			case DISP_MATCH_FAIL:
               app_kn_cas_show_ca_prompt("CARD PAIRING FAILED");
	
				//sprintf(buf,"%s%d","kn ca report code",type-MSG_POP_COMMON_END-1);
				//app_kn_cas_show_ca_prompt(buf);
				break;
			case DISP_SMC_REMOVED: 
				app_kn_cas_show_ca_prompt("ERROR NO CARD");
				printf("ÖÇÄÜ¿¨°Î×ß!\n"); 
				break;
			case DISP_TIMEEXPIRED:
				app_kn_cas_show_ca_prompt("ECM OUTDATE ERROR");
				break;
			case DISP_INSTANCE_CLOSE:
			case DISP_OUT_WORKTIME:
			case DISP_MASK_LIMITED:
				if(zone_flag == 1)
				{
					sprintf(buf,"%s","Zone forbid error!");
					app_kn_cas_show_ca_prompt(buf);
				}
				printf("ÇøÓòÏÞ²¥!  %d\n",zone_flag);
				break;
			case DISP_NO_RIGHT:// 10
				app_kn_cas_show_ca_prompt("EMMECM RET DES ERROR");
				printf("ÖÇÄÜ¿¨Ã»ÓÐÊÚÈ¨!\n");
				break;
			case DISP_OUT_LEVEL:
			case DISP_TYPE_LIMIT:
			case DISP_PPV:
			case DISP_PPC:
				break;
			case DISP_PPV_AWARD_SUCCESS:
			case DISP_PPC_AWARD_SUCCESS:
				app_kn_cas_show_ca_prompt("AUTHORITY SUCCESS!");
				break;
			case DISP_PPC_AWARD_FAIL:
			case DISP_PPV_AWARD_FAIL:
				//sprintf(buf,"%s%d","kn ca report code",type-MSG_POP_COMMON_END-1);
				//app_kn_cas_show_ca_prompt(buf);
                app_kn_cas_show_ca_prompt("AUTHORITY FAILED!");
				break;
			case HCNCAS_STOP_URGE_FEE_NOTIFY:
				app_kn_cas_hide_ca_prompt();
				hcncas_debug("clear notiy fee---\n");
				break;	
			case HCNCAS_DISPLAY_URGE_FEE_NOTIFY:
				app_kn_cas_api_get_remain_day(&remain_day);
                
			#ifdef SUPPORT_YUENAN
				sprintf(buf,"%s%d%s","Display remain ",remain_day," days,Pls renew!");
			#else
                #if (1==LINUX_OTT_SUPPORT)
                {
                //http://bianma.911cha.com/
                char *str_buffer1="\xE6\x82\xA8\xE7\x9A\x84\xE6\x94\xB6\xE8\xA7\x86\xE6\x9C\x9F\xE8\xBF\x98\xE5\x89\xA9\xE4\xBD\x99\0";
                char *str_buffer2="\xE5\xA4\xA9\xEF\xBC\x8C\xE8\xAF\xB7\xE7\xBB\xAD\xE8\xB4\xB9\x21\0";
    			char buff2[16]={0};
    			sprintf((void*)buff2,"%d", remain_day);
    			strcpy(buf,str_buffer1);
    			strcat(buf,buff2);
                strcat(buf,str_buffer2);
                }
                
                #else
				sprintf(buf,"%s%d%s","ÄúµÄÊÕÊÓÆÚ»¹Ê£Óà",remain_day,"Ìì£¬ÇëÐø·Ñ!");
                #endif
			#endif
				app_kn_cas_show_ca_prompt(buf);
                
				hcncas_debug("HCNCAS_DISPLAY_URGE_FEE_NOTIFY »¹Ê£Óà%dÌì\n",remain_day);
				hcncas_debug("ÊÚÈ¨¿ìµ½ÆÚ---1\n");
				break;
			case DISP_REMAIN_DAY:
				//sprintf(buf,"%s%d","kn ca report code",type-MSG_POP_COMMON_END-1);

                sprintf(buf,"Display remain 9 days,Pls renew!");
                app_kn_cas_show_ca_prompt(buf);
				
				hcncas_debug("DISP_REMAIN_DAY\n");
				break;
			case DISP_CANCEL_MATCH://ÒÑÈ¡ÏûÅä¶Ô
			    app_kn_cas_show_ca_prompt("CARD CANCEL PAIRED OK");
				break;
	        case DISP_CANCEL_MATCHFAIL: //È¡ÏûÅä¶ÔÊ§°Ü
	            app_kn_cas_show_ca_prompt("CARD CANCEL PAIRED FAILED");
				break;
	        case DISP_SMC_NOTOPEN: //ÖÇÄÜ¿¨Î´ÆôÓÃ
	            app_kn_cas_show_ca_prompt("CARD NOT WORKING");
				break;
			case DISP_CREDIT_CONUT_FULL:	//µãÊýÒÑÂú
			//case DISP_UNRECOGNISED_CARD: //ÎÞ·¨Ê¶±ðÖÇÄÜ¿¨
			//case DISP_ERRORVERSION_CARD://ÖÇÄÜ¿¨°æ±¾´íÎó
				//sprintf(buf,"%s%d","kn ca report code",type-MSG_POP_COMMON_END-1);
				//app_kn_cas_show_ca_prompt(buf);
				app_kn_cas_show_ca_prompt("CARD CONUT FULL");
				break;
				
			default:
				break;
	}
	return 0;
}






int32_t app_kn_cas_api_osd_exec(void)
{
/*
* ¹ö¶¯ÏûÏ¢ÏÔÊ¾¡¢Òþ²Ø
*/
	app_kn_cas_rolling_message_exec();
	

/*
* ÓÊ¼þÍ¼±êÏÔÊ¾¡¢Òþ²Ø
*/
	app_kn_cas_email_flag_exec();

/*
* Ö¸ÎÆÏÔÊ¾¡¢Òþ²Ø
*/
	app_kn_cas_finger_exec();
	return 0;
}



APP_CAS_API_ControlBlock_t knCasPortControlBlock=
{
	.m_cas_init_callback =  app_kn_cas_api_init,
	.m_cas_close_callback = NULL,
	.m_cas_api_card_in_callback = app_kn_cas_api_card_in,
	.m_cas_api_card_out_callback = app_kn_cas_api_card_out,
	.m_cas_api_release_ecm_filter_callback= app_kn_cas_api_release_ecm_filter,
	.m_cas_api_start_ecm_filter_callback = app_kn_cas_api_start_ecm_filter,
	.m_cas_api_release_emm_filter_callback = app_kn_cas_api_release_emm_filter,
	.m_cas_api_start_emm_filter_callback = app_kn_cas_api_start_emm_filter,
	.m_cas_api_check_cat_ca_descriptor_valid_callback= app_kn_cas_api_check_cat_ca_descriptor_valid,
	.m_cas_api_check_pmt_ca_descriptor_valid_callback= app_kn_cas_api_check_pmt_ca_descriptor_valid,
	.m_cas_api_is_ca_pop_msg_type_callback= app_kn_cas_api_is_ca_pop_msg_type,
	.m_cas_api_pop_msg_exec_callback= app_kn_cas_api_pop_msg_exec,
	.m_cas_api_osd_exec_callback= app_kn_cas_api_osd_exec,
	.m_cas_api_gxmsg_ca_on_event_exec_callback= app_kn_cas_gxmsg_ca_on_event_exec, 								
	.m_cas_api_init_data_callback = app_kn_cas_api_init_data,
	.m_cas_api_get_data_callback = app_kn_cas_api_get_data,
	.m_cas_api_get_count_callback = app_kn_cas_api_get_count,
	.m_cas_api_delete_callback = app_kn_cas_api_delete,
	.m_cas_api_delete_all_callback = app_kn_cas_api_delete_all,
	.m_cas_api_buy_ipp_callback = NULL,
	.m_cas_api_get_lock_status_callback =	app_kn_cas_api_get_lock_status,
	.m_cas_api_feed_mother_child_card_callback = NULL,
	.m_cas_api_change_pin_callback = NULL,
	//.m_cas_api_verify_pin_callback = NULL,
	.m_cas_api_change_rating_callback = NULL,
	.m_cas_api_change_worktime_callback= NULL,
};




