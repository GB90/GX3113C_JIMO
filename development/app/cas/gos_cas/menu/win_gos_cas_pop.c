/*****************************************************************************
* 						   CONFIDENTIAL
*        Hangzhou GuoXin Science and Technology Co., Ltd.
*                      (C)2012, All right reserved
******************************************************************************

******************************************************************************
* File Name :	win_gos_cas_pop.c
* Author    : 	zhouhm
* Project   :	goxceed dvbc
* Type      :
******************************************************************************
* Purpose   :	
******************************************************************************
* Release History:
VERSION	Date			  AUTHOR         Description
1.0  	2014.04.22		  zhouhm 	 			creation
*****************************************************************************/
#include "app_common_play.h"
#include "app_common_porting_stb_api.h"
#include "app_win_interface.h"
#include "app_gos_cas_api_demux.h"

extern void app_show_prompt(char* context);
extern void app_hide_prompt(void); 

extern uint32_t s_duration;
extern uint32_t s_ippt;
extern uint32_t s_ippt_duration;

void app_gos_cas_hide_ca_prompt_message(uint8_t type)
{
#if 0
	switch(type)
		{
			case CA_GOS_SMART_IPPT:				/*IPPT��ʾ*/
				break;
			case CA_GOS_SMART_LAST_DAY: 		/*�����ʾ����*/
				break;
			default:
				break;
		}
#endif

	return;		
}


void app_gos_cas_show_ca_prompt_message(uint8_t type)
{
#if 0
	char messagebuf[100]={0};
	switch(type)
		{
			case CA_GOS_SMART_IPPT:				/*IPPT��ʾ*/
				sprintf(messagebuf,"��ʱ�շ� :%d �� / %d ��",(s_ippt_duration>>8),(s_ippt_duration&0x00ff));
				sprintf(messagebuf,"ʣ����� :%u �� ",(s_ippt>>16));
				sprintf(messagebuf,"��Ч���� :%u �� ",(s_ippt&0xffff));
				break;
			case CA_GOS_SMART_LAST_DAY: 		/*�����ʾ����*/
				sprintf(messagebuf,"%s:%d","�����ʾ���� :",s_duration);
				break;
			default:
				break;
		}

	return;	
#endif
}


void app_gos_cas_show_ca_prompt(char* context)
{
	app_show_prompt(context);
	return;		
}

void app_gos_cas_hide_ca_prompt(void)
{
	app_hide_prompt();
	return;			
}




