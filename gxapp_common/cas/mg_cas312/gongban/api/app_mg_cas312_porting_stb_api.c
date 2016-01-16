/*****************************************************************************
* 						   CONFIDENTIAL
*        Hangzhou GuoXin Science and Technology Co., Ltd.
*                      (C)2014, All right reserved
******************************************************************************

******************************************************************************
* File Name :	app_mg_cas312_porting_stb_api.c
* Author    : 	wangjian
* Project   :	runde 3113c
* Type      :
******************************************************************************
* Purpose   :	
******************************************************************************
* Release History:
VERSION	Date			  AUTHOR         Description
1.0  	2014.12.16		  zhouhm 	 			creation
2.0  2014.12.16              wangjian         modify for Runde 3113c mgcas3.1.2
*****************************************************************************/
#include <gxtype.h>
#include "gxapp_sys_config.h"
#include "gxmsg.h"
#include "mg312def.h"
#include "app_common_porting_stb_api.h"
#include "app_common_play.h"
#include "app_common_porting_ca_demux.h"
#include "app_common_porting_ca_descramble.h"
#include "app_common_porting_ca_smart.h"
#include "app_common_porting_ca_flash.h"

#include "app_mg_cas312_api_demux.h"
#include "app_mg_cas312_api_smart.h"
#include "app_mg_cas312_api_baseinfo.h"
#include "app_mg_cas312_api_lock_service.h"
#include "app_mg_cas312_api_email.h"
#include "app_mg_cas312_api_event.h"
#include "app_mg_cas312_api_rolling_message.h"
#include "app_mg_cas312_api_ppv.h"


/*Marcos ----------------------------------------------------------*/
#define MGCAS_LIB_STATUS  (MG_LIB_FORMAL)

/*Extern ---------------------------------------------------------*/
extern 	void app_mg_cas312_hide_ca_prompt(void);
extern void app_mg_cas312_show_ca_prompt(char* context);


/*API---------------------------------------------------------------*/
uint8_t app_mg_cas312_api_init(void)
{
	app_porting_ca_flash_init(EMail_SIZE);//flash�ռ��ʼ��
	app_porting_ca_demux_init();//��ʼ��CA������
	app_porting_ca_descramble_init();
	app_mg_cas312_rolling_message_init();
    app_mg_cas312_api_init_email_data();//email���ݳ�ʼ��
	app_porting_ca_create_filter_task();//ca����������
	MG_Init_CAS(MGCAS_LIB_STATUS);
	app_porting_ca_smart_create_task();//���ܿ�״̬�����

	return TRUE;
}

/*
*  ��ȡ��Ӧ����֮ǰ����ʼ�����ݹ���
(һ�㴴����Ӧ����֮ǰ��create�е���)��
* ���ʼ���ʼ�����Ȩ��
*/
int32_t app_mg_cas312_api_init_data(dvb_ca_data_type_t date_type)
{
	int32_t dvb_ca_flag =  app_flash_get_config_dvb_ca_flag();
	if (dvb_ca_flag != DVB_CA_TYPE_MG312)
	{
		return FALSE;
	}

	switch (date_type)
	{
		case DVB_CA_BASE_INFO:  /*������Ϣ*/
		    return app_mg_cas312_api_init_baseinfo_data();
			
		case DVB_CA_EMAIL_INFO: /*�ʼ�*/
			return app_mg_cas312_api_init_email_data();	
			
		case DVB_CA_IPPV_POP_INFO: /*IPPV�������Ϣ*/
            return app_mg_cas312_api_init_ppv_data();
			
		default:
			break;	
	}

	return FALSE;		
}

uint8_t app_mg_cas312_api_buy_ipp(ca_buy_ipp_t *data)
{
    int32_t dvb_ca_flag =  app_flash_get_config_dvb_ca_flag();
	if (dvb_ca_flag != DVB_CA_TYPE_MG312)
	{
		return 0;
	}

	if (NULL == data)
	{
		return 0;	
	}

	switch (data->date_type)
	{
	    case DVB_CA_IPPV_POP_INFO: /*IPPV�������Ϣ*/
	         return app_mg_cas312_api_buy_ppv_data(data);
	    default:
			break;	
	}
    
    return 1;
}

char* app_mg_cas312_api_get_data(ca_get_date_t *data)
{
	int32_t dvb_ca_flag =  app_flash_get_config_dvb_ca_flag();
	if (dvb_ca_flag != DVB_CA_TYPE_MG312)
	{
		return NULL;
	}

	if (NULL == data)
	{
		return NULL;	
	}

	switch (data->date_type)
	{
		case DVB_CA_BASE_INFO:  /*������Ϣ*/
		    return app_mg_cas312_api_get_baseinfo_data(data);	

		case DVB_CA_EMAIL_INFO: /*�ʼ�*/
            return app_mg_cas312_api_get_email_data(data); 
        
		case DVB_CA_IPPV_POP_INFO: /*IPPV�������Ϣ*/
            return app_mg_cas312_api_get_ppv_data(data);
	
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
uint8_t app_mg_cas312_api_update_data(ca_get_date_t *data)
{
	int32_t dvb_ca_flag =  app_flash_get_config_dvb_ca_flag();

	if (dvb_ca_flag != DVB_CA_TYPE_MG312)
	{
		return 0;
	}
	
	if (NULL == data)
	{
		return 0;	
	}

	switch (data->date_type)
	{
		default:
		break;	
	}
	
	return 0;	
}

uint8_t app_mg_cas312_api_get_count(ca_get_count_t *data)
{
	int32_t dvb_ca_flag = app_flash_get_config_dvb_ca_flag();
	
	if (dvb_ca_flag != DVB_CA_TYPE_MG312)
	{
		return 0;
	}

	if (NULL == data)
	{
		return 0;
	}
	
	switch (data->date_type)
	{
		case DVB_CA_EMAIL_INFO: /*�ʼ�*/
			return app_mg_cas312_api_get_email_count(data);
			break;
			
		default:
			break;	
	}
	
	return 0;		
}

bool app_mg_cas312_api_delete(ca_delete_data_t *data)
{
	int32_t dvb_ca_flag = app_flash_get_config_dvb_ca_flag();
	
	if (dvb_ca_flag != DVB_CA_TYPE_MG312)
	{
		return FALSE;
	}

	if (NULL == data)
	{
		return FALSE;
	}
	
	switch (data->date_type)
	{
		case DVB_CA_EMAIL_INFO: /*�ʼ�*/
			return app_mg_cas312_api_delete_email(data);
			break;
			
		default:
			break;	
	}
	
	return 0;	
}

bool app_mg_cas312_api_delete_all(ca_delete_data_t *data)
{
	int32_t dvb_ca_flag =  app_flash_get_config_dvb_ca_flag();
	
	if (dvb_ca_flag != DVB_CA_TYPE_MG312)
	{
		return FALSE;
	}

	if (NULL == data)
	{
		return FALSE;
	}

	switch (data->date_type)
	{
		case DVB_CA_EMAIL_INFO: /*�ʼ�*/		
			return app_mg_cas312_api_delete_all_email(data);
			break;
		
		default:
			break;	
	}

	return 0;		
}

int32_t app_mg_cas312_gxmsg_ca_on_event_exec(GxMessage* msg)
{
	switch (msg->msg_id)
	{
		case GXMSG_CA_ON_EVENT:
    	{
    		GxMsgProperty0_OnEvent *event = NULL;
			
    		event = GxBus_GetMsgPropertyPtr(msg, GxMsgProperty0_OnEvent);
    		switch (event->key) 
			{
				case DVB_CA_EMAIL_INFO:		/*�ʼ�ͼ����ʾ*/
					app_mg_cas312_gxmsg_ca_on_event_email(event);
					break;

				case DVB_CA_ROLLING_INFO: /*OSD������Ϣ*/
                    app_mg_cas312_gxmsg_ca_on_event_msg(event);
					break;
					
				case DVB_CA_FINGER_INFO: /*ָ��*/
                    app_mg_cas312_gxmsg_ca_on_event_finger(event);
					break;

				case DVB_CA_IPPV_POP_INFO:  /*IPPV�������Ϣ*/
					app_mg_cas312_gxmsg_ca_on_event_ippv_logo(event);
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

uint8_t app_mg_cas312_api_is_ca_pop_msg_type(uint8_t type)
{
	if ((type >= (MSG_POP_COMMON_END+CA_MG_SUCCESS+1)) 
		&& (type <= (MSG_POP_COMMON_END+CA_MG_ID_MAX+1)))
	{
		return TRUE;
	}
	
	return FALSE;
}

//��Ϣ��ʾ��
int32_t app_mg_cas312_api_pop_msg_exec(uint8_t type)
{
	switch (type-MSG_POP_COMMON_END-1)
	{
		case CA_MG_SUCCESS: /* ȡ����ǰ����ʾ */
			app_mg_cas312_hide_ca_prompt();
			break;
			
		case CA_MG_ERR_SCARD_NOT_ACT: /* �뼤���ӿ� */
			app_mg_cas312_show_ca_prompt("Err Scard Not Act");
			break;
			
		case CA_MG_ERR_ZONE_CODE: /* ���ܿ���������� */
			app_mg_cas312_show_ca_prompt("Err Zone Code");
			break;
			
		case CA_MG_ERR_NO_MONEY: /* ���ܿ����� */
			app_mg_cas312_show_ca_prompt("Err No Money");
			break;
			
		case CA_MG_ERR_NOT_AUTHORED: /* ����Ŀδ��Ȩ */
			app_mg_cas312_show_ca_prompt("Err Not Authored");
			break;
			
		case CA_MG_ERR_CARD_LOCKED: /* ���ܿ����� */
			app_mg_cas312_show_ca_prompt("Error Card Locked");
			break;
			
		case CA_MG_ERR_APP_LOCKED: /* �������� */
			app_mg_cas312_show_ca_prompt("Err App Locked");
			break;
			
		case CA_MG_ERR_PIN_AUTHEN: /* ������Դ��� */
		{
			MG_U8 pinErrCnt = 0;
			char strTemp[50] = {0};
			
			pinErrCnt = MG_Get_PinErrCount();
			if (0x80 == (pinErrCnt & 0x80))
			{
				if (0x20 == (pinErrCnt & 0x20))
				{
					/*���ܿ�����ԣ�������δ���*/
					app_mg_cas312_show_ca_prompt("Err Pin Authen Card Paired");	
				}

				if (0x10 == (pinErrCnt & 0x10))
				{
					/*���ܿ�δ��ԣ������������*/
					app_mg_cas312_show_ca_prompt("Err Pin Authen App Locked");
				}
			}
			else
			{
				sprintf(strTemp, "���ܿ��ٲ���%d�κ󽫱�����", (pinErrCnt & 0x0F));
				app_mg_cas312_show_ca_prompt(strTemp);
			}		
		}	
			break;
			
		case CA_MG_ERR_EXTER_AUTHEN: /* ���ܿ���֤ʧ�� */
			app_mg_cas312_show_ca_prompt("Err Exter Authen");
			break;

		case CA_MG_ERR_DONOT_TELEVIEW_PPV: /* PPV��Ŀ���밴������������ */
			app_mg_cas312_show_ca_prompt("Err Don't Teleview PPV");
			break;
			
		case CA_MG_ERR_CARD_NOTFOUND: /* ��������ܿ� */
			app_mg_cas312_show_ca_prompt("Error Card Not Found");
			break;
			
		case CA_MG_ERR_CARD_NOTMATCH: /* ���ܿ���ƥ�� */
			app_mg_cas312_show_ca_prompt("Err Card Not Match");
			break;
			
		case CA_MG_ERR_UPDATE_GK: /* ���ܿ�����ʧ�� */
			app_mg_cas312_show_ca_prompt("Err Update GK");
			break;

		case CA_MG_EMM_APP_STOP: /*���ܿ���ͣ����*/
			app_mg_cas312_show_ca_prompt("Emm App Stop");
			break;
			
		case CA_MG_EMM_APP_RESUME: /*���ܿ��ָ�����*/
			app_mg_cas312_show_ca_prompt("Emm App Resume");
			break;
			
		case CA_MG_EMM_PROG_UNAUTH: /*ȡ����Ŀ����*/
			app_mg_cas312_show_ca_prompt("Emm Prog UnAuth");
			break;
			
		case CA_MG_EMM_PROG_AUTH: /*���ӽ�Ŀ����*/
			app_mg_cas312_show_ca_prompt("Emm Prog Auth");
			break;
			
		case CA_MG_EMM_EN_PARTNER: /*���û������*/
			app_mg_cas312_show_ca_prompt("Emm En Partner");
			break;
			
		case CA_MG_EMM_DIS_PARTNER: /*ȡ���������*/
			app_mg_cas312_show_ca_prompt("Emm Dis Partner");
			break;
			
		case CA_MG_EMM_SONCARD: /*������ĸ��*/
			app_mg_cas312_show_ca_prompt("Emm SonCard");
			break;
			
		case CA_MG_EMM_UPDATE_GNO: /*���ܿ���Ÿ���*/
			app_mg_cas312_show_ca_prompt("Emm Update Gno");
			break;
			
		case CA_MG_EMM_UPDATE_GK: /*���ܿ���������β忨*/
			app_mg_cas312_show_ca_prompt("Emm312 Update GK");
			break;
			
		case CA_MG_EMM_CHARGE:
			{
                MG_U32 value, tmpHighV, tmpLowV;
				char strTemp[50] = {0};
				
				value = MG_Get_ChargeValue();
				tmpHighV = value / 10000;
				tmpLowV = (value - tmpHighV*10000) / 100;
				sprintf(strTemp, "���ܿ��ɹ���ֵ:%d.%02dԪ", tmpHighV, tmpLowV);
				app_mg_cas312_show_ca_prompt(strTemp);
				printf("***[app_mg_cas312_api_pop_msg_exec](%s)@***\n", strTemp);
			}
			break;
			
		case CA_MG_EMM_URGENT_SERVICE: /*Ӧ���㲥*/
			app_mg_cas312_show_ca_prompt("Emm Urgent Service");
			break;
			
		case CA_MG_EMM_MODIFY_ZONECODE: /*���ܿ����������*/
			app_mg_cas312_show_ca_prompt("Emm Modify ZoneCode");
			break;

		case CA_MG_EMM_UPDATE_COUT: /*���ܿ�����ֵ����*/
			app_mg_cas312_show_ca_prompt("Emm Update Cout");
			break;	
			
		case CA_MG_EMM_UPDATE_CRT: /*��Ȩ����*/
			app_mg_cas312_show_ca_prompt("Emm Update Crt");
			break;
			
		default:
			break;
	}
	
	return 0;
}

int32_t app_mg_cas312_api_osd_exec(void)
{
	app_mg_cas312_rolling_message_exec();//������Ϣ����
	app_mg_cas312_email_flag_exec();// �ʼ�ͼ����ʾ�����ؿ���
	app_mg_cas312_finger_exec();//ָ����Ϣ��ʾ����
    
	return 1;
}


APP_CAS_API_ControlBlock_t MgCas312PortControlBlock =
{
	.m_cas_init_callback = app_mg_cas312_api_init,//caģ���ʼ��
	.m_cas_close_callback = NULL,
	.m_cas_api_card_in_callback = app_mg_cas312_api_card_in,//���ܿ�����
	.m_cas_api_card_out_callback = app_mg_cas312_api_card_out,//���ܿ��Ƴ�
	.m_cas_api_release_ecm_filter_callback = app_mg_cas312_api_release_ecm_filter,
	.m_cas_api_start_ecm_filter_callback = app_mg_cas312_api_start_ecm_filter,//ecm������
	.m_cas_api_release_emm_filter_callback = app_mg_cas312_api_release_emm_filter,
	.m_cas_api_start_emm_filter_callback = app_mg_cas312_api_start_emm_filter,//emm����������
	.m_cas_api_check_cat_ca_descriptor_valid_callback = app_mg_cas312_api_check_cat_ca_descriptor_valid,//ca ģ��cat��������У��
	.m_cas_api_check_pmt_ca_descriptor_valid_callback = app_mg_cas312_api_check_pmt_ca_descriptor_valid,//caģ��pmt��������У��
	.m_cas_api_is_ca_pop_msg_type_callback = app_mg_cas312_api_is_ca_pop_msg_type,//ca��Ϣ�ж�
	.m_cas_api_pop_msg_exec_callback = app_mg_cas312_api_pop_msg_exec,//����ca��Ϣ��
	.m_cas_api_osd_exec_callback = app_mg_cas312_api_osd_exec,//200ms��ʱ������
	.m_cas_api_gxmsg_ca_on_event_exec_callback = app_mg_cas312_gxmsg_ca_on_event_exec, 	//ca�¼�����							

    .m_cas_api_init_data_callback = app_mg_cas312_api_init_data,//��ȡ����ǰ�ĳ�ʼ������
	.m_cas_api_get_data_callback = app_mg_cas312_api_get_data,//��ȡ���ݽӿ�
	.m_cas_api_get_count_callback = app_mg_cas312_api_get_count,//��ȡ��������ֵ
	.m_cas_api_delete_callback = app_mg_cas312_api_delete,//ɾ�������ʼ�����
	.m_cas_api_delete_all_callback = app_mg_cas312_api_delete_all,//ɾ�������ʼ�
	.m_cas_api_buy_ipp_callback = app_mg_cas312_api_buy_ipp,//iptv�ӿ� (�������ܿ�)
	.m_cas_api_get_lock_status_callback = app_mg_cas312_api_get_lock_status, //Ӧ���㲥״̬
	.m_cas_api_feed_mother_child_card_callback = NULL,
	.m_cas_api_change_pin_callback = NULL,
	.m_cas_api_verify_pin_callback = NULL,
	.m_cas_api_change_rating_callback = NULL,
	.m_cas_api_change_worktime_callback = NULL,
};


