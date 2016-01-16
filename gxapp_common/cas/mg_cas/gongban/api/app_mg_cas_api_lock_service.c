/*****************************************************************************
* 						   CONFIDENTIAL
*        Hangzhou GuoXin Science and Technology Co., Ltd.
*                      (C)2014, All right reserved
******************************************************************************

******************************************************************************
* File Name :	app_mg_cas_api_lock_service.c
* Author    : 	zhouhm
* Project   :	goxceed dvbc
* Type      :
******************************************************************************
* Purpose   :	ģ��ͷ�ļ�
******************************************************************************
* Release History:
VERSION	Date			  AUTHOR         Description
1.0  	2014.04.16		  zhouhm 	 			creation
*****************************************************************************/
#include "app_common_play.h"
#include "app_common_prog.h"
#include "app_common_search.h"
#include "app_common_porting_stb_api.h"
#include "app_common_porting_ca_flash.h"
#include "app_common_porting_ca_smart.h"
#include "app_common_porting_ca_demux.h"
#include "app_common_porting_ca_descramble.h"
#include "app_common_lcn.h"
#include "app_mg_cas_api_demux.h"
#include "app_mg_cas_api_lock_service.h"
#include "gxmsg.h"
#include "gxprogram_manage_berkeley.h"
#include "service/gxplayer.h"
#include "gxplayer_url.h"

static uint8_t lock_service_status = FALSE;
extern 	void MGCA_ShowBugMessage(MG_S16 messageType);
extern void app_win_exist_to_full_screen(void);
void app_mg_cas_api_lock_service(uint16_t wServiceID)
{
	int32_t serviceIndex= 0;	// �߼�Ƶ����
	GxBusPmDataProg prog_data = {0};
	GxBusPmViewInfo sysinfo;
	GxBusPmViewInfo old_sysinfo;
	int32_t config;
	uint16_t pos;
//	uint16_t wServiceID = (MG_ServiceID[0]<<8)+MG_ServiceID[1];
	printf(" app_mg_cas_api_lock_service wServiceID=%d \n",
		wServiceID);	


	GxBus_PmViewInfoGet(&sysinfo);
	memcpy(&old_sysinfo,&sysinfo,sizeof(GxBusPmViewInfo));
	serviceIndex = GxBus_PmProgPosGetByServiceId(wServiceID);		// �����߼�Ƶ����
	if (-1 == serviceIndex)
	{
		printf("app_mg_cas_api_lock_service GxBus_PmProgPosGet fail\n");
		return;
	}

	/*ͨ���߼�Ƶ�� �ţ���ȡ��Ŀλ��*/
	config = app_flash_get_config_lcn_flag();
	if(LCN_STATE_ON == config)
	{
		if (FALSE == app_lcn_list_get_num_prog_pos(serviceIndex,&pos))
		{
			printf("app_mg_cas_api_lock_service app_get_prog_pos_by_LCN fail\n");
			return; 					
		}
	}
	else
	{
		app_prog_get_pos_in_group_by_service_id(wServiceID,&pos);
	}	
	MGCA_ShowBugMessage(CA_MG_EMM_URGENT_SERVICE);
	app_win_exist_to_full_screen();

	GxBus_PmProgGetByPos(pos,1,&prog_data); 
	printf("PROG NAME = %s\n", prog_data.prog_name);
	if(GXCORE_SUCCESS == GxBus_PmViewInfoGet(&sysinfo))
	{
		if(sysinfo.stream_type == GXBUS_PM_PROG_TV)
		{
			sysinfo.tv_prog_cur = prog_data.id;
		}
#if (1 == DVB_HD_LIST)
		else if(sysinfo.stream_type == GXBUS_PM_PROG_HD_SERVICE)
		{
			sysinfo.hd_prog_cur = prog_data.id;
		}
#endif
		else
		{
			sysinfo.radio_prog_cur = prog_data.id;
		}
		GxBus_PmViewInfoModify(&sysinfo);
		app_prog_save_playing_pos_in_group(pos);//���ý�Ŀ��
	}

	app_play_switch_prog_clear_msg();	
	app_play_reset_play_timer(0);//��ʱ����̨,׼�����Ž�Ŀ		
	//app_mg_cas_api_set_lock_status(TRUE);
	return;	
}

uint8_t app_mg_cas_api_set_lock_status(uint8_t lock_status)
{
	if (lock_service_status != lock_status)
		lock_service_status = lock_status;
	return lock_service_status;
	
}

//Ӧ���㲥����״̬
uint8_t app_mg_cas_api_get_lock_status(void)
{
	return lock_service_status;
}


