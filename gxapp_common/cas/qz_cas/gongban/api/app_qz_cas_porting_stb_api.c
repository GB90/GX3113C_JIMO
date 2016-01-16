/*****************************************************************************
* 						   CONFIDENTIAL
*        Hangzhou GuoXin Science and Technology Co., Ltd.
*                      (C)2014, All right reserved
******************************************************************************

******************************************************************************
* File Name :	app_mg_cas_porting_stb_api.c
* Author    : 	zhouhm
* Project   :	goxceed dvbc
* Type      :
******************************************************************************
* Purpose   :	
******************************************************************************
* Release History:
VERSION	Date			  AUTHOR         Description
1.0  	2014.04.11		  zhouhm 	 			creation
*****************************************************************************/
#include <gxtype.h>
#include "gxapp_sys_config.h"
#include "gxmsg.h"
#include "ca_process.h"
#include "app_common_play.h"
#include "app_common_table_pmt.h"
#include "app_common_porting_stb_api.h"
#include "app_common_porting_ca_flash.h"
#include "app_common_porting_ca_demux.h"
#include "app_common_porting_ca_descramble.h"
#include "app_common_porting_ca_smart.h"
#include "app_qz_cas_api_demux.h"
#include "app_qz_cas_api_smart.h"
#include "app_qz_cas_api_email.h"
#include "app_qz_cas_api_rolling_message.h"
#include "app_qz_cas_api_baseinfo.h"

extern 	void app_qz_cas_hide_ca_prompt(void);
extern void app_qz_cas_show_ca_prompt(char* context);
extern void QZCA_InitAreaCode();
int32_t qzcas_get_config_hs_mode(void)
{
	int32_t config = 0;
	GxBus_ConfigGetInt("QZHostSlaveMode", &config, HOST_MODE);
	return config;
}

int32_t qzcas_set_config_hs_mode(int32_t config)
{
	GxBus_ConfigSetInt("QZHostSlaveMode",config);
	return 0;
}
//��ʼ��
uint8_t app_qz_cas_api_init(void)
{
    printf("==========app_qz_cas_api_init\n");
	app_porting_ca_flash_init(EMail_SIZE);//flash�ռ��ʼ��
	app_porting_ca_demux_init();//��ʼ��CA������
	app_porting_ca_descramble_init();
    app_qz_cas_rolling_message_init();
    app_qz_cas_api_init_email_data();//email���ݳ�ʼ��
	app_porting_ca_create_filter_task();//ca����������
	QZ_Init_CAS(0);
    QZCA_InitAreaCode();
	app_porting_ca_smart_create_task();//���ܿ�״̬�����
	return TRUE;
}

/*
*  ��ȡ��Ӧ����֮ǰ����ʼ�����ݹ���
(һ�㴴����Ӧ����֮ǰ��create�е���)��
* ���ʼ���ʼ�����Ȩ��
*/
int32_t app_qz_cas_api_init_data(dvb_ca_data_type_t date_type)
{
	int32_t dvb_ca_flag =  app_flash_get_config_dvb_ca_flag();
	if (dvb_ca_flag != DVB_CA_TYPE_QZ)
		return FALSE;

	switch(date_type)
		{
			case DVB_CA_BASE_INFO:  /*������Ϣ*/
                break;
			case DVB_CA_OPERATOR_INFO: /*��Ӫ����Ϣ*/
				break;
			case DVB_CA_EMAIL_INFO: /*�ʼ�*/
				return app_qz_cas_api_init_email_data();
			case DVB_CA_ENTITLE_INFO: /*��Ȩ*/		
				break;
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

	
	return FALSE;		
}

char * app_qz_cas_api_get_data(ca_get_date_t *data)
{
	int32_t dvb_ca_flag =  app_flash_get_config_dvb_ca_flag();
	if (dvb_ca_flag != DVB_CA_TYPE_QZ)
		return NULL;


	if (NULL == data)
		return NULL;	

	switch(data->date_type)
	{
		case DVB_CA_BASE_INFO:  /*������Ϣ*/
		    return app_qz_cas_api_get_baseinfo_data(data);	
		    break;
		case DVB_CA_OPERATOR_INFO: /*��Ӫ����Ϣ*/	
			break;
		case DVB_CA_EMAIL_INFO: /*�ʼ�*/
            return app_qz_cas_api_get_email_data(data); 
            break;
		case DVB_CA_ENTITLE_INFO: /*��Ȩ*/
			break;
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

	
	return NULL;	
}

/*
*  ����CA����
(CA���ݷ������ʱ����)��
* ��ɾ���ʼ�������Ȩ��CA֪ͨ��Ȩ��Ϣ�仯��
*/
uint8_t app_qz_cas_api_update_data(ca_get_date_t *data)
{
	int32_t dvb_ca_flag =  app_flash_get_config_dvb_ca_flag();

	if (dvb_ca_flag != DVB_CA_TYPE_QZ)
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

uint8_t app_qz_cas_api_get_count(ca_get_count_t *data)
{
	int32_t dvb_ca_flag =  app_flash_get_config_dvb_ca_flag();
	if (dvb_ca_flag != DVB_CA_TYPE_QZ)
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
			return app_qz_cas_api_get_email_count(data);
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

bool app_qz_cas_api_delete(ca_delete_data_t *data)
{
	int32_t dvb_ca_flag =  app_flash_get_config_dvb_ca_flag();
	if (dvb_ca_flag != DVB_CA_TYPE_QZ)
		return FALSE;


	if (NULL == data)
		return FALSE;
	
	switch(data->date_type)
	{
		case DVB_CA_EMAIL_INFO: /*�ʼ�*/
			return app_qz_cas_api_delete_email(data);	
			break;
		default:
			break;	
	}
	
	return 0;	
}

bool app_qz_cas_api_delete_all(ca_delete_data_t *data)
{
	int32_t dvb_ca_flag =  app_flash_get_config_dvb_ca_flag();
	if (dvb_ca_flag != DVB_CA_TYPE_QZ)
		return FALSE;


	if (NULL == data)
		return FALSE;
	

	switch(data->date_type)
	{
		case DVB_CA_EMAIL_INFO: /*�ʼ�*/		
			return app_qz_cas_api_delete_all_email(data);
			break;
		default:
			break;	
	}

	
	return 0;		
}


//�¼�����
//ca�¼�����
int32_t app_qz_cas_gxmsg_ca_on_event_exec(GxMessage * msg)
{
	switch(msg->msg_id)
	{
		case GXMSG_CA_ON_EVENT:
    	{
    		GxMsgProperty0_OnEvent* event;
    		event = GxBus_GetMsgPropertyPtr(msg, GxMsgProperty0_OnEvent);
    		//printf("app_qz_cas_gxmsg_ca_on_event_exec GXMSG_CA_ON_EVENT event->key=%d\n",event->key);
    		switch (event->key) {
					case DVB_CA_BASE_INFO:		       /*������Ϣ*/
						break;
					case DVB_CA_OPERATOR_INFO:		/*��Ӫ����Ϣ*/	
						break;
					case DVB_CA_EMAIL_INFO:		/*�ʼ�ͼ����ʾ*/
                        {
                        //printf("1111,email\n");
						app_qz_cas_gxmsg_ca_on_event_email(event);
                        //printf("1111#\n");
					    }
						break;
					case DVB_CA_ENTITLE_INFO: /*��Ȩ��ʾ��*/
						break;
					case DVB_CA_DETITLE_INFO: /*����Ȩ*/							
						break;
					case DVB_CA_EMERGENCY_INFO:           /*Ӧ���㲥*/
						break;
					case DVB_CA_ROLLING_INFO: /*OSD������Ϣ*/
						{
                          //printf("2222,rolling\n");
                          app_qz_cas_gxmsg_ca_on_event_msg(event);
                          //printf("2222#\n");
						}
						break;
					case DVB_CA_FINGER_INFO:                  /*ָ��*/
						break;
					case DVB_CA_CARD_UPDATE_INFO:       /*������*/
						break;
					case DVB_CA_FETURE_INFO:                  /*����ֵ*/
						break;
					case DVB_CA_IPPV_POP_INFO:  /*IPPV�������Ϣ*/
						break;
					case DVB_CA_IPPT_POP_INFO:                /*IPPT�������Ϣ*/
						break;
					case DVB_CA_IPPV_PROG_INFO:            /*IPPV�ѹ����Ŀ��Ϣ*/
						break;
					case DVB_CA_IPPV_SLOT_INFO:             /*IPPVǮ����Ϣ*/
						break;
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

uint8_t app_qz_cas_api_is_ca_pop_msg_type(uint8_t type)
{
	if ((type >= MSG_POP_COMMON_END+CA_QZ_SUCCESS+1)&&(type <=MSG_POP_COMMON_END+CA_QZ_ERR_CARD_NOTFOUND+1))
		return TRUE;
	
	return FALSE;
}

//��Ϣ��ʾ��
int32_t app_qz_cas_api_pop_msg_exec(uint8_t type)
{
	switch(type-MSG_POP_COMMON_END-1)
	{
		case CA_QZ_SUCCESS: /* ȡ����ǰ����ʾ */
			app_qz_cas_hide_ca_prompt();
			break;
        case CA_QZ_ERR_NO_MEMORY:
            app_qz_cas_show_ca_prompt("Err Scard No Memory");
			break;
        case CA_QZ_ERR_ILLEGAL_CARD: /* ���ܿ�ͨ�Ŵ���*/
			app_qz_cas_show_ca_prompt("Err Illegal Card");
			break;
        case CA_QZ_ERR_AUTH_OVERDUE: /* ���ܿ���Ȩ����*/
			app_qz_cas_show_ca_prompt("Err Auth Overdue");
			break;
        case CA_QZ_ERR_AUTH_NOTSTART: /*δ����Ȩ��ʼ����*/
			app_qz_cas_show_ca_prompt("Err Auth Overdue");
			break;
        case CA_QZ_ERR_CARD_NOTMATCH: /* ���ܿ���ƥ�� */
			app_qz_cas_show_ca_prompt("Err Card Not Match");
			break;
         case CA_QZ_ERR_CARD_NOTINIT: /*���ܿ�δ��ʼ��*/
			app_qz_cas_show_ca_prompt("Err Card Not Init");
			break;
        case CA_QZ_ERR_PPV_OUTDATE: /*PPV����*/
			app_qz_cas_show_ca_prompt("Err PPV Out of Date");
			break;
        case CA_QZ_ERR_PINERR_CARD_PAIRED: /*���ܿ�û������Ϊ�������*/
			app_qz_cas_show_ca_prompt("Err Pin Authen Card Paired");
			break;
        case CA_QZ_ERR_ECM_NOT_AUTHORIZED:/*Ƶ��δ��Ȩ*/
            app_qz_cas_show_ca_prompt("Err Not Authored");
            break;
         case CA_QZ_ERR_ZONE_CODE:/*�����޲�*/
            app_qz_cas_show_ca_prompt("Err Limit Area");
            break;
         case CA_QZ_ERR_LIMIT_CARD:/*���޲�*/
            app_qz_cas_show_ca_prompt("Err Limit Card");
            break;
         case CA_QZ_ERR_NOT_CHILD_MODE:/*���ܿ�û������Ϊ�ӿ�ģʽ*/
            app_qz_cas_show_ca_prompt("Err Not Child Mode");
            break;
		case CA_QZ_ERR_NO_CHILD_AUTH: /* �뼤���ӿ� */
			app_qz_cas_show_ca_prompt("Err Scard Not Act");
			break;
        case CA_QZ_ERR_CHILD_NOT_AUTHORIZED:/*�ӿ�δ��Ȩ*/
            app_qz_cas_show_ca_prompt("Err Scard Not Authored");
            break;
        case CA_QZ_ERR_CHILD_AUTH_TIMEOUT:/*�����ӿ���Ȩ��ʱ*/
            app_qz_cas_show_ca_prompt("Err Scard Auth Timeout");
            break;
        case CA_QZ_ERR_CHILD_AUTH_EXPIRED:/*�ӿ���Ȩ����*/
            app_qz_cas_show_ca_prompt("Err Scard Auth Timeout");
            break;
        case CA_QZ_ERR_CARD_NOTFOUND: /* ��������ܿ� */
			app_qz_cas_show_ca_prompt("Error Card Not Found");
			break;
		default:
			break;
		}
	return 0;
}

//200ms��ʱ���ص�
int32_t app_qz_cas_api_osd_exec(void)
{
	app_qz_cas_rolling_message_exec();//������Ϣ����
	app_qz_cas_email_flag_exec();// �ʼ�ͼ����ʾ�����ؿ���
	return 1;
}

APP_CAS_API_ControlBlock_t QzCasPortControlBlock=
{
	.m_cas_init_callback =  app_qz_cas_api_init,//caģ���ʼ��
	.m_cas_close_callback = NULL,
	.m_cas_api_card_in_callback = app_qz_cas_api_card_in,//���ܿ�����
	.m_cas_api_card_out_callback = app_qz_cas_api_card_out,//���ܿ��Ƴ�
	.m_cas_api_release_ecm_filter_callback= app_qz_cas_api_release_ecm_filter,
	.m_cas_api_start_ecm_filter_callback = app_qz_cas_api_start_ecm_filter,//ecm������
	.m_cas_api_release_emm_filter_callback = app_qz_cas_api_release_emm_filter,
	.m_cas_api_start_emm_filter_callback = app_qz_cas_api_start_emm_filter,//emm����������
	.m_cas_api_check_cat_ca_descriptor_valid_callback= app_qz_cas_api_check_cat_ca_descriptor_valid,//ca ģ��cat��������У��
	.m_cas_api_check_pmt_ca_descriptor_valid_callback= app_qz_cas_api_check_pmt_ca_descriptor_valid,//caģ��pmt��������У��
	.m_cas_api_is_ca_pop_msg_type_callback= app_qz_cas_api_is_ca_pop_msg_type,//ca��Ϣ�ж�
	.m_cas_api_pop_msg_exec_callback= app_qz_cas_api_pop_msg_exec,//����ca��Ϣ��
	.m_cas_api_osd_exec_callback= app_qz_cas_api_osd_exec,//200ms��ʱ������
	.m_cas_api_gxmsg_ca_on_event_exec_callback= app_qz_cas_gxmsg_ca_on_event_exec, 	//ca�¼�����							
    /********************************************/
    .m_cas_api_init_data_callback = app_qz_cas_api_init_data,//��ȡ����ǰ�ĳ�ʼ������
	.m_cas_api_get_data_callback = app_qz_cas_api_get_data,//��ȡ���ݽӿ�
	.m_cas_api_get_count_callback = app_qz_cas_api_get_count,//��ȡ��������ֵ
	.m_cas_api_delete_callback = app_qz_cas_api_delete,//ɾ�������ʼ�����
	.m_cas_api_delete_all_callback = app_qz_cas_api_delete_all,//ɾ�������ʼ�
	.m_cas_api_buy_ipp_callback = NULL,//iptv�ӿ� (�������ܿ�)
	.m_cas_api_get_lock_status_callback = NULL,//Ӧ���㲥״̬
	.m_cas_api_feed_mother_child_card_callback = NULL,
	.m_cas_api_change_pin_callback = NULL,
	.m_cas_api_verify_pin_callback = NULL,
	.m_cas_api_change_rating_callback = NULL,
	.m_cas_api_change_worktime_callback= NULL,
};




