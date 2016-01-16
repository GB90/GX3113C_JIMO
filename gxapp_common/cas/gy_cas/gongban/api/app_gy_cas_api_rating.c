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
#include <gxtype.h>
#include "gxmsg.h"
#include "gxapp_sys_config.h"
#include "app_common_play.h"
#include "app_common_table_pmt.h"
#include "app_common_porting_stb_api.h"
#include "app_common_porting_ca_flash.h"
#include "app_common_porting_ca_demux.h"
#include "app_common_porting_ca_descramble.h"
#include "app_common_porting_ca_smart.h"
	
#include "gyca_def.h"
#include "gyca_pub.h"
#include "gyca_osp.h"
#include "gyca_interface.h"
#include "gyca_porting.h"
	
#include "app_gy_cas_api_pin.h"
#include "app_gy_cas_api_smart.h"
#include "app_gy_cas_api_demux.h"
#include "app_gy_cas_api_rating.h"


uint8_t app_gy_cas_api_init_rating_data(void)
{
	return 0;
}

char * app_gy_cas_api_get_rating_data(ca_get_date_t *data)
{
	uint32_t pos = 0 ;                             
	uint32_t ID = 0;
    static char            s_text_buffer[30];

    unsigned char ratevalue;


	if (NULL == data)
		return NULL;

	pos = data->pos;
	ID = data->ID;

	memset(s_text_buffer,0,sizeof(s_text_buffer));
	
	switch(ID)
	{
		case GY_RATING_LEVEL_ID:
			GYCA_GetParentControl(&ratevalue);
			sprintf(s_text_buffer, "%d", ratevalue-1);
			return s_text_buffer;
			break;
		default:
			break;
	}
	return NULL;

}

char* app_gy_cas_api_change_rating(ca_rating_t* data)
{
	GYS32 wRet = 0;
	uint8_t pin[7]={0};


	if (NULL == data)
		return NULL;
	
	memcpy(pin,data->pin,6);

	wRet =  GYCA_SetParentControl((GYU8*)pin,data->rate+1);

	switch( wRet )
	{
		case GYCA_OK:
			return (char*)"Set Maturity Rating Succeed";
			break;
		case GYCA_CARD_ERR:
			return (char*)"Invalid Smartcard";
			break;
		case GYCA_PIN_ERR:
			return (char*)"Invalid Cur PIN";
			break;
		case GYCA_PARENTLEVEL_ERR:
			return (char*)"Invalid Rating Param";
			break;
		default:
			return (char*)"Set Maturity Rating Fail";
	}
	
	return NULL;

}







