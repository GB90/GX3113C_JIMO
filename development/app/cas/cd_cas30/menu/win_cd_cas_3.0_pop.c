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
#include "win_cd_cas_3.0_pop.h"
#include "app_cd_cas_3.0_api_demux.h"
#include "win_cd_cas_3.0_rolling_message.h"
#include "win_cd_cas_3.0_finger.h"
#include "win_cd_cas_3.0_entitle.h"
#include "win_cd_cas_3.0_ippv_buy.h"
#include "app_cd_cas_3.0_api_email.h"
#include "app_cd_cas_3.0_api_detitle.h"
#define SERIAL_START_ADDR (0x30000)

/*
* app_cd_cas30_show_ca_prompt与app_show_prompt实现功能有可能存在差别
* 
*/
extern void app_show_prompt(char* context);
extern void app_hide_prompt(void); 

void app_cd_cas30_show_ca_prompt(char* context)
{
	app_show_prompt(context);
	return;		
}

/*
* app_cd_cas30_hide_ca_prompt与app_hide_prompt实现功能有可能存在差别
* 
*/
void app_cd_cas30_hide_ca_prompt(void)
{
	app_hide_prompt();
	return;			
}



int32_t app_cd_cas30_api_get_uniqueid(uint32_t *pdwUniqueID)
{
	/*
	* serial number read ,SERIAL_START_ADDR & MAX_SEARIAL_LEN    
	*/
	int8_t serialNumber[MAX_SEARIAL_LEN+1]={0};
	uint32_t		m_sequenceNumber = 0;
	app_flash_get_serial_number(SERIAL_START_ADDR,(char *)serialNumber,MAX_SEARIAL_LEN);
	m_sequenceNumber =  (serialNumber[12]-0x30)*10000000;
	m_sequenceNumber +=(serialNumber[13]-0x30)*1000000;
	m_sequenceNumber +=(serialNumber[14]-0x30)*100000;
	m_sequenceNumber +=(serialNumber[15]-0x30)*10000;
	m_sequenceNumber +=(serialNumber[16]-0x30)*1000;
	m_sequenceNumber +=(serialNumber[17]-0x30)*100;
	m_sequenceNumber +=(serialNumber[18]-0x30)*10;
	m_sequenceNumber +=(serialNumber[19]-0x30);
	*pdwUniqueID = m_sequenceNumber;
	return 0;
}




