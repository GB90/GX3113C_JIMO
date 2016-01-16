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
        app_porting_ca_os_sleep(50);//�Ƽ�ֵ��100ms,�ɵ�
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

//��ʼ��
uint8_t app_kp_cas_api_init(void)
{
    handle_t	handle;
	app_porting_ca_flash_init(KPCA_FLASH_SIZE+EMail_SIZE);//flash�ռ��ʼ��
	app_porting_ca_demux_init();//��ʼ��CA������
	app_porting_ca_descramble_init();
	app_kp_cas_rolling_message_init();
    app_kp_cas_api_init_email_data();//email���ݳ�ʼ��
	app_porting_ca_create_filter_task();//ca����������
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
*  ��ȡ��Ӧ����֮ǰ����ʼ�����ݹ���
(һ�㴴����Ӧ����֮ǰ��create�е���)��
* ���ʼ���ʼ�����Ȩ��
*/
int32_t app_kp_cas_api_init_data(dvb_ca_data_type_t date_type)
{
	int32_t dvb_ca_flag =  app_flash_get_config_dvb_ca_flag();
	if (dvb_ca_flag != DVB_CA_TYPE_KP)
		return FALSE;

	switch(date_type)
		{
			case DVB_CA_BASE_INFO:  /*������Ϣ*/
			    return app_kp_cas_api_init_baseinfo_data();
			case DVB_CA_OPERATOR_INFO: /*��Ӫ����Ϣ*/
				break;
			case DVB_CA_EMAIL_INFO: /*�ʼ�*/
				return app_kp_cas_api_init_email_data();
			case DVB_CA_ENTITLE_INFO: /*��Ȩ*/		
                return app_kp_cas_api_init_entitle_data();
			case DVB_CA_DETITLE_INFO: /*����Ȩ*/	
				break;					
			case DVB_CA_CARD_UPDATE_INFO: /*������*/
				break;
			case DVB_CA_FETURE_INFO: /*����ֵ*/
				break;
			case DVB_CA_IPPV_POP_INFO: /*IPPV�������Ϣ*/
                //return app_mg_cas_api_init_ppv_data();
				break;
			case DVB_CA_IPPT_POP_INFO: /*IPPT�������Ϣ*/
				break;
			case DVB_CA_IPPV_PROG_INFO: /*IPPV�ѹ����Ŀ��Ϣ*/				
				break;
			case DVB_CA_IPPV_SLOT_INFO: /*IPPVǮ����Ϣ*/
				return app_kp_cas_api_init_ippv_slot_data();
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
                return app_kp_cas_api_init_worktime_data();
			case DVB_CA_CURTAIN_INFO: /*������Ϣ*/
				break;
			case DVB_CA_PAIRED_INFO: /*�����Ϣ*/
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
			case DVB_CA_BASE_INFO:  /*������Ϣ*/
                  return app_kp_cas_api_get_baseinfo_data(data);							
			case DVB_CA_OPERATOR_INFO: /*��Ӫ����Ϣ*/	
				break;
			case DVB_CA_EMAIL_INFO: /*�ʼ�*/
                return app_kp_cas_api_get_email_data(data); 
			case DVB_CA_ENTITLE_INFO: /*��Ȩ*/
                return app_kp_cas_api_get_entitle_data(data); 
			case DVB_CA_DETITLE_INFO: /*����Ȩ*/
				break;									
			case DVB_CA_CARD_UPDATE_INFO: /*������*/
				break;
			case DVB_CA_FETURE_INFO: /*����ֵ*/
				break;			
			case DVB_CA_IPPV_POP_INFO: /*IPPV�������Ϣ*/
                //return app_mg_cas_api_get_ppv_data(data);
			case DVB_CA_IPPT_POP_INFO: /*IPPT�������Ϣ*/
				break;
			case DVB_CA_IPPV_PROG_INFO: /*IPPV�ѹ����Ŀ��Ϣ*/
				break;
			case DVB_CA_IPPV_SLOT_INFO: /*IPPVǮ����Ϣ*/	
                return app_kp_cas_api_get_ippv_slot_data(data);
//			case DVB_CA_CHILD_CARD_INFO: /*�ӿ���Ϣ*/
//				break;
			case DVB_CA_MOTHER_CARD_INFO: /*ĸ����Ϣ*/
				break;
//				return app_mg_cas_api_get_mother_child_data(data);				
			case DVB_CA_CHANGE_PIN_INFO: /*�޸�������Ϣ*/
				break;
			case DVB_CA_PROGRESS_INFO: /*���ܿ�����������Ϣ*/
				break;
			case DVB_CA_RATING_INFO: /*���˼�����Ϣ*/
                return app_kp_cas_api_get_watchlevel_data(data);			
			case DVB_CA_WORK_TIME_INFO: /*����ʱ����Ϣ*/
                return app_kp_cas_api_get_worktime_data(data);				
			case DVB_CA_CURTAIN_INFO: /*������Ϣ*/
				break;
			case DVB_CA_PAIRED_INFO: /*�����Ϣ*/
				break;							
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
uint8_t app_kp_cas_api_update_data(ca_get_date_t *data)
{
	int32_t dvb_ca_flag =  app_flash_get_config_dvb_ca_flag();

	if (dvb_ca_flag != DVB_CA_TYPE_KP)
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

uint8_t app_kp_cas_api_get_count(ca_get_count_t *data)
{
	int32_t dvb_ca_flag =  app_flash_get_config_dvb_ca_flag();
	if (dvb_ca_flag != DVB_CA_TYPE_KP)
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
			return app_kp_cas_api_get_email_count(data);
		case DVB_CA_ENTITLE_INFO: /*��Ȩ*/
          	return app_kp_cas_api_get_entitle_count(data);
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
//				return app_mg_cas_api_get_features_count(data);				
		case DVB_CA_IPPV_POP_INFO: /*IPPV�������Ϣ*/
			break;
		case DVB_CA_IPPT_POP_INFO: /*IPPT�������Ϣ*/
			break;
		case DVB_CA_IPPV_PROG_INFO: /*IPPV�ѹ����Ŀ��Ϣ*/				
			break;
		case DVB_CA_IPPV_SLOT_INFO: /*IPPVǮ����Ϣ*/
            return app_kp_cas_api_get_ippv_slot_count(data);
//			case DVB_CA_CHILD_CARD_INFO: /*�ӿ���Ϣ*/
//				break;
		case DVB_CA_MOTHER_CARD_INFO: /*ĸ����Ϣ*/
			break;
//				return app_mg_cas_api_get_mother_child_count(data);				
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

bool app_kp_cas_api_delete(ca_delete_data_t *data)
{
	int32_t dvb_ca_flag =  app_flash_get_config_dvb_ca_flag();
	if (dvb_ca_flag != DVB_CA_TYPE_KP)
		return FALSE;


	if (NULL == data)
		return FALSE;
	
	switch(data->date_type)
	{
		case DVB_CA_EMAIL_INFO: /*�ʼ�*/
			return app_kp_cas_api_delete_email(data);
		case DVB_CA_DETITLE_INFO: /*����Ȩ*/
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
		case DVB_CA_EMAIL_INFO: /*�ʼ�*/		
			return app_kp_cas_api_delete_all_email(data);
		case DVB_CA_DETITLE_INFO: /*����Ȩ*/	
			break;
		default:
			break;	
	}

	
	return 0;		
}


//�¼�����
//ca�¼�����
int32_t app_kp_cas_gxmsg_ca_on_event_exec(GxMessage * msg)
{
	switch(msg->msg_id)
	{
		case GXMSG_CA_ON_EVENT:
    	{
    		GxMsgProperty0_OnEvent* event;
    		event = GxBus_GetMsgPropertyPtr(msg, GxMsgProperty0_OnEvent);
    		switch (event->key) {


					case DVB_CA_BASE_INFO:		       /*������Ϣ*/
						break;
					case DVB_CA_OPERATOR_INFO:		/*��Ӫ����Ϣ*/	
						break;
					case DVB_CA_EMAIL_INFO:		/*�ʼ�ͼ����ʾ*/
						app_kp_cas_gxmsg_ca_on_event_email(event);
						break;
					case DVB_CA_ENTITLE_INFO: /*��Ȩ��ʾ��*/
                        printf("event,key: DVB_CA_ENTITLE_INFO\n");
                        //app_mg_cas_gxmsg_ca_on_event_entitle(event);
						break;
					case DVB_CA_DETITLE_INFO: /*����Ȩ*/							
						break;
					case DVB_CA_EMERGENCY_INFO:           /*Ӧ���㲥*/
						break;
					case DVB_CA_ROLLING_INFO: /*OSD������Ϣ*/
                        printf("event,key: DVB_CA_ROLLING_INFO\n");
                        app_kp_cas_gxmsg_ca_on_event_msg(event);
						break;
					case DVB_CA_FINGER_INFO:                  /*ָ��*/
                        printf("event: DVB_KP_FINGER_INFO\n");
                        app_kp_cas_gxmsg_ca_on_event_finger(event);
						break;
					case DVB_CA_CARD_UPDATE_INFO:       /*������*/
						break;
					case DVB_CA_FETURE_INFO:                  /*����ֵ*/
						break;
					case DVB_CA_IPPV_POP_INFO:  /*IPPV�������Ϣ*/
                        //printf("event: DVB_CA_IPPV_POP_INFO\n");
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

uint8_t app_kp_cas_api_is_ca_pop_msg_type(uint8_t type)
{
	if ((type >= MSG_POP_COMMON_END+CA_KPCA_MESSAGE_SUCCESS+1)&&(type <=MSG_POP_COMMON_END+CA_KPCA_MESSAGE_FREEZE+1))
		return TRUE;
	
	return FALSE;
}

//��Ϣ��ʾ��
int32_t app_kp_cas_api_pop_msg_exec(uint8_t type)
{
	switch(type-MSG_POP_COMMON_END-1)
	{
		case CA_KPCA_MESSAGE_SUCCESS: /* ȡ����ǰ����ʾ */
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

//200ms��ʱ���ص�
int32_t app_kp_cas_api_osd_exec(void)
{
	app_kp_cas_rolling_message_exec();//������Ϣ����
	app_kp_cas_email_flag_exec();// �ʼ�ͼ����ʾ�����ؿ���
	app_kp_cas_finger_exec();//ָ����Ϣ��ʾ����
	
	return 1;
}



APP_CAS_API_ControlBlock_t KpCasPortControlBlock=
{
	.m_cas_init_callback =  app_kp_cas_api_init,//caģ���ʼ��
	.m_cas_close_callback = NULL,
	.m_cas_api_card_in_callback = NULL,//���ܿ�����
	.m_cas_api_card_out_callback = NULL,//���ܿ��Ƴ�
	.m_cas_api_release_ecm_filter_callback= app_kp_cas_api_release_ecm_filter,
	.m_cas_api_start_ecm_filter_callback = app_kp_cas_api_start_ecm_filter,//ecm������
	.m_cas_api_release_emm_filter_callback = app_kp_cas_api_release_emm_filter,
	.m_cas_api_start_emm_filter_callback = app_kp_cas_api_start_emm_filter,//emm����������
	.m_cas_api_check_cat_ca_descriptor_valid_callback= app_kp_cas_api_check_cat_ca_descriptor_valid,//ca ģ��cat��������У��
	.m_cas_api_check_pmt_ca_descriptor_valid_callback= app_kp_cas_api_check_pmt_ca_descriptor_valid,//caģ��pmt��������У��
	.m_cas_api_is_ca_pop_msg_type_callback= app_kp_cas_api_is_ca_pop_msg_type,//ca��Ϣ�ж�
	.m_cas_api_pop_msg_exec_callback= app_kp_cas_api_pop_msg_exec,//����ca��Ϣ��
	.m_cas_api_osd_exec_callback= app_kp_cas_api_osd_exec,//200ms��ʱ������
	.m_cas_api_gxmsg_ca_on_event_exec_callback= app_kp_cas_gxmsg_ca_on_event_exec, 	//ca�¼�����							
    /********************************************/
    .m_cas_api_init_data_callback = app_kp_cas_api_init_data,//��ȡ����ǰ�ĳ�ʼ������
	.m_cas_api_get_data_callback = app_kp_cas_api_get_data,//��ȡ���ݽӿ�
	.m_cas_api_get_count_callback = app_kp_cas_api_get_count,//��ȡ��������ֵ
	.m_cas_api_delete_callback = app_kp_cas_api_delete,//ɾ�������ʼ�����
	.m_cas_api_delete_all_callback = app_kp_cas_api_delete_all,//ɾ�������ʼ�
	.m_cas_api_buy_ipp_callback = NULL,//iptv�ӿ� (�������ܿ�)
	.m_cas_api_get_lock_status_callback = NULL,//Ӧ���㲥״̬
	.m_cas_api_feed_mother_child_card_callback = NULL,
	.m_cas_api_change_pin_callback = app_kp_cas_api_change_pin,
	.m_cas_api_verify_pin_callback = NULL,
	.m_cas_api_change_rating_callback = app_kp_cas_api_change_watchlevel,
	.m_cas_api_change_worktime_callback= app_kp_cas_api_change_worktime,
};





